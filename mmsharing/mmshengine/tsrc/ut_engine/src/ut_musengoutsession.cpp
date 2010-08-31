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
#include "sipprofile.h"
#include "musenglivesession.h"
#include "musengclipsession.h"
#include "mussipprofilehandler.h"
#include "mussessionproperties.h"
#include "contactenginestub.h"

//  SYSTEM INCLUDES
#include <lcsourcefilecontrol.h>
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
#include <mcecamerasource.h>


#include <sipstrings.h>

#include <audiopreference.h>
#include <uri8.h>

_LIT( KTestContactName, "nokia" );

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
    
    PropertyHelper::SetErrorCode( KErrNone );
    
    iLcSessionObserver = new( ELeave )CLcSessionObserverStub;
    iLcUiProvider = new( ELeave )CLcUiProviderStub;
    iAudioRoutingObserver = new( ELeave )CMusEngObserverStub;
    // Name is published using publish/subscribe key by Availblity
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KContactName,
                                        KTestContactName ) );
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    

    SIPStrings::OpenL();
    
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KRemoteSipAddress,
                                        KTestRecipientSipUri ) );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngOutSession::Teardown()
    {
    SIPStrings::Close();
    delete iLiveSession;
    delete iLcSessionObserver;
    delete iLcUiProvider;
    delete iAudioRoutingObserver;
    PropertyHelper::SetErrorCode( KErrNone );
    PropertyHelper::Close();
    }


// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngOutSession::UT_EstablishLcSessionL()
    {
    // Initial state
    EUNIT_ASSERT( !iLiveSession->iSession );
    
    ///////////////////////////////////////////////////////////////////////////
    // 1. Simulate failing session structure construction.
    iLiveSession->EstablishLcSessionL();
    
    iLiveSession->iSession->iState = CMceSession::EIdle;
    delete iLiveSession->iSession->Streams()[0];
    const RPointerArray<CMceMediaStream>& constStreams = 
                                            iLiveSession->iSession->Streams();
    const_cast<RPointerArray<CMceMediaStream>&>(constStreams)[0] = NULL;

    ///////////////////////////////////////////////////////////////////////////
    // 2. Normal invite
    iLiveSession->EstablishLcSessionL();
    
    EUNIT_ASSERT( iLiveSession->iSession );
    // Next assertion ensures that session structure is new
    EUNIT_ASSERT( iLiveSession->iSession->Streams().Count() > 0 );   
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
        
    // Ensure there is no SDP lines at media level
    MDesC8Array* mediaSdpLines = 
        iLiveSession->iSession->Streams()[0]->iMediaSDPLines;
    EUNIT_ASSERT( mediaSdpLines );
    EUNIT_ASSERT( mediaSdpLines->MdcaCount() == 0 );

    ///////////////////////////////////////////////////////////////////////////
    // 3. Normal operator invite
    delete iLiveSession;
    iLiveSession = NULL;
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
   	iLiveSession->iOperatorVariant = ETrue;
   	CSIPProfile* profile = iLiveSession->iSipProfileHandler->Profile();
    delete profile->iArray;
    profile->iArray = NULL;
    profile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
    profile->iArray->AppendL( KMusTestUri );

    iLiveSession->EstablishLcSessionL();
    
    EUNIT_ASSERT( iLiveSession->iSession );
    // Next assertion ensures that session structure is new
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
    
    // Ensure there is only a=type and a=application attributes (and no b=TIAS)
    // at session level for operator variant
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines );
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines->Count() == 2 );
    EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines->MdcaPoint( 0 ) ==
                  KMusEngSessionSdpLineApplication() )
   	EUNIT_ASSERT( iLiveSession->iSession->iSessionSDPLines->MdcaPoint( 1 ) ==
                  KMusEngSessionSdpLineType() )
                  
    EUNIT_ASSERT( iLiveSession->iSession->Streams().Count() > 0 );   
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0] ); 
    EUNIT_ASSERT( iLiveSession->iSession->Streams()[0]->Type() == KMceVideo );

    // Ensure there is only b=AS and no b=TIAS present at media level
    // for operator variant
    mediaSdpLines = iLiveSession->iSession->Streams()[0]->iMediaSDPLines;
    EUNIT_ASSERT( mediaSdpLines );
    EUNIT_ASSERT( mediaSdpLines->MdcaCount() == 1 );
    EUNIT_ASSERT( mediaSdpLines->MdcaPoint( 0 ) == 
                  KMusEngSessionSdpLineBandwidthField() );
    
    ///////////////////////////////////////////////////////////////////////////
    // 4. Try invite again, must fail
    TRAPD( error, iLiveSession->EstablishLcSessionL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrAlreadyExists );
    
    // 5. Simulate normal session ending, no need for observer call in this case
    iLiveSession->iSession->iState = CMceSession::ETerminated;
    
    // Try again. Establishment must be OK with new MceSession object
    iLiveSession->EstablishLcSessionL();
    EUNIT_ASSERT( iLiveSession->iSession->State() == CMceSession::EOffering );
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngOutSession::UT_EstablishLcSession_RecipientResolvingL()
    {   
    // Manual query from user, 
    // the user cancels the query (MLcUiProvider::Query returns EFalse)
    PropertyHelper::SetErrorCode( KErrNotFound );
    TRAPD( err, iLiveSession->EstablishLcSessionL() )
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT_EQUALS( KErrCancel, err )        
 
    // Manual query from user succeeds
    iLcSessionObserver->Reset();
    iLcUiProvider->Reset();        
    delete iLiveSession;
    iLiveSession = NULL;
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
    iLcUiProvider->iSimulatedReturnValue = ETrue;
    iLcUiProvider->iRecipient.Set( KTestRecipient2SipUri );
    PropertyHelper::SetErrorCode( KErrNotFound );
    iLiveSession->EstablishLcSessionL();
    EUNIT_ASSERT_EQUALS( 1, iLiveSession->iTriedInvitations )
    EUNIT_ASSERT_EQUALS( 
        TInt( CLcUiProviderStub::EInputRecipient ),
        iLcUiProvider->iCalledFunction )
    EUNIT_ASSERT_EQUALS( 
        KTestRecipient2SipUri8(), 
        *( iLiveSession->iRecipient ) )        
    EUNIT_ASSERT_EQUALS( KTestRecipient2SipUri(),iLiveSession->RemoteDisplayName() )  

    // Multiple resolved recipients
    iLcSessionObserver->Reset();
    iLcUiProvider->Reset();
    iLcUiProvider->iSimulatedReturnValue = ETrue;
    PropertyHelper::SetErrorCode( KErrNone );
    TBuf<200> multipleAddr;
    multipleAddr.Append( KTestRecipient2SipUri );
    multipleAddr.Append( _L(",") );
    multipleAddr.Append( KTestRecipientSipUri );
    
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KRemoteSipAddress,
                                        multipleAddr ) );
    delete iLiveSession;
    iLiveSession = NULL;
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
    iLiveSession->EstablishLcSessionL();
        
    EUNIT_ASSERT_EQUALS( 0, iLiveSession->iTriedInvitations )
    EUNIT_ASSERT_EQUALS( 
        TInt( CLcUiProviderStub::ESelectRecipient ),
        iLcUiProvider->iCalledFunction )
    // Stub selected first one from multiple addresses list
    EUNIT_ASSERT_EQUALS( 
        KTestRecipient2SipUri8(), 
        *( iLiveSession->iRecipient ) )    
       
    // Multiple resolved recipient, user doesn't select any
    iLcSessionObserver->Reset();
    iLcUiProvider->Reset();
    delete iLiveSession;
    iLiveSession = NULL;
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
    TRAP( err, iLiveSession->EstablishLcSessionL() );
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    EUNIT_ASSERT_EQUALS( KErrNotFound, err )   
    EUNIT_ASSERT_EQUALS( 0, iLiveSession->iTriedInvitations )
    EUNIT_ASSERT_EQUALS( 
        TInt( CLcUiProviderStub::ESelectRecipient ),
        iLcUiProvider->iCalledFunction )
    
    // Malformed multiple addresses, selection fails, manual entry is launched
    iLcSessionObserver->Reset();
    iLcUiProvider->Reset();
    iLcUiProvider->iSimulatedReturnValue = ETrue;
    iLcUiProvider->iRecipient.Set( KTestRecipient2SipUri );
    PropertyHelper::SetErrorCode( KErrNone );
    multipleAddr.Copy( _L(",") );
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KRemoteSipAddress,
                                        multipleAddr ) );
    delete iLiveSession;
    iLiveSession = NULL;
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
    TRAP( err, iLiveSession->EstablishLcSessionL() );
    if ( err == KErrNoMemory )
       {
       User::Leave( err );
       }
    EUNIT_ASSERT_EQUALS( 
        TInt( CLcUiProviderStub::EInputRecipient ),
        iLcUiProvider->iCalledFunction )    
    EUNIT_ASSERT_EQUALS( KErrNone, err );
    EUNIT_ASSERT_EQUALS( 1, iLiveSession->iTriedInvitations )
    EUNIT_ASSERT_EQUALS( 
        KTestRecipient2SipUri8(), 
        *( iLiveSession->iRecipient ) )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngOutSession::UT_EstablishLcSession_RegistrationPendingL()
    {
    // Set registration as pending
    iLiveSession->iSipProfileHandler->Profile()->iTBoolValue = EFalse;
    
    iLiveSession->EstablishLcSessionL();
    EUNIT_ASSERT( !iLiveSession->iSession )
    EUNIT_ASSERT( iLiveSession->iRegistrationPending )
    
    // Fake registration, session starts
    iLiveSession->iSipProfileHandler->Profile()->iTBoolValue = ETrue;
    iLiveSession->ProfileRegistered();
    
    if ( iLcSessionObserver->iCalledFunction == CLcSessionObserverStub::ESessionFailed )
        {
        // Session didn't start because of running out of memory
        User::Leave( KErrNoMemory );
        }
    
    iLiveSession->iDeltaTimer->Remove( iLiveSession->iInvitationResponseEntry );
    CMusEngMceOutSession::InvitationResponseTimerExpired( iLiveSession );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ENoAnswer ) )    
    iLcSessionObserver->Reset();
    
    EUNIT_ASSERT( iLiveSession->iSession )
    EUNIT_ASSERT_EQUALS( iLiveSession->iSession->State(), 
                         CMceSession::EOffering )
    EUNIT_ASSERT( !iLiveSession->iRegistrationPending )    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngOutSession::UT_TerminateLcSessionL()
    {
    // No MCE session
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
        iLiveSession->TerminateLcSessionL(), KErrNotReady )

    // Cancel a session that is in offering state
    iLiveSession->EstablishLcSessionL();
    EUNIT_ASSERT_EQUALS( TInt( CMceSession::EOffering ),
                         TInt( iLiveSession->iSession->iState ) )
    iLiveSession->TerminateLcSessionL();
    EUNIT_ASSERT_EQUALS( TInt( CMceSession::ECancelling ),
                         TInt( iLiveSession->iSession->iState ) )
    
    // Terminate an established session
    iLiveSession->iSession->iState = CMceSession::ETerminated;
    iLiveSession->EstablishLcSessionL();
    iLiveSession->iSession->iState = CMceSession::EEstablished;
    iLiveSession->TerminateLcSessionL();
    EUNIT_ASSERT_EQUALS( TInt( CMceSession::ETerminating ),
                         TInt( iLiveSession->iSession->iState ) )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void UT_CMusEngOutSession::UT_EstablishSessionLL()
    {
    // Try to establish, must fail, because of missing session
    TRAPD( error, iLiveSession->EstablishSessionL() );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Call to CMusEngOutMceSession::InviteL leads to call to EstablishL
    iLiveSession->EstablishLcSessionL();
    
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
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngOutSession::UT_HandleTerminationL()
    {
    // Try different values    
    iLiveSession->HandleTermination( KSipStatusCode408ConnectionTimeOut, 
                                     KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ENoAnswer ) )
    iLcSessionObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode415UnsupportedMediaType, 
                                     KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ESessionRejected ) ) 
    iLcSessionObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode488NotAcceptableHere, 
                                     KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ESessionRejected ) )
    iLcSessionObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode606NotAcceptable, 
                                     KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ESessionRejected ) )
    iLcSessionObserver->Reset(); 
    
    iLiveSession->HandleTermination( KSipStatusCode486BusyHere, 
                                     KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ERecipientBusy ) )
    iLcSessionObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode487RequestCancelled, 
                                     KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ESessionCancelled ) )
    iLcSessionObserver->Reset();
    
    iLiveSession->HandleTermination( KSipStatusCode603Decline, KNullDesC8()  );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ESessionRejected ) )
    iLcSessionObserver->Reset();

    iLiveSession->HandleTermination( KSipStatusCode480TemporarilyNotAvailable, 
                                     KNullDesC8() );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ERecipientTemporarilyNotAvailable ) )
    iLcSessionObserver->Reset();
    
    // Receive 486 with operator variant
    iLiveSession->iOperatorVariant = ETrue;
    iLiveSession->HandleTermination( KSipStatusCode486BusyHere, KNullDesC8()  );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ESessionRejected ) )    
    iLcSessionObserver->Reset();
    iLiveSession->iOperatorVariant = EFalse;
    
    // Normal termination, let the base class handle
    iLiveSession->HandleTermination(  KSipStatusCode200OK, KNullDesC8()  );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionStateChanged ) )
    iLcSessionObserver->Reset();
    }    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
void UT_CMusEngOutSession::UT_HandleRecipientNotFoundTerminationL()
    {
    EUNIT_ASSERT_EQUALS( iLiveSession->iTriedInvitations, 0 );
    
    // Retry possible when 404 is received but fails because of recipient 
    // proposal is missing for some reason, recipient not found callback
    // is called instead of retry
    delete iLiveSession->iRemoteSipAddressProposal;
    iLiveSession->iRemoteSipAddressProposal = NULL;
    iLcSessionObserver->Reset();
    iLiveSession->HandleTermination( KSipStatusCode404RecipientNotFound, 
                                     KNullDesC8() );
    
    EUNIT_ASSERT( iLiveSession->iDeltaTimer->IsActive() == ETrue )
    iLiveSession->AsyncBrakeCompleted( iLiveSession );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ERecipientNotFound ) )
    
    // Retry possible when 404 is received
    delete iLiveSession->iRemoteSipAddressProposal;
    iLiveSession->iRemoteSipAddressProposal = NULL;
    iLiveSession->iRemoteSipAddressProposal = KTestRecipient2SipUri().AllocL();
    iLcSessionObserver->Reset();
    iLcUiProvider->iRecipient.Set( KTestRecipient2SipUri );
    iLcUiProvider->iSimulatedReturnValue = ETrue;
    iLiveSession->HandleTermination( KSipStatusCode404RecipientNotFound, 
                                     KNullDesC8() );
       
    EUNIT_ASSERT( iLiveSession->iDeltaTimer->IsActive() == ETrue );
    iLiveSession->AsyncBrakeCompleted( iLiveSession );

    // Memory running out might cause that retry is not done
    if ( iLcSessionObserver->iCalledFunction == CLcSessionObserverStub::ESessionFailed )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT_EQUALS( 1, iLiveSession->iTriedInvitations )    
    EUNIT_ASSERT_EQUALS( 
        KTestRecipient2SipUri8(), 
        *( iLiveSession->iRecipient ) )
    
    // Retry not anymore possible when 404 received (e.g. manual address query
    // was done for invite).
    iLcSessionObserver->Reset();
    iLiveSession->HandleTermination( KSipStatusCode404RecipientNotFound, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iLiveSession->iDeltaTimer->IsActive() == ETrue )
    iLiveSession->AsyncBrakeCompleted( iLiveSession );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ERecipientNotFound ) )
    
    // 416 has identical handling
    iLcSessionObserver->Reset();
    iLiveSession->HandleTermination( KSipStatusCode416UnsupportedUriScheme, 
                                     KNullDesC8() );
    EUNIT_ASSERT( iLiveSession->iDeltaTimer->IsActive() == ETrue );
    iLiveSession->AsyncBrakeCompleted( iLiveSession );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ERecipientNotFound ) )
   
    // 479 has identical handling
    iLcSessionObserver->Reset();
    iLiveSession->HandleTermination( KSipStatusCode479NotAbleToProcessURI, 
                                    KNullDesC8() );
    EUNIT_ASSERT( iLiveSession->iDeltaTimer->IsActive() == ETrue );
    iLiveSession->AsyncBrakeCompleted( iLiveSession );
    EUNIT_ASSERT_EQUALS( TInt( iLcSessionObserver->iCalledFunction ),
                         TInt( CLcSessionObserverStub::ESessionFailed ) )
    EUNIT_ASSERT_EQUALS( iLcSessionObserver->iError,
                         TInt( MLcSession::ERecipientNotFound ) )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//   
void UT_CMusEngOutSession::UT_AdjustVideoCodecLL()
    {
    //H263
    CMceH263Codec* codecH263 = CMceH263Codec::NewLC( KMceSDPNameH2632000() );
    iLiveSession->CMusEngMceOutSession::AdjustVideoCodecL( *codecH263,
                                                           KMceCameraSource );
    // Test payloadtype
    EUNIT_ASSERT( codecH263->iPayloadType == 96 )
    CleanupStack::PopAndDestroy( codecH263 );
    
    //H264
    CMceAvcCodec* codecAvc = CMceAvcCodec::NewLC( KMceSDPNameH264() );
    iLiveSession->CMusEngMceOutSession::AdjustVideoCodecL( *codecAvc,
                                                           KMceCameraSource  );
    // Test payloadtype
    EUNIT_ASSERT( codecAvc->iPayloadType == 98 )
    CleanupStack::PopAndDestroy( codecAvc );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//   
void UT_CMusEngOutSession::UT_AdjustAudioCodecLL()
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
void UT_CMusEngOutSession::UT_CreateMceSessionStructureLL()
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

    CMusEngClipSession* clipSession = CMusEngClipSession::NewL();
    CleanupStack::PushL( clipSession );
    clipSession->SetLcSessionObserver( iLcSessionObserver );
    clipSession->SetLcUiProvider( iLcUiProvider );    
    clipSession->LocalVideoPlayer()->LcSourceFileControl()->SetLcFileNameL(
        KTestVideoFileName() );
    
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

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//   
void UT_CMusEngOutSession::UT_ConstructLL()
    {
    TUint32 profileid = 0;
    CSIPProfile* profile = iLiveSession->iSipProfileHandler->Profile();
    profile->GetParameter( KSIPProfileId, profileid );
    //stub creates defaultprofile if profileid is zero
    //stub does not care if the profileid is given or not
    //profileid of new profile is always 1
    EUNIT_ASSERT(profile->iIsDefaultProfile)

    TInt error = NULL;
    const TUint KSipProfileId2( 2 );
    TRAP( error, RProperty::Set( NMusSessionApi::KCategoryUid,
                                 NMusSessionApi::KSipProfileId,
                                 KSipProfileId2 ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    profile = NULL;
    profileid = 0;
    
    SIPStrings::Close();
    delete iLcSessionObserver;
    iLcSessionObserver = NULL;
    delete iLiveSession;
    iLiveSession = NULL;
        
    iLcSessionObserver = new( ELeave )CLcSessionObserverStub;
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
    
    SIPStrings::OpenL();
    
    profile = iLiveSession->iSipProfileHandler->Profile();
    profile->GetParameter( KSIPProfileId, profileid );
    EUNIT_ASSERT( !profile->iIsDefaultProfile )
    
    
    TRAP( error, RProperty::Delete( NMusSessionApi::KCategoryUid,
                                     NMusSessionApi::KSipProfileId ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    profile = NULL;
    profileid = 0;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusEngOutSession::UT_ContactSavingLL()
    {
    ContactEngineStubHelper::Reset();
    
    // Saving of contact is done at destruction phase only if recipient
    // has been queried from client
    //
    CMusEngLiveSession* liveSession = CMusEngLiveSession::NewL();
    liveSession->SetLcSessionObserver( iLcSessionObserver );
    liveSession->SetLcUiProvider( iLcUiProvider );    
    delete liveSession;
    liveSession = NULL;
    EUNIT_ASSERT( ContactEngineStubHelper::GetCalledFunction() == EContactEngineStubNone );
    
    
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KContactId,
                                        2 ) );
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KTelNumber,
                                        _L("12341234") ) );
    
    liveSession = CMusEngLiveSession::NewL();
    CleanupStack::PushL( liveSession );
    liveSession->SetLcSessionObserver( iLcSessionObserver );
    liveSession->SetLcUiProvider( iLcUiProvider );    
    
    delete liveSession->iRecipient;
    liveSession->iRecipient = NULL;
    liveSession->iRecipient = _L8("sip:yep@10.10.10.10").AllocL();
    liveSession->iAddressQueried = ETrue;
    CleanupStack::PopAndDestroy( liveSession );
    if ( ContactEngineStubHelper::GetCalledFunction() != EContactEngineStubSetText )
        {
        // out-of-memory was trap ignored and saving failed because of that
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( ContactEngineStubHelper::GetCalledFunction() == EContactEngineStubSetText );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void UT_CMusEngOutSession::UT_RemoteAddressLL()
    {
    delete iLiveSession->iRecipient;
    iLiveSession->iRecipient = NULL;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iLiveSession->RemoteAddressL(), KErrNotReady )
    
    iLiveSession->iRecipient = KTestRecipient2SipUri8().AllocL();
    
    HBufC* remoteAddr = iLiveSession->RemoteAddressL();
    CleanupStack::PushL( remoteAddr );
    EUNIT_ASSERT_EQUALS( KTestRecipient2SipUri(), *remoteAddr )
    CleanupStack::PopAndDestroy( remoteAddr );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusEngOutSession::UT_RemoteDisplayNameL()
    {
    // Test 1 : Default setting, contact name set
    EUNIT_ASSERT_EQUALS ( KTestContactName(), iLiveSession->RemoteDisplayName() )    
    
    // Test 2 : Contact name set to null descriptor
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                       NMusSessionApi::KContactName,
                                       KNullDesC) );
    delete iLiveSession;
    iLiveSession = NULL;
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
    EUNIT_ASSERT( iLiveSession->RemoteDisplayName().Length() == 0 );
    
    // Test 3 : Manual address entered
    PropertyHelper::SetErrorCode( KErrNone );
    delete iLiveSession;
    iLiveSession = NULL;
    iLcSessionObserver->Reset();
    iLcUiProvider->Reset();
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KContactName,
                                        KTestContactName ) );    
    iLcUiProvider->iSimulatedReturnValue = ETrue;
    iLcUiProvider->iRecipient.Set( KTestRecipientSipUri );
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
    PropertyHelper::SetErrorCode( KErrNotFound );
    iLiveSession->EstablishLcSessionL();    
    EUNIT_ASSERT_EQUALS( 1, iLiveSession->iTriedInvitations )
    EUNIT_ASSERT_EQUALS( KTestRecipientSipUri(),iLiveSession->RemoteDisplayName() )
    
    // Test 4 : Contact name has zero lenth and multiple address entry query. 
    //          Displayname should have user selected address.
    //          Rare scenario.
    PropertyHelper::SetErrorCode( KErrNone );
    iLcSessionObserver->Reset();
    iLcUiProvider->Reset();
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KContactName,
                                        KNullDesC) );
    delete iLiveSession;
    iLiveSession = NULL;    
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
    iLcUiProvider->iSimulatedReturnValue = ETrue;    
    PropertyHelper::SetErrorCode( KErrNone );
    TBuf<200> multipleAddr;
    multipleAddr.Append( KTestRecipientTelUri );
    multipleAddr.Append( _L(",") );
    multipleAddr.Append( KTestRecipientSipUri );
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KRemoteSipAddress,
                                        multipleAddr ) );
    iLiveSession->EstablishLcSessionL();    
    /* Stub selects the first one automatically */
    EUNIT_ASSERT_EQUALS( KTestRecipientTelUri(),iLiveSession->RemoteDisplayName() );
    
    // Test 5 : Contact name has zero lenth and no manual entry queried. 
    //          Recipient has only teluri. So displayname should have tel uri address.
    PropertyHelper::SetErrorCode( KErrNone );
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KContactName,
                                        KNullDesC) );
    delete iLiveSession;
    iLiveSession = NULL;    
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
    iLcUiProvider->iSimulatedReturnValue = ETrue;
    TBuf<200> singleAddr;
    singleAddr.Append( KTestRecipientTelUri );    
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KRemoteSipAddress,
                                        singleAddr ) );
    PropertyHelper::SetErrorCode( KErrNone );
    iLiveSession->EstablishLcSessionL();
    EUNIT_ASSERT_EQUALS( KTestRecipientTelUri(),iLiveSession->RemoteDisplayName() );
    
    // Test 6 : Contact name has zero lenth and recipient has teluri 
    //          and invitation fails to teluri.
    //          Manual address query entered and now display should have entered 
    //          manual address.
    PropertyHelper::SetErrorCode( KErrNone );
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                           NMusSessionApi::KContactName,
                                           KNullDesC) );
    delete iLiveSession;
    iLiveSession = NULL;    
    iLiveSession = CMusEngLiveSession::NewL();
    iLiveSession->SetLcSessionObserver( iLcSessionObserver );
    iLiveSession->SetLcUiProvider( iLcUiProvider );    
    iLcUiProvider->iRecipient.Set( KTestRecipientSipUri );
    iLcUiProvider->iSimulatedReturnValue = ETrue;
    singleAddr.Copy( KTestRecipientTelUri );
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid,
                                        NMusSessionApi::KRemoteSipAddress,
                                        singleAddr ) );
    PropertyHelper::SetErrorCode( KErrNotFound );
    iLiveSession->EstablishLcSessionL(); 
    EUNIT_ASSERT_EQUALS( 1, iLiveSession->iTriedInvitations )
    EUNIT_ASSERT_EQUALS( KTestRecipientSipUri(),iLiveSession->RemoteDisplayName() );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngOutSession,
    "UT_CMusEngOutSesssion",
    "UNIT" )

EUNIT_TEST(
    "EstablishLcSessionL - test ",
    "CMusEngOutSession",
    "EstablishLcSessionL",
    "FUNCTIONALITY",
    SetupL, UT_EstablishLcSessionL, Teardown)

EUNIT_TEST(
    "TerminateLcSessionL - test ",
    "CMusEngOutSession",
    "TerminateLcSessionL",
    "FUNCTIONALITY",
    SetupL, UT_TerminateLcSessionL, Teardown)    
    
EUNIT_TEST(
    "EstablishLcSessionL recipient resolving - test ",
    "CMusEngOutSession",
    "EstablishLcSessionL",
    "FUNCTIONALITY",
    SetupL, UT_EstablishLcSession_RecipientResolvingL, Teardown)

EUNIT_TEST(
    "EstablishLcSessionL registration pending - test ",
    "CMusEngOutSession",
    "EstablishLcSessionL",
    "FUNCTIONALITY",
    SetupL, UT_EstablishLcSession_RegistrationPendingL, Teardown)

EUNIT_TEST(
    "EstablishSessionL - test ",
    "CMusEngOutSession",
    "EstablishSessionL",
    "FUNCTIONALITY",
    SetupL, UT_EstablishSessionLL, Teardown)   

EUNIT_TEST(
    "HandleTermination - test ",
    "CMusEngOutSession",
    "HandleTermination",
    "FUNCTIONALITY",
    SetupL, UT_HandleTerminationL, Teardown)   

EUNIT_TEST(
    "HandleTermination recipient not found - test ",
    "CMusEngOutSession",
    "HandleTermination recipient not found",
    "FUNCTIONALITY",
    SetupL, UT_HandleRecipientNotFoundTerminationL, Teardown)  
    
EUNIT_TEST(
    "AdjustVideoCodecL - test ",
    "CMusEngOutSession",
    "AdjustVideoCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustVideoCodecLL, Teardown)   

EUNIT_TEST(
    "AdjustAudioCodecL - test ",
    "CMusEngOutSession",
    "AdjustAudioCodecL",
    "FUNCTIONALITY",
    SetupL, UT_AdjustAudioCodecLL, Teardown)   
    
EUNIT_TEST(
    "CreateMceSessionStructureL - test ",
    "CMusEngOutSession",
    "CreateMceSessionStructureL",
    "FUNCTIONALITY",
    SetupL, UT_CreateMceSessionStructureLL, Teardown)    
  
EUNIT_TEST(
    "ConstructLL - test ",
    "CMusEngOutSession",
    "ConstructLL",
    "FUNCTIONALITY",
    SetupL, UT_ConstructLL, Teardown) 

EUNIT_TEST(
    "ContactSavingL - test ",
    "CMusEngOutSession",
    "ContactSavingL",
    "FUNCTIONALITY",
    SetupL, UT_ContactSavingLL, Teardown) 

EUNIT_TEST(
    "RemoteAddressLL - test ",
    "CMusEngOutSession",
    "RemoteAddressLL",
    "FUNCTIONALITY",
    SetupL, UT_RemoteAddressLL, Teardown)

EUNIT_TEST(
    "RemoteDisplayName - test ",
    "CMusEngOutSession",
    "RemoteDisplayName",
    "FUNCTIONALITY",
    SetupL, UT_RemoteDisplayNameL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE


