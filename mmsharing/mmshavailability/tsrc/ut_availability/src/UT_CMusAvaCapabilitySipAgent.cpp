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
#include "UT_CMusAvaCapabilitySipAgent.h"
#include "musavasettingsimp.h"
#include "musavaobserverimp.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "muslogger.h"



//  EXTERNAL INCLUDES
#include <EscapeUtils.h>
#include <digia/eunit/eunitmacros.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <stringpool.h>
#include <sipservertransaction.h>
#include <siptransactionbase.h>
#include <digia/eunit/eunitdecorators.h>
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


// CONSTRUCTION
UT_CMusAvaCapabilitySipAgent* UT_CMusAvaCapabilitySipAgent::NewL()
    {
    UT_CMusAvaCapabilitySipAgent* self = UT_CMusAvaCapabilitySipAgent::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaCapabilitySipAgent* UT_CMusAvaCapabilitySipAgent::NewLC()
    {
    UT_CMusAvaCapabilitySipAgent* self = new( ELeave ) UT_CMusAvaCapabilitySipAgent();
    CleanupStack::PushL( self );
	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaCapabilitySipAgent::~UT_CMusAvaCapabilitySipAgent()
    {
    }

// Default constructor
UT_CMusAvaCapabilitySipAgent::UT_CMusAvaCapabilitySipAgent()
    {
    }

// Second phase construct
void UT_CMusAvaCapabilitySipAgent::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMusAvaCapabilitySipAgent::SetupL(  )
    {
    iSharedObject = CMusAvaSharedObject::GetSingletonL();
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iOptionHandler = CMusAvaOptionHandler::NewL( *iAvailabilityObserver,
                                                 *iConcreteSettings );
    iExchange = iOptionHandler->iCapabilityExchange;
    iSipAgent = CMusAvaCapabilitySipAgent::NewL( *iExchange, 
                                                *iOptionHandler->iSharedObj,
                                                 iOptionHandler->iObserver);
    
    iCapability = iOptionHandler->iSwisCapability;

    MSIPConnectionObserver& connectionObserver = 
                            iSharedObject->MusAvaSip().ConnectionObserver();
    
    CSIP& sip = iSharedObject->MusAvaSip().Sip();
    iSharedObject->MusAvaSip().CreateProfileL();
    iProfile = iSharedObject->MusAvaSip().Profile();
    TUint32 iapId;
    TInt err = iProfile->GetParameter( KSIPAccessPointId, iapId );
    iSIPConnection = CSIPConnection::NewL( sip, 
                                           iapId,
                                           connectionObserver );

    iSharedObject->MusAvaSip().ConnectionL(); // Forced connection creation

    if ( !iProfile->iArray )
        {
        iProfile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
        iProfile->iArray->AppendL( _L8("sip:user@domain.com") );
        }
    if ( !iProfile->iRegisteredContact )
        {
        iProfile->iRegisteredContact = KRegisteredContact().AllocL();
        }

    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
    iStorage->Set( MusSettingsKeys::KFastStartupMode, 
        MusSettingsKeys::EFastModeOff );
    
    iProfileId = MultimediaSharingSettings::SipProfileSettingL();
    } 

void UT_CMusAvaCapabilitySipAgent::Teardown(  )
    {
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
    }

void UT_CMusAvaCapabilitySipAgent::UT_CMusAvaCapabilitySipAgent_NewLL(  )
    {
    EUNIT_ASSERT( iSipAgent );
    }

void UT_CMusAvaCapabilitySipAgent::UT_CMusAvaCapabilitySipAgent_ExecuteCapabilityQueryLL(  )
    {
    EUNIT_ASSERT( iSipAgent );
    EUNIT_ASSERT( iExchange->Terminals().Count() == 0 );
    
    TInt ret = NULL;
    TRAPD ( error, ret = iSharedObject->MusAvaSip().CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    
    iSipAgent->ExecuteCapabilityQueryL( *iCapability, 
                                        KCapabilityTestSIPAddressUri_B );
    EUNIT_ASSERT( iStorage->iClientRequest->Method() == 
                            SIPStrings::StringF( SipStrConsts::EOptions ) );
                            
    EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().Content().Length() > 0 );
    EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().ContentType() );
    EUNIT_ASSERT( iStorage->iClientRequest->RemoteUri().Uri().UriDes().Compare( 
            KCapabilityTestSIPAddressUri8_B ) == 0);
    EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().UserHeaders().Count() == 6 );
    MUS_HEADER_ASSERT( iStorage->iClientRequest, 0, KCapabilityTestAcceptContactHeader );
    MUS_HEADER_ASSERT( iStorage->iClientRequest, 5, KCapabilityTestContactHeader_A1Updated );
    MUS_HEADER_ASSERT( iStorage->iClientRequest, 3, KCapabilityTestAcceptHeader );
    }

void UT_CMusAvaCapabilitySipAgent::UT_CMusAvaCapabilitySipAgent_IncomingRequestL(  )
    {
    
    CSIPServerTransaction* request = CapabilityTestHelper::OptionsRequestL( 
                                                KCapabilityTestAddress_A,
                                                KCapabilityTestAddress_B,
                                                KCapabilitySwisFeature,
                                                KCapabilitySwisFeature,
                                                KCapabilityTestAcceptHeader,
                                                KCapabilityTestTerminalID_B );
                                                
    TUint32 iapId = iSIPConnection->IapId();

    /*EUNIT_ASSERT( iSipAgent->IncomingRequest( iapId, request ) == KErrNone );

    EUNIT_ASSERT( server->iResponseToRequest == SIPStrings::StringF( SipStrConsts::EOptions ) );
    EUNIT_ASSERT( server->iClientResponse->StatusCode() == KMUSAVASIP200 );
    EUNIT_ASSERT( server->iClientResponse->MessageElements().Content().Length() > 0 );
    EUNIT_ASSERT( server->iClientResponse->MessageElements().Content().Compare( KCapabilityTestSDP ) == 0 );
        
    EUNIT_ASSERT( server->iClientResponse->MessageElements().UserHeaders().Count() == 1 );
    MUS_HEADER_ASSERT( server->iClientResponse, 0, KCapabilityTestContactHeader_A );
    */
    EUNIT_ASSERT( iSipAgent->IncomingRequest( iapId, request ) == KErrNotFound );
    delete request;
    
    EUNIT_ASSERT( !iOptionHandler->iCapabilityQueryAnswered );  
    iOptionHandler->iObserver.AvailabilityChanged(MMusAvaObserver::EMusAvaOptionHandler,
            MMusAvaObserver::EMusAvaStatusCallOnHold);
    request = CapabilityTestHelper::OptionsRequestL( 
                                                KCapabilityTestAddress_A,
                                                KCapabilityTestAddress_B,
                                                KCapabilitySwisFeature,
                                                KCapabilitySwisFeature,
                                                KCapabilityTestAcceptHeader,
                                                KCapabilityTestTerminalID_B );    
    EUNIT_ASSERT( iSipAgent->IncomingRequest( request ) == KErrNotFound );
    delete request;
    
    iOptionHandler->iObserver.AvailabilityChanged(MMusAvaObserver::EMusAvaOptionHandler,
                MMusAvaObserver::EMusAvaStatusOptionsNotSent);
    request = CapabilityTestHelper::OptionsRequestL( 
                                                 KCapabilityTestAddress_A,
                                                 KCapabilityTestAddress_B,
                                                 KCapabilitySwisFeature,
                                                 KCapabilitySwisFeature,
                                                 KCapabilityTestAcceptHeader,
                                                 KCapabilityTestTerminalID_B );    
    EUNIT_ASSERT( iSipAgent->IncomingRequest( request ) == KErrNone );   

    iOptionHandler->iCapabilityQueryAnswered = ETrue; 
    EUNIT_ASSERT( iOptionHandler->iCapabilityQueryAnswered );

    request = CapabilityTestHelper::OptionsRequestL( 
                                                KCapabilityTestAddress_A,
                                                KCapabilityTestAddress_B,
                                                KCapabilitySwisFeature,
                                                KCapabilitySwisFeature,
                                                KCapabilityTestAcceptHeader,
                                                KCapabilityTestTerminalID_B );
    
    EUNIT_ASSERT( iSipAgent->IncomingRequest( request ) == KErrNotFound ); 
    delete request;
    
    CSIPServerTransaction* transaction = 
                CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    iStorage->SetError( KErrGeneral );
    EUNIT_ASSERT( iSipAgent->IncomingRequest( transaction ) == KErrNone );   
    iStorage->SetError( KErrNone );
    
    
    
    
    }

void UT_CMusAvaCapabilitySipAgent::UT_CMusAvaCapabilitySipAgent_HandleIncomingRequestLL(  )
    {
    CSIPServerTransaction* transaction = 
    CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    CleanupStack::PushL( transaction );
    transaction->iIsClientTransaction = ETrue;
    transaction->iState = CSIPTransactionBase::ETrying; 
    
    TUriParser8 Uri;
    User::LeaveIfError( Uri.Parse( KCapabilityTestAddress_A ) );
    
  	CUri8* uri = CUri8::NewLC( Uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    transaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );
    
    iSipAgent->HandleIncomingRequestL( *transaction );
    //iSipAgent->IncomingRequest( transaction );
    
    EUNIT_ASSERT( iStorage->iClientResponse->ReasonPhrase() == 
                SIPStrings::StringF( SipStrConsts::EPhraseNotImplemented ) );
    EUNIT_ASSERT( iStorage->iClientResponse->StatusCode() == KMUSAVASIPNotImplemented );
    CleanupStack::PopAndDestroy( transaction );
    
    }

void UT_CMusAvaCapabilitySipAgent::UT_CMusAvaCapabilitySipAgent_IncomingResponseL(  )
    {
    CMusAvaCapabilityQuery* query = NULL;
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EStandard );
//init
    for( TInt i = 0; i<5;i++ )
        {
        query = CMusAvaCapabilityQuery::NewL( *iCapability,
                                            *iSIPConnection,
                                            *iProfile,
                                            KCapabilityTestSIPAddressUri_B );
        CleanupStack::PushL( query );

        EUNIT_ASSERT( query->Result() ==  KCapabilityQueryNotReady );
        EUNIT_ASSERT( query->State() == CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
        EUNIT_ASSERT( !query->iTimer );
        EUNIT_ASSERT( !query->iTrx );
        
        EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
        if ( i == 0 )
            {
            EUNIT_ASSERT( query->Terminal().Id().Length() == 0 );
            }
        else
            {
            EUNIT_ASSERT( query->Terminal().Id().Length() > 0 );
            EUNIT_ASSERT( query->Terminal().Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
            }
        
        EUNIT_ASSERT( !query->Terminal().RequestedQuery() );
        EUNIT_ASSERT( !query->Terminal().IsDiscovered() );
        
        // Since several queries cannot be executed, we delete the old one... 
        delete query->iTerminal->iQuery;
        query->iTerminal->iQuery = NULL;
        
        // ...before execution
        query->Terminal().ExecuteQueryL( query );
        CleanupStack::Pop( query );

        EUNIT_ASSERT( query->Terminal().iQuery == query );
        EUNIT_ASSERT( query->Result() ==  KCapabilityQueryNotReady );
        EUNIT_ASSERT( query->State() == CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );
        EUNIT_ASSERT( !query->iTimer );
        EUNIT_ASSERT( query->iTrx );
        
        EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
        if ( i == 0 )
            {
            EUNIT_ASSERT( query->Terminal().Id().Length() == 0 );
            }
        else
            {
            EUNIT_ASSERT( query->Terminal().Id().Length() > 0 );
            EUNIT_ASSERT( query->Terminal().Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
            }
        
        
        HBufC* uri16 = EscapeUtils::ConvertToUnicodeFromUtf8L( query->Terminal().Uri() );
        EUNIT_ASSERT( uri16->Compare( KCapabilityTestSIPAddressUri_B ) == 0 );
        delete uri16;
        uri16 = NULL;
        EUNIT_ASSERT( !query->Terminal().RequestedQuery() );
        EUNIT_ASSERT( !query->Terminal().IsDiscovered() );

        EUNIT_ASSERT( iStorage->iClientRequest->Method() == SIPStrings::StringF( SipStrConsts::EOptions ) );
        EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().Content().Length() > 0 );
        EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().ContentType() );
        EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().UserHeaders().Count() == 6 );
        MUS_HEADER_ASSERT( iStorage->iClientRequest, 0, KCapabilityTestAcceptContactHeader );
        MUS_HEADER_ASSERT( iStorage->iClientRequest, 5, KCapabilityTestContactHeader_A1Updated );
        MUS_HEADER_ASSERT( iStorage->iClientRequest, 3, KCapabilityTestAcceptHeader );
    
    //set response
        //    GSMA     Cingular
        if ( i == 0 || i == 3 )//200 OK FT
            {
            CapabilityTestHelper::SetResponseL( *query->iTrx, KMUSAVASIP200, SipStrConsts::EPhraseOk,
                                          KCapabilityTestAddress_B,
                                          KCapabilitySwisFeature,
                                          KCapabilityTestTerminalID_B,
                                          KPAssertedIdentity_B,
                                          KCapabilityTestSDP );
            }
        //         GSMA     Cingular
        else if ( i == 1 || i == 4 )//200 OK NO FT
            {
            CapabilityTestHelper::SetResponseL( *query->iTrx, KMUSAVASIP200, SipStrConsts::EPhraseOk,
                                          KCapabilityTestAddress_B,
                                          KNullDesC8,
                                          KCapabilityTestTerminalID_B,
                                          KPAssertedIdentity_B,
                                          KCapabilityTestSDP );
            
            }
        else if ( i == 2 )//GSMA & 404
            {
            CapabilityTestHelper::SetResponseL( *query->iTrx, KMUSAVASIPNotFound, SipStrConsts::EPhraseNotFound,
                                          KCapabilityTestAddress_B,
                                          KNullDesC8,
                                          KNullDesC8,
                                          KNullDesC8,
                                          KNullDesC8 );
            
            }

        if ( i == 4 )
            {
            EUNIT_ASSERT( 
                MultimediaSharingSettings::OperatorVariantSettingL() ==
                                                MusSettingsKeys::EStandard );
                // set cingular variant key on
            MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EOperatorSpecific );
            EUNIT_ASSERT( MultimediaSharingSettings::OperatorVariantSettingL()
                        == MusSettingsKeys::EOperatorSpecific );
            }

        iSipAgent->IncomingResponse( *query->iTrx );
        
        EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
        TInt count = 0;
        EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
        if ( count == 0 )
            {
            EUNIT_ASSERT( query->Terminal().Id().Length() > 0 );
            EUNIT_ASSERT( query->Terminal().Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
            uri16 = EscapeUtils::ConvertToUnicodeFromUtf8L( query->Terminal().Uri() );
            //EUNIT_ASSERT( uri16->Compare( iOptionHandler->SipAddressProposal() ) == 0 );
            delete uri16;
            uri16 = NULL;
            EUNIT_ASSERT( !query->Terminal().RequestedQuery() );


            if ( i == 0 || i == 3 )//200 OK FT
                {
                EUNIT_ASSERT( 
                    MultimediaSharingSettings::OperatorVariantSettingL() ==
                                                MusSettingsKeys::EStandard );
                EUNIT_ASSERT( query->Terminal().iQuery->State() == CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
                EUNIT_ASSERT( query->Terminal().IsDiscovered() );
                EUNIT_ASSERT( !query->Retrying() );
                EUNIT_ASSERT( query->iTimer == NULL );
                }
            else if ( i == 1 )//GSMA & 200 OK NO FT
                {
                EUNIT_ASSERT( 
                    MultimediaSharingSettings::OperatorVariantSettingL() ==
                                                MusSettingsKeys::EStandard );
                EUNIT_ASSERT( query->Terminal().iQuery == query );
                EUNIT_ASSERT( query->Terminal().iQuery->Result() == KCapabilityCapabilitesNotFound );
                EUNIT_ASSERT( query->Terminal().IsDiscovered() );
                EUNIT_ASSERT( !query->Retrying() );
                EUNIT_ASSERT( query->iTimer == NULL );
                }
            else if ( i == 4 )//( not yet Cingular) & 200 OK NO FT
                {
                EUNIT_ASSERT( MultimediaSharingSettings::OperatorVariantSettingL()
                        == MusSettingsKeys::EOperatorSpecific );
                MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EStandard );
                EUNIT_ASSERT( query->Terminal().iQuery == query );
                EUNIT_ASSERT( query->Terminal().iQuery->Result() == KCapabilityCapabilitesNotFound );
                EUNIT_ASSERT( query->Terminal().IsDiscovered() );
                EUNIT_ASSERT( !query->Retrying() );
                EUNIT_ASSERT( query->iTimer == NULL );
                }
            else if ( i == 2 )  //GSMA & 404
                {
                EUNIT_ASSERT( 
                    MultimediaSharingSettings::OperatorVariantSettingL() ==
                                                    MusSettingsKeys::EStandard );
                EUNIT_ASSERT( !query->Terminal().iQuery == NULL );
                EUNIT_ASSERT( !query->Terminal().IsDiscovered() );
                EUNIT_ASSERT( query->Retrying() );
                EUNIT_ASSERT( query->iTimer != NULL );
            
                query->OnExpiredL( KErrNone );
                EUNIT_ASSERT( iStorage->iClientRequest->Method() == SIPStrings::StringF( SipStrConsts::EOptions ) );
                EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().Content().Length() > 0 );
                EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().ContentType() );
                EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().UserHeaders().Count() == 6 );
                MUS_HEADER_ASSERT( iStorage->iClientRequest, 0, KCapabilityTestAcceptContactHeader );
                MUS_HEADER_ASSERT( iStorage->iClientRequest, 5, KCapabilityTestContactHeader_A1Updated );
                MUS_HEADER_ASSERT( iStorage->iClientRequest, 3, KCapabilityTestAcceptHeader );
        
                CapabilityTestHelper::SetResponseL( *query->iTrx, KMUSAVASIPNotFound, SipStrConsts::EPhraseNotFound,
                                          KCapabilityTestAddress_B,
                                          KNullDesC8,
                                          KNullDesC8,
                                          KNullDesC8,
                                          KNullDesC8 );
                //iExchange->AnswerToQueryReceived( *query->iTrx );
                iSipAgent->IncomingResponse( *query->iTrx );
                EUNIT_ASSERT( query->Terminal().iQuery == query );
                EUNIT_ASSERT( query->Terminal().iQuery->Result() == KCapabilityCapabilitesNotFound );
                EUNIT_ASSERT( query->Terminal().IsDiscovered() );
                EUNIT_ASSERT( !query->Retrying() );
                EUNIT_ASSERT( query->iTimer == NULL );
                }
            }
        else
            {
            break;    
            }
        }
    }

void UT_CMusAvaCapabilitySipAgent::UT_CMusAvaCapabilitySipAgent_ErrorOccuredL(  )
    {
    CMusAvaCapabilityQuery* query = NULL;
    
    query = CMusAvaCapabilityQuery::NewL( *iCapability,
                                            *iSIPConnection,
                                            *iProfile,
                                            KCapabilityTestSIPAddressUri_B );
    CleanupStack::PushL( query );

    EUNIT_ASSERT( query->Result() ==  KCapabilityQueryNotReady );
    EUNIT_ASSERT( query->State() == 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryCreated );
    EUNIT_ASSERT( !query->iTimer );
    EUNIT_ASSERT( !query->iTrx );
        
    EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
    EUNIT_ASSERT( query->Terminal().Id().Length() == 0 );
    EUNIT_ASSERT( !query->Terminal().RequestedQuery() );
    EUNIT_ASSERT( !query->Terminal().IsDiscovered() );
        
    query->Terminal().ExecuteQueryL( query );
    CleanupStack::Pop( query );

    EUNIT_ASSERT( query->Terminal().iQuery == query );
    EUNIT_ASSERT( query->Result() ==  KCapabilityQueryNotReady );
    EUNIT_ASSERT( query->State() == 
                    CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting );
    EUNIT_ASSERT( !query->iTimer );
    EUNIT_ASSERT( query->iTrx );

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
    TInt err = 404;
    EUNIT_ASSERT( iSipAgent->ErrorOccured( err, *query->iTrx ) == KErrNone );
    EUNIT_ASSERT( query->State() == 
                CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
                
    CSIPServerTransaction* transaction = 
                CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );                
    EUNIT_ASSERT( iSipAgent->ErrorOccured( err, *transaction ) == KErrNone );                
    delete transaction;
    }
    
void UT_CMusAvaCapabilitySipAgent::UT_CMusAvaCapabilitySipAgent_TimedOutL()    
    {
    CSIPServerTransaction* request = CapabilityTestHelper::OptionsRequestL( 
                                            KCapabilityTestAddress_A,
                                            KCapabilityTestAddress_B,
                                            KCapabilitySwisFeature,
                                            KCapabilitySwisFeature,
                                            KCapabilityTestAcceptHeader,
                                            KCapabilityTestTerminalID_B );
    EUNIT_ASSERT( iSipAgent->TimedOut( *request ) == KErrNotSupported );
    delete request;
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaCapabilitySipAgent,
    "CMusAvaCapabilitySipAgent",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaCapabilitySipAgent",
    "NewL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilitySipAgent_NewLL, Teardown)
    
EUNIT_TEST(
    "ExecuteCapabilityQueryL - test ",
    "CMusAvaCapabilitySipAgent",
    "ExecuteCapabilityQueryL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilitySipAgent_ExecuteCapabilityQueryLL, Teardown)
    
EUNIT_TEST(
    "IncomingRequest - test ",
    "CMusAvaCapabilitySipAgent",
    "IncomingRequest",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilitySipAgent_IncomingRequestL, Teardown)

EUNIT_TEST(
    "HandleIncomingRequestL - test ",
    "CMusAvaCapabilitySipAgent",
    "HandleIncomingRequestL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilitySipAgent_HandleIncomingRequestLL, Teardown)

EUNIT_TEST(
    "IncomingResponse - test ",
    "CMusAvaCapabilitySipAgent",
    "IncomingResponse",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilitySipAgent_IncomingResponseL, Teardown)

EUNIT_TEST(
    "ErrorOccured - test ",
    "CMusAvaCapabilitySipAgent",
    "ErrorOccured",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilitySipAgent_ErrorOccuredL, Teardown)
         
EUNIT_TEST(
    "TimedOut - test ",
    "CMusAvaCapabilitySipAgent",
    "TimedOut",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilitySipAgent_TimedOutL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
