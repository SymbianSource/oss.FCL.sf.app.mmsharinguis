/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: 
*
*/

// USER
#include "musengclipvideoplayer.h"
#include "musengcamerahandler.h"
#include "musengmceutils.h"
#include "musengdisplayhandler.h"
#include "muslogger.h"

// SYSTEM
#include <mcesession.h>
#include <mcevideostream.h>
#include <mcefilesource.h>
#include <drmcommon.h>

// CONSTANTS
const TInt64 KMicroSecondsInOneSecond = 1000000;
const TInt KFastWindingFactor = 4;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngClipVideoPlayer* CMusEngClipVideoPlayer::NewL( 
    MMusEngDisplayHandler& aDisplayHandler,
    MLcAudioControl& aLcAudioControl )
    {
    return new( ELeave )CMusEngClipVideoPlayer( 
        aDisplayHandler, aLcAudioControl );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngClipVideoPlayer::CMusEngClipVideoPlayer( 
    MMusEngDisplayHandler& aDisplayHandler,
    MLcAudioControl& aLcAudioControl ) :
    CMusEngLocalVideoPlayer( aDisplayHandler, aLcAudioControl )
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngClipVideoPlayer::~CMusEngClipVideoPlayer()
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngClipVideoPlayer::HasClipEnded()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipVideoPlayer::HasClipEnded" )

    TBool hasClipEnded( EFalse );

    if ( iMceSession )
        {    
        CMceVideoStream* videoOut = NULL;
        
        TRAPD( err, 
               videoOut = MusEngMceUtils::GetVideoOutStreamL( *iMceSession ) );
        if ( err != KErrNone ) 
            {
            MUS_LOG1( "mus: [ENGINE]     Error in GetVideoOutStreamL %d", err )
            return EFalse;
            }

        CMceFileSource* filesource = NULL;
        TRAP( err, filesource = MusEngMceUtils::GetFileSourceL( *iMceSession ) )

        if ( err == KErrNone )
            {
            TTimeIntervalMicroSeconds position;
            TTimeIntervalMicroSeconds duration;
            TRAP( err, position = filesource->PositionL() );
            TRAPD( err2, duration = filesource->DurationL() );
            if ( err != KErrNone || err2 != KErrNone )
                {
                return EFalse;
                }
                
            MUS_LOG2( "mus: [ENGINE]    position = %Ld, duration = %Ld", 
                      position.Int64(), duration.Int64() )
                        
            hasClipEnded = 
                ( position.Int64() == 0 && 
                  !filesource->IsEnabled() && 
                  videoOut->State() == CMceMediaStream::EDisabled );
            }
        }

    if ( hasClipEnded )
        {
        iDelayFileEndingPos = 0;
        }
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngClipVideoPlayer::HasClipEnded" )
    
    return hasClipEnded;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngClipVideoPlayer::SetBufferingPeriod( 
    const TTimeIntervalMicroSeconds& aPeriod )
    {
    iBufferingPeriod = aPeriod;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
TBool CMusEngClipVideoPlayer::LcIsPlayingL()
    {
    __ASSERT_ALWAYS( iMceSession, User::Leave( KErrNotReady ) );
    return ( MusEngMceUtils::GetFileSourceL( *iMceSession )->IsEnabled() );
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CMusEngClipVideoPlayer::LcPlayL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngClipVideoPlayer::LcPlayL" )
    
    __ASSERT_ALWAYS( iMceSession, User::Leave( KErrNotReady ) );
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );    
    
    if ( !file->IsEnabled() )
        {
        file->EnableL();   
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngClipVideoPlayer::LcPlayL" )    
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CMusEngClipVideoPlayer::LcPauseL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngClipVideoPlayer::LcPauseL" )
    
    __ASSERT_ALWAYS( iMceSession, User::Leave( KErrNotReady ) );
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );    
    
    if ( file->IsEnabled() )
        {
        file->DisableL();
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngClipVideoPlayer::LcPauseL" )   
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcSourceFileControl* CMusEngClipVideoPlayer::LcSourceFileControl()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcSourceFileControl
// -----------------------------------------------------------------------------
//
void CMusEngClipVideoPlayer::SetLcFileNameL( const TFileName& aFileName )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngClipVideoPlayer::SetLcFileNameL" )
    
    __ASSERT_ALWAYS( !IsProtectedFileL( aFileName ),
                     User::Leave( KErrPermissionDenied ) );    

    if ( iMceSession )
        {
        CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );
        file->UpdateL( aFileName );
        }
    
    iFileName = aFileName;
    MUS_LOG( "mus: [ENGINE]  <- CMusEngClipVideoPlayer::SetLcFileNameL" )    
    }

// -----------------------------------------------------------------------------
// From MLcSourceFileControl
// -----------------------------------------------------------------------------
//
TFileName& CMusEngClipVideoPlayer::LcFileName()
    {
    return iFileName;
    }

// -----------------------------------------------------------------------------
// From MLcSourceFileControl
// Since MCE does not at the moment support SetFastForwardL function, this
// functionality is implemented by taking a timestamp when forwarding is
// started and calculating a new position when it is ended.
// -----------------------------------------------------------------------------
//
void CMusEngClipVideoPlayer::LcFastForwardL( TBool aUseFFWD )
    {
    MUS_LOG1( "mus: [ENGINE] -> CMusEngClipVideoPlayer::FastForward( %d )", 
              aUseFFWD )

    __ASSERT_ALWAYS( iMceSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );

    if ( aUseFFWD )
        {
        // Ignore if we are already fastforwarding
        if ( iFFWDStartTime.Int64() > 0 )
            {
            return;
            }

        // Stop rewinding if ongoing, else just pause file source
        if ( iFRWDStartTime.Int64() > 0 )
            {
            LcFastRewindL( EFalse );
            }
        else
            {
            file->DisableL();
            }
            
        // Get timestamp for starttime
        iFFWDStartTime.HomeTime();
        }
    else
        {
        // Leave if we are not fastforwarding
        if ( iFFWDStartTime.Int64() == 0 )
            {
            User::Leave( KErrAlreadyExists );
            }

        // Set new position
        file->SetPositionL( PositionMicroSecondsL( ETrue ) );
        
        // Reset timer
        iFFWDStartTime = TTime( 0 );
        }

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipVideoPlayer::FastForward" )
    }

// -----------------------------------------------------------------------------
// From MLcSourceFileControl
// Since MCE does not at the moment support SetFastRewindL function, this
// functionality is implemented by taking a timestamp when rewinding is
// started and calculating a new position when it is ended.
// -----------------------------------------------------------------------------
//
void CMusEngClipVideoPlayer::LcFastRewindL( TBool aUseFRWD )
    {
    MUS_LOG1( "mus: [ENGINE] -> CMusEngClipVideoPlayer::FastRewind( %d )", 
              aUseFRWD )

    __ASSERT_ALWAYS( iMceSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );

    if ( aUseFRWD )
        {
        // Ignore if we are already fastrewinding
        if ( iFRWDStartTime.Int64() > 0 )
            {
            return;
            }

        // Stop fastforwarding if ongoing, else just pause file source
        if ( iFFWDStartTime.Int64() > 0 )
            {
            LcFastForwardL( EFalse );
            }
        else
            {
            file->DisableL();
            }
            
        // Get timestamp for starttime
        iFRWDStartTime.HomeTime();
        }
    else
        {
        // Leave if we are not fastrewinding
        if ( iFRWDStartTime.Int64() == 0 )
            {
            User::Leave( KErrAlreadyExists );
            }

        // Set new position
        file->SetPositionL( PositionMicroSecondsL( ETrue ) );
        
        // Reset timer
        iFRWDStartTime = TTime( 0 );
        }

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipVideoPlayer::FastRewind" )
    }

// -----------------------------------------------------------------------------
// From MLcSourceFileControl
// -----------------------------------------------------------------------------
//
TTimeIntervalSeconds CMusEngClipVideoPlayer::LcFileDurationL()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipVideoPlayer::DurationL" )

    __ASSERT_ALWAYS( iMceSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );

    TTimeIntervalMicroSeconds duration = file->DurationL();

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipVideoPlayer::DurationL" )
    
    return TTimeIntervalSeconds( static_cast< TInt >( 
        duration.Int64() / KMicroSecondsInOneSecond ) );
    }

// -----------------------------------------------------------------------------
// From MLcSourceFileControl
// -----------------------------------------------------------------------------
//
TTimeIntervalSeconds CMusEngClipVideoPlayer::LcFilePositionL()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipVideoPlayer::LcFilePositionL" )
    
    TTimeIntervalMicroSeconds currentPosition = PositionMicroSecondsL();
    
    MUS_LOG1( "mus: [ENGINE] <- CMusEngClipVideoPlayer::LcFilePositionL, %d", 
              currentPosition.Int64() )
    
    return TTimeIntervalSeconds( static_cast< TInt >( 
        currentPosition.Int64() / KMicroSecondsInOneSecond ) );
    }

// -----------------------------------------------------------------------------
// From MLcSourceFileControl
// -----------------------------------------------------------------------------
//
void CMusEngClipVideoPlayer::SetLcFilePositionL( 
    const TTimeIntervalSeconds& aPosition )
    {
    MUS_LOG1( "mus: [ENGINE] -> CMusEngClipVideoPlayer::SetPositionL ( %d )",
              aPosition.Int() )

    __ASSERT_ALWAYS( iMceSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );

    TTimeIntervalMicroSeconds position( 
        KMicroSecondsInOneSecond * static_cast< TInt64 >( aPosition.Int() ) );

    if ( position == 0 )
        {
        iRewindedToBeginning = ETrue;
        }
        
    file->SetPositionL( position );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipVideoPlayer::SetPositionL ()" )
    }

// -----------------------------------------------------------------------------
// Check is file DRM protected.
// -----------------------------------------------------------------------------
//
TBool CMusEngClipVideoPlayer::IsProtectedFileL( const TDesC& aClipFile )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipVideoPlayer::IsProtectedFileL(...)" )

    TBool isDRMProtected = EFalse;
    DRMCommon* drmapi = DRMCommon::NewL();
    CleanupStack::PushL( drmapi );

    User::LeaveIfError( drmapi->Connect() );
    //Check DRM file protection
    User::LeaveIfError( drmapi->IsProtectedFile( aClipFile, isDRMProtected ) );
    drmapi->Disconnect();

    CleanupStack::PopAndDestroy( drmapi );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipVideoPlayer::IsProtectedFileL(...)" )
    return isDRMProtected;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMusEngClipVideoPlayer::PositionMicroSecondsL( 
    TBool aActualPosition )
    {
    __ASSERT_ALWAYS( iMceSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );

    TTimeIntervalMicroSeconds position = file->PositionL();
    TTimeIntervalMicroSeconds duration = file->DurationL();
       
    TTimeIntervalMicroSeconds calculatedPosition;
    
    // Adjust position if we are fastforwarding or -rewinding
    if ( iFFWDStartTime.Int64() != 0 )
        {
        TTime now;
        now.HomeTime();
        calculatedPosition = KFastWindingFactor *
                             now.MicroSecondsFrom( iFFWDStartTime ).Int64() +
                             position.Int64();
        if ( calculatedPosition > duration )
            {
            calculatedPosition = duration;
            }
        }
    else if ( iFRWDStartTime.Int64() != 0 )
        {
        TTime now;
        now.HomeTime();
        calculatedPosition = position.Int64() -
                             KFastWindingFactor *
                             now.MicroSecondsFrom( iFRWDStartTime ).Int64();
        if ( calculatedPosition < 0 )
            {
            calculatedPosition = 0;
            }
            
        if ( calculatedPosition == 0 )
            {
            iRewindedToBeginning = ETrue;
            }
        }
    else
        {
        calculatedPosition = position;
        }
    
    if ( !aActualPosition )
        {
        calculatedPosition = 
            GetVideoSinkRelativeFilePos( calculatedPosition, duration );
        }
        
    return calculatedPosition;
    }

// -----------------------------------------------------------------------------
// Modifies file position if position has reached end before clip has ended.
// File position is not going in sync with local video playback as playback
// buffers media before starting playing.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMusEngClipVideoPlayer::GetVideoSinkRelativeFilePos( 
    const TTimeIntervalMicroSeconds& aActualPosition, 
    const TTimeIntervalMicroSeconds& aDuration )
    { 
    MUS_LOG1( "mus: [ENGINE] PositionMicroSecondsL, pos before mod:%d", 
              aActualPosition.Int64() )
    
    TTimeIntervalMicroSeconds tempCalculatedPosition( aActualPosition );
    
    if ( iDelayFileEndingPos != 0 )
        {
        iDelayFileEndingPos = aDuration;
        tempCalculatedPosition = iDelayFileEndingPos;
        }
    else
        {
        // FRWD can go to zero even if clip has not ended, do not modify 
        // time in such situation.
        if ( aActualPosition == 0 && 
             !HasClipEnded() && 
             iFRWDStartTime.Int64() == 0 && 
             !iRewindedToBeginning )
            {
            const TInt KMusDelayEndingModifier = 2;
            iDelayFileEndingPos = aDuration.Int64() - 
                iBufferingPeriod.Int64() / KMusDelayEndingModifier;
            tempCalculatedPosition = iDelayFileEndingPos;
            if ( iPreviousPos > tempCalculatedPosition )
                {
                tempCalculatedPosition = iPreviousPos;
                }
            }
        else
            {
            iDelayFileEndingPos = 0;
            }
        
        if ( iRewindedToBeginning && aActualPosition > 0 )
            {
            iRewindedToBeginning = EFalse;
            }
            
        if ( tempCalculatedPosition < 0 )
            {
            tempCalculatedPosition = 0;
            }
        }
    
    iPreviousPos = tempCalculatedPosition;
            
    return tempCalculatedPosition;
    }

// End of file
