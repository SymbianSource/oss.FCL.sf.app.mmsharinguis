/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
#include "UT_CMusAvaTerminal.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES
#include "musavaCapabilityExchange.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "musavaoptionhandler.h"
#include "musavasettingsimp.h"
#include "musavaobserverimp.h"
#include "musavaterminal.h"
#include "MusAvaCapability.h"
#include "MusAvaCapabilityQuery.h"
#include "MusAvaCapabilityTestHelper.h"
#include "CSipSseTestTls.h"
#include "musavacapabilitycontext.h"
#include "musavasip.h"
#include "musavasharedobject.h"
#include "mussettings.h"


// CONSTRUCTION
UT_CMusAvaTerminal* UT_CMusAvaTerminal::NewL()
    {
    UT_CMusAvaTerminal* self = UT_CMusAvaTerminal::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaTerminal* UT_CMusAvaTerminal::NewLC()
    {
    UT_CMusAvaTerminal* self = new( ELeave ) UT_CMusAvaTerminal();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaTerminal::~UT_CMusAvaTerminal()
    {
    }

// Default constructor
UT_CMusAvaTerminal::UT_CMusAvaTerminal()
    {
    }

// Second phase construct
void UT_CMusAvaTerminal::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaTerminal::SetupL(  )
    {

    MSIPObserver* dummyObserver = NULL;
    CSIPProfileRegistry* dummyRegistry = NULL;
    MSIPConnectionObserver* dummyConnectionObserver = NULL;
    
    TUid uid = TUid::Uid( 1 );
    TUint32 iapId = 32;
    
    iSIP = CSIP::NewL( uid, *dummyObserver );
    iProfile = CSIPProfile::NewL( dummyRegistry );
    iSIPConnection = CSIPConnection::NewL( *iSIP, 
                                            iapId,
                                            *dummyConnectionObserver );
                               
    
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iOptionHandler = CMusAvaOptionHandler::NewL( *iAvailabilityObserver,
                                                 *iConcreteSettings );

    iExchange = iOptionHandler->iCapabilityExchange;
    
    iCapability = iOptionHandler->iSwisCapability;

    iTerminal = CMusAvaTerminal::NewL( *iExchange,
                                     KCapabilityTestAddress_B,
                                     KCapabilityTestTerminalID_B );
    if ( !iProfile->iRegisteredContact )
        {
        iProfile->iRegisteredContact = KRegisteredContact().AllocL();
        }
    
    CSipSseTestTls::OpenL();
    }


void UT_CMusAvaTerminal::Teardown(  )
    {

    CSipSseTestTls::Close();
    delete iTerminal;
    delete iOptionHandler;
    delete iConcreteSettings;
    delete iAvailabilityObserver;

    delete iSIPConnection;
    delete iProfile;
    delete iSIP;
    
    }

void UT_CMusAvaTerminal::UT_CMusAvaTerminal_NewLL(  )
    {
    
                                         
    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->IsDiscovered() );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );
    

    }

void UT_CMusAvaTerminal::UT_CMusAvaTerminal_MatchLL(  )
    {

    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->IsDiscovered() );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );

    
    EUNIT_ASSERT( iTerminal->MatchL( KCapabilityTestAddress_B, 
                                KCapabilityTestTerminalID_B ) );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) 
                == 0 );
    
    EUNIT_ASSERT( !iTerminal->MatchL( KCapabilityTestTerminalID_B, 
                KCapabilityTestAddress_B ) );//!wrong way around
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) 
                == 0 );

    EUNIT_ASSERT( iTerminal->MatchL( KCapabilityTestAddress_B, KNullDesC8 ) );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) 
                == 0 );

    EUNIT_ASSERT( !iTerminal->MatchL( KNullDesC8, KNullDesC8 ) );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) 
                == 0 );
    
    EUNIT_ASSERT( !iTerminal->MatchL( KNullDesC8, 
                KCapabilityTestTerminalID_B ) );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) 
                == 0 );

    iTerminal->AttachUriL( KNullDesC8 );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) 
                == 0 );

    iTerminal->AttachIdL( KNullDesC8 );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) 
                == 0 );
    
    delete iTerminal->iTerminalId;
    iTerminal->iTerminalId = NULL;
    iTerminal->iTerminalId = KNullDesC8().AllocL();
    EUNIT_ASSERT( iTerminal->Id().Compare( KNullDesC8 ) == 0 );

    EUNIT_ASSERT( iTerminal->MatchL( KCapabilityTestAddress_B, KNullDesC8 ) );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Id().Compare( KNullDesC8 ) == 0 );
    
    EUNIT_ASSERT( iTerminal->MatchL( KCapabilityTestAddress_B, 
                                    KCapabilityTestTerminalID_B ) );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) 
                == 0 );
    
    
    }

void UT_CMusAvaTerminal::UT_CMusAvaTerminal_ExecuteQueryLL(  )
    {
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    if ( !iProfile->iArray )
        {
        iProfile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
        iProfile->iArray->AppendL( _L8("sip:user@domain.com") );
        }
    
    EUNIT_ASSERT( iExchange->Terminals().Count() == 0 );
    
    CMusAvaCapabilityQuery* query = NULL;
    query = CMusAvaCapabilityQuery::NewL( *iCapability,
                                            *iSIPConnection,
                                            *iProfile,
                                            KCapabilityTestSIPAddressUri_B );
    CleanupStack::PushL( query );
    EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
        
    CMusAvaTerminal& terminal = query->Terminal();
    EUNIT_ASSERT( terminal.iQueries == 1 );
    EUNIT_ASSERT( terminal.iQuery == NULL );
    
    TRAPD( error, terminal.ExecuteQueryL( NULL ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error != KErrNone );

    EUNIT_ASSERT( terminal.iQueries == 1 );
    EUNIT_ASSERT( terminal.iQuery == NULL );

    query->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );
    TRAP( error, terminal.ExecuteQueryL( query ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error != KErrNone );
    
    EUNIT_ASSERT( terminal.iQueries == 1 );
    EUNIT_ASSERT( terminal.iQuery == NULL );
    
    query->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    terminal.ExecuteQueryL( query );
    CleanupStack::Pop( query );
    
    EUNIT_ASSERT( terminal.iQueries == 1 );
    EUNIT_ASSERT( terminal.iQuery == query );
    
    query = CMusAvaCapabilityQuery::NewL( *iCapability,
                                        *iSIPConnection,
                                        *iProfile,
                                        KCapabilityTestSIPAddressUri_B );
    CleanupStack::PushL( query );
    EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
    EUNIT_ASSERT( terminal.iQueries == 2 );
    
    //Second query cannot be executed
    EUNIT_ASSERT_SPECIFIC_LEAVE( terminal.ExecuteQueryL( query ),
                                 KErrAlreadyExists )
    EUNIT_ASSERT( terminal.iQuery != query );
    CleanupStack::PopAndDestroy( query );
    }


void UT_CMusAvaTerminal::UT_CMusAvaTerminal_QueryCompletedLL(  )
    {
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    if ( !iProfile->iArray )
        {
        iProfile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
        iProfile->iArray->AppendL( _L8("sip:user@domain.com") );
        }
    
    EUNIT_ASSERT( iExchange->Terminals().Count() == 0 );
    
    CMusAvaCapabilityQuery* query = NULL;
    query = CMusAvaCapabilityQuery::NewL( *iCapability,
                                            *iSIPConnection,
                                            *iProfile,
                                            KCapabilityTestSIPAddressUri_B  );
    CleanupStack::PushL( query );
    EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
        
    CMusAvaTerminal& terminal = query->Terminal();
    EUNIT_ASSERT( terminal.iQueries == 1 );

    TBool ret;
    TRAPD( err, ret = terminal.QueryCompletedL( *query->iTrx ) );
    if ( err == KErrNoMemory ) User::Leave( err );
    EUNIT_ASSERT( !ret );
    
    terminal.ExecuteQueryL( query );
    CleanupStack::Pop( query );

    EUNIT_ASSERT( terminal.iQueries == 1 );
    EUNIT_ASSERT( terminal.iQuery == query );

    query->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );

    //timeout
    CapabilityTestHelper::SetResponseL( *query->iTrx, 
                                    KMUSAVASIPTimeout, 
                                    SipStrConsts::EPhraseRequestTimeout,
                                    KCapabilityTestAddress_B,
                                    KNullDesC8,
                                    KNullDesC8,
                                    KNullDesC8,
                                    KNullDesC8 );

    TRAP( err, ret = terminal.QueryCompletedL( *query->iTrx ) );
    if ( err == KErrNoMemory ) User::Leave( err );
    EUNIT_ASSERT( !ret );
    
    query->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );
    
    //200 OK with no User-Agent
    CapabilityTestHelper::SetResponseL( *query->iTrx, 
                                KMUSAVASIP200, 
                                SipStrConsts::EPhraseOk,
                                KCapabilityTestAddress_B,
                                KCapabilitySwisFeature,
                                KNullDesC8,
                                KPAssertedIdentity_B,
                                KCapabilityTestSDP );

    TRAP( err, ret = terminal.QueryCompletedL( *query->iTrx ) );
    if ( err == KErrNoMemory ) User::Leave( err );
    EUNIT_ASSERT( ret );
    
    query->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );
    
    //200 OK with User-Agent
    CapabilityTestHelper::SetResponseL( *query->iTrx, 
                                KMUSAVASIP200, 
                                SipStrConsts::EPhraseOk,
                                KCapabilityTestAddress_B,
                                KCapabilitySwisFeature,
                                KCapabilityTestTerminalID_B,
                                KPAssertedIdentity_B,
                                KCapabilityTestSDP );
                              
    TRAP( err, ret = terminal.QueryCompletedL( *query->iTrx ) );
    if ( err == KErrNoMemory ) User::Leave( err );
    EUNIT_ASSERT( ret );
    
    TRAP( err, ret = terminal.QueryCompletedL( *query->iTrx ) );
    if ( err == KErrNoMemory ) User::Leave( err );
    EUNIT_ASSERT( !ret );
    }

void UT_CMusAvaTerminal::UT_CMusAvaTerminal_QueryCanceledL(  )
    {
    if ( !iProfile->iArray )
        {
        iProfile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
        iProfile->iArray->AppendL( _L8("sip:user@domain.com") );
        }

    EUNIT_ASSERT( iExchange->Terminals().Count() == 0 );
    
    CMusAvaCapabilityQuery* query = NULL;
    query = CMusAvaCapabilityQuery::NewL( *iCapability,
                                            *iSIPConnection,
                                            *iProfile,
                                            KCapabilityTestSIPAddressUri_B  );
    CleanupStack::PushL( query );
    EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
        
    CMusAvaTerminal& terminal = query->Terminal();
    EUNIT_ASSERT( terminal.iQueries == 1 );
    
    terminal.ExecuteQueryL( query );
    CleanupStack::Pop( query );

    EUNIT_ASSERT( terminal.iQueries == 1 );
    EUNIT_ASSERT( terminal.iQuery == query );

    query->SetState( CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );

    //200 OK with User-Agent
    CapabilityTestHelper::SetResponseL( *query->iTrx, 
                                KMUSAVASIP200, 
                                SipStrConsts::EPhraseOk,
                                KCapabilityTestAddress_B,
                                KCapabilitySwisFeature,
                                KCapabilityTestTerminalID_B,
                                KPAssertedIdentity_B,
                                KCapabilityTestSDP );
    //TInt err = 404;
    EUNIT_ASSERT( terminal.QueryCanceled( *query->iTrx ) );
    EUNIT_ASSERT( query->State() == 
                CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );

    }

void UT_CMusAvaTerminal::UT_CMusAvaTerminal_QueryRequestedLL(  )
    {    

    CSIPServerTransaction* options = NULL;
    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iTerminal->IsDiscovered() );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );

        
    options =   
        CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                         KCapabilityTestAddress_B,
                                         KNullDesC8,
                                         KCapabilitySwisFeature,
                                         KCapabilityTestAcceptHeader,
                                         KCapabilityTestTerminalID_B );
    CleanupStack::PushL( options );    

    TRAPD( error, iTerminal->QueryRequestedL( *options ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error != KErrNone );
    
    
    CleanupStack::PopAndDestroy( options );

    options =   
        CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                         KCapabilityTestAddress_B,
                                         KCapabilitySwisFeature,
                                         KCapabilitySwisFeature,
                                         KCapabilityTestAcceptHeader,
                                         KCapabilityTestTerminalID_B );
    CleanupStack::PushL( options );

    iTerminal->QueryRequestedL( *options );
    
    EUNIT_ASSERT( iTerminal->RequestedQuery() == iCapability );
    EUNIT_ASSERT( iConcreteSettings->SipAddresses().MdcaCount() == 0 );
    
    CleanupStack::PopAndDestroy( options );

    // with Asserted Identity, check also that SDP of received request is used
    options =   
        CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                         KCapabilityTestAddress_B,
                                         KCapabilitySwisFeature,
                                         KCapabilitySwisFeature,
                                         KCapabilityTestAcceptHeader,
                                         KCapabilityTestTerminalID_B,
                                         KCapabilityPAssertedIdentity,
                                         KCapabilityTestSDP);
    CleanupStack::PushL( options );        
    iTerminal->QueryRequestedL( *options );   

    
    EUNIT_ASSERT( iConcreteSettings->SipAddresses().MdcaCount() == 1 );    
    EUNIT_ASSERT_EQUALS( iConcreteSettings->iVideoCodecs->MdcaCount(), 1 );
    EUNIT_ASSERT_EQUALS( iConcreteSettings->iVideoCodecs->MdcaPoint(0), _L("H263-2000/90000") );
    CleanupStack::PopAndDestroy( options );
  
    CSipSseTestTls* server = CSipSseTestTls::Storage();
    CSipSseTestTls::Storage()->Clear();
    iTerminal->iRequestedQuery = NULL;
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );
    
    // with Asserted Identity, check also that SDP of received request is used
   options =   
        CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                         KCapabilityTestAddress_B,
                                         KCapabilitySwisFeature,
                                         KCapabilitySwisFeature,
                                         KCapabilityTestAcceptHeader,
                                         KCapabilityTestTerminalID_B,
                                         KCapabilityPAssertedIdentity,
                                         KCapabilityTestSDP264And263);
    CleanupStack::PushL( options );        
    iTerminal->QueryRequestedL( *options );   
    EUNIT_ASSERT( iConcreteSettings->SipAddresses().MdcaCount() == 1 );    
    EUNIT_ASSERT_EQUALS( iConcreteSettings->iVideoCodecs->MdcaCount(), 2 );
    EUNIT_ASSERT_EQUALS( iConcreteSettings->iVideoCodecs->MdcaPoint(0), _L("H264/90000") );
    EUNIT_ASSERT_EQUALS( iConcreteSettings->iVideoCodecs->MdcaPoint(1), _L("H263-2000/90000") );
    CleanupStack::PopAndDestroy( options );
    
    CSipSseTestTls::Storage()->Clear();
    
    iTerminal->iRequestedQuery = NULL;
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );

    options =   
        CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                         KCapabilityTestAddress_B,
                                         KCapabilityTestDataFeature,
                                         KCapabilitySwisFeature,
                                         KCapabilityTestAcceptHeader,
                                         KCapabilityTestTerminalID_B);
    CleanupStack::PushL( options );
    
    iTerminal->QueryRequestedL( *options );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );
    EUNIT_ASSERT( server->iResponseToRequest == 
                            SIPStrings::StringF( SipStrConsts::EOptions ) );
    EUNIT_ASSERT( server->iClientResponse->StatusCode() == 
                            KMUSAVASIPNotImplemented );
    
    CleanupStack::PopAndDestroy( options );   
  

    }

void UT_CMusAvaTerminal::UT_CMusAvaTerminal_IsDiscoveredL(  )
    {
    if ( !iProfile->iArray )
        {
        iProfile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
        iProfile->iArray->AppendL( _L8("sip:user@domain.com") );
        }

    CMusAvaCapabilityQuery* query = NULL;
    query = CMusAvaCapabilityQuery::NewL( *iCapability,
                                            *iSIPConnection,
                                            *iProfile,
                                             KCapabilityTestSIPAddressUri_B );
    CleanupStack::PushL( query );

    EUNIT_ASSERT( iTerminal->Id().Compare( KCapabilityTestTerminalID_B ) 
                == 0 );
    EUNIT_ASSERT( iTerminal->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );
    EUNIT_ASSERT( iTerminal->iQueries == 0 );
    
    EUNIT_ASSERT( iTerminal->IsDiscovered() );

    iTerminal->iQueries = 0;
    iTerminal->iRequestedQuery = iCapability;
    EUNIT_ASSERT( !iTerminal->IsDiscovered() );

    iTerminal->AttachQuery( *query );
    EUNIT_ASSERT( iTerminal->iQueries == 1 );
    iTerminal->iRequestedQuery = NULL;
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );
    
    EUNIT_ASSERT( !iTerminal->IsDiscovered() );

    iTerminal->iRequestedQuery = iCapability;

    EUNIT_ASSERT( iTerminal->iQueries == 1 );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == iCapability );
    EUNIT_ASSERT( !iTerminal->IsDiscovered() );
    
    iTerminal->AttachQuery( *query );
    EUNIT_ASSERT( iTerminal->iQueries == 2 );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == iCapability );
    EUNIT_ASSERT( !iTerminal->IsDiscovered() );

    iTerminal->iRequestedQuery = NULL;
    EUNIT_ASSERT( iTerminal->iQueries == 2 );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );
    EUNIT_ASSERT( !iTerminal->IsDiscovered() );

    iTerminal->iQuery = query;
    CleanupStack::Pop( query );

    EUNIT_ASSERT( iTerminal->iQueries == 2 );
    EUNIT_ASSERT( iTerminal->iQuery == query );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );
    EUNIT_ASSERT( !iTerminal->IsDiscovered() );
    
    iTerminal->DetachQuery( *query );
    
    EUNIT_ASSERT( iTerminal->iQueries == 1 );
    EUNIT_ASSERT( iTerminal->iQuery == query );
    EUNIT_ASSERT( iTerminal->iQuery->State() == 
                        CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );
    EUNIT_ASSERT( !iTerminal->IsDiscovered() );
    
    iTerminal->iRequestedQuery = iCapability;
    EUNIT_ASSERT( iTerminal->iQueries == 1 );
    EUNIT_ASSERT( iTerminal->iQuery == query );
    EUNIT_ASSERT( iTerminal->iQuery->State() == 
            CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == iCapability );
    EUNIT_ASSERT( !iTerminal->IsDiscovered() );
    
    iTerminal->iQuery->SetState( 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    iTerminal->iRequestedQuery = NULL;
    EUNIT_ASSERT( iTerminal->iQueries == 1 );
    EUNIT_ASSERT( iTerminal->iQuery == query );
    EUNIT_ASSERT( iTerminal->iQuery->State() == 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == NULL );
    EUNIT_ASSERT( iTerminal->IsDiscovered() );

    iTerminal->iRequestedQuery = iCapability;
    EUNIT_ASSERT( iTerminal->iQueries == 1 );
    EUNIT_ASSERT( iTerminal->iQuery == query );
    EUNIT_ASSERT( iTerminal->iQuery->State() == 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    EUNIT_ASSERT( iTerminal->RequestedQuery() == iCapability );
    EUNIT_ASSERT( iTerminal->IsDiscovered() );
//    CMusAvaSharedObject::DeleteSingleton();
    }

//  TEST TABLE
EUNIT_BEGIN_TEST_TABLE(
    UT_CMusAvaTerminal,
    "CMusAvaTerminal",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaTerminal",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaTerminal_NewLL, Teardown)

EUNIT_TEST(
    "MatchL - test ",
    "CMusAvaTerminal",
    "MatchL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaTerminal_MatchLL, Teardown)


EUNIT_TEST(
    "ExecuteQueryL - test ",
    "CMusAvaTerminal",
    "ExecuteQueryL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaTerminal_ExecuteQueryLL, Teardown)


EUNIT_TEST(
    "QueryCompletedL - test ",
    "CMusAvaTerminal",
    "QueryCompletedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaTerminal_QueryCompletedLL, Teardown)


EUNIT_TEST(
    "QueryCanceled - test ",
    "CMusAvaTerminal",
    "QueryCanceled",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaTerminal_QueryCanceledL, Teardown)


EUNIT_TEST(
    "QueryRequestedL - test ",
    "CMusAvaTerminal",
    "QueryRequestedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaTerminal_QueryRequestedLL, Teardown)

EUNIT_TEST(
    "IsDiscovered - test ",
    "CMusAvaTerminal",
    "IsDiscovered",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaTerminal_IsDiscoveredL, Teardown)




    
EUNIT_END_TEST_TABLE

//  END OF FILE
