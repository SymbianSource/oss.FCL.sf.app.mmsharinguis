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


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
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
    iObserver = new( ELeave ) CMusEngObserverStub;
    iClipSession = CMusEngClipSession::NewL( TRect(0,0, 100,100),
                                             *iObserver,
                                             *iObserver,
                                             *iObserver );
    iClipSession->SetClipL( KTestVideoFileName );
    iClipSession->iVideoCodecList = KMceSDPNameH264().AllocL();

    SIPStrings::OpenL();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::Setup2L()
    {
    iObserver = new( ELeave ) CMusEngObserverStub;
    iClipSession = CMusEngClipSession::NewL( TRect(0,0, 100,100),
                                             *iObserver,
                                             *iObserver,
                                             *iObserver );
    iClipSession->SetClipL( KTestVideoFileName );
    iClipSession->iVideoCodecList = KMceSDPNameH263().AllocL();

    SIPStrings::OpenL();
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::Teardown()
    {
    SIPStrings::Close();
    delete iClipSession;
    delete iObserver;
    }



// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_NewLL()
    {
    EUNIT_ASSERT( iClipSession )
    EUNIT_ASSERT( iClipSession->iFileName != KNullDesC() )
    EUNIT_ASSERT( !iClipSession->iSession )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_SetClipLL()
    {
    // Try with protected file, cannot use EUNIT_ASSERT_SPECIFIC_LEAVE
    TRAPD( error, iClipSession->SetClipL( KMusDrmProtectedFileName() ) )
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrPermissionDenied )
    EUNIT_ASSERT_EQUALS( iClipSession->iFileName, KTestVideoFileName() )

    // Change the file before session is established
    iClipSession->SetClipL( KTestAvcVideoFileName() );
    EUNIT_ASSERT_EQUALS( iClipSession->iFileName, KTestAvcVideoFileName() )
    EUNIT_ASSERT( !iClipSession->iSession )
    
    // simulate session establishment
    ESTABLISH_OUT_SESSION( iClipSession );
    
    // Now test with established session
    iClipSession->SetClipL( KTestVideoFileName() );
    
    CMceFileSource* file = 
                    MusEngMceUtils::GetFileSourceL( *iClipSession->iSession );
    
    EUNIT_ASSERT_EQUALS( iClipSession->iFileName, KTestVideoFileName() )
    EUNIT_ASSERT_EQUALS( file->iFileName, KTestVideoFileName() )
    
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_FastForwardLL()
    {
    // Try before establishment
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->FastForwardL( ETrue ),
                                 KErrNotReady )
    
    // Establish session, simulate position and duration and try again   
    ESTABLISH_OUT_SESSION( iClipSession );
    
    CMceFileSource* file = 
                    MusEngMceUtils::GetFileSourceL( *iClipSession->iSession );
                    
    file->iDuration = KMusEngTestFileDuration;
    file->iPosition = KMusEngTestFilePosition; 
    
    iClipSession->FastForwardL( ETrue );

    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() > 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() == 0 )
    
    // Simulate fastforwarding for a while
    User::After( 1000 );

    // Try to fastforward when already fastforwarding, will be ignored
    iClipSession->FastForwardL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() > 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() == 0 )
    
    // Stop fastforwarding
    iClipSession->FastForwardL( EFalse );
    EUNIT_ASSERT( file->iPosition > KMusEngTestFilePosition )
    EUNIT_ASSERT( file->iPosition != file->iDuration )
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() == 0 )
    
    // Try to stop fastforwarding again, leaves 
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->FastForwardL( EFalse ),
                                 KErrAlreadyExists )
    
    // Start fastrewinding
    iClipSession->FastRewindL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() > 0 )
    
    // Start fastforwarding, rewinding should be stopped and FFWD started
    iClipSession->FastForwardL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() > 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() == 0 )
    
    // Simulate fastforwarding for a while
    User::After( 1000 );
    
    // Simulate that clip is almost in end and fastforwarding would have 
    // continued over end, position should be set to duration.
    
    file->iPosition = TTimeIntervalMicroSeconds( file->iDuration.Int64() - 1 );
    
    iClipSession->FastForwardL( EFalse );
    EUNIT_ASSERT( file->iPosition == file->iDuration )
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() == 0 )
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_FastRewindLL()
    {
    // Try before establishment
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->FastRewindL( ETrue ),
                                 KErrNotReady )
    
    // Establish session, simulate position and duration and try again   
    ESTABLISH_OUT_SESSION( iClipSession );
    
    CMceFileSource* file = 
                    MusEngMceUtils::GetFileSourceL( *iClipSession->iSession );
                    
    file->iDuration = KMusEngTestFileDuration;
    file->iPosition = KMusEngTestFilePosition; 
    
    iClipSession->FastRewindL( ETrue );

    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() > 0 )
    
    // Simulate fastrewinding for a while
    User::After( 1000 );
    
    // Try to fastrewind when already fastrewinding, will be ignored
    iClipSession->FastRewindL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() > 0 )
    
    // Stop fastrewinding
    iClipSession->FastRewindL( EFalse );
    EUNIT_ASSERT( file->iPosition < KMusEngTestFilePosition )
    EUNIT_ASSERT( file->iPosition != TTimeIntervalMicroSeconds( 0 ) )
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() == 0 )
    
    // Try to stop fastrewinding again, leaves 
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->FastRewindL( EFalse ),
                                 KErrAlreadyExists )
    
    // Start fastforwarding
    iClipSession->FastForwardL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() >= 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() == 0 )
    
    // Start fastrewinding, forwarding should be stopped and FFWD started
    iClipSession->FastRewindL( ETrue );
    EUNIT_ASSERT( !file->iIsEnabled );
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() >= 0 )
    
    // Simulate that clip has just begun and fastrewinding would have 
    // continued over beginning, position should be set to zero.
    
    file->iPosition = TTimeIntervalMicroSeconds( 1 );
    
    // Simulate fastrewinding for a while
    User::After( 1000 );
    
    iClipSession->FastRewindL( EFalse );
    EUNIT_ASSERT( file->iPosition == TTimeIntervalMicroSeconds( 0 ) )
    EUNIT_ASSERT( !file->iIsEnabled )
    EUNIT_ASSERT( iClipSession->iFFWDStartTime.Int64() == 0 )
    EUNIT_ASSERT( iClipSession->iFRWDStartTime.Int64() == 0 ) 
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_PositionLL()
    {
    // Try before establishment
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->PositionL(), KErrNotReady )
    
    // Establish session and try again
    ESTABLISH_OUT_SESSION( iClipSession );
    
    CMceFileSource* file = 
                    MusEngMceUtils::GetFileSourceL( *iClipSession->iSession );
    file->iPosition = 2000000;
    
    EUNIT_ASSERT( iClipSession->PositionL().Int() * 1000000 == 
                  file->iPosition.Int64() )
    
    // Ask position while fastforwarding, it should be bigger than real position
    iClipSession->FastForwardL( ETrue );
    
    User::After( 1000000 ); // We have to wait since dividing in PositionL and 
                            // multiplying before comparison loses difference 
    
    EUNIT_ASSERT( iClipSession->PositionL().Int() * 1000000 >
                  file->iPosition.Int64() )
    
    // Ask position while fastforwarding beyond end of clip, clip duration is
    // returned
    file->iPosition = file->iDuration;
    
    EUNIT_ASSERT( iClipSession->PositionL().Int() * 1000000 ==
                  file->iDuration.Int64() )
    
    // Stop fastforwarding, start fastrewinding, position is set to the end clip
    iClipSession->FastForwardL( EFalse );
    iClipSession->FastRewindL( ETrue );
    
    User::After( 1000000 ); // We have to wait since dividing in PositionL and 
                            // multiplying before comparison loses difference 
    
    // Ask position while fastrewinding, it should be smaller than real 
    // position
    EUNIT_ASSERT( iClipSession->PositionL().Int() * 1000000 <
                  file->iPosition.Int64() )
    
    // Ask position while fastrewinding beyond the beginning of clip, zero 
    // returned
    file->iPosition = 0;
    
    EUNIT_ASSERT( iClipSession->PositionL().Int() * 1000000 == 0 )
    
    // Asking position when rewinded to beginning but rewinding has ended 
    // and clip has not ended (position should not be altered in that case)
    iClipSession->iRewindedToBeginning = ETrue;
    file->iPosition = 0;
    iClipSession->iFRWDStartTime = TTime( 0 );
    
    EUNIT_ASSERT( iClipSession->PositionL().Int() * 1000000 == 0 )
    
    // Position has proceeded from beginning, rewinding to beginning info
    // is cleared.
    file->iPosition = 10000000;
    iClipSession->iFRWDStartTime = TTime( 0 );
    
    EUNIT_ASSERT( iClipSession->PositionL().Int() * 1000000 == 10000000 )
    EUNIT_ASSERT( iClipSession->iRewindedToBeginning == EFalse )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_DurationLL()
    {
    // Try before establishment
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->DurationL(), KErrNotReady )
    
    // Establish session and try again
    ESTABLISH_OUT_SESSION( iClipSession );
    
    CMceFileSource* file = 
                    MusEngMceUtils::GetFileSourceL( *iClipSession->iSession );
    file->iDuration = 2000000;
    
    EUNIT_ASSERT( iClipSession->DurationL().Int() * 1000000 == 
                  file->iDuration.Int64() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_SetPositionLL()
    {
    TTimeIntervalSeconds time( 20 );
    
    // Try before establishment
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->SetPositionL( time ),
                                 KErrNotReady )
    
    // Normal case with already disabled file source
    ESTABLISH_OUT_SESSION( iClipSession );
    
    CMceFileSource* file = 
                MusEngMceUtils::GetFileSourceL( *iClipSession->iSession ); 
    file->iIsEnabled = EFalse;
    
    iClipSession->SetPositionL( time );
    
    EUNIT_ASSERT( file->iPosition.Int64() == 
                  static_cast<TInt64>(time.Int()) * 1000000 )
    EUNIT_ASSERT( !file->iIsEnabled )
    
    // Normal case with enabled file source
    TTimeIntervalSeconds anotherTime( 30 );

    file->iIsEnabled = ETrue;
    iClipSession->SetPositionL( anotherTime );
    
    EUNIT_ASSERT( file->iPosition.Int64() == 
                  static_cast<TInt64>(anotherTime.Int()) * 1000000 )
    EUNIT_ASSERT( file->iIsEnabled )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_TranscodeLToAvcL()
    {
    // Check that transcoding is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                    iClipSession->TranscodeL( KTestVideoFileName() ),
                    KErrNotReady )
    
    // Construct session with video and audio streams that must transcoded 
    
    CSIPProfile* profile = iClipSession->iSipProfileHandler->Profile();
  
    iClipSession->iSession = CMceOutSession::NewL( 
                                    *(iClipSession->iManager),
                                    *profile,
                                    KTestRecipientSipUri8() );
                             
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();

    CMceRtpSink* rtpsink = CMceRtpSink::NewLC();
    videoStream->AddSinkL( rtpsink );
    CleanupStack::Pop( rtpsink );

    CMceFileSource* fileSource = 
        CMceFileSource::NewLC( *iClipSession->iManager, KTestAvcVideoFileName() );
    videoStream->SetSourceL( fileSource );                            
    CleanupStack::Pop( fileSource );

    iClipSession->iSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );
    
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
        
    audioStream->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    
    audioStream->SetSourceL( fileSource );
    
    iClipSession->iSession->AddStreamL( audioStream );
    CleanupStack::Pop( audioStream );
    
    videoStream->iState = CMceMediaStream::ETranscodingRequired;
    audioStream->iState = CMceMediaStream::ETranscodingRequired; 
    
    // Remove all codecs (file has some unknown codec type)
    RPointerArray<CMceVideoCodec> videoCodecs = videoStream->Codecs();
    for ( TInt i = 0; i < videoCodecs.Count(); i++ )
        {
        videoStream->RemoveCodecL( *videoCodecs[ i ] );
        }
    RPointerArray<CMceVideoCodec> videoCodecs2 = videoStream->Codecs();
    EUNIT_ASSERT_EQUALS( videoCodecs2.Count(), 0 )
    EUNIT_ASSERT( audioStream->Codecs().Count() > 1 )  
    
    // Add some stream which must not be transcoded
    
    CMceAudioStream* inStream = CMceAudioStream::NewLC();
        
    inStream->AddSinkL( CMceSpeakerSink::NewLC() );
    CleanupStack::Pop();
    
    inStream->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop();
    
    iClipSession->iSession->AddStreamL( inStream );
    CleanupStack::Pop( inStream );

    // Transcode
    iClipSession->TranscodeL( KTestAvcVideoFileName() );
    
    // Check that transcoding has begun (transcoding to AVC as we know
    // that other end supports it
    EUNIT_ASSERT( iClipSession->iTranscodingOngoing )
    EUNIT_ASSERT( videoStream->State() == CMceMediaStream::ETranscoding )
    EUNIT_ASSERT( audioStream->State() == CMceMediaStream::ETranscoding )
    EUNIT_ASSERT( inStream->State() != CMceMediaStream::ETranscoding )
    
    // Check that codecs have been replaced
    const RPointerArray<CMceVideoCodec> videoCodecs3 = videoStream->Codecs();
    EUNIT_ASSERT_EQUALS( videoCodecs3.Count(), 1 )
    EUNIT_ASSERT( videoCodecs3[0]->SdpName().FindF( KMceSDPNameH264() ) >= 0 )
    EUNIT_ASSERT( audioStream->Codecs().Count() == 1 )  
    EUNIT_ASSERT( audioStream->Codecs()[0]->AllowedBitrates() == 
                  KMceAllowedAmrNbBitrate475 )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_TranscodeLToH263L()
    {
    // Check that transcoding is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                    iClipSession->TranscodeL( KTestVideoFileName() ),
                    KErrNotReady )
    
    // Construct session with video and audio streams that must transcoded 
    
    CSIPProfile* profile = iClipSession->iSipProfileHandler->Profile();
  
    iClipSession->iSession = CMceOutSession::NewL( 
                                    *(iClipSession->iManager),
                                    *profile,
                                    KTestRecipientSipUri8() );
                             
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();

    CMceRtpSink* rtpsink = CMceRtpSink::NewLC();
    videoStream->AddSinkL( rtpsink );
    CleanupStack::Pop( rtpsink );

    CMceFileSource* fileSource = 
        CMceFileSource::NewLC( *iClipSession->iManager, KTestAvcVideoFileName() );
    videoStream->SetSourceL( fileSource );                            
    CleanupStack::Pop( fileSource );

    iClipSession->iSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );
    
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
        
    audioStream->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    
    audioStream->SetSourceL( fileSource );
    
    iClipSession->iSession->AddStreamL( audioStream );
    CleanupStack::Pop( audioStream );
    
    videoStream->iState = CMceMediaStream::ETranscodingRequired;
    audioStream->iState = CMceMediaStream::ETranscodingRequired; 
    
    const RPointerArray<CMceVideoCodec> videoCodecs = videoStream->Codecs();
    EUNIT_ASSERT_EQUALS( videoCodecs.Count(), 1 )
    EUNIT_ASSERT( videoCodecs[0]->SdpName().FindF( KMceSDPNameH264() ) >= 0 )
    EUNIT_ASSERT( audioStream->Codecs().Count() > 1 )  
    
    TSize resolution(200,200); // Some value
    videoStream->Codecs()[0]->SetResolutionL( resolution );
    audioStream->Codecs()[0]->SetBitrate( KMceAllowedAmrNbBitrateAll );
    
    // Add some stream which must not be transcoded
    
    CMceAudioStream* inStream = CMceAudioStream::NewLC();
        
    inStream->AddSinkL( CMceSpeakerSink::NewLC() );
    CleanupStack::Pop();
    
    inStream->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop();
    
    iClipSession->iSession->AddStreamL( inStream );
    CleanupStack::Pop( inStream );

    // Transcode
    iClipSession->TranscodeL( KTestAvcVideoFileName() );
    
    // Check that transcoding has begun (transcoding to H263 as we don't
    // know whether other end supports H264)    EUNIT_ASSERT( iClipSession->iTranscodingOngoing )
    EUNIT_ASSERT( videoStream->State() == CMceMediaStream::ETranscoding )
    EUNIT_ASSERT( audioStream->State() == CMceMediaStream::ETranscoding )
    EUNIT_ASSERT( inStream->State() != CMceMediaStream::ETranscoding )
    
    // Check that codecs have been replaced
    const RPointerArray<CMceVideoCodec> videoCodecs2 = videoStream->Codecs();
    EUNIT_ASSERT_EQUALS( videoCodecs2.Count(), 1 )
    EUNIT_ASSERT( videoCodecs2[0]->SdpName().FindF( KMceSDPNameH263() ) >= 0 )
    EUNIT_ASSERT( audioStream->Codecs().Count() == 1 )  
    
    EUNIT_ASSERT( videoStream->Codecs()[0]->Resolution() != resolution )
    EUNIT_ASSERT( audioStream->Codecs()[0]->AllowedBitrates() == 
                  KMceAllowedAmrNbBitrate475 )
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngClipSession::UT_CancelTranscodeLL()
    {
    // Check that canceling transcoding is not possible before actual
    // transcoding
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->CancelTranscodeL(),
                                 KErrNotReady )
    
    // Construct session structure
    
    CSIPProfile* profile = iClipSession->iSipProfileHandler->Profile();
  
    iClipSession->iSession = CMceOutSession::NewL( 
                                    *(iClipSession->iManager),
                                    *profile,
                                    KTestRecipientSipUri8() );
                             
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();

    CMceRtpSink* rtpsink = CMceRtpSink::NewLC();
    videoStream->AddSinkL( rtpsink );
    CleanupStack::Pop( rtpsink );

    CMceFileSource* fileSource = 
        CMceFileSource::NewLC( *iClipSession->iManager, KTestVideoFileName() );
    videoStream->SetSourceL( fileSource );                            
    CleanupStack::Pop( fileSource );

    iClipSession->iSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );
    
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
        
    audioStream->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
    
    audioStream->SetSourceL( fileSource );
    
    iClipSession->iSession->AddStreamL( audioStream );
    CleanupStack::Pop( audioStream );
    
    // Set need for transcoding
    videoStream->iState = CMceMediaStream::ETranscodingRequired;
    audioStream->iState = CMceMediaStream::ETranscodingRequired; 

    // Transcode
    iClipSession->TranscodeL( KTestAvcVideoFileName() );

    EUNIT_ASSERT( videoStream->State() == CMceMediaStream::ETranscoding )
    EUNIT_ASSERT( audioStream->State() == CMceMediaStream::ETranscoding )
        
    // Cancel
    iClipSession->CancelTranscodeL();
    
    EUNIT_ASSERT( videoStream->State() == 
                  CMceMediaStream::ETranscodingRequired )
    EUNIT_ASSERT( audioStream->State() == 
                  CMceMediaStream::ETranscodingRequired )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngClipSession::UT_PlayLL()
    {
    // Check that resuming is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->PlayL(), KErrNotReady )
    
    ESTABLISH_OUT_SESSION( iClipSession );

    // Check that playing is not possible during FFWD
    iClipSession->iFFWDStartTime = TTime( 10 );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->PlayL(), KErrNotReady )
    iClipSession->iFFWDStartTime = TTime( 0 );
 
    // Check that playing is not possible during FRWD
    iClipSession->iFRWDStartTime = TTime( 10 );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->PlayL(), KErrNotReady )
    iClipSession->iFRWDStartTime = TTime( 0 );

    // Successful case
    CMceFileSource* file = 
            MusEngMceUtils::GetFileSourceL( *(iClipSession->iSession) );

    file->iIsEnabled = EFalse;

    iClipSession->PlayL();

    EUNIT_ASSERT( file->IsEnabled() )
    
    // Try to play again, request should be ignored
    
    iClipSession->PlayL();

    EUNIT_ASSERT( file->IsEnabled() )

    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngClipSession::UT_PauseLL()
    {
    // Check that pausing is not possible before invite
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->PauseL(), KErrNotReady )
    
    ESTABLISH_OUT_SESSION( iClipSession );

    // Check that pausing is not possible during FFWD
    iClipSession->iFFWDStartTime = TTime( 10 );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->PauseL(), KErrNotReady )
    iClipSession->iFFWDStartTime = TTime( 0 );
 
    // Check that pausing is not possible during FRWD
    iClipSession->iFRWDStartTime = TTime( 10 );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->PauseL(), KErrNotReady )
    iClipSession->iFRWDStartTime = TTime( 0 );

    // Successful case
    
    CMceFileSource* file = 
            MusEngMceUtils::GetFileSourceL( *(iClipSession->iSession) );

    file->iIsEnabled = ETrue;
    
    iClipSession->PauseL();

    EUNIT_ASSERT( !file->IsEnabled() )
    
    // Try to pause again, request should be ignored
    
    iClipSession->PauseL();

    EUNIT_ASSERT( !file->IsEnabled() )

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngClipSession::UT_IsPlayingLL()
    {
    // Try without a session 
    EUNIT_ASSERT_SPECIFIC_LEAVE( iClipSession->IsPlayingL(), KErrNotReady );
    
    // Normal cases
    ESTABLISH_OUT_SESSION( iClipSession );
    
    iClipSession->PlayL();
    EUNIT_ASSERT( iClipSession->IsPlayingL() )
    
    iClipSession->PauseL();
    EUNIT_ASSERT( !iClipSession->IsPlayingL() )
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
    
    // Check that structure cannot be completed before setting the file name
    iClipSession->iFileName = KNullDesC();          
    CSIPProfile* profile = iClipSession->iSipProfileHandler->Profile();
  
    iClipSession->iSession = CMceOutSession::NewL( 
                                    *(iClipSession->iManager),
                                    *profile,
                                    KTestRecipientSipUri8() );

    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                iClipSession->CompleteSessionStructureL( *localBundle ),
                KErrNotReady )
    
    // Normal case
    iClipSession->iFileName = KTestVideoFileName();                     
    iClipSession->CompleteSessionStructureL( *localBundle );
    
    EUNIT_ASSERT( iClipSession->iSession->Streams().Count() == 3 )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[0]->Type() == KMceVideo )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[0]->Source() )
    EUNIT_ASSERT( iClipSession->iSession->Streams()[0]->Source()->Type() ==
                  KMceFileSource )
    EUNIT_ASSERT( !iClipSession->IsPlayingL() )              
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
    iClipSession->InviteL( KTestRecipientSipUri() );
   
    // Try all the stream states
    CMceMediaStream* changedStream = iClipSession->iSession->Streams()[0];
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->iStreamStreamingCalled )
    iObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() )
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
    iClipSession->InviteL( KTestRecipientSipUri() );
    
    // Try all the non-default stream states
    CMceMediaStream* changedStream = iClipSession->iSession->Streams()[0];
    CMceMediaSource* changedSource = changedStream->Source();
    
    // EDisabled, end of clip
    changedStream->iState = CMceMediaStream::EDisabled;
    changedSource->iIsEnabled = EFalse;
    iClipSession->StreamStateChanged( *changedStream, *changedSource );
    
    EUNIT_ASSERT( iObserver->iEndOfClipCalled == ETrue )
    changedSource->iIsEnabled = ETrue;
    iObserver->Reset();
    
    // ETranscodingRequired, transcoding has failed
    iClipSession->iSession->iState = CMceSession::EIdle;
    iClipSession->iTranscodingOngoing = ETrue;
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->iTranscodingFailedCalled )
    EUNIT_ASSERT( !iClipSession->iTranscodingOngoing )
    iObserver->Reset();
    
    // ETranscoding, transcoding has progresssed
    iClipSession->iSession->iState = CMceSession::EIdle;
    changedStream->iState = CMceMediaStream::ETranscoding;
    static_cast<CMceFileSource*>(changedSource)->iTranscodingPercentage = 20;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->iTranscodingProgressedPercentage == 20 )
    iObserver->Reset();
    
    // ETranscoding, transcoding has progresssed, querying percentage fails
    iClipSession->iSession->iState = CMceSession::EIdle;
    changedStream->iState = CMceMediaStream::ETranscoding;
    iObserver->iTranscodingProgressedPercentage = -1; // make assertion possible
    static_cast<CMceFileSource*>(changedSource)->iFailWithCode = KErrNotReady;
    static_cast<CMceFileSource*>(changedSource)->iTranscodingPercentage = 30;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->iTranscodingProgressedPercentage == 0 )
    iObserver->Reset();

    // EInitialized, transcoding has completed, establishment fails
    iClipSession->iSession->iState = CMceSession::EIdle;
    iClipSession->iSession->iFailWithCode = KErrCorrupt; // != KErrNone
    iClipSession->iTranscodingOngoing = ETrue;
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<CMceFileSource*>(changedSource)->iTranscodingPercentage = 100;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->iTranscodingCompletedInitCalled )
    EUNIT_ASSERT( iObserver->iTranscodingCompletedFinalizeCalled )
    EUNIT_ASSERT( iObserver->iSessionFailedCalled )
    EUNIT_ASSERT( !iClipSession->iTranscodingOngoing )
    iObserver->Reset();
    
    // EInitialized, transcoding has completed, establishment succeeds
    iClipSession->iSession->iState = CMceSession::EIdle;
    iClipSession->iTranscodingOngoing = ETrue;
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<CMceFileSource*>(changedSource)->iTranscodingPercentage = 100;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->iTranscodingCompletedInitCalled )
    EUNIT_ASSERT( iObserver->iTranscodingCompletedFinalizeCalled )    
    // Next cannot be asserted since it is not true with alloc decoration
    // EUNIT_ASSERT( !iObserver->iSessionFailedCalled )  
    EUNIT_ASSERT( !iClipSession->iTranscodingOngoing )
    iObserver->Reset();
    
    
    // Test default stream state change behavior, remove or change when
    // behavior changes
    
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() )

    // Special meaning (Transcoding ready), tested separately
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->iStreamStreamingCalled )
    iObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    // This state has non-default meaning, tested before defaults
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                            *changedStream, *changedSource );
    EUNIT_ASSERT( iObserver->IsReseted() )

    
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
    iClipSession->InviteL( KTestRecipientSipUri() );
                                     
    // Test default stream state change behavior
    CMceMediaStream* changedStream = iClipSession->iSession->Streams()[0];
    CMceMediaSink* changedSink = changedStream->Sinks()[0];

    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iObserver->iStreamStreamingCalled )
    iObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() )
                                                            
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    static_cast<MMceStreamObserver*>(iClipSession)->StreamStateChanged( 
                                                            *changedStream,
                                                            *changedSink );
    EUNIT_ASSERT( iObserver->IsReseted() )
    
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
void UT_CMusEngClipSession::UT_HasClipEndedL()
    {
    // Try before establishing the session
    EUNIT_ASSERT( !iClipSession->HasClipEnded() )
    
    // Try with session, but without video out stream 
    
    CSIPProfile* profile = iClipSession->iSipProfileHandler->Profile();
  
    iClipSession->iSession = CMceOutSession::NewL( 
                                    *(iClipSession->iManager),
                                    *profile,
                                    KTestRecipientSipUri8() ); 
    
    EUNIT_ASSERT( !iClipSession->HasClipEnded() )
    
    // Try with video out stream without source...
    CMceVideoStream* videoOut = CMceVideoStream::NewLC();
    
    videoOut->AddSinkL( CMceRtpSink::NewLC() );
    CleanupStack::Pop();
     
    iClipSession->iSession->AddStreamL( videoOut );
    CleanupStack::Pop( videoOut );
    
    EUNIT_ASSERT( !iClipSession->HasClipEnded() )
    
    // And with enabled source and stream     
    videoOut->SetSourceL( CMceFileSource::NewLC( *iClipSession->iManager,
                                                 iClipSession->iFileName ) );
    CleanupStack::Pop();
    
    EUNIT_ASSERT( !iClipSession->HasClipEnded() )
    
    // try with different position and duration
    (static_cast<CMceFileSource*> (videoOut->Source()))->iPosition = 90;
    (static_cast<CMceFileSource*> (videoOut->Source()))->iDuration = 111;
    EUNIT_ASSERT( !iClipSession->HasClipEnded() )
    
    // Disapling source
    (static_cast<CMceFileSource*> (videoOut->Source()))->DisableL();
    EUNIT_ASSERT( !iClipSession->HasClipEnded() )

    // Disapling stream
    videoOut->iState = CMceMediaStream::EDisabled;
    EUNIT_ASSERT( !iClipSession->HasClipEnded() )
       
    // and finaly try with "real" end of clip 
    (static_cast<CMceFileSource*> (videoOut->Source()))->iPosition = 0;
    (static_cast<CMceFileSource*> (videoOut->Source()))->iDuration = 111;
    
    EUNIT_ASSERT( iClipSession->HasClipEnded() )
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
    
    videoOut->SetSourceL( CMceFileSource::NewLC( *iClipSession->iManager,
                                                 iClipSession->iFileName ) );
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
    iClipSession->SetClipL( KTestAvcVideoFileName() );

    // Try to establish, must fail, because of missing session
    TRAPD( error, iClipSession->EstablishSessionL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    EUNIT_ASSERT( iClipSession->iVideoCodecList );

    ///////
    // 1.Test that in case the peer party supports H264, no transcoding is needed
    
    // Call to CMusEngOutMceSession::InviteL leads to call to EstablishL
    iClipSession->InviteL( KTestRecipientSipUri );
    
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

    EUNIT_ASSERT( !iObserver->iTranscodingNeededCalled )             
    
    ///////
    // 2.Test the case when we don't know whether peer supports H264, 
    // transcoding is needed, H264 codec has to be removed from the codec list

    delete iClipSession->iSession;
    iClipSession->iSession = NULL;
    
    delete iClipSession->iVideoCodecList;
    iClipSession->iVideoCodecList = NULL;
    
    // Call to CMusEngOutMceSession::InviteL leads to call to EstablishL
    iClipSession->InviteL( KTestRecipientSipUri );

    const RPointerArray<CMceMediaStream>& streams2 = iClipSession->iSession->Streams();

    for ( TInt i = 0; i < streams2.Count(); ++i )
         {
         if ( streams2[i]->Type() == KMceVideo )
             {
             CMceVideoStream* videoStream = static_cast<CMceVideoStream*>( streams2[i] );
             const RPointerArray<CMceVideoCodec> codecs = videoStream->Codecs();
             EUNIT_ASSERT_EQUALS( codecs.Count(), 1 )
             EUNIT_ASSERT( codecs[0]->SdpName().FindF( KMceSDPNameH263() ) >= 0 )
             }
         }
 
    EUNIT_ASSERT( iObserver->iTranscodingNeededCalled )
    
    
    ///////
    // 3.Test that if peer doesn't supports H264, transcoding is needed
    // H264 codec has to be removed from the codec list

    iObserver->iTranscodingNeededCalled = EFalse;
    
    delete iClipSession->iSession;
    iClipSession->iSession = NULL;
     
    iClipSession->iVideoCodecList = KMceSDPNameH263().AllocL();
    
    // Call to CMusEngOutMceSession::InviteL leads to call to EstablishL
    iClipSession->InviteL( KTestRecipientSipUri );

    const RPointerArray<CMceMediaStream>& streams3 = iClipSession->iSession->Streams();

    for ( TInt i = 0; i < streams3.Count(); ++i )
         {
         if ( streams2[i]->Type() == KMceVideo )
             {
             CMceVideoStream* videoStream = static_cast<CMceVideoStream*>( streams3[i] );
             const RPointerArray<CMceVideoCodec> codecs = videoStream->Codecs();
             EUNIT_ASSERT_EQUALS( codecs.Count(), 1 )
             EUNIT_ASSERT( codecs[0]->SdpName().FindF( KMceSDPNameH263() ) >= 0 )
             }
         }
  
    EUNIT_ASSERT( iObserver->iTranscodingNeededCalled )
          
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
    "SetClipL - test ",
    "CMusEngClipSession",
    "AetClipL",
    "FUNCTIONALITY",
    SetupL, UT_SetClipLL, Teardown)

EUNIT_TEST(
    "FastForwardL - test ",
    "CMusEngClipSession",
    "FastForwardL",
    "FUNCTIONALITY",
    SetupL, UT_FastForwardLL, Teardown)

EUNIT_TEST(
    "FastRewindL - test ",
    "CMusEngClipSession",
    "FastRewindL",
    "FUNCTIONALITY",
    SetupL, UT_FastRewindLL, Teardown)

EUNIT_TEST(
    "PositionL - test ",
    "CMusEngClipSession",
    "PositionL",
    "FUNCTIONALITY",
    SetupL, UT_PositionLL, Teardown)

EUNIT_TEST(
    "DurationL - test ",
    "CMusEngClipSession",
    "DurationL",
    "FUNCTIONALITY",
    SetupL, UT_DurationLL, Teardown)

EUNIT_TEST(
    "SetPositionL - test ",
    "CMusEngClipSession",
    "SetPositionL",
    "FUNCTIONALITY",
    SetupL, UT_SetPositionLL, Teardown)
    
EUNIT_TEST(
    "TranscodeL - To AVC test ",
    "CMusEngClipSession",
    "TranscodeL",
    "FUNCTIONALITY",
    SetupL, UT_TranscodeLToAvcL, Teardown)

EUNIT_TEST(
    "TranscodeL - To H263 test ",
    "CMusEngClipSession",
    "TranscodeL",
    "FUNCTIONALITY",
    Setup2L, UT_TranscodeLToH263L, Teardown)
    
EUNIT_TEST(
    "CancelTranscodeL - test ",
    "CMusEngClipSession",
    "CancelTranscodeL",
    "FUNCTIONALITY",
    SetupL, UT_CancelTranscodeLL, Teardown)
    
EUNIT_TEST(
    "PlayL - test ",
    "CMusEngClipSession",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_PlayLL, Teardown)

EUNIT_TEST(
    "PauseL - test ",
    "CMusEngClipSession",
    "PauseL",
    "FUNCTIONALITY",
    SetupL, UT_PauseLL, Teardown)

EUNIT_TEST(
    "IsPlayingL - test ",
    "CMusEngClipSession",
    "IsPlayingL",
    "FUNCTIONALITY",
    SetupL, UT_IsPlayingLL, Teardown)

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
    "HasClipEnded - test ",
    "CMusEngClipSession",
    "HasClipEnded",
    "FUNCTIONALITY",
    SetupL, UT_HasClipEndedL, Teardown)

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


