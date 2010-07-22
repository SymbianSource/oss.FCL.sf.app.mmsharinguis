/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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


//  CLASS HEADER
#include "UT_CMusAvaCapabilityQuery.h"
#include "musavasettingsimp.h"
#include "musavaobserverimp.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "muslogger.h"
#include "mussesseioninformationapi.h"

//  EXTERNAL INCLUDES
#include <EscapeUtils.h>
#include <digia/eunit/eunitmacros.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <stringpool.h>
#include <sipservertransaction.h>
#include <siptransactionbase.h>
//  INTERNAL INCLUDES
#include "musavaoptionhandler.h"
#include "musavacapabilitytesthelper.h"
#include "CSipSseTestTls.h"
#include "musavaCapabilityExchange.h"
#include "musavacapabilitysipagent.h"
#include "MusAvaCapabilityQuery.h"
#include "musavaterminal.h"
#include "sipprofile.h"
#include "musavasharedobject.h"
#include "mussettingskeys.h"
#include "mussettings.h"
#include "musavasip.h"
#include "musavacapabilityqueryobserverimp.h"


// CONSTRUCTION
UT_CMusAvaCapabilityQuery* UT_CMusAvaCapabilityQuery::NewL()
    {
    UT_CMusAvaCapabilityQuery* self = UT_CMusAvaCapabilityQuery::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaCapabilityQuery* UT_CMusAvaCapabilityQuery::NewLC()
    {
    UT_CMusAvaCapabilityQuery* self = new( ELeave ) UT_CMusAvaCapabilityQuery();
    CleanupStack::PushL( self );
	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaCapabilityQuery::~UT_CMusAvaCapabilityQuery()
    {
    }

// Default constructor
UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery()
    {
    }

// Second phase construct
void UT_CMusAvaCapabilityQuery::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMusAvaCapabilityQuery::SetupL(  )
    {
    iSharedObject = CMusAvaSharedObject::GetSingletonL();
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iOptionHandler = CMusAvaOptionHandler::NewL( *iAvailabilityObserver,
                                                 *iConcreteSettings );
    iExchange = iOptionHandler->iCapabilityExchange;
    iSipAgent = CMusAvaCapabilitySipAgent::NewL( *iExchange, 
                                                *iOptionHandler->iSharedObj,
                                                iOptionHandler->iObserver );
    
    iCapability = iOptionHandler->iSwisCapability;

    MSIPConnectionObserver& connectionObserver = 
                            iSharedObject->MusAvaSip().ConnectionObserver();
    
    CSIP& sip = iSharedObject->MusAvaSip().Sip();
    iSharedObject->MusAvaSip().CreateProfileL();    
    iProfile = iSharedObject->MusAvaSip().Profile();
    if ( !iProfile->iArray )
        {
        iProfile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
        iProfile->iArray->AppendL( _L8("sip:user@domain.com") );
        }
    if ( !iProfile->iRegisteredContact )
        {
        iProfile->iRegisteredContact = KRegisteredContact().AllocL();
        }
    
    TUint32 iapId;
    TInt err = iProfile->GetParameter( KSIPAccessPointId, iapId );
    iSIPConnection = CSIPConnection::NewL( sip, 
                                           iapId,
                                           connectionObserver );

    iQuery = CMusAvaCapabilityQuery::NewL( *iCapability,
                                            *iSIPConnection,
                                            *iProfile,
                                            KCapabilityTestSIPAddressUri_B );

    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
    iStorage->Set( MusSettingsKeys::KFastStartupMode, 
        MusSettingsKeys::EFastModeOff );
    iOpVariantSetting = MultimediaSharingSettings::OperatorVariantSettingL();
    } 

void UT_CMusAvaCapabilityQuery::Teardown(  )
    {
    delete HelpFlags::iTestSipAddress;
    HelpFlags::iTestSipAddress = NULL;
    delete iQuery;
    CMusAvaSharedObject::DeleteSingleton();
    if ( iStorage )
        {
        iStorage->Clear();
        CSipSseTestTls::Close();
        iStorage = NULL;
        }

    delete iSipAgent;
    delete iOptionHandler;
    delete iConcreteSettings;
    delete iAvailabilityObserver;
    delete iSIPConnection;
    PropertyHelper::Close();
    }

void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_NewLL(  )
    {    
    // 1.basic test and iQuery is constructed already. Check it
    EUNIT_ASSERT( iQuery );

    // 2.test for sipaddress is empty,should leave
    delete iQuery;
    iQuery = NULL; // clean the previous one
    TRAPD( error, iQuery = CMusAvaCapabilityQuery::NewL( *iCapability,
                                              *iSIPConnection,
                                              *iProfile,
                                              KNullDesC ) );
    EUNIT_ASSERT( error == KErrNoMemory || error == KErrArgument );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !iQuery );   // iQuery Should be empty.

    // 3.Contact user part tests, getting KSIPRegisteredContact leaves
    delete iProfile->iRegisteredContact;
    iProfile->iRegisteredContact = NULL;    
    TRAP( error, iQuery = CMusAvaCapabilityQuery::NewL( *iCapability,
                                            *iSIPConnection,
                                            *iProfile,
                                            KCapabilityTestSIPAddressUri_B ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNotFound );    
    EUNIT_ASSERT( !iQuery );   // iQuery Should be empty. 

    // 4.Contact user part tests, CSIPContactHeader::DecodeL has less than 1.
    iProfile->iRegisteredContact = KRegisteredContact().AllocL();
    HelpFlags::iEmptyContactHeader = ETrue;
    TRAP( error, iQuery = CMusAvaCapabilityQuery::NewL( *iCapability,
                                            *iSIPConnection,
                                            *iProfile,
                                            KCapabilityTestSIPAddressUri_B ) );
    HelpFlags::iEmptyContactHeader = EFalse;
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNotFound );    
    EUNIT_ASSERT( !iQuery );   // iQuery Should be empty. 

    // 5.Contact user part tests ,CSIPContactHeader::DecodeL has 1 contact
    delete iQuery;
    iQuery = NULL; // clean the previous one    
    delete iProfile->iRegisteredContact;
    iProfile->iRegisteredContact = NULL;
    iProfile->iRegisteredContact = KRegisteredContact().AllocL();    
    
    HelpFlags::iTestSipAddress = CSIPAddress::DecodeL( KRegisteredContact ); 
    HelpFlags::iUseTestSipAddress = ETrue;
    
    TRAP( error, iQuery = CMusAvaCapabilityQuery::NewL( *iCapability,
                                            *iSIPConnection,
                                            *iProfile,
                                            KCapabilityTestSIPAddressUri_B ) );             
    HelpFlags::iUseTestSipAddress = EFalse;
    if ( error == KErrNoMemory ) User::Leave( error );
    delete HelpFlags::iTestSipAddress;
    HelpFlags::iTestSipAddress = NULL;
    
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( iQuery );

    // 6.Contact user part tests ,CSIPContactHeader::DecodeL has 1 contact but
    // sip address empty
    delete iQuery;
    iQuery = NULL; // clean the previous one

    delete iProfile->iRegisteredContact;
    iProfile->iRegisteredContact = NULL;
    iProfile->iRegisteredContact = KRegisteredContact().AllocL();

    HelpFlags::iUseTestSipAddress = ETrue;
    TRAP(error,iQuery = CMusAvaCapabilityQuery::NewL( *iCapability,
                                            *iSIPConnection,
                                            *iProfile,
                                            KCapabilityTestSIPAddressUri_B ));   
    HelpFlags::iUseTestSipAddress = EFalse;
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNotFound );
    EUNIT_ASSERT( !iQuery ); // iQuery Should be empty. 
    
    }

void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_ExecuteLL(  )
    {
    /* includes testing of functions:
     * Prepare(),
     * DoCompletedL() case KMUSAVASIP200
     * DoCompleted200OKL() 
     * ValidateCodecL()
     * ValidateAttributesL()
     * ValidateContactL(),
     * Retrying()
     * Capability()
     * SetState()
     * Terminal()
     * ExecuteL()
     * CompletedL()
     * State()
     * Result()
     * AddAcceptContactHeaderL()
     * AddContactHeaderL()
     * ResetAndDestroyTrx()
     * LocalAddress()
     */
    
    EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
        
    CMusAvaTerminal& terminal = iQuery->Terminal();
    EUNIT_ASSERT( terminal.iQueries == 1 );
    EUNIT_ASSERT( terminal.iQuery == NULL );
    
    EUNIT_ASSERT( iQuery->Terminal().Uri().Compare( KCapabilityTestSIPAddressUri8_B ) == 0 );
    
    iQuery->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryPrepared );
    TRAPD( error, iQuery->ExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error != KErrNone );

    iQuery->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    iQuery->ExecuteL( );
    
    EUNIT_ASSERT( iQuery->State() ==  
                    CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );
    EUNIT_ASSERT( !iQuery->iTimer );
    EUNIT_ASSERT( iQuery->iTrx );
        
    EUNIT_ASSERT( iStorage->iClientRequest->Method() == 
                            SIPStrings::StringF( SipStrConsts::EOptions ) );
    EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().Content().Length() 
                != 0 );
    _LIT8(contentType,"application/sdp");            
    EUNIT_ASSERT(
        iStorage->iClientRequest->MessageElements().ContentType() );
    EUNIT_ASSERT( 
            iStorage->iClientRequest->MessageElements().UserHeaders().Count() 
                == 6 );
    MUS_HEADER_ASSERT( iStorage->iClientRequest, 0, 
                    KCapabilityTestAcceptContactHeader );
    MUS_HEADER_ASSERT( iStorage->iClientRequest, 5, 
                    KCapabilityTestContactHeader_A1Updated );
    MUS_HEADER_ASSERT( iStorage->iClientRequest, 3, 
                    KCapabilityTestAcceptHeader );
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaStatusNotExecuted );

    // set response 200 OK FT
    CapabilityTestHelper::SetResponseL( *iQuery->iTrx, KMUSAVASIP200, 
                                        SipStrConsts::EPhraseOk,
                                        KCapabilityTestAddress_B,
                                        KCapabilitySwisFeature,
                                        KCapabilityTestTerminalID_B,
                                        KPAssertedIdentity_B,
                                        KCapabilityTestSDP );
    iQuery->CompletedL( *iQuery->iTrx );
    
    MusSettingsKeys::TOperatorVariant op = MultimediaSharingSettings::OperatorVariantSettingL();
    EUNIT_ASSERT( op == MusSettingsKeys::EStandard );

    EUNIT_ASSERT( iQuery->iState == 
                CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    EUNIT_ASSERT( !iQuery->Retrying() );
    EUNIT_ASSERT( iQuery->iTimer == NULL );

    EUNIT_ASSERT( iStorage->iClientRequest->Method() == 
                            SIPStrings::StringF( SipStrConsts::EOptions ) );
                            
    EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().Content().Length()
             > 0 );
    EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().ContentType() );
    EUNIT_ASSERT( iStorage->iClientRequest->RemoteUri().Uri().UriDes().Compare( 
            KCapabilityTestSIPAddressUri8_B ) == 0);
    EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().UserHeaders().
            Count() == 6 );
    MUS_HEADER_ASSERT( iStorage->iClientRequest, 
                    0, 
                    KCapabilityTestAcceptContactHeader );
    MUS_HEADER_ASSERT( iStorage->iClientRequest, 
                    5, 
                    KCapabilityTestContactHeader_A1Updated );
    MUS_HEADER_ASSERT( iStorage->iClientRequest, 3, 
                    KCapabilityTestAcceptHeader );
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaStatusAvailable );
    
                            
    iQuery->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );                                
    TRAP( error, iQuery->ExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    
    iQuery->DoRetryL();
    TRAP( error, iQuery->ExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    iQuery->DoRetryL();
    
    // set TerminalId for CMusAvaCapabilityExchange, terminal Id will be added to headers
    CMusAvaCapabilityQueryObserverImp* capabQueryObserver = 
                        new ( ELeave ) CMusAvaCapabilityQueryObserverImp();
    CleanupStack::PushL( capabQueryObserver );                        
    CMusAvaCapabilityExchange* exchange =
                        CMusAvaCapabilityExchange::NewL( 
                                            KCapabilityTestTerminalID_A, 
                                            *capabQueryObserver );
    CleanupStack::PushL( exchange );                        
    CMusAvaCapability* capability = CMusAvaCapability::NewL( *exchange );
    CleanupStack::PushL( capability );                        
    
    CMusAvaCapabilityQuery* query = CMusAvaCapabilityQuery::NewL( *capability,
                                            *iSIPConnection,
                                            *iProfile,
                                            KCapabilityTestSIPAddressUri_B );
    CleanupStack::PushL( query ); 
    
    query->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    query->ExecuteL();
    EUNIT_ASSERT_EQUALS( iStorage->iClientRequest->MessageElements().UserHeaders().Count(), 7 ); 
    MUS_HEADER_ASSERT( iStorage->iClientRequest, 6, 
                    KCapabilityTestUserAgent_A );
    
    CleanupStack::PopAndDestroy( query );
    CleanupStack::PopAndDestroy( capability );
    CleanupStack::PopAndDestroy( exchange );
    CleanupStack::PopAndDestroy( capabQueryObserver );
    }

void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_CanceledL(  )
    {
    
    iQuery->ExecuteL( );
    
    
    EUNIT_ASSERT( iQuery->State() ==  
                    CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );
    
    // set response 200 OK FT
    CapabilityTestHelper::SetResponseL( *iQuery->iTrx, 
                                        KMUSAVASIP200,
                                        SipStrConsts::EPhraseOk,
                                        KCapabilityTestAddress_B,
                                        KCapabilitySwisFeature,
                                        KCapabilityTestTerminalID_B,
                                        KPAssertedIdentity_B,
                                        KCapabilityTestSDP );
    
    iQuery->Canceled( *iQuery->iTrx );
    EUNIT_ASSERT( iQuery->State() == 
                CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    EUNIT_ASSERT( iQuery->Result() == KCapabilityCapabilitesNotFound );
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaOptionNotAvailable );
                            
                            
    // give different response than iTrx                                        
    iQuery->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    iQuery->ExecuteL();
    CSIPClientTransaction* cliTransaction = NULL;
    cliTransaction = CSIPClientTransaction::NewL( iQuery->iTrx->Type() );
    CleanupStack::PushL( cliTransaction );
   
    CapabilityTestHelper::SetResponseL( *cliTransaction, 
                                        KMUSAVASIPForbidden, 
                                        SipStrConsts::EPhraseOk,
                                        KCapabilityTestAddress_B,
                                        KCapabilitySwisFeature,
                                        KCapabilityTestTerminalID_B,
                                        KPAssertedIdentity_B,
                                        KCapabilityTestSDP );
    iQuery->Canceled( *cliTransaction );                              
    EUNIT_ASSERT_NOT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );              
    iQuery->ResetAndDestroyTrx();
    iQuery->Canceled( *cliTransaction );
    EUNIT_ASSERT_NOT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );                  
    CleanupStack::PopAndDestroy( cliTransaction );                            
    }
    
void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_CompletedLL(  )
    {
    TRAPD( error, RProperty::Define(
                            NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMUSForbidden,
                            RProperty::EInt,
                            0 ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
                               
    TRAP( error, RProperty::Set(
    	NMusSessionInformationApi::KCategoryUid,
    	NMusSessionInformationApi::KMUSForbidden,
    	( TInt ) NMusSessionInformationApi::EMUSAllowed ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    // Test for forbidden response (403)
    
    //Cingular specific case && 403 Forbidden
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EOperatorSpecific );
    iQuery->ExecuteL( );
    
    EUNIT_ASSERT( iQuery->State() ==  
                    CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );

    // set response 200 OK FT
    CapabilityTestHelper::SetResponseL( *iQuery->iTrx, 
                                        KMUSAVASIPForbidden, 
                                        SipStrConsts::EPhraseOk,
                                        KCapabilityTestAddress_B,
                                        KCapabilitySwisFeature,
                                        KCapabilityTestTerminalID_B,
                                        KPAssertedIdentity_B,
                                        KCapabilityTestSDP );
    
    iQuery->CompletedL( *iQuery->iTrx );
    EUNIT_ASSERT( iQuery->State() == 
                CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    EUNIT_ASSERT( iQuery->Result() == KCapabilityCapabilitiesForbidden );
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaFailureCode );
    // check if it is "forbidden"
    TInt forbidden;
    TRAP( error, RProperty::Get( NMusSessionInformationApi::KCategoryUid,
	                    NMusSessionInformationApi::KMUSForbidden,
	                    forbidden ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

	EUNIT_ASSERT_EQUALS( NMusSessionInformationApi::EMUSForbidden, forbidden );                            
    // set back the forbidden flag to allowed	
    TRAP( error, RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMUSForbidden,
                        ( TInt ) NMusSessionInformationApi::EMUSAllowed ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EStandard );
                                         
    // give different response than iTrx                                        
    iQuery->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    iQuery->ExecuteL( );
    CSIPClientTransaction* cliTransaction = NULL;
    cliTransaction = CSIPClientTransaction::NewL( iQuery->iTrx->Type() );
    CleanupStack::PushL( cliTransaction );
   
    CapabilityTestHelper::SetResponseL( *cliTransaction, 
                                        KMUSAVASIPForbidden, 
                                        SipStrConsts::EPhraseOk,
                                        KCapabilityTestAddress_B,
                                        KCapabilitySwisFeature,
                                        KCapabilityTestTerminalID_B,
                                        KPAssertedIdentity_B,
                                        KCapabilityTestSDP );
    iQuery->CompletedL( *cliTransaction );
    EUNIT_ASSERT_NOT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );              
    iQuery->ResetAndDestroyTrx();
    iQuery->CompletedL( *cliTransaction );
    EUNIT_ASSERT_NOT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );                  
    CleanupStack::PopAndDestroy( cliTransaction );                                        

    // set response 199
    iQuery->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    iQuery->ExecuteL( ); 
    CapabilityTestHelper::SetResponseL( *iQuery->iTrx, 
                                        KMUSAVASIP200-1, 
                                        SipStrConsts::EPhraseOk,
                                        KCapabilityTestAddress_B,
                                        KCapabilitySwisFeature,
                                        KCapabilityTestTerminalID_B,
                                        KPAssertedIdentity_B,
                                        KCapabilityTestSDP );
                

    iQuery->CompletedL( *iQuery->iTrx );
    EUNIT_ASSERT_NOT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted ); 
    iQuery->ResetAndDestroyTrx();
    
    // set response <100
    iQuery->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    iQuery->ExecuteL( );
    CapabilityTestHelper::SetResponseL( *iQuery->iTrx, 
                                        KMUSAVASIP200-150, 
                                        SipStrConsts::EPhraseOk,
                                        KCapabilityTestAddress_B,
                                        KCapabilitySwisFeature,
                                        KCapabilityTestTerminalID_B,
                                        KPAssertedIdentity_B,
                                        KCapabilityTestSDP );
    iQuery->CompletedL( *iQuery->iTrx );
    EUNIT_ASSERT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );          
    }

void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_CompletedL1L(  )
    {
    //Standard case && 403 Forbidden
    iQuery->ExecuteL( );
    
    EUNIT_ASSERT( iQuery->State() ==  
                    CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );
    
    // set response 200 OK FT
    CapabilityTestHelper::SetResponseL( *iQuery->iTrx, 
                                        KMUSAVASIPForbidden, 
                                        SipStrConsts::EPhraseOk,
                                        KCapabilityTestAddress_B,
                                        KCapabilitySwisFeature,
                                        KCapabilityTestTerminalID_B,
                                        KPAssertedIdentity_B,
                                        KCapabilityTestSDP );
    
    iQuery->CompletedL( *iQuery->iTrx );
    EUNIT_ASSERT( iQuery->State() == 
                CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    EUNIT_ASSERT( iQuery->Result() == KCapabilityCapabilitesNotFound );
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaOptionNotAvailable );
    
    }
    
void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_CompletedL2L(  )
    {
    //Standard case
    iQuery->ExecuteL( );
    
    EUNIT_ASSERT( iQuery->State() ==  
                    CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );
    
    // set response 200 OK FT
    CapabilityTestHelper::SetResponse2L( *iQuery->iTrx, 
                                        KMUSAVASIP200, 
                                        SipStrConsts::EPhraseOk,
                                        KCapabilityTestAddress_B,
                                        KCapabilitySwisFeature,
                                        KCapabilityTestTerminalID_B,
                                        KPAssertedIdentity_B,
                                        KPAssertedIdentity_B1,
                                        KCapabilityTestSDP );
    
    iQuery->CompletedL( *iQuery->iTrx );
    EUNIT_ASSERT( iQuery->State() == 
                CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    EUNIT_ASSERT( iQuery->Result() == KCapabilityCapabilitesReady );
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaStatusAvailable );
    const MDesCArray& sipAddresses = iOptionHandler->iSettings.SipAddresses();
    EUNIT_ASSERT( sipAddresses.MdcaCount() == 1);
    EUNIT_ASSERT( sipAddresses.MdcaPoint( 0 ).Compare( KPAssertedIdentity_B_16() )
             == KErrNone ); 
    }

void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_ValidateAndStoreCodecsLL()
    {
    
    // OK - H.263 codec
    TBool ret = EFalse;
    CSdpDocument* sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDP );
    TRAPD( error, ret = iQuery->ValidateAndStoreCodecsL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    CleanupStack::PopAndDestroy( sdpContent ); 
    
    // OK - H.264 codec
    ret = EFalse;
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDP264 );
    TRAP( error, ret = iQuery->ValidateAndStoreCodecsL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    CleanupStack::PopAndDestroy( sdpContent ); 

    // OK - both H.263 and H.264 codec
    ret = EFalse;
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDP264And263 );
    TRAP( error, ret = iQuery->ValidateAndStoreCodecsL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    CleanupStack::PopAndDestroy( sdpContent ); 

    // NOK - invalid codec
    ret = ETrue;
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPWrongCodec );
    TRAP( error, ret = iQuery->ValidateAndStoreCodecsL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !ret );
    CleanupStack::PopAndDestroy( sdpContent );
}

void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_ValidateAttributesLL() 
    {
    
    // set operator specific
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EOperatorSpecific );
    
    // operator specific OK                         
    CSdpDocument* sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDP );
    TBool ret = EFalse;
    TRAPD( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    CleanupStack::PopAndDestroy( sdpContent ); 

    // operator specific no application                                 
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPNoApplication );
    TRAP( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !ret );
    CleanupStack::PopAndDestroy( sdpContent ); 
    
    // operator specific wrong application                                 
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPWrongApplication );
    TRAP( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !ret );
    CleanupStack::PopAndDestroy( sdpContent );     

    // operator specific no type                                                
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPNoType );
    TRAP( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !ret );
    CleanupStack::PopAndDestroy( sdpContent );     
   
    // operator specific wrong type                                                
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPWrongType );
    TRAP( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !ret );
    CleanupStack::PopAndDestroy( sdpContent );     

    // set standard
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    
    // OK                         
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDP );
    TRAP( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    CleanupStack::PopAndDestroy( sdpContent ); 

    // no application, good type                                
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPNoApplication );
    TRAP( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    CleanupStack::PopAndDestroy( sdpContent ); 
            
    // wrong application, good type
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPWrongApplication );
    TRAP( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    CleanupStack::PopAndDestroy( sdpContent ); 

    // no type, good application                                                
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPNoType );
    TRAP( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    CleanupStack::PopAndDestroy( sdpContent ); 
    
    // wrong type, good application                                                
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPWrongType );
    TRAP( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    CleanupStack::PopAndDestroy( sdpContent ); 

    // no application, no type                                
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPNoAppNoType );
    TRAP( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !ret );
    CleanupStack::PopAndDestroy( sdpContent ); 
    
    // no application, wrong type                                
    sdpContent = CSdpDocument::DecodeLC( KCapabilityTestSDPNoAppWrongType );
    TRAP( error, ret = iQuery->ValidateAttributesL( *sdpContent ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !ret );
    CleanupStack::PopAndDestroy( sdpContent ); 
    }
    
    
void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_ValidateContactLL()     
    {
    
    // no contact header                                     
    CSIPClientTransaction* optionResp = CapabilityTestHelper::OptionsResponseL( 
                                                KMUSAVASIP200,
                                                SipStrConsts::EPhraseOk,
                                                KCapabilityTestAddress_B,
                                                KNullDesC8,
                                                KCapabilityTestTerminalID_B,
                                                KPAssertedIdentity_B,
                                                KCapabilityTestSDP );
    CleanupStack::PushL( optionResp );
    TBool ret = EFalse;
    TRAPD( error, ret = iQuery->ValidateContactL( *optionResp ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !ret );
    CleanupStack::PopAndDestroy( optionResp );     
    
    // valid contact header                                     
    optionResp = CapabilityTestHelper::OptionsResponseL( KMUSAVASIP200,
                                                SipStrConsts::EPhraseOk,
                                                KCapabilityTestAddress_B,
                                                KCapabilitySwisFeature,
                                                KCapabilityTestTerminalID_B,
                                                KPAssertedIdentity_B,
                                                KCapabilityTestSDP );
    CleanupStack::PushL( optionResp );
    
    TRAP( error, ret = iQuery->ValidateContactL( *optionResp ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    CleanupStack::PopAndDestroy( optionResp );         
    
    // false contact header                                     
    optionResp = CapabilityTestHelper::OptionsResponseL( KMUSAVASIP200,
                                                SipStrConsts::EPhraseOk,
                                                KCapabilityTestAddress_B,
                                                KCapabilitySwisFeatureFalse,
                                                KCapabilityTestTerminalID_B,
                                                KPAssertedIdentity_B,
                                                KCapabilityTestSDP );
    CleanupStack::PushL( optionResp );
    TRAP( error, ret = iQuery->ValidateContactL( *optionResp ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !ret );
    CleanupStack::PopAndDestroy( optionResp );             
    
    // wrong contact header                                     
    optionResp = CapabilityTestHelper::OptionsResponseL( KMUSAVASIP200,
                                                SipStrConsts::EPhraseOk,
                                                KCapabilityTestAddress_B,
                                                KCapabilityTestDataFeature,
                                                KCapabilityTestTerminalID_B,
                                                KPAssertedIdentity_B,
                                                KCapabilityTestSDP );
    CleanupStack::PushL( optionResp );
    TRAP( error, ret = iQuery->ValidateContactL( *optionResp ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !ret );
    CleanupStack::PopAndDestroy( optionResp );                 
    }
    
    
void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_PrepareL()     
    {
    RPointerArray<CSIPHeaderBase> headers;
    CleanupClosePushL(headers);
    
    iQuery->Prepare( headers );
    EUNIT_ASSERT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryPrepared );
    
    iQuery->DoRetryL();
    
    EUNIT_ASSERT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    iQuery->Prepare( headers );
    EUNIT_ASSERT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    iQuery->DoRetryL();
    EUNIT_ASSERT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );    
  
    CleanupStack::PopAndDestroy( &headers );
    }
    
    
void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_DoCompletedLL()
    {
    
    // response elements are NULL
    CSIPClientTransaction* optionResp = NULL;
    optionResp = CapabilityTestHelper::OptionsResponseL( 
                                                    KMUSAVASIP200,
                                                    SipStrConsts::EPhraseOk,
                                                    KCapabilityTestAddress_B,
                                                    KNullDesC8,
                                                    KCapabilityTestTerminalID_B,
                                                    KPAssertedIdentity_B,
                                                    KCapabilityTestSDP );
    CleanupStack::PushL( optionResp );    
    optionResp->SetResponseElements( NULL );
    
    TRAPD( error, iQuery->DoCompletedL( *optionResp ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrGeneral );
    CleanupStack::PopAndDestroy( optionResp );
    
    // status code KMUSAVASIPNotImplemented
    optionResp = CapabilityTestHelper::OptionsResponseL( 
                                                    KMUSAVASIPNotImplemented,
                                                    SipStrConsts::EPhraseOk,
                                                    KCapabilityTestAddress_B,
                                                    KNullDesC8,
                                                    KCapabilityTestTerminalID_B,
                                                    KPAssertedIdentity_B,
                                                    KCapabilityTestSDP );
    CleanupStack::PushL( optionResp );    
    iQuery->DoCompletedL( *optionResp );
    EUNIT_ASSERT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );    
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityCapabilitesReady );        
    CleanupStack::PopAndDestroy( optionResp ); 
    
    // status code KMUSAVASIPTemporarilyUnavailable
    optionResp = CapabilityTestHelper::OptionsResponseL( 
                                            KMUSAVASIPTemporarilyUnavailable,
                                            SipStrConsts::EPhraseOk,
                                            KCapabilityTestAddress_B,
                                            KNullDesC8,
                                            KCapabilityTestTerminalID_B,
                                            KPAssertedIdentity_B,
                                            KCapabilityTestSDP );
    CleanupStack::PushL( optionResp );    
    iQuery->DoCompletedL( *optionResp );
    EUNIT_ASSERT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );    
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityQueryNotReady );        
    TRAP( error, iQuery->DoRetryL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    CleanupStack::PopAndDestroy( optionResp );     
    
    // status code KMUSAVASIPServerInternalError
    optionResp = CapabilityTestHelper::OptionsResponseL( 
                                            KMUSAVASIPServerInternalError,
                                            SipStrConsts::EPhraseOk,
                                            KCapabilityTestAddress_B,
                                            KNullDesC8,
                                            KCapabilityTestTerminalID_B,
                                            KPAssertedIdentity_B,
                                            KCapabilityTestSDP );
    CleanupStack::PushL( optionResp );    
    TRAP( error, iQuery->DoCompletedL( *optionResp ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );    
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityCapabilitesNotFound );        
    CleanupStack::PopAndDestroy( optionResp );         
    
    }
    
    
void UT_CMusAvaCapabilityQuery::UT_CMusAvaCapabilityQuery_DoCompleted200OKLL()    
    {
    
    // no content
    CSIPClientTransaction* optionResp = CapabilityTestHelper::OptionsResponseL( 
                                                KMUSAVASIP200,
                                                SipStrConsts::EPhraseOk,
                                                KCapabilityTestAddress_B,
                                                KCapabilitySwisFeature,
                                                KCapabilityTestTerminalID_B,
                                                KPAssertedIdentity_B,
                                                KNullDesC8 );
    CleanupStack::PushL( optionResp );    
    iQuery->DoCompleted200OKL( *optionResp );
    EUNIT_ASSERT_EQUALS( iQuery->State(), CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );    
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityCapabilitesNotFound );    
    CleanupStack::PopAndDestroy( optionResp ); 
    
    
    // wrong content type header
    optionResp = 
            CapabilityTestHelper::OptionsResponseL( KMUSAVASIP200,
                                                    SipStrConsts::EPhraseOk,
                                                    KCapabilityTestAddress_B,
                                                    KCapabilitySwisFeature,
                                                    KCapabilityTestTerminalID_B,
                                                    KPAssertedIdentity_B,
                                                    KCapabilityTestSDP );
    CleanupStack::PushL( optionResp );    
    
    const TDesC8& sdp = KCapabilityTestSDP;
    HBufC8* content = NULL;
    content = sdp.AllocL();
    CleanupStack::PushL( content );
    const CSIPResponseElements* response = optionResp->ResponseElements();
    CSIPMessageElements& message = 
            CONST_CAST(CSIPResponseElements*, response)->MessageElements();
    
    CSIPContentTypeHeader* contentTypeHeader = CSIPContentTypeHeader::NewLC( 
                                                KMUSAVASIPMediaTypeApplication,
                                                KMUSAVASIPParamFalse );
                                                    
    message.SetContentL( content, contentTypeHeader );    
    CleanupStack::Pop( contentTypeHeader ); 
    CleanupStack::Pop( content ); 
    iQuery->DoCompleted200OKL( *optionResp );
    EUNIT_ASSERT_EQUALS( iQuery->State(), 
                        CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );    
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityCapabilitesNotFound ); 
    
    content = sdp.AllocL();
    CleanupStack::PushL( content );
    contentTypeHeader = CSIPContentTypeHeader::NewLC( KMUSAVASIPParamFalse,
                                                  KMUSAVASIPMediaSubTypeSDP );
    message.SetContentL( content, contentTypeHeader );
    CleanupStack::Pop( contentTypeHeader ); 
    CleanupStack::Pop( content );                                                       
    iQuery->DoCompleted200OKL( *optionResp );
    EUNIT_ASSERT_EQUALS( iQuery->State(), 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityCapabilitesNotFound ); 
    
    CleanupStack::PopAndDestroy( optionResp );
    
    
    // wrong codec
    optionResp = CapabilityTestHelper::OptionsResponseL( KMUSAVASIP200,
                                                SipStrConsts::EPhraseOk,
                                                KCapabilityTestAddress_B,
                                                KCapabilitySwisFeature,
                                                KCapabilityTestTerminalID_B,
                                                KPAssertedIdentity_B,
                                                KCapabilityTestSDPWrongCodec );
    CleanupStack::PushL( optionResp );    
    iQuery->DoCompleted200OKL( *optionResp );
    EUNIT_ASSERT_EQUALS( iQuery->State(), 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );    
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityCapabilitesNotFound );    
    CleanupStack::PopAndDestroy( optionResp );   
    
    // set standard
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                                MusSettingsKeys::EStandard );
        
    // wrong attribute (no application, no type ), wrong feature tag
    optionResp = CapabilityTestHelper::OptionsResponseL( KMUSAVASIP200,
                                            SipStrConsts::EPhraseOk,
                                            KCapabilityTestAddress_B,
                                            KCapabilitySwisFeatureFalse,
                                            KCapabilityTestTerminalID_B,
                                            KPAssertedIdentity_B,
                                            KCapabilityTestSDPNoAppNoType );
    CleanupStack::PushL( optionResp );    
    iQuery->DoCompleted200OKL( *optionResp );
    EUNIT_ASSERT_EQUALS( iQuery->State(), 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityCapabilitesNotFound );    
    CleanupStack::PopAndDestroy( optionResp );      
    
    
    // good attribute, wrong feature tag => KCapabilityCapabilitesReady 
    optionResp = CapabilityTestHelper::OptionsResponseL( KMUSAVASIP200,
                                            SipStrConsts::EPhraseOk,
                                            KCapabilityTestAddress_B,
                                            KCapabilitySwisFeatureFalse,
                                            KCapabilityTestTerminalID_B,
                                            KPAssertedIdentity_B,
                                            KCapabilityTestSDP );
    CleanupStack::PushL( optionResp );    
    iQuery->DoCompleted200OKL( *optionResp );
    EUNIT_ASSERT_EQUALS( iQuery->State(), 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );    
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityCapabilitesNotFound );    
    CleanupStack::PopAndDestroy( optionResp );         
    
    // wrong attribute (no application, no type ), good feature tag => KCapabilityCapabilitesReady 
    optionResp = CapabilityTestHelper::OptionsResponseL( KMUSAVASIP200,
                                            SipStrConsts::EPhraseOk,
                                            KCapabilityTestAddress_B,
                                            KCapabilitySwisFeature,
                                            KCapabilityTestTerminalID_B,
                                            KPAssertedIdentity_B,
                                            KCapabilityTestSDPNoAppNoType );
    CleanupStack::PushL( optionResp );    
    iQuery->DoCompleted200OKL( *optionResp );
    EUNIT_ASSERT_EQUALS( iQuery->State(), 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityCapabilitesReady  );    
    CleanupStack::PopAndDestroy( optionResp );    
    
    
    
    // set operator specific
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                            MusSettingsKeys::EOperatorSpecific );
    
    // operator specific, wrong attribute (no application, no type )
    optionResp = CapabilityTestHelper::OptionsResponseL( KMUSAVASIP200,
                                            SipStrConsts::EPhraseOk,
                                            KCapabilityTestAddress_B,
                                            KCapabilitySwisFeature,
                                            KCapabilityTestTerminalID_B,
                                            KPAssertedIdentity_B,
                                            KCapabilityTestSDPNoAppNoType );
    CleanupStack::PushL( optionResp );    
    iQuery->DoCompleted200OKL( *optionResp );
    EUNIT_ASSERT_EQUALS( iQuery->State(), 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );    
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityCapabilitesNotFound );    
    CleanupStack::PopAndDestroy( optionResp );      
    
    
    // operator specific, wrong feature tag
    optionResp = CapabilityTestHelper::OptionsResponseL( KMUSAVASIP200,
                                            SipStrConsts::EPhraseOk,
                                            KCapabilityTestAddress_B,
                                            KCapabilitySwisFeatureFalse,
                                            KCapabilityTestTerminalID_B,
                                            KPAssertedIdentity_B,
                                            KCapabilityTestSDP );
    CleanupStack::PushL( optionResp );    
    iQuery->DoCompleted200OKL( *optionResp );
    EUNIT_ASSERT_EQUALS( iQuery->State(), 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );    
    EUNIT_ASSERT_EQUALS( iQuery->Result(), KCapabilityCapabilitesNotFound );    
    CleanupStack::PopAndDestroy( optionResp );      
                                    
    
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaCapabilityQuery,
    "CMusAvaCapabilityQuery",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaCapabilityQuery",
    "NewL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_NewLL, Teardown)
    
EUNIT_TEST(
    "ExecuteL - test ",
    "CMusAvaCapabilityQuery",
    "ExecuteL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_ExecuteLL, Teardown)

EUNIT_TEST(
    "Canceled - test ",
    "CMusAvaCapabilityQuery",
    "Canceled",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_CanceledL, Teardown)

EUNIT_TEST(
    "CompletedL - test ",
    "CMusAvaCapabilityQuery",
    "CompletedL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_CompletedLL, Teardown)

EUNIT_TEST(
    "CompletedL1 - test ",
    "CMusAvaCapabilityQuery",
    "CompletedL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_CompletedL1L, Teardown)
    
EUNIT_TEST(
    "CompletedL2 - test ",
    "CMusAvaCapabilityQuery",
    "CompletedL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_CompletedL2L, Teardown)
    
EUNIT_TEST(
    "ValidateAndStoreCodecsL - test ",
    "CMusAvaCapabilityQuery",
    "ValidateAndStoreCodecsL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_ValidateAndStoreCodecsLL, Teardown)   

EUNIT_TEST(
    "ValidateAttributesL - test ",
    "CMusAvaCapabilityQuery",
    "ValidateAttributesL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_ValidateAttributesLL, Teardown)   
    
EUNIT_TEST(
    "ValidateContactL - test ",
    "CMusAvaCapabilityQuery",
    "ValidateContactL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_ValidateContactLL, Teardown)        
    
EUNIT_TEST(
    "Prepare - test ",
    "CMusAvaCapabilityQuery",
    "Prepare",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_PrepareL, Teardown)  
    
EUNIT_TEST(
    "DoCompletedL - test ",
    "CMusAvaCapabilityQuery",
    "DoCompletedL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_DoCompletedLL, Teardown)                

EUNIT_TEST(
    "DoCompleted200OKL - test ",
    "CMusAvaCapabilityQuery",
    "DoCompleted200OKL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityQuery_DoCompleted200OKLL, Teardown)  
    
EUNIT_END_TEST_TABLE

//  END OF FILE
