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
#include "ut_musenglivesession.h"
#include "musengstubs.h"
#include "musengtestdefs.h"
#include "musenglivesession.h"
#include "mussipprofilehandler.h"
#include "musengmceutils.h"
#include "mussettings.h"
#include "mussessionproperties.h"
#include "musenglivevideoplayer.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <mceoutsession.h>
#include <mcestreambundle.h>
#include <mcevideostream.h>
#include <mcecamerasource.h>
#include <mcertpsink.h>
#include <mcefilesink.h>
#include <mceh263codec.h>
#include <mceavccodec.h>
#include <mceaudiocodec.h>



//use step in different time period
const TInt KZoomBigStepCount = 15;
const TInt KZoomSmallStepCount = KZoomBigStepCount*2;
const TInt KZoomStepMinSize = 1;

// 0.4 second
const TInt64 KZoomFasterTime = 400000;//must bigger than 1/3 second

//insure that step is 1
#define USER_SLEEP  User::After(KZoomFasterTime)



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngLiveSession* UT_CMusEngLiveSession::NewL()
    {
    UT_CMusEngLiveSession* self = UT_CMusEngLiveSession::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngLiveSession* UT_CMusEngLiveSession::NewLC()
    {
    UT_CMusEngLiveSession* self = new( ELeave ) UT_CMusEngLiveSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngLiveSession::~UT_CMusEngLiveSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngLiveSession::UT_CMusEngLiveSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::SetupL()
    {    
    iLcSessionObserver = new( ELeave )CLcSessionObserverStub;
    iLcUiProvider = new( ELeave )CLcUiProviderStub;
    iAudioRoutingObserver = new( ELeave )CMusEngObserverStub;    
    
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );
                                             
    iRecordedLiveSession = CMusEngLiveSession::NewL();
    iRecordedLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iRecordedLiveSession->SetLcUiProvider( iLcUiProvider );
    MLcDestinationFileControl* destinationFileControl =
        iRecordedLiveSession->LocalVideoPlayer()->LcDestinationFileControl();
    destinationFileControl->SetLcFileNameL( KTestVideoFileName() );
    
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KRemoteSipAddress,
                                        KTestRecipientSipUri ) );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::Teardown()
    {
    delete iLiveSession;
    delete iRecordedLiveSession;
    delete iLcSessionObserver;
    delete iLcUiProvider;
    delete iAudioRoutingObserver;
    PropertyHelper::Close();
    // Delete static data from CenRep stub
    CRepository::iStaticWriteAvcKeysToStaticData = EFalse;
    CRepository::DeleteStubAvcConfigKeys();
    CRepository::ResetStubGlobal();
    CRepository::iForceFailWithCode = KErrNone;
    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_NewLL()
    {
    EUNIT_ASSERT( iLiveSession )
    EUNIT_ASSERT( iRecordedLiveSession )
    EUNIT_ASSERT( iLiveSession->iCameraHandler.iCameraUsage == MusSettingsKeys::EUseCameraSwapping );     
    EUNIT_ASSERT( iLiveSession->iMceManagerUid == TUid::Uid( KMusUiUid ) );

    delete iLiveSession;
    iLiveSession = NULL;
    CRepository::SetStubGlobal( MusSettingsKeys::KCameraUsage,
                                MusSettingsKeys::EUseOnlySecondaryCamera );

    iLiveSession = CMusEngLiveSession::NewL();
    
    EUNIT_ASSERT_EQUALS( TInt( iLiveSession->iCameraHandler.iCameraUsage ),
                         TInt( MusSettingsKeys::EUseOnlySecondaryCamera ) );     
    
    delete iLiveSession;
    iLiveSession = NULL;
    CRepository::SetStubGlobal( MusSettingsKeys::KCameraUsage,
                                MusSettingsKeys::EUseOnlyMainCamera );

    iLiveSession = CMusEngLiveSession::NewL();   
    
    EUNIT_ASSERT_EQUALS( TInt( iLiveSession->iCameraHandler.iCameraUsage ),
                         TInt( MusSettingsKeys::EUseOnlyMainCamera ) );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_CompleteSessionStructureLL()
    {
    CMceStreamBundle* localBundle = 
        CMceStreamBundle::NewLC( CMceStreamBundle::ELS );
    
    // Check that structure cannot be completed before creating the session
    TRAPD( error, iLiveSession->CompleteSessionStructureL( *localBundle ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Normal case
  
    CSIPProfile* profile = iLiveSession->iSipProfileHandler->Profile();
    
    iLiveSession->iSession = CMceOutSession::NewL( 
                                    *(iLiveSession->iManager),
                                    *profile,
                                    KTestRecipientSipUri8() );
                             
    iLiveSession->CompleteSessionStructureL( *localBundle );
    
    EUNIT_ASSERT( localBundle->Streams().Count() == 0 )
    
    EUNIT_ASSERT( iLiveSession->iSession->Streams().Count() == 1 );
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Type() == KMceVideo );
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Source() );
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Source()->Type() ==
                  KMceCameraSource );
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Sinks().Count() == 1 );
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Sinks()[0]->Type() ==
                  KMceRTPSink );
    
    // Test that also recording stream is constructed if needed
    
    iRecordedLiveSession->iSession = CMceOutSession::NewL( 
                                            *(iLiveSession->iManager),
                                            *profile,
                                            KTestRecipientSipUri8() );
                                    
    iRecordedLiveSession->CompleteSessionStructureL( *localBundle );
    
    EUNIT_ASSERT( localBundle->Streams().Count() == 0 )
    
    CMceCameraSource* camera = 
            MusEngMceUtils::GetCameraL( *(iRecordedLiveSession->iSession) );
                    
    // Check that there is recorded stream and that source is same camera
    CMceMediaSink* file = NULL;
    const RPointerArray<CMceMediaStream>& streams = 
                                    iRecordedLiveSession->iSession->Streams();
                                        
    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( streams[i]->Type() == KMceVideo )
            {
            if ( streams[i]->Sinks()[0]->Type() == KMceFileSink )
                {
                file = streams[i]->Sinks()[0];
                }
            EUNIT_ASSERT( streams[i]->Source() == camera )
            }
        }
        
    EUNIT_ASSERT( file )
    
  

    EUNIT_ASSERT( iLiveSession->iCameraHandler.iCurrentCamera ==
                        TMusEngCameraHandler::EBackCamera );
    
    delete iLiveSession->iSession; 
    iLiveSession->iSession = NULL; 
    
    iLiveSession->iSession = CMceOutSession::NewL( 
                                    *(iLiveSession->iManager),
                                    *profile,
                                    KTestRecipientSipUri8() );
    
    iLiveSession->iCameraHandler.iCameraUsage = MusSettingsKeys::EUseOnlySecondaryCamera;     
    iLiveSession->CompleteSessionStructureL( *localBundle );
    
    EUNIT_ASSERT( iLiveSession->iCameraHandler.iCurrentCamera ==
                        TMusEngCameraHandler::EFrontCamera );
    
    
    CleanupStack::PopAndDestroy( localBundle );                                 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_AdjustVideoCodecLL()
    {
    CRepository::SetStubAvcConfigKeysL( KNullDesC8() );
    
    CMceVideoCodec* codecH263 = CMceH263Codec::NewLC( KMceSDPNameH263() );    
    CMceVideoCodec* codecH2632000 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );
    CMceVideoCodec* codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    CMceVideoCodec* codecUnknown = CMceH263Codec::NewLC( KNullDesC8() );
        
    iLiveSession->AdjustVideoCodecL( *codecH263, KMceCameraSource );
    iLiveSession->AdjustVideoCodecL( *codecH2632000, KMceCameraSource );    
    iLiveSession->AdjustVideoCodecL( *codecAvc, KMceCameraSource );  
    iLiveSession->AdjustVideoCodecL( *codecUnknown, KMceCameraSource );

    // Test that bit rates has been set to H263 & AVC codecs
    EUNIT_ASSERT( codecH263->iAllowedBitrates == KMceAllowedH263BitrateAll )
    EUNIT_ASSERT( codecH263->iMaxBitRate == KMceH263Level45Bitrate )
    EUNIT_ASSERT( codecH263->iBitrate == 80000 )
    
    EUNIT_ASSERT( codecH2632000->iAllowedBitrates == 
                  KMceAllowedH263BitrateAll )
    EUNIT_ASSERT( codecH2632000->iMaxBitRate == KMceH263Level45Bitrate )
    EUNIT_ASSERT( codecH2632000->iBitrate == 80000 )
    
    EUNIT_ASSERT( codecAvc->iAllowedBitrates == 
                                        KMceAvcCodecProfileIdBaseline | 
                                        KMceAvcCodecProfileIopConstraintSet | 
                                        KMceAvcBitrateLevel1b )
    EUNIT_ASSERT( iLiveSession->iStoreEncoderConfigInfo )


    // Test that bitrates has not been set to unknown codecs
    EUNIT_ASSERT( codecUnknown->iAllowedBitrates != 
                  KMceAllowedH263BitrateAll )
    EUNIT_ASSERT( codecUnknown->iMaxBitRate != KMceH263Level45Bitrate )
    EUNIT_ASSERT( codecUnknown->iBitrate != KMceH263Level10Bitrate )
    
    // Test that base class has been called to all codecs
    EUNIT_ASSERT( codecH263->iPayloadType == 96 )
    EUNIT_ASSERT( codecH2632000->iPayloadType == 96 )
    EUNIT_ASSERT( codecAvc->iPayloadType == 98 )
    EUNIT_ASSERT( codecUnknown->iPayloadType == 0 )
    
    CleanupStack::PopAndDestroy( codecUnknown );
    CleanupStack::PopAndDestroy( codecAvc );
    CleanupStack::PopAndDestroy( codecH2632000 );
    CleanupStack::PopAndDestroy( codecH263 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_AdjustAudioCodecLL()
    {
    CMceAudioCodec* codec = 
                iLiveSession->iManager->SupportedAudioCodecs()[0]->CloneL();
    CleanupStack::PushL( codec );
    iLiveSession->AdjustAudioCodecL( *codec );
    
    // Does nothing so just test that base class has been called
    
    EUNIT_ASSERT( codec->iPayloadType == 97 )
    
    CleanupStack::PopAndDestroy( codec );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_DoCodecSelectionLL()
    {
    CMceVideoStream* stream = CMceVideoStream::NewLC();
    
    CMceVideoCodec* codecH263 = CMceH263Codec::NewLC( _L8( "Foo" ) );
    stream->AddCodecL( codecH263 );
    CleanupStack::Pop( codecH263 );
        
    CMceVideoCodec* codecH2632000 = CMceH263Codec::NewLC( _L8( "Bar" ) );
    stream->AddCodecL( codecH2632000 );
    CleanupStack::Pop( codecH2632000 );
    
    CMceVideoCodec* codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    stream->AddCodecL( codecAvc );
    CleanupStack::Pop( codecAvc );
   
    // No assumptions so all the codecs are left to stream 
    iLiveSession->DoCodecSelectionL( *stream );    
    EUNIT_ASSERT_EQUALS( stream->Codecs().Count(), 3 )
    
    // Check that AVC codecs are removed if they are known to be not supported
    delete iLiveSession->iVideoCodecList;
    iLiveSession->iVideoCodecList = NULL;
    iLiveSession->iVideoCodecList = _L8("SomethingThatIsNotAVC").AllocL();
    iLiveSession->DoCodecSelectionL( *stream );
    EUNIT_ASSERT_EQUALS( stream->Codecs().Count(), 2 )
    EUNIT_ASSERT( stream->Codecs()[0]->SdpName() != KMceSDPNameH264() )
    EUNIT_ASSERT( stream->Codecs()[1]->SdpName() != KMceSDPNameH264() )
    
    CleanupStack::PopAndDestroy( stream );
    }

// -----------------------------------------------------------------------------
// Although all states are not meaningfull for LiveSession, all states are
// tested in order to force the updating of unit tests when behavior is changed 
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_StreamStateChangedL()
    {
    // Try without a session, nothing happens
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *videoStream );
    CleanupStack::PopAndDestroy( videoStream );
    
    // Simulate sending invite
    iLiveSession->EstablishLcSessionL();
    
    // Try all the stream states
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( *changedStream );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    }

// -----------------------------------------------------------------------------
// Although all states are not meaningfull for LiveSession, all states are
// tested in order to force the updating of unit tests when behavior is changed 
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_StreamStateChangedWithSourceL()
    {
    // Try without a session, nothing happens
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                    *videoStream, 
                    *CMceCameraSource::NewLC( *iLiveSession->iManager ) );
    CleanupStack::PopAndDestroy(); // camera
    CleanupStack::PopAndDestroy( videoStream );
    
    // Simulate sending invite
    iLiveSession->EstablishLcSessionL();
    
    // Try all the stream states
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    CMceMediaSource* changedSource = changedStream->Source();
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );;
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) ) 
    iLcSessionObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    static_cast<MMceStreamObserver*>(iLiveSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    }
    
// -----------------------------------------------------------------------------
// Although all states are not meaningfull for LiveSession, all states are
// tested in order to force the updating of unit tests when behavior is changed 
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_StreamStateChangedWithSinkL()
    {    
    // Try without a session, nothing happens
    ESTABLISH_OUT_SESSION( iLiveSession )
    
    iRecordedLiveSession->StreamStateChanged(
                *iLiveSession->iSession->Streams()[0],
                *iLiveSession->iSession->Streams()[0]->Sinks()[0] );
    
    // Try with session
    
    ESTABLISH_OUT_SESSION( iRecordedLiveSession )
    MLcDestinationFileControl* destinationFileControl =
        iRecordedLiveSession->LocalVideoPlayer()->LcDestinationFileControl();
    destinationFileControl->LcRecordL( ETrue );
    
    // Test state CMceMediaStream::ENoResources
                    
    // Try without recording stream, nothing happens
    
    iLiveSession->StreamStateChanged(
                *iLiveSession->iSession->Streams()[0],
                *iLiveSession->iSession->Streams()[0]->Sinks()[0] );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();

    // Try again with recording stream, but indicate change in another stream,
    // nothing happens    
    iRecordedLiveSession->StreamStateChanged(
                *iLiveSession->iSession->Streams()[0],
                *iLiveSession->iSession->Streams()[0]->Sinks()[0] );
                
    // Try again with recording stream, but with stream state != ENoResources, 
    // nothing happens
    CMceVideoStream* recordingStream = 
        MusEngMceUtils::GetRecordingStream( *iRecordedLiveSession->iSession );
        
    EUNIT_ASSERT( recordingStream )
    EUNIT_ASSERT( recordingStream->State() != CMceMediaStream::ENoResources )
    
    iRecordedLiveSession->StreamStateChanged( *recordingStream,
                                              *recordingStream->Sinks()[0] );
    
    // Try again with recording stream and stream state == ENoResources,
    // but with enabled sink, nothing happens
        
    recordingStream->iState = CMceMediaStream::ENoResources;
    EUNIT_ASSERT( recordingStream->Sinks()[0]->iIsEnabled )
    
    iRecordedLiveSession->StreamStateChanged( *recordingStream,
                                              *recordingStream->Sinks()[0] );
    
    // Disk full case
    
    recordingStream->Sinks()[0]->iIsEnabled = EFalse;
    
    iRecordedLiveSession->StreamStateChanged( *recordingStream,
                                              *recordingStream->Sinks()[0] );
    
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::EDiskFull ) )
    iLcSessionObserver->Reset();
    
    
    // Test default stream state change behavior
    
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    CMceMediaSink* changedSink = changedStream->Sinks()[0];
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // ENoResources, stream has no needed resources to stream
    // This state has non-default meaning, tested before defaults
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    iLiveSession->StreamStateChanged( *changedStream, *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_EstablishLcSessionL()
    {
    iLiveSession->EstablishLcSessionL();
    EUNIT_ASSERT_EQUALS( TInt( CMceSession::EOffering ),
                         TInt( iLiveSession->iSession->iState ) )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngLiveSession::UT_LocalVideoPlayerL()
    {
    EUNIT_ASSERT( iLiveSession->LocalVideoPlayer() == 
                  iLiveSession->iLiveVideoPlayer )
    }

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngLiveSession,
    "UT_CMusEngLiveSesssion",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusEngLiveSession",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewLL, Teardown)

EUNIT_TEST(
    "CompleteSessionStructureL - test ",
    "CMusEngLiveSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureLL, Teardown)

EUNIT_TEST(
    "AdjustVideoCodecL - test ",
    "CMusEngLiveSession",
    "AdjustVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustVideoCodecLL, Teardown)

EUNIT_TEST(
    "AdjustAudioCodecL - test ",
    "CMusEngLiveSession",
    "AdjustAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustAudioCodecLL, Teardown)

EUNIT_TEST(
    "DoCodecSelectionL- test ",
    "CMusEngLiveSession",
    "DoCodecSelectionL",
    "FUNCTIONALITY",
    SetupL, UT_DoCodecSelectionLL, Teardown)

EUNIT_TEST(
    "StreamStateChangedL() - test ",
    "CMusEngLiveSession",
    "StreamStateChangedL()",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedL, Teardown)

EUNIT_TEST(
    "StreamStateChangedL( src ) - test ",
    "CMusEngLiveSession",
    "StreamStateChangedL( src )",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedWithSourceL, Teardown)        

EUNIT_TEST(
    "StreamStateChangedL( sink ) - test ",
    "CMusEngLiveSession",
    "StreamStateChangedL( sink )",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedWithSinkL, Teardown) 

EUNIT_TEST(
    "EstablishLcSessionL - test ",
    "CMusEngLiveSession",
    "EstablishLcSessionL",
    "FUNCTIONALITY",
    SetupL, UT_EstablishLcSessionL, Teardown)
    
EUNIT_TEST(
    "LocalVideoPlayer - test ",
    "CMusEngLiveSession",
    "LocalVideoPlayer",
    "FUNCTIONALITY",
    SetupL, UT_LocalVideoPlayerL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE


