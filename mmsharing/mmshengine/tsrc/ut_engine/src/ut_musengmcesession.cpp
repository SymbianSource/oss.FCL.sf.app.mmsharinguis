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
#include "ut_musengmcesession.h"
#include "musengstubs.h"
#include "musengtestdefs.h"

#include "mussettings.h"
#include "musenglivesession.h"
#include "musengclipsession.h"
#include "musengreceivesession.h"
#include "musengsessiondurationtimer.h"
#include "musengtelephoneutils.h"
#include "musengmceutils.h"

//  SYSTEM INCLUDES

#include <digia/eunit/eunitmacros.h>
#include <mceinsession.h>
#include <mcevideostream.h>
#include <mcedisplaysink.h>
#include <mcertpsource.h>
#include <mcespeakersink.h>
#include <mceaudiostream.h>
#include <mcertpsink.h>
#include <mcevideocodec.h>
#include <mceaudiocodec.h>
#include <audiopreference.h>
#include <mceh263codec.h>
#include <mceavccodec.h>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngMceSession* UT_CMusEngMceSession::NewL()
    {
    UT_CMusEngMceSession* self = UT_CMusEngMceSession::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngMceSession* UT_CMusEngMceSession::NewLC()
    {
    UT_CMusEngMceSession* self = new( ELeave ) UT_CMusEngMceSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngMceSession::~UT_CMusEngMceSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngMceSession::UT_CMusEngMceSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::SetupL(  )
    {
    iObserver = new( ELeave ) CMusEngObserverStub;
    iLiveSession = CMusEngLiveSession::NewL( KNullDesC,
                                             TRect(0,0, 100,100),
                                             *iObserver,
                                             *iObserver,
                                             *iObserver );
    
    iSomeOtherSession = CMusEngLiveSession::NewL( KNullDesC,
                                                  TRect(0,0, 100,100),
                                                  *iObserver,
                                                  *iObserver,
                                                  *iObserver );
    ESTABLISH_OUT_SESSION( iSomeOtherSession );
    
    iClipSession = CMusEngClipSession::NewL( TRect(0,0, 100,100),
                                             *iObserver,
                                             *iObserver,
                                             *iObserver );                                             
    iClipSession->SetClipL( KTestVideoFileName() );

    TBuf<50> videoCodec;
    videoCodec.Copy( KMceSDPNameH264() );
    iClipSession->SetSupportedVideoCodecListL(videoCodec);
    
    // Construct and establish an incoming session
    iReceiveSession = CMusEngReceiveSession::NewL( TRect(0,0, 100,100), 
                                                   *iObserver,
                                                   *iObserver );
    CMceInSession* inSession = CMceInSession::NewL( *iReceiveSession->iManager,
                                                    KTestOriginator() );
                                                    
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    
    videoStream->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop();
    
    inSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );
                                                    
    iReceiveSession->iManager->iInSessionObserver->IncomingSession( 
                                                        inSession, 
                                                        &iContainer );
    iReceiveSession->iSession->iState = CMceSession::EProceeding;
    iReceiveSession->AcceptInvitationL( ETrue );
    
    iObserver->Reset();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::Teardown(  )
    {
    delete iLiveSession;
    delete iClipSession;
    delete iReceiveSession;
    delete iObserver;
    delete iSomeOtherSession;
    
    }



// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_TerminateLL()
    {
    TRAPD( error, iLiveSession->TerminateL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    EUNIT_ASSERT( iLiveSession->iSession->iState == CMceSession::EEstablished );
    
    iLiveSession->TerminateL();
    
    EUNIT_ASSERT( iLiveSession->iSession->iState == CMceSession::ETerminating );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_CMusEngMceSession_GetSessionTimeL()
    {
    EUNIT_ASSERT( iLiveSession->GetSessionTime() < TTimeIntervalSeconds( 0 ) );
    
    // Invite
    iLiveSession->InviteL( KTestRecipientSipUri );
    
    EUNIT_ASSERT( iLiveSession->GetSessionTime() < TTimeIntervalSeconds( 0 ) );
    
    // Simulate establishment
    iLiveSession->iSession->iState = CMceSession::EEstablished;
    iLiveSession->iStartTime.HomeTime();
    
    TTimeIntervalSeconds sessionTime = iLiveSession->GetSessionTime();
    EUNIT_ASSERT( sessionTime >= TTimeIntervalSeconds( 0 ) );
    }
    
void UT_CMusEngMceSession::UT_CMusEngMceSession_IsDisplayEnabledLL()
    {
    TRAPD( error, iLiveSession->IsDisplayEnabledL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );

    ESTABLISH_OUT_SESSION( iLiveSession );
     
    //Enable
    CMceDisplaySink* display = 
                    MusEngMceUtils::GetDisplayL( *(iLiveSession->iSession) );
    display->iIsEnabled = ETrue;
    EUNIT_ASSERT( iLiveSession->IsDisplayEnabledL() );
    
    //Disable
    display->iIsEnabled = EFalse;
    EUNIT_ASSERT( !iLiveSession->IsDisplayEnabledL() );
    }
	
	
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_ConnectionActiveL()
    {
    EUNIT_ASSERT( !iLiveSession->ConnectionActive() );
    
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    EUNIT_ASSERT( iLiveSession->ConnectionActive() );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_ContainsAudioLL()
    {
    // Try without session, fails
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->ContainsAudioL(), KErrNotReady )
    
    // Establish session without audio
    ESTABLISH_OUT_SESSION( iLiveSession )
    
    EUNIT_ASSERT( !iLiveSession->ContainsAudioL() )
    
    // Establish session with audio
    ESTABLISH_OUT_SESSION( iClipSession )
    
    EUNIT_ASSERT( iClipSession->ContainsAudioL() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_IsMutedLL()
    {
    // Try without session, fails
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->IsMutedL(), KErrNotReady )
    
    // Establish session without audio
    ESTABLISH_OUT_SESSION( iLiveSession )
    
    EUNIT_ASSERT( iLiveSession->IsMutedL() )
    
    // Establish session with audio
    ESTABLISH_OUT_SESSION( iClipSession )
    
    EUNIT_ASSERT( !iClipSession->IsMutedL() )  // Not muted although disabled
    
    iClipSession->EnableDisplayL( ETrue ); // Enabled also speaker
    
    EUNIT_ASSERT( !iClipSession->IsMutedL() )  // Enabled and not muted

    iClipSession->MuteL();
    
    EUNIT_ASSERT( iClipSession->IsMutedL() ) // Explicitly muted
    
    iClipSession->UnmuteL();
    
    EUNIT_ASSERT( !iClipSession->IsMutedL() ) // Explicitly unmuted
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_OrientationLL()
    {
    // Try without session, fails
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->OrientationL(), KErrNotReady )
    
    // Establish a session and try all the MCE rotation values
    ESTABLISH_OUT_SESSION( iLiveSession )
    
    CMceDisplaySink* display = 
                    MusEngMceUtils::GetDisplayL( *iLiveSession->iSession );

    display->iRotation = CMceDisplaySink::ENone;
    EUNIT_ASSERT_EQUALS( iLiveSession->OrientationL(), 
                         CMusEngMceSession::EPortrait )
    
    display->iRotation = CMceDisplaySink::EClockwise90Degree;
    EUNIT_ASSERT_EQUALS( iLiveSession->OrientationL(), 
                         CMusEngMceSession::ELandscape )
    
    display->iRotation = CMceDisplaySink::EClockwise180Degree;
    EUNIT_ASSERT_EQUALS( iLiveSession->OrientationL(), 
                         CMusEngMceSession::ELandscape )
    
    display->iRotation = CMceDisplaySink::EClockwise270Degree;
    EUNIT_ASSERT_EQUALS( iLiveSession->OrientationL(), 
                         CMusEngMceSession::ELandscape )
            
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_SetOrientationLL()
    {
    // Try without session, fails
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->SetOrientationL( 
                                                CMusEngMceSession::EPortrait ), 
                                 KErrNotReady )

    // Establish a session
    ESTABLISH_OUT_SESSION( iLiveSession )
    
    CMceDisplaySink* display = 
                    MusEngMceUtils::GetDisplayL( *iLiveSession->iSession );
    
    // Check the initial state
    display->iRotation = CMceDisplaySink::ENone;
    EUNIT_ASSERT_EQUALS( iLiveSession->OrientationL(), 
                         CMusEngMceSession::EPortrait )      
                         
    // Successfully change orientation
    
    iLiveSession->SetOrientationL( CMusEngMceSession::ELandscape );
    EUNIT_ASSERT_EQUALS( iLiveSession->OrientationL(), 
                         CMusEngMceSession::ELandscape )
    
    iLiveSession->SetOrientationL( CMusEngMceSession::EPortrait );
    EUNIT_ASSERT_EQUALS( iLiveSession->OrientationL(), 
                         CMusEngMceSession::EPortrait )                     
                         
    }
        

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_VolumeUpLL()
    {    
    // Try without session, fails
    TRAPD( error, iLiveSession->VolumeUpL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Test only usage of CMceSpeaker, phone part is tested in base class
    
    // Check that nothing crashes without any speakers    
    iReceiveSession->VolumeUpL();
    
    // Establish session
    ESTABLISH_OUT_SESSION( iClipSession )
    
    TInt initialVolume = iClipSession->VolumeL();
    
    iClipSession->VolumeUpL();

    // Check that all speaker volumes are adjusted and store one speaker pointer
    
    CMceSpeakerSink* speaker = NULL;
    
    CMceSession* session = iClipSession->iSession;
    
    for ( TInt i = 0; i < session->Streams().Count(); ++i )
        {
        for ( TInt j = 0; j < session->Streams()[i]->Sinks().Count(); ++j )
            {
            if ( session->Streams()[i]->Sinks()[j]->Type() == KMceSpeakerSink )
                {
                speaker = static_cast<CMceSpeakerSink*>(
                                            session->Streams()[i]->Sinks()[j] );
                EUNIT_ASSERT( speaker->VolumeL() == initialVolume + 1 );
                }
            }
        } 

    while ( iClipSession->VolumeL() < speaker->MaxVolumeL() )
        {
        iClipSession->VolumeUpL();
        }

    iClipSession->VolumeUpL(); // Does not do anything to volume
    
    EUNIT_ASSERT( iClipSession->VolumeL() == speaker->MaxVolumeL() );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_VolumeDownLL()
    {
        // Try without session, fails
    TRAPD( error, iLiveSession->VolumeDownL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Test only usage of CMceSpeaker, phone part is tested in base class
    
    // Check that nothing crashes without any speakers    
    iReceiveSession->VolumeUpL();
    
    // Establish session
    ESTABLISH_OUT_SESSION( iClipSession )
    
    TInt initialVolume = iClipSession->VolumeL();
    
    iClipSession->VolumeDownL();

    // Check that all speaker volumes are adjusted and store one speaker pointer
    
    CMceSpeakerSink* speaker = NULL;
    
    CMceSession* session = iClipSession->iSession;
    
    for ( TInt i = 0; i < session->Streams().Count(); ++i )
        {
        for ( TInt j = 0; j < session->Streams()[i]->Sinks().Count(); ++j )
            {
            if ( session->Streams()[i]->Sinks()[j]->Type() == KMceSpeakerSink )
                {
                speaker = static_cast<CMceSpeakerSink*>(
                                            session->Streams()[i]->Sinks()[j] );
                EUNIT_ASSERT( speaker->VolumeL() == initialVolume - 1 );
                }
            }
        } 

    while ( iClipSession->VolumeL() > 0 )
        {
        iClipSession->VolumeDownL();
        }

    iClipSession->VolumeDownL(); // Does not do anything to volume
    
    EUNIT_ASSERT_EQUALS( iClipSession->VolumeL(), 0 );
        
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_SetVolumeLL()
    {    
    // Try without session, fails
    TInt newVolume = KTelephonyVolumeDefaultValue + 1;
    
    TRAPD( error, iLiveSession->SetVolumeL( newVolume ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Test only usage of CMceSpeaker, phone part is tested in base class
    
    // Check that nothing crashes without any speakers    
    iReceiveSession->SetVolumeL( newVolume );
    
    // Establish session
    ESTABLISH_OUT_SESSION( iClipSession )
    
    TInt initialVolume = iClipSession->VolumeL();
    
    iClipSession->SetVolumeL( newVolume );

    // Check that all speaker volumes are adjusted and store one speaker pointer
    
    CMceSpeakerSink* speaker = NULL;
    
    CMceSession* session = iClipSession->iSession;
    
    for ( TInt i = 0; i < session->Streams().Count(); ++i )
        {
        for ( TInt j = 0; j < session->Streams()[i]->Sinks().Count(); ++j )
            {
            if ( session->Streams()[i]->Sinks()[j]->Type() == KMceSpeakerSink )
                {
                speaker = static_cast<CMceSpeakerSink*>(
                                            session->Streams()[i]->Sinks()[j] );
                EUNIT_ASSERT( speaker->VolumeL() == newVolume );
                }
            }
        } 

    newVolume = speaker->MaxVolumeL() + 1;
    iClipSession->SetVolumeL( newVolume );
    EUNIT_ASSERT( iClipSession->VolumeL() == speaker->MaxVolumeL() );
    
    newVolume = -1;
    iClipSession->SetVolumeL( newVolume );
    EUNIT_ASSERT( iClipSession->VolumeL() == 0 );
    
    // Test situation where MCE max volume is bigger than MUS max vol
    SetMaxVolForSpeakersL(*session, KMusEngMaxVolume * 10);
    newVolume = 5;
    iClipSession->SetVolumeL( newVolume );
    EUNIT_ASSERT( iClipSession->VolumeL() == newVolume );
    EUNIT_ASSERT( speaker->VolumeL() == newVolume * 10 );
    newVolume = 1;
    iClipSession->SetVolumeL( newVolume );
    EUNIT_ASSERT( iClipSession->VolumeL() == newVolume );
    EUNIT_ASSERT( speaker->VolumeL() == newVolume * 10 );
    newVolume = KMusEngMaxVolume;
    iClipSession->SetVolumeL( newVolume );
    EUNIT_ASSERT( iClipSession->VolumeL() == newVolume );
    EUNIT_ASSERT( speaker->VolumeL() == newVolume * 10 );
    
    SetMaxVolForSpeakersL(*session, KMusEngMaxVolume * 1000);
    newVolume = 7;
    iClipSession->SetVolumeL( newVolume );
    EUNIT_ASSERT( iClipSession->VolumeL() == newVolume );
    EUNIT_ASSERT( speaker->VolumeL() == newVolume * 1000 );
    newVolume = 1;
    iClipSession->SetVolumeL( newVolume );
    EUNIT_ASSERT( iClipSession->VolumeL() == newVolume );
    EUNIT_ASSERT( speaker->VolumeL() == newVolume * 1000 );
    newVolume = KMusEngMaxVolume;
    iClipSession->SetVolumeL( newVolume );
    EUNIT_ASSERT( iClipSession->VolumeL() == newVolume );
    EUNIT_ASSERT( speaker->VolumeL() == newVolume * 1000 );
    
    
    // Test situation where MCE max volume is smaller than MUS max vol
    SetMaxVolForSpeakersL(*session, KMusEngMaxVolume / 2);
    newVolume = 3;
    iClipSession->SetVolumeL( newVolume );
    EUNIT_ASSERT( iClipSession->VolumeL() == newVolume );
    EUNIT_ASSERT( speaker->VolumeL() == newVolume / 2 );
    newVolume = 1;
    iClipSession->SetVolumeL( newVolume );
    EUNIT_ASSERT( iClipSession->VolumeL() == newVolume );
    EUNIT_ASSERT( speaker->VolumeL() == 1 );
    newVolume = KMusEngMaxVolume;
    iClipSession->SetVolumeL( newVolume );
    EUNIT_ASSERT( iClipSession->VolumeL() == newVolume );
    EUNIT_ASSERT( speaker->VolumeL() == newVolume / 2 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_EnableDisplayL()
    {
    TRAPD( error, iLiveSession->EnableDisplayL( ETrue ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    CMceDisplaySink* display = 
                    MusEngMceUtils::GetDisplayL( *(iLiveSession->iSession) );
    
    // Disable
    iLiveSession->EnableDisplayL( EFalse );
    
    EUNIT_ASSERT( !display->iIsEnabled );
    
    // Try to disable again, request should be ignored
    
    iLiveSession->EnableDisplayL( EFalse );
    
    EUNIT_ASSERT( !display->iIsEnabled );

    // Enable
    
    iLiveSession->EnableDisplayL( ETrue );
    
    EUNIT_ASSERT( display->iIsEnabled );
    
    // Try to enable again, request should be ignored
    
    iLiveSession->EnableDisplayL( ETrue );
    
    EUNIT_ASSERT( display->iIsEnabled );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_MuteLL()
    {
    TRAPD( error, iClipSession->MuteL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    ESTABLISH_OUT_SESSION_AND_START_STREAMING( iClipSession );
    
    const RPointerArray<CMceMediaStream> streams = 
                                            iClipSession->iSession->Streams();
    
    iClipSession->MuteL();
    
    // Check that all audio speaker sinks are disabled and only those                                        
    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        for ( TInt j = 0; j < streams[i]->Sinks().Count(); ++j )
            {
            CMceMediaSink* sink = streams[i]->Sinks()[j];
            if ( sink->Type() == KMceSpeakerSink  &&
                 streams[i]->Type() == KMceAudio )
                {
                EUNIT_ASSERT( sink->IsEnabled() == EFalse )
                }    
            else
                {
                EUNIT_ASSERT( sink->IsEnabled() == ETrue )
                } 
            }        
        }
    
    EUNIT_ASSERT( iClipSession->iExplicitlyMuted ) 
    
    // Try to mute again, request should be ignored without actions or leaving

    iClipSession->MuteL();

    }
    

// -----------------------------------------------------------------------------
// Relies on previous test
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_UnmuteLL()
    {
    TRAPD( error, iClipSession->UnmuteL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    ESTABLISH_OUT_SESSION( iClipSession );
    
    const RPointerArray<CMceMediaStream> streams = 
                                            iClipSession->iSession->Streams();
    
    iClipSession->MuteL();
    
    iClipSession->UnmuteL();
    
    // Check that all audio speaker sinks are enabled
                                            
    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        for ( TInt j = 0; j < streams[i]->Sinks().Count(); ++j )
            {
            if ( streams[i]->Sinks()[j]->Type() == KMceSpeakerSink )
                {
                EUNIT_ASSERT( streams[i]->Sinks()[j]->IsEnabled() == ETrue )
                }
            }        
        }
    
    EUNIT_ASSERT( !iClipSession->iExplicitlyMuted ) 
    
    // Try to unmute again, request should be ignored without actions or leaving

    iClipSession->UnmuteL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_HandleTerminationL()
    { 
    // Try different values
    iLiveSession->CMusEngMceSession::HandleTermination( KSipStatusCodeNoCodeSet,
                                                        KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionTerminatedCalled );
    iObserver->Reset();

    iLiveSession->CMusEngMceSession::HandleTermination( KSipStatusCode200OK,
                                                        KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionTerminatedCalled );
    iObserver->Reset();

    iLiveSession->CMusEngMceSession::HandleTermination( KSipStatusCodeUnknown,
                                                        KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionFailedCalled );
    iObserver->Reset();
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_AdjustVideoCodecLL()
    {
    // CMusEngMceSession::AdjustVideoCodecL does nothing, so NULL test is enough
    CMceVideoCodec* codec = 
                iLiveSession->iManager->SupportedVideoCodecs()[0]->CloneL();
    CleanupStack::PushL( codec );
    iLiveSession->CMusEngMceSession::AdjustVideoCodecL( *codec );
    CleanupStack::PopAndDestroy( codec );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_AdjustAudioCodecLL()
    {
    CMceAudioCodec* codec = 
                iLiveSession->iManager->SupportedAudioCodecs()[0]->CloneL();
    CleanupStack::PushL( codec );
    iLiveSession->CMusEngMceSession::AdjustAudioCodecL( *codec );
    
    EUNIT_ASSERT( codec->iMMFPriority == KAudioPrioritySwisPlayback )
    EUNIT_ASSERT( codec->iMMFPriorityPreference == KAudioPrefSwisPlayback )
    
    CleanupStack::PopAndDestroy( codec );
    }
        
        
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_RectChangedLL()
    {
    
    // Does nothing before session establishment
    
    iLiveSession->RectChangedL();
    
    ESTABLISH_OUT_SESSION( iLiveSession );

    // Next leads to a call to RectChangedL()
    iLiveSession->SetRectL( TRect( 100, 200, 300, 400 ) ); 
    
    // Display size has been updated
    
    CMceDisplaySink* display = 
                MusEngMceUtils::GetDisplayL( *( iLiveSession->iSession ) );
    
    EUNIT_ASSERT( display->DisplayRectL() == iLiveSession->Rect() );
    
    // Terminate session and try again, rect must not be changed
    
    iLiveSession->iSession->iState = CMceSession::ETerminated;
    
    TRect newRect( 200, 300, 400, 500 );
    
    iLiveSession->SetRectL( newRect ); 
    
    EUNIT_ASSERT( display->DisplayRectL() != newRect )
    
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_SetSessionSdpLinesLL()
    {
    // set operator variant off
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EStandard );
                     
    // There is no sdp lines in session, right one is added

    CMceInSession* inSession = CMceInSession::NewL( *iReceiveSession->iManager,
                                                    KTestOriginator );
    CleanupStack::PushL( inSession );
    
    iReceiveSession->SetSessionSdpLinesL( *inSession );
    
    MDesC8Array* sdpLines = inSession->SessionSDPLinesL();
    CleanupDeletePushL( sdpLines );
    
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 1 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineXApplication() );
    
    CleanupStack::PopAndDestroy(); // sdplines

    // There is one line ready, replace it with right one
    CDesC8Array* newSdpLines = new ( ELeave ) CDesC8ArrayFlat( 3 );
    CleanupStack::PushL( newSdpLines );
    newSdpLines->AppendL( KTestRecipientRandomText8() );
    inSession->SetSessionSDPLinesL( newSdpLines );
    CleanupStack::Pop( newSdpLines );
    
    iReceiveSession->SetSessionSdpLinesL( *inSession );
    
    sdpLines = inSession->SessionSDPLinesL();
    CleanupDeletePushL( sdpLines );
      
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 1 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineXApplication() );
    
    CleanupStack::PopAndDestroy(); // sdplines
    

    CleanupStack::PopAndDestroy( inSession );

    // for operator variant testing
    // set operator variant
    iReceiveSession->iOperatorVariant = ETrue;

    // There is no sdp lines in session, right ones are added to session and media level
    inSession = CMceInSession::NewL( *iReceiveSession->iManager,
                                                    KTestOriginator );
    CleanupStack::PushL( inSession );
    
    iReceiveSession->SetSessionSdpLinesL( *inSession );
    
    sdpLines = inSession->SessionSDPLinesL();
    CleanupDeletePushL( sdpLines );
    
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 2 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineApplication() );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 1 ) == KMusEngSessionSdpLineType() );
    
    CleanupStack::PopAndDestroy(); // sdplines
    
    MDesC8Array* mediaSdpLines = 
        iReceiveSession->iSession->Streams()[ 0 ]->MediaAttributeLinesL();
    CleanupDeletePushL( mediaSdpLines );
    TBool bandwidthFoundFromMediaLevel = EFalse;
    for ( TInt i = 0; mediaSdpLines && i < mediaSdpLines->MdcaCount(); i++ )
        {
        if ( mediaSdpLines->MdcaPoint( i ).Compare( 
                KMusEngSessionSdpLineBandwidthField() ) == 0 )
            {
            bandwidthFoundFromMediaLevel = ETrue;
            }
        }
    EUNIT_ASSERT( bandwidthFoundFromMediaLevel );
    CleanupStack::PopAndDestroy( mediaSdpLines );

    // There is one line ready, replace it with right ones
    newSdpLines = new ( ELeave ) CDesC8ArrayFlat( 3 );
    CleanupStack::PushL( newSdpLines );
    newSdpLines->AppendL( KTestRecipientRandomText8() );
    inSession->SetSessionSDPLinesL( newSdpLines );
    CleanupStack::Pop( newSdpLines );
    
    iReceiveSession->SetSessionSdpLinesL( *inSession );
    
    sdpLines = inSession->SessionSDPLinesL();
    CleanupDeletePushL( sdpLines );
      
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 2 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineApplication() );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 1 ) == KMusEngSessionSdpLineType() );
    
    CleanupStack::PopAndDestroy(); // sdplines
    

    CleanupStack::PopAndDestroy( inSession );


    // set operator variant off
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EStandard );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_AdjustStreamsAndCodecsLL()
    {
    // Check that function cannot be called before creating the session
    TRAPD( error, iClipSession->AdjustStreamsAndCodecsL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Establishing a clip session leads to call to AdjustStreamsAndCodecsL
    ESTABLISH_OUT_SESSION( iClipSession )
   
    const RPointerArray<CMceMediaStream>& streams = 
                                            iClipSession->iSession->Streams();
    
    TBool videoCodecFound = EFalse;
    TBool audioCodecFound = EFalse;
    
    // Test that AdjustVideoCodecL and AdjustAudioCodecL have been called
    CMceVideoStream* videoStream = NULL;
    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( streams[i]->Type() == KMceAudio )
            {
            CMceAudioStream* audioStream = 
                            static_cast<CMceAudioStream*>( streams[i] );
            
            const RPointerArray<CMceAudioCodec>& codecs = audioStream->Codecs();
            
            for ( TInt i = 0; i < codecs.Count(); ++i )
                {
                audioCodecFound = ETrue;

                EUNIT_ASSERT( codecs[i]->MMFPriority() == 
                              KAudioPrioritySwisPlayback )
                EUNIT_ASSERT( codecs[i]->MMFPriorityPreference() == 
                              KAudioPrefSwisPlayback )
                }
            }
        else
            {
            videoStream = static_cast<CMceVideoStream*>( streams[i] );
            
            const RPointerArray<CMceVideoCodec>& codecs = videoStream->Codecs();
            
            for ( TInt i = 0; i < codecs.Count(); ++i )
                {
                videoCodecFound = ETrue;

                EUNIT_ASSERT( codecs[i]->iPayloadType == 96 ||
                              codecs[i]->iPayloadType == 98 )
                }
            }
    
        }
        
    EUNIT_ASSERT( audioCodecFound ) // At least one audiocodec found
    EUNIT_ASSERT( videoCodecFound ) // At least one videocodec found
    
    // Test removing of H.263 multiples (mode 2000 preferred)
    EUNIT_ASSERT( videoStream != NULL )
    for ( TInt i = videoStream->Codecs().Count() - 1; i >= 0; i-- )
        {
        videoStream->RemoveCodecL( 
            *const_cast<CMceVideoCodec*>( videoStream->Codecs()[ i ] ) );
        }
    CMceH263Codec* codecH2632000 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );
    videoStream->AddCodecL( codecH2632000 );
    CleanupStack::Pop( codecH2632000 );
    
    CMceH263Codec* codecH2631998 = CMceH263Codec::NewLC( KMceSDPNameH2631998() );
    videoStream->AddCodecL( codecH2631998 );
    CleanupStack::Pop( codecH2631998 );
    
    iClipSession->AdjustStreamsAndCodecsL();
    
    // 1998 version should have been removed
    for ( TInt i = 0; i < videoStream->Codecs().Count(); i++ )
        {
        EUNIT_ASSERT( 
            !videoStream->Codecs()[ i ]->SdpName().CompareF( KMceSDPNameH2632000() ) );
        }
        
    // Test removing of multiples with different bitrates
    for ( TInt i = videoStream->Codecs().Count() - 1; i >= 0; i-- )
        {
        videoStream->RemoveCodecL( 
            *const_cast<CMceVideoCodec*>( videoStream->Codecs()[ i ] ) );
        }
    codecH2632000 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );
    codecH2632000->SetMaxBitrateL( 64000 );
    videoStream->AddCodecL( codecH2632000 );
    CleanupStack::Pop( codecH2632000 );
    
    codecH2632000 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );
    codecH2632000->SetMaxBitrateL( 128000 );
    videoStream->AddCodecL( codecH2632000 );
    CleanupStack::Pop( codecH2632000 );
    
    iClipSession->AdjustStreamsAndCodecsL();
    
     // Lower bitrate version should have been removed
    for ( TInt i = 0; i < videoStream->Codecs().Count(); i++ )
        {
        EUNIT_ASSERT( 
            videoStream->Codecs()[ i ]->MaxBitRate() == 128000 );
        }
    
    // Test for Configuration based codec removal. Magic uid not set.    
    CMceVideoCodec* codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );    
    videoStream->AddCodecL( codecAvc );
    CleanupStack::Pop( codecAvc );         
    iClipSession->AdjustStreamsAndCodecsL();   
    TBool avcCodecFound(EFalse);
    for ( TInt i = 0; i < videoStream->Codecs().Count(); i++ )
      {      
      if( !videoStream->Codecs()[ i ]->SdpName().CompareF( KMceSDPNameH264 ) )
        {
        avcCodecFound = ETrue;
        }      
      }
    // AVC should present
    EUNIT_ASSERT(avcCodecFound);
    
    // Test for Configuration based codec removal. Magic uid set.
    // Store the real value and revert back later , so that other tests will not be affected.
    TUid uid = MultimediaSharingSettings::EncodingDeviceL();    
    TInt32 KMusDisableAVC = 0x0fffffff;
    MultimediaSharingSettings::SetPropertyValueL( MusSettingsKeys::KEncodingDevice,KMusDisableAVC);
    codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );    
    videoStream->AddCodecL( codecAvc );
    CleanupStack::Pop( codecAvc );  
    codecH2632000 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );
    videoStream->AddCodecL( codecH2632000 );
    CleanupStack::Pop( codecH2632000 );
    iClipSession->AdjustStreamsAndCodecsL();    
    for ( TInt i = 0; i < videoStream->Codecs().Count(); i++ )
       {
       // H264 ( AVC ) codec should be removed
       EUNIT_ASSERT( 
                   videoStream->Codecs()[ i ]->SdpName().CompareF( KMceSDPNameH264 ) );
       }
    
    
    MultimediaSharingSettings::SetPropertyValueL( MusSettingsKeys::KEncodingDevice,uid.iUid);
    
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_CMusEngMceSession_IncomingSessionL()
    {
    // New session should get rejected and deleted in any case, first failure
    CMceInSession* inSession = CMceInSession::NewL( 
                                      *iLiveSession->iManager, KNullDesC8 );
    CleanupStack::PushL( inSession );
    iLiveSession->IncomingSession( inSession, NULL );
    CleanupStack::Pop( inSession );
    
    // And then succesfull case, this would leak memory if session was not
    // deleted
    inSession = CMceInSession::NewL( *iLiveSession->iManager, KNullDesC8 );
    CleanupStack::PushL( inSession );
    iLiveSession->IncomingSession( inSession, NULL );
    CleanupStack::Pop( inSession );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_CMusEngMceSession_IncomingUpdateL()
    {
    CMceInSession* inSession = CMceInSession::NewL( 
                                      *iLiveSession->iManager, KNullDesC8 );
    
    // Try to update before having a session, session gets rejected and deleted
    CleanupStack::PushL( inSession );
    iLiveSession->IncomingUpdate( *inSession, inSession, NULL );
    CleanupStack::Pop( inSession );
    
    
    // Try to update with unknown session, session gets rejected and deleted
    ESTABLISH_OUT_SESSION( iLiveSession )
    
    inSession = CMceInSession::NewL( 
                                      *iLiveSession->iManager, KNullDesC8 );
    CleanupStack::PushL( inSession );
    iLiveSession->IncomingUpdate( *inSession, inSession, NULL );
    CleanupStack::Pop( inSession );
    
    // Normal case, old session gets deleted. New session gets rejected.
    
    inSession = CMceInSession::NewL( 
                                      *iLiveSession->iManager, KNullDesC8 );
    CleanupStack::PushL( inSession );
    iLiveSession->IncomingUpdate( *iLiveSession->iSession, inSession, NULL );
    CleanupStack::Pop( inSession );
    
    EUNIT_ASSERT( iLiveSession->iSession == inSession )
    EUNIT_ASSERT( iLiveSession->iSession->iState == CMceSession::ETerminated )
    
    }
        

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_CMusEngMceSession_StreamStateChangedL()
    {
    // Try without a session, nothing happens
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    iLiveSession->CMusEngMceSession::StreamStateChanged( *audioStream );
    CleanupStack::PopAndDestroy( audioStream );
    
    // Simulate sending invite
    iLiveSession->InviteL( KTestRecipientSipUri() );
    
    // No observer set, so expect nothing to be done, cannot be asserted
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];

    // Try all the stream states
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->iStreamStreamingCalled );
    iObserver->Reset();
    
    // EStreaming, stream is streaming, other than video out- or instream
    changedStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->CMusEngMceSession::StreamStateChanged( 
                                *iLiveSession->iSession->Streams()[1] );
    EUNIT_ASSERT( iObserver->IsReseted() );
    iObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    }
    

// -----------------------------------------------------------------------------
// All the possibilities are tested in order to make sure unit tests are changed
// when function is changed
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_CMusEngMceSession_StreamStateChangedWithSourceL()
    {
    // Try without a session, nothing happens
    iLiveSession->CMusEngMceSession::StreamStateChanged( 
                                        *CMceAudioStream::NewLC(),
                                        *CMceRtpSource::NewLC() );
                                        
    CleanupStack::PopAndDestroy(); // rtp                                    
    CleanupStack::PopAndDestroy(); // audiostream
    
    // Simulate sending invite
    iLiveSession->InviteL( KTestRecipientSipUri() );
    
    // Try all the stream states
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->iStreamStreamingCalled );
    iObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    }


// -----------------------------------------------------------------------------
// All the possibilities are tested in order to make sure unit tests are changed
// when function is changed
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_CMusEngMceSession_StreamStateChangedWithSinkL()
    {
    // Try without a session, nothing happens
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    iLiveSession->CMusEngMceSession::StreamStateChanged( *audioStream );
    CleanupStack::PopAndDestroy( audioStream );
    
    // Simulate sending invite
    iLiveSession->InviteL( KTestRecipientSipUri() );
    
    // Try all the stream states
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->iStreamStreamingCalled );
    iObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    }        


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngMceSession::UT_CMusEngMceSession_SessionStateChangedL()
    {
    // Simulate sending invite
    iLiveSession->InviteL( KTestRecipientSipUri() );   

    // No container, so expect to be handled as internal error
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, NULL );
    EUNIT_ASSERT( iObserver->iSessionFailedCalled );
    iObserver->Reset();    
    
    // Try to tell session about some other session's state transition
    
    TMceTransactionDataContainer container;
 
    iLiveSession->SessionStateChanged( *iSomeOtherSession->iSession, &container );
    EUNIT_ASSERT( iObserver->IsReseted() );    
      
    // Try all the normal cases:
    
    // EIdle
    iLiveSession->iSession->iState = CMceSession::EIdle;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    // Set up any text to simulate reasonphrase
    HBufC8* anyReasonPhrase = KTestOriginator().AllocL();
    container.SetReasonPhrase( anyReasonPhrase );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iObserver->iSessionFailedCalled );
    iObserver->Reset();
    container.Clear();
    
    // EOffering
    iLiveSession->iSession->iState = CMceSession::EOffering;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EIncoming
    iLiveSession->iSession->iState = CMceSession::EIncoming;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iObserver->iSessionFailedCalled );
    iObserver->Reset();
    
    // EReserving
    iLiveSession->iSession->iState = CMceSession::EReserving;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EAnswering
    iLiveSession->iSession->iState = CMceSession::EAnswering;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EProceeding
    iLiveSession->iSession->iState = CMceSession::EProceeding;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // EEstablished
    iLiveSession->iSession->iState = CMceSession::EEstablished;
    container.SetStatusCode( KSipStatusCode200OK );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iObserver->iSessionEstablishedCalled );
    iObserver->Reset();
    
    // EEstablished again (meaning expiration of session timer 
    // and refresment of session )
    iLiveSession->iSession->iState = CMceSession::EEstablished;
    container.SetStatusCode( KSipStatusCode200OK );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // ECancelling
    iLiveSession->iSession->iState = CMceSession::ECancelling;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iObserver->IsReseted() );
    iObserver->Reset();
    
    // ETerminating
    iLiveSession->iSession->iState = CMceSession::ETerminating;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iObserver->IsReseted() );
    iObserver->Reset();
    
    // ETerminated
    iLiveSession->iSession->iState = CMceSession::ETerminated;
    container.SetStatusCode( KSipStatusCode200OK );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iObserver->iSessionTerminatedCalled );
    iObserver->Reset();
    
    // Default case
    iLiveSession->iSession->iState = static_cast<CMceSession::TState>( 10 );
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, &container );
    EUNIT_ASSERT( iObserver->IsReseted() );

    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//     
void UT_CMusEngMceSession::UT_CMusEngMceSession_SessionConnectionStateChangedL()
    {
    
    // Try to tell session about another session's connection state change
    
    iLiveSession->SessionConnectionStateChanged( *iSomeOtherSession->iSession, 
                                                 ETrue );
    EUNIT_ASSERT( iObserver->IsReseted() ); // No session yet, nothing happened
    
    // Simulate sending invite
    iLiveSession->InviteL( KTestRecipientSipUri() );
    
    // Now try again to notify session about session connection state change
    // of an another session
    iLiveSession->SessionConnectionStateChanged( *iSomeOtherSession->iSession, 
                                                 ETrue );
    EUNIT_ASSERT( iObserver->IsReseted() ); // Wrong session, nothing happened
    
    // Try tell session connection state is active
    iLiveSession->SessionConnectionStateChanged( *iLiveSession->iSession, 
                                                 ETrue );
    EUNIT_ASSERT( iObserver->IsReseted() ); // Session active, nothing happened
    
    // Try tell session connection state is inactive
    iLiveSession->SessionConnectionStateChanged( *iLiveSession->iSession, 
                                                 EFalse );
    EUNIT_ASSERT( iObserver->iSessionConnectionLostCalled );
    iObserver->Reset();
    
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusEngMceSession::UT_CMusEngMceSession_FailedL()
    {
    // Try to tell session about failure of another session

    iLiveSession->Failed( *iSomeOtherSession->iSession, 0 );
    EUNIT_ASSERT( iObserver->IsReseted() ); // No session yet, nothing happened
    
    // Simulate sending invite
    iLiveSession->InviteL( KTestRecipientSipUri() );
    
    // Now try again to notify session about failure of an another session
    iLiveSession->Failed( *iSomeOtherSession->iSession, 0 );
    EUNIT_ASSERT( iObserver->IsReseted() ); // Wrong session, nothing happened
    
    // Tell session that it has failed
    iLiveSession->Failed( *iLiveSession->iSession, 0 );
    EUNIT_ASSERT( iObserver->iSessionFailedCalled );
    iObserver->Reset();
    
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//     
void UT_CMusEngMceSession::UT_CMusEngMceSession_UpdateFailedL()
    {
    // Try to tell session about update failure of another session
    iLiveSession->UpdateFailed( *iSomeOtherSession->iSession, NULL );
    EUNIT_ASSERT( iObserver->iSessionFailedCalled ); // No container, internal error
    iObserver->Reset();
    
    // Try again with container
    TMceTransactionDataContainer container;
    iLiveSession->UpdateFailed( *iSomeOtherSession->iSession, &container );
    EUNIT_ASSERT( iObserver->IsReseted() ); // No session yet, nothing happened
    
    // Invite and try again.
    iLiveSession->InviteL( KTestRecipientSipUri() );
    iLiveSession->UpdateFailed( *iSomeOtherSession->iSession, &container );
    EUNIT_ASSERT( iObserver->IsReseted() ); // Wrong session, nothing happened¨
    
    // Try with right session
    iLiveSession->UpdateFailed( *iLiveSession->iSession, &container );
    EUNIT_ASSERT( iObserver->iSessionFailedCalled );
    iObserver->Reset();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusEngMceSession::UT_CMusEngMceSession_SRReceivedL()
    {
    EUNIT_ASSERT( iLiveSession->iSecondsFromLastRtcpReport == 0 );
    
    // No session yet, nothing happens
    iLiveSession->SRReceived( *iSomeOtherSession->iSession,
                              *(iSomeOtherSession->iSession->Streams()[0]) );
    
    EUNIT_ASSERT( iLiveSession->iSecondsFromLastRtcpReport == 0 );
    
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    // Simulate bigger value
    iLiveSession->iSecondsFromLastRtcpReport = 10;
    
    // We have session, but informing concearn wrong session, timer is 
    // not nullified
    iLiveSession->SRReceived( *iSomeOtherSession->iSession,
                              *(iSomeOtherSession->iSession->Streams()[0]) );
    
    EUNIT_ASSERT( iLiveSession->iSecondsFromLastRtcpReport == 10 );
    
    // Right session but wrong stream, timer should not be nullified
    iLiveSession->SRReceived( *iLiveSession->iSession,
                              *(iSomeOtherSession->iSession->Streams()[0]) );
    
    EUNIT_ASSERT( iLiveSession->iSecondsFromLastRtcpReport == 10 );
    
    // Right stream and established session, timer should be nullified
    
    
    iLiveSession->SRReceived( *iLiveSession->iSession,
                              *(iLiveSession->iSession->Streams()[0]) );

    EUNIT_ASSERT( iLiveSession->iSecondsFromLastRtcpReport == 0 );
    
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//     
void UT_CMusEngMceSession::UT_CMusEngMceSession_RRReceivedL()
    {
    EUNIT_ASSERT( iLiveSession->iSecondsFromLastRtcpReport == 0 );
    
    // No session yet, nothing happens
    iLiveSession->RRReceived( *iSomeOtherSession->iSession,
                              *(iSomeOtherSession->iSession->Streams()[0]) );
    
    EUNIT_ASSERT( iLiveSession->iSecondsFromLastRtcpReport == 0 );
    
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    // Simulate bigger value
    iLiveSession->iSecondsFromLastRtcpReport = 10;
    
    // We have session, but informing concearn wrong session, timer is 
    // not nullified
    iLiveSession->RRReceived( *iSomeOtherSession->iSession,
                              *(iSomeOtherSession->iSession->Streams()[0]) );
    
    EUNIT_ASSERT( iLiveSession->iSecondsFromLastRtcpReport == 10 );
    
    // Right session but wrong stream, timer should not be nullified
    iLiveSession->RRReceived( *iLiveSession->iSession,
                              *(iSomeOtherSession->iSession->Streams()[0]) );
    
    EUNIT_ASSERT( iLiveSession->iSecondsFromLastRtcpReport == 10 );
    
    // Right stream and established session, timer should be nullified
    
    
    iLiveSession->RRReceived( *iLiveSession->iSession,
                              *(iLiveSession->iSession->Streams()[0]) );

    EUNIT_ASSERT( iLiveSession->iSecondsFromLastRtcpReport == 0 );    
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//      
void UT_CMusEngMceSession::UT_CMusEngMceSession_InactivityTimeoutL()
    {    
    //   Construct an audio stream for this test
    
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    
    CMceRtpSource* rtpSource = CMceRtpSource::NewLC();
    audioStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource );
    
    iReceiveSession->InactivityTimeout( *audioStream, *rtpSource );
    
    EUNIT_ASSERT( iObserver->IsReseted() )
    
    CleanupStack::PopAndDestroy( audioStream );
    
    // And now real inactivity timeout
    
    iReceiveSession->InactivityTimeout( 
                *iReceiveSession->iSession->Streams()[0],
                *static_cast<CMceRtpSource*>
                        (iReceiveSession->iSession->Streams()[0]->Source()) );
    
    EUNIT_ASSERT( iObserver->iStreamIdleCalled )
    
    iObserver->Reset();
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//       
void UT_CMusEngMceSession::UT_CMusEngMceSession_SsrcAddedL()
    {
    // As CMusEngMceSession::SsrcAdded is expected to do nothing, just call
    // it to make sure it does not weaken the test coverage
    
    CMceRtpSource* rtpSource = static_cast<CMceRtpSource*>( 
                            iReceiveSession->iSession->Streams()[0]->Source() );   
    iReceiveSession->SsrcAdded( 
                        *iReceiveSession->iSession->Streams()[0],
                        *rtpSource,
                        0 );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_CMusEngMceSession_SsrcRemovedL()
    {
    // As CMusEngMceSession::SsrcRemoved is expected to do nothing, just call
    // it to make sure it does not weaken the test coverage
    
    CMceRtpSource* rtpSource = static_cast<CMceRtpSource*>( 
                            iReceiveSession->iSession->Streams()[0]->Source() );   

    iReceiveSession->SsrcRemoved( 
                        *iReceiveSession->iSession->Streams()[0],
                        *rtpSource,
                        0 );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_CMusEngMceSession_UpdateTimerEventL()
    {
    iLiveSession->UpdateTimerEvent();
    EUNIT_ASSERT( iObserver->iSessionTimeChangedCalled )
    EUNIT_ASSERT( iLiveSession->iUpdateTimer->IsActive() )
    iLiveSession->iUpdateTimer->Cancel();
    iObserver->Reset();
    
    iLiveSession->iSecondsFromLastRtcpReport = 25; // >KMusEngRtcpInactivityThreshold
    iLiveSession->UpdateTimerEvent();
    EUNIT_ASSERT( iObserver->iSessionTimeChangedCalled )
    EUNIT_ASSERT( iObserver->iInactivityTimeoutCalled );
    EUNIT_ASSERT( iLiveSession->iUpdateTimer->IsActive() )
    iLiveSession->iUpdateTimer->Cancel();
    iObserver->Reset();
    }   


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_IsRoamingBetweenAPsAllowedL()
    {
    // No session
    EUNIT_ASSERT( iLiveSession->IsRoamingBetweenAPsAllowed() )

    // Session idle
    _LIT( KRecipientSipUri, "sip:user@some.where" );
    iLiveSession->InviteL( KRecipientSipUri );
    iLiveSession->iSession->iState = CMceSession::EIdle;
    EUNIT_ASSERT( iLiveSession->IsRoamingBetweenAPsAllowed() )
    
    // Session established
    iLiveSession->iSession->iState = CMceSession::EEstablished;
    EUNIT_ASSERT( !iLiveSession->IsRoamingBetweenAPsAllowed() )
    
    // Session terminated
    iLiveSession->iSession->iState = CMceSession::ETerminated;
    EUNIT_ASSERT( iLiveSession->IsRoamingBetweenAPsAllowed() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_CMusEngMceSession_VolumeChangedL()
    {
    // Try without session, nothing happens
    EUNIT_ASSERT( iObserver->iVolume == 0 );
    iLiveSession->VolumeChanged( 1, EFalse );
    EUNIT_ASSERT( !VerifySpeakersVolume(*iLiveSession, 1) )
    EUNIT_ASSERT( iObserver->iVolume == 0 );

    // Establish session
    ESTABLISH_OUT_SESSION( iClipSession )
    
    // Try without observer
    iClipSession->VolumeChanged( 2, EFalse );
    EUNIT_ASSERT( VerifySpeakersVolume(*iClipSession, 2) )
    EUNIT_ASSERT( iObserver->iVolume == 0 );
    
    
    // Try with observer set
    iClipSession->SetVolumeChangeObserver( iObserver );
    iClipSession->VolumeChanged( 3, EFalse );
    EUNIT_ASSERT( VerifySpeakersVolume(*iClipSession, 3) )
    EUNIT_ASSERT( iObserver->iVolume == 3 );
    
    }

// HELPERS
TBool UT_CMusEngMceSession::VerifySpeakersVolume(CMusEngMceSession& aSession, TInt aVolume)
    {
    TBool result = EFalse;
    
    CMceSpeakerSink* speaker = NULL;    
    CMceSession* session = aSession.iSession;
    
    if ( session ) 
        {
        for ( TInt i = 0; i < session->Streams().Count(); ++i )
            {
            for ( TInt j = 0; j < session->Streams()[i]->Sinks().Count(); ++j )
                {
                if ( session->Streams()[i]->Sinks()[j]->Type() == KMceSpeakerSink )
                    {
                    speaker = static_cast<CMceSpeakerSink*>(
                                              session->Streams()[i]->Sinks()[j] );
                    result = ( speaker->VolumeL() == aVolume );
                    }
                }
            } 
        }
    return result;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::SetMaxVolForSpeakersL(CMceSession& aSession, TInt aMaxVol)
    {
    for ( TInt i = 0; i < aSession.Streams().Count(); ++i )
            {
            for ( TInt j = 0; j < aSession.Streams()[i]->Sinks().Count(); ++j )
                {
                if ( aSession.Streams()[i]->Sinks()[j]->Type() == KMceSpeakerSink )
                    {
                    CMceSpeakerSink* speaker = static_cast<CMceSpeakerSink*>(
                                                aSession.Streams()[i]->Sinks()[j] );
                    speaker->iMaxVolume = aMaxVol;
                    }
                }
            } 
    }
 
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngMceSession,
    "UT_CMusEngMceSesssion",
    "UNIT" )

EUNIT_TEST(
    "TerminateL - test ",
    "CMusEngMceSession",
    "TerminateL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_TerminateLL, Teardown)

EUNIT_TEST(
    "GetSessionTime - test ",
    "CMusEngMceSession",
    "GetSessionTime",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_GetSessionTimeL, Teardown)

EUNIT_TEST(
    "ConnectionActive - test ",
    "CMusEngMceSession",
    "ConnectionActive",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_ConnectionActiveL, Teardown) 

EUNIT_TEST(
    "ContainsAudioL - test ",
    "CMusEngMceSession",
    "ContainsAudioL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_ContainsAudioLL, Teardown) 

EUNIT_TEST(
    "IsMutedL - test ",
    "CMusEngMceSession",
    "IsMutedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_IsMutedLL, Teardown) 

EUNIT_TEST(
    "OrientationL - test ",
    "CMusEngMceSession",
    "OrientationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_OrientationLL, Teardown) 

EUNIT_TEST(
    "SetOrientationL - test ",
    "CMusEngMceSession",
    "SetOrientationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_SetOrientationLL, Teardown) 
    
EUNIT_TEST(
    "VolumeUpL - test ",
    "CMusEngMceSession",
    "VolumeUpL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_VolumeUpLL, Teardown) 

EUNIT_TEST(
    "VolumeDownL - test ",
    "CMusEngMceSession",
    "VolumeDownL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_VolumeDownLL, Teardown) 

EUNIT_TEST(
    "SetVolumeL - test ",
    "CMusEngMceSession",
    "SetVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_SetVolumeLL, Teardown)
    
EUNIT_TEST(
    "EnableDisplay - test ",
    "CMusEngMceSession",
    "EnableDisplay",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_EnableDisplayL, Teardown)   

EUNIT_TEST(
    "MuteL - test ",
    "CMusEngMceSession",
    "MuteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_MuteLL, Teardown)   

EUNIT_TEST(
    "UnmuteL - test ",
    "CMusEngMceSession",
    "UnmuteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_UnmuteLL, Teardown)   

EUNIT_TEST(
    "HandleTermination - test ",
    "CMusEngMceSession",
    "HandleTermination",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_HandleTerminationL, Teardown)   

EUNIT_TEST(
    "AdjustVideoCodecL - test ",
    "CMusEngMceSession",
    "AdjustVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_AdjustVideoCodecLL, Teardown) 

EUNIT_TEST(
    "AdjustAudioCodecL - test ",
    "CMusEngMceSession",
    "AdjustAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_AdjustAudioCodecLL, Teardown) 
    
EUNIT_TEST(
    "RectChangedL - test ",
    "CMusEngMceSession",
    "RectChangedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_RectChangedLL, Teardown) 

EUNIT_TEST(
    "SetSessionSdpLinesL - test ",
    "CMusEngMceSession",
    "SetSessionSdpLinesL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_SetSessionSdpLinesLL, Teardown) 

EUNIT_TEST(
    "AdjustStreamsAndCodecsL - test ",
    "CMusEngMceSession",
    "AdjustStreamsAndCodecsL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_AdjustStreamsAndCodecsLL, Teardown)

EUNIT_TEST(
    "IncomingSession - test ",
    "CMusEngMceSession",
    "IncomingSession",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_IncomingSessionL, Teardown)          

EUNIT_TEST(
    "IncomingUpdate - test ",
    "CMusEngMceSession",
    "IncomingUpdate",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_IncomingUpdateL, Teardown)      

EUNIT_TEST(
    "StreamStateChangedL( source ) - test ",
    "CMusEngMceSession",
    "StreamStateChangedL( source )",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_StreamStateChangedWithSourceL, Teardown)          

EUNIT_TEST(
    "SessionStateChanged - test ",
    "CMusEngMceSession",
    "SessionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_SessionStateChangedL, Teardown) 
    
EUNIT_TEST(
    "SessionConnectionStateChanged - test ",
    "CMusEngMceSession",
    "SessionConnectionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_SessionConnectionStateChangedL, Teardown) 

EUNIT_TEST(
    "Failed - test ",
    "CMusEngMceSession",
    "Failed",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_FailedL, Teardown)     

EUNIT_TEST(
    "UpdateFailed - test ",
    "CMusEngMceSession",
    "UpdateFailed",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_UpdateFailedL, Teardown) 

EUNIT_TEST(
    "StreamStateChanged - test ",
    "CMusEngMceSession",
    "StreamStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_StreamStateChangedL, Teardown)             

EUNIT_TEST(
    "StreamStateChangedL (sink ) - test ",
    "CMusEngMceSession",
    "StreamStateChangedL( sink )",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_StreamStateChangedWithSinkL, Teardown) 

EUNIT_TEST(
    "SRReceived - test ",
    "CMusEngMceSession",
    "SRReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_SRReceivedL, Teardown)     

EUNIT_TEST(
    "RRReceived - test ",
    "CMusEngMceSession",
    "RRReceived",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_RRReceivedL, Teardown) 

EUNIT_TEST(
    "InactivityTimeout - test ",
    "CMusEngMceSession",
    "InactivityTimeout",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_InactivityTimeoutL, Teardown)     

EUNIT_TEST(
    "SsrcAdded - test ",
    "CMusEngMceSession",
    "SsrcAdded",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_SsrcAddedL, Teardown) 

EUNIT_TEST(
    "SsrcRemoved - test ",
    "CMusEngMceSession",
    "SsrcRemoved",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_SsrcRemovedL, Teardown)         
EUNIT_TEST(
    "IsDisplayEnabledL - test ",
    "CMusEngMceSession",
    "IsDisplayEnabledL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_IsDisplayEnabledLL, Teardown)  	

EUNIT_TEST(
    "UpdateTimerEvent - test ",
    "CMusEngMceSession",
    "UpdateTimerEvent",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_UpdateTimerEventL, Teardown)              

EUNIT_TEST(
    "IsRoamingBetweenAPsAllowed - test ",
    "CMusEngMceSession",
    "IsRoamingBetweenAPsAllowed",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_IsRoamingBetweenAPsAllowedL, Teardown)
        
EUNIT_TEST(
    "VolumeChanged - test ",
    "CMusEngMceSession",
    "VolumeChanged",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_VolumeChangedL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
