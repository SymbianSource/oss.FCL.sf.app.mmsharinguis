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
#include "ut_musengoutsession.h"
#include "musengstubs.h"
#include "musengtestdefs.h"

#include "musenglivesession.h"
#include "musengclipsession.h"
#include "mussipprofilehandler.h"


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>

#include <mceoutsession.h>
#include <mcestreambundle.h>
#include <mceaudiostream.h>
#include <mcevideostream.h>
#include <mcespeakersink.h>
#include <mcedisplaysink.h>
#include <mceh263codec.h>
#include <mceavccodec.h>
#include <mceaudiocodec.h>

#include <sipprofile.h>
#include <sipstrings.h>

#include <audiopreference.h>
#include <uri8.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngOutSession* UT_CMusEngOutSession::NewL()
    {
    UT_CMusEngOutSession* self = UT_CMusEngOutSession::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngOutSession* UT_CMusEngOutSession::NewLC()
    {
    UT_CMusEngOutSession* self = new( ELeave ) UT_CMusEngOutSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngOutSession::~UT_CMusEngOutSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngOutSession::UT_CMusEngOutSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngOutSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngOutSession::SetupL()
    {
    iObserver = new( ELeave ) CMusEngObserverStub;
    iLiveSession = CMusEngLiveSession::NewL( KNullDesC(),
                                             TRect(0,0, 100,100),
                                             *iObserver,
                                             *iObserver,
                                             *iObserver );
    SIPStrings::OpenL();
    
    iLiveSession->iPrivate = EFalse;
    iLiveSession->iPrivateNumber = EFalse;
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngOutSession::Teardown()
    {
    SIPStrings::Close();
    delete iLiveSession;
    delete iObserver;
    }



// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngOutSession::UT_CMusEngOutSession_InviteLL()
    {

    // Initial state
    EUNIT_ASSERT( !iLiveSession->iSession );
    
    // Simulate failing session structure construction.
    iLiveSession->InviteL( KTestRecipientSipUri );
    
    iLiveSession->iSession->iState = CMceSession::EIdle;
    delete iLiveSession->iSession->Streams()[0];
    const RPointerArray<CMceMediaStream>& constStreams = 
                                            iLiveSession->iSession->Streams();
    const_cast<RPointerArray<CMceMediaStream>&>(constStreams)[0] = NULL;

    // Normal invite
    iLiveSession->InviteL( KTestRecipientSipUri );
    
    EUNIT_ASSERT( iLiveSession->iSession );
    // Next assertion ensures that session structure is new
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0] ); 
    EUNIT_ASSERT( *(iLiveSession->iRecipient) == KTestRecipientSipUri8() );
    EUNIT_ASSERT( iLiveSession->iSession->State() == CMceSession::EOffering );
    
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders );
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->Count() == 2 );
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 0 ) ==
                  KMusEngAcceptContactHeader() );
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 1 ) ==
                  KMusAcceptHeader() );    
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines );
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines->Count() == 1 );
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines->MdcaPoint( 0 ) ==
                  KMusEngSessionSdpLineXApplication() );
    
    // Normal operator invite
    delete iLiveSession;
    iLiveSession = NULL;
    iLiveSession = CMusEngLiveSession::NewL( KNullDesC,
                                             TRect(0,0, 100,100),
                                             *iObserver,
                                             *iObserver,
                                             *iObserver );
   	iLiveSession->iOperatorVariant = ETrue;
   	CSIPProfile* profile = iLiveSession->iSipProfileHandler->Profile();
    delete profile->iArray;
    profile->iArray = NULL;
    profile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
    profile->iArray->AppendL( KMusTestUri );

    iLiveSession->InviteL( KTestRecipientSipUri );
    
    EUNIT_ASSERT( iLiveSession->iSession );
    // Next assertion ensures that session structure is new
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0] ); 
    EUNIT_ASSERT( *(iLiveSession->iRecipient) == KTestRecipientSipUri8() );
    EUNIT_ASSERT( iLiveSession->iSession->State() == CMceSession::EOffering );
    
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders );
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->Count() == 3 );
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 0 ) ==
                  KMusEngAcceptContactHeader() );
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 1 ) ==
                  KMusAcceptHeader() );
    
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 2 ) ==
                  KMusPPreferredIdentityTestHeader() );
    
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines );
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines->Count() == 2 );
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines->MdcaPoint( 0 ) ==
                  KMusEngSessionSdpLineApplication() )
   	EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines->MdcaPoint( 1 ) ==
                  KMusEngSessionSdpLineType() )
                  
    EUNIT_ASSERT( iLiveSession->iSession->Streams().Count() > 0 );   
    TBool foundBandwidthLineFromMediaLevel( EFalse );
    for ( TInt i = 0; i <  iLiveSession->iSession->Streams().Count(); i++ )
        {
        if ( iLiveSession->iSession->Streams()[0]->Type() == KMceVideo )
            {
            if ( iLiveSession->iSession->Streams()[0]->iMediaSDPLines->MdcaCount() > 0 &&
                 iLiveSession->iSession->Streams()[0]->iMediaSDPLines->MdcaPoint( 0 ) ==
                    KMusEngSessionSdpLineBandwidthField() )
                {
                foundBandwidthLineFromMediaLevel = ETrue;
                }
            }
        }
    EUNIT_ASSERT( foundBandwidthLineFromMediaLevel );
                      
    // Try invite again, must fail
    TRAPD( error, iLiveSession->InviteL( KTestRecipientSipUri ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrAlreadyExists );
    
    // Simulate normal session ending, no need for observer call in this case
    iLiveSession->iSession->iState = CMceSession::ETerminated;
    
    // Try again. Establishment must be OK with new MceSession object
    iLiveSession->InviteL( KTestRecipientSipUri );
    EUNIT_ASSERT( iLiveSession->iSession->State() == CMceSession::EOffering );

      
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngOutSession::UT_CMusEngOutSession_CancelInviteLL()
    {
    // Try to cancel, must fail
    TRAPD( error, iLiveSession->CancelInviteL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Invite
    iLiveSession->InviteL( KTestRecipientSipUri );
    EUNIT_ASSERT( iLiveSession->iSession->State() == CMceSession::EOffering );
    
    // Cancel
    iLiveSession->CancelInviteL();
    EUNIT_ASSERT( iLiveSession->iSession->State() == CMceSession::ECancelling );
    
    // Try to cancel again, does nothing
    iLiveSession->CancelInviteL();
    EUNIT_ASSERT( iLiveSession->iSession->State() == CMceSession::ECancelling );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void UT_CMusEngOutSession::UT_CMusEngOutSession_EstablishSessionLL()
    {
    // Try to establish, must fail, because of missing session
    TRAPD( error, iLiveSession->EstablishSessionL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    iLiveSession->iPrivate = ETrue;
    iLiveSession->iPrivateNumber = EFalse;
    
    // Call to CMusEngOutMceSession::InviteL leads to call to EstablishL
    iLiveSession->InviteL( KTestRecipientSipUri );
    
    // Check that ports are correct
    
    for ( TInt i = 0; i < iLiveSession->iSession->Streams().Count(); ++i )
        {
        CMceMediaStream* stream = iLiveSession->iSession->Streams()[i];
        if ( stream->Type() == KMceAudio )
            {
            EUNIT_ASSERT( stream->iLocalMediaPort == 57344 )
            }
        else
            {
            EUNIT_ASSERT( stream->iLocalMediaPort == 49152 )
            }
        }
        
    // Check Accept-Contact -header
    
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders )
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->Count() == 2 )
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 0 ) ==
                  KMusEngAcceptContactHeader() )
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 1 ) ==
                  KMusAcceptHeader() );   
    EUNIT_ASSERT( iLiveSession->iSession->State() == CMceSession::EOffering );
    
    // Privacy test : Check Private -header & originator
    delete iLiveSession;
    iLiveSession = NULL;
    iLiveSession = CMusEngLiveSession::NewL( KNullDesC(),
                                                TRect(0,0, 100,100),
                                                *iObserver,
                                                *iObserver,
                                                *iObserver );
    iLiveSession->iPrivate = ETrue;
    iLiveSession->iPrivateNumber = ETrue;
    
    // Call to CMusEngOutMceSession::InviteL leads to call to EstablishL
    iLiveSession->InviteL( KTestRecipientSipUri );
    EUNIT_ASSERT( *iLiveSession->iSession->iOriginator == KMusAnonymousHeader );
    
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders )
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->Count() == 3 )
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 0 ) ==
                  KMusEngAcceptContactHeader() )
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 1 ) ==
                  KMusAcceptHeader() );   
    EUNIT_ASSERT( iLiveSession->iSession->iHeaders->MdcaPoint( 2 ).Find( KMusPrivacyHeader() ) == 0 );
    
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngOutSession::UT_CMusEngOutSession_HandleTerminationL()
    {

    // Try different values
    iLiveSession->HandleTermination( KSipStatusCode400BadRequest, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionBadRequestCalled );
    iObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode401Unauthorized, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionUnauthorizedCalled );
    iObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode402PaymentRequired, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionPaymentRequiredCalled );
    iObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode404RecipientNotFound, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionRecipientNotFoundCalled );
    iObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode416UnsupportedUriScheme, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionRecipientNotFoundCalled );
    iObserver->Reset();

    iLiveSession->HandleTermination( KSipStatusCode479NotAbleToProcessURI, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionRecipientNotFoundCalled );
    iObserver->Reset();
    
    iLiveSession->HandleTermination( 
                            KSipStatusCode407ProxyAuthenticationRequired, 
                            KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionProxyAuthenticationRequiredCalled );
    iObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode408ConnectionTimeOut, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionRequestTimeOutCalled );
    iObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode415UnsupportedMediaType, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionUnsupportedMediaTypeCalled );
    iObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode488NotAcceptableHere, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionUnsupportedMediaTypeCalled );
    iObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode606NotAcceptable, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionUnsupportedMediaTypeCalled );
    iObserver->Reset(); 
    
    iLiveSession->HandleTermination( KSipStatusCode486BusyHere, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionBusyHereCalled );
    iObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode487RequestCancelled, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionRequestCancelledCalled );
    iObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode603Decline, KNullDesC8()  );
    EUNIT_ASSERT( iObserver->iSessionRejectedCalled );
    iObserver->Reset();

    iLiveSession->HandleTermination( KSipStatusCode480TemporarilyNotAvailable, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iObserver->iSessionTemporarilyNotAvailable );
    iObserver->Reset();
    
    // Receive 486 with operator variant
    iLiveSession->iOperatorVariant = ETrue;
    iLiveSession->HandleTermination( KSipStatusCode486BusyHere, KNullDesC8()  );
    EUNIT_ASSERT( iObserver->iSessionRejectedCalled )
    EUNIT_ASSERT( !iObserver->iSessionBusyHereCalled )
    iObserver->Reset();
    iLiveSession->iOperatorVariant = EFalse;
    
    // Normal termination, let the base class handle
    iLiveSession->HandleTermination(  KSipStatusCode200OK, KNullDesC8()  );
    EUNIT_ASSERT( iObserver->iSessionTerminatedCalled ); // called by base class
    iObserver->Reset();

    }    


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//   
void UT_CMusEngOutSession::UT_CMusEngOutSession_AdjustVideoCodecLL()
    {
    //H263
    CMceH263Codec* codecH263 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );
    iLiveSession->CMusEngMceOutSession::AdjustVideoCodecL( *codecH263 );
    // Test payloadtype
    EUNIT_ASSERT( codecH263->iPayloadType == 96 )
    CleanupStack::PopAndDestroy( codecH263 );
    
    //H264
    CMceAvcCodec* codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    iLiveSession->CMusEngMceOutSession::AdjustVideoCodecL( *codecAvc );
    // Test payloadtype
    EUNIT_ASSERT( codecAvc->iPayloadType == 98 )
    CleanupStack::PopAndDestroy( codecAvc );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//   
void UT_CMusEngOutSession::UT_CMusEngOutSession_AdjustAudioCodecLL()
    {
    CMceAudioCodec* codec = 
                iLiveSession->iManager->SupportedAudioCodecs()[0]->CloneL();
    CleanupStack::PushL( codec );
    iLiveSession->CMusEngMceOutSession::AdjustAudioCodecL( *codec );
    
    // Test payloadtype
    
    EUNIT_ASSERT( codec->iPayloadType == 97 )
    
    // Test that base class has been called
    
    EUNIT_ASSERT( codec->iMMFPriority == KAudioPrioritySwisPlayback )
    EUNIT_ASSERT( codec->iMMFPriorityPreference == KAudioPrefSwisPlayback )
    
    CleanupStack::PopAndDestroy( codec );
    }
        

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//   
void UT_CMusEngOutSession::UT_CMusEngOutSession_CreateMceSessionStructureLL()
    {
    // Test removal of QoS-lines
    
    iLiveSession->iRecipient = KTestRecipientSipUri8().AllocL();
  
    // IETF profile
    iLiveSession->CreateMceSessionStructureL();
    TUint modifierValue = 500; //Just some value that is not used
    iLiveSession->iSession->GetModifierL( KMcePreconditions, modifierValue );
    EUNIT_ASSERT( modifierValue == KMcePreconditionsNotUsed );
    
    delete iLiveSession->iSession;
    iLiveSession->iSession = NULL;
    
    // IMS profile
    CSIPProfile* profile = iLiveSession->iSipProfileHandler->Profile();
    profile->iTypeInfo.iSIPProfileClass = TSIPProfileTypeInfo::EIms;
    iLiveSession->CreateMceSessionStructureL();
    modifierValue = 0;
    iLiveSession->iSession->GetModifierL( KMcePreconditions, modifierValue );
    EUNIT_ASSERT( modifierValue == KMcePreconditionsSupported );
    
    // IMS profile with force internet signaling
    
    // TODO: Stub MultimediaSharingSettings, set ForceInternetSignalingSettingL
    // to return EForceInternetSignaling and test again
    
    // Test bundling
    
    EUNIT_ASSERT( iLiveSession->iSession->Bundles().Count() == 0 )

    CMusEngClipSession* clipSession = CMusEngClipSession::NewL(
                                                    TRect(0, 100, 200, 300 ),
                                                    *iObserver,
                                                    *iObserver,
                                                    *iObserver );
    CleanupStack::PushL( clipSession );
    
    clipSession->SetClipL( KTestVideoFileName );
    
    clipSession->iRecipient = KTestRecipientSipUri8().AllocL();
    
    clipSession->CreateMceSessionStructureL();
    
    CMceSession* session = clipSession->iSession;
    
    EUNIT_ASSERT( session->Bundles().Count() == 1 )
    EUNIT_ASSERT( session->Bundles()[0]->Streams().Count() == 2 )
    EUNIT_ASSERT( session->Bundles()[0]->Streams()[0]->Type() == KMceAudio )
    EUNIT_ASSERT( session->Bundles()[0]->Streams()[0]->Sinks()[0]->Type() == 
                  KMceSpeakerSink )
    EUNIT_ASSERT( session->Bundles()[0]->Streams()[1]->Type() == KMceVideo )              
    EUNIT_ASSERT( session->Bundles()[0]->Streams()[1]->Sinks()[0]->Type() == 
                  KMceDisplaySink )

    CleanupStack::PopAndDestroy( clipSession );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngOutSession,
    "UT_CMusEngOutSesssion",
    "UNIT" )

EUNIT_TEST(
    "InviteL - test ",
    "CMusEngOutSession",
    "InviteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngOutSession_InviteLL, Teardown)

EUNIT_TEST(
    "CancelInviteL - test ",
    "CMusEngOutSession",
    "CancelInviteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngOutSession_CancelInviteLL, Teardown)

EUNIT_TEST(
    "EstablishSessionL - test ",
    "CMusEngOutSession",
    "EstablishSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngOutSession_EstablishSessionLL, Teardown)   

EUNIT_TEST(
    "HandleTermination - test ",
    "CMusEngOutSession",
    "HandleTermination",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngOutSession_HandleTerminationL, Teardown)   

EUNIT_TEST(
    "AdjustVideoCodecL - test ",
    "CMusEngOutSession",
    "AdjustVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngOutSession_AdjustVideoCodecLL, Teardown)   

EUNIT_TEST(
    "AdjustAudioCodecL - test ",
    "CMusEngOutSession",
    "AdjustAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngOutSession_AdjustAudioCodecLL, Teardown)   
    
EUNIT_TEST(
    "CreateMceSessionStructureL - test ",
    "CMusEngOutSession",
    "CreateMceSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngOutSession_CreateMceSessionStructureLL, Teardown)    

EUNIT_END_TEST_TABLE

//  END OF FILE


