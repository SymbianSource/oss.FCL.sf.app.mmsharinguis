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
#include "UT_CMusAvaCapabilityExchange.h"
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
#include "musavaCapabilityExchange.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "musavacapabilitytesthelper.h"
#include "musavacapability.h"
#include "musavaterminal.h"
#include "CSipSseTestTls.h"
#include "musavaoptionhandler.h"
#include "musavasettingsimp.h"
#include "musavacapabilityquery.h"
#include "mussettings.h"
#include "mussettingskeys.h"
#include "musavaobserverimp.h"
#include "musavasharedobject.h"
#include "musavasip.h"
#include "musavaobserver.h"
// CONSTRUCTION
UT_CMusAvaCapabilityExchange* UT_CMusAvaCapabilityExchange::NewL()
    {
    UT_CMusAvaCapabilityExchange* self = UT_CMusAvaCapabilityExchange::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaCapabilityExchange* UT_CMusAvaCapabilityExchange::NewLC()
    {
    UT_CMusAvaCapabilityExchange* self = new( ELeave ) UT_CMusAvaCapabilityExchange();
    CleanupStack::PushL( self );
	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaCapabilityExchange::~UT_CMusAvaCapabilityExchange()
    {
    }

// Default constructor
UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange()
    {
    }

// Second phase construct
void UT_CMusAvaCapabilityExchange::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

_LIT8( KTestNewTerminal, "new@foo.bar");

void UT_CMusAvaCapabilityExchange::SetupL(  )
    {
    MSIPObserver* dummyObserver = NULL;
    CSIPProfileRegistry* dummyRegistry = NULL;
    MSIPConnectionObserver* dummyConnectionObserver = NULL;
    
    TUid uid = TUid::Uid( 1 );
    TUint32 iapId = 32;
    
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iOptionHandler = CMusAvaOptionHandler::NewL( *iAvailabilityObserver,
                                                 *iConcreteSettings );
    iSIP = CSIP::NewL( uid, *dummyObserver );
    iProfile = CSIPProfile::NewL( dummyRegistry );
    iOptionHandler->iSharedObj->MusAvaSip().iSipProfile = iProfile;                                                
    iSIPConnection = CSIPConnection::NewL( *iSIP, 
                                            iapId,
                                            *dummyConnectionObserver );
    iOptionHandler->iSharedObj->MusAvaSip().iSipConnection = iSIPConnection;
                               
    iExchange = iOptionHandler->iCapabilityExchange;
    
    iCapability = iOptionHandler->iSwisCapability;
    if ( !iProfile->iRegisteredContact )
        {
        iProfile->iRegisteredContact = KRegisteredContact().AllocL();
        }
    
    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
    iStorage->Set( MusSettingsKeys::KFastStartupMode, 
        MusSettingsKeys::EFastModeOff );
    } 

void UT_CMusAvaCapabilityExchange::Teardown(  )
    {
    if ( iStorage )
       {
       iStorage->Clear();
       CSipSseTestTls::Close();
       iStorage = NULL;
       }
       
    delete iOptionHandler;
    delete iAvailabilityObserver;
    delete iConcreteSettings;
    delete iSIP;
    }

void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_NewLL(  )
    {
    EUNIT_ASSERT( iExchange->Capabilities().Count() == 1 );
    EUNIT_ASSERT( iExchange->Terminals().Count() == 0 );
    EUNIT_ASSERT( iExchange->Capabilities()[0] == iCapability );
    }

void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_QueryObserverL(  )
    {
    EUNIT_ASSERT( &iExchange->QueryObserver() == iOptionHandler );
    }

void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_TerminalIdL(  )
    {
    EUNIT_ASSERT( iExchange->TerminalId().Compare( KNullDesC8 ) == 0 );
    }

void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_QueryReceivedL()
    {
    EUNIT_DISABLE_ALLOC_DECORATOR;
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    if ( !iProfile->iArray )
        {
        iProfile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
        iProfile->iArray->AppendL( _L8("sip:user@domain.com") );
        }
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EStandard );
    MultimediaSharingSettings::SetCapabilityQuerySettingL(
                                        MusSettingsKeys::ESequential );
    
    CSIPServerTransaction* request = NULL;
    
    TBufC16<16> str(KCapabilityTestSIPAddressUri_BSIP);
    HBufC16* ptr;
    ptr = str.AllocLC(); 
    CDesCArrayFlat* sipAddress = new( ELeave ) CDesCArrayFlat( 1 );
    CleanupStack::PushL( sipAddress );
    sipAddress->AppendL( *ptr );
    iOptionHandler->iSettings.SetSipAddressesL( *sipAddress );
    sipAddress->Reset();
    CleanupStack::PopAndDestroy( sipAddress );
    CleanupStack::PopAndDestroy(); //ptr   
    EUNIT_ENABLE_ALLOC_DECORATOR;

    EUNIT_ASSERT( iExchange->Capabilities().Count() == 1 );
    EUNIT_ASSERT( iExchange->Capabilities()[0] == iCapability );
    EUNIT_ASSERT( iCapability->Feature().DesC().Compare( KCapabilitySwisFeature ) == 0 );

//ok & No terminal id configured
    EUNIT_DISABLE_ALLOC_DECORATOR;
	iStorage->Clear();
    iExchange->iTerminals.ResetAndDestroy();
	iOptionHandler->iState = MMusAvaObserver::EMusAvaStatusNotExecuted;
    request = CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                                KCapabilityTestAddress_B,
                                                KCapabilitySwisFeature,
                                                KCapabilitySwisFeature,
                                                KCapabilityTestAcceptHeader,
                                                KCapabilityTestTerminalID_B );
    
    iOptionHandler->iCapabilitiesRequestAnswered = EFalse;                                         
    EUNIT_ENABLE_ALLOC_DECORATOR;
    iExchange->QueryReceived( request );
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && 
        iOptionHandler->State() != MMusAvaObserver::EMusAvaStatusOptionsSent )
        {
        User::Leave( KErrNoMemory );
        }
    
    EUNIT_ASSERT( iOptionHandler->State() == MMusAvaObserver::EMusAvaStatusOptionsSent );    
    EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
    EUNIT_ASSERT( iExchange->Terminals()[0]->Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
    EUNIT_ASSERT( iExchange->Terminals()[0]->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
    EUNIT_ASSERT( iExchange->Terminals()[0]->RequestedQuery() );
    EUNIT_ASSERT( !iExchange->Terminals()[0]->IsDiscovered() );

    EUNIT_ASSERT( iStorage->iResponseToRequest == SIPStrings::StringF( SipStrConsts::EOptions ) );
    EUNIT_ASSERT( iStorage->iClientResponse->StatusCode() == KMUSAVASIP200 );
    EUNIT_ASSERT( iStorage->iClientResponse->MessageElements().Content().Length() > 0 );
    EUNIT_ASSERT( iStorage->iClientResponse->MessageElements().Content().Compare( KCapabilityTestSDP264And263 ) == 0 );
        
    EUNIT_ASSERT( iStorage->iClientResponse->MessageElements().UserHeaders().Count() == 3 );
    MUS_HEADER_ASSERT( iStorage->iClientResponse, 0, KCapabilityTestContactHeader_A );
    iStorage->Clear();
    }
    
void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_QueryReceived1L()
    {
    EUNIT_DISABLE_ALLOC_DECORATOR;
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    if ( !iProfile->iArray )
        {
        iProfile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
        iProfile->iArray->AppendL( _L8("sip:user@domain.com") );
        }
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EStandard );
    MultimediaSharingSettings::SetCapabilityQuerySettingL(
                                        MusSettingsKeys::ESequential );
    
    CSIPServerTransaction* request = NULL;
    
//fails
    iStorage->Clear();
    iOptionHandler->iState = MMusAvaObserver::EMusAvaStatusNotExecuted;
    request = CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                                KCapabilityTestAddress_B,
                                                KNullDesC8,//Accepct-Contact
                                                KCapabilitySwisFeature,
                                                KCapabilityTestAcceptHeader,
                                                KCapabilityTestTerminalID_B );
                                         
    EUNIT_ENABLE_ALLOC_DECORATOR;
    iExchange->QueryReceived( request );
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && 
        ( iStorage->iResponseToRequest != SIPStrings::StringF( SipStrConsts::EOptions ) ||
        iExchange->Terminals().Count() < 1 ) )
        {
        User::Leave( KErrNoMemory );
        }

    EUNIT_ASSERT( iStorage->iResponseToRequest == SIPStrings::StringF( SipStrConsts::EOptions ) );
    EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
    EUNIT_ASSERT( iExchange->Terminals()[0]->IsDiscovered() );
    EUNIT_ASSERT( iStorage->iResponseToRequest == SIPStrings::StringF( SipStrConsts::EOptions ) );
    EUNIT_ASSERT( iStorage->iClientResponse->StatusCode() == KMUSAVASIPServerInternalError );
    EUNIT_ASSERT( iStorage->iClientResponse->MessageElements().Content().Length() == 0 );
    
    delete iExchange->iTerminals[ 0 ];
    iExchange->iTerminals.Remove( 0 );
    EUNIT_ASSERT( iExchange->Terminals().Count() == 0 );
    iStorage->Clear();
    }
    
void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_QueryReceived2L()
    {
    EUNIT_DISABLE_ALLOC_DECORATOR;
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    if ( !iProfile->iArray )
        {
        iProfile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
        iProfile->iArray->AppendL( _L8("sip:user@domain.com") );
        }
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EStandard );
    MultimediaSharingSettings::SetCapabilityQuerySettingL(
                                        MusSettingsKeys::ESequential );
    
    CSIPServerTransaction* request = NULL;
    
    TBufC16<16> str(KCapabilityTestSIPAddressUri_BSIP);
    HBufC16* ptr;
    ptr = str.AllocLC(); 
    CDesCArrayFlat* sipAddress = new( ELeave ) CDesCArrayFlat( 1 );
    CleanupStack::PushL( sipAddress );
    sipAddress->AppendL( *ptr );
    iOptionHandler->iSettings.SetSipAddressesL( *sipAddress );
    sipAddress->Reset();
    CleanupStack::PopAndDestroy( sipAddress );
    CleanupStack::PopAndDestroy(); //ptr   
    EUNIT_ENABLE_ALLOC_DECORATOR;

    // cingular case && first OPTIONS request received

    iExchange->iTerminals.ResetAndDestroy();
    iOptionHandler->iCapabilitiesRequestAnswered = EFalse;
    // set cingular variant key on
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EOperatorSpecific );                           
    request = CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                                KCapabilityTestAddress_B,
                                                KCapabilitySwisFeature,
                                                KCapabilitySwisFeature,
                                                KCapabilityTestAcceptHeader,
                                                KCapabilityTestTerminalID_B );
                                                
    iOptionHandler->iState = MMusAvaObserver::EMusAvaStatusNotExecuted;
    iExchange->QueryReceived( request );
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && 
        iOptionHandler->State() != MMusAvaObserver::EMusAvaStatusOptionsSent )
        {
        User::Leave( KErrNoMemory );
        }

    EUNIT_ASSERT( iOptionHandler->State() == MMusAvaObserver::EMusAvaStatusOptionsSent);
    EUNIT_ASSERT( iOptionHandler->CapabilitiesResolvedForCingular() == ETrue );
    MUS_HEADER_ASSERT( iStorage->iClientResponse, 0, KCapabilityTestContactHeader_A );

    iStorage->Clear();
    // cingular case && second OPTIONS request received
    // set cingular variant key on
    EUNIT_DISABLE_ALLOC_DECORATOR;
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EOperatorSpecific );                           


    iExchange->iTerminals.ResetAndDestroy();
    iOptionHandler->iState = MMusAvaObserver::EMusAvaStatusNotExecuted;
    iOptionHandler->iCapabilitiesRequestAnswered = ETrue;

    request = CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                                KCapabilityTestAddress_B,
                                                KCapabilitySwisFeature,
                                                KCapabilitySwisFeature,
                                                KCapabilityTestAcceptHeader,
                                                KCapabilityTestTerminalID_B );
    EUNIT_ENABLE_ALLOC_DECORATOR;
    iExchange->QueryReceived( request );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && 
        iStorage->iClientResponse->MessageElements().UserHeaders().Count() == 0 )
        {
        User::Leave( KErrNoMemory );
        }
        
    EUNIT_ASSERT( iOptionHandler->CapabilitiesResolvedForCingular() == ETrue );
    EUNIT_ASSERT( MultimediaSharingSettings::OperatorVariantSettingL() ==
            MusSettingsKeys::EOperatorSpecific );
    EUNIT_ASSERT( iOptionHandler->State() == MMusAvaObserver::EMusAvaStatusNotExecuted );
    MUS_HEADER_ASSERT( iStorage->iClientResponse, 0, KCapabilityTestContactHeaderNoFeaturetag_A );
    }
    
void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_CancelQueryL()
    {
    EUNIT_DISABLE_ALLOC_DECORATOR;
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
    EUNIT_ENABLE_ALLOC_DECORATOR;
    iExchange->CancelQuery( err, *query->iTrx );
    EUNIT_ASSERT( query->State() == 
                CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted );
    }
  
void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_AddCapabilityLL(  )
    {
    TInt index = KErrNotFound;
    CMusAvaCapability* capability = NULL;
    
        for ( TInt i=0; i < iExchange->iCapabilities.Count() && index == KErrNotFound; i++ )
        {
        capability = iExchange->iCapabilities[i];
        if ( capability->Feature() == iCapability->Feature() )
            {
            index = i;
            }
        }
    EUNIT_ASSERT( index == 0 );
    }

void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_TerminalLL()
    {
    EUNIT_ASSERT( iExchange->Terminals().Count() == 0 );

    CSIPServerTransaction* request = NULL;
    
    for( TInt i = 0;i<6;i++ )
        {
        
        if ( i == 0 || i == 1 )
            {
                
            request = 
                CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                                 KCapabilityTestAddress_B,
                                                 KCapabilitySwisFeature,
                                                 KCapabilitySwisFeature,
                                                 KCapabilityTestAcceptHeader,
                                                 KNullDesC8 );
            }
        else if ( i == 2 || i == 3 )
            {
            request = 
                CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                                 KCapabilityTestAddress_B,
                                                 KCapabilitySwisFeature,
                                                 KCapabilitySwisFeature,
                                                 KCapabilityTestAcceptHeader,
                                                 KCapabilityTestTerminalID_B );
            }
        else if ( i == 4 )
            {
            request = 
                CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                                 KCapabilityTestAddress_A,//OK
                                                 KCapabilitySwisFeature,
                                                 KCapabilitySwisFeature,
                                                 KCapabilityTestAcceptHeader,
                                                 KNullDesC8 );
            }
        else if ( i == 5 )
            {
            request = 
                CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                                 KCapabilityTestAddress_A,//OK
                                                 KCapabilitySwisFeature,
                                                 KCapabilitySwisFeature,
                                                 KCapabilityTestAcceptHeader,
                                                 KCapabilityTestTerminalID_A );
            }

        CleanupStack::PushL( request );
        
        // Next function call creates a terminal for capability exchage
        iExchange->TerminalL( *request );
        
        if ( i == 0 )
            {
            // Next function has the same uri as the already created terminal has, it should not create the new one
            iExchange->TerminalL( KCapabilityTestAddress_B );
            EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Id().Compare( KNullDesC8 ) == 0 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
            }
        else if ( i == 1 )
            {
            // Next function has the same uri as the already created terminal has, it should not create the new one
            iExchange->TerminalL( KCapabilityTestAddress_B );
            EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Id().Compare( KNullDesC8 ) == 0 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
            }
        else if ( i == 2 )
            {
            // Next function has the same uri as the already created terminal has, it should not create the new one
            iExchange->TerminalL( KCapabilityTestAddress_B );
            EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
            }
        else if ( i == 3 )
            {
            // Next function has the same uri as the already created terminal has, it should not create the new one
            iExchange->TerminalL( KCapabilityTestAddress_B );
            EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
            }
        else if ( i == 4 )
            {
            // Next function has the same uri as the already created terminal has, it should not create the new one
            iExchange->TerminalL( KCapabilityTestAddress_B );
            EUNIT_ASSERT( iExchange->Terminals().Count() == 2 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
            EUNIT_ASSERT( iExchange->Terminals()[1]->Id().Compare( KNullDesC8 ) == 0 );
            EUNIT_ASSERT( iExchange->Terminals()[1]->Uri().Compare( KCapabilityTestAddress_A ) == 0 );
            }
        else if ( i == 5 )
            {
            // Next function has the same uri as the already created terminal has, it should not create the new one
            iExchange->TerminalL( KCapabilityTestAddress_B );
            EUNIT_ASSERT( iExchange->Terminals().Count() == 2 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
            EUNIT_ASSERT( iExchange->Terminals()[0]->Uri().Compare( KCapabilityTestAddress_B ) == 0 );
            EUNIT_ASSERT( iExchange->Terminals()[1]->Id().Compare( KCapabilityTestTerminalID_A ) == 0 );
            EUNIT_ASSERT( iExchange->Terminals()[1]->Uri().Compare( KCapabilityTestAddress_A ) == 0 );
            }

        CleanupStack::PopAndDestroy( request );

        }
    
    }

void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_RejectQueryLL()
    {
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CSIPServerTransaction* request = NULL;
    
    iStorage->Clear();
    
    request = 
        CapabilityTestHelper::OptionsRequestL( KCapabilityTestAddress_A,
                                         KCapabilityTestAddress_B,
                                         KCapabilitySwisFeature,
                                         KCapabilitySwisFeature,
                                         KCapabilityTestAcceptHeader,
                                         KNullDesC8 );


    CleanupStack::PushL( request );
    EUNIT_ENABLE_ALLOC_DECORATOR;

    iExchange->RejectQueryL( *request, KMUSAVASIPNotImplemented );
    EUNIT_ASSERT( iStorage->iResponseToRequest == SIPStrings::StringF( SipStrConsts::EOptions ) );
    EUNIT_ASSERT( iStorage->iClientResponse->StatusCode() == KMUSAVASIPNotImplemented );
    EUNIT_ASSERT( iStorage->iClientResponse->ReasonPhrase() == SIPStrings::StringF( SipStrConsts::EEmpty ) );
    EUNIT_ASSERT( iStorage->iClientResponse->MessageElements().Content().Length() == 0 );

    iStorage->Clear();

    iExchange->RejectQueryL( *request, KErrGeneral );
    EUNIT_ASSERT( iStorage->iResponseToRequest == SIPStrings::StringF( SipStrConsts::EOptions ) );
    EUNIT_ASSERT( iStorage->iClientResponse->StatusCode() == KMUSAVASIPServerInternalError );
    EUNIT_ASSERT( iStorage->iClientResponse->ReasonPhrase() == SIPStrings::StringF( SipStrConsts::EPhraseServerInternalError ) );
    EUNIT_ASSERT( iStorage->iClientResponse->MessageElements().Content().Length() == 0 );

    CleanupStack::PopAndDestroy( request );
    }

void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_FindByFeatureL()
    {
    EUNIT_ASSERT( iExchange->FindByFeature(*iCapability) == 0 );
    }

void UT_CMusAvaCapabilityExchange::UT_CMusAvaCapabilityExchange_AnswerToQueryReceivedL(  )
    {
    if ( !iProfile->iArray )
        {
        iProfile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
        iProfile->iArray->AppendL( _L8("sip:user@domain.com") );
        }
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EStandard );

    EUNIT_ASSERT( iExchange->Capabilities().Count() == 1 );
    EUNIT_ASSERT( iExchange->Capabilities()[0] == iCapability );
    EUNIT_ASSERT( iCapability->Feature().DesC().Compare( KCapabilitySwisFeature ) == 0 );
    
    CMusAvaCapabilityQuery* query = NULL;
    
    
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

        //iExchange->iTerminal
        iExchange->AnswerToQueryReceived( *query->iTrx );
        TInt count = NULL;
        EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
        if ( count > 0 && 
            query->Terminal().Id().Length() == 0 )
            {
            User::Leave( KErrNoMemory );
            }
        if ( count > 0 && 
            i == 1 && 
            query->Terminal().iQuery->Result() != KCapabilityCapabilitesNotFound )
            {
            User::Leave( KErrNoMemory );
            }
        if ( count > 0 && 
            i == 2 && 
            !query->Retrying() )
            {
            User::Leave( KErrNoMemory );
            }
        
        EUNIT_ASSERT( iExchange->Terminals().Count() == 1 );
        EUNIT_ASSERT( query->Terminal().Id().Length() > 0 );
        EUNIT_ASSERT( query->Terminal().Id().Compare( KCapabilityTestTerminalID_B ) == 0 );
        uri16 = EscapeUtils::ConvertToUnicodeFromUtf8L( query->Terminal().Uri() );
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
            
            CapabilityTestHelper::SetResponseL( *query->iTrx, KMUSAVASIPNotFound, SipStrConsts::EPhraseNotFound,
                                          KCapabilityTestAddress_B,
                                          KNullDesC8,
                                          KNullDesC8,
                                          KNullDesC8,
                                          KNullDesC8 );
            iExchange->AnswerToQueryReceived( *query->iTrx );
            EUNIT_ASSERT( query->Terminal().iQuery == query );
            EUNIT_ASSERT( query->Terminal().iQuery->Result() == KCapabilityCapabilitesNotFound );
            EUNIT_ASSERT( query->Terminal().IsDiscovered() );
            EUNIT_ASSERT( !query->Retrying() );
            EUNIT_ASSERT( query->iTimer == NULL );

            }
        }
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EStandard );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaCapabilityExchange,
    "CMusAvaCapabilityExchange",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaCapabilityExchange",
    "NewL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_NewLL, Teardown)
    
EUNIT_TEST(
    "QueryObserverL - test ",
    "CMusAvaCapabilityExchange",
    "QueryObserver",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_QueryObserverL, Teardown)        

EUNIT_TEST(
    "TerminalId - test ",
    "CMusAvaCapabilityExchange",
    "TerminalId",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_TerminalIdL, Teardown)
    

EUNIT_TEST(
    "QueryReceivedL - test ",
    "CMusAvaCapabilityExchange",
    "QueryReceivedL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_QueryReceivedL, Teardown)

EUNIT_TEST(
    "QueryReceivedL1 - test ",
    "CMusAvaCapabilityExchange",
    "QueryReceivedL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_QueryReceived1L, Teardown)

EUNIT_TEST(
    "QueryReceivedL2 - test ",
    "CMusAvaCapabilityExchange",
    "QueryReceivedL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_QueryReceived2L, Teardown)

EUNIT_TEST(
    "CancelQuery - test ",
    "CMusAvaCapabilityExchange",
    "CancelQuery",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_CancelQueryL, Teardown)   
     
 EUNIT_TEST(
    "AddCapability - test ",
    "CMusAvaCapabilityExchange",
    "AddCapability",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_AddCapabilityLL, Teardown)           
         
 EUNIT_TEST(
    "TerminalL - test ",
    "CMusAvaCapabilityExchange",
    "TerminalL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_TerminalLL, Teardown)           
         
 EUNIT_TEST(
    "RejectQueryL - test ",
    "CMusAvaCapabilityExchange",
    "RejectQueryL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_RejectQueryLL, Teardown)           
         
 EUNIT_TEST(
    "FindByFeature - test ",
    "CMusAvaCapabilityExchange",
    "FindByFeature",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_FindByFeatureL, Teardown)           
         
 EUNIT_TEST(
    "AnswerToQueryReceived - test ",
    "CMusAvaCapabilityExchange",
    "AnswerToQueryReceived",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaCapabilityExchange_AnswerToQueryReceivedL, Teardown)           
         
EUNIT_END_TEST_TABLE

//  END OF FILE
