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
#include "ut_musengtwowayrecvsession.h"
#include "musengstubs.h"
#include "musengtestdefs.h"
#include "musengtwowayrecvsession.h"
#include "musengmceutils.h"
#include "mussessionproperties.h"
#include "contactenginestub.h"
#include "musenglivevideoplayer.h"

//  SYSTEM INCLUDES
#include <eunitmacros.h>

#include <mceinsession.h>
#include <mcestreambundle.h>
#include <mcevideostream.h>
#include <mceaudiostream.h>
#include <mcertpsource.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <mcecamerasource.h>
#include <mcefilesource.h>
#include <mcertpsink.h>
#include <mceaudiocodec.h>
#include <mceh263codec.h>
#include <mceavccodec.h>
#include <musmanager.h>

#include <audiopreference.h>

// Incoming session may be rejected silently due memory running out. Ensure that 
// incoming session was handled fully by using this macro.
#define MUSENG_TEST_ASSERT_INCOMING_SESSION_ALLOC_FAILURE_L \
__ASSERT_ALWAYS( iReceiveSession->iSession && \
iReceiveSession->iSession->iState != CMceSession::ETerminated, User::Leave( KErrNoMemory ) );


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngTwoWayRecvSession* UT_CMusEngTwoWayRecvSession::NewL()
    {
    UT_CMusEngTwoWayRecvSession* self = UT_CMusEngTwoWayRecvSession::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
UT_CMusEngTwoWayRecvSession* UT_CMusEngTwoWayRecvSession::NewLC()
    {
    UT_CMusEngTwoWayRecvSession* self = new( ELeave ) UT_CMusEngTwoWayRecvSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngTwoWayRecvSession::~UT_CMusEngTwoWayRecvSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngTwoWayRecvSession::UT_CMusEngTwoWayRecvSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::SetupL(  )
    {
    iLcSessionObserver = new( ELeave )CLcSessionObserverStub;
    iReceiveSession = CMusEngTwoWayRecvSession::NewL();
    iReceiveSession->SetLcSessionObserver( iLcSessionObserver );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::Teardown(  )
    {
    delete iReceiveSession;
    delete iLcSessionObserver;
    PropertyHelper::Close();
    }


// TEST HELPERS

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMceVideoStream* UT_CMusEngTwoWayRecvSession::ConstructVideoInStreamLC()
    {
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    
    videoStream->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop();
    
    videoStream->AddSinkL( 
                CMceDisplaySink::NewLC( *iReceiveSession->iManager ) );
    CleanupStack::Pop();    
    
    return videoStream;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMceVideoStream* UT_CMusEngTwoWayRecvSession::ConstructVideoOutStreamLC()
    {
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();    
       
    videoStream->SetSourceL( 
                CMceCameraSource::NewLC( *iReceiveSession->iManager ) );
    CleanupStack::Pop();
    
    videoStream->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    
    return videoStream;
    }    


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//   
CMceAudioStream* UT_CMusEngTwoWayRecvSession::ConstructAudioInStreamLC()
    {
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
   
    audioStream->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop();
    
    audioStream->AddSinkL( CMceSpeakerSink::NewLC() );
    CleanupStack::Pop();
    
    return audioStream;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//   
CMceAudioStream* UT_CMusEngTwoWayRecvSession::ConstructAudioOutStreamLC()
    {
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();

    audioStream->SetSourceL( 
        CMceFileSource::NewLC( *iReceiveSession->iManager, KNullDesC() ) );
    CleanupStack::Pop();
    
    audioStream->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();

    return audioStream;

    }
                                                 

// -----------------------------------------------------------------------------
// Helper to construct incoming sessions.
// -----------------------------------------------------------------------------
//
CMceInSession* UT_CMusEngTwoWayRecvSession::ConstructInSessionLC( 
                                             TInt aVideoStreamCount,
                                             TInt aAudioStreamCount,
                                             TBool aConstructSinks )
    {
    CMceInSession* inSession = CMceInSession::NewL( 
                                        *(iReceiveSession->iManager), 
                                        KTestOriginator() );
    CleanupStack::PushL( inSession );
                                        
    for ( TInt i = 0; i < aVideoStreamCount; ++i )
        {
        CMceVideoStream* videoStream = CMceVideoStream::NewLC();
        
        CMceRtpSource* rtpSource = CMceRtpSource::NewLC();
        videoStream->SetSourceL( rtpSource );
        CleanupStack::Pop( rtpSource );
        
        if ( aConstructSinks )
            {
            CMceDisplaySink* display = CMceDisplaySink::NewLC( 
                                                *(iReceiveSession->iManager) );
            videoStream->AddSinkL( display );
            CleanupStack::Pop( display );
            }
        
        inSession->AddStreamL( videoStream );
        CleanupStack::Pop( videoStream );
        }
    
    for ( TInt i = 0; i < aAudioStreamCount; ++i )
        {
        CMceAudioStream* audioStream = CMceAudioStream::NewLC();
        
        CMceRtpSource* rtpSource = CMceRtpSource::NewLC();
        audioStream->SetSourceL( rtpSource );
        CleanupStack::Pop( rtpSource );
        
        if ( aConstructSinks )
            {
            CMceSpeakerSink* speaker = CMceSpeakerSink::NewLC();
            audioStream->AddSinkL( speaker );
            CleanupStack::Pop( speaker );
            }
        
        inSession->AddStreamL( audioStream );
        CleanupStack::Pop( audioStream );
        }    
    
    return inSession;
    }
                                             
// -----------------------------------------------------------------------------
// Helper to construct incoming two-way video session.
// -----------------------------------------------------------------------------
//
CMceInSession* UT_CMusEngTwoWayRecvSession::ConstructTwoWayInSessionLC( TBool aConstructSinks )
    {
    CMceInSession* inSession = CMceInSession::NewL( 
                                        *(iReceiveSession->iManager), 
                                        KTestOriginator() );
    CleanupStack::PushL( inSession );
                                        
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
        
    CMceRtpSource* rtpSource = CMceRtpSource::NewLC();
    videoStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource );
    
    if ( aConstructSinks )
        {
        CMceDisplaySink* display = CMceDisplaySink::NewLC( 
                                            *(iReceiveSession->iManager) );
        videoStream->AddSinkL( display );
        CleanupStack::Pop( display );
        }
    
    CMceVideoStream* videoOutStream = CMceVideoStream::NewLC();
          
    CMceCameraSource* cameraSource = CMceCameraSource::NewLC( *(iReceiveSession->iManager) );
    videoOutStream->SetSourceL( cameraSource );
    CleanupStack::Pop( cameraSource );
      
    if ( aConstructSinks )
        {
        CMceRtpSink* rtpSink = CMceRtpSink::NewLC();
        videoOutStream->AddSinkL( rtpSink );
        CleanupStack::Pop( rtpSink );
        }
    videoStream->BindL( videoOutStream );
    CleanupStack::Pop( videoOutStream );
    
    inSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );
    
    return inSession;
    }                                             


// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_NewLL(  )
    {
    EUNIT_ASSERT( iReceiveSession );
    EUNIT_ASSERT( !iReceiveSession->iSession );
    EUNIT_ASSERT( iReceiveSession->iMceManagerUid == 
                  TUid::Uid(CMusManager::ESipInviteDesired2WayVideo) );
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_EnableDisplayLL()
    {
    TRAPD( error, iReceiveSession->EnableDisplayL( ETrue ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    TMceTransactionDataContainer container;
    CMceInSession* inSession = ConstructTwoWayInSessionLC();
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    MUSENG_TEST_ASSERT_INCOMING_SESSION_ALLOC_FAILURE_L
    
    CMceDisplaySink* display = 
                    MusEngMceUtils::GetDisplayL( *(iReceiveSession->iSession) );
    
    CMceDisplaySink* receivingDisplay = 
                    MusEngMceUtils::GetReceivingDisplayL( *iReceiveSession->iSession );
    
    // Disable
    iReceiveSession->EnableDisplayL( EFalse );
    
    EUNIT_ASSERT( !display->iIsEnabled );
    EUNIT_ASSERT( !receivingDisplay->iIsEnabled );
    
    // Try to disable again, request should be ignored
    
    iReceiveSession->EnableDisplayL( EFalse );
    
    EUNIT_ASSERT( !display->iIsEnabled );
    EUNIT_ASSERT( !receivingDisplay->iIsEnabled );

    // Enable
    
    iReceiveSession->EnableDisplayL( ETrue );
    
    EUNIT_ASSERT( display->iIsEnabled );
    EUNIT_ASSERT( receivingDisplay->iIsEnabled );
    
    // Try to enable again, request should be ignored
    
    iReceiveSession->EnableDisplayL( ETrue );
    
    EUNIT_ASSERT( display->iIsEnabled );
    EUNIT_ASSERT( receivingDisplay->iIsEnabled );
        
    // Session does not have sending stream (viewfinder does not exist)
    //
    delete iReceiveSession->iSession;
    iReceiveSession->iSession = NULL;
    inSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    display = MusEngMceUtils::GetVfDisplay( *(iReceiveSession->iSession) );  
    EUNIT_ASSERT( display == NULL );
    receivingDisplay = MusEngMceUtils::GetReceivingDisplayL( *iReceiveSession->iSession );
    iReceiveSession->EnableDisplayL( EFalse );
    EUNIT_ASSERT( !receivingDisplay->iIsEnabled );  

    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_SetOrientationLL()
    {
    // Try without session, fails
    EUNIT_ASSERT_SPECIFIC_LEAVE( iReceiveSession->SetOrientationL( 
                                               CMusEngMceSession::EPortrait ), 
                                KErrNotReady )
    
    TMceTransactionDataContainer container;
    CMceInSession* inSession = ConstructTwoWayInSessionLC();
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    MUSENG_TEST_ASSERT_INCOMING_SESSION_ALLOC_FAILURE_L
    
    CMceDisplaySink* display = 
                   MusEngMceUtils::GetDisplayL( *iReceiveSession->iSession );
    
    CMceDisplaySink* receivingDisplay = 
                   MusEngMceUtils::GetReceivingDisplayL( *iReceiveSession->iSession );
       
    
    // Check the initial state
    display->iRotation = CMceDisplaySink::ENone;
    receivingDisplay->iRotation = CMceDisplaySink::ENone;
    EUNIT_ASSERT_EQUALS( iReceiveSession->OrientationL(), 
                        CMusEngMceSession::EPortrait )      
                        
    // Successfully change orientation
    
    iReceiveSession->SetOrientationL( CMusEngMceSession::ELandscape );
    EUNIT_ASSERT_EQUALS( iReceiveSession->OrientationL(), 
                        CMusEngMceSession::ELandscape )
    
    EUNIT_ASSERT_EQUALS( display->iRotation, CMceDisplaySink::EClockwise90Degree );
    EUNIT_ASSERT_EQUALS( receivingDisplay->iRotation, CMceDisplaySink::EClockwise90Degree );
    
    iReceiveSession->SetOrientationL( CMusEngMceSession::EPortrait );
    EUNIT_ASSERT_EQUALS( iReceiveSession->OrientationL(), 
                        CMusEngMceSession::EPortrait )          
    
    EUNIT_ASSERT_EQUALS( display->iRotation, CMceDisplaySink::ENone );
    EUNIT_ASSERT_EQUALS( receivingDisplay->iRotation, CMceDisplaySink::ENone );
    
    // Session does not have sending stream (viewfinder does not exist)
    delete iReceiveSession->iSession;
    iReceiveSession->iSession = NULL;
    inSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    MUSENG_TEST_ASSERT_INCOMING_SESSION_ALLOC_FAILURE_L
    display = MusEngMceUtils::GetVfDisplay( *(iReceiveSession->iSession) );  
    EUNIT_ASSERT( display == NULL );
    receivingDisplay = MusEngMceUtils::GetReceivingDisplayL( *iReceiveSession->iSession );
    iReceiveSession->SetOrientationL( CMusEngMceSession::ELandscape );
    EUNIT_ASSERT_EQUALS( receivingDisplay->iRotation, CMceDisplaySink::EClockwise90Degree );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_OrientationLL()
    {
    // Try without session, fails
    EUNIT_ASSERT_SPECIFIC_LEAVE( iReceiveSession->OrientationL(), 
                                KErrNotReady )
    
    TMceTransactionDataContainer container;
    CMceInSession* inSession = ConstructTwoWayInSessionLC();
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    MUSENG_TEST_ASSERT_INCOMING_SESSION_ALLOC_FAILURE_L
    
    CMceDisplaySink* receivingDisplay = 
         MusEngMceUtils::GetReceivingDisplayL( *iReceiveSession->iSession );
    receivingDisplay->iRotation = CMceDisplaySink::ENone;
    
    EUNIT_ASSERT_EQUALS( iReceiveSession->OrientationL(), 
                           CMusEngMceSession::EPortrait )      
    
    receivingDisplay->iRotation = CMceDisplaySink::EClockwise90Degree;
        
    EUNIT_ASSERT_EQUALS( iReceiveSession->OrientationL(), 
                               CMusEngMceSession::ELandscape ) 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_SetLocalRectLL()
    {
    // Does nothing real before session establishment
    TRect rect1( 100, 200, 300, 500 );
    iReceiveSession->SetSecondaryRectL( rect1 ); 
    
    EUNIT_ASSERT( rect1 == iReceiveSession->iLocalRect );
    
    // Local rect changing affects to viewfinder display sink not to receiving video
    //
    TMceTransactionDataContainer container;
    CMceInSession* inSession = ConstructTwoWayInSessionLC();
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    MUSENG_TEST_ASSERT_INCOMING_SESSION_ALLOC_FAILURE_L
    
    CMceDisplaySink* vfDisplay = 
                   MusEngMceUtils::GetDisplayL( *( iReceiveSession->iSession ) );
    CMceDisplaySink* receivingDisplay = 
                   MusEngMceUtils::GetReceivingDisplayL( *( iReceiveSession->iSession ) );

    EUNIT_ASSERT( vfDisplay->DisplayRectL() == iReceiveSession->iLocalRect );
    EUNIT_ASSERT( receivingDisplay->DisplayRectL() != iReceiveSession->iLocalRect );
        
    TRect rect2( 200, 200, 300, 500 );
    iReceiveSession->SetSecondaryRectL( rect2 ); 
    EUNIT_ASSERT( rect2 == iReceiveSession->iLocalRect );
    EUNIT_ASSERT( vfDisplay->DisplayRectL() == iReceiveSession->iLocalRect );
    EUNIT_ASSERT( receivingDisplay->DisplayRectL() != iReceiveSession->iLocalRect );
    
    // Terminate session and try again, rect must not be changed
    
    iReceiveSession->iSession->iState = CMceSession::ETerminated;
    
    TRect rect3( 300, 300, 400, 600 );
    
    iReceiveSession->SetSecondaryRectL( rect3 ); 
    
    EUNIT_ASSERT( vfDisplay->DisplayRectL() != rect3 )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_SetRectsLL()
    {
    // Does nothing real before session establishment
    TRect localrect1( 100, 200, 300, 400 );
    TRect remoterect1( 100, 200, 300, 500 );
    iReceiveSession->SetRectsL( remoterect1, localrect1 ); 
    
    EUNIT_ASSERT( localrect1 == iReceiveSession->iLocalRect );
    EUNIT_ASSERT( remoterect1 == iReceiveSession->iRect );
    
    // Local rect changing affects to viewfinder display sink and remote rect to
    // receiving video display sink
    //
    TMceTransactionDataContainer container;
    CMceInSession* inSession = ConstructTwoWayInSessionLC();
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    MUSENG_TEST_ASSERT_INCOMING_SESSION_ALLOC_FAILURE_L
    
    CMceDisplaySink* vfDisplay = 
                   MusEngMceUtils::GetDisplayL( *( iReceiveSession->iSession ) );
    CMceDisplaySink* receivingDisplay = 
                   MusEngMceUtils::GetReceivingDisplayL( *( iReceiveSession->iSession ) );

    TRect localrect2( 200, 200, 300, 400 );
    TRect remoterect2( 200, 200, 300, 500 );
    EUNIT_ASSERT( vfDisplay->DisplayRectL() != localrect2 );
    EUNIT_ASSERT( receivingDisplay->DisplayRectL() != remoterect2 );

    iReceiveSession->SetRectsL( remoterect2, localrect2 ); 
    EUNIT_ASSERT( localrect2 == iReceiveSession->iLocalRect );
    EUNIT_ASSERT( remoterect2 == iReceiveSession->iRect );
    EUNIT_ASSERT( vfDisplay->DisplayRectL() == iReceiveSession->iLocalRect );
    EUNIT_ASSERT( receivingDisplay->DisplayRectL() == iReceiveSession->iRect );
    
    // Terminate session and try again, rect must not be changed
    
    iReceiveSession->iSession->iState = CMceSession::ETerminated;
    
    TRect localrect3( 300, 200, 300, 400 );
    TRect remoterect3( 300, 200, 300, 500 );
    
    iReceiveSession->SetRectsL( remoterect3, localrect3 );
    
    EUNIT_ASSERT( vfDisplay->DisplayRectL() != localrect3 )
    EUNIT_ASSERT( receivingDisplay->DisplayRectL() != remoterect3 );
    
    // Session does not have sending stream (viewfinder does not exist)
    
    TRect localrect4( 400, 200, 300, 400 );
    TRect remoterect4( 400, 200, 300, 500 );
    delete iReceiveSession->iSession;
    iReceiveSession->iSession = NULL;
    inSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    MUSENG_TEST_ASSERT_INCOMING_SESSION_ALLOC_FAILURE_L
    vfDisplay = MusEngMceUtils::GetVfDisplay( *(iReceiveSession->iSession) );  
    EUNIT_ASSERT( vfDisplay == NULL );
    receivingDisplay = MusEngMceUtils::GetReceivingDisplayL( *iReceiveSession->iSession );
    iReceiveSession->SetRectsL( remoterect4, localrect4 );
    EUNIT_ASSERT( localrect4 == iReceiveSession->iLocalRect );
    EUNIT_ASSERT( remoterect4 == iReceiveSession->iRect );
    EUNIT_ASSERT( receivingDisplay->DisplayRectL() == iReceiveSession->iRect );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_CameraL()
    {
    // Camera cannot be used without MCE session
    //
    EUNIT_ASSERT_LEAVE( iReceiveSession->Camera().ChangeCameraL( 
            TMusEngCameraHandler::EFrontCamera ) );
    
    // Camera cannot be used without video out stream
    //
    TMceTransactionDataContainer container;
    CMceInSession* inSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    MUSENG_TEST_ASSERT_INCOMING_SESSION_ALLOC_FAILURE_L
    EUNIT_ASSERT_LEAVE( iReceiveSession->Camera().ChangeCameraL( 
            TMusEngCameraHandler::EFrontCamera ) );
    
    // Camera can be used when video out stream exists
    //
    delete iReceiveSession->iSession;
    iReceiveSession->iSession = NULL;
    inSession = ConstructTwoWayInSessionLC();
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    MUSENG_TEST_ASSERT_INCOMING_SESSION_ALLOC_FAILURE_L
    iReceiveSession->Camera().ChangeCameraL( 
            TMusEngCameraHandler::EFrontCamera );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_LocalVideoPlayerL()
    {
    EUNIT_ASSERT( iReceiveSession->LocalVideoPlayer() == 
                  iReceiveSession->iLiveVideoPlayer )
    }
   
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_HandleSessionStateChangedL()
    {
    // Unknown session, let the base class handle 
    CMceInSession* inSession = ConstructInSessionLC( 1, 1 );

    iReceiveSession->HandleSessionStateChanged( *inSession, 0, KNullDesC8() );
    
    // Simulate incoming session
    TMceTransactionDataContainer container;
    
    iReceiveSession->IncomingSession( inSession, &container ); 
    CleanupStack::Pop( inSession );
    
    // Session is in state EReserving, let the base class handle
    iReceiveSession->HandleSessionStateChanged( *inSession, 0, KNullDesC8() );
    
    // The normal cases:
    
    // 1 ) First transition to state EProceeding, user and remote end should
    //     be notified
    inSession->iState = CMceSession::EProceeding;
    iReceiveSession->HandleSessionStateChanged( *inSession, 0, KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionStateChanged ) )
    EUNIT_ASSERT( iReceiveSession->iRingLCalled )
    iLcSessionObserver->Reset();
    
    // 2 ) Now simulate second transition to EProceeding state which can happen
    //     if we force 100rel to be used. User and remote end should not be 
    //     notified anymore but change should be ignored.
    inSession->iState = CMceSession::EProceeding;
    iReceiveSession->HandleSessionStateChanged( *inSession, 0, KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( CLcSessionObserverStub::EUnknown ),
                         TInt( iLcSessionObserver->iCalledFunction ) )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusEngTwoWayRecvSession::UT_AdjustVideoCodecLL()
    {
    CMceVideoCodec* codecH263 = CMceH263Codec::NewLC( KMceSDPNameH263() );    
    CMceVideoCodec* codecH2632000 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );
    CMceVideoCodec* codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    CMceVideoCodec* codecUnknown = CMceH263Codec::NewLC( KNullDesC8() );
    
    iReceiveSession->AdjustVideoCodecL( *codecH263, KMceRTPSource );
    iReceiveSession->AdjustVideoCodecL( *codecH2632000, KMceRTPSource );
    iReceiveSession->AdjustVideoCodecL( *codecAvc, KMceRTPSource );
    iReceiveSession->AdjustVideoCodecL( *codecUnknown, KMceRTPSource );
    
    EUNIT_ASSERT_EQUALS( codecH263->KeepAliveTimer(), 6 )
    EUNIT_ASSERT_EQUALS( codecH263->KeepAlivePayloadType(), 96 )
    EUNIT_ASSERT_EQUALS( codecH263->KeepAliveData(), KNullDesC8() )
    
    EUNIT_ASSERT_EQUALS( codecH2632000->KeepAliveTimer(), 6 )
    EUNIT_ASSERT_EQUALS( codecH2632000->KeepAlivePayloadType(), 96 )
    EUNIT_ASSERT_EQUALS( codecH2632000->KeepAliveData(), KNullDesC8() )
    
    EUNIT_ASSERT_EQUALS( codecAvc->KeepAliveTimer(), 6 )
    EUNIT_ASSERT_EQUALS( codecAvc->KeepAlivePayloadType(), 98 )
    EUNIT_ASSERT_EQUALS( codecAvc->KeepAliveData(), KNullDesC8() )
    
    EUNIT_ASSERT_EQUALS( codecUnknown->KeepAliveTimer(), 6 )
    EUNIT_ASSERT_EQUALS( codecUnknown->KeepAlivePayloadType(), 0 )
    EUNIT_ASSERT_EQUALS( codecUnknown->KeepAliveData(), KNullDesC8() )
                
    CleanupStack::PopAndDestroy( codecUnknown );
    CleanupStack::PopAndDestroy( codecAvc );
    CleanupStack::PopAndDestroy( codecH2632000 );
    CleanupStack::PopAndDestroy( codecH263 );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusEngTwoWayRecvSession::UT_AdjustAudioCodecLL()
    {
    CMceAudioCodec* codec = 
                iReceiveSession->iManager->SupportedAudioCodecs()[0]->CloneL();
    CleanupStack::PushL( codec );
    iReceiveSession->AdjustAudioCodecL( *codec );
    
    // Test keepalive timer
    
    TUint8 keepAliveTimer = codec->KeepAliveTimer();
    EUNIT_ASSERT( codec->KeepAliveTimer() == 6 )
    EUNIT_ASSERT( codec->KeepAlivePayloadType() == 97 )
    EUNIT_ASSERT( codec->KeepAliveData() == KNullDesC8() )

    // Test that base class has been called
    
    EUNIT_ASSERT( codec->iMMFPriority == KAudioPrioritySwisPlayback )
    EUNIT_ASSERT( codec->iMMFPriorityPreference == KAudioPrefSwisPlayback )
    
    CleanupStack::PopAndDestroy( codec );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusEngTwoWayRecvSession::UT_DoCodecSelectionLL()
    {    
    // Check that Non-Interleaved AVC is preferred over single NAL 
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    
    CMceVideoCodec* codecAvcSingleNal = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    codecAvcSingleNal->iCodecMode = KMceAvcModeSingleNal; 
    videoStream->AddCodecL( codecAvcSingleNal );
    CleanupStack::Pop( codecAvcSingleNal );
    
    CMceVideoCodec* codecAvcNonInterleaved = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    codecAvcNonInterleaved->iCodecMode = KMceAvcModeNonInterleaved;
    videoStream->AddCodecL( codecAvcNonInterleaved );
    CleanupStack::Pop( codecAvcNonInterleaved );
    
    iReceiveSession->DoCodecSelectionL( *videoStream );
    
    EUNIT_ASSERT_EQUALS( videoStream->Codecs().Count(), 1 )
    EUNIT_ASSERT_EQUALS( videoStream->Codecs()[0]->SdpName(), KMceSDPNameH264() )
    EUNIT_ASSERT_EQUALS( videoStream->Codecs()[0]->CodecMode(),
                         KMceAvcModeNonInterleaved )
    
    CleanupStack::PopAndDestroy( videoStream );
                         
    // Check that Single NAL AVC is preferred over H263
    videoStream = CMceVideoStream::NewLC();
    
    CMceVideoCodec* codecH263 = CMceH263Codec::NewLC( KMceSDPNameH263() );    
    videoStream->AddCodecL( codecH263 );
    CleanupStack::Pop( codecH263 );
    
    codecAvcSingleNal = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    codecAvcSingleNal->iCodecMode = KMceAvcModeSingleNal; 
    videoStream->AddCodecL( codecAvcSingleNal );
    CleanupStack::Pop( codecAvcSingleNal );

    iReceiveSession->DoCodecSelectionL( *videoStream );
    
    EUNIT_ASSERT_EQUALS( videoStream->Codecs().Count(), 1 )
    EUNIT_ASSERT_EQUALS( videoStream->Codecs()[0]->SdpName(), KMceSDPNameH264() )
    EUNIT_ASSERT_EQUALS( videoStream->Codecs()[0]->CodecMode(),
                         KMceAvcModeSingleNal )
                         
    CleanupStack::PopAndDestroy( videoStream );
    
    // Check that H263 codecs without AVC are handled correctly (in base class)
    videoStream = CMceVideoStream::NewLC();
    
    codecH263 = CMceH263Codec::NewLC( KMceSDPNameH263() );    
    videoStream->AddCodecL( codecH263 );
    CleanupStack::Pop( codecH263 );
    
    CMceVideoCodec* codecH2632000 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );   
    videoStream->AddCodecL( codecH2632000 );
    CleanupStack::Pop( codecH2632000 );
    
    iReceiveSession->DoCodecSelectionL( *videoStream );
    
    EUNIT_ASSERT_EQUALS( videoStream->Codecs().Count(), 1 )
    
    CleanupStack::PopAndDestroy( videoStream );
    
    // Check that AVC non-interleaved with best bitrate is preferred
    videoStream = CMceVideoStream::NewLC();
    
    codecAvcNonInterleaved = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    codecAvcNonInterleaved->iCodecMode = KMceAvcModeNonInterleaved;
    codecAvcNonInterleaved->SetMaxBitrateL( 80000 );
    videoStream->AddCodecL( codecAvcNonInterleaved );
    CleanupStack::Pop( codecAvcNonInterleaved );
    
    CMceVideoCodec* codecAvcNonInterleaved128 = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    codecAvcNonInterleaved128->iCodecMode = KMceAvcModeNonInterleaved;
    codecAvcNonInterleaved128->SetMaxBitrateL( 128000 );
    videoStream->AddCodecL( codecAvcNonInterleaved128 );
    CleanupStack::Pop( codecAvcNonInterleaved128 );
    
    iReceiveSession->DoCodecSelectionL( *videoStream );
    
    EUNIT_ASSERT_EQUALS( videoStream->Codecs().Count(), 1 )
    EUNIT_ASSERT( videoStream->Codecs()[0] == codecAvcNonInterleaved128 )

    CleanupStack::PopAndDestroy( videoStream );
    
    // Check that in case of several AVC single NALs, the best bitrate is preferred
    videoStream = CMceVideoStream::NewLC();
    
    codecAvcSingleNal = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    codecAvcSingleNal->iCodecMode = KMceAvcModeSingleNal;
    codecAvcSingleNal->SetMaxBitrateL( 80000 );
    videoStream->AddCodecL( codecAvcSingleNal );
    CleanupStack::Pop( codecAvcSingleNal );
    
    CMceVideoCodec* codecAvcSingleNal128 = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    codecAvcSingleNal128->iCodecMode = KMceAvcModeSingleNal;
    codecAvcSingleNal128->SetMaxBitrateL( 128000 );
    videoStream->AddCodecL( codecAvcSingleNal128 );
    CleanupStack::Pop( codecAvcSingleNal128 );
    
    iReceiveSession->DoCodecSelectionL( *videoStream );
    
    EUNIT_ASSERT_EQUALS( videoStream->Codecs().Count(), 1 )
    EUNIT_ASSERT( videoStream->Codecs()[0] == codecAvcSingleNal128 )

    CleanupStack::PopAndDestroy( videoStream );
   
    }
            

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngTwoWayRecvSession::UT_IncomingSessionL()
    {
    _LIT8( KMusEngSwisTestIdentifier, "Require: precondition" );
    _LIT8( KMusEngTestHeader, "Some header" );

    TMceTransactionDataContainer container;
    
    // Simulate SWIS 2.2 invite. There are no streams, so completion fails and 
    // session gets rejected. Still prenotification has been given.
    CMceInSession* inSession = ConstructInSessionLC( 0, 0 );
    CDesC8Array* headers = new ( ELeave ) CDesC8ArrayFlat( 2 );
    CleanupStack::PushL( headers );
    headers->AppendL( KMusEngTestHeader() );
    headers->AppendL( KMusEngSwisTestIdentifier() );
    container.SetHeaders( headers );
    CleanupStack::Pop( headers );
    iReceiveSession->IncomingSession( inSession, &container );    
    CleanupStack::Pop( inSession );
    container.Clear();
    
    EUNIT_ASSERT( iReceiveSession->iSession );
    EUNIT_ASSERT( iReceiveSession->iSession->iState == 
                  CMceSession::ETerminated );
    
    EUNIT_ASSERT( iReceiveSession->iSession->i100RelUsage == 
                  KMce100RelRequired )
    EUNIT_ASSERT( !iReceiveSession->iRingLCalled )              

    // Try again with valid session, normal case
    inSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingSession( inSession, &container );    
    CleanupStack::Pop( inSession );
    
    EUNIT_ASSERT( iReceiveSession->iSession );
    // Next state depends on success of completion of session structure.
    // When using alloc failure state is ETerminated, in succesfull case it is
    // EReserving. However it is set to "normal" state EReserving after 
    // assertion
    EUNIT_ASSERT( iReceiveSession->iSession->iState == 
                  CMceSession::EReserving ||
                  iReceiveSession->iSession->iState == 
                  CMceSession::ETerminated );
    iReceiveSession->iSession->iState = CMceSession::EReserving;
    
    // Try again without container, new session gets terminated and deleted
    inSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingSession( inSession, NULL );    
    CleanupStack::Pop( inSession );
    
    EUNIT_ASSERT( iReceiveSession->iSession );
    EUNIT_ASSERT( iReceiveSession->iSession != inSession );
    EUNIT_ASSERT( iReceiveSession->iSession->iState == 
                  CMceSession::EReserving );
                               
    // Try again when there is session pending, new session gets terminated
    // and deleted
    inSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingSession( inSession, &container );    
    CleanupStack::Pop( inSession );
    
    EUNIT_ASSERT( iReceiveSession->iSession );
    EUNIT_ASSERT( iReceiveSession->iSession != inSession );
    EUNIT_ASSERT( iReceiveSession->iSession->iState == 
                  CMceSession::EReserving );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_CompleteSessionStructureL_WithoutSessionL()
    {
    TRAPD( error, iReceiveSession->CompleteSessionStructureL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_CompleteSessionStructureL_WithoutStreamsL()
    {
    CMceInSession* inSession = ConstructInSessionLC( 0, 0 );
    iReceiveSession->iSession = inSession; // Transfers ownership
    CleanupStack::Pop( inSession );
    
    TRAPD( error, iReceiveSession->CompleteSessionStructureL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrCorrupt );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_CompleteSessionStructureL_WithOutgoingStreamL()
    {    
    // Stream will be removed.
    CMceInSession* inSession = ConstructInSessionLC( 0, 0 );
    
    inSession->AddStreamL( ConstructVideoOutStreamLC() );
    CleanupStack::Pop();
    
    iReceiveSession->iSession = inSession; // Transfers ownership
    CleanupStack::Pop( inSession );
    
    TRAPD( error, iReceiveSession->CompleteSessionStructureL() )
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrCorrupt );
    EUNIT_ASSERT( inSession->Streams().Count() == 0 );
    EUNIT_ASSERT( inSession->Bundles().Count() == 0 );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_CompleteSessionStructureL_WithVideoInL()
    {
    CMceInSession* inSession = ConstructInSessionLC( 1, 0 );

    iReceiveSession->iSession = inSession; // Transfers ownership
    CleanupStack::Pop( inSession );

    iReceiveSession->CompleteSessionStructureL();

    EUNIT_ASSERT( inSession->Streams().Count() == 1 );
    EUNIT_ASSERT( inSession->Streams()[0]->IsEnabled() )
    EUNIT_ASSERT( inSession->Streams()[0]->Type() == KMceVideo )
    EUNIT_ASSERT( inSession->Streams()[0]->Sinks().Count() == 1 )
    EUNIT_ASSERT( inSession->Streams()[0]->Sinks()[0]->Type() == 
                  KMceDisplaySink )
    
    // No audio, bundle is not constructed
    EUNIT_ASSERT( inSession->Bundles().Count() == 0 )

    }


// -----------------------------------------------------------------------------
// Two-way video in incoming session. Incoming stream is the main stream.
// Both directions are enabled. Local viewfinder stream is added to display
// outgoing video locally
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_CompleteSessionStructureL_WithVideoOutBoundToVideoInL()
    {    
    CMceInSession* inSession = ConstructInSessionLC( 1, 0 );

    CMceVideoStream* videoOut = ConstructVideoOutStreamLC();
    inSession->Streams()[0]->BindL( videoOut );
    CleanupStack::Pop( videoOut );
    
    iReceiveSession->iSession = inSession; // Transfers ownership
    CleanupStack::Pop( inSession );

    iReceiveSession->CompleteSessionStructureL();

    EUNIT_ASSERT( inSession->Streams().Count() == 2 );

    CMceMediaStream* videoIn = inSession->Streams()[0];
     
    EUNIT_ASSERT( videoIn->IsEnabled() )
    EUNIT_ASSERT( videoIn->Type() == KMceVideo )
    EUNIT_ASSERT( videoIn->Sinks().Count() == 1 )
    EUNIT_ASSERT( videoIn->Sinks()[0]->Type() == KMceDisplaySink )

    EUNIT_ASSERT( videoIn->BoundStream() )
    EUNIT_ASSERT( videoIn->BoundStreamL().IsEnabled() )    
    
    CMceMediaStream* videoVf = inSession->Streams()[1];
    EUNIT_ASSERT( videoVf->IsEnabled() )
    EUNIT_ASSERT( videoVf->Type() == KMceVideo )
    EUNIT_ASSERT( videoVf->Sinks().Count() == 1 )
    EUNIT_ASSERT( videoVf->Sinks()[0]->Type() == KMceDisplaySink )
    EUNIT_ASSERT( videoVf->Source()->Type() == KMceCameraSource )
  
    // No audio, bundle is not constructed
    EUNIT_ASSERT( inSession->Bundles().Count() == 0 )
    }


// -----------------------------------------------------------------------------
// Two-way video in incoming session. Incoming stream is the bound stream.
// Both directions are enabled. Local viewfinder stream is added to display
// outgoing video locally
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_CompleteSessionStructureL_WithVideoInBoundToVideoOutL()
    {
    CMceInSession* inSession = ConstructInSessionLC( 0, 0 );

    CMceVideoStream* videoOut = ConstructVideoOutStreamLC();

    CMceVideoStream* videoIn = ConstructVideoInStreamLC();
    videoOut->BindL( videoIn );
    CleanupStack::Pop( videoIn );
    
    inSession->AddStreamL( videoOut );
    CleanupStack::Pop( videoOut );
    
    iReceiveSession->iSession = inSession; // Transfers ownership
    CleanupStack::Pop( inSession );

    iReceiveSession->CompleteSessionStructureL();

    EUNIT_ASSERT( inSession->Streams().Count() == 2 );
    EUNIT_ASSERT( videoOut->IsEnabled() )
    EUNIT_ASSERT( videoOut->Type() == KMceVideo )
    EUNIT_ASSERT( videoOut->Sinks().Count() == 1 )
    EUNIT_ASSERT( videoOut->Sinks()[0]->Type() == KMceRTPSink )
    EUNIT_ASSERT( videoOut->Sinks()[0]->IsEnabled() )

    EUNIT_ASSERT( videoOut->BoundStream() )
    EUNIT_ASSERT( videoOut->BoundStreamL().IsEnabled() )
    EUNIT_ASSERT( videoOut->BoundStreamL().Type() == KMceVideo )
    EUNIT_ASSERT( videoOut->BoundStreamL().Sinks().Count() == 1 )
    EUNIT_ASSERT( videoOut->BoundStreamL().Sinks()[0]->Type() == 
                  KMceDisplaySink )   
    EUNIT_ASSERT( videoOut->BoundStreamL().Sinks()[0]->IsEnabled() )
    
    CMceMediaStream* videoVf = inSession->Streams()[1];
    EUNIT_ASSERT( videoVf->IsEnabled() )
    EUNIT_ASSERT( videoVf->Type() == KMceVideo )
    EUNIT_ASSERT( videoVf->Sinks().Count() == 1 )
    EUNIT_ASSERT( videoVf->Sinks()[0]->Type() == KMceDisplaySink )
    EUNIT_ASSERT( videoVf->Source()->Type() == KMceCameraSource )
    
    // No audio, bundle is not constructed
    EUNIT_ASSERT( inSession->Bundles().Count() == 0 )
    }


// -----------------------------------------------------------------------------
// Construct 3 video instreams:
// 0. Without bound stream
// 1. Bounds video out stream
// 2. Bounded by video out stream
// Bound stream should be selected and vf stream is added
// -----------------------------------------------------------------------------
//    
void UT_CMusEngTwoWayRecvSession::UT_CompleteSessionStructureL_WithMultipleVideoInsL()
    {
    // construct session and stream 0
    CMceInSession* inSession = ConstructInSessionLC( 2, 0 );
    
    // finish constructing stream 1
    CMceVideoStream* videoOut = ConstructVideoOutStreamLC();
    inSession->Streams()[1]->BindL( videoOut );
    CleanupStack::Pop( videoOut );

    // finish constructing stream 2
    videoOut = ConstructVideoOutStreamLC();

    CMceVideoStream* videoIn = ConstructVideoInStreamLC();
    videoOut->BindL( videoIn );
    CleanupStack::Pop( videoIn );
    
    inSession->AddStreamL( videoOut );
    CleanupStack::Pop( videoOut );
    
    // Transfers ownership
    iReceiveSession->iSession = inSession; 
    CleanupStack::Pop( inSession );
    
    // complete
    iReceiveSession->CompleteSessionStructureL();
    
    // Bound stream should be used, other original streams should be deleted
    // but vf stream is added
    EUNIT_ASSERT( inSession->Streams().Count() == 2 );
    
    // First stream
    EUNIT_ASSERT( inSession->Streams()[0]->IsEnabled() )
    EUNIT_ASSERT( inSession->Streams()[0]->Type() == KMceVideo )
    EUNIT_ASSERT( inSession->Streams()[0]->Sinks().Count() == 1 )
    EUNIT_ASSERT( inSession->Streams()[0]->Sinks()[0]->Type() == KMceDisplaySink )
    EUNIT_ASSERT( inSession->Streams()[0]->Sinks()[0]->IsEnabled() )
    EUNIT_ASSERT( inSession->Streams()[0]->BoundStream() )
    EUNIT_ASSERT( inSession->Streams()[0]->BoundStreamL().Type() == KMceVideo )
    
    CMceMediaStream* videoVf = inSession->Streams()[1];
    EUNIT_ASSERT( videoVf->IsEnabled() )
    EUNIT_ASSERT( videoVf->Type() == KMceVideo )
    EUNIT_ASSERT( videoVf->Sinks().Count() == 1 )
    EUNIT_ASSERT( videoVf->Sinks()[0]->Type() == KMceDisplaySink )
    EUNIT_ASSERT( videoVf->Source()->Type() == KMceCameraSource )

    // No audio, bundle is not constructed
    EUNIT_ASSERT( inSession->Bundles().Count() == 0 )
    }


// -----------------------------------------------------------------------------
// Streams in this test case
// 0. Video in
// 1. Audio in
// 2. Audio out bound to audio in
// 3. Audio in bound to audio out
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::
                UT_CompleteSessionStructureL_WithVideoAndMultipleAudioInsL()
    {
    // Construct session and streams 0 and 1
    CMceInSession* inSession = inSession = ConstructInSessionLC( 1, 2 );
    
    // Construct stream 2
    CMceAudioStream* audioOut = ConstructAudioOutStreamLC();
    inSession->Streams()[2]->BindL( audioOut );
    CleanupStack::Pop( audioOut );
    
    // Construct stream 3
    
    audioOut = ConstructAudioOutStreamLC();
    
    CMceAudioStream* audioIn = ConstructAudioInStreamLC();
    audioOut->BindL( audioIn );
    CleanupStack::Pop( audioIn );
    
    inSession->AddStreamL( audioOut );
    CleanupStack::Pop( audioOut );
    
    // Transfers ownership
    iReceiveSession->iSession = inSession; 
    CleanupStack::Pop( inSession );

    // Complete
    iReceiveSession->CompleteSessionStructureL();
    EUNIT_ASSERT( inSession->Streams().Count() == 4 );
    
    // Stream 0
    EUNIT_ASSERT( inSession->Streams()[0]->Type() == KMceVideo )
    EUNIT_ASSERT( inSession->Streams()[0]->IsEnabled() )
    EUNIT_ASSERT( inSession->Streams()[0]->Sinks().Count() == 1 )
    EUNIT_ASSERT( inSession->Streams()[0]->Sinks()[0]->Type() == 
                  KMceDisplaySink )
    
    // Stream 1
    EUNIT_ASSERT( inSession->Streams()[1]->Type() == KMceAudio )
    EUNIT_ASSERT( inSession->Streams()[1]->IsEnabled() )
    EUNIT_ASSERT( inSession->Streams()[1]->Sinks().Count() == 1 )
    EUNIT_ASSERT( inSession->Streams()[1]->Sinks()[0]->Type() == 
                  KMceSpeakerSink );
    CMceSpeakerSink* speaker = 
        static_cast<CMceSpeakerSink*>(inSession->Streams()[1]->Sinks()[0]);
    EUNIT_ASSERT( speaker->VolumeL() == iReceiveSession->LcVolumeL() )
    
    // Stream 2 and bound stream
    EUNIT_ASSERT( inSession->Streams()[2]->Type() == KMceAudio )
    EUNIT_ASSERT( inSession->Streams()[2]->IsEnabled() )
    EUNIT_ASSERT( inSession->Streams()[2]->Sinks().Count() == 1 )
    EUNIT_ASSERT( inSession->Streams()[2]->Sinks()[0]->Type() == 
                  KMceSpeakerSink );
    speaker = static_cast<CMceSpeakerSink*>(inSession->Streams()[2]->Sinks()[0]);
    EUNIT_ASSERT( speaker->VolumeL() == iReceiveSession->LcVolumeL() )

    EUNIT_ASSERT( inSession->Streams()[2]->BoundStream() )
    EUNIT_ASSERT( !inSession->Streams()[2]->BoundStreamL().IsEnabled() )
    EUNIT_ASSERT( inSession->Streams()[2]->BoundStreamL().Type() == KMceAudio )
    
    EUNIT_ASSERT( inSession->Streams()[2]->BoundStreamL().Sinks().Count() == 1 )
    EUNIT_ASSERT( inSession->Streams()[2]->BoundStreamL().Sinks()[0]->Type() == 
                  KMceRTPSink );
    
    // Stream 3 and bound stream
    EUNIT_ASSERT( inSession->Streams()[3]->Type() == KMceAudio )
    EUNIT_ASSERT( !inSession->Streams()[3]->IsEnabled() )
    EUNIT_ASSERT( inSession->Streams()[3]->Sinks().Count() == 1 )
    EUNIT_ASSERT( inSession->Streams()[3]->Sinks()[0]->Type() == KMceRTPSink )
    EUNIT_ASSERT( !inSession->Streams()[3]->Sinks()[0]->IsEnabled() )

    EUNIT_ASSERT( inSession->Streams()[3]->BoundStream() )
    EUNIT_ASSERT( inSession->Streams()[3]->BoundStreamL().IsEnabled() )
    EUNIT_ASSERT( inSession->Streams()[3]->BoundStreamL().Type() == KMceAudio )
    EUNIT_ASSERT( inSession->Streams()[3]->BoundStreamL().Sinks().Count() == 1 )    
    EUNIT_ASSERT( inSession->Streams()[3]->BoundStreamL().Sinks()[0]->Type() == 
                  KMceSpeakerSink )
    speaker = static_cast<CMceSpeakerSink*>
                        (inSession->Streams()[3]->BoundStreamL().Sinks()[0]);
    EUNIT_ASSERT( speaker->VolumeL() == iReceiveSession->LcVolumeL() )
              
    // Check for bundles
    EUNIT_ASSERT( inSession->Bundles().Count() == 1 )
    EUNIT_ASSERT( inSession->Bundles()[0]->Streams().Count() == 4 )
    EUNIT_ASSERT( inSession->Bundles()[0]->Streams()[0]->Type() == KMceAudio  )              
    EUNIT_ASSERT( inSession->Bundles()[0]->Streams()[0]->Sinks()[0]->Type() == 
                  KMceSpeakerSink  )
    EUNIT_ASSERT( inSession->Bundles()[0]->Streams()[1]->Type() == KMceAudio  )              
    EUNIT_ASSERT( inSession->Bundles()[0]->Streams()[1]->Sinks()[0]->Type() == 
                  KMceSpeakerSink  )
    EUNIT_ASSERT( inSession->Bundles()[0]->Streams()[2]->Type() == KMceAudio  )              
    EUNIT_ASSERT( inSession->Bundles()[0]->Streams()[2]->Sinks()[0]->Type() == 
                  KMceSpeakerSink  )
    EUNIT_ASSERT( inSession->Bundles()[0]->Streams()[3]->Type() == KMceVideo )
    EUNIT_ASSERT( inSession->Bundles()[0]->Streams()[3]->Sinks()[0]->Type() == 
                  KMceDisplaySink )
    }


// -----------------------------------------------------------------------------
// Streams in this test case
// 0. Video in
// 1. Audio in
// 2. Audio out bound to audio in
// 3. Audio in bound to audio out
// All audio will be removed because of operator variant
// -----------------------------------------------------------------------------
//
void UT_CMusEngTwoWayRecvSession::UT_CompleteSessionStructureL_OperatorVariant()
    {  
    iReceiveSession->iOperatorVariant = ETrue;
    
    // Construct session and streams 0 and 1
    CMceInSession* inSession = inSession = ConstructInSessionLC( 1, 2 );
    
    // Construct stream 2
    CMceAudioStream* audioOut = ConstructAudioOutStreamLC();
    inSession->Streams()[2]->BindL( audioOut );
    CleanupStack::Pop( audioOut );
    
    // Construct stream 3
    
    audioOut = ConstructAudioOutStreamLC();
    
    CMceAudioStream* audioIn = ConstructAudioInStreamLC();
    audioOut->BindL( audioIn );
    CleanupStack::Pop( audioIn );
    
    inSession->AddStreamL( audioOut );
    CleanupStack::Pop( audioOut );
    
    // Transfers ownership
    iReceiveSession->iSession = inSession; 
    CleanupStack::Pop( inSession );

    // Complete
    iReceiveSession->CompleteSessionStructureL();
    
    // Check that all audio has been removed and no  bundles has been
    // constructed
    EUNIT_ASSERT_EQUALS( inSession->Streams().Count(), 1 )
    EUNIT_ASSERT_EQUALS( inSession->Streams()[0]->Type(), KMceVideo )
    EUNIT_ASSERT_EQUALS( inSession->Bundles().Count(), 0 )
    
    
    // Session with bandwidth attribute at sessionlevel
    // -> bandwidth is taken in use at session level
    //
    delete iReceiveSession->iSession;
    iReceiveSession->iSession = NULL;
    
    inSession = ConstructInSessionLC( 1, 0 );
    CDesC8Array* sdpLines = new ( ELeave ) CDesC8ArrayFlat( 1 );
    CleanupStack::PushL( sdpLines );
    sdpLines->AppendL( KMusEngSessionSdpLineBandwidthField() );
    inSession->SetSessionSDPLinesL( sdpLines );
    CleanupStack::Pop( sdpLines );
    
    // Transfers ownership
    iReceiveSession->iSession = inSession; 
    CleanupStack::Pop( inSession );
    
    iReceiveSession->CompleteSessionStructureL();
    
    MDesC8Array* newSdpLines = iReceiveSession->iSession->SessionSDPLinesL();
    CleanupDeletePushL( newSdpLines );
    TBool bandwidthFoundFromSessionLevel( EFalse );
    for ( TInt i = 0; newSdpLines && i < newSdpLines->MdcaCount(); i++ )
        {
        if ( newSdpLines->MdcaPoint( i ).Compare( 
                KMusEngSessionSdpLineBandwidthField() ) == 0 )
            {
            bandwidthFoundFromSessionLevel = ETrue;
            }
        }
    EUNIT_ASSERT( bandwidthFoundFromSessionLevel );
    CleanupStack::PopAndDestroy( newSdpLines );
    
    newSdpLines = iReceiveSession->iSession->Streams()[ 0 ]->MediaAttributeLinesL();
    CleanupDeletePushL( newSdpLines );
    TBool bandwidthFoundFromMediaLevel( EFalse );
    for ( TInt i = 0; newSdpLines && i < newSdpLines->MdcaCount(); i++ )
        {
        if ( newSdpLines->MdcaPoint( i ).Compare( 
                KMusEngSessionSdpLineBandwidthField() ) == 0 )
            {
            bandwidthFoundFromMediaLevel = ETrue;
            }
        }
    EUNIT_ASSERT( !bandwidthFoundFromMediaLevel );
    CleanupStack::PopAndDestroy( newSdpLines );
    
    // Session with bandwidth attribute at media level
    // -> bandwidth is taken in use at media level
    //
    delete iReceiveSession->iSession;
    iReceiveSession->iSession = NULL;
    
    inSession = ConstructInSessionLC( 1, 0 );
    
    sdpLines = new ( ELeave ) CDesC8ArrayFlat( 1 );
    CleanupStack::PushL( sdpLines );
    sdpLines->AppendL( KMusEngSessionSdpLineBandwidthField() );
    inSession->Streams()[ 0 ]->SetMediaAttributeLinesL( sdpLines );
    CleanupStack::Pop( sdpLines );
    
    // Transfers ownership
    iReceiveSession->iSession = inSession; 
    CleanupStack::Pop( inSession );
    
    iReceiveSession->CompleteSessionStructureL();
    
    newSdpLines = iReceiveSession->iSession->SessionSDPLinesL();
    CleanupDeletePushL( newSdpLines );
    bandwidthFoundFromSessionLevel = EFalse;
    for ( TInt i = 0; newSdpLines && i < newSdpLines->MdcaCount(); i++ )
        {
        if ( newSdpLines->MdcaPoint( i ).Compare( 
                KMusEngSessionSdpLineBandwidthField() ) == 0 )
            {
            bandwidthFoundFromSessionLevel = ETrue;
            }
        }
    EUNIT_ASSERT( !bandwidthFoundFromSessionLevel );
    CleanupStack::PopAndDestroy( newSdpLines );
    
    newSdpLines = iReceiveSession->iSession->Streams()[ 0 ]->MediaAttributeLinesL();
    CleanupDeletePushL( newSdpLines );
    bandwidthFoundFromMediaLevel = EFalse;
    for ( TInt i = 0; newSdpLines && i < newSdpLines->MdcaCount(); i++ )
        {
        if ( newSdpLines->MdcaPoint( i ).Compare( 
                KMusEngSessionSdpLineBandwidthField() ) == 0 )
            {
            bandwidthFoundFromMediaLevel = ETrue;
            }
        }
    EUNIT_ASSERT( bandwidthFoundFromMediaLevel );
    CleanupStack::PopAndDestroy( newSdpLines );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngTwoWayRecvSession,
    "UT_CMusEngTwoWayRecvSession",
    "UNIT" )

    
EUNIT_TEST(
    "NewL - test ",
    "CMusEngTwoWayRecvSession",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewLL, Teardown)
        
EUNIT_TEST(
    "EnableDisplayL - test ",
    "CMusEngTwoWayRecvSession",
    "EnableDisplayL",
    "FUNCTIONALITY",
    SetupL, UT_EnableDisplayLL, Teardown)   
    
EUNIT_TEST(
    "SetOrientationL - test ",
    "CMusEngTwoWayRecvSession",
    "SetOrientationL",
    "FUNCTIONALITY",
    SetupL, UT_SetOrientationLL, Teardown)

EUNIT_TEST(
    "OrientationL - test ",
    "CMusEngTwoWayRecvSession",
    "OrientationL",
    "FUNCTIONALITY",
    SetupL, UT_OrientationLL, Teardown)
    
EUNIT_TEST(
    "SetLocalRectL - test ",
    "CMusEngTwoWayRecvSession",
    "SetLocalRectL",
    "FUNCTIONALITY",
    SetupL, UT_SetLocalRectLL, Teardown)
    
EUNIT_TEST(
    "SetRectsL - test ",
    "CMusEngTwoWayRecvSession",
    "SetRectsL",
    "FUNCTIONALITY",
    SetupL, UT_SetRectsLL, Teardown)

EUNIT_TEST(
    "Camera - test ",
    "CMusEngTwoWayRecvSession",
    "Camera",
    "FUNCTIONALITY",
    SetupL, UT_CameraL, Teardown)
    
EUNIT_TEST(
    "LocalVideoPlayer - test ",
    "CMusEngTwoWayRecvSession",
    "LocalVideoPlayer",
    "FUNCTIONALITY",
    SetupL, UT_LocalVideoPlayerL, Teardown)

EUNIT_TEST(
    "HandleSessionStateChanged - test ",
    "CMusEngTwoWayRecvSession",
    "HandleSessionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_HandleSessionStateChangedL, Teardown)

EUNIT_TEST(
    "AdjustVideoCodecL - test ",
    "CMusEngTwoWayRecvSession",
    "AdjustVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustVideoCodecLL, Teardown)

EUNIT_TEST(
    "AdjustAudioCodecL - test ",
    "CMusEngTwoWayRecvSession",
    "AdjustAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustAudioCodecLL, Teardown)

EUNIT_TEST(
    "DoCodecSelectionL - test ",
    "CMusEngTwoWayRecvSession",
    "DoCodecSelectionL",
    "FUNCTIONALITY",
    SetupL, UT_DoCodecSelectionLL, Teardown)

EUNIT_TEST(
    "IncomingSession - test ",
    "CMusEngTwoWayRecvSession",
    "IncomingSession",
    "FUNCTIONALITY",
    SetupL, UT_IncomingSessionL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithoutSessionL - test without a session",
    "CMusEngTwoWayRecvSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithoutSessionL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithoutStreamsL - without streams",
    "CMusEngTwoWayRecvSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithoutStreamsL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithOutgoingStreamL - with outgoing stream",
    "CMusEngTwoWayRecvSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithOutgoingStreamL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithVideoInL - with video in stream",
    "CMusEngTwoWayRecvSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithVideoInL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithVideoOutBoundToVideoInL - with sendrecv video, video out bound to video in",
    "CMusEngTwoWayRecvSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithVideoOutBoundToVideoInL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithVideoInBoundToVideoOutL - with sendrecv video, video in bound to video out",
    "CMusEngTwoWayRecvSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithVideoInBoundToVideoOutL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithMultipleVideoInsL - with multiple video in streams",
    "CMusEngTwoWayRecvSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithMultipleVideoInsL, Teardown)
        
EUNIT_TEST(
    "CompleteSessionStructureL - test one video and multiple  audio streams",
    "CMusEngTwoWayRecvSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithVideoAndMultipleAudioInsL, Teardown)
    
EUNIT_TEST(
    "CompleteSessionStructureL - test operator variant",
    "CMusEngTwoWayRecvSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_OperatorVariant, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE


