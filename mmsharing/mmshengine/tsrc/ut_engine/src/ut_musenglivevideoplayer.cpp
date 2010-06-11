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
#include "ut_musenglivevideoplayer.h"
#include "musenglivevideoplayer.h"
#include "musengtestdefs.h"

//  SYSTEM INCLUDES
#include <eunitmacros.h>
#include <mceoutsession.h>
#include <mcevideostream.h>
#include <mcertpsink.h>
#include <mcecamerasource.h>
#include <mcefilesink.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngLiveVideoPlayer* UT_CMusEngLiveVideoPlayer::NewL()
    {
    UT_CMusEngLiveVideoPlayer* self = UT_CMusEngLiveVideoPlayer::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngLiveVideoPlayer* UT_CMusEngLiveVideoPlayer::NewLC()
    {
    UT_CMusEngLiveVideoPlayer* self = new( ELeave ) UT_CMusEngLiveVideoPlayer();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngLiveVideoPlayer::~UT_CMusEngLiveVideoPlayer()
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngLiveVideoPlayer::UT_CMusEngLiveVideoPlayer()
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }   

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::SetupL()
    {
    iMceSession = CMceOutSession::NewL();
    CMceVideoStream* videoStream = CMceVideoStream::NewLC(); 
    CMceRtpSink* rtpsink = CMceRtpSink::NewLC();
    videoStream->AddSinkL( rtpsink );
    CleanupStack::Pop( rtpsink );
    CMceCameraSource* camera = CMceCameraSource::NewLC();
    iCameraHandler.InitializeL( *camera );
    videoStream->SetSourceL( camera );
    CleanupStack::Pop( camera );
    iMceSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );
    
    iCameraHandler.SetSession( iMceSession );     
    iLiveVideoPlayer = 
        CMusEngLiveVideoPlayer::NewL(  
            iDisplayHandlerStub,
            iCameraHandler, 
            iLcAudioControlStub );
    iLiveVideoPlayer->SetMceSession( iMceSession );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::Teardown()
    {
    delete iLiveVideoPlayer;
    delete iMceSession;
    iLcAudioControlStub.Reset();
    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_NewLL()
    {    
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcVideoPlayerStateL()
    {
    // iMceSession not set
    iLiveVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iLiveVideoPlayer->LcVideoPlayerState() ) )    
    iLiveVideoPlayer->SetMceSession( iMceSession );
    
    // Establish Session and simulate media playing.
    iMceSession->iState = CMceSession::EEstablished;
    for ( TInt i = 0; i < iMceSession->Streams().Count(); i++ )
        {
        iMceSession->Streams()[i]->iState = CMceMediaStream::EStreaming;
        }
    
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPlaying ), 
                         TInt( iLiveVideoPlayer->LcVideoPlayerState() ) )
                                                
    // Media streams unavailable
    iMceSession->Streams()[0]->iState = CMceMediaStream::EUninitialized;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iLiveVideoPlayer->LcVideoPlayerState() ) )

    // All other stream states
    iMceSession->Streams()[0]->iState = CMceMediaStream::EInitialized;	
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EInit ), 
                         TInt( iLiveVideoPlayer->LcVideoPlayerState() ) )	
	
    iMceSession->Streams()[0]->iState = CMceMediaStream::EBuffering;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EBuffering ), 
                         TInt( iLiveVideoPlayer->LcVideoPlayerState() ) )
		
    iMceSession->Streams()[0]->iState = CMceMediaStream::EIdle;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPaused ), 
                         TInt( iLiveVideoPlayer->LcVideoPlayerState() ) )	
	
    iMceSession->Streams()[0]->iState = CMceMediaStream::EDisabled;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPaused ), 
                         TInt( iLiveVideoPlayer->LcVideoPlayerState() ) )
	
    iMceSession->Streams()[0]->iState = CMceMediaStream::EStreaming;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPlaying ), 
                         TInt( iLiveVideoPlayer->LcVideoPlayerState() ) )

    iMceSession->Streams()[0]->iState = CMceMediaStream::ENoResources;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iLiveVideoPlayer->LcVideoPlayerState() ) )
	
    iMceSession->Streams()[0]->iState = CMceMediaStream::ETranscodingRequired;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iLiveVideoPlayer->LcVideoPlayerState() ) )
	
    iMceSession->Streams()[0]->iState = CMceMediaStream::ETranscoding;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iLiveVideoPlayer->LcVideoPlayerState() ) )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcPlayL()
    {
    // Play
    iLiveVideoPlayer->LcPlayL();
    EUNIT_ASSERT( iLiveVideoPlayer->LcIsPlayingL() );
    
    // Pause
    iLiveVideoPlayer->LcPauseL();
    EUNIT_ASSERT( !iLiveVideoPlayer->LcIsPlayingL() );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcWindowL()
    {
    EUNIT_ASSERT( iLiveVideoPlayer->LcWindow() == iLiveVideoPlayer )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcCameraControlL()
    {
    EUNIT_ASSERT( iLiveVideoPlayer->LcCameraControl() == &iCameraHandler )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcSourceFileControlL()
    {
    EUNIT_ASSERT( iLiveVideoPlayer->LcSourceFileControl() == NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcDestinationFileControlL()
    {
    EUNIT_ASSERT( 
        iLiveVideoPlayer->LcDestinationFileControl() == iLiveVideoPlayer )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcAudioControlL()
    {
    EUNIT_ASSERT( iLiveVideoPlayer->LcAudioControl() == &iLcAudioControlStub )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcZoomControlL()
    {
    EUNIT_ASSERT( iLiveVideoPlayer->LcZoomControl() == &iCameraHandler )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcBrightnessControlL()
    {
    EUNIT_ASSERT( iLiveVideoPlayer->LcBrightnessControl() == &iCameraHandler )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_EnableLcWindowL()
    {
    EUNIT_ASSERT( !iLiveVideoPlayer->IsLcWindowEnabled() )
    
    // Enable
    iLiveVideoPlayer->EnableLcWindowL( ETrue );
    EUNIT_ASSERT( iLiveVideoPlayer->IsLcWindowEnabled() )
    
    // Disable
    iLiveVideoPlayer->EnableLcWindowL( EFalse );
    EUNIT_ASSERT( !iLiveVideoPlayer->IsLcWindowEnabled() )    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcWindowRectL()
    {
    TRect rect( 12, 34, 56, 78 );
    iLiveVideoPlayer->SetLcWindowRectL( rect );
    EUNIT_ASSERT( iLiveVideoPlayer->LcWindowRect() == rect )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcWindowOrientationL()
    {
    iLiveVideoPlayer->SetLcWindowOrientationL( MLcWindow::EPortrait );
    EUNIT_ASSERT_EQUALS( TInt( MLcWindow::EPortrait ), 
                         TInt( iLiveVideoPlayer->LcWindowOrientationL() ) )
    
    iLiveVideoPlayer->SetLcWindowOrientationL( MLcWindow::ELandscape );
    EUNIT_ASSERT_EQUALS( TInt( MLcWindow::ELandscape ), 
                         TInt( iLiveVideoPlayer->LcWindowOrientationL() ) )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_SetLcFileNameL()
    {
    EUNIT_ASSERT_EQUALS( KNullDesC(), iLiveVideoPlayer->LcFileName() )
    
    _LIT( KFileName, "my_clip.format");
    TFileName fileName( KFileName );
    iLiveVideoPlayer->SetLcFileNameL( fileName );
    EUNIT_ASSERT_EQUALS( fileName, iLiveVideoPlayer->LcFileName() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveVideoPlayer::UT_LcRecordL()
    {
    // No MCE session
    iLiveVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iLiveVideoPlayer->LcRecordL( ETrue ), KErrNotReady )
    
    // No file sink in the session
    iLiveVideoPlayer->SetMceSession( iMceSession );
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iLiveVideoPlayer->LcRecordL( ETrue ), KErrNotReady )      
    
    // Record, not yet recording
    EUNIT_ASSERT( !iLiveVideoPlayer->LcIsRecording() )
    CMceVideoStream* streamForRecording = CMceVideoStream::NewLC(); 
    CMceFileSink* fileSink = CMceFileSink::NewLC( KTestVideoFileName() );
    fileSink->DisableL();
    streamForRecording->AddSinkL( fileSink );
    CleanupStack::Pop( fileSink );
    iMceSession->AddStreamL( streamForRecording );
    CleanupStack::Pop( streamForRecording );   
    iLiveVideoPlayer->LcRecordL( ETrue );    
    EUNIT_ASSERT( iLiveVideoPlayer->LcIsRecording() )
    
    // Record, already recording
    iLiveVideoPlayer->LcRecordL( ETrue );    
    EUNIT_ASSERT( iLiveVideoPlayer->LcIsRecording() ) 
    
    // Stop recording
    iLiveVideoPlayer->LcRecordL( EFalse );    
    EUNIT_ASSERT( !iLiveVideoPlayer->LcIsRecording() )         
    
    // Stop recording, not anymore recording
    iLiveVideoPlayer->LcRecordL( EFalse );    
    EUNIT_ASSERT( !iLiveVideoPlayer->LcIsRecording() )
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngLiveVideoPlayer,
    "UT_CMusEngLiveVideoPlayer",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusEngLiveVideoPlayer",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewLL, Teardown)    
    
EUNIT_TEST(
    "LcVideoPlayerState - test ",
    "CMusEngLiveVideoPlayer",
    "LcVideoPlayerState",
    "FUNCTIONALITY",
    SetupL, UT_LcVideoPlayerStateL, Teardown)

EUNIT_TEST(
    "LcPlayL - test ",
    "CMusEngLiveVideoPlayer",
    "LcPlayL",
    "FUNCTIONALITY",
    SetupL, UT_LcPlayL, Teardown)

EUNIT_TEST(
    "LcWindow - test ",
    "CMusEngLiveVideoPlayer",
    "LcWindow",
    "FUNCTIONALITY",
    SetupL, UT_LcWindowL, Teardown)

EUNIT_TEST(
    "LcCameraControl - test ",
    "CMusEngLiveVideoPlayer",
    "LcCameraControl",
    "FUNCTIONALITY",
    SetupL, UT_LcCameraControlL, Teardown)

EUNIT_TEST(
    "LcSourceFileControl - test ",
    "CMusEngLiveVideoPlayer",
    "LcSourceFileControl",
    "FUNCTIONALITY",
    SetupL, UT_LcSourceFileControlL, Teardown)

EUNIT_TEST(
    "LcDestinationFileControl - test ",
    "CMusEngLiveVideoPlayer",
    "LcDestinationFileControl",
    "FUNCTIONALITY",
    SetupL, UT_LcDestinationFileControlL, Teardown)

EUNIT_TEST(
    "LcAudioControl - test ",
    "CMusEngLiveVideoPlayer",
    "LcAudioControl",
    "FUNCTIONALITY",
    SetupL, UT_LcAudioControlL, Teardown)

EUNIT_TEST(
    "LcZoomControl - test ",
    "CMusEngLiveVideoPlayer",
    "LcZoomControl",
    "FUNCTIONALITY",
    SetupL, UT_LcZoomControlL, Teardown)

EUNIT_TEST(
    "LcBrightnessControl - test ",
    "CMusEngLiveVideoPlayer",
    "LcBrightnessControl",
    "FUNCTIONALITY",
    SetupL, UT_LcBrightnessControlL, Teardown) 

EUNIT_TEST(
    "EnableLcWindowL - test ",
    "CMusEngLiveVideoPlayer",
    "EnableLcWindowL",
    "FUNCTIONALITY",
    SetupL, UT_EnableLcWindowL, Teardown)    
        
EUNIT_TEST(
    "LcWindowRect - test ",
    "CMusEngLiveVideoPlayer",
    "LcWindowRect",
    "FUNCTIONALITY",
    SetupL, UT_LcWindowRectL, Teardown)  
        
EUNIT_TEST(
    "LcWindowOrientation - test ",
    "CMusEngLiveVideoPlayer",
    "LcWindowOrientation",
    "FUNCTIONALITY",
    SetupL, UT_LcWindowOrientationL, Teardown)
    
EUNIT_TEST(
    "SetLcFileNameL - test ",
    "CMusEngLiveVideoPlayer",
    "SetLcFileNameL",
    "FUNCTIONALITY",
    SetupL, UT_SetLcFileNameL, Teardown)    
    
EUNIT_TEST(
    "LcRecordL - test ",
    "CMusEngLiveVideoPlayer",
    "LcRecordL",
    "FUNCTIONALITY",
    SetupL, UT_LcRecordL, Teardown)    
    
EUNIT_END_TEST_TABLE

//  END OF FILE


