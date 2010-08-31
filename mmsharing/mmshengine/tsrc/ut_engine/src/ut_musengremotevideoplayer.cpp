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
#include "ut_musengremotevideoplayer.h"
#include "musengremotevideoplayer.h"
#include "mceinsession.h"
#include "mcevideostream.h"
#include "mcertpsource.h"
#include "mcedisplaysink.h"

//  SYSTEM INCLUDES
#include <eunitmacros.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngRemoteVideoPlayer* UT_CMusEngRemoteVideoPlayer::NewL()
    {
    UT_CMusEngRemoteVideoPlayer* self = UT_CMusEngRemoteVideoPlayer::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngRemoteVideoPlayer* UT_CMusEngRemoteVideoPlayer::NewLC()
    {
    UT_CMusEngRemoteVideoPlayer* self = new( ELeave ) UT_CMusEngRemoteVideoPlayer();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngRemoteVideoPlayer::~UT_CMusEngRemoteVideoPlayer()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngRemoteVideoPlayer::UT_CMusEngRemoteVideoPlayer()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::SetupL()
    {
    iMceSession = CMceInSession::NewL();
    CMceVideoStream* videoStream = CMceVideoStream::NewLC(); 
    CMceRtpSource* source = CMceRtpSource::NewLC();
    videoStream->SetSourceL( source );
    CleanupStack::Pop( source );
    CMceDisplaySink* sink = CMceDisplaySink::NewLC();
    videoStream->AddSinkL( sink );
    CleanupStack::Pop( sink );
    iMceSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );    
    
    iRemoteVideoPlayer = CMusEngRemoteVideoPlayer::NewL( 
        iDisplayHandlerStub,
        iLcAudioControlStub );
    iRemoteVideoPlayer->SetMceSession( iMceSession );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::Teardown()
    {
    delete iRemoteVideoPlayer;
    delete iMceSession;
    iLcAudioControlStub.Reset();  
    }



// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_NewLL()
    {    
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_LcVideoPlayerStateL()
    {
    // iMceSession not set
    iRemoteVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )
    iRemoteVideoPlayer->SetMceSession( iMceSession );
    
    // Establish Session and simulate media playing.
    iMceSession->iState = CMceSession::EEstablished;
    for ( TInt i = 0; i < iMceSession->Streams().Count(); i++ )
        {
        iMceSession->Streams()[i]->iState = CMceMediaStream::EStreaming;
        }
    
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPlaying ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )
                                                
    // Media streams unavailable
    iMceSession->Streams()[0]->iState = CMceMediaStream::EUninitialized;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )

    // All other stream states
    iMceSession->Streams()[0]->iState = CMceMediaStream::EInitialized;  
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EInit ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )  
    
    iMceSession->Streams()[0]->iState = CMceMediaStream::EBuffering;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EBuffering ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )
        
    iMceSession->Streams()[0]->iState = CMceMediaStream::EIdle;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPaused ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )  
    
    iMceSession->Streams()[0]->iState = CMceMediaStream::EDisabled;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPaused ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )
    
    iDisplayHandlerStub.iIsActive = ETrue;
    iMceSession->Streams()[0]->iState = CMceMediaStream::EStreaming;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPlaying ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )
    
    iDisplayHandlerStub.iIsActive = EFalse;
    iMceSession->Streams()[0]->iState = CMceMediaStream::EStreaming;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EPaused ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )

    iMceSession->Streams()[0]->iState = CMceMediaStream::ENoResources;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )
    
    iMceSession->Streams()[0]->iState = CMceMediaStream::ETranscodingRequired;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )
    
    iMceSession->Streams()[0]->iState = CMceMediaStream::ETranscoding;
    EUNIT_ASSERT_EQUALS( TInt( MLcVideoPlayer::EUnavailable ), 
                         TInt( iRemoteVideoPlayer->LcVideoPlayerState() ) )                      
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_LcPlayL()
    {
    // iMceSession not set
    iRemoteVideoPlayer->SetMceSession( NULL );
    EUNIT_ASSERT( !iRemoteVideoPlayer->LcIsPlayingL() )
    iRemoteVideoPlayer->SetMceSession( iMceSession );
    
    iMceSession->iState = CMceSession::EEstablished;
    for ( TInt i = 0; i < iMceSession->Streams().Count(); i++ )
        {
        iMceSession->Streams()[i]->iState = CMceMediaStream::EStreaming;
        }

    // Play Test
    iRemoteVideoPlayer->LcPlayL();
    EUNIT_ASSERT( iRemoteVideoPlayer->LcIsPlayingL() )
    
    // Pause Test
    iMceSession->Streams()[0]->iState = CMceMediaStream::EDisabled;
    iRemoteVideoPlayer->LcPauseL();
    EUNIT_ASSERT( !iRemoteVideoPlayer->LcIsPlayingL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_LcWindowL()
    {
    EUNIT_ASSERT( iRemoteVideoPlayer->LcWindow() == iRemoteVideoPlayer )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_LcCameraControlL()
    {
    EUNIT_ASSERT( iRemoteVideoPlayer->LcCameraControl() == NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_LcSourceFileControlL()
    {
    EUNIT_ASSERT( iRemoteVideoPlayer->LcSourceFileControl() == NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_LcDestinationFileControlL()
    {
    EUNIT_ASSERT( iRemoteVideoPlayer->LcDestinationFileControl() == NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_LcAudioControlL()
    {
    EUNIT_ASSERT( iRemoteVideoPlayer->LcAudioControl() == &iLcAudioControlStub )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_LcZoomControlL()
    {
    EUNIT_ASSERT( iRemoteVideoPlayer->LcZoomControl() == NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_LcBrightnessControlL()
    {
    EUNIT_ASSERT( iRemoteVideoPlayer->LcBrightnessControl() == NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_EnableLcWindowL()
    {
    EUNIT_ASSERT( !iRemoteVideoPlayer->IsLcWindowEnabled() )
    
    // Enable
    iRemoteVideoPlayer->EnableLcWindowL( ETrue );
    EUNIT_ASSERT( iRemoteVideoPlayer->IsLcWindowEnabled() )
    
    // Disable
    iRemoteVideoPlayer->EnableLcWindowL( EFalse );
    EUNIT_ASSERT( !iRemoteVideoPlayer->IsLcWindowEnabled() )    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_LcWindowRectL()
    {
    TRect rect( 12, 34, 56, 78 );
    iRemoteVideoPlayer->SetLcWindowRectL( rect );
    EUNIT_ASSERT( iRemoteVideoPlayer->LcWindowRect() == rect )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngRemoteVideoPlayer::UT_LcWindowOrientationL()
    {
    iRemoteVideoPlayer->SetLcWindowOrientationL( MLcWindow::EPortrait );
    EUNIT_ASSERT_EQUALS( TInt( MLcWindow::EPortrait ), 
                         TInt( iRemoteVideoPlayer->LcWindowOrientationL() ) )
    
    iRemoteVideoPlayer->SetLcWindowOrientationL( MLcWindow::ELandscape );
    EUNIT_ASSERT_EQUALS( TInt( MLcWindow::ELandscape ), 
                         TInt( iRemoteVideoPlayer->LcWindowOrientationL() ) )
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngRemoteVideoPlayer,
    "UT_CMusEngRemoteVideoPlayer",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusEngRemoteVideoPlayer",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewLL, Teardown)    
    
EUNIT_TEST(
    "LcVideoPlayerState - test ",
    "CMusEngRemoteVideoPlayer",
    "LcVideoPlayerState",
    "FUNCTIONALITY",
    SetupL, UT_LcVideoPlayerStateL, Teardown)

EUNIT_TEST(
    "LcPlayL - test ",
    "CMusEngRemoteVideoPlayer",
    "LcPlayL",
    "FUNCTIONALITY",
    SetupL, UT_LcPlayL, Teardown)

EUNIT_TEST(
    "LcWindow - test ",
    "CMusEngRemoteVideoPlayer",
    "LcWindow",
    "FUNCTIONALITY",
    SetupL, UT_LcWindowL, Teardown)

EUNIT_TEST(
    "LcCameraControl - test ",
    "CMusEngRemoteVideoPlayer",
    "LcCameraControl",
    "FUNCTIONALITY",
    SetupL, UT_LcCameraControlL, Teardown)

EUNIT_TEST(
    "LcSourceFile - test ",
    "CMusEngRemoteVideoPlayer",
    "LcSourceFile",
    "FUNCTIONALITY",
    SetupL, UT_LcSourceFileControlL, Teardown)

EUNIT_TEST(
    "LcDestinationFile - test ",
    "CMusEngRemoteVideoPlayer",
    "LcDestinationFile",
    "FUNCTIONALITY",
    SetupL, UT_LcDestinationFileControlL, Teardown)

EUNIT_TEST(
    "LcAudioControl - test ",
    "CMusEngRemoteVideoPlayer",
    "LcAudioControl",
    "FUNCTIONALITY",
    SetupL, UT_LcAudioControlL, Teardown)

EUNIT_TEST(
    "LcZoomControl - test ",
    "CMusEngRemoteVideoPlayer",
    "LcZoomControl",
    "FUNCTIONALITY",
    SetupL, UT_LcZoomControlL, Teardown)

EUNIT_TEST(
    "LcBrightnessControl - test ",
    "CMusEngRemoteVideoPlayer",
    "LcBrightnessControl",
    "FUNCTIONALITY",
    SetupL, UT_LcBrightnessControlL, Teardown)

EUNIT_TEST(
    "EnableLcWindowL - test ",
    "CMusEngRemoteVideoPlayer",
    "EnableLcWindowL",
    "FUNCTIONALITY",
    SetupL, UT_EnableLcWindowL, Teardown)    
        
EUNIT_TEST(
    "LcWindowRect - test ",
    "CMusEngRemoteVideoPlayer",
    "LcWindowRect",
    "FUNCTIONALITY",
    SetupL, UT_LcWindowRectL, Teardown)  
        
EUNIT_TEST(
    "LcWindowOrientation - test ",
    "CMusEngRemoteVideoPlayer",
    "LcWindowOrientation",
    "FUNCTIONALITY",
    SetupL, UT_LcWindowOrientationL, Teardown)
    
    
EUNIT_END_TEST_TABLE

//  END OF FILE


