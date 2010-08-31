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
#include "ut_musengmceutils.h"
#include "musengmceutils.h"
#include "mussipprofilehandler.h"
#include "musengstubs.h"
#include "musengtestdefs.h"
 

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>

#include <mcemanager.h>
#include <mceoutsession.h>
#include <mcevideostream.h>
#include <mceaudiostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcecamerasource.h>
#include <mcefilesource.h>
#include <mcespeakersink.h>
#include <mcertpsource.h>
#include <mcefilesink.h>

const TUint KSipProfileId( 1 );

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_MusEngMceUtils* UT_MusEngMceUtils::NewL()
    {
    UT_MusEngMceUtils* self = UT_MusEngMceUtils::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_MusEngMceUtils* UT_MusEngMceUtils::NewLC()
    {
    UT_MusEngMceUtils* self = new( ELeave ) UT_MusEngMceUtils();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_MusEngMceUtils::~UT_MusEngMceUtils()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_MusEngMceUtils::UT_MusEngMceUtils()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::SetupL()
    {
    iManager = CMceManager::NewL( TUid::Null(), NULL );
    iObserver = new( ELeave ) CMusEngObserverStub;
    iObserver->iRoamingBetweenAPsAllowed = ETrue;
    iProfileHandler = CMusSipProfileHandler::NewL( *iObserver );
    
    iProfileHandler->CreateProfileL( KSipProfileId );
    iEmptySession = CMceOutSession::NewL( *iManager, 
                                          *iProfileHandler->Profile(),
                                          KTestRecipientSipUri8() );
    
    // Contruct an outgoing video sesssion                                 
    iVideoOutSession = CMceOutSession::NewL( *iManager,
                                             *iProfileHandler->Profile(),
                                             KTestRecipientSipUri8() );
    
    
    CMceVideoStream* videoOut = CMceVideoStream::NewLC();
    
    videoOut->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    
    videoOut->AddSinkL( CMceDisplaySink::NewLC( *iManager ) );
    CleanupStack::Pop();
    
    videoOut->SetSourceL( CMceCameraSource::NewLC( *iManager ) );
    CleanupStack::Pop();
    
    iVideoOutSession->AddStreamL( videoOut );
    CleanupStack::Pop( videoOut );
    
    // Consruct a video session with incoming video and audio stream
    // Audio part
    iVideoInSession = CMceOutSession::NewL( *iManager,
                                            *iProfileHandler->Profile(), 
                                            KTestRecipientSipUri8() );
    
    CMceAudioStream* audioIn = CMceAudioStream::NewLC();
    
    audioIn->AddSinkL( CMceSpeakerSink::NewLC() );
    CleanupStack::Pop();
    
    audioIn->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop();
    
    iVideoInSession->AddStreamL( audioIn );
    CleanupStack::Pop( audioIn );
    
    // Video part
    CMceVideoStream* videoIn  = CMceVideoStream::NewLC();
    
    CMceDisplaySink* receivingDisplay = CMceDisplaySink::NewLC( *iManager );
    videoIn->AddSinkL( receivingDisplay );
    CleanupStack::Pop();
    iReceivingDisplay = receivingDisplay;
    
    videoIn->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop();
    
    iVideoInSession->AddStreamL( videoIn );
    CleanupStack::Pop( videoIn );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::Setup2L()
    {
    iManager = CMceManager::NewL( TUid::Null(), NULL );
    iObserver = new( ELeave ) CMusEngObserverStub;
    iObserver->iRoamingBetweenAPsAllowed = ETrue;
    iProfileHandler = CMusSipProfileHandler::NewL( *iObserver );
    
    iProfileHandler->CreateProfileL( KSipProfileId );
    iEmptySession = CMceOutSession::NewL( *iManager, 
                                          *iProfileHandler->Profile(),
                                          KTestRecipientSipUri8() );
    
    // Contruct an outgoing video sesssion                                 
    iVideoOutSession = CMceOutSession::NewL( *iManager,
                                             *iProfileHandler->Profile(),
                                             KTestRecipientSipUri8() );
    
    
    CMceVideoStream* videoOut = CMceVideoStream::NewLC();
    
    videoOut->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    
    videoOut->AddSinkL( CMceDisplaySink::NewLC( *iManager ) );
    CleanupStack::Pop();
    
    videoOut->SetSourceL( CMceCameraSource::NewLC( *iManager ) );
    CleanupStack::Pop();
    
    // Video part
    CMceVideoStream* videoIn  = CMceVideoStream::NewLC();
    
    CMceDisplaySink* receivingDisplay = CMceDisplaySink::NewLC( *iManager );
    videoIn->AddSinkL( receivingDisplay );
    CleanupStack::Pop();
    iReceivingDisplay = receivingDisplay;
    
    videoIn->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop();
    
    videoOut->BindL( videoIn );
    CleanupStack::Pop( videoIn );
    
    iVideoOutSession->AddStreamL( videoOut );
    CleanupStack::Pop( videoOut );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::Teardown()
    {
    delete iEmptySession;
    delete iVideoOutSession;
    delete iVideoInSession;
    iVideoInSession = NULL;
    delete iProfileHandler;
    delete iObserver;
    delete iManager;
    iReceivingDisplay = NULL; // Not owned
    }



// TEST CASES



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_IsVideoInStreamL()
    {
    // Stream 0 is audio stream
    EUNIT_ASSERT( !MusEngMceUtils::IsVideoInStream( 
                                        *iVideoInSession->Streams()[0] ) )
    
    // No source
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    EUNIT_ASSERT( !MusEngMceUtils::IsVideoInStream( *videoStream ) )
    CleanupStack::PopAndDestroy( videoStream );
    
    // Camera source
    EUNIT_ASSERT( !MusEngMceUtils::IsVideoInStream( 
                                        *iVideoOutSession->Streams()[0] ) )
                                        
    // Video in stream
    EUNIT_ASSERT( MusEngMceUtils::IsVideoInStream( 
                                        *iVideoInSession->Streams()[1] ) )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_IsVideoOutStreamL()
    {
    // Stream 0 is audio stream
    EUNIT_ASSERT( !MusEngMceUtils::IsVideoOutStream( 
                                        *iVideoInSession->Streams()[0] ) )
    
    // No sink
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    EUNIT_ASSERT( !MusEngMceUtils::IsVideoOutStream( *videoStream ) )
    CleanupStack::PopAndDestroy( videoStream );
    
    // display sink
    EUNIT_ASSERT( !MusEngMceUtils::IsVideoOutStream( 
                                        *iVideoInSession->Streams()[1] ) )
                                        
    // Video outstream
    EUNIT_ASSERT( MusEngMceUtils::IsVideoOutStream( 
                                        *iVideoOutSession->Streams()[0] ) )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_IsAudioInStreamL()
    {
    // Stream 0 is video stream
    EUNIT_ASSERT( !MusEngMceUtils::IsAudioInStream( 
                                        *iVideoOutSession->Streams()[0] ) )
    
    // No source
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    EUNIT_ASSERT( !MusEngMceUtils::IsAudioInStream( *audioStream ) )
    
    // File source
    audioStream->SetSourceL( CMceFileSource::NewLC( *iManager, KNullDesC() ) );
    CleanupStack::Pop();
    
    EUNIT_ASSERT( !MusEngMceUtils::IsAudioInStream( *audioStream ) )
    CleanupStack::PopAndDestroy( audioStream );
                                        
    // Audio in stream
    EUNIT_ASSERT( MusEngMceUtils::IsAudioInStream( 
                                        *iVideoInSession->Streams()[0] ) )
    }   
        
        
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetVideoOutStreamLL()
    {
    // No streams, must fail
    EUNIT_ASSERT_SPECIFIC_LEAVE(
                MusEngMceUtils::GetVideoOutStreamL( *iEmptySession ),
                KErrNotFound )
    
    // Add an audio out stream to empty sessionand and try again, still fails
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    
    audioStream->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    
    iEmptySession->AddStreamL( audioStream );
    CleanupStack::Pop(audioStream);
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                MusEngMceUtils::GetVideoOutStreamL( *iEmptySession ),
                KErrNotFound )
    
    // Incoming video stream, fails
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                MusEngMceUtils::GetVideoOutStreamL( *iVideoInSession ),
                KErrNotFound )
    
    // One incoming video stream with bound outstream, must find bound stream
    CMceVideoStream* boundVideoOut = CMceVideoStream::NewLC();
    boundVideoOut->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    boundVideoOut->SetSourceL( CMceCameraSource::NewLC( *iManager ) );
    CleanupStack::Pop();
    reinterpret_cast<CMceVideoStream*>( 
            iVideoInSession->Streams()[ 1 ] )->BindL( boundVideoOut );
    CleanupStack::Pop( boundVideoOut );
    CMceVideoStream* videoStream = MusEngMceUtils::GetVideoOutStreamL( 
                                                    *iVideoInSession );
    EUNIT_ASSERT( videoStream )
    EUNIT_ASSERT( videoStream->Type() == KMceVideo )
    EUNIT_ASSERT( videoStream->Sinks().Count() == 1 ) // Rtp
    EUNIT_ASSERT( videoStream->Sinks()[0]->Type() == KMceRTPSink )

    // One outgoing video stream, succeeds
    videoStream = MusEngMceUtils::GetVideoOutStreamL( *iVideoOutSession );
    EUNIT_ASSERT( videoStream )
    EUNIT_ASSERT( videoStream->Type() == KMceVideo )
    EUNIT_ASSERT( videoStream->Sinks().Count() == 2 ) // Rtp, Display
    EUNIT_ASSERT( videoStream->Sinks()[0]->Type() == KMceRTPSink )
    EUNIT_ASSERT( videoStream->Sinks()[1]->Type() == KMceDisplaySink )
        
    // Add another stream
    CMceVideoStream* videoOut = CMceVideoStream::NewLC();
    
    videoOut->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    
    videoOut->SetSourceL( CMceFileSource::NewLC( *iManager,
                                                 KTestVideoFileName() ) );
    CleanupStack::Pop();
    
    iVideoOutSession->AddStreamL( videoOut );
    CleanupStack::Pop( videoOut );
    
    // Now there are two outgoing video streams, must fail
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                MusEngMceUtils::GetVideoOutStreamL( *iVideoOutSession ),
                KErrOverflow )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetVideoInStreamLL()
    {
    // No streams, must fail
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                MusEngMceUtils::GetVideoInStreamL( *iEmptySession ),
                KErrNotFound )
    
    // Outgoing video stream, fails
    EUNIT_ASSERT_SPECIFIC_LEAVE(
                MusEngMceUtils::GetVideoInStreamL( *iVideoOutSession ),
                KErrNotFound )
    
    // Add stream without source and try again, still fails
    CMceVideoStream* newVideoStream = CMceVideoStream::NewLC();
    iVideoOutSession->AddStreamL( newVideoStream );
    CleanupStack::Pop( newVideoStream );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                MusEngMceUtils::GetVideoInStreamL( *iVideoOutSession ),
                KErrNotFound )
    
    // One incoming video stream, succeeds
    CMceVideoStream* videoStream = MusEngMceUtils::GetVideoInStreamL( 
                                                        *iVideoInSession );
    
    EUNIT_ASSERT( videoStream )
    EUNIT_ASSERT( videoStream->Type() == KMceVideo )
    EUNIT_ASSERT( videoStream->Sinks().Count() == 1 ) // Display
    EUNIT_ASSERT( videoStream->Sinks()[0]->Type() == KMceDisplaySink )
    EUNIT_ASSERT( videoStream->Source() )
    EUNIT_ASSERT( videoStream->Source()->Type() == KMceRTPSource )
    
    // Add another video in stream
    
    CMceVideoStream* videoIn = CMceVideoStream::NewLC();
    
    videoIn->AddSinkL( CMceDisplaySink::NewLC( *iManager ) );
    CleanupStack::Pop();
    
    videoIn->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop();
    
    iVideoInSession->AddStreamL( videoIn );
    CleanupStack::Pop( videoIn );
    
    // Now there are two incoming video streams, must fail
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                MusEngMceUtils::GetVideoInStreamL( *iVideoInSession ),
                KErrOverflow )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetRecordingStreamL()
    {
    CMceVideoStream* recordingStream = 
                        MusEngMceUtils::GetRecordingStream( *iVideoInSession );
    EUNIT_ASSERT( !recordingStream )
    
    recordingStream = MusEngMceUtils::GetRecordingStream( *iVideoOutSession );
    EUNIT_ASSERT( !recordingStream )
    
    // Create recording stream
    CMceVideoStream* streamForRecording = CMceVideoStream::NewLC();
        
    CMceFileSink* fileSink = CMceFileSink::NewLC( KTestVideoFileName() );
    streamForRecording->AddSinkL( fileSink );
    CleanupStack::Pop( fileSink );
    
    iVideoOutSession->AddStreamL( streamForRecording );
    CleanupStack::Pop( streamForRecording );
    
    // Test that it is found
    recordingStream = MusEngMceUtils::GetRecordingStream( *iVideoOutSession );
    EUNIT_ASSERT( recordingStream ) 
    EUNIT_ASSERT( recordingStream == streamForRecording )      
    }
    

// -----------------------------------------------------------------------------
// From stream
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetMediaSinkFromStreamL()
    {    
    CMceMediaSink* sink = MusEngMceUtils::GetMediaSink( 
                                            *(iVideoInSession->Streams()[0]), 
                                            KMceRTPSink );
    EUNIT_ASSERT( !sink )
    
    sink = MusEngMceUtils::GetMediaSink( *(iVideoOutSession->Streams()[0]), 
                                         KMceRTPSink );
    
    EUNIT_ASSERT( sink )
    EUNIT_ASSERT( sink->Type() == KMceRTPSink )
    
    }


// -----------------------------------------------------------------------------
// From stream
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetMediaSinkLFromStreamL()
    {                                                      
    CMceMediaSink* sink = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE(
            MusEngMceUtils::GetMediaSinkL( *(iVideoInSession->Streams()[0]), 
                                           KMceRTPSink ),
            KErrNotFound )
    
    sink = MusEngMceUtils::GetMediaSinkL( *(iVideoOutSession->Streams()[0]), 
                                          KMceRTPSink );
    EUNIT_ASSERT( sink->Type() == KMceRTPSink )
    }  
    
    
// -----------------------------------------------------------------------------
// From session
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetMediaSinkFromSessionL()
    {
    CMceMediaSink* sink = MusEngMceUtils::GetMediaSink( *iEmptySession, 
                                                        KMceRTPSink );
    EUNIT_ASSERT( !sink )
    
    sink = MusEngMceUtils::GetMediaSink( *iVideoOutSession, KMceRTPSink );
    
    EUNIT_ASSERT( sink )
    EUNIT_ASSERT( sink->Type() == KMceRTPSink )
    }


// -----------------------------------------------------------------------------
// From session
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetMediaSinkLFromSessionL()
    {
    CMceMediaSink* sink = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( MusEngMceUtils::GetMediaSinkL( *iEmptySession, 
                                                                KMceRTPSink ),
                                 KErrNotFound )
    
    sink = MusEngMceUtils::GetMediaSinkL( *iVideoOutSession, KMceRTPSink );
    EUNIT_ASSERT( sink->Type() == KMceRTPSink )
    }  
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetCameraLL()
    {
    // empty session fails
    EUNIT_ASSERT_SPECIFIC_LEAVE( MusEngMceUtils::GetCameraL( *iEmptySession ),
                                 KErrNotFound )
    
    // succeed
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iVideoOutSession );
    
    EUNIT_ASSERT( camera )
    EUNIT_ASSERT( camera->Type() == KMceCameraSource )
    
    // delete source from video out stream and try without it, fails
    iVideoOutSession->Streams()[0]->SetSourceL( NULL );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                MusEngMceUtils::GetCameraL( *iVideoOutSession ),
                KErrNotFound )
    
    // Add file source and try again, still fails 
    iVideoOutSession->Streams()[0]->SetSourceL( 
                    CMceFileSource::NewLC( *iManager, KTestVideoFileName() ) );
    CleanupStack::Pop();
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                MusEngMceUtils::GetCameraL( *iVideoOutSession ),
                KErrNotFound )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetFileSourceLL()
    {
    // empty session fails
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                MusEngMceUtils::GetFileSourceL( *iEmptySession ),
                KErrNotFound )
    
    // delete source from video out stream and try without it, fails
    iVideoOutSession->Streams()[0]->SetSourceL( NULL );
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                MusEngMceUtils::GetFileSourceL( *iVideoOutSession ),
                KErrNotFound )
    
    // Add camera source and try again, still fails 
    iVideoOutSession->Streams()[0]->SetSourceL( 
                                    CMceCameraSource::NewLC( *iManager ) );
    CleanupStack::Pop();
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                MusEngMceUtils::GetFileSourceL( *iVideoOutSession ),
                KErrNotFound )
    
    // Replace camera with file source, now succeeds    
    iVideoOutSession->Streams()[0]->SetSourceL( 
                    CMceFileSource::NewLC( *iManager, KTestVideoFileName() ) );
    CleanupStack::Pop();
    
    CMceFileSource* file = MusEngMceUtils::GetFileSourceL( *iVideoOutSession );
    
    EUNIT_ASSERT( file )
    EUNIT_ASSERT( file->Type() == KMceFileSource )
    
    
    }
   

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetDisplayL()
    {
    CMceDisplaySink* display = MusEngMceUtils::GetDisplay( *iEmptySession );
    
    EUNIT_ASSERT( !display )
    
    display = MusEngMceUtils::GetDisplay( *iVideoOutSession );
    
    EUNIT_ASSERT( display )
    EUNIT_ASSERT( display->Type() == KMceDisplaySink )
    }  


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetDisplayLL()
    {
    CMceDisplaySink* display = NULL;
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( MusEngMceUtils::GetDisplayL( *iEmptySession ),
                                 KErrNotFound )
    
    display = MusEngMceUtils::GetDisplayL( *iVideoOutSession );
    
    EUNIT_ASSERT( display )
    EUNIT_ASSERT( display->Type() == KMceDisplaySink )
    }  


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_AddDisplayLL()
    {
   
    TRect rect( 110, 111, 112, 113 );
    
    // Check that display cannot be added to audio stream
    
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    EUNIT_ASSERT_SPECIFIC_LEAVE (
                MusEngMceUtils::AddDisplayL( *audioStream, *iManager, rect ),
                KErrArgument )
    CleanupStack::PopAndDestroy( audioStream );
    
    // Check that display can be added to video stream
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    
    MusEngMceUtils::AddDisplayL( *videoStream, *iManager, rect );
    
    EUNIT_ASSERT_EQUALS( videoStream->Sinks().Count(), 1 )
    EUNIT_ASSERT_EQUALS( videoStream->Sinks()[0]->Type(), KMceDisplaySink )
    EUNIT_ASSERT( videoStream->Sinks()[0]->IsEnabled() )
    
    // Check the rect
    CMceDisplaySink* display = 
                    static_cast<CMceDisplaySink*>(videoStream->Sinks()[0]);
    
    EUNIT_ASSERT_EQUALS( display->DisplayRectL(), rect );
    
    // Enable display and check that another display is not added on request.
    // Still existing display must be enabled and new rect set
    display->EnableL();
    TRect newRect( 115, 116, 117, 118 );
    MusEngMceUtils::AddDisplayL( *videoStream, *iManager, newRect );
    
    EUNIT_ASSERT_EQUALS( videoStream->Sinks().Count(), 1 )
    EUNIT_ASSERT_EQUALS( videoStream->Sinks()[0]->Type(), KMceDisplaySink )
    EUNIT_ASSERT( videoStream->Sinks()[0]->IsEnabled() )
    
    EUNIT_ASSERT( videoStream->Sinks()[0] == display )
    
    // Check that rect is still changed
    EUNIT_ASSERT_EQUALS( display->DisplayRectL(), newRect );
    
    CleanupStack::PopAndDestroy( videoStream );
    
    // Add disabled display
    CMceVideoStream* videoStream2 = CMceVideoStream::NewLC();
    MusEngMceUtils::AddDisplayL( *videoStream2, *iManager, rect, ETrue );
    EUNIT_ASSERT_EQUALS( videoStream2->Sinks().Count(), 1 )
    EUNIT_ASSERT_EQUALS( videoStream2->Sinks()[0]->Type(), KMceDisplaySink )
    EUNIT_ASSERT( !videoStream2->Sinks()[0]->IsEnabled() );
    CleanupStack::PopAndDestroy( videoStream2 );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_AddSpeakerLL()
    {
    // Check that speaker cannot be added to video stream
    
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    EUNIT_ASSERT_SPECIFIC_LEAVE (
                MusEngMceUtils::AddSpeakerL( *videoStream ),
                KErrArgument )
    CleanupStack::PopAndDestroy( videoStream );
    
    // Check that speaker can be added to audio stream
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    
    MusEngMceUtils::AddSpeakerL( *videoStream );
    
    EUNIT_ASSERT_EQUALS( audioStream->Sinks().Count(), 1 )
    EUNIT_ASSERT_EQUALS( audioStream->Sinks()[0]->Type(), KMceSpeakerSink )
    EUNIT_ASSERT( audioStream->Sinks()[0]->IsEnabled() )
    
    // Enable speaker and check that another speaker is not added on request
    // Still existing speaker must be enabled
    CMceSpeakerSink* speaker = 
                    static_cast<CMceSpeakerSink*>(videoStream->Sinks()[0]);

    speaker->EnableL();

    MusEngMceUtils::AddSpeakerL( *audioStream );
    
    EUNIT_ASSERT_EQUALS( audioStream->Sinks().Count(), 1 )
    EUNIT_ASSERT_EQUALS( audioStream->Sinks()[0]->Type(), KMceSpeakerSink )
    EUNIT_ASSERT( audioStream->Sinks()[0]->IsEnabled() )
    
    EUNIT_ASSERT( audioStream->Sinks()[0] == speaker )
    
    CleanupStack::PopAndDestroy( audioStream );
    }    


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_DisableStreamLL()
    {
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();

    // Disable only stream
    EUNIT_ASSERT( audioStream->IsEnabled() )
    MusEngMceUtils::DisableStreamL( *audioStream );
    EUNIT_ASSERT( !audioStream->IsEnabled() )
    
    audioStream->EnableL();
    
    // Disable stream and source
    CMceRtpSource* rtpSource = CMceRtpSource::NewLC();
    audioStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource );
    
    EUNIT_ASSERT( audioStream->IsEnabled() )
    EUNIT_ASSERT( rtpSource->IsEnabled() )
    MusEngMceUtils::DisableStreamL( *audioStream );
    EUNIT_ASSERT( !audioStream->IsEnabled() )
    EUNIT_ASSERT( !rtpSource->IsEnabled() )
    
    audioStream->EnableL();
    rtpSource->EnableL();
    
    // Disable stream, source and sinks
    CMceSpeakerSink* speaker =  CMceSpeakerSink::NewLC();
    audioStream->AddSinkL( speaker );
    CleanupStack::Pop( speaker );
    
    EUNIT_ASSERT( audioStream->IsEnabled() )
    EUNIT_ASSERT( rtpSource->IsEnabled() )
    EUNIT_ASSERT( speaker->IsEnabled() )
    MusEngMceUtils::DisableStreamL( *audioStream );
    EUNIT_ASSERT( !audioStream->IsEnabled() )
    EUNIT_ASSERT( !rtpSource->IsEnabled() )
    EUNIT_ASSERT( !speaker->IsEnabled() )
    
    CleanupStack::PopAndDestroy( audioStream );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetReceivingDisplayL()
    {
    CMceDisplaySink* display = MusEngMceUtils::GetReceivingDisplay( *iEmptySession );
    
    EUNIT_ASSERT( !display )
    
    display = MusEngMceUtils::GetReceivingDisplay( *iVideoOutSession );
    
    EUNIT_ASSERT( display )
    EUNIT_ASSERT( display->Type() == KMceDisplaySink )
    EUNIT_ASSERT( display == iReceivingDisplay )
    }  


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetReceivingDisplayLL()
    {
    CMceDisplaySink* display = NULL;
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( MusEngMceUtils::GetReceivingDisplayL( *iEmptySession ),
                                 KErrNotFound )
    
    display = MusEngMceUtils::GetReceivingDisplayL( *iVideoOutSession );
    
    EUNIT_ASSERT( display )
    EUNIT_ASSERT( display->Type() == KMceDisplaySink )
    EUNIT_ASSERT( display == iReceivingDisplay )
    }  

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_GetVfDisplayL()
    {
    CMceDisplaySink* display = MusEngMceUtils::GetVfDisplay( *iEmptySession );
    
    EUNIT_ASSERT( !display )
    
    display = MusEngMceUtils::GetVfDisplay( *iVideoOutSession );
    
    EUNIT_ASSERT( display )
    EUNIT_ASSERT( display->Type() == KMceDisplaySink )
    EUNIT_ASSERT( display != iReceivingDisplay )
    } 

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusEngMceUtils::UT_EnableInactivityTimerL()
    {
    // Instream not found, nothing is done
    EUNIT_ASSERT_EQUALS( MusEngMceUtils::EnableInactivityTimer( *iEmptySession, 1000 ), KErrNotFound );

    // In stream found
    EUNIT_ASSERT_EQUALS( MusEngMceUtils::EnableInactivityTimer( *iVideoOutSession, 1000 ), KErrNone );
    } 

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_MusEngMceUtils,
    "UT_MusEngMceUtils",
    "UNIT" )

EUNIT_TEST(
    "IsVideoInStream - test ",
    "MusEngMceUtils",
    "IsVideoInStream",
    "FUNCTIONALITY",
    SetupL, UT_IsVideoInStreamL, Teardown)

EUNIT_TEST(
    "IsVideoOutStream - test ",
    "MusEngMceUtils",
    "IsVideoOutStream",
    "FUNCTIONALITY",
    SetupL, UT_IsVideoOutStreamL, Teardown)
    
EUNIT_TEST(
    "IsAudioInStream - test ",
    "MusEngMceUtils",
    "IsAudioInStream",
    "FUNCTIONALITY",
    SetupL, UT_IsAudioInStreamL, Teardown)

EUNIT_TEST(
    "GetVideoOutStreamL - test ",
    "MusEngMceUtils",
    "GetVideoOutStreamL",
    "FUNCTIONALITY",
    SetupL, UT_GetVideoOutStreamLL, Teardown)

EUNIT_TEST(
    "GetVideoInStreamL - test ",
    "MusEngMceUtils",
    "GetVideoInStreamL",
    "FUNCTIONALITY",
    SetupL, UT_GetVideoInStreamLL, Teardown)

EUNIT_TEST(
    "GetRecordingStream - test ",
    "MusEngMceUtils",
    "GetRecordingStream",
    "FUNCTIONALITY",
    SetupL, UT_GetRecordingStreamL, Teardown)

EUNIT_TEST(
    "GetMediaSink - test ",
    "MusEngMceUtils",
    "GetMediaSink",
    "FUNCTIONALITY",
    SetupL, UT_GetMediaSinkFromStreamL, Teardown)

EUNIT_TEST(
    "GetMediaSinkL - test ",
    "MusEngMceUtils",
    "GetMediaSinkL",
    "FUNCTIONALITY",
    SetupL, UT_GetMediaSinkLFromStreamL, Teardown)

EUNIT_TEST(
    "GetMediaSink2 - test ",
    "MusEngMceUtils",
    "GetMediaSink2",
    "FUNCTIONALITY",
    SetupL, UT_GetMediaSinkFromSessionL, Teardown)

EUNIT_TEST(
    "GetMediaSinkL2 - test ",
    "MusEngMceUtils",
    "GetMediaSinkL2",
    "FUNCTIONALITY",
    SetupL, UT_GetMediaSinkLFromSessionL, Teardown)

EUNIT_TEST(
    "GetCameraL - test ",
    "MusEngMceUtils",
    "GetCameraL",
    "FUNCTIONALITY",
    SetupL, UT_GetCameraLL, Teardown)

EUNIT_TEST(
    "GetFileSourceL - test ",
    "MusEngMceUtils",
    "GetFileSourceL",
    "FUNCTIONALITY",
    SetupL, UT_GetFileSourceLL, Teardown)

EUNIT_TEST(
    "GetDisplay - test ",
    "MusEngMceUtils",
    "GetDisplay",
    "FUNCTIONALITY",
    SetupL, UT_GetDisplayL, Teardown)

EUNIT_TEST(
    "GetDisplayL - test ",
    "MusEngMceUtils",
    "GetDisplayL",
    "FUNCTIONALITY",
    SetupL, UT_GetDisplayLL, Teardown)

EUNIT_TEST(
    "AddDisplayL - test ",
    "MusEngMceUtils",
    "AddDisplayL",
    "FUNCTIONALITY",
    SetupL, UT_AddDisplayLL, Teardown)

EUNIT_TEST(
    "AddSpeakerL - test ",
    "MusEngMceUtils",
    "AddSpeakerL",
    "FUNCTIONALITY",
    SetupL, UT_AddSpeakerLL, Teardown)    

EUNIT_TEST(
    "DisableStreamL - test ",
    "MusEngMceUtils",
    "DisableStreamL",
    "FUNCTIONALITY",
    SetupL, UT_DisableStreamLL, Teardown)        

EUNIT_TEST(
    "GetReceivingDisplay - test ",
    "MusEngMceUtils",
    "GetReceivingDisplay",
    "FUNCTIONALITY",
    Setup2L, UT_GetReceivingDisplayL, Teardown)

EUNIT_TEST(
    "GetReceivingDisplayL - test ",
    "MusEngMceUtils",
    "GetReceivingDisplayL",
    "FUNCTIONALITY",
    Setup2L, UT_GetReceivingDisplayLL, Teardown)
    
EUNIT_TEST(
    "GetVfDisplay - test ",
    "MusEngMceUtils",
    "GetVfDisplay",
    "FUNCTIONALITY",
    Setup2L, UT_GetVfDisplayL, Teardown)

EUNIT_TEST(
    "EnableInactivityTimer - test ",
    "MusEngMceUtils",
    "EnableInactivityTimer",
    "FUNCTIONALITY",
    Setup2L, UT_EnableInactivityTimerL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE


