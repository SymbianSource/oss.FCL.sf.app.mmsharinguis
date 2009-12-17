/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
#include "musengclipsession.h"
#include "musengclipsessionobserver.h"
#include "musengsessionobserver.h"
#include "musengmceoutsession.h"
#include "musenglivesession.h"
#include "musengmceutils.h"
#include "musenglogger.h"

// SYSTEM
#include <mcemanager.h>
#include <mcesession.h>
#include <mcestreambundle.h>
#include <mcefilesource.h>
#include <mcemediastream.h>
#include <mcevideostream.h>
#include <mceaudiostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <mceamrcodec.h>
#include <mcevideocodec.h>
#include <DRMCommon.h>
#include <f32file.h>


// CONSTANTS

const TInt64 KMicroSecondsInOneSecond = 1000000;
const TInt KMusEngAmrBitRate = KMceAmrNbBitrate475;
const TUint KMusEngAllowedAmrBitrates = KMceAllowedAmrNbBitrate475;

const TInt KFastWindingFactor = 4;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusEngClipSession* CMusEngClipSession::NewL(
                        const TRect& aRect,
                        MMusEngSessionObserver& aSessionObserver,
                        MMusEngOutSessionObserver& aOutSessionObserver,
                        MMusEngClipSessionObserver& aClipSessionObserver,
                        TUint aSipProfileId )
    {
    CMusEngClipSession* self = new( ELeave ) CMusEngClipSession(
                                                    aSessionObserver,
                                                    aOutSessionObserver,
                                                    aClipSessionObserver,
                                                    aRect );
    CleanupStack::PushL( self );
    self->ConstructL( aSipProfileId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngClipSession::~CMusEngClipSession()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::~CMusEngClipSession()" )
    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::~CMusEngClipSession()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::SetClipL( const TDesC& aFileName )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::SetClipL(...)" )

    __ASSERT_ALWAYS( !IsProtectedFileL( aFileName ),
                     User::Leave( KErrPermissionDenied ) );
    
    if ( iSession )
        {
        CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iSession );
        file->UpdateL( aFileName );
        }

    iFileName = aFileName;

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::SetClipL(...)" )

    }


// -----------------------------------------------------------------------------
// Since MCE does not at the moment support SetFastForwardL function, this
// functionality is implemented by taking a timestamp when forwarding is
// started and calculating a new position when it is ended.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::FastForwardL( TBool aUseFFWD )
    {
    MUS_LOG1( "mus: [ENGINE] -> CMusEngClipSession::FastForward(%d)", aUseFFWD )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iSession );

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
            FastRewindL( EFalse );
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
        MUS_LOG( "                 SetPositionL returned without error " )
        
        // Reset timer
        iFFWDStartTime = TTime( 0 );
        }

    MUS_LOG1( "mus: [ENGINE] <- CMusEngClipSession::FastForward(%d)", aUseFFWD )
    }


// -----------------------------------------------------------------------------
// Since MCE does not at the moment support SetFastRewindL function, this
// functionality is implemented by taking a timestamp when rewinding is
// started and calculating a new position when it is ended.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::FastRewindL( TBool aUseFRWD )
    {
    MUS_LOG1( "mus: [ENGINE] -> CMusEngClipSession::FastRewind(%d)", aUseFRWD )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iSession );

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
            FastForwardL( EFalse );
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
        MUS_LOG( "                 SetPositionL returned without error " )
        
        // Reset timer
        iFRWDStartTime = TTime( 0 );
        }

    MUS_LOG1( "mus: [ENGINE] <- CMusEngClipSession::FastRewind(%d)", aUseFRWD  )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalSeconds CMusEngClipSession::PositionL()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::PositionL()" )
    
    TTimeIntervalMicroSeconds currentPosition = PositionMicroSecondsL();
    
    MUS_LOG1( "mus: [ENGINE] <- CMusEngClipSession::PositionL(), pos:%d", 
              currentPosition.Int64() )
    return TTimeIntervalSeconds( static_cast<TInt>( 
                    currentPosition.Int64() / KMicroSecondsInOneSecond ) );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalSeconds CMusEngClipSession::DurationL()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::DurationL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iSession );

    TTimeIntervalMicroSeconds duration = file->DurationL();

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::DurationL()" )
    return TTimeIntervalSeconds(
            static_cast<TInt>( duration.Int64() / KMicroSecondsInOneSecond ) );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::SetPositionL (
                        const TTimeIntervalSeconds& aPosition )
    {
    MUS_LOG1( "mus: [ENGINE] -> CMusEngClipSession::SetPositionL (%d)",
              aPosition.Int() )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iSession );

    TTimeIntervalMicroSeconds position( 
            KMicroSecondsInOneSecond * static_cast<TInt64>(aPosition.Int()) );

    if ( position == 0 )
        {
        iRewindedToBeginning = ETrue;
        }
        
    file->SetPositionL( position );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::SetPositionL ()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::TranscodeL( const TFileName& aFileName )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::TranscodeL(...)" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iSession );

    const RPointerArray<CMceMediaStream>& streams = iSession->Streams();

    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( streams[i]->State() == CMceMediaStream::ETranscodingRequired )
            {
            if ( streams[i]->Type() == KMceAudio )
                {
                AddAmrCodecL( static_cast<CMceAudioStream&>( *streams[i] ) );
                }
            else
                {
                AddVideoCodecL( static_cast<CMceVideoStream&>( *streams[i] ) );
                }
            }
        }

    file->TranscodeL( aFileName );

    iTranscodingOngoing = ETrue;
    
    iTranscodingDestFileName = aFileName;

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::TranscodeL(...)" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::CancelTranscodeL()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::CancelTranscodeL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iSession );

    TRAPD( err, file->CancelTranscodeL() );
    err = err; // Silence warning in UREL build
    MUS_LOG1( "mus: [ENGINE] - cancel result %d", err )
    // Even if cancel fails, try to delete the partial clip

    MUS_LOG( "mus: [ENGINE] - delete the partially converted clip" )
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    CFileMan* fileMan = CFileMan::NewL( fs );    
    CleanupStack::PushL( fileMan );

    MUS_LOG_TDESC8( "mus: [ENGINE] - trascoding destination filename",
                    iTranscodingDestFileName )
    err = fileMan->Delete( iTranscodingDestFileName );
    MUS_LOG1( "mus: [ENGINE] - file delete result %d", err )

    CleanupStack::PopAndDestroy( fileMan );
    CleanupStack::PopAndDestroy(); // fs

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::CancelTranscodeL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::PlayL()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::PlayL()" )

    __ASSERT_ALWAYS( iSession &&
                     iFFWDStartTime.Int64() == 0 &&
                     iFRWDStartTime.Int64() == 0, 
                     User::Leave( KErrNotReady ) );                     

    
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iSession );

    if ( !file->IsEnabled() )
        {
        file->EnableL();
        
        iClipEnded = EFalse;
        // No need to enable audio separarely
        }
    else
        {
        MUS_LOG( "mus: [ENGINE]    File is already enabled, ignore request" )
        }
        
    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::PlayL()" )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngClipSession::PauseL()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::PauseL()" )

    __ASSERT_ALWAYS( iSession &&
                     iFFWDStartTime.Int64() == 0 &&
                     iFRWDStartTime.Int64() == 0, 
                     User::Leave( KErrNotReady ) );  

    
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iSession );
    
    if ( file->IsEnabled() )
        {
        file->DisableL();

        // No need to disable audio separarely
        }
    else
        {
        MUS_LOG( "mus: [ENGINE]    File is already disabled, ignore request" )
        }
        
    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::PauseL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngClipSession::IsPlayingL()
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    return ( MusEngMceUtils::GetFileSourceL( *iSession )->IsEnabled() );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::CompleteSessionStructureL(
                                            CMceStreamBundle& aLocalBundle )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::CompleteSessionStructureL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( iFileName != KNullDesC(), User::Leave( KErrNotReady ) );

    // Create outgoing video stream
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    
    CMceRtpSink* rtpsink = CMceRtpSink::NewLC();
    videoStream->AddSinkL( rtpsink );
    CleanupStack::Pop( rtpsink );

    CMceFileSource* fileSource = CMceFileSource::NewLC( *iManager,
                                                        iFileName );
    fileSource->DisableL(); // Start session in pause mode.
    videoStream->SetSourceL( fileSource );
    CleanupStack::Pop( fileSource );

    iSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );

    ConstructAudioStructureL( aLocalBundle );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::CompleteSessionStructureL()" )
    }


// -----------------------------------------------------------------------------
// Checks that that there is no need for transcoding before calling
// overridden base class variant of this function.
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::EstablishSessionL()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::EstablishSessionL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    const RPointerArray<CMceMediaStream>& streams = iSession->Streams();

    TBool transcodingRequired = EFalse;
    
    if ( iVideoCodecList )
        {
        MUS_LOG_TDESC8( "iVideoCodecList: ", iVideoCodecList->Des() )
        }
    
    CMceVideoStream* videoStream = NULL;
    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( streams[i]->State() == CMceMediaStream::ETranscodingRequired )
            {
            transcodingRequired = ETrue;
            }
        else if ( streams[i]->Type() == KMceVideo &&
                  !IsH264Supported() )
            {
            MUS_LOG( "                -> video stream found!!!" )
            videoStream = static_cast<CMceVideoStream*>( streams[i] );
            
            //transcoding of H264 is not needed only if we know explicitly
            //that the peer supports it (from OPTIONS response)
                            
            const RPointerArray<CMceVideoCodec>& codecs = videoStream->Codecs();
            for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
                {
                if ( codecs[codecIndex]->SdpName() == KMceSDPNameH264() )                     
                    {
                    transcodingRequired = ETrue;
                    MUS_LOG( " -> Removing H264 codec from video stream" )
                    videoStream->RemoveCodecL( *codecs[codecIndex] );
                    codecIndex = 0;
                    }
                }
            
            if ( codecs.Count() == 0)
                {
                // At least one codec should be in the stream list. 
                MUS_LOG( " -> Adding codec, since codecs list is empty " )
                AddVideoCodecL( *videoStream );  
                }                
            } 
        }

    if ( transcodingRequired )
        {
        iClipSessionObserver.TranscodingNeeded();
        }
    else
        {                
        CMusEngMceOutSession::EstablishSessionL();
        // Now session state is right to adjust volume
        SetSpeakerVolumeL( VolumeL() );
        }

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::EstablishSessionL()" )
    }

// -----------------------------------------------------------------------------
// Implemented for MMceStreamObserver
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::StreamStateChanged( CMceMediaStream& aStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::StreamStateChanged()" )
    
    if ( !iSession )
        {
        return;
        }
        
    DetermineBufferingPeriod( aStream );
    
    CMusEngMceSession::StreamStateChanged( aStream );
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::StreamStateChanged()" )
    }

// -----------------------------------------------------------------------------
// Implemented for MMceStreamObserver
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::StreamStateChanged( CMceMediaStream& aStream,
                                             CMceMediaSource& aSource )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::StreamStateChanged( src )" )
    
    if ( !iSession )
        {
        return;
        }

    MUS_ENG_LOG_STREAM_STATE( aStream )
    
    DetermineBufferingPeriod( aStream );

    if ( aStream.State() == CMceMediaStream::ETranscoding )
        {
        CMceFileSource* file = static_cast<CMceFileSource*>(aStream.Source());
        TInt progressPercentage = 0;
        TRAP_IGNORE( progressPercentage = file->TranscodingProgressL() )
        iClipSessionObserver.TranscodingProgressed( progressPercentage );
        }
    else if ( aStream.State() == CMceMediaStream::EInitialized )
        {
        if ( iTranscodingOngoing )
            {
            MUS_LOG( "mus: [ENGINE]     Transcoding completed." )
            
            // Filename has been updated in MCE side but unfortunately
            // there's no getter for the filename in API.
            iFileName = iTranscodingDestFileName;
        
            iTranscodingOngoing = EFalse;
            
            iClipSessionObserver.TranscodingCompletedInit();  
                
            TRAPD( error, EstablishSessionL() )
            if ( error != KErrNone )
                {
                iSessionObserver.SessionFailed();
                }
                              
            // Next call does not return before session establishment
            iClipSessionObserver.TranscodingCompletedFinalize();                           
            }
        }
    else if ( aStream.State() == CMceMediaStream::ETranscodingRequired &&
              iTranscodingOngoing )
        {
        MUS_LOG( "mus: [ENGINE]     Transcoding failed." )
        
        iClipSessionObserver.TranscodingFailed();
        iTranscodingOngoing = EFalse;
        }
    else if ( HasClipEnded() )
        {
        MUS_LOG( "mus: [ENGINE]     Clip ended." )
        
        iDelayFileEndingPos = 0;
        iClipEnded = ETrue;
        
        iClipSessionObserver.EndOfClip();
        }
    else
        {
        // Cannot handle, forward to the ancestor class
        CMusEngMceSession::StreamStateChanged( aStream, aSource );
        }

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::StreamStateChanged( src )" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngClipSession::CMusEngClipSession(
                        MMusEngSessionObserver& aSessionObserver,
                        MMusEngOutSessionObserver& aOutSessionObserver,
                        MMusEngClipSessionObserver& aClipSessionObserver,
                        const TRect& aRect )
    : CMusEngMceOutSession( aRect,
                            aSessionObserver,
                            aOutSessionObserver ),
      iClipSessionObserver( aClipSessionObserver )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::ConstructL( TUint aSipProfileId )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::ConstructL(...)" )

    CMusEngMceOutSession::ConstructL( aSipProfileId );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::ConstructL(...)" )
    }


// -----------------------------------------------------------------------------
// Check is file DRM protected.
//
// -----------------------------------------------------------------------------
//
TBool CMusEngClipSession::IsProtectedFileL( const TDesC& aClipFile )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::IsProtectedFileL(...)" )

    TBool isDRMProtected = EFalse;
    DRMCommon* drmapi = DRMCommon::NewL();
    CleanupStack::PushL( drmapi );

    User::LeaveIfError( drmapi->Connect() );
    //Check DRM file protection
    User::LeaveIfError( drmapi->IsProtectedFile( aClipFile, isDRMProtected ) );
    drmapi->Disconnect();

    CleanupStack::PopAndDestroy( drmapi );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::IsProtectedFileL(...)" )
    return isDRMProtected;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::AddAmrCodecL( CMceAudioStream& aAudioStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::AddAmrCodecL" )

    // Remove old codecs

    while ( aAudioStream.Codecs().Count() > 0 )
        {
        aAudioStream.RemoveCodecL( *aAudioStream.Codecs()[0] );
        }

    // Create AMR codec instance

    const RPointerArray<const CMceAudioCodec>& supportedCodecs =
                                            iManager->SupportedAudioCodecs();

    CMceAudioCodec* amr = NULL;

    for ( TInt i = 0; i < supportedCodecs.Count(); ++i )
        {
        if ( supportedCodecs[i]->SdpName() == KMceSDPNameAMR() )
            {
            amr = supportedCodecs[i]->CloneL();
            CleanupStack::PushL( amr );

            User::LeaveIfError(
                    amr->SetAllowedBitrates( KMusEngAllowedAmrBitrates ) );
            User::LeaveIfError( amr->SetBitrate( KMusEngAmrBitRate ) );
            aAudioStream.AddCodecL( amr );

            CleanupStack::Pop( amr );
            break; // We must have only one codec
            }
        }

    __ASSERT_ALWAYS( amr, User::Leave( KErrNotFound ) );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::AddAmrCodecL" )
    }


// -----------------------------------------------------------------------------
// Create codec instance, H264 is used only if other end supports it for sure,
// otherwise H263 is used.
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::AddVideoCodecL( CMceVideoStream& aVideoStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::AddVideoCodecL" )

    // Remove old codecs

    while ( aVideoStream.Codecs().Count() > 0 )
        {
        aVideoStream.RemoveCodecL( *aVideoStream.Codecs()[0] );
        }

    const RPointerArray<const CMceVideoCodec>& supportedCodecs =
                                            iManager->SupportedVideoCodecs();

    CMceVideoCodec* addedCodec = NULL;
    
    TPtrC8 addedCodecName = 
            IsH264Supported() ? KMceSDPNameH264() : KMceSDPNameH2632000();
    
    MUS_LOG_TDESC8( "mus: [ENGINE] adding codec : ", addedCodecName ); 
            
    for ( TInt i = 0; i < supportedCodecs.Count(); ++i )
        {
        if ( supportedCodecs[i]->SdpName() == addedCodecName )
            {
            addedCodec = supportedCodecs[i]->CloneL();
            CleanupStack::PushL( addedCodec );
            aVideoStream.AddCodecL( addedCodec );
            CleanupStack::Pop( addedCodec );
            break; // We must have only one codec
            }
        }

    __ASSERT_ALWAYS( addedCodec, User::Leave( KErrNotFound ) );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::AddVideoCodecL" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngClipSession::HasClipEnded()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::HasClipEnded()" )

    TBool hasClipEnded = EFalse;

    if ( iSession )
        {
        
        CMceVideoStream* videoOut = NULL;
        
        TRAPD( error, 
               videoOut = MusEngMceUtils::GetVideoOutStreamL( *iSession ) );
        if( error != KErrNone ) 
            {
            MUS_LOG1( "mus: [ENGINE]     Error in GetVideoOutStreamL #%d", error )
            return hasClipEnded;
            }

        CMceFileSource* filesource = NULL;
        TRAP( error, filesource = MusEngMceUtils::GetFileSourceL( *iSession ) )

        if ( error == KErrNone )
            {
            TTimeIntervalMicroSeconds position;
            TTimeIntervalMicroSeconds duration;
            TRAP( error, position = filesource->PositionL() );
            TRAPD( error1, duration = filesource->DurationL() );
            if ( error != KErrNone || error1 != KErrNone )
                {
                return hasClipEnded;
                }
                
            MUS_LOG2( "mus: [ENGINE]    position = %Ld, duration = %Ld", 
                        position.Int64(), 
                        duration.Int64() )
                        
            TRAP( error, hasClipEnded = 
                        ( position.Int64() == 0 && 
                          !filesource->IsEnabled() && 
                          videoOut->State() == CMceMediaStream::EDisabled ) )
            if(  hasClipEnded )
                {
                MUS_LOG( "mus: [ENGINE]     End of clip" )
                }
            }
        }

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::HasClipEnded()" )
    return hasClipEnded;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMusEngClipSession::PositionMicroSecondsL( 
    TBool aActualPosition )
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iSession );

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
// If member file contains audio, add appropriate amount of audio streams to
// session structure
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::ConstructAudioStructureL(
                                            CMceStreamBundle& aLocalBundle )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::ConstructAudioStructureL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    // There is no clip audio present in operator variant
    if ( iOperatorVariant )
        {
        MUS_LOG( "                 Operator variant, no audio constructed" )
        MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::ConstructAudioStructureL()" )
        return;
        }

    CMceFileSource* fileSource = MusEngMceUtils::GetFileSourceL( *iSession );

    TInt audioElementCount = fileSource->MediaElementCountL( KMceAudio );

    MUS_LOG1( "mus: [ENGINE]    File contains %d audio elements",
              audioElementCount )

    for ( TInt i = 0; i < audioElementCount; ++i )
        {
        fileSource->SetCurrentMediaElementL( KMceAudio, i );

        MUS_LOG1( "mus: [ENGINE]     Current audio element set to : %d ", i )

        // Set up an audio outstream.
        CMceAudioStream* audioOut = CMceAudioStream::NewLC();

        CMceRtpSink* rtpSink = CMceRtpSink::NewLC();  
        audioOut->AddSinkL( rtpSink );
        CleanupStack::Pop( rtpSink );

        audioOut->SetSourceL( fileSource );

        iSession->AddStreamL( audioOut );
        CleanupStack::Pop( audioOut );

        MUS_LOG( "mus: [ENGINE]     Audio outstream created" )

        // Set up an audio stream to local speaker
        audioOut = CMceAudioStream::NewLC();
        
        MusEngMceUtils::AddSpeakerL( *audioOut );

        audioOut->SetSourceL( fileSource );

        iSession->AddStreamL( audioOut );
        CleanupStack::Pop( audioOut );

        MUS_LOG( "mus: [ENGINE]     Local audio stream created" )

        aLocalBundle.AddStreamL( *audioOut );

        MUS_LOG( "mus: [ENGINE]     Local audio stream added to bundle" )
        }

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::ConstructAudioStructureL()" )
    }

// -----------------------------------------------------------------------------
// Calculates how long MCE buffers based on time between buffering and streaming
// events. 
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::DetermineBufferingPeriod( CMceMediaStream& aStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::DetermineBufferingPeriod()" )
    
    // Determine file position modifier from time difference between buffering
    // and streaming events
    if ( aStream.State() == CMceMediaStream::EBuffering )
        {
        iBufferingStartedTime.HomeTime();
        }
    
    if ( aStream.State() == CMceMediaStream::EStreaming )
        {
        if ( iBufferingStartedTime.Int64() != 0 )
            {
            TTime currentTime;
            currentTime.HomeTime();
            
            TTimeIntervalMicroSeconds bufferingPeriod = 
                currentTime.MicroSecondsFrom( iBufferingStartedTime );
            
            const TInt KMusMinimumBufferingPeriod( 500000 );
            if ( bufferingPeriod > KMusMinimumBufferingPeriod )
                {
                iBufferingPeriod = bufferingPeriod;
                }
                
            MUS_LOG1( "mus: [ENGINE] current buffering period:%d", 
                      iBufferingPeriod.Int64() )
            
            iBufferingStartedTime = 0;
            }
        }
        
    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::DetermineBufferingPeriod()" )
    }

// -----------------------------------------------------------------------------
// Modifies file position if position has reached end before clip has ended.
// File position is not going in sync with local video playback as playback
// buffers media before starting playing.
// -----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds CMusEngClipSession::GetVideoSinkRelativeFilePos( 
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
             !iClipEnded && 
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngClipSession::IsH264Supported() const
    {
    return ( iVideoCodecList && iVideoCodecList->FindF( KMceSDPNameH264() ) >= 0 );
    }
    
// End of file

