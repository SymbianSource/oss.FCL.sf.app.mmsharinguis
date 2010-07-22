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
#include "ut_musengClipsession.h"
#include "musengstubs.h"
#include "musengtestdefs.h"
#include "musengclipsession.h"
#include "mussipprofilehandler.h"
#include "musengmceutils.h"
#include "mussessionproperties.h"

//  SYSTEM INCLUDES
#include <lcvideoplayer.h>
#include <lcsourcefilecontrol.h>
#include <eunitmacros.h>
#include <mceoutsession.h>
#include <mcestreambundle.h>
#include <mcevideostream.h>
#include <mceaudiostream.h>
#include <mcefilesource.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <mcertpsource.h>
#include <mcevideocodec.h>
#include <mceamrcodec.h>
#include <drmcommon.h>
#include <sipprofile.h>
#include <sipstrings.h>
#include <e32property.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngClipSession* UT_CMusEngClipSession::NewL()
    {
    UT_CMusEngClipSession* self = UT_CMusEngClipSession::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngClipSession* UT_CMusEngClipSession::NewLC()
    {
    UT_CMusEngClipSession* self = new( ELeave ) UT_CMusEngClipSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngClipSession::~UT_CMusEngClipSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngClipSession::UT_CMusEngClipSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::SetupL()
    {
    iLcSessionObserver = new( ELeave )CLcSessionObserverStub;
    iLcUiProvider = new( ELeave )CLcUiProviderStub;
    iAudioRoutingObserver = new( ELeave )CMusEngObserverStub; 
    
    iClipSession = CMusEngClipSession::NewL();
    iClipSession->SetLcSessionObserver( iLcSessionObserver );
    iClipSession->SetLcUiProvider( iLcUiProvider );    
    iClipSession->LocalVideoPlayer()->LcSourceFileControl()->SetLcFileNameL(
        KTestVideoFileName() );
    delete iClipSession->iVideoCodecList;
    iClipSession->iVideoCodecList = NULL;
    iClipSession->iVideoCodecList = KMceSDPNameH264().AllocL();

    SIPStrings::OpenL();
    
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KRemoteSipAddress,
                                        KTestRecipientSipUri ) );
    }

    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::Teardown()
    {
    SIPStrings::Close();
    delete iClipSession;
    delete iLcSessionObserver;
    delete iLcUiProvider;
    delete iAudioRoutingObserver;
    PropertyHelper::Close();
    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_NewLL()
    {
    EUNIT_ASSERT( iClipSession )
    EUNIT_ASSERT( !iClipSession->iSession )
    EUNIT_ASSERT( iClipSession->iMceManagerUid == TUid::Uid( KMusUiUid ) );
    }
        

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_CompleteSessionStructureLL()
    {   
    CMceStreamBundle* localBundle = 
        CMceStreamBundle::NewLC( CMceStreamBundle::ELS );
    
    // Check that structure cannot be completed before creating the session
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                iClipSession->CompleteSessionStructureL( *localBundle ),
                KErrNotReady )
    
    // Normal case
    CSIPProfile* profile = iClipSession->iSipProfileHandler->Profile();          
    iClipSession->iSession = CMceOutSession::NewL( 
        *iClipSession->iManager, *profile, KTestRecipientSipUri8() );
    
    iClipSession->CompleteSessionStructureL( *localBundle );
    EUNIT_ASSERT( iClipSession->iSession->Streams().Count() == 3 )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[0]->Type() == KMceVideo )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[0]->Source() )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[0]->Source()->Type() ==
                  KMceFileSource )             
    EUNIT_ASSERT( iClipSession->iSession->Streams()[0]->Sinks().Count() == 1 )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[0]->Sinks()[0]->Type() ==
                  KMceRTPSink )
    
    // Check that only stream with speaker has been added to a bundle
    EUNIT_ASSERT( localBundle->Streams().Count() == 1 )
    EUNIT_ASSERT( localBundle->Streams()[0]->Type() == KMceAudio )
    EUNIT_ASSERT( localBundle->Streams()[0]->Sinks().Count() > 0 )
    EUNIT_ASSERT( localBundle->Streams()[0]->Sinks()[0]->Type() == 
                  KMceSpeakerSink )
    
    CleanupStack::PopAndDestroy( localBundle );
    }


// -----------------------------------------------------------------------------
// Although all states are not meaningfull for LiveSession, all states are
// tested in order to force the updating of unit tests when behavior is changed 
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_StreamStateChangedL()
    {
    // Try without a session, nothing happens
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *videoStream );
    CleanupStack::PopAndDestroy( videoStream );
    
    // Simulate sending invite
    iClipSession->EstablishLcSessionL();
   
    // Try all the stream states
    CMceMediaStream* changedStream = iClipSession->iSession->Streams()[0];
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    }
    

// -----------------------------------------------------------------------------
// Although all states are not meaningfull for ClipSession, all states are
// tested in order to force the updating of unit tests when behavior is changed 
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_StreamStateChangedWithSourceL()
    {
    // Try without a session, nothing happens
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    CMceRtpSource* rtpSource = CMceRtpSource::NewLC();
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                    *videoStream,
                    *rtpSource );     
    CleanupStack::PopAndDestroy( rtpSource );         
    CleanupStack::PopAndDestroy( videoStream );
    
    // Simulate sending invite
    iClipSession->EstablishLcSessionL();
    
    // Try all the non-default stream states
    CMceMediaStream* changedStream = iClipSession->iSession->Streams()[0];
    CMceMediaSource* changedSource = changedStream->Source();
    
    // EDisabled, end of clip
    changedStream->iState = CMceMediaStream::EDisabled;
    changedSource->iIsEnabled = EFalse;
    iClipSession->StreamStateChanged( *changedStream, *changedSource );
    
    // TODO: EUNIT_ASSERT( iLcSessionObserver->iEndOfClipCalled )
    changedSource->iIsEnabled = ETrue;
    iLcSessionObserver->Reset();
    
    // Test default stream state change behavior, remove or change when
    // behavior changes
    
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )

    // Special meaning (Transcoding ready), tested separately
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    // This state has non-default meaning, tested before defaults
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    }


// -----------------------------------------------------------------------------
// Although all states are not meaningfull for LiveSession, all states are
// tested in order to force the updating of unit tests when behavior is changed 
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_StreamStateChangedWithSinkL()
    {
    // Try without a session, nothing happens
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                        *videoStream,
                                                        *CMceRtpSink::NewLC() );
    CleanupStack::PopAndDestroy(); // rtp
    CleanupStack::PopAndDestroy( videoStream );
    
    // Simulate sending invite
    iClipSession->EstablishLcSessionL();
                                     
    // Test default stream state change behavior
    CMceMediaStream* changedStream = iClipSession->iSession->Streams()[0];
    CMceMediaSink* changedSink = changedStream->Sinks()[0];

    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
                                                            
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_AddAmrCodecLL()
    {
    // Check that all the codecs are replaced with AMR codec
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    
    audioStream->InitializeL( iClipSession->iManager );
    
    EUNIT_ASSERT( audioStream->Codecs().Count() == 2 )
    
    iClipSession->AddAmrCodecL( *audioStream );
    
    EUNIT_ASSERT( audioStream->Codecs().Count() == 1 )
    EUNIT_ASSERT( audioStream->Codecs()[0]->SdpName() == KMceSDPNameAMR() )
    EUNIT_ASSERT( audioStream->Codecs()[0]->AllowedBitrates() == 
                  KMceAllowedAmrNbBitrate475 )
    EUNIT_ASSERT( audioStream->Codecs()[0]->Bitrate() == 
                  KMceAmrNbBitrate475 )
    
    // Check that function leaves  if there is no AMR codec
    
    iClipSession->iManager->iSupportedAudioCodecs.ResetAndDestroy();
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->AddAmrCodecL( *audioStream ),
                                 KErrNotFound )
    
    CleanupStack::PopAndDestroy( audioStream );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_AddVideoCodecLL()
    {
    // Check that all the codecs are replaced with H263 codec if other end
    // does not support H264
    //
    delete iClipSession->iVideoCodecList;
    iClipSession->iVideoCodecList = NULL;
    iClipSession->iVideoCodecList = KMceSDPNameH263().AllocL();
    
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    
    videoStream->InitializeL( iClipSession->iManager );
    
    EUNIT_ASSERT( videoStream->Codecs().Count() > 1 )
    
    iClipSession->AddVideoCodecL( *videoStream );
    
    EUNIT_ASSERT( videoStream->Codecs().Count() == 1 )
    EUNIT_ASSERT( videoStream->Codecs()[0]->SdpName() == KMceSDPNameH2632000() )
    
    // Check that all codecs are replaced with H264 codec if other end
    // supports it
    //
    _LIT8( KMusTestCodecListDelim, ";" );
    delete iClipSession->iVideoCodecList;
    iClipSession->iVideoCodecList = NULL;
    iClipSession->iVideoCodecList = 
        HBufC8::NewL( KMceSDPNameH263().Length() + 
                      KMceSDPNameH264().Length() + 
                      KMusTestCodecListDelim().Length() );
    iClipSession->iVideoCodecList->Des().Copy( KMceSDPNameH263() );
    iClipSession->iVideoCodecList->Des().Append( KMusTestCodecListDelim() );
    iClipSession->iVideoCodecList->Des().Append( KMceSDPNameH264() );
    iClipSession->AddVideoCodecL( *videoStream );
    
    EUNIT_ASSERT( videoStream->Codecs().Count() == 1 )
    EUNIT_ASSERT( videoStream->Codecs()[0]->SdpName() == KMceSDPNameH264() )
    
    // Check that function leaves  if there is no H264 codec
    //
    iClipSession->iManager->iSupportedVideoCodecs.ResetAndDestroy();
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->AddVideoCodecL( *videoStream ),
                                 KErrNotFound )
    
    CleanupStack::PopAndDestroy( videoStream );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_ConstructAudioStructureLL()
    {
    
    CMceStreamBundle* localBundle = 
                            CMceStreamBundle::NewLC( CMceStreamBundle::ELS );
   
    // Check that audio structure cannot be constructed before 
    // creating the session
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                iClipSession->ConstructAudioStructureL( *localBundle ),
                KErrNotReady )
    
    // Try without a file source
    
    CSIPProfile* profile = iClipSession->iSipProfileHandler->Profile();
  
    iClipSession->iSession = CMceOutSession::NewL( 
                                    *(iClipSession->iManager),
                                    *profile,
                                    KTestRecipientSipUri8() );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                iClipSession->ConstructAudioStructureL( *localBundle ),
                KErrNotFound )
    
    // Normal case
    
    CMceVideoStream* videoOut = CMceVideoStream::NewLC();
    
    videoOut->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    
    videoOut->SetSourceL( CMceFileSource::NewLC( 
        *iClipSession->iManager,
        iClipSession->LocalVideoPlayer()->LcSourceFileControl()->LcFileName() ) );
    CleanupStack::Pop();                                             
     
    iClipSession->iSession->AddStreamL( videoOut );
    CleanupStack::Pop( videoOut );
      
    iClipSession->ConstructAudioStructureL( *localBundle );
    
    EUNIT_ASSERT( iClipSession->iSession->Streams().Count() == 3 )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[1]->Type() == KMceAudio )  
    EUNIT_ASSERT( iClipSession->iSession->Streams()[2]->Type() == KMceAudio ) 
    EUNIT_ASSERT( iClipSession->iSession->Streams()[1]->Source() == 
                  videoOut->Source() )  
    EUNIT_ASSERT( iClipSession->iSession->Streams()[2]->Source() == 
                  videoOut->Source() )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[1]->Sinks().Count() == 1 )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[2]->Sinks().Count() == 1 )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[1]->Sinks()[0]->Type() ==
                  KMceRTPSink )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[2]->Sinks()[0]->Type() ==
                  KMceSpeakerSink )                         
    
    // Check that only stream with speaker has been added to a bundle
    EUNIT_ASSERT( localBundle->Streams().Count() == 1 )
    EUNIT_ASSERT( localBundle->Streams()[0]->Type() == KMceAudio )
    EUNIT_ASSERT( localBundle->Streams()[0]->Sinks().Count() == 1 )
    EUNIT_ASSERT( localBundle->Streams()[0]->Sinks()[0]->Type() == 
                  KMceSpeakerSink )
    
    CleanupStack::PopAndDestroy( localBundle );
    }


// -----------------------------------------------------------------------------
// Check that audio is not constructed in operator variant
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_ConstructAudioStructureL_OperatorVariantL()
    { 
    iClipSession->iOperatorVariant = ETrue;
   	CSIPProfile* profile = iClipSession->iSipProfileHandler->Profile();
    delete profile->iArray;
    profile->iArray = NULL;
    profile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
    profile->iArray->AppendL( KMusSipUri );
  
    // Establish session, ConstructAudioStructureL gets called
    ESTABLISH_OUT_SESSION( iClipSession );
    
    // Check that only two video streams has been constructed
    EUNIT_ASSERT_EQUALS( iClipSession->iSession->Streams().Count(), 2 )
    EUNIT_ASSERT_EQUALS( iClipSession->iSession->Streams()[0]->Type(), 
                         KMceVideo )  
    EUNIT_ASSERT_EQUALS( iClipSession->iSession->Streams()[1]->Type(),
                         KMceVideo ) 
    
    // Check that no bundles has been constructed
    EUNIT_ASSERT_EQUALS( iClipSession->iSession->Bundles().Count(), 0 )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_EstablishSessionLL()
    {
    iClipSession->LocalVideoPlayer()->LcSourceFileControl()->SetLcFileNameL(
         KTestAvcVideoFileName() );

    // Try to establish, must fail, because of missing session
    TRAPD( error, iClipSession->EstablishSessionL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    EUNIT_ASSERT( iClipSession->iVideoCodecList );

    ///////
    // 1.Test that in case the peer party supports H264, no transcoding is needed
    
    // Call to CMusEngOutMceSession::InviteL leads to call to EstablishL
    iClipSession->EstablishLcSessionL();
    
    const RPointerArray<CMceMediaStream>& streams = iClipSession->iSession->Streams();

    for ( TInt i = 0; i < streams.Count(); ++i )
         {
         if ( streams[i]->Type() == KMceVideo )
             {
             
             CMceVideoStream* videoStream = static_cast<CMceVideoStream*>( streams[i] );
             const RPointerArray<CMceVideoCodec> codecs = videoStream->Codecs();
             EUNIT_ASSERT_EQUALS( codecs.Count(), 1 )
             EUNIT_ASSERT( codecs[0]->SdpName().FindF( KMceSDPNameH264() ) >= 0 )
             
             }
         }
    
    ///////
    // 2.Test the case when we don't know whether peer supports H264, 
    // transcoding is needed => function will leave with KErrNotSupported

    delete iClipSession->iSession;
    iClipSession->iSession = NULL;
    
    delete iClipSession->iVideoCodecList;
    iClipSession->iVideoCodecList = NULL;
    
    MUSENG_EUNIT_ASSERT_SPECIFIC_LEAVE( 
                   iClipSession->EstablishLcSessionL(),
                   KErrNotSupported )
    
    
    ///////
    // 3.Test that if peer doesn't supports H264, transcoding is needed
    // => function will leave with KErrNotSupported
    
    delete iClipSession->iSession;
    iClipSession->iSession = NULL;
     
    iClipSession->iVideoCodecList = KMceSDPNameH263().AllocL();
    
    MUSENG_EUNIT_ASSERT_SPECIFIC_LEAVE( 
                    iClipSession->EstablishLcSessionL(),
                    KErrNotSupported )
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngClipSession,
    "UT_CMusEngClipSesssion",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusEngClipSession",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewLL, Teardown)

EUNIT_TEST(
    "CompleteSessionStructureL - test ",
    "CMusEngClipSession",
    "CompleteSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CompleteSessionStructureLL, Teardown)

EUNIT_TEST(
    "StreamStateChangedL() - test ",
    "CMusEngClipSession",
    "StreamStateChangedL()",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedL, Teardown)
    
EUNIT_TEST(
    "StreamStateChangedL( source ) - test ",
    "CMusEngClipSession",
    "StreamStateChangedL( source )",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedWithSourceL, Teardown)

EUNIT_TEST(
    "StreamStateChangedL( sink ) - test ",
    "CMusEngClipSession",
    "StreamStateChangedL( sink )",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedWithSinkL, Teardown)
        
EUNIT_TEST(
    "AddAmrCodecL - test ",
    "CMusEngClipSession",
    "AddAmrCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AddAmrCodecLL, Teardown)

EUNIT_TEST(
    "AddVideoCodecL - test ",
    "CMusEngClipSession",
    "AddVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AddVideoCodecLL, Teardown)

EUNIT_TEST(
    "ConstructAudioStructureL - test ",
    "CMusEngClipSession",
    "ConstructAudioStructureL",
    "FUNCTIONALITY",
    SetupL, UT_ConstructAudioStructureLL, Teardown)

EUNIT_TEST(
    "ConstructAudioStructureL - test operator specific behavior ",
    "CMusEngClipSession",
    "ConstructAudioStructureL",
    "FUNCTIONALITY",
    SetupL, UT_ConstructAudioStructureL_OperatorVariantL, Teardown)    

EUNIT_TEST(
    "EstablishSessionL - test ",
    "CMusEngClipSession",
    "EstablishSessionL",
    "FUNCTIONALITY",
    SetupL, UT_EstablishSessionLL, Teardown)    

        
EUNIT_END_TEST_TABLE

//  END OF FILE


