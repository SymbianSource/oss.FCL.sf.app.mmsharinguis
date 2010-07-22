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


//  INTERNAL INCLUDES
#include "ut_musengclipvideoplayer.h"
#include "musengtestdefs.h"
#include "musengmceutils.h"
#include "musengclipvideoplayer.h"
#include "mceoutsession.h"
#include "mcevideostream.h"
#include "mcertpsink.h"
#include "mcefilesource.h"
#include "drmcommon.h"

//  SYSTEM INCLUDES
#include <eunitmacros.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngClipVideoPlayer* UT_CMusEngClipVideoPlayer::NewL()
    {
    UT_CMusEngClipVideoPlayer* self = UT_CMusEngClipVideoPlayer::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngClipVideoPlayer* UT_CMusEngClipVideoPlayer::NewLC()
    {
    UT_CMusEngClipVideoPlayer* self = new( ELeave ) UT_CMusEngClipVideoPlayer();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngClipVideoPlayer::~UT_CMusEngClipVideoPlayer()
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngClipVideoPlayer::UT_CMusEngClipVideoPlayer()
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }   

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::SetupL()
    {
    iMceSession = CMceOutSession::NewL();
    CMceVideoStream* videoStream = CMceVideoStream::NewLC(); 
    CMceRtpSink* rtpsink = CMceRtpSink::NewLC();
    videoStream->AddSinkL( rtpsink );
    CleanupStack::Pop( rtpsink );
    CMceFileSource* fileSource = 
        CMceFileSource::NewLC( *iMceSession->iManager, KTestVideoFileName() );
    fileSource->DisableL();
    videoStream->SetSourceL( fileSource );
    CleanupStack::Pop( fileSource );
    iMceSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );    
    iClipVideoPlayer = 
        CMusEngClipVideoPlayer::NewL(  
            iDisplayHandlerStub,
            iLcAudioControlStub );
    iClipVideoPlayer->SetMceSession( iMceSession );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::Teardown()
    {
    delete iClipVideoPlayer;
    delete iMceSession;
    iLcAudioControlStub.Reset();
    }


// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_HasClipEndedL()
    {
    CMceSession* invalidMceSession = CMceOutSession::NewL();
    CleanupStack::PushL( invalidMceSession );    
    
    // Try before establishing the session
    iClipVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT( !iClipVideoPlayer->HasClipEnded() )
    
    // Try with session, but without video out stream
    iClipVideoPlayer->SetMceSession( invalidMceSession );
    EUNIT_ASSERT( !iClipVideoPlayer->HasClipEnded() )
    
    // Try with video out stream without source...
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    videoStream->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    invalidMceSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );
    EUNIT_ASSERT( !iClipVideoPlayer->HasClipEnded() )
    
    // And with enabled source and stream 
    CMceFileSource* fileSource = 
        CMceFileSource::NewLC( 
            *invalidMceSession->iManager, KTestVideoFileName() );
    videoStream->SetSourceL( fileSource );
    CleanupStack::Pop( fileSource );   
    EUNIT_ASSERT( !iClipVideoPlayer->HasClipEnded() )
    
    // try with different position and duration
    fileSource->iPosition = 90;
    fileSource->iDuration = 111;
    EUNIT_ASSERT( !iClipVideoPlayer->HasClipEnded() )
    
    // Disabled source
    fileSource->DisableL();
    EUNIT_ASSERT( !iClipVideoPlayer->HasClipEnded() )

    // Disabled stream
    videoStream->iState = CMceMediaStream::EDisabled;
    EUNIT_ASSERT( !iClipVideoPlayer->HasClipEnded() )
       
    // and finaly try with "real" end of clip 
    fileSource->iPosition = 0;
    fileSource->iDuration = 111;
    
    EUNIT_ASSERT( iClipVideoPlayer->HasClipEnded() )
    
    CleanupStack::PopAndDestroy( invalidMceSession );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcVideoPlayerStateL()
    {
    // iMceSession not set
    iClipVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iClipVideoPlayer->LcVideoPlayerState() ) )    
    iClipVideoPlayer->SetMceSession( iMceSession );
    
    // Establish Session and simulate media playing.
    iMceSession->iState = CMceSession::EEstablished;
    for ( TInt i = 0; i < iMceSession->Streams().Count(); i++ )
        {
        iMceSession->Streams()[i]->iState = CMceMediaStream::EStreaming;
        }
    
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPlaying ), 
                         TInt( iClipVideoPlayer->LcVideoPlayerState() ) )
                                                
    // Media streams unavailable
    iMceSession->Streams()[0]->iState = CMceMediaStream::EUninitialized;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iClipVideoPlayer->LcVideoPlayerState() ) )

    // All other stream states
    iMceSession->Streams()[0]->iState = CMceMediaStream::EInitialized;	
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EInit ), 
                         TInt( iClipVideoPlayer->LcVideoPlayerState() ) )	
	
    iMceSession->Streams()[0]->iState = CMceMediaStream::EBuffering;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EBuffering ), 
                         TInt( iClipVideoPlayer->LcVideoPlayerState() ) )
		
    iMceSession->Streams()[0]->iState = CMceMediaStream::EIdle;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPaused ), 
                         TInt( iClipVideoPlayer->LcVideoPlayerState() ) )	
	
    iMceSession->Streams()[0]->iState = CMceMediaStream::EDisabled;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPaused ), 
                         TInt( iClipVideoPlayer->LcVideoPlayerState() ) )
	
    iMceSession->Streams()[0]->iState = CMceMediaStream::EStreaming;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPlaying ), 
                         TInt( iClipVideoPlayer->LcVideoPlayerState() ) )

    iMceSession->Streams()[0]->iState = CMceMediaStream::ENoResources;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iClipVideoPlayer->LcVideoPlayerState() ) )
	
    iMceSession->Streams()[0]->iState = CMceMediaStream::ETranscodingRequired;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iClipVideoPlayer->LcVideoPlayerState() ) )
	
    iMceSession->Streams()[0]->iState = CMceMediaStream::ETranscoding;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iClipVideoPlayer->LcVideoPlayerState() ) )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcPlayL()
    {
    // MCE session not set
    iClipVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipVideoPlayer->LcPlayL(), KErrNotReady )

    // File source does not exist
    iClipVideoPlayer->SetMceSession( iMceSession );
    iMceSession->Streams()[ 0 ]->SetSourceL( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipVideoPlayer->LcPlayL(), KErrNotFound )
    
    // Play
    CMceFileSource* fileSource = 
        CMceFileSource::NewLC( *iMceSession->iManager, KTestVideoFileName() );
    fileSource->DisableL();
    iMceSession->Streams()[ 0 ]->SetSourceL( fileSource );
    CleanupStack::Pop( fileSource );   
    EUNIT_ASSERT( !iClipVideoPlayer->LcIsPlayingL() )
    iClipVideoPlayer->LcPlayL();    
    EUNIT_ASSERT( iClipVideoPlayer->LcIsPlayingL() )
    
    // Try to play again
    iClipVideoPlayer->LcPlayL();
    EUNIT_ASSERT( iClipVideoPlayer->LcIsPlayingL() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcPauseL()
    {
    // MCE session not set
    iClipVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipVideoPlayer->LcPauseL(), KErrNotReady )

    // File source does not exist
    iClipVideoPlayer->SetMceSession( iMceSession );
    iMceSession->Streams()[ 0 ]->SetSourceL( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipVideoPlayer->LcPauseL(), KErrNotFound )     
    
    // Pause 
    CMceFileSource* fileSource = 
        CMceFileSource::NewLC( *iMceSession->iManager, KTestVideoFileName() );
    fileSource->EnableL();
    iMceSession->Streams()[ 0 ]->SetSourceL( fileSource );
    CleanupStack::Pop( fileSource );      
    EUNIT_ASSERT( iClipVideoPlayer->LcIsPlayingL() )
    iClipVideoPlayer->LcPauseL();
    EUNIT_ASSERT( !iClipVideoPlayer->LcIsPlayingL() )

    // Try to pause already paused clip
    iClipVideoPlayer->LcPauseL();
    EUNIT_ASSERT( !iClipVideoPlayer->LcIsPlayingL() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcIsPlayingL()
    {
    // MCE session not set
    iClipVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipVideoPlayer->LcIsPlayingL(), KErrNotReady )

    // Player is not playing when file source is disabled
    iClipVideoPlayer->SetMceSession( iMceSession );
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );    
    file->DisableL();
    EUNIT_ASSERT( !iClipVideoPlayer->LcIsPlayingL() )

    // Player is playing when file source is enabled
    file->EnableL();
    EUNIT_ASSERT( iClipVideoPlayer->LcIsPlayingL() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_SetLcFileNameL()
    {    
    // Set file before iMceSession is set
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );
    iClipVideoPlayer->SetMceSession( NULL );
    iClipVideoPlayer->SetLcFileNameL( KTestAvcVideoFileName() );
    EUNIT_ASSERT_EQUALS( iClipVideoPlayer->LcFileName(), KTestAvcVideoFileName() )
    EUNIT_ASSERT_EQUALS( file->iFileName, KTestVideoFileName() )

    // Set file after session is set
    iClipVideoPlayer->SetMceSession( iMceSession );
    file->iFileName = KTestAvcVideoFileName();
    iClipVideoPlayer->SetLcFileNameL( KTestVideoFileName() );
    EUNIT_ASSERT_EQUALS( iClipVideoPlayer->LcFileName(), KTestVideoFileName() )
    EUNIT_ASSERT_EQUALS( file->iFileName, KTestVideoFileName() )

    // Set DRM protected file, file names not changed
    MUSENG_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iClipVideoPlayer->SetLcFileNameL( KMusDrmProtectedFileName() ),
        KErrPermissionDenied )
    EUNIT_ASSERT_EQUALS( iClipVideoPlayer->LcFileName(), KTestVideoFileName() )
    EUNIT_ASSERT_EQUALS( file->iFileName, KTestVideoFileName() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcFastForwardL()
    {
    // Try before establishment
    iClipVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iClipVideoPlayer->LcFastForwardL( ETrue ), KErrNotReady )
    
    // Establish session, simulate position and duration and try again 
    iClipVideoPlayer->SetMceSession( iMceSession );
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );                  
    file->iDuration = KMusEngTestFileDuration;
    file->iPosition = KMusEngTestFilePosition; 
    iClipVideoPlayer->LcFastForwardL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipVideoPlayer->iFFWDStartTime.Int64() > 0 )
    EUNIT_ASSERT( iClipVideoPlayer->iFRWDStartTime.Int64() == 0 )
    
    // Simulate fastforwarding for a while
    User::After( 1000 );

    // Try to fastforward when already fastforwarding, will be ignored
    iClipVideoPlayer->LcFastForwardL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipVideoPlayer->iFFWDStartTime.Int64() > 0 )
    EUNIT_ASSERT( iClipVideoPlayer->iFRWDStartTime.Int64() == 0 )
    
    // Stop fastforwarding
    iClipVideoPlayer->LcFastForwardL( EFalse );
    EUNIT_ASSERT( file->iPosition > KMusEngTestFilePosition )
    EUNIT_ASSERT( file->iPosition != file->iDuration )
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipVideoPlayer->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipVideoPlayer->iFRWDStartTime.Int64() == 0 )
    
    // Try to stop fastforwarding again, leaves 
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iClipVideoPlayer->LcFastForwardL( EFalse ), KErrAlreadyExists )
    
    // Start fastrewinding
    iClipVideoPlayer->LcFastRewindL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipVideoPlayer->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipVideoPlayer->iFRWDStartTime.Int64() > 0 )
    
    // Start fastforwarding, rewinding should be stopped and FFWD started
    iClipVideoPlayer->LcFastForwardL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipVideoPlayer->iFFWDStartTime.Int64() > 0 )
    EUNIT_ASSERT( iClipVideoPlayer->iFRWDStartTime.Int64() == 0 )
    
    // Simulate fastforwarding for a while
    User::After( 1000 );
    
    // Simulate that clip is almost in end and fastforwarding would have 
    // continued over end, position should be set to duration.
    file->iPosition = TTimeIntervalMicroSeconds( file->iDuration.Int64() - 1 );
    iClipVideoPlayer->LcFastForwardL( EFalse );
    EUNIT_ASSERT( file->iPosition == file->iDuration )
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT_EQUALS( 0, iClipVideoPlayer->iFFWDStartTime.Int64() )
    EUNIT_ASSERT_EQUALS( 0, iClipVideoPlayer->iFRWDStartTime.Int64() )    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcFastRewindL()
    {
    // Try before establishment
    iClipVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iClipVideoPlayer->LcFastRewindL( ETrue ), KErrNotReady )
    
    // Establish session, simulate position and duration and try again   
    iClipVideoPlayer->SetMceSession( iMceSession );
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );          
    file->iDuration = KMusEngTestFileDuration;
    file->iPosition = KMusEngTestFilePosition; 
    iClipVideoPlayer->LcFastRewindL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipVideoPlayer->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipVideoPlayer->iFRWDStartTime.Int64() > 0 )
    
    // Simulate fastrewinding for a while
    User::After( 1000 );
    
    // Try to fastrewind when already fastrewinding, will be ignored
    iClipVideoPlayer->LcFastRewindL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipVideoPlayer->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipVideoPlayer->iFRWDStartTime.Int64() > 0 )
    
    // Stop fastrewinding
    iClipVideoPlayer->LcFastRewindL( EFalse );
    EUNIT_ASSERT( file->iPosition < KMusEngTestFilePosition )
    EUNIT_ASSERT( file->iPosition != TTimeIntervalMicroSeconds( 0 ) )
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipVideoPlayer->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipVideoPlayer->iFRWDStartTime.Int64() == 0 )
    
    // Try to stop fastrewinding again, leaves 
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iClipVideoPlayer->LcFastRewindL( EFalse ), KErrAlreadyExists )
    
    // Start fastforwarding
    iClipVideoPlayer->LcFastForwardL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipVideoPlayer->iFFWDStartTime.Int64() >= 0 )
    EUNIT_ASSERT( iClipVideoPlayer->iFRWDStartTime.Int64() == 0 )
    
    // Start fastrewinding, forwarding should be stopped and FFWD started
    iClipVideoPlayer->LcFastRewindL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled );
    EUNIT_ASSERT( iClipVideoPlayer->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipVideoPlayer->iFRWDStartTime.Int64() >= 0 )
    
    // Simulate that clip has just begun and fastrewinding would have 
    // continued over beginning, position should be set to zero.
    
    file->iPosition = TTimeIntervalMicroSeconds( 1 );
    
    // Simulate fastrewinding for a while
    User::After( 1000 );
    
    iClipVideoPlayer->LcFastRewindL( EFalse );
    EUNIT_ASSERT( file->iPosition == TTimeIntervalMicroSeconds( 0 ) )
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipVideoPlayer->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipVideoPlayer->iFRWDStartTime.Int64() == 0 ) 
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcFilePositionL()
    {
    // Try before establishment
    iClipVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iClipVideoPlayer->LcFilePositionL(), KErrNotReady )
    
    // Establish session and try again
    iClipVideoPlayer->SetMceSession( iMceSession );
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );
    file->iPosition = 2000000;   
    EUNIT_ASSERT( iClipVideoPlayer->LcFilePositionL().Int() * 1000000 == 
                  file->iPosition.Int64() )
    
    // Ask position while fastforwarding, it should be bigger than real position
    iClipVideoPlayer->LcFastForwardL( ETrue );
    User::After( 1000000 ); // We have to wait since dividing in LcFilePositionL and 
                            // multiplying before comparison loses difference  
    EUNIT_ASSERT( iClipVideoPlayer->LcFilePositionL().Int() * 1000000 >
                  file->iPosition.Int64() )
    
    // Ask position while fastforwarding beyond end of clip, clip duration is
    // returned
    file->iPosition = file->iDuration;
    EUNIT_ASSERT( iClipVideoPlayer->LcFilePositionL().Int() * 1000000 ==
                  file->iDuration.Int64() )
    
    // Stop fastforwarding, start fastrewinding, position is set to the end clip
    iClipVideoPlayer->LcFastForwardL( EFalse );
    iClipVideoPlayer->LcFastRewindL( ETrue );   
    User::After( 1000000 ); // We have to wait since dividing in LcFilePositionL and 
                            // multiplying before comparison loses difference 
    
    // Ask position while fastrewinding, it should be smaller than real 
    // position
    EUNIT_ASSERT( iClipVideoPlayer->LcFilePositionL().Int() * 1000000 <
                  file->iPosition.Int64() )
    
    // Ask position while fastrewinding beyond the beginning of clip, zero 
    // returned
    file->iPosition = 0;
    EUNIT_ASSERT( iClipVideoPlayer->LcFilePositionL().Int() * 1000000 == 0 )
    
    // Asking position when rewinded to beginning but rewinding has ended 
    // and clip has not ended (position should not be altered in that case)
    iClipVideoPlayer->iRewindedToBeginning = ETrue;
    file->iPosition = 0;
    iClipVideoPlayer->iFRWDStartTime = TTime( 0 );   
    EUNIT_ASSERT( iClipVideoPlayer->LcFilePositionL().Int() * 1000000 == 0 )
    
    // Position has proceeded from beginning, rewinding to beginning info
    // is cleared.
    file->iPosition = 10000000;
    iClipVideoPlayer->iFRWDStartTime = TTime( 0 );
    EUNIT_ASSERT( iClipVideoPlayer->LcFilePositionL().Int() * 1000000 == 10000000 )
    EUNIT_ASSERT( iClipVideoPlayer->iRewindedToBeginning == EFalse )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcFileDurationL()
    {
    // Try before establishment
    iClipVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iClipVideoPlayer->LcFileDurationL(), KErrNotReady )
    
    // Establish session and try again
    iClipVideoPlayer->SetMceSession( iMceSession );    
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession );
    file->iDuration = 2000000;
    EUNIT_ASSERT( iClipVideoPlayer->LcFileDurationL().Int() * 1000000 == 
                  file->iDuration.Int64() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_SetLcFilePositionL()
    {
    TTimeIntervalSeconds time( 20 );
    
    // Try before establishment
    iClipVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iClipVideoPlayer->SetLcFilePositionL( time ), KErrNotReady )
    
    // Normal case with already disabled file source
    iClipVideoPlayer->SetMceSession( iMceSession );    
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iMceSession ); 
    file->iIsEnabled = EFalse;    
    iClipVideoPlayer->SetLcFilePositionL( time );
    EUNIT_ASSERT( file->iPosition.Int64() == 
                  static_cast< TInt64 >( time.Int() ) * 1000000 )
    EUNIT_ASSERT( !file->iIsEnabled )
    
    // Normal case with enabled file source
    TTimeIntervalSeconds anotherTime( 30 );
    file->iIsEnabled = ETrue;
    iClipVideoPlayer->SetLcFilePositionL( anotherTime ); 
    EUNIT_ASSERT( file->iPosition.Int64() ==
                  static_cast< TInt64 >( anotherTime.Int() ) * 1000000 )
    EUNIT_ASSERT( file->iIsEnabled )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcWindowL()
    {
    EUNIT_ASSERT( iClipVideoPlayer->LcWindow() == iClipVideoPlayer )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcCameraControlL()
    {
    EUNIT_ASSERT( iClipVideoPlayer->LcCameraControl() == NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcSourceFileControlL()
    {
    EUNIT_ASSERT( iClipVideoPlayer->LcSourceFileControl() == iClipVideoPlayer )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcDestinationFileControlL()
    {
    EUNIT_ASSERT( iClipVideoPlayer->LcDestinationFileControl() == NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcAudioControlL()
    {
    EUNIT_ASSERT( iClipVideoPlayer->LcAudioControl() == &iLcAudioControlStub )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcZoomControlL()
    {
    EUNIT_ASSERT( iClipVideoPlayer->LcZoomControl() == NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipVideoPlayer::UT_LcBrightnessControlL()
    {
    EUNIT_ASSERT( iClipVideoPlayer->LcBrightnessControl() == NULL )
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngClipVideoPlayer,
    "UT_CMusEngClipVideoPlayer",
    "UNIT" )   
    
EUNIT_TEST(
    "HasClipEnded - test ",
    "CMusEngClipVideoPlayer",
    "HasClipEnded",
    "FUNCTIONALITY",
    SetupL, UT_HasClipEndedL, Teardown)

EUNIT_TEST(
    "LcVideoPlayerState - test ",
    "CMusEngClipVideoPlayer",
    "LcVideoPlayerState",
    "FUNCTIONALITY",
    SetupL, UT_LcVideoPlayerStateL, Teardown)

EUNIT_TEST(
    "LcPlayL - test ",
    "CMusEngClipVideoPlayer",
    "LcPlayL",
    "FUNCTIONALITY",
    SetupL, UT_LcPlayL, Teardown)

EUNIT_TEST(
    "LcPauseL - test ",
    "CMusEngClipVideoPlayer",
    "LcPauseL",
    "FUNCTIONALITY",
    SetupL, UT_LcPauseL, Teardown)

EUNIT_TEST(
    "LcIsPlayingL - test ",
    "CMusEngClipVideoPlayer",
    "LcIsPlayingL",
    "FUNCTIONALITY",
    SetupL, UT_LcIsPlayingL, Teardown)
    
EUNIT_TEST(
    "SetLcFileNameL - test ",
    "CMusEngClipVideoPlayer",
    "SetLcFileNameL",
    "FUNCTIONALITY",
    SetupL, UT_SetLcFileNameL, Teardown)

EUNIT_TEST(
    "LcFastForwardL - test ",
    "CMusEngClipVideoPlayer",
    "LcFastForwardL",
    "FUNCTIONALITY",
    SetupL, UT_LcFastForwardL, Teardown)

EUNIT_TEST(
    "LcFastRewindL - test ",
    "CMusEngClipVideoPlayer",
    "LcFastRewindL",
    "FUNCTIONALITY",
    SetupL, UT_LcFastRewindL, Teardown)

EUNIT_TEST(
    "LcFilePositionL - test ",
    "CMusEngClipVideoPlayer",
    "LcFilePositionL",
    "FUNCTIONALITY",
    SetupL, UT_LcFilePositionL, Teardown)

EUNIT_TEST(
    "LcFileDurationL - test ",
    "CMusEngClipVideoPlayer",
    "LcFileDurationL",
    "FUNCTIONALITY",
    SetupL, UT_LcFileDurationL, Teardown)

EUNIT_TEST(
    "SetLcFilePositionL - test ",
    "CMusEngClipVideoPlayer",
    "SetLcFilePositionL",
    "FUNCTIONALITY",
    SetupL, UT_SetLcFilePositionL, Teardown)    
    
EUNIT_TEST(
    "LcWindow - test ",
    "CMusEngClipVideoPlayer",
    "LcWindow",
    "FUNCTIONALITY",
    SetupL, UT_LcWindowL, Teardown)

EUNIT_TEST(
    "LcCameraControl - test ",
    "CMusEngClipVideoPlayer",
    "LcCameraControl",
    "FUNCTIONALITY",
    SetupL, UT_LcCameraControlL, Teardown)

EUNIT_TEST(
    "LcSourceFileControl - test ",
    "CMusEngClipVideoPlayer",
    "LcSourceFileControl",
    "FUNCTIONALITY",
    SetupL, UT_LcSourceFileControlL, Teardown)

EUNIT_TEST(
    "LcDestinationFileControl - test ",
    "CMusEngClipVideoPlayer",
    "LcDestinationFileControl",
    "FUNCTIONALITY",
    SetupL, UT_LcDestinationFileControlL, Teardown)

EUNIT_TEST(
    "LcAudioControl - test ",
    "CMusEngClipVideoPlayer",
    "LcAudioControl",
    "FUNCTIONALITY",
    SetupL, UT_LcAudioControlL, Teardown)

EUNIT_TEST(
    "LcZoomControl - test ",
    "CMusEngClipVideoPlayer",
    "LcZoomControl",
    "FUNCTIONALITY",
    SetupL, UT_LcZoomControlL, Teardown)

EUNIT_TEST(
    "LcBrightnessControl - test ",
    "CMusEngClipVideoPlayer",
    "LcBrightnessControl",
    "FUNCTIONALITY",
    SetupL, UT_LcBrightnessControlL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE

