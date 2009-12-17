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
#include "ut_musengreceivesession.h"
#include "musengstubs.h"
#include "musengtestdefs.h"
#include "musengreceivesession.h"
#include "centralrepository.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>

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

#include <audiopreference.h>




// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngReceiveSession* UT_CMusEngReceiveSession::NewL()
    {
    UT_CMusEngReceiveSession* self = UT_CMusEngReceiveSession::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
UT_CMusEngReceiveSession* UT_CMusEngReceiveSession::NewLC()
    {
    UT_CMusEngReceiveSession* self = new( ELeave ) UT_CMusEngReceiveSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngReceiveSession::~UT_CMusEngReceiveSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngReceiveSession::UT_CMusEngReceiveSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngReceiveSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngReceiveSession::SetupL(  )
    {
    CRepository::iStaticEncoderUid = 0;
    iObserver = new( ELeave ) CMusEngObserverStub;
    iReceiveSession = CMusEngReceiveSession::NewL( TRect(0,0, 100,100), 
                                                   *iObserver,
                                                   *iObserver );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngReceiveSession::Teardown(  )
    {
    delete iReceiveSession;
    delete iObserver;
    CRepository::iStaticEncoderUid = 0;
    
    }


// TEST HELPERS

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMceVideoStream* UT_CMusEngReceiveSession::ConstructVideoInStreamLC()
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
CMceVideoStream* UT_CMusEngReceiveSession::ConstructVideoOutStreamLC()
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
CMceAudioStream* UT_CMusEngReceiveSession::ConstructAudioInStreamLC()
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
CMceAudioStream* UT_CMusEngReceiveSession::ConstructAudioOutStreamLC()
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
CMceInSession* UT_CMusEngReceiveSession::ConstructInSessionLC( 
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
                                             
                                             


// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngReceiveSession::UT_NewLL(  )
    {
    EUNIT_ASSERT( iReceiveSession );
    EUNIT_ASSERT( !iReceiveSession->iSession );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngReceiveSession::UT_AcceptInvitationLL()
    {
    TMceTransactionDataContainer container;
    
    // There is no pending session to accept, must fail
    TRAPD( error, iReceiveSession->AcceptInvitationL( ETrue ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );

    // There is pending session, but it is not yet reserved resources,
    // so accepting fails
    CMceInSession* inSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    
    TRAP( error, iReceiveSession->AcceptInvitationL( ETrue ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Simulate resource reservation and reject
    iReceiveSession->iSession->iState = CMceSession::EProceeding;
    iReceiveSession->AcceptInvitationL( EFalse );
    
    EUNIT_ASSERT( iReceiveSession->iSession->iState ==
                  CMceSession::ETerminated );
    
    // Construct new session and reject it with operator variant
    inSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    iReceiveSession->iSession->iState = CMceSession::EProceeding;
    iReceiveSession->iOperatorVariant = ETrue;
    
    iReceiveSession->AcceptInvitationL( EFalse );
    
    EUNIT_ASSERT( iReceiveSession->iSession->iState ==
                  CMceSession::ETerminated );

    // Construct new session and accept it
    inSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    iReceiveSession->iSession->iState = CMceSession::EProceeding;

    iReceiveSession->AcceptInvitationL( ETrue );   
    
    EUNIT_ASSERT( iReceiveSession->iSession );
    EUNIT_ASSERT( iReceiveSession->iSession == inSession );
    EUNIT_ASSERT( iReceiveSession->iSession->iState == 
                  CMceSession::EAnswering );
    
    
    }    


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngReceiveSession::UT_HandleSessionStateChangedL()
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
    EUNIT_ASSERT( iObserver->iIncomingSessionCalled )
    EUNIT_ASSERT( iReceiveSession->iRingLCalled )
    iObserver->Reset();
    
    // 2 ) Now simulate second transition to EProceeding state which can happen
    //     if we force 100rel to be used. User and remote end should not be 
    //     notified anymore but change should be ignored.
    iReceiveSession->HandleSessionStateChanged( *inSession, 0, KNullDesC8() );
    EUNIT_ASSERT( iObserver->IsReseted() )
    iObserver->Reset();
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusEngReceiveSession::UT_AdjustVideoCodecLL()
    {
    CMceVideoCodec* codecH263 = CMceH263Codec::NewLC( KMceSDPNameH263() );    
    CMceVideoCodec* codecH2632000 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );
    CMceVideoCodec* codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    CMceVideoCodec* codecUnknown = CMceH263Codec::NewLC( KNullDesC8() );
    
    iReceiveSession->AdjustVideoCodecL( *codecH263 );
    iReceiveSession->AdjustVideoCodecL( *codecH2632000 );
    iReceiveSession->AdjustVideoCodecL( *codecAvc );
    iReceiveSession->AdjustVideoCodecL( *codecUnknown );
    
    EUNIT_ASSERT_EQUALS( codecH263->KeepAliveTimer(), 5 )
    EUNIT_ASSERT_EQUALS( codecH263->KeepAlivePayloadType(), 96 )
    EUNIT_ASSERT_EQUALS( codecH263->KeepAliveData(), KNullDesC8() )
    
    EUNIT_ASSERT_EQUALS( codecH2632000->KeepAliveTimer(), 5 )
    EUNIT_ASSERT_EQUALS( codecH2632000->KeepAlivePayloadType(), 96 )
    EUNIT_ASSERT_EQUALS( codecH2632000->KeepAliveData(), KNullDesC8() )
    
    EUNIT_ASSERT_EQUALS( codecAvc->KeepAliveTimer(), 5 )
    EUNIT_ASSERT_EQUALS( codecAvc->KeepAlivePayloadType(), 98 )
    EUNIT_ASSERT_EQUALS( codecAvc->KeepAliveData(), KNullDesC8() )
    
    EUNIT_ASSERT_EQUALS( codecUnknown->KeepAliveTimer(), 5 )
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
void UT_CMusEngReceiveSession::UT_AdjustAudioCodecLL()
    {
    CMceAudioCodec* codec = 
                iReceiveSession->iManager->SupportedAudioCodecs()[0]->CloneL();
    CleanupStack::PushL( codec );
    iReceiveSession->AdjustAudioCodecL( *codec );
    
    // Test keepalive timer
    
    TUint8 keepAliveTimer = codec->KeepAliveTimer();
    EUNIT_ASSERT( codec->KeepAliveTimer() == 5 )
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
void UT_CMusEngReceiveSession::UT_DoCodecSelectionLL()
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
void UT_CMusEngReceiveSession::UT_IncomingSessionL()
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
    
    EUNIT_ASSERT( iObserver->iIncomingSessionPreNotificationCalled )
    
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
    
    // Try again when there is usage of avc is turned off
    iReceiveSession->iSession->iState = CMceSession::ETerminated;
    TInt32 KMusDisableAVC = 0x0fffffff;
    CRepository::iStaticEncoderUid = KMusDisableAVC;

    inSession = ConstructInSessionLC( 1, 0 );
    
    // There will 3 AVC codecs and 1 H263 codec
    EUNIT_ASSERT( static_cast<CMceVideoStream*>( inSession->Streams()[0] )->Codecs().Count() == 4 );
    iReceiveSession->IncomingSession( inSession, &container );    
    CleanupStack::Pop( inSession );
    
    EUNIT_ASSERT( iReceiveSession->iSession->iState == 
                  CMceSession::EReserving ||
                  iReceiveSession->iSession->iState == 
                  CMceSession::ETerminated );
    
    // Should be only the H263 codec
    EUNIT_ASSERT( static_cast<CMceVideoStream*>( iReceiveSession->iSession->Streams()[0] )->Codecs().Count() <= 4 );
    
    //No supported codecs
    iReceiveSession->iSession->iState = CMceSession::ETerminated;
    iReceiveSession->iManager->iSupportedVideoCodecs.ResetAndDestroy();
    inSession = ConstructInSessionLC( 1, 0 );
    
    // There will no codecs, simulates situation where no codecs were match
    //currently session is not been rejected
    EUNIT_ASSERT( static_cast<CMceVideoStream*>( inSession->Streams()[0] )->Codecs().Count() == 0 );
    iReceiveSession->IncomingSession( inSession, &container );    
    CleanupStack::Pop( inSession );
    
    EUNIT_ASSERT( iReceiveSession->iSession->iState == 
                  CMceSession::EReserving ||
                  iReceiveSession->iSession->iState == 
                  CMceSession::ETerminated );
    
    // Should be only the H263 codec
    EUNIT_ASSERT( static_cast<CMceVideoStream*>( inSession->Streams()[0] )->Codecs().Count() == 0 );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngReceiveSession::UT_IncomingUpdateL()
    {
    TMceTransactionDataContainer container;
    
    // We do not have session and thus cannot have an update, new session gets
    // rejected and deleted.
    CMceInSession* oldSession = ConstructInSessionLC( 0, 0 );
    CMceInSession* newSession = ConstructInSessionLC( 0, 0 );
    iReceiveSession->IncomingUpdate( *oldSession, newSession, NULL );
    CleanupStack::Pop( newSession );
    CleanupStack::PopAndDestroy( oldSession );
    
    // Start establishing a session
    CMceInSession* inSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingSession( inSession, &container );
    CleanupStack::Pop( inSession );
    
    iReceiveSession->iSession->iState = CMceSession::EProceeding;
    iReceiveSession->AcceptInvitationL( ETrue );
    
    // Now we have an insession, try to update session that is not ours, 
    // new one gets rejected and deleted.
    oldSession = ConstructInSessionLC( 0, 0 );
    newSession = ConstructInSessionLC( 0, 0 );
    iReceiveSession->IncomingUpdate( *oldSession, newSession, NULL );
    CleanupStack::Pop( newSession );
    CleanupStack::PopAndDestroy( oldSession );
    
    // Try to update our insession before it is established. New session gets
    // rejected and deleted.
    newSession = ConstructInSessionLC( 0, 0 );
    iReceiveSession->IncomingUpdate( *iReceiveSession->iSession, 
                                     newSession, 
                                     NULL );
    CleanupStack::Pop( newSession );
    
    // Finish establishing insession
    iReceiveSession->iSession->iState = CMceSession::EEstablished;
    
    // Try to update the established session with invalid session, it gets
    // rejected and deleted
    newSession = ConstructInSessionLC( 0, 0 );
    iReceiveSession->IncomingUpdate( *iReceiveSession->iSession, 
                                     newSession, 
                                     NULL );
    CleanupStack::Pop( newSession ); 
    
    // Succeed in update 
    newSession = ConstructInSessionLC( 1, 0 );
    iReceiveSession->IncomingUpdate( *iReceiveSession->iSession, 
                                     newSession, 
                                     NULL );
    CleanupStack::Pop( newSession ); 
    EUNIT_ASSERT( iReceiveSession->iSession );
    EUNIT_ASSERT( iReceiveSession->iSession == newSession );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusEngReceiveSession::UT_StreamStateChangedL()
    {
    TMceTransactionDataContainer container;
    
    CMceInSession* inSession = ConstructInSessionLC( 1, 1 );
    
    CMceMediaStream* videoStream = inSession->Streams()[0];
    CMceMediaStream* audioStream = inSession->Streams()[1];
    
    // Try without a session, nothing happens
    iReceiveSession->StreamStateChanged( *audioStream );
    
    // Establish session 
    
    iReceiveSession->IncomingSession( inSession, &container );    
    CleanupStack::Pop( inSession );
    iObserver->Reset();

    // Try with audiostream, nothing happens
    audioStream->iState = CMceMediaStream::EBuffering;
    iReceiveSession->StreamStateChanged( *audioStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // Try with videostream, but without a source, nothing happens 
        
        // Needed hacking
        CMceRtpSource* rtpSource = 
                        static_cast<CMceRtpSource*>( videoStream->Source() );
    videoStream->iSource = NULL;
    
    iReceiveSession->StreamStateChanged( *videoStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // Try with videostream and a camera source, nothing happens 
        
        // Needed hacking
        videoStream->iSource = 
                    CMceCameraSource::NewLC( *iReceiveSession->iManager );
        CleanupStack::Pop();
    
    iReceiveSession->StreamStateChanged( *videoStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // Remove hacks
    delete videoStream->iSource;
    videoStream->iSource = rtpSource;
        
    // Buffering, normal case
    videoStream->iState = CMceMediaStream::EBuffering;
    iReceiveSession->StreamStateChanged( *videoStream );
    EUNIT_ASSERT( iObserver->iStreamBufferingCalled )
    iObserver->Reset();
    
    // Streaming, normal case
    videoStream->iState = CMceMediaStream::EStreaming;
    iReceiveSession->StreamStateChanged( *videoStream );
    EUNIT_ASSERT( iObserver->iStreamStreamingCalled )
    EUNIT_ASSERT( rtpSource->iInactivityTimer == 2000 ) 
    iObserver->Reset();
    
    // Try default behaviors
    videoStream->iState = CMceMediaStream::EIdle;
    iReceiveSession->StreamStateChanged( *videoStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngReceiveSession::UT_CompleteSessionStructureL_WithoutSessionL()
    {
    TRAPD( error, iReceiveSession->CompleteSessionStructureL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngReceiveSession::UT_CompleteSessionStructureL_WithoutStreamsL()
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
void UT_CMusEngReceiveSession::UT_CompleteSessionStructureL_WithOutgoingStreamL()
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
void UT_CMusEngReceiveSession::UT_CompleteSessionStructureL_WithVideoInL()
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
// Video out is bound stream, it gets disabled
// -----------------------------------------------------------------------------
//
void UT_CMusEngReceiveSession::UT_CompleteSessionStructureL_WithVideoOutBoundToVideoInL()
    {    
    CMceInSession* inSession = ConstructInSessionLC( 1, 0 );

    CMceVideoStream* videoOut = ConstructVideoOutStreamLC();
    inSession->Streams()[0]->BindL( videoOut );
    CleanupStack::Pop( videoOut );
    
    iReceiveSession->iSession = inSession; // Transfers ownership
    CleanupStack::Pop( inSession );

    iReceiveSession->CompleteSessionStructureL();

    EUNIT_ASSERT( inSession->Streams().Count() == 1 );

    CMceMediaStream* videoIn = inSession->Streams()[0];
     
    EUNIT_ASSERT( videoIn->IsEnabled() )
    EUNIT_ASSERT( videoIn->Type() == KMceVideo )
    EUNIT_ASSERT( videoIn->Sinks().Count() == 1 )
    EUNIT_ASSERT( videoIn->Sinks()[0]->Type() == KMceDisplaySink )

    EUNIT_ASSERT( videoIn->BoundStream() )
    EUNIT_ASSERT( !videoIn->BoundStreamL().IsEnabled() )    
    
    // No audio, bundle is not constructed
    EUNIT_ASSERT( inSession->Bundles().Count() == 0 )
    }


// -----------------------------------------------------------------------------
// Video in is bound stream, another one gets disabled
// -----------------------------------------------------------------------------
//
void UT_CMusEngReceiveSession::UT_CompleteSessionStructureL_WithVideoInBoundToVideoOutL()
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

    EUNIT_ASSERT( inSession->Streams().Count() == 1 );
    EUNIT_ASSERT( !videoOut->IsEnabled() )
    EUNIT_ASSERT( videoOut->Type() == KMceVideo )
    EUNIT_ASSERT( videoOut->Sinks().Count() == 1 )
    EUNIT_ASSERT( videoOut->Sinks()[0]->Type() == KMceRTPSink )
    EUNIT_ASSERT( !videoOut->Sinks()[0]->IsEnabled() )

    EUNIT_ASSERT( videoOut->BoundStream() )
    EUNIT_ASSERT( videoOut->BoundStreamL().IsEnabled() )
    EUNIT_ASSERT( videoOut->BoundStreamL().Type() == KMceVideo )
    EUNIT_ASSERT( videoOut->BoundStreamL().Sinks().Count() == 1 )
    EUNIT_ASSERT( videoOut->BoundStreamL().Sinks()[0]->Type() == 
                  KMceDisplaySink )   
    EUNIT_ASSERT( videoOut->BoundStreamL().Sinks()[0]->IsEnabled() )
                  
    // No audio, bundle is not constructed
    EUNIT_ASSERT( inSession->Bundles().Count() == 0 )
    }


// -----------------------------------------------------------------------------
// Construct 3 video instreams:
// 0. Without bound stream
// 1. Bounds video out stream
// 2. Bounded by video out stream
// -----------------------------------------------------------------------------
//    
void UT_CMusEngReceiveSession::UT_CompleteSessionStructureL_WithMultipleVideoInsL()
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
    
    // First should be used, 4 other streams should be deleted
    
    EUNIT_ASSERT( inSession->Streams().Count() == 1 );
    
    // First stream
    EUNIT_ASSERT( inSession->Streams()[0]->IsEnabled() )
    EUNIT_ASSERT( inSession->Streams()[0]->Type() == KMceVideo )
    EUNIT_ASSERT( inSession->Streams()[0]->Sinks().Count() == 1 )
    EUNIT_ASSERT( inSession->Streams()[0]->Sinks()[0]->Type() == KMceDisplaySink )
    EUNIT_ASSERT( inSession->Streams()[0]->Sinks()[0]->IsEnabled() )

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
void UT_CMusEngReceiveSession::
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
    EUNIT_ASSERT( speaker->VolumeL() == iReceiveSession->VolumeL() )
    
    // Stream 2 and bound stream
    EUNIT_ASSERT( inSession->Streams()[2]->Type() == KMceAudio )
    EUNIT_ASSERT( inSession->Streams()[2]->IsEnabled() )
    EUNIT_ASSERT( inSession->Streams()[2]->Sinks().Count() == 1 )
    EUNIT_ASSERT( inSession->Streams()[2]->Sinks()[0]->Type() == 
                  KMceSpeakerSink );
    speaker = static_cast<CMceSpeakerSink*>(inSession->Streams()[2]->Sinks()[0]);
    EUNIT_ASSERT( speaker->VolumeL() == iReceiveSession->VolumeL() )

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
    EUNIT_ASSERT( speaker->VolumeL() == iReceiveSession->VolumeL() )
              
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
void UT_CMusEngReceiveSession::UT_CompleteSessionStructureL_OperatorVariant()
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
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusEngReceiveSession::UT_ParseAssertedIdentityL()
    {  
    _LIT8( KMusEngAssertedIdentitySip, "P-Asserted-Identity: sip:fluffy@cisco.com" );
    _LIT8( KMusEngIdentitySip, "fluffy" );
    _LIT8( KMusEngAssertedIdentityTel, "P-Asserted-Identity: tel:+14085264000" );
    _LIT8( KMusEngAssertedIdentityTelBrace, "P-Asserted-Identity: <tel:+14085264000>" );
    _LIT8( KMusEngIdentityTel, "+14085264000" );
    _LIT8( KMusEngTestHeader, "Some header" );
    
    _LIT8( KMusEngAssertedIdentityInvalid, "P-Asserted-Identity: sip:fluffy cisco.com" );

    TMceTransactionDataContainer container;
    CDesC8Array* headers;

    // only sip uri in asserted identity header 
    headers = new ( ELeave ) CDesC8ArrayFlat( 2 );
    CleanupStack::PushL( headers );
    headers->AppendL( KMusEngTestHeader );
    headers->AppendL( KMusEngAssertedIdentitySip );
    container.SetHeaders( headers );
    CleanupStack::Pop( headers );
    iReceiveSession->ParseAssertedIdentity( &container );    
    container.Clear(); 
    EUNIT_ASSERT_EQUALS( iReceiveSession->iIdentity, KMusEngIdentitySip )
    
    // sip && tel uris in asserted identity headers
    headers = new ( ELeave ) CDesC8ArrayFlat( 3 );
    CleanupStack::PushL( headers );
    headers->AppendL( KMusEngTestHeader );
    headers->AppendL( KMusEngAssertedIdentitySip );
    headers->AppendL( KMusEngAssertedIdentityTel );    
    container.SetHeaders( headers );
    CleanupStack::Pop( headers );
    iReceiveSession->ParseAssertedIdentity( &container );    
    container.Clear();  
    EUNIT_ASSERT_EQUALS( iReceiveSession->iIdentity, KMusEngIdentityTel )    
    
    // tel uri with braces in asserted identity header 
    headers = new ( ELeave ) CDesC8ArrayFlat( 2 );
    CleanupStack::PushL( headers );
    headers->AppendL( KMusEngTestHeader );
    headers->AppendL( KMusEngAssertedIdentityTelBrace );
    container.SetHeaders( headers );
    CleanupStack::Pop( headers );
    iReceiveSession->ParseAssertedIdentity( &container );    
    container.Clear(); 
    EUNIT_ASSERT_EQUALS( iReceiveSession->iIdentity, KMusEngIdentityTel )    
    
   // invalid uri in asserted identity header
    headers = new ( ELeave ) CDesC8ArrayFlat( 2 );
    CleanupStack::PushL( headers );
    headers->AppendL( KMusEngTestHeader );
    headers->AppendL( KMusEngAssertedIdentityInvalid );
    container.SetHeaders( headers );
    CleanupStack::Pop( headers );
    iReceiveSession->ParseAssertedIdentity( &container );    
    container.Clear(); 
    EUNIT_ASSERT_EQUALS( iReceiveSession->iIdentity, KNullDesC8 )    
    
    // null container
    iReceiveSession->ParseAssertedIdentity( NULL );
    EUNIT_ASSERT_EQUALS( iReceiveSession->iIdentity, KNullDesC8 )    
    
    // null header
    iReceiveSession->ParseAssertedIdentity( &container );
    EUNIT_ASSERT_EQUALS( iReceiveSession->iIdentity, KNullDesC8 )    
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngReceiveSession,
    "UT_CMusEngReceiveSesssion",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusEngReceiveSession",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewLL, Teardown)

EUNIT_TEST(
    "AcceptInvitationL - test ",
    "CMusEngReceiveSession",
    "AcceptInvitationL",
    "FUNCTIONALITY",
    SetupL, UT_AcceptInvitationLL, Teardown)

EUNIT_TEST(
    "HandleSessionStateChanged - test ",
    "CMusEngReceiveSession",
    "HandleSessionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_HandleSessionStateChangedL, Teardown)

EUNIT_TEST(
    "AdjustVideoCodecL - test ",
    "CMusEngReceiveSession",
    "AdjustVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustVideoCodecLL, Teardown)

EUNIT_TEST(
    "AdjustAudioCodecL - test ",
    "CMusEngReceiveSession",
    "AdjustAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustAudioCodecLL, Teardown)

EUNIT_TEST(
    "DoCodecSelectionL - test ",
    "CMusEngReceiveSession",
    "DoCodecSelectionL",
    "FUNCTIONALITY",
    SetupL, UT_DoCodecSelectionLL, Teardown)

EUNIT_TEST(
    "IncomingSession - test ",
    "CMusEngReceiveSession",
    "IncomingSession",
    "FUNCTIONALITY",
    SetupL, UT_IncomingSessionL, Teardown)

EUNIT_TEST(
    "IncomingUpdate - test ",
    "CMusEngReceiveSession",
    "IncomingUpdate",
    "FUNCTIONALITY",
    SetupL, UT_IncomingUpdateL, Teardown)

EUNIT_TEST(
    "StreamStateChanged - test ",
    "CMusEngReceiveSession",
    "StreamStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithoutSessionL - test without a session",
    "CMusEngReceiveSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithoutSessionL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithoutStreamsL - without streams",
    "CMusEngReceiveSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithoutStreamsL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithOutgoingStreamL - with outgoing stream",
    "CMusEngReceiveSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithOutgoingStreamL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithVideoInL - with video in stream",
    "CMusEngReceiveSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithVideoInL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithVideoOutBoundToVideoInL - with sendrecv video, video out bound to video in",
    "CMusEngReceiveSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithVideoOutBoundToVideoInL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithVideoInBoundToVideoOutL - with sendrecv video, video in bound to video out",
    "CMusEngReceiveSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithVideoInBoundToVideoOutL, Teardown)

EUNIT_TEST(
    "UT_CompleteSessionStructureL_WithMultipleVideoInsL - with multiple video in streams",
    "CMusEngReceiveSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithMultipleVideoInsL, Teardown)
        
EUNIT_TEST(
    "CompleteSessionStructureL - test one video and multiple  audio streams",
    "CMusEngReceiveSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_WithVideoAndMultipleAudioInsL, Teardown)
    
EUNIT_TEST(
    "CompleteSessionStructureL - test operator variant",
    "CMusEngReceiveSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureL_OperatorVariant, Teardown)
   
EUNIT_TEST(
    "ParseAssertedIdentity",
    "CMusEngReceiveSession",
    "ParseAssertedIdentity",
    "FUNCTIONALITY",
    SetupL, UT_ParseAssertedIdentityL, Teardown)
      
   
EUNIT_END_TEST_TABLE

//  END OF FILE


