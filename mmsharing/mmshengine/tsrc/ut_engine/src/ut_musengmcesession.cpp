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
#include "mussessionproperties.h"
#include "contactenginestub.h"
#include "musenglivevideoplayer.h"

//  SYSTEM INCLUDES
#include <lcsourcefilecontrol.h>
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
#include <mceavccodec.h>
#include <mceh263codec.h>
#include <mcecamerasource.h>
#include <mcefilesource.h>
#include <telmicmutestatuspskeys.h>

// CONSTANTS

_LIT8( KMusAvcBitrateLevel1TestText, "TestTextForAvcBrL1Level" );
_LIT8( KMusAvcBitrateLevel1bTestText, "TestTextForAvcBrL1bLevel" );
_LIT8( KMusAvcBitrateLevel1_1TestText, "TestTextForAvcBrL1_1Level" );
_LIT8( KMusAvcBitrateLevel1_2TestText, "TestTextForAvcBrL1_2Level" );
_LIT8( KMusAvcBitrateLevel1_3TestText, "TestTextForAvcBrL1_3Level" );
_LIT8( KMusAvcBitrateLevel2TestText, "TestTextForAvcBrL2Level" );

_LIT8( KMusAvcBitrateLevel_1b_ConfigKey, "AvcBrL1b=TestTextForAvcBrL1bLevel;" );

_LIT8( KMusAvcBitrateLevel_1_1_ConfigKey, 
"AvcBrL1_1=TestTextForAvcBrL1_1Level;");

_LIT8( KMusAvcBitrateLevels_1_1_And_1b_ConfigKeys, 
"AvcBrL1_1=TestTextForAvcBrL1_1Level;\
AvcBrL1b=TestTextForAvcBrL1bLevel;" );

_LIT8( KMusAvcAllLevelsConcatenation,
"AvcBrL1=TestTextForAvcBrL1Level;\
AvcBrL1b=TestTextForAvcBrL1bLevel;\
AvcBrL1_1=TestTextForAvcBrL1_1Level;\
AvcBrL1_2=TestTextForAvcBrL1_2Level;\
AvcBrL1_3=TestTextForAvcBrL1_3Level;\
AvcBrL2=TestTextForAvcBrL2Level;" );


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
    iLcSessionObserver = new( ELeave )CLcSessionObserverStub;
    iLcUiProvider = new( ELeave )CLcUiProviderStub;

    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KRemoteSipAddress,
                                        KTestRecipientSipUri ) );    
    
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
    
    iSomeOtherSession = CMusEngLiveSession::NewL();
    iSomeOtherSession->SetLcSessionObserver( iLcSessionObserver );
    iSomeOtherSession->SetLcUiProvider( iLcUiProvider );
    
    ESTABLISH_OUT_SESSION( iSomeOtherSession );
   
    iClipSession = CMusEngClipSession::NewL();
    iClipSession->SetLcSessionObserver( iLcSessionObserver );
    iClipSession->SetLcUiProvider( iLcUiProvider );
    MLcSourceFileControl* sourceFileControl =
        iClipSession->LocalVideoPlayer()->LcSourceFileControl();
    sourceFileControl->SetLcFileNameL( KTestAvcVideoFileName() );

    delete iClipSession->iVideoCodecList;
    iClipSession->iVideoCodecList = NULL;
    iClipSession->iVideoCodecList = KMceSDPNameH264().AllocL();
    
    // Construct and establish an incoming session
    iReceiveSession = CMusEngReceiveSession::NewL();
    iReceiveSession->SetLcSessionObserver( iLcSessionObserver ); 
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
    iReceiveSession->EstablishLcSessionL();
   
    iLcSessionObserver->Reset();
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
    delete iLcSessionObserver;
    delete iLcUiProvider;
    delete iSomeOtherSession;
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
void UT_CMusEngMceSession::UT_LcSessionStateL()
    {
    // No MCE session -> MLcSession::EUninitialized
    EUNIT_ASSERT_EQUALS( MLcSession::EUninitialized, 
                         iLiveSession->LcSessionState() )
                         
    // MLcSession::EOpen
    ESTABLISH_OUT_SESSION( iLiveSession )
    EUNIT_ASSERT_EQUALS( TInt( CMceSession::EEstablished ),
                         TInt( iLiveSession->iSession->iState ) )  
    EUNIT_ASSERT_EQUALS( TInt( MLcSession::EOpen ), 
                         TInt( iLiveSession->LcSessionState() ) )

    // MLcSession::EInitialized
    iLiveSession->iSession->iState = CMceSession::EIdle;
    EUNIT_ASSERT_EQUALS( TInt( MLcSession::EInitialized ), 
                         TInt( iLiveSession->LcSessionState() ) )

    // MLcSession::EReceived
    iLiveSession->iSession->iState = CMceSession::EIncoming;
    EUNIT_ASSERT_EQUALS( TInt( MLcSession::EReceived ),
                         TInt( iLiveSession->LcSessionState() ) )

    iLiveSession->iSession->iState = CMceSession::EProceeding; 
    EUNIT_ASSERT_EQUALS( TInt( MLcSession::EReceived ), 
                         TInt( iLiveSession->LcSessionState() ) )
    
    iLiveSession->iSession->iState = CMceSession::EReserving; 
    EUNIT_ASSERT_EQUALS( TInt( MLcSession::EReceived ), 
                         TInt( iLiveSession->LcSessionState() ) )   
    
    // MLcSession::EOpening
    iLiveSession->iSession->iState = CMceSession::EOffering;
    EUNIT_ASSERT_EQUALS( TInt( MLcSession::EOpening ), 
                         TInt( iLiveSession->LcSessionState() ) )
    
    iLiveSession->iSession->iState = CMceSession::EAnswering;
    EUNIT_ASSERT_EQUALS( TInt( MLcSession::EOpening ), 
                         TInt( iLiveSession->LcSessionState() ) )

    // MLcSession::EClosing
    iLiveSession->iSession->iState = CMceSession::ECancelling; 
    EUNIT_ASSERT_EQUALS( TInt( MLcSession::EClosing ), 
                         TInt( iLiveSession->LcSessionState() ) )
    
    iLiveSession->iSession->iState = CMceSession::ETerminating; 
    EUNIT_ASSERT_EQUALS( TInt( MLcSession::EClosing ), 
                         TInt( iLiveSession->LcSessionState() ) )
    
    // MLcSession::EClosed
    iLiveSession->iSession->iState = CMceSession::ETerminated;
    EUNIT_ASSERT_EQUALS( TInt( MLcSession::EClosed ), 
                         TInt( iLiveSession->LcSessionState() ) )   
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_RemoteVideoPlayerL()
    {
    EUNIT_ASSERT( iLiveSession->CMusEngMceSession::RemoteVideoPlayer() == NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_LocalVideoPlayerL()
    {
    EUNIT_ASSERT( iLiveSession->CMusEngMceSession::LocalVideoPlayer() == NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_LocalDisplayNameL()
    {
    EUNIT_ASSERT_EQUALS( KNullDesC(), iLiveSession->LocalDisplayName() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_RemoteDisplayNameL()
    {
    // Clear the thread common storage to ensrue its not polluted. 
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                   NMusSessionApi::KContactName,
                                   KNullDesC) );
    
    EUNIT_ASSERT_EQUALS( KNullDesC(), iLiveSession->RemoteDisplayName() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_RemoteDetailsL()
    {
    // Clear the thread common storage to ensrue its not polluted. 
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                   NMusSessionApi::KTelNumber,
                                   KNullDesC) );
    
    EUNIT_ASSERT_EQUALS( KNullDesC(), iLiveSession->RemoteDetails() )
    
    _LIT( KMusTestDetails, "1222233499" );
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                   NMusSessionApi::KTelNumber,
                                   KMusTestDetails) );
    EUNIT_ASSERT_EQUALS( KMusTestDetails(), iLiveSession->RemoteDetails() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_SetParameterL()
    {
    const TInt dummy( 0 );
    EUNIT_ASSERT_EQUALS( KErrNotSupported, 
                         iLiveSession->SetParameter( dummy, dummy ) )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_ParameterValueL()
    {
    const TInt dummy( 0 );
    EUNIT_ASSERT_EQUALS( KErrNotSupported, 
                         iLiveSession->ParameterValue( dummy ) )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_IsLcAudioMutedL()
    {
    // No MCE session
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->IsLcAudioMutedL(), KErrNotReady )
    
    ESTABLISH_OUT_SESSION( iLiveSession )
    
    // No audio
    EUNIT_ASSERT( iLiveSession->IsLcAudioMutedL() )
    
    // Contains audio, but explicitly muted
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    iLiveSession->iSession->AddStreamL( audioStream );
    CleanupStack::Pop( audioStream );
    iLiveSession->iExplicitlyMuted = ETrue;
    EUNIT_ASSERT( iLiveSession->IsLcAudioMutedL() )
    
    // Contains audio, not muted
    iLiveSession->iExplicitlyMuted = EFalse;
    EUNIT_ASSERT( !iLiveSession->IsLcAudioMutedL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_MuteLcAudioL()
    {
    // No MCE session
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->IsLcAudioMutedL(), KErrNotReady )
    
    ESTABLISH_OUT_SESSION( iLiveSession )
    
    // Mute 
    iLiveSession->MuteLcAudioL( ETrue );
    EUNIT_ASSERT( iLiveSession->iExplicitlyMuted )
    EUNIT_ASSERT( iLiveSession->IsLcAudioMutedL() )
    
    // Unmute
    iLiveSession->MuteLcAudioL( EFalse );
    EUNIT_ASSERT( !iLiveSession->iExplicitlyMuted )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_MuteLcMicL()
    {
    // Mute
    iLiveSession->MuteLcMicL( ETrue );
    User::LeaveIfError( RProperty::Set( KPSUidTelMicrophoneMuteStatus,
                                        KTelMicrophoneMuteState,
                                        EPSTelMicMuteOn ) );  
    EUNIT_ASSERT( iLiveSession->IsLcMicMutedL() )
    
    // Unmute
    iLiveSession->MuteLcMicL( EFalse );
    User::LeaveIfError( RProperty::Set( KPSUidTelMicrophoneMuteStatus,
                                        KTelMicrophoneMuteState,
                                        EPSTelMicMuteOff ) );    
    EUNIT_ASSERT( !iLiveSession->IsLcMicMutedL() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_EnableLcLoudspeakerL()
    {
    // Check that enabling the loudspeaker is allowed
    EUNIT_ASSERT( iLiveSession->IsEnablingLcLoudspeakerAllowed() )
    
    // Disable 
    iLiveSession->EnableLcLoudspeakerL( EFalse );
    EUNIT_ASSERT( !iLiveSession->IsLcLoudspeakerEnabled() )
    
    // Enable
    iLiveSession->EnableLcLoudspeakerL( ETrue );
    EUNIT_ASSERT( iLiveSession->IsLcLoudspeakerEnabled() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_SetLcVolumeL()
    {
    ESTABLISH_OUT_SESSION( iLiveSession )
    
    // Set volume
    const TInt volume( 5 );
    iLiveSession->SetLcVolumeL( volume );
    EUNIT_ASSERT_EQUALS( volume, iLiveSession->LcVolumeL() );
    
    // Increase volume
    iLiveSession->IncreaseLcVolumeL();
    EUNIT_ASSERT_EQUALS( volume+1, iLiveSession->LcVolumeL() );
    
    // Decrease volume
    iLiveSession->DecreaseLcVolumeL();
    EUNIT_ASSERT_EQUALS( volume, iLiveSession->LcVolumeL() );    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_GetSessionTimeL()
    {
    EUNIT_ASSERT( iLiveSession->GetSessionTime() < TTimeIntervalSeconds( 0 ) );
    
    // Invite
    iLiveSession->EstablishLcSessionL();
    
    EUNIT_ASSERT( iLiveSession->GetSessionTime() < TTimeIntervalSeconds( 0 ) );
    
    // Simulate establishment
    iLiveSession->iSession->iState = CMceSession::EEstablished;
    iLiveSession->iStartTime.HomeTime();
    
    TTimeIntervalSeconds sessionTime = iLiveSession->GetSessionTime();
    EUNIT_ASSERT( sessionTime >= TTimeIntervalSeconds( 0 ) );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_HandleTerminationL()
    { 
    // Try different values
    iLiveSession->CMusEngMceSession::HandleTermination( KSipStatusCodeNoCodeSet,
                                                        KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionStateChanged ) )
    iLcSessionObserver->Reset();

    iLiveSession->CMusEngMceSession::HandleTermination( KSipStatusCode200OK,
                                                        KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionStateChanged ) )
    iLcSessionObserver->Reset();

    iLiveSession->CMusEngMceSession::HandleTermination( KSipStatusCodeUnknown,
                                                        KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError, KErrGeneral )
    iLcSessionObserver->Reset();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_AdjustVideoCodecL()
    {
    CRepository::SetStubAvcConfigKeysL( KNullDesC8() );
        
    CMceVideoCodec* codecH263 = CMceH263Codec::NewLC( KMceSDPNameH263() );
    CMceVideoCodec* codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    CMceVideoCodec* codecAvcFromFile = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    
    iLiveSession->CMusEngMceSession::AdjustVideoCodecL( *codecH263,
                                                        KMceCameraSource );
    EUNIT_ASSERT( !iLiveSession->iStoreEncoderConfigInfo )

    iLiveSession->CMusEngMceSession::AdjustVideoCodecL( *codecAvcFromFile,
                                                        KMceFileSource);  
    EUNIT_ASSERT( !iLiveSession->iStoreEncoderConfigInfo )
    
    iLiveSession->CMusEngMceSession::AdjustVideoCodecL( *codecAvc,
                                                        KMceCameraSource );  
    EUNIT_ASSERT( iLiveSession->iStoreEncoderConfigInfo )
    
    CleanupStack::PopAndDestroy( codecAvcFromFile );
    CleanupStack::PopAndDestroy( codecAvc );
    CleanupStack::PopAndDestroy( codecH263 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_AdjustAudioCodecL()
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
void UT_CMusEngMceSession::UT_RectChangedL()
    {
    // Does nothing before session establishment
    
    iLiveSession->RectChangedL();
    
    ESTABLISH_OUT_SESSION( iLiveSession );

    // Next leads to a call to RectChangedL()
    iLiveSession->SetRectL( TRect( 100, 200, 300, 400 ) ); 
    
    // Display size has been updated
    
    CMceDisplaySink* display = 
        MusEngMceUtils::GetDisplayL( *( iLiveSession->iSession ) );
    
    EUNIT_ASSERT( display->DisplayRectL() == iLiveSession->Rect() )
    
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
void UT_CMusEngMceSession::UT_SetSessionSdpLinesL()
    {
    // set operator variant off
    iReceiveSession->iOperatorVariant = EFalse;
    CRepository::SetStubGlobal( MusSettingsKeys::KVideoBandwidth,
                                128 );
                     
    // 1. There is b=AS line at session level
    // => Xapplication, b=AS and b=TIAS set to session level  
    CMceInSession* inSession = CMceInSession::NewL( *iReceiveSession->iManager,
                                                    KTestOriginator );
    CleanupStack::PushL( inSession );    
    EUNIT_ASSERT( !inSession->iSessionSDPLines );
    inSession->iSessionSDPLines = new ( ELeave ) CDesC8ArrayFlat( 1 );
    inSession->iSessionSDPLines->AppendL( KMusEngSessionSdpLineBandwidthField() );
        
    iReceiveSession->SetSessionSdpLinesL( *inSession, ETrue );
    
    MDesC8Array* sdpLines = inSession->iSessionSDPLines;    
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 3 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineXApplication() );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 1 ).Find( 
            KMusEngSessionSdpLineBandwidthLine() ) == 0 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 2 ).Find( 
            KMusEngSessionSdpLineTiasLine() ) == 0 );
    
    
    // 2. There are b=TIAS sdp line at session  
    // => Xapplication, b=AS and b=TIAS set to session level
    delete inSession->iSessionSDPLines;
    inSession->iSessionSDPLines = NULL;
    inSession->iSessionSDPLines = new ( ELeave ) CDesC8ArrayFlat( 1 );
    inSession->iSessionSDPLines->AppendL( KMusEngSessionSdpLineTiasLine() );
        
    iReceiveSession->SetSessionSdpLinesL( *inSession, ETrue );
    
    sdpLines = inSession->iSessionSDPLines;    
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 3 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineXApplication() );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 1 ).Find( 
            KMusEngSessionSdpLineBandwidthLine() ) == 0 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 2 ).Find( 
            KMusEngSessionSdpLineTiasLine() ) == 0 );
            
    // 3. Simulating outcoming session, i.e. 2d param aForceBandwidthLine is EFalse  
    // => only Xapplication SDP line is set
    delete inSession->iSessionSDPLines;
    inSession->iSessionSDPLines = NULL;
         
    iReceiveSession->SetSessionSdpLinesL( *inSession, EFalse );
     
    sdpLines = inSession->iSessionSDPLines;    
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 1 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineXApplication() );
    
    // 4. No KVideoBandwidth entry in CenRep => TIAS usage should be disabled
    // There is AS and TIAS at session level => AS is taken at session level
    CRepository::iForceFailWithCode = KErrNotFound;
    delete inSession->iSessionSDPLines;
    inSession->iSessionSDPLines = NULL;
    inSession->iSessionSDPLines = new ( ELeave ) CDesC8ArrayFlat( 1 );
    inSession->iSessionSDPLines->AppendL( KMusEngSessionSdpLineTiasLine() );
    inSession->iSessionSDPLines->AppendL( KMusEngSessionSdpLineBandwidthField() );
        
    iReceiveSession->SetSessionSdpLinesL( *inSession, ETrue );
    
    sdpLines = inSession->iSessionSDPLines;    
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 2 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineXApplication() );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 1 ) == KMusEngSessionSdpLineBandwidthField() );
    
    CleanupStack::PopAndDestroy( inSession );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_SetSessionSdpLines_OperatorL()
    {
    // set operator variant
    iReceiveSession->iOperatorVariant = ETrue;

    // 1. There is b=TIAS sdp line in session => only application and  
    // type lines are set, no bandwidth related attributes
    CMceInSession* inSession = CMceInSession::NewL( *iReceiveSession->iManager,
                                     KTestOriginator );
    CleanupStack::PushL( inSession );        
    
    EUNIT_ASSERT( !inSession->iSessionSDPLines );
    inSession->iSessionSDPLines = new ( ELeave ) CDesC8ArrayFlat( 1 );
    inSession->iSessionSDPLines->AppendL( KMusEngSessionSdpLineTiasLine() );
        
    iReceiveSession->SetSessionSdpLinesL( *inSession, ETrue );
    
    MDesC8Array* sdpLines = inSession->iSessionSDPLines;    
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 2 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineApplication() );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 1 ) == KMusEngSessionSdpLineType() );    

    // 2. There are b=AS and b=TIAS sdp lines in session => application and  
    // type SDP lines as well as b=AS bandwidth attributes are set
    delete inSession->iSessionSDPLines;
    inSession->iSessionSDPLines = NULL;
    inSession->iSessionSDPLines = new ( ELeave ) CDesC8ArrayFlat( 2 );
    inSession->iSessionSDPLines->AppendL( KMusEngSessionSdpLineBandwidthField() );
    inSession->iSessionSDPLines->AppendL( KMusEngSessionSdpLineTiasLine() );
        
    iReceiveSession->SetSessionSdpLinesL( *inSession, ETrue );
    
    sdpLines = inSession->iSessionSDPLines;    
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 3 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineApplication() );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 1 ) == KMusEngSessionSdpLineType() );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 2 ) == KMusEngSessionSdpLineBandwidthField() );
        
    
    // 3. Simulating outcoming session, i.e. 2d param aForceBandwidthLine is EFalse  
    // => only application and type SDP lines are set
    delete inSession->iSessionSDPLines;
    inSession->iSessionSDPLines = NULL;
        
    iReceiveSession->SetSessionSdpLinesL( *inSession, EFalse );
    
    sdpLines = inSession->iSessionSDPLines;    
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 2 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineApplication() );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 1 ) == KMusEngSessionSdpLineType() );
        
    CleanupStack::PopAndDestroy( inSession );   
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_SetMediaSdpLinesL()
    {
    // set operator variant off
    iReceiveSession->iOperatorVariant = EFalse;
    CRepository::SetStubGlobal( MusSettingsKeys::KVideoBandwidth,
                                128 );

    // 1. There is no b=AS, b=TIAS sdp lines at session level
    // => b=AS and b=TIAS are taken at media level
    CMceInSession* inSession = CMceInSession::NewL( *iReceiveSession->iManager,
                                                    KTestOriginator );
    CleanupStack::PushL( inSession );
    
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();        
    videoStream->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop(); //rtpSource        
    videoStream->AddSinkL( CMceDisplaySink::NewLC( *iReceiveSession->iManager ) );
    CleanupStack::Pop(); //displaySink
        
    inSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );

    iReceiveSession->SetMediaSdpLinesL( *videoStream, ETrue );
    
    MDesC8Array* sdpLines = inSession->Streams()[ 0 ]->iMediaSDPLines;
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 2 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ).Find( 
            KMusEngSessionSdpLineBandwidthLine() ) == 0 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 1 ).Find( 
            KMusEngSessionSdpLineTiasLine() ) == 0 );
    
    
    // 2. There is b=AS sdp line at session and media level
    // => b=AS and b=TIAS are taken at media level
    delete inSession->iSessionSDPLines;
    inSession->iSessionSDPLines = NULL;
    inSession->iSessionSDPLines = new ( ELeave ) CDesC8ArrayFlat( 1 );
    inSession->iSessionSDPLines->AppendL( KMusEngSessionSdpLineBandwidthField() );
    
    iReceiveSession->SetMediaSdpLinesL( *videoStream, ETrue );
    
    sdpLines = inSession->Streams()[ 0 ]->iMediaSDPLines;
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 2 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ).Find( 
            KMusEngSessionSdpLineBandwidthLine() ) == 0 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 1 ).Find( 
            KMusEngSessionSdpLineTiasLine() ) == 0 );

    // 3. Simulating outcoming session, i.e. 2d param aForceBandwidthLine is EFalse
    // => no bandwidth attributes at media level
    delete inSession->iSessionSDPLines;
    inSession->iSessionSDPLines = NULL;
     
    delete inSession->Streams()[ 0 ]->iMediaSDPLines;
    inSession->Streams()[ 0 ]->iMediaSDPLines = NULL;
     
    iReceiveSession->SetMediaSdpLinesL( *videoStream, EFalse );
     
    sdpLines = inSession->Streams()[ 0 ]->iMediaSDPLines;
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 0 );

    // 4. No KVideoBandwidth entry in CenRep => TIAS usage should be disabled
    // There is no bandwidth attributes in session => AS is taken at meida level
    CRepository::iForceFailWithCode = KErrNotFound;
    delete inSession->iSessionSDPLines;
    inSession->iSessionSDPLines = NULL;
        
    iReceiveSession->SetMediaSdpLinesL( *videoStream, ETrue );
    
    sdpLines = inSession->Streams()[ 0 ]->iMediaSDPLines;
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 1 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineBandwidthField() );

    CleanupStack::PopAndDestroy( inSession );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_SetMediaSdpLines_OperatorL()
    {
    // set operator variant
    iReceiveSession->iOperatorVariant = ETrue;

    // 1. There is no b=AS sdp line at session level
    // => b=AS is taken at media level
    CMceInSession* inSession = CMceInSession::NewL( *iReceiveSession->iManager,
                                                    KTestOriginator );
    CleanupStack::PushL( inSession );
  
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();        
    videoStream->SetSourceL( CMceRtpSource::NewLC() );
    CleanupStack::Pop(); //rtpSource        
    videoStream->AddSinkL( CMceDisplaySink::NewLC( *iReceiveSession->iManager ) );
    CleanupStack::Pop(); //displaySink
        
    inSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );

    EUNIT_ASSERT( !inSession->iSessionSDPLines );   
    inSession->iSessionSDPLines = new ( ELeave ) CDesC8ArrayFlat( 1 );
    inSession->iSessionSDPLines->AppendL( KMusEngSessionSdpLineTiasLine() );
        
    iReceiveSession->SetMediaSdpLinesL( *videoStream, ETrue );
    
    MDesC8Array* sdpLines = inSession->Streams()[ 0 ]->iMediaSDPLines;
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 1 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineBandwidthField() );
    
    
    // 2. There is b=AS sdp line at session and media level
    // => b=AS is not taken at media level
    delete inSession->iSessionSDPLines;
    inSession->iSessionSDPLines = NULL;
    inSession->iSessionSDPLines = new ( ELeave ) CDesC8ArrayFlat( 1 );
    inSession->iSessionSDPLines->AppendL( KMusEngSessionSdpLineBandwidthField() );
    
    delete inSession->Streams()[ 0 ]->iMediaSDPLines;
    inSession->Streams()[ 0 ]->iMediaSDPLines = NULL;
    inSession->Streams()[ 0 ]->iMediaSDPLines =  
                new ( ELeave ) CDesC8ArrayFlat( 1 ); 
    inSession->Streams()[ 0 ]->iMediaSDPLines->AppendL( 
                KMusEngSessionSdpLineBandwidthField() );
    
    iReceiveSession->SetMediaSdpLinesL( *videoStream, ETrue );
    
    EUNIT_ASSERT( inSession->Streams()[ 0 ]->iMediaSDPLines );
    EUNIT_ASSERT( inSession->Streams()[ 0 ]->iMediaSDPLines->MdcaCount() == 0 );


    // 3. Simulating outcoming session, i.e. 2d param aForceBandwidthLine is EFalse
    // => b=AS is taken at media level
    delete inSession->iSessionSDPLines;
    inSession->iSessionSDPLines = NULL;
     
    delete inSession->Streams()[ 0 ]->iMediaSDPLines;
    inSession->Streams()[ 0 ]->iMediaSDPLines = NULL;
     
    iReceiveSession->SetMediaSdpLinesL( *videoStream, EFalse );
     
    sdpLines = inSession->Streams()[ 0 ]->iMediaSDPLines;
    EUNIT_ASSERT( sdpLines );
    EUNIT_ASSERT( sdpLines->MdcaCount() == 1 );
    EUNIT_ASSERT( sdpLines->MdcaPoint( 0 ) == KMusEngSessionSdpLineBandwidthField() );
    
    CleanupStack::PopAndDestroy( inSession );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_AdjustStreamsAndCodecsL()
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
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_IncomingSessionL()
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
void UT_CMusEngMceSession::UT_IncomingUpdateL()
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
void UT_CMusEngMceSession::UT_StreamStateChangedL()
    {
    // Try without a session, nothing happens
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    iLiveSession->CMusEngMceSession::StreamStateChanged( *audioStream );
    CleanupStack::PopAndDestroy( audioStream );
    
    // Simulate sending invite
    iLiveSession->EstablishLcSessionL();
    
    // No observer set, so expect nothing to be done, cannot be asserted
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];

    // Try all the stream states
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EStreaming, stream is streaming, other than video out- or instream
    changedStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->CMusEngMceSession::StreamStateChanged( 
                                *iLiveSession->iSession->Streams()[1] );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    iLcSessionObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    }
    

// -----------------------------------------------------------------------------
// All the possibilities are tested in order to make sure unit tests are changed
// when function is changed
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_StreamStateChangedWithSourceL()
    {
    // Try without a session, nothing happens
    iLiveSession->CMusEngMceSession::StreamStateChanged( 
                                        *CMceAudioStream::NewLC(),
                                        *CMceRtpSource::NewLC() );
                                        
    CleanupStack::PopAndDestroy(); // rtp                                    
    CleanupStack::PopAndDestroy(); // audiostream
    
    // Simulate sending invite
    iLiveSession->EstablishLcSessionL();
    
    // Try all the stream states
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    }


// -----------------------------------------------------------------------------
// All the possibilities are tested in order to make sure unit tests are changed
// when function is changed
// -----------------------------------------------------------------------------
//  
void UT_CMusEngMceSession::UT_StreamStateChangedWithSinkL()
    {
    // Try without a session, nothing happens
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    iLiveSession->CMusEngMceSession::StreamStateChanged( *audioStream );
    CleanupStack::PopAndDestroy( audioStream );
    
    // Simulate sending invite
    iLiveSession->EstablishLcSessionL();
    
    // Try all the stream states
    CMceMediaStream* changedStream = iLiveSession->iSession->Streams()[0];
    
    // EUninitialized, stream is created, unexpected change, nothing happens
    changedStream->iState = CMceMediaStream::EUninitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EInitialized, stream is initialized
    changedStream->iState = CMceMediaStream::EInitialized;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EBuffering, stream is buffering
    changedStream->iState = CMceMediaStream::EBuffering;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EIdle, stream is not receiving RTP
    changedStream->iState = CMceMediaStream::EIdle;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EStreaming, stream is streaming
    changedStream->iState = CMceMediaStream::EStreaming;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EPlayerStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EDisabled, stream is explicitly disabled
    changedStream->iState = CMceMediaStream::EDisabled;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ENoResources, stream has no needed resources to stream
    changedStream->iState = CMceMediaStream::ENoResources;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscodingRequired, stream requires non-realtime transcoding
    changedStream->iState = CMceMediaStream::ETranscodingRequired;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ETranscoding, stream is transcoding in non-realtime
    changedStream->iState = CMceMediaStream::ETranscoding;
    iLiveSession->CMusEngMceSession::StreamStateChanged( *changedStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    }        


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngMceSession::UT_SessionStateChangedL()
    {
    // Simulate sending invite
    iLiveSession->EstablishLcSessionL();   

    // No container, so expect to be handled as internal error
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, NULL );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError, KErrGeneral )
    iLcSessionObserver->Reset();    
    
    // Try to tell session about some other session's state transition
    
    TMceTransactionDataContainer container;
 
    iLiveSession->SessionStateChanged( *iSomeOtherSession->iSession, &container );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );    
      
    // Try all the normal cases:
    
    // EIdle
    iLiveSession->iSession->iState = CMceSession::EIdle;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    // Set up any text to simulate reasonphrase
    HBufC8* anyReasonPhrase = KTestOriginator().AllocL();
    container.SetReasonPhrase( anyReasonPhrase );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError, KErrGeneral )
    iLcSessionObserver->Reset();
    container.Clear();
    
    // EOffering
    iLiveSession->iSession->iState = CMceSession::EOffering;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EIncoming
    iLiveSession->iSession->iState = CMceSession::EIncoming;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError, KErrGeneral )
    iLcSessionObserver->Reset();
    
    // EReserving
    iLiveSession->iSession->iState = CMceSession::EReserving;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EAnswering
    iLiveSession->iSession->iState = CMceSession::EAnswering;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EProceeding
    iLiveSession->iSession->iState = CMceSession::EProceeding;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // EEstablished
    iLiveSession->iSession->iState = CMceSession::EEstablished;
    container.SetStatusCode( KSipStatusCode200OK );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionStateChanged ) )
    iLcSessionObserver->Reset();
    
    // EEstablished again (meaning expiration of session timer 
    // and refresment of session )
    iLiveSession->iSession->iState = CMceSession::EEstablished;
    container.SetStatusCode( KSipStatusCode200OK );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    
    // ECancelling
    iLiveSession->iSession->iState = CMceSession::ECancelling;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    iLcSessionObserver->Reset();
    
    // ETerminating
    iLiveSession->iSession->iState = CMceSession::ETerminating;
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );
    iLcSessionObserver->Reset();
    
    // ETerminated
    iLiveSession->iSession->iState = CMceSession::ETerminated;
    container.SetStatusCode( KSipStatusCode200OK );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, 
                                       &container );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionStateChanged ) )
    iLcSessionObserver->Reset();
    
    // Default case
    iLiveSession->iSession->iState = static_cast<CMceSession::TState>( 10 );
    container.SetStatusCode( KSipStatusCodeNoCodeSet );
    iLiveSession->SessionStateChanged( *iLiveSession->iSession, &container );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() );

    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//     
void UT_CMusEngMceSession::UT_HandleSessionStateChanged_EncoderKeyStoringL()
    {
    // Make repository empty, config keys must be written
    CRepository::SetStubAvcConfigKeysL( KNullDesC8() );
    
    // Invite    
    iLiveSession->EstablishLcSessionL();
    iLiveSession->iSession->iState = CMceSession::EEstablished;
    
    // Force failure on CenRep
    CRepository::iForceFailWithCode = KErrNoMemory;
    
    // Simulate session state transition notification
    iLiveSession->HandleSessionStateChanged( 
        *iLiveSession->iSession, 200, KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError, KErrNoMemory )    
    
    // Normal case, something will be written to CenRep    
    iClipSession->EstablishLcSessionL();
    iClipSession->iSession->iState = CMceSession::EEstablished;
    iClipSession->HandleSessionStateChanged( 
                            *iClipSession->iSession, 200, KNullDesC8() );  
    HBufC8* info = MultimediaSharingSettings::EncoderConfigInfoLC();
    EUNIT_ASSERT_NOT_EQUALS( *info, KMusAvcBitrateLevel_1b_ConfigKey() )
    CleanupStack::PopAndDestroy( info );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//     
void UT_CMusEngMceSession::UT_SessionConnectionStateChangedL()
    {
    
    // Try to tell session about another session's connection state change
    
    iLiveSession->SessionConnectionStateChanged( *iSomeOtherSession->iSession, 
                                                 ETrue );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() ); // No session yet, nothing happened
    
    // Simulate sending invite
    iLiveSession->EstablishLcSessionL();
    
    // Now try again to notify session about session connection state change
    // of an another session
    iLiveSession->SessionConnectionStateChanged( *iSomeOtherSession->iSession, 
                                                 ETrue );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() ); // Wrong session, nothing happened
    
    // Try tell session connection state is active
    iLiveSession->SessionConnectionStateChanged( *iLiveSession->iSession, 
                                                 ETrue );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() ); // Session active, nothing happened
    
    // Try tell session connection state is inactive
    iLiveSession->SessionConnectionStateChanged( *iLiveSession->iSession, 
                                                 EFalse );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::EConnectionLost ) )
    iLcSessionObserver->Reset();  
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusEngMceSession::UT_FailedL()
    {
    // Try to tell session about failure of another session

    iLiveSession->Failed( *iSomeOtherSession->iSession, 0 );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() ); // No session yet, nothing happened
    
    // Simulate sending invite
    iLiveSession->EstablishLcSessionL();
    
    // Now try again to notify session about failure of an another session
    iLiveSession->Failed( *iSomeOtherSession->iSession, 0 );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() ); // Wrong session, nothing happened
    
    // Tell session that it has failed
    iLiveSession->Failed( *iLiveSession->iSession, KErrNotFound );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError, KErrNotFound )
    iLcSessionObserver->Reset();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//     
void UT_CMusEngMceSession::UT_UpdateFailedL()
    {
    // Try to tell session about update failure of another session
    iLiveSession->UpdateFailed( *iSomeOtherSession->iSession, NULL );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError, KErrGeneral )
    iLcSessionObserver->Reset();
    
    // Try again with container
    TMceTransactionDataContainer container;
    iLiveSession->UpdateFailed( *iSomeOtherSession->iSession, &container );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() ); // No session yet, nothing happened
    
    // Invite and try again.
    iLiveSession->EstablishLcSessionL();
    iLiveSession->UpdateFailed( *iSomeOtherSession->iSession, &container );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() ); // Wrong session, nothing happened¨
    
    // Try with right session
    iLiveSession->UpdateFailed( *iLiveSession->iSession, &container );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError, KErrGeneral )
    iLcSessionObserver->Reset();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
// 
void UT_CMusEngMceSession::UT_SRReceivedL()
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
void UT_CMusEngMceSession::UT_RRReceivedL()
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
void UT_CMusEngMceSession::UT_SsrcAddedL()
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
void UT_CMusEngMceSession::UT_SsrcRemovedL()
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
void UT_CMusEngMceSession::UT_UpdateTimerEventL()
    {
    iLcSessionObserver->Reset();
    iLiveSession->UpdateTimerEvent();
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::EUnknown ) )
    EUNIT_ASSERT( iLiveSession->iUpdateTimer->IsActive() )
    iLiveSession->iUpdateTimer->Cancel();
    iLcSessionObserver->Reset();
    
    iLiveSession->iSecondsFromLastRtcpReport = 25; // >KMusEngRtcpInactivityThreshold
    iLiveSession->UpdateTimerEvent();
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::EMediaInactivityTimeout ) )
    EUNIT_ASSERT( iLiveSession->iUpdateTimer->IsActive() )
    iLiveSession->iUpdateTimer->Cancel();
    iLcSessionObserver->Reset(); 
    }   


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_IsRoamingBetweenAPsAllowedL()
    {
    // No session
    EUNIT_ASSERT( iLiveSession->IsRoamingBetweenAPsAllowed() )

    // Session idle
    iLiveSession->EstablishLcSessionL();
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
void UT_CMusEngMceSession::UT_SaveContactL()
    {     
    _LIT8( KTestSaveContactAddr, "sip:abc@10.10.10.10");
    
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KTelNumber,
                                        _L("12341234") ) );
    
    // Operator variant
    iLiveSession->iOperatorVariant = ETrue;
    MUSENG_EUNIT_ASSERT_SPECIFIC_LEAVE( 
            iLiveSession->SaveContactL( KTestSaveContactAddr ), KErrNotSupported )
    
    // No address to save
    iLiveSession->iOperatorVariant = EFalse;
    MUSENG_EUNIT_ASSERT_SPECIFIC_LEAVE( 
               iLiveSession->SaveContactL( KNullDesC8 ), KErrArgument )

    // Contact id KErrNotSupported (multiple matching contacts)
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                     NMusSessionApi::KContactId,
                                     KErrNotSupported ) );
    
    MUSENG_EUNIT_ASSERT_SPECIFIC_LEAVE( 
                iLiveSession->SaveContactL( KTestSaveContactAddr ), KErrNotFound )
    
    // SIP field exists already
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KContactId,
                                        2 ) );

    ContactEngineStubHelper::SetSIPFieldFound( ETrue );
    MUSENG_EUNIT_ASSERT_SPECIFIC_LEAVE( 
                   iLiveSession->SaveContactL( KTestSaveContactAddr ), KErrAlreadyExists )
    
    // No correct field in fields info
    ContactEngineStubHelper::Reset();
    ContactEngineStubHelper::SetSIPFieldInfoFound( EFalse );
    MUSENG_EUNIT_ASSERT_SPECIFIC_LEAVE( 
                       iLiveSession->SaveContactL( KTestSaveContactAddr ), KErrNotFound )
    
    // Success
    ContactEngineStubHelper::Reset();
    iLiveSession->SaveContactL( KTestSaveContactAddr );
    EUNIT_ASSERT( ContactEngineStubHelper::GetCalledFunction() == EContactEngineStubSetText );
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_RectL()
    {
    iLiveSession->SetRectL( TRect( 0, 0, 100, 100 ) );    
    TRect rect = iLiveSession->Rect();
    
    // Initial values
    EUNIT_ASSERT( rect.iTl.iX == 0 );
    EUNIT_ASSERT( rect.iTl.iY == 0 );
    EUNIT_ASSERT( rect.iBr.iX == 100 );
    EUNIT_ASSERT( rect.iBr.iY == 100 );
    
    iLiveSession->iRect.iTl.iX = 100;
    iLiveSession->iRect.iTl.iY = 200;
    iLiveSession->iRect.iBr.iX = 300;
    iLiveSession->iRect.iBr.iY = 400;
    
    rect = iLiveSession->Rect();
    
    EUNIT_ASSERT( rect.iTl.iX == 100 );
    EUNIT_ASSERT( rect.iTl.iY == 200 );
    EUNIT_ASSERT( rect.iBr.iX == 300 );
    EUNIT_ASSERT( rect.iBr.iY == 400 );  
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_SetRectLL()
    {
    TRect rect;

    rect.iTl.iX = 100;
    rect.iTl.iY = 200;
    rect.iBr.iX = 300;
    rect.iBr.iY = 400;

    iLiveSession->SetRectL( rect );
    
    EUNIT_ASSERT( iLiveSession->iRect.iTl.iX == 100 );
    EUNIT_ASSERT( iLiveSession->iRect.iTl.iY == 200 );
    EUNIT_ASSERT( iLiveSession->iRect.iBr.iX == 300 );
    EUNIT_ASSERT( iLiveSession->iRect.iBr.iY == 400 );

    ESTABLISH_OUT_SESSION( iLiveSession );
    
    CMceDisplaySink* display = 
                MusEngMceUtils::GetDisplayL( *( iLiveSession->iSession ) );
    TRect displayRect = display->DisplayRectL();
    
    EUNIT_ASSERT( displayRect.iTl.iX == 100 );
    EUNIT_ASSERT( displayRect.iTl.iY == 200 );
    EUNIT_ASSERT( displayRect.iBr.iX == 300 );
    EUNIT_ASSERT( displayRect.iBr.iY == 400 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_SecondaryRectL()
    {
    iLiveSession->SetSecondaryRectL( TRect( 0, 0, 100, 100 ) );    
    TRect rect = iLiveSession->SecondaryRect();
    
    // Initial values
    EUNIT_ASSERT( rect.iTl.iX == 0 );
    EUNIT_ASSERT( rect.iTl.iY == 0 );
    EUNIT_ASSERT( rect.iBr.iX == 100 );
    EUNIT_ASSERT( rect.iBr.iY == 100 );
    
    iLiveSession->iRect.iTl.iX = 100;
    iLiveSession->iRect.iTl.iY = 200;
    iLiveSession->iRect.iBr.iX = 300;
    iLiveSession->iRect.iBr.iY = 400;
    
    rect = iLiveSession->SecondaryRect();
    
    EUNIT_ASSERT( rect.iTl.iX == 100 );
    EUNIT_ASSERT( rect.iTl.iY == 200 );
    EUNIT_ASSERT( rect.iBr.iX == 300 );
    EUNIT_ASSERT( rect.iBr.iY == 400 );  
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_SetSecondaryRectLL()
    {
    TRect rect;

    rect.iTl.iX = 100;
    rect.iTl.iY = 200;
    rect.iBr.iX = 300;
    rect.iBr.iY = 400;

    iLiveSession->SetSecondaryRectL( rect );
    
    EUNIT_ASSERT( iLiveSession->iRect.iTl.iX == 100 );
    EUNIT_ASSERT( iLiveSession->iRect.iTl.iY == 200 );
    EUNIT_ASSERT( iLiveSession->iRect.iBr.iX == 300 );
    EUNIT_ASSERT( iLiveSession->iRect.iBr.iY == 400 );

    ESTABLISH_OUT_SESSION( iLiveSession );
    
    CMceDisplaySink* display = 
                MusEngMceUtils::GetDisplayL( *( iLiveSession->iSession ) );
    TRect displayRect = display->DisplayRectL();
    
    EUNIT_ASSERT( displayRect.iTl.iX == 100 );
    EUNIT_ASSERT( displayRect.iTl.iY == 200 );
    EUNIT_ASSERT( displayRect.iBr.iX == 300 );
    EUNIT_ASSERT( displayRect.iBr.iY == 400 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_EnableDisplayLL()
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
    
    EUNIT_ASSERT( iLiveSession->IsDisplayEnabled() )
    EUNIT_ASSERT( iLiveSession->IsDisplayActive() )
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
void UT_CMusEngMceSession::UT_SetEncodingDeviceLL()
    {
    CMceAvcCodec* codec = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    
    // Generic error in CenRep, leave expected
    CRepository::iForceFailWithCode = KErrNotReady;
    EUNIT_ASSERT_LEAVE( iLiveSession->SetEncodingDeviceL( *codec ) )
    EUNIT_ASSERT_EQUALS( codec->iEncodingDecodingDevice, TUid::Uid( 0 ) )
    
    // No entry in CenRep, default value expected
    CRepository::iForceFailWithCode = KErrNotFound;
    const TUid KDefaultEncodingDevice( TUid::Uid( 0x20001C13 ) );
    iLiveSession->SetEncodingDeviceL( *codec );
    EUNIT_ASSERT_EQUALS( codec->iEncodingDecodingDevice, 
                         KDefaultEncodingDevice )
    
    // Entry in CenRep
    const TInt KSomeEncodingDevice( 0x20001C15 );
    CRepository::iStaticEncoderUid = KSomeEncodingDevice;
    iLiveSession->SetEncodingDeviceL( *codec );
    EUNIT_ASSERT_EQUALS( codec->iEncodingDecodingDevice, 
                         TUid::Uid( KSomeEncodingDevice ) )
                                             
    CleanupStack::PopAndDestroy( codec );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_SetCodecConfigKeyLL()
    {
    CMceAvcCodec* codec = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    codec->SetAllowedBitrates( KMceAvcBitrateLevel1b );

    // Generic error in CenRep, leave expected
    CRepository::iForceFailWithCode = KErrNotReady;
    EUNIT_ASSERT_LEAVE( iLiveSession->SetCodecConfigKeyL( *codec ) );
    EUNIT_ASSERT( !codec->iConfigKey )
    EUNIT_ASSERT( !iLiveSession->iStoreEncoderConfigInfo )
    CRepository::iForceFailWithCode = KErrNone;
    
    // No entry in CenRep, nothing happens
    iLiveSession->SetCodecConfigKeyL( *codec );
    EUNIT_ASSERT( !codec->iConfigKey )
    EUNIT_ASSERT( !iLiveSession->iStoreEncoderConfigInfo )
    
    // Empty entry in CenRep
    CRepository::SetStubAvcConfigKeysL( KNullDesC8() );
    iLiveSession->SetCodecConfigKeyL( *codec );
    EUNIT_ASSERT( !codec->iConfigKey )
    EUNIT_ASSERT( iLiveSession->iStoreEncoderConfigInfo )
    iLiveSession->iStoreEncoderConfigInfo = EFalse; // emulate original situation
    
    // Entry in CenRep
    CRepository::SetStubAvcConfigKeysL( KMusAvcBitrateLevel_1b_ConfigKey );
    iLiveSession->SetCodecConfigKeyL( *codec );
    EUNIT_ASSERT( codec->iConfigKey )
    HBufC8* key = codec->ConfigKeyL();
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1bTestText() )
    CleanupStack::PopAndDestroy( key );
    EUNIT_ASSERT( !iLiveSession->iStoreEncoderConfigInfo )
    
    CleanupStack::PopAndDestroy( codec );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_ReadCodecConfigKeyLL()
    {
    HBufC8* key( NULL );
    
    // Try with H263, leave expected
    CMceVideoCodec* codecH263 = CMceH263Codec::NewLC( KMceSDPNameH263() );  
    EUNIT_ASSERT_LEAVE( key = iLiveSession->ReadCodecConfigKeyL( *codecH263 ) )
    CleanupStack::PopAndDestroy( codecH263 );
    
    // Try without an entry in CenRep, leave expected
    CMceVideoCodec* codec = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    codec->SetAllowedBitrates( 0 | KMceAvcBitrateLevel1 );
    EUNIT_ASSERT_LEAVE( key = iLiveSession->ReadCodecConfigKeyL( *codec ) )

    // Create CenRep entry which does not include wanted bitrate, NULL value
    // expected
    CRepository::SetStubAvcConfigKeysL( KMusAvcBitrateLevel1bTestText() );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( !key )
    
    // Create entry with all AVC keys and try with all the values
    CRepository::SetStubAvcConfigKeysL( KMusAvcAllLevelsConcatenation() );                           
    
    // KMceAvcBitrateLevel1
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1TestText );
    CleanupStack::PopAndDestroy( key );
    
    // KMceAvcBitrateLevel1b
    codec->SetAllowedBitrates( KMceAvcBitrateLevel1b );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1bTestText );
    CleanupStack::PopAndDestroy( key );
    
    // KMceAvcBitrateLevel1_1
    codec->SetAllowedBitrates( KMceAvcBitrateLevel1_1 );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1_1TestText );
    CleanupStack::PopAndDestroy( key );
    
    // KMceAvcBitrateLevel1_2
    codec->SetAllowedBitrates( KMceAvcBitrateLevel1_2 );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1_2TestText );
    CleanupStack::PopAndDestroy( key );
    
    // KMceAvcBitrateLevel1_3
    codec->SetAllowedBitrates( KMceAvcBitrateLevel1_3 );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel1_3TestText );
    CleanupStack::PopAndDestroy( key );
    
    // KMceAvcBitrateLevel2
    codec->SetAllowedBitrates( KMceAvcBitrateLevel2 );
    key = iLiveSession->ReadCodecConfigKeyL( *codec );
    EUNIT_ASSERT( key );
    CleanupStack::PushL( key );
    EUNIT_ASSERT_EQUALS( *key, KMusAvcBitrateLevel2TestText );
    CleanupStack::PopAndDestroy( key );
   
    CleanupStack::PopAndDestroy( codec );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_StoreEncoderConfigInfoLL()
    {
    CRepository::iStaticWriteAvcKeysToStaticData = ETrue;
    
    // Try with clean CenRep
    MultimediaSharingSettings::SetEncoderConfigInfoL( KNullDesC8() );   
    
    ESTABLISH_OUT_SESSION( iLiveSession ) // calls StoreEncoderConfigInfoL
    // If StoreEncoderConfigInfoL leaves with KErrNoMemory,
    // MMusEngSessionObserver::SessionFailed will be called.
    if ( iLcSessionObserver->iCalledFunction == CLcSessionObserverStub::ESessionFailed )
        {
        User::Leave( KErrNoMemory );
        }

    HBufC8* keys = MultimediaSharingSettings::EncoderConfigInfoLC();    
    EUNIT_ASSERT_EQUALS( *keys, KMusAvcBitrateLevel_1b_ConfigKey() )
    CleanupStack::PopAndDestroy( keys );
    
    // Try without a clean CenRep 
    MultimediaSharingSettings::SetEncoderConfigInfoL(
                                        KMusAvcBitrateLevel_1_1_ConfigKey() );   
    
    delete iLiveSession;
    iLiveSession = NULL;
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );
    
    ESTABLISH_OUT_SESSION( iLiveSession ) // calls StoreEncoderConfigInfoL
    // If StoreEncoderConfigInfoL leaves with KErrNoMemory,
    // MMusEngSessionObserver::SessionFailed will be called.
    if ( iLcSessionObserver->iCalledFunction == CLcSessionObserverStub::ESessionFailed )
        {
        User::Leave( KErrNoMemory );
        }
                     
    keys = MultimediaSharingSettings::EncoderConfigInfoLC();
    EUNIT_ASSERT_EQUALS( *keys, 
                         KMusAvcBitrateLevels_1_1_And_1b_ConfigKeys() )
    CleanupStack::PopAndDestroy( keys );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_StreamStreamingL()
    {
    // Wrong stream type, no callbacks to MLcSessionObserver
    CMceAudioStream* audioStream = CMceAudioStream::NewLC();
    iLiveSession->StreamStreaming( *audioStream );
    CleanupStack::PopAndDestroy( audioStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // Wrong state, no callbacks to MLcSessionObserver
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    videoStream->iState = CMceMediaStream::EIdle;
    iLiveSession->StreamStreaming( *videoStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
    
    // Receive session, RTP source and remote player present
    iLcSessionObserver->Reset();
    videoStream->iState = CMceMediaStream::EStreaming;
    CMceRtpSource* rtpSource = CMceRtpSource::NewLC();
    videoStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource );
    iReceiveSession->StreamStreaming( *videoStream );
    EUNIT_ASSERT_EQUALS( 
        TInt( CLcSessionObserverStub::EPlayerStateChanged ),
        iLcSessionObserver->iCalledFunction )
    EUNIT_ASSERT_EQUALS( 
        iReceiveSession->RemoteVideoPlayer(),
        iLcSessionObserver->iCurrentPlayer )
    
    // Live session, RTP sink and local player present
    iLcSessionObserver->Reset();
    videoStream->SetSourceL( NULL );
    CMceRtpSink* rtpSink = CMceRtpSink::NewLC();
    videoStream->AddSinkL( rtpSink );
    CleanupStack::Pop( rtpSink );
    iLiveSession->StreamStreaming( *videoStream );
    EUNIT_ASSERT_EQUALS( 
        TInt( CLcSessionObserverStub::EPlayerStateChanged ),
        iLcSessionObserver->iCalledFunction )
    EUNIT_ASSERT_EQUALS( 
        iLiveSession->LocalVideoPlayer(),
        iLcSessionObserver->iCurrentPlayer )

    // Live session, RTP sink present, but no local player
    iLcSessionObserver->Reset();
    delete iLiveSession->iLiveVideoPlayer;
    iLiveSession->iLiveVideoPlayer = NULL;
    iLiveSession->StreamStreaming( *videoStream );
    EUNIT_ASSERT( iLcSessionObserver->IsReseted() )
        
    CleanupStack::PopAndDestroy( videoStream );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_InformObserverAboutSessionStateChangeL()
    {
    // Observer does not exist
    iLiveSession->SetLcSessionObserver( NULL );
    iLiveSession->InformObserverAboutSessionStateChange();
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EUnknown, 
        iLcSessionObserver->iCalledFunction )
    
    // Observer exists
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->InformObserverAboutSessionStateChange();
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::ESessionStateChanged, 
        iLcSessionObserver->iCalledFunction )  
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_InformObserverAboutSessionUpdateL()
    {
    // Observer does not exist
    iLiveSession->SetLcSessionObserver( NULL );
    iLiveSession->InformObserverAboutSessionUpdate();
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EUnknown, 
        iLcSessionObserver->iCalledFunction )    
       
    // Observer exists
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->InformObserverAboutSessionUpdate();
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::ESessionUpdated, 
        iLcSessionObserver->iCalledFunction )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_InformObserverAboutSessionFailureL()
    {
    // Observer does not exist
    iLiveSession->SetLcSessionObserver( NULL );
    iLiveSession->InformObserverAboutSessionFailure( KErrGeneral );
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EUnknown, 
        iLcSessionObserver->iCalledFunction )
    EUNIT_ASSERT_EQUALS( KErrNone, iLcSessionObserver->iError )    
    
    // Observer exists
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->InformObserverAboutSessionFailure( KErrGeneral );
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::ESessionFailed, 
        iLcSessionObserver->iCalledFunction )
    EUNIT_ASSERT_EQUALS( KErrGeneral, iLcSessionObserver->iError )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_InformObserverAboutPlayerStateChangeL()
    {
    // Observer does not exist
    iLiveSession->SetLcSessionObserver( NULL );
    iLiveSession->InformObserverAboutPlayerStateChange( NULL );
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EUnknown, 
        iLcSessionObserver->iCalledFunction )    
    
    // Observer exists, but the player is NULL
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->InformObserverAboutPlayerStateChange( NULL );
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EUnknown, 
        iLcSessionObserver->iCalledFunction )    
    
    // Both observer and player exist
    iLiveSession->InformObserverAboutPlayerStateChange( 
        iLiveSession->LocalVideoPlayer() );
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EPlayerStateChanged, 
        iLcSessionObserver->iCalledFunction )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_InformObserverAboutPlayerUpdateL()
    {
    // Observer does not exist
    iLiveSession->SetLcSessionObserver( NULL );
    iLiveSession->InformObserverAboutPlayerUpdate( NULL );
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EUnknown, 
        iLcSessionObserver->iCalledFunction )    
    
    // Observer exists, but the player is NULL
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->InformObserverAboutPlayerUpdate( NULL );
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EUnknown, 
        iLcSessionObserver->iCalledFunction )    
    
    // Both observer and player exist
    iLiveSession->InformObserverAboutPlayerUpdate( 
        iLiveSession->LocalVideoPlayer() );
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EPlayerUpdated, 
        iLcSessionObserver->iCalledFunction )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_InformObserverAboutPlayerFailureL()
    {
    // Observer does not exist
    iLiveSession->SetLcSessionObserver( NULL );
    iLiveSession->InformObserverAboutPlayerFailure( NULL, KErrGeneral );
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EUnknown, 
        iLcSessionObserver->iCalledFunction )
    EUNIT_ASSERT_EQUALS( KErrNone, iLcSessionObserver->iError )
    
    // Observer exists, but the player is NULL
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->InformObserverAboutPlayerFailure( NULL, KErrGeneral );
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EUnknown, 
        iLcSessionObserver->iCalledFunction )
    EUNIT_ASSERT_EQUALS( KErrNone, iLcSessionObserver->iError )
    
    // Both observer and player exist
    iLiveSession->InformObserverAboutPlayerFailure( 
        iLiveSession->LocalVideoPlayer(), KErrGeneral );
    EUNIT_ASSERT_EQUALS( 
        CLcSessionObserverStub::EPlayerFailed, 
        iLcSessionObserver->iCalledFunction )
    EUNIT_ASSERT_EQUALS( KErrGeneral, iLcSessionObserver->iError )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_LcUiProviderL()
    {
    // Provider does not exist 
    iLiveSession->SetLcUiProvider( NULL );
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->LcUiProviderL(), KErrNotReady )
    
    // Provider exists
    iLiveSession->SetLcUiProvider( iLcUiProvider  );
    EUNIT_ASSERT( &iLiveSession->LcUiProviderL() == iLcUiProvider )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UI_IsBackgroundStartupL()
    {
    // fast mode is not defined
    EUNIT_ASSERT( !iLiveSession->IsBackgroundStartup() )
    
    // fast mode is Off
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KFastMode,
                                        1 ) );

    EUNIT_ASSERT( !iLiveSession->IsBackgroundStartup() )

    // fast mode is On
    // Bg value would be set to true at session contruct phase if property
    // is set to EFastModeOn at that time.
    iLiveSession->iBackground = ETrue;
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KFastMode,
                                        0 ) );

    EUNIT_ASSERT( iLiveSession->IsBackgroundStartup() )    
    
    // Ensure after session recreation fast mode is off 
    delete iLiveSession;
    iLiveSession = NULL;
    iLiveSession = CMusEngLiveSession::NewL();
    EUNIT_ASSERT( !iLiveSession->IsBackgroundStartup() ) 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_InformUiProviderAboutReceivingStartL()
    {
    // Observer does not exist
    iLiveSession->SetLcUiProvider( NULL );
    iLiveSession->InformUiProviderAboutReceivingStart();
    EUNIT_ASSERT_EQUALS( TInt( CLcUiProviderStub::EUnknown ), 
                         iLcUiProvider->iCalledFunction )
    
    // Observer exists, but we are on foreground (fast mode is off)
    iLiveSession->SetLcUiProvider( iLcUiProvider );
    iLiveSession->InformUiProviderAboutReceivingStart();
    EUNIT_ASSERT_EQUALS( TInt( CLcUiProviderStub::EUnknown ), 
                         iLcUiProvider->iCalledFunction )
    
    // Observer exists, we are on background
    iLiveSession->iBackground = ETrue;
    EUNIT_ASSERT( !iLcUiProvider->iForeground )
    iLiveSession->InformUiProviderAboutReceivingStart();
    EUNIT_ASSERT_EQUALS( TInt( CLcUiProviderStub::EHandleForegroundStatus ),
                         iLcUiProvider->iCalledFunction )
    EUNIT_ASSERT( iLcUiProvider->iForeground ) 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_SetForegroundStatusL()
    {
    iLiveSession->iBackground = ETrue;
    EUNIT_ASSERT( iLiveSession->SetForegroundStatus( ETrue ) == KErrNone );
    EUNIT_ASSERT( !iLiveSession->iBackground );
    EUNIT_ASSERT( iLiveSession->SetForegroundStatus( EFalse ) == KErrNone );
    EUNIT_ASSERT( !iLiveSession->iBackground ); // Still bg
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngMceSession::UT_UpdateLcSessionLL()
    {
    // No MCE session
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->UpdateLcSessionL(), KErrNotReady )
        
    ESTABLISH_OUT_SESSION( iLiveSession );
    
    iLiveSession->UpdateLcSessionL();
    
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngMceSession,
    "UT_CMusEngMceSesssion",
    "UNIT" )

EUNIT_TEST(
    "LcSessionState - test ",
    "CMusEngMceSession",
    "LcSessionState",
    "FUNCTIONALITY",
    SetupL, UT_LcSessionStateL, Teardown)
        
EUNIT_TEST(
    "RemoteVideoPlayer - test ",
    "CMusEngMceSession",
    "RemoteVideoPlayer",
    "FUNCTIONALITY",
    SetupL, UT_RemoteVideoPlayerL, Teardown)
        
EUNIT_TEST(
    "LocalVideoPlayer - test ",
    "CMusEngMceSession",
    "LocalVideoPlayer",
    "FUNCTIONALITY",
    SetupL, UT_LocalVideoPlayerL, Teardown) 
        
EUNIT_TEST(
    "LocalDisplayNameL - test ",
    "CMusEngMceSession",
    "LocalDisplayNameL",
    "FUNCTIONALITY",
    SetupL, UT_LocalDisplayNameL, Teardown)

EUNIT_TEST(
    "RemoteDisplayName - test ",
    "CMusEngMceSession",
    "RemoteDisplayName",
    "FUNCTIONALITY",
    SetupL, UT_RemoteDisplayNameL, Teardown)
    
EUNIT_TEST(
    "RemoteDetails - test ",
    "CMusEngMceSession",
    "RemoteDetails",
    "FUNCTIONALITY",
    SetupL, UT_RemoteDetailsL, Teardown)

EUNIT_TEST(
    "SetParameter - test ",
    "CMusEngMceSession",
    "SetParameter",
    "FUNCTIONALITY",
    SetupL, UT_SetParameterL, Teardown)
        
EUNIT_TEST(
    "ParameterValue - test ",
    "CMusEngMceSession",
    "ParameterValue",
    "FUNCTIONALITY",
    SetupL, UT_ParameterValueL, Teardown)    
        
EUNIT_TEST(
    "IsLcAudioMutedL - test ",
    "CMusEngMceSession",
    "IsLcAudioMutedL",
    "FUNCTIONALITY",
    SetupL, UT_IsLcAudioMutedL, Teardown)
        
EUNIT_TEST(
    "MuteLcAudioL - test ",
    "CMusEngMceSession",
    "MuteLcAudioL",
    "FUNCTIONALITY",
    SetupL, UT_MuteLcAudioL, Teardown)    

EUNIT_TEST(
    "MuteLcMicL - test ",
    "CMusEngMceSession",
    "MuteLcMicL",
    "FUNCTIONALITY",
    SetupL, UT_MuteLcMicL, Teardown)
        
EUNIT_TEST(
    "EnableLcLoudspeakerL - test ",
    "CMusEngMceSession",
    "EnableLcLoudspeakerL",
    "FUNCTIONALITY",
    SetupL, UT_EnableLcLoudspeakerL, Teardown)
        
EUNIT_TEST(
    "SetLcVolumeL - test ",
    "CMusEngMceSession",
    "SetLcVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_SetLcVolumeL, Teardown)

EUNIT_TEST(
    "GetSessionTime - test ",
    "CMusEngMceSession",
    "GetSessionTime",
    "FUNCTIONALITY",
    SetupL, UT_GetSessionTimeL, Teardown)   

EUNIT_TEST(
    "HandleTermination - test ",
    "CMusEngMceSession",
    "HandleTermination",
    "FUNCTIONALITY",
    SetupL, UT_HandleTerminationL, Teardown)   

EUNIT_TEST(
    "AdjustVideoCodecL - test ",
    "CMusEngMceSession",
    "AdjustVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustVideoCodecL, Teardown) 

EUNIT_TEST(
    "AdjustAudioCodecL - test ",
    "CMusEngMceSession",
    "AdjustAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustAudioCodecL, Teardown) 
    
EUNIT_TEST(
    "RectChangedL - test ",
    "CMusEngMceSession",
    "RectChangedL",
    "FUNCTIONALITY",
    SetupL, UT_RectChangedL, Teardown) 

EUNIT_TEST(
    "SetSessionSdpLinesL - test ",
    "CMusEngMceSession",
    "SetSessionSdpLinesL",
    "FUNCTIONALITY",
    SetupL, UT_SetSessionSdpLinesL, Teardown) 

EUNIT_TEST(
    "SetSessionSdpLinesL - operator variant test ",
    "CMusEngMceSession",
    "SetSessionSdpLinesL",
    "FUNCTIONALITY",
    SetupL, UT_SetSessionSdpLines_OperatorL, Teardown)
    
EUNIT_TEST(
    "SetMediaSdpLinesL - test ",
    "CMusEngMceSession",
    "SetMediaSdpLinesL",
    "FUNCTIONALITY",
    SetupL, UT_SetMediaSdpLinesL, Teardown) 

EUNIT_TEST(
    "SetMediaSdpLinesL - operator variant test ",
    "CMusEngMceSession",
    "SetMediaSdpLinesL",
    "FUNCTIONALITY",
    SetupL, UT_SetMediaSdpLines_OperatorL, Teardown) 
    
EUNIT_TEST(
    "AdjustStreamsAndCodecsL - test ",
    "CMusEngMceSession",
    "AdjustStreamsAndCodecsL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustStreamsAndCodecsL, Teardown)

EUNIT_TEST(
    "IncomingSession - test ",
    "CMusEngMceSession",
    "IncomingSession",
    "FUNCTIONALITY",
    SetupL, UT_IncomingSessionL, Teardown)          

EUNIT_TEST(
    "IncomingUpdate - test ",
    "CMusEngMceSession",
    "IncomingUpdate",
    "FUNCTIONALITY",
    SetupL, UT_IncomingUpdateL, Teardown)      

EUNIT_TEST(
    "StreamStateChangedL( source ) - test ",
    "CMusEngMceSession",
    "StreamStateChangedL( source )",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedWithSourceL, Teardown)          

EUNIT_TEST(
    "SessionStateChanged - test ",
    "CMusEngMceSession",
    "SessionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_SessionStateChangedL, Teardown)
    
EUNIT_TEST(
    "HandleSessionStateChanged - encoder key storing ",
    "CMusEngMceSession",
    "HandleSessionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_HandleSessionStateChanged_EncoderKeyStoringL, Teardown)
    
EUNIT_TEST(
    "SessionConnectionStateChanged - test ",
    "CMusEngMceSession",
    "SessionConnectionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_SessionConnectionStateChangedL, Teardown) 

EUNIT_TEST(
    "Failed - test ",
    "CMusEngMceSession",
    "Failed",
    "FUNCTIONALITY",
    SetupL, UT_FailedL, Teardown)     

EUNIT_TEST(
    "UpdateFailed - test ",
    "CMusEngMceSession",
    "UpdateFailed",
    "FUNCTIONALITY",
    SetupL, UT_UpdateFailedL, Teardown) 

EUNIT_TEST(
    "StreamStateChanged - test ",
    "CMusEngMceSession",
    "StreamStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedL, Teardown)             

EUNIT_TEST(
    "StreamStateChangedL (sink ) - test ",
    "CMusEngMceSession",
    "StreamStateChangedL( sink )",
    "FUNCTIONALITY",
    SetupL, UT_StreamStateChangedWithSinkL, Teardown) 

EUNIT_TEST(
    "SRReceived - test ",
    "CMusEngMceSession",
    "SRReceived",
    "FUNCTIONALITY",
    SetupL, UT_SRReceivedL, Teardown)     

EUNIT_TEST(
    "RRReceived - test ",
    "CMusEngMceSession",
    "RRReceived",
    "FUNCTIONALITY",
    SetupL, UT_RRReceivedL, Teardown) 

EUNIT_TEST(
    "SsrcAdded - test ",
    "CMusEngMceSession",
    "SsrcAdded",
    "FUNCTIONALITY",
    SetupL, UT_SsrcAddedL, Teardown) 

EUNIT_TEST(
    "SsrcRemoved - test ",
    "CMusEngMceSession",
    "SsrcRemoved",
    "FUNCTIONALITY",
    SetupL, UT_SsrcRemovedL, Teardown)         

EUNIT_TEST(
    "UpdateTimerEvent - test ",
    "CMusEngMceSession",
    "UpdateTimerEvent",
    "FUNCTIONALITY",
    SetupL, UT_UpdateTimerEventL, Teardown)              

EUNIT_TEST(
    "IsRoamingBetweenAPsAllowed - test ",
    "CMusEngMceSession",
    "IsRoamingBetweenAPsAllowed",
    "FUNCTIONALITY",
    SetupL, UT_IsRoamingBetweenAPsAllowedL, Teardown)

EUNIT_TEST(
    "SaveContactL - test ",
    "CMusEngMceSession",
    "SaveContactL",
    "FUNCTIONALITY",
    SetupL, UT_SaveContactL, Teardown)    

EUNIT_TEST(
    "Rect - test ",
    "CMusEngMceSession",
    "Rect",
    "FUNCTIONALITY",
    SetupL, UT_RectL, Teardown)

EUNIT_TEST(
    "SetRectL - test ",
    "CMusEngMceSession",
    "SetRectL",
    "FUNCTIONALITY",
    SetupL, UT_SetRectLL, Teardown)

EUNIT_TEST(
    "SecondaryRect - test ",
    "CMusEngMceSession",
    "SecondaryRect",
    "FUNCTIONALITY",
    SetupL, UT_SecondaryRectL, Teardown)

EUNIT_TEST(
    "SetSecondaryRectL - test ",
    "CMusEngMceSession",
    "SetSecondaryRectL",
    "FUNCTIONALITY",
    SetupL, UT_SetSecondaryRectLL, Teardown)

EUNIT_TEST(
    "EnableDisplayL - test ",
    "CMusEngMceSession",
    "EnableDisplayL",
    "FUNCTIONALITY",
    SetupL, UT_EnableDisplayLL, Teardown)
    
EUNIT_TEST(
    "SetOrientationL - test ",
    "CMusEngMceSession",
    "SetOrientationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngMceSession_SetOrientationLL, Teardown) 

EUNIT_TEST(
    "SetEncodingDeviceL - test ",
    "CMusEngMceSession",
    "SetEncodingDeviceL",
    "FUNCTIONALITY",
    SetupL, UT_SetEncodingDeviceLL, Teardown)   

EUNIT_TEST(
    "SetCodecConfigKeyL - test ",
    "CMusEngMceSession",
    "SetCodecConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_SetCodecConfigKeyLL, Teardown)    

EUNIT_TEST(
    "ReadCodecConfigKeyL - test ",
    "CMusEngMceSession",
    "ReadCodecConfigKeyL",
    "FUNCTIONALITY",
    SetupL, UT_ReadCodecConfigKeyLL, Teardown)        

EUNIT_TEST(
    "StoreEncoderConfigInfoL - test ",
    "CMusEngMceSession",
    "StoreEncoderConfigInfoL",
    "FUNCTIONALITY",
    SetupL, UT_StoreEncoderConfigInfoLL, Teardown)   

EUNIT_TEST(
    "StreamStreaming - test ",
    "CMusEngMceSession",
    "StreamStreaming",
    "FUNCTIONALITY",
    SetupL, UT_StreamStreamingL, Teardown)   
   
EUNIT_TEST(
    "InformObserverAboutSessionStateChange - test ",
    "CMusEngMceSession",
    "InformObserverAboutSessionStateChange",
    "FUNCTIONALITY",
    SetupL, UT_InformObserverAboutSessionStateChangeL, Teardown)

EUNIT_TEST(
    "InformObserverAboutSessionUpdate - test ",
    "CMusEngMceSession",
    "InformObserverAboutSessionUpdate",
    "FUNCTIONALITY",
    SetupL, UT_InformObserverAboutSessionUpdateL, Teardown)

EUNIT_TEST(
    "InformObserverAboutSessionFailure - test ",
    "CMusEngMceSession",
    "InformObserverAboutSessionFailure",
    "FUNCTIONALITY",
    SetupL, UT_InformObserverAboutSessionFailureL, Teardown)

EUNIT_TEST(
    "InformObserverAboutPlayerStateChange - test ",
    "CMusEngMceSession",
    "InformObserverAboutPlayerStateChange",
    "FUNCTIONALITY",
    SetupL, UT_InformObserverAboutPlayerStateChangeL, Teardown)

EUNIT_TEST(
    "InformObserverAboutPlayerUpdate - test ",
    "CMusEngMceSession",
    "InformObserverAboutPlayerUpdate",
    "FUNCTIONALITY",
    SetupL, UT_InformObserverAboutPlayerUpdateL, Teardown)

EUNIT_TEST(
    "InformObserverAboutPlayerFailure - test ",
    "CMusEngMceSession",
    "InformObserverAboutPlayerFailure",
    "FUNCTIONALITY",
    SetupL, UT_InformObserverAboutPlayerFailureL, Teardown)
    
EUNIT_TEST(
    "LcUiProviderL - test ",
    "CMusEngMceSession",
    "LcUiProviderL",
    "FUNCTIONALITY",
    SetupL, UT_LcUiProviderL, Teardown)

EUNIT_TEST(
    "IsBackgroundStartup - test ",
    "CMusEngMceSession",
    "IsBackgroundStartup",
    "FUNCTIONALITY",
    SetupL, UI_IsBackgroundStartupL, Teardown)

EUNIT_TEST(
    "InformUiProviderAboutReceivingStart - test ",
    "CMusEngMceSession",
    "InformUiProviderAboutReceivingStart",
    "FUNCTIONALITY",
    SetupL, UT_InformUiProviderAboutReceivingStartL, Teardown)

EUNIT_TEST(
    "SetForegroundStatus - test ",
    "CMusEngMceSession",
    "SetForegroundStatus",
    "FUNCTIONALITY",
    SetupL, UT_SetForegroundStatusL, Teardown)
    
EUNIT_TEST(
    "UpdateLcSessionL - test ",
    "CMusEngMceSession",
    "UpdateL",
    "FUNCTIONALITY",
    SetupL, UT_UpdateLcSessionLL, Teardown)

    
EUNIT_END_TEST_TABLE

//  END OF FILE


