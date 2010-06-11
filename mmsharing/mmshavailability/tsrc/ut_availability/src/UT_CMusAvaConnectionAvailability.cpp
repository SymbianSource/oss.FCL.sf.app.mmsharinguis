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
#include "UT_CMusAvaConnectionAvailability.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "musavaavailability.h"
#include "muslogger.h"
#include "musavaobserver.h"
#include "musavaobserverimp.h"
#include "musavadefaultimp.h"
#include "musavaobserverimp.h"
#include "musavasettingsimp.h"
#include "musavaconnectionmonitor.h"
#include "musavasharedobject.h"
#include "musavasip.h"
#include "mussettingskeys.h"
#include "mussettings.h"
#include "CSipSseTestTls.h"
#include "musavanetworkregistrationstatus.h"
#include "musavacapabilitytesthelper.h" //remove compilation warning


//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
//#include <telephonyinternalpskeys.h>
#include <etelmm.h>
#include <rconnmon.h>
#include <nifvar.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <digia/eunit/eunitdecorators.h>


//  INTERNAL INCLUDES
#include "musavaconnectionavailability.h"



// CONSTRUCTION
UT_CMusAvaConnectionAvailability* UT_CMusAvaConnectionAvailability::NewL()
    {
    UT_CMusAvaConnectionAvailability* self = UT_CMusAvaConnectionAvailability::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaConnectionAvailability* UT_CMusAvaConnectionAvailability::NewLC()
    {
    UT_CMusAvaConnectionAvailability* self = new( ELeave ) UT_CMusAvaConnectionAvailability();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaConnectionAvailability::~UT_CMusAvaConnectionAvailability()
    {
    }

// Default constructor
UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability()
    {
    }

// Second phase construct
void UT_CMusAvaConnectionAvailability::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaConnectionAvailability::SetupL(  )
    {
    SIPStrings::OpenL();
    CSipSseTestTls::OpenL();
    iProfileId = MultimediaSharingSettings::SipProfileSettingL();    
    iStorage = CSipSseTestTls::Storage();
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iConnectionAvailability  = 
    CMusAvaConnectionAvailability::NewL( *iAvailabilityObserver, *iConcreteSettings );
                                    
    iSharedObject = CMusAvaSharedObject::GetSingletonL();	
    } 

void UT_CMusAvaConnectionAvailability::Teardown(  )
    {
    delete iAvailabilityObserver;
    iAvailabilityObserver = NULL;
    delete iConcreteSettings;
    iConcreteSettings = NULL;
    delete iConnectionAvailability;
    iConnectionAvailability = NULL;
  
    if ( iStorage )
        {
        iStorage->Clear();
        CSipSseTestTls::Close();
        iStorage = NULL;
        }
    CMusAvaSharedObject::DeleteSingleton();
    SIPStrings::Close();
    } 

void UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability_NewLL()
    {
    EUNIT_ASSERT( iConnectionAvailability );
    }  

void UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability_DoExecuteLL()
    {
	// Test: VS is allowed in All Netwoks by Default. 

	// Test1: VS in 3G Always Allowed. 
	TInt error = KErrArgument;
	TInt count = KErrNone;
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iConnectionAvailability->State() 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusAvaStatusAvailable );


	//Test2 : VS is even allowed when Network type is Unknown
	iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeUnknown );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    CMusAvaConnectionMonitor& avaConnectionMonitor = iSharedObject->ConnectionMonitor();
    avaConnectionMonitor.iConnectionMonitor.iConnectionId = 1; 
    avaConnectionMonitor.iConnectionMonitor.iConnectionCounter = 1;
    avaConnectionMonitor.iConnectionMonitor.iUintAttributeValue = 1;
    avaConnectionMonitor.iConnectionMonitor.iConnectionInfoError = KErrNone;    
    
    //EUNIT_DISABLE_ALLOC_DECORATOR;
    
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    
    //EUNIT_DISABLE_ALLOC_DECORATOR;
    
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    
    //HKK Define own error condition in which it can fil. 
    
    
    if ( count > 0 && ( iConnectionAvailability->State() 
        == MMusAvaObserver::EMusActivationError) ||
        (iConnectionAvailability->State() == MMusAvaObserver::EMusAvaNetworkType ))
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusAvaStatusAvailable );
	
	// Test3: VS in GSM Network is Allowed Ensure, PDP Context Need to be up to ensure network does 
	// support data connection; Note DTM Flag is not needed anymore since VS is allowed in all network. 
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeGsm );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    avaConnectionMonitor.iConnectionMonitor.iConnectionId = 1; 
    avaConnectionMonitor.iConnectionMonitor.iConnectionCounter = 1;
    avaConnectionMonitor.iConnectionMonitor.iUintAttributeValue = 1;
    avaConnectionMonitor.iConnectionMonitor.iConnectionInfoError = KErrNone;    
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && ( iConnectionAvailability->State() 
        == MMusAvaObserver::EMusActivationError ) ||
        (iConnectionAvailability->State() == MMusAvaObserver::EMusAvaNetworkType ))
        
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusAvaStatusAvailable );
	
	
	// Test4: VS in GSM Network is dis-allowed if PDP context is not up. 
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeGsm );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    avaConnectionMonitor.iConnectionMonitor.iConnectionId = 1; 
    avaConnectionMonitor.iConnectionMonitor.iConnectionCounter = 0;
    avaConnectionMonitor.iConnectionMonitor.iUintAttributeValue = 1;
    avaConnectionMonitor.iConnectionMonitor.iConnectionInfoError = KErrNone;    
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && ( iConnectionAvailability->State() 
        == MMusAvaObserver::EMusActivationError ) ||
        (iConnectionAvailability->State() == MMusAvaObserver::EMusAvaNetworkType ))
        
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusAvaEdgeDtmStatusUnknown );
    }


void UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability_DoExecuteLL2()
    {
    // Restrict VS to be only Used in 3G mode, 
    
   	iStorage->Set ( MusSettingsKeys::KAllowOnlyIn3GNetwork,
   	MusSettingsKeys::EAllowed3GOnly );       
	
	// Test: In GSM VS is not Allowed when its restricted to be only used in 3G
    
    TInt ret = 0;
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeGsm );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    MultimediaSharingSettings::SetEdgeDtmSupportSettingL( MusSettingsKeys::EDtmModeNotAllowed );
    TRAPD( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 &&
         iConnectionAvailability->State() !=  MMusAvaObserver::EMusAvaNetworkType )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusAvaNetworkType );

    // Test2: In 3G VS is allowed.
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iConnectionAvailability->State() 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusAvaStatusAvailable );
    
    // Test3: Active in home networks
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iConnectionAvailability->State() 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusAvaStatusAvailable );
    
    //Test4: activation set off
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::ENever );
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusActivationError ); 

    // Test5: Edge/Dtm activation settings
    // case 1 Dtm mode allowed and atleast one pdp context exist
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeGsm );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    MultimediaSharingSettings::SetEdgeDtmSupportSettingL( MusSettingsKeys::EDtmModeAllowed );
    CMusAvaConnectionMonitor& avaConnectionMonitor = iSharedObject->ConnectionMonitor();
    avaConnectionMonitor.iConnectionMonitor.iConnectionId = 1; 
    avaConnectionMonitor.iConnectionMonitor.iConnectionCounter = 1;
    avaConnectionMonitor.iConnectionMonitor.iUintAttributeValue = 1;
    avaConnectionMonitor.iConnectionMonitor.iConnectionInfoError = KErrNone;    
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iConnectionAvailability->State() 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusAvaStatusAvailable );
    
    // Test5: Edge DTM is allowed but no pdp context
    // case 2 Dtm mode allowed and no pdp context exist
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeGsm );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    MultimediaSharingSettings::SetEdgeDtmSupportSettingL( MusSettingsKeys::EDtmModeAllowed );    
    avaConnectionMonitor.iConnectionMonitor.iConnectionId = 1; 
    avaConnectionMonitor.iConnectionMonitor.iConnectionCounter = 0;
    avaConnectionMonitor.iConnectionMonitor.iUintAttributeValue = 1;
    avaConnectionMonitor.iConnectionMonitor.iConnectionInfoError = KErrNone;    
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iConnectionAvailability->State() 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusAvaEdgeDtmStatusUnknown );
    
    // Test6: case 2 Dtm mode not allowed
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeGsm );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    MultimediaSharingSettings::SetEdgeDtmSupportSettingL( MusSettingsKeys::EDtmModeNotAllowed );
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iConnectionAvailability->State() 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusAvaNetworkType );
    
    // Reset the Central Reppository to the default value to avoid 
    // poluting common storage.
    
   	iStorage->Set ( MusSettingsKeys::KAllowOnlyIn3GNetwork,
   	MusSettingsKeys::EAllowedAllBearers );       
    }
 		
void UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability_NameL()
    {
    EUNIT_ASSERT( iConnectionAvailability->Name() == MMusAvaObserver::EMusAvaBearerStatus );
    }
  
void UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability_StopL()
    {
    iConnectionAvailability->Stop();
    EUNIT_ASSERT( iSharedObject->MusAvaTelephonyStatus().iObserver.Count() == 0 );
    }

void UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability_PhoneNetworkModeStatusL()
    {
    //Test1:  VS by default would allow all type of Networks. 
    // Unknow Network Type : Allowed
    iConnectionAvailability->PhoneNetworkModeStatus( RMobilePhone::ENetworkModeUnknown );
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && ( iConnectionAvailability->State() ==  MMusAvaObserver::EMusActivationError ) ||
    					(iConnectionAvailability->State() == MMusAvaObserver::EMusAvaNetworkType ))
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->iState ==  MMusAvaObserver::EMusAvaStatusAvailable );
    
    //GSM Network is Allowed:
    iConnectionAvailability->PhoneNetworkModeStatus( RMobilePhone::ENetworkModeGsm );
    
    count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && ( iConnectionAvailability->State() ==  MMusAvaObserver::EMusActivationError ) ||
    					(iConnectionAvailability->State() == MMusAvaObserver::EMusAvaNetworkType ))
        {
        User::Leave( KErrNoMemory );
        }
    
    EUNIT_ASSERT( iConnectionAvailability->iState ==  MMusAvaObserver::EMusAvaStatusAvailable );
    
    //WCDMA: 3G is Allowed
    iConnectionAvailability->PhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    MMusAvaObserver::TAvailabilityStatus state = MMusAvaObserver::EMusAvaStatusNotExecuted;
    state = iConnectionAvailability->State();
    count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && (state == MMusAvaObserver::EMusActivationError ) ||
    				    ( state == MMusAvaObserver::EMusAvaNetworkType ))
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( state ==  MMusAvaObserver::EMusAvaStatusAvailable );

    //Test2:  Restrict VS to be only used in 3G
	iStorage->Set ( MusSettingsKeys::KAllowOnlyIn3GNetwork,
      	MusSettingsKeys::EAllowed3GOnly );       
	
	//Unknown Network DisAllowed
    iConnectionAvailability->PhoneNetworkModeStatus( RMobilePhone::ENetworkModeUnknown );
    EUNIT_ASSERT( iConnectionAvailability->iState ==  MMusAvaObserver::EMusAvaNetworkType );
    
    //GSM Network is Dis-Allowed:
    iConnectionAvailability->PhoneNetworkModeStatus( RMobilePhone::ENetworkModeGsm );
    EUNIT_ASSERT( iConnectionAvailability->iState ==  MMusAvaObserver::EMusAvaNetworkType );

    
    //3G is Allowed:
    iConnectionAvailability->PhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    MMusAvaObserver::TAvailabilityStatus state1 = MMusAvaObserver::EMusAvaStatusNotExecuted;
    state1 = iConnectionAvailability->State();
    TInt count1 = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count1 > 0 && state1 == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( state1 ==  MMusAvaObserver::EMusAvaStatusAvailable );
    
    //Reset the key to the default value.
   	iStorage->Set ( MusSettingsKeys::KAllowOnlyIn3GNetwork,
      	MusSettingsKeys::EAllowedAllBearers );       
    }
                          
void UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability_NetworkRegistrationStatusL()
    {
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    iConnectionAvailability->iState = MMusAvaObserver::EMusAvaStatusAvailable;
    iConnectionAvailability->NetworkRegistrationStatus( RMobilePhone::ERegistrationUnknown );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iConnectionAvailability->iState 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->iState ==  MMusAvaObserver::EMusAvaRoamingError );
    iConnectionAvailability->iState = MMusAvaObserver::EMusAvaStatusAvailable;
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    iConnectionAvailability->NetworkRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iConnectionAvailability->iState 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->iState ==  MMusAvaObserver::EMusAvaStatusAvailable );
    }
    
void UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability_EventLL()
    {
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    CMusAvaConnectionMonitor& avaConnectionMonitor = iSharedObject->ConnectionMonitor();
    avaConnectionMonitor.iConnectionMonitor.iConnectionId = 1; 
    avaConnectionMonitor.iConnectionMonitor.iConnectionCounter = 1;
    avaConnectionMonitor.iConnectionMonitor.iUintAttributeValue = 1;
    avaConnectionMonitor.iConnectionMonitor.iConnectionInfoError = KErrNone;
    
    //Test create connection
    //Access point 
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    //IAP 
    avaConnectionMonitor.iConnectionID = 0;
    CConnMonCreateConnection* createConnection = 
        new( ELeave ) CConnMonCreateConnection( 1 );
    CleanupStack::PushL( createConnection );
    iConnectionAvailability->EventL( *createConnection );
    CleanupStack::PopAndDestroy( createConnection );
    EUNIT_ASSERT( avaConnectionMonitor.iConnectionMonitor.iConnectionId == 1 );
    //Connection is created
    avaConnectionMonitor.iConnectionMonitor.iConnectionId = 1;
    createConnection = 
        new( ELeave ) CConnMonCreateConnection( 2 );
    CleanupStack::PushL( createConnection );
    iConnectionAvailability->EventL( *createConnection );
    CleanupStack::PopAndDestroy( createConnection );
    EUNIT_ASSERT( avaConnectionMonitor.iConnectionMonitor.iConnectionId == 1 );
    avaConnectionMonitor.iConnectionID = 0;
    //PDP context deleted, before is created
    createConnection = 
        new( ELeave ) CConnMonCreateConnection( 1 );
    CleanupStack::PushL( createConnection );
    avaConnectionMonitor.iConnectionMonitor.iConnectionInfoError = KErrNotFound;
    iConnectionAvailability->EventL( *createConnection );
    EUNIT_ASSERT( iConnectionAvailability->State() ==
        MMusAvaObserver::EMusAvaConnectionErr );
    CleanupStack::PopAndDestroy( createConnection );
    avaConnectionMonitor.iConnectionMonitor.iConnectionInfoError = KErrNone;
    
    //Test delete connection /////////////////////////////////////////////////////
    CConnMonDeleteConnection* deleteConnection = 
        new( ELeave ) CConnMonDeleteConnection( 1, 64, 64, ETrue );
    CleanupStack::PushL( deleteConnection );
    iConnectionAvailability->EventL( *deleteConnection );
    EUNIT_ASSERT( iConnectionAvailability->State() ==
        MMusAvaObserver::EMusAvaConnectionErr );
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaConnectionErr );
    CleanupStack::PopAndDestroy( deleteConnection );
    iAvailabilityObserver->iStatus =  MMusAvaObserver::EMusAvaStatusNotExecuted ;
    iAvailabilityObserver->iName = MMusAvaObserver::EMusAvaNameNotDefined;
    iConnectionAvailability->SetState( 
                                MMusAvaObserver::EMusAvaStatusNotExecuted );
        ;
    //Connection ID not found
    CConnMonDeleteConnection* deleteConnection2 = 
        new( ELeave ) CConnMonDeleteConnection( 3 , 64, 64, ETrue );
    CleanupStack::PushL( deleteConnection2 );
    iConnectionAvailability->EventL( *deleteConnection2 );
    //Initial value or previous values
    EUNIT_ASSERT( iConnectionAvailability->State() ==
        MMusAvaObserver::EMusAvaStatusNotExecuted );
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaStatusNotExecuted );
    CleanupStack::PopAndDestroy( deleteConnection2 );
    iAvailabilityObserver->iStatus = MMusAvaObserver::EMusAvaStatusNotExecuted;
    iConnectionAvailability->SetState(
                                MMusAvaObserver::EMusAvaStatusNotExecuted );
        
    // Connection Status Change ///////////////////////////////////////////////
    
    //Connection status found, but no connection  
    CConnMonConnectionStatusChange* connectionStatusChange = 
        new( ELeave ) CConnMonConnectionStatusChange( 2, 1, KConnectionFailure  );                                              
    CleanupStack::PushL( connectionStatusChange );
    iConnectionAvailability->EventL( *connectionStatusChange );   
    CleanupStack::PopAndDestroy( connectionStatusChange ); 
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaStatusNotExecuted ); 

    //Connection status and connection ID are found
    avaConnectionMonitor.iConnectionID = 1;  
    connectionStatusChange = 
        new( ELeave ) CConnMonConnectionStatusChange( 1, 1, KConnectionFailure  );                                                 
    CleanupStack::PushL( connectionStatusChange );
    iConnectionAvailability->EventL( *connectionStatusChange );   
    CleanupStack::PopAndDestroy( connectionStatusChange ); 
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaConnectionErr );  
    iConnectionAvailability->SetState(
                                MMusAvaObserver::EMusAvaStatusNotExecuted ); 
    iAvailabilityObserver->iStatus =
        MMusAvaObserver::EMusAvaStatusNotExecuted; 
    //Connection status and connection ID are not found 
    connectionStatusChange = new( ELeave ) CConnMonConnectionStatusChange( 
                            2, 1, KConfigDaemonFinishedDeregistrationStop  );
                                                     
    CleanupStack::PushL( connectionStatusChange );
    iConnectionAvailability->EventL( *connectionStatusChange );   
    CleanupStack::PopAndDestroy( connectionStatusChange ); 
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaStatusNotExecuted );  
        
     //Connection status is not found, but connection ID is found   
    connectionStatusChange = 
        new( ELeave ) CConnMonConnectionStatusChange( 1, 1, KConfigDaemonFinishedDeregistrationStop  );                                               
    CleanupStack::PushL( connectionStatusChange );
    iConnectionAvailability->EventL( *connectionStatusChange );   
    CleanupStack::PopAndDestroy( connectionStatusChange ); 
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaStatusNotExecuted );  
        
    //Connection created, but connection ID is not found
    connectionStatusChange = 
        new( ELeave ) CConnMonConnectionStatusChange( 2, 1, KConnectionOpen  );                                               
    CleanupStack::PushL( connectionStatusChange );
    iConnectionAvailability->EventL( *connectionStatusChange );   
    CleanupStack::PopAndDestroy( connectionStatusChange ); 
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaStatusNotExecuted );  
    //Connection created, but connection ID is found    
    connectionStatusChange = 
        new( ELeave ) CConnMonConnectionStatusChange( 1, 1, KConnectionOpen  );                                               
    CleanupStack::PushL( connectionStatusChange );
    iConnectionAvailability->EventL( *connectionStatusChange );   
    CleanupStack::PopAndDestroy( connectionStatusChange ); 
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaStatusAvailable );  
    //Test network status ////////////////////////////////////////////////////
    CConnMonNetworkStatusChange* networkStatusChange = 
        new( ELeave ) CConnMonNetworkStatusChange( EBearerIdWCDMA, EConnMonStatusActive );
    CleanupStack::PushL( networkStatusChange );
    iConnectionAvailability->EventL( *networkStatusChange );
    EUNIT_ASSERT( iConnectionAvailability->State() ==
        MMusAvaObserver::EMusAvaStatusAvailable );
    CleanupStack::PopAndDestroy( networkStatusChange );    

    networkStatusChange = 
        new( ELeave ) CConnMonNetworkStatusChange( EBearerIdWCDMA, EConnMonStatusNotAvailable );
    CleanupStack::PushL( networkStatusChange );
    iConnectionAvailability->EventL( *networkStatusChange );
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaConnectionErr );
    CleanupStack::PopAndDestroy( networkStatusChange );
    iAvailabilityObserver->iStatus =
                                MMusAvaObserver::EMusAvaStatusNotExecuted;
    iAvailabilityObserver->iName = MMusAvaObserver::EMusAvaNameNotDefined;
    iConnectionAvailability->SetState(
                                MMusAvaObserver::EMusAvaStatusNotExecuted );
        
        
   //Network status not available
    CConnMonNetworkStatusChange* networkStatusChange4 = 
        new( ELeave ) CConnMonNetworkStatusChange( 3, EConnMonStatusNotAvailable );
    CleanupStack::PushL( networkStatusChange4 );
    iConnectionAvailability->EventL( *networkStatusChange4 );
    EUNIT_ASSERT( iConnectionAvailability->State() ==
        MMusAvaObserver::EMusAvaStatusNotExecuted );
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaStatusNotExecuted );
    CleanupStack::PopAndDestroy( networkStatusChange4 );
    iAvailabilityObserver->iStatus =
                                MMusAvaObserver::EMusAvaStatusNotExecuted;
    iAvailabilityObserver->iName = MMusAvaObserver::EMusAvaNameNotDefined;
        
    //Test network Registration ///////////////////////////////////////////////
    iConnectionAvailability->SetState(
                                        MMusAvaObserver::EMusAvaStatusAvailable );
    CConnMonNetworkRegistrationChange* registrationChange = 
        new( ELeave ) CConnMonNetworkRegistrationChange( EBearerIdWCDMA, 
        ENetworkRegistrationHomeNetwork );
    CleanupStack::PushL( registrationChange );
    iConnectionAvailability->EventL( *registrationChange );
    EUNIT_ASSERT( iConnectionAvailability->State() ==
        MMusAvaObserver::EMusAvaStatusAvailable );
    CleanupStack::PopAndDestroy( registrationChange );
    iAvailabilityObserver->iStatus =
                                MMusAvaObserver::EMusAvaStatusNotExecuted;
    iAvailabilityObserver->iName = MMusAvaObserver::EMusAvaNameNotDefined;
    iConnectionAvailability->SetState(
                                    MMusAvaObserver::EMusAvaStatusAvailable );
        
    registrationChange = new( ELeave ) CConnMonNetworkRegistrationChange(
                        EBearerIdWCDMA, 
                        ENetworkRegistrationRoaming );
    CleanupStack::PushL( registrationChange );
    iConnectionAvailability->EventL( *registrationChange );
    EUNIT_ASSERT( iConnectionAvailability->State() ==
    MMusAvaObserver::EMusAvaRoamingError );
    CleanupStack::PopAndDestroy( registrationChange );
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
                MMusAvaObserver::EMusAvaRoamingError );
    iAvailabilityObserver->iStatus =
                                MMusAvaObserver::EMusAvaStatusNotExecuted;
    iAvailabilityObserver->iName = MMusAvaObserver::EMusAvaNameNotDefined;
        
    //Test bearer change///////////////////////////////////////////////////////
                
    CConnMonBearerChange* bearerChange = 
        new( ELeave ) CConnMonBearerChange( EBearerIdWCDMA, EBearerWCDMA );
    CleanupStack::PushL( bearerChange );
    iConnectionAvailability->EventL( *bearerChange );
    EUNIT_ASSERT( iConnectionAvailability->State() ==
        MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaStatusAvailable );
    CleanupStack::PopAndDestroy( bearerChange );
    iAvailabilityObserver->iStatus =
                                MMusAvaObserver::EMusAvaStatusNotExecuted;
    iAvailabilityObserver->iName = MMusAvaObserver::EMusAvaNameNotDefined;
        
   //Unknown unknown bearer change
    bearerChange = 
        new( ELeave ) CConnMonBearerChange( EBearerIdGSM, EBearerGPRS );
    CleanupStack::PushL( bearerChange );
    iConnectionAvailability->EventL( *bearerChange );
    EUNIT_ASSERT( iConnectionAvailability->State() ==
        MMusAvaObserver::EMusAvaNetworkType );
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==
        MMusAvaObserver::EMusAvaNetworkType );
    CleanupStack::PopAndDestroy( bearerChange );
    iAvailabilityObserver->iStatus =
                                MMusAvaObserver::EMusAvaStatusNotExecuted;
    iAvailabilityObserver->iName = MMusAvaObserver::EMusAvaNameNotDefined;
    }

void UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability_NetworkRegistrationAndSettingsLL()
    {
    iStorage->Clear();
     	MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EOperatorSpecific );
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys:: ENever  );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL() 
    == MMusAvaObserver::EMusActivationError );
    
    iStorage->Clear();
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys:: ENever  );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusActivationError );
    
    iStorage->Clear();
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeUnknown );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegistrationUnknown );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL() 
    == MMusAvaObserver::EMusActivationError );
    
    iStorage->Clear();
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::ENever );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusActivationError );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegistrationUnknown );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegistrationUnknown );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusActivationError );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::ENever );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusActivationError );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
     	iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationAllowed);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationAllowed);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationAllowed);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationExecuted);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationExecuted);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusActivationError );
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationNotExecuted);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaManualActivation );
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationAllowed);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationNotAllowed);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationNotAllowed);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusActivationError );
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationNotExecuted);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaManualActivation );
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationAllowed);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    }
    
    
void UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability_ManualActivationLL()
 	{
 	MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EOperatorSpecific );
 	EUNIT_ASSERT( iConnectionAvailability->ManualActivationL() == MMusAvaObserver::EMusActivationError );	
 	
 	MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
 	iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationAllowed);
 	EUNIT_ASSERT( iConnectionAvailability->ManualActivationL() == MMusAvaObserver::EMusAvaStatusAvailable );	
 	
	iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationNotExecuted );
 	EUNIT_ASSERT( iConnectionAvailability->ManualActivationL() == MMusAvaObserver::EMusAvaManualActivation );	
 	
 	iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationExecuted );
 	EUNIT_ASSERT( iConnectionAvailability->ManualActivationL() == MMusAvaObserver::EMusActivationError );	
 	}
 	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaConnectionAvailability,
    "CMusAvaConnectionAvailability",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaConnectionAvailability",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionAvailability_NewLL, Teardown)
    
EUNIT_TEST(
    "DoExecuteL - test ",
    "CMusAvaConnectionAvailability",
    "DoExecuteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionAvailability_DoExecuteLL, Teardown)
    
EUNIT_TEST(
    "DoExecuteL - test 3G Restricted",
    "CMusAvaConnectionAvailability",
    "DoExecuteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionAvailability_DoExecuteLL2, Teardown)
    
    
EUNIT_TEST(
    "Name - test ",
    "CMusAvaConnectionAvailability",
    "Name",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionAvailability_NameL, Teardown)
    
EUNIT_TEST(
    "Stop - test ",
    "CMusAvaConnectionAvailability",
    "Stop",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionAvailability_StopL, Teardown)
 
    
 EUNIT_TEST(
    "PhoneNetworkModeStatusL - test ",
    "CMusAvaConnectionAvailability",
    "PhoneNetworkModeStatusL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionAvailability_PhoneNetworkModeStatusL, Teardown)
    
 EUNIT_TEST(
    "NetworkRegistrationStatus - test ",
    "CMusAvaConnectionAvailability",
    "NetworkRegistrationStatus",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionAvailability_NetworkRegistrationStatusL, Teardown)
    
 EUNIT_TEST(
    "EventL - test ",
    "CMusAvaConnectionAvailability",
    "EventL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionAvailability_EventLL, Teardown)

  
EUNIT_TEST(
    "NetworkRegistrationAndSettingsL - test ",
    "CMusAvaConnectionAvailability",
    "NetworkRegistrationAndSettingsL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionAvailability_NetworkRegistrationAndSettingsLL, Teardown)
    
EUNIT_TEST(
    "ManualActivationL - test ",
    "CMusAvaConnectionAvailability",
    "ManualActivationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionAvailability_ManualActivationLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
