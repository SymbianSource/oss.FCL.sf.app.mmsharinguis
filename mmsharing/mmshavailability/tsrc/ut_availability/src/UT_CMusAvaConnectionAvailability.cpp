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
    if ( count > 0 && iConnectionAvailability->State() 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusAvaNetworkType );
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    
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

    // No roaming for operator variant
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EOperatorSpecific );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iConnectionAvailability->State() 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusActivationError );
    
    // operator variant OK
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EOperatorSpecific );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    
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
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    
    // activation set off
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::ENever );
    TRAP( error, iConnectionAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_ASSERT( iConnectionAvailability->State() ==  MMusAvaObserver::EMusActivationError ); 

    //Edge/Dtm activation settings
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
    
    // case 2 Dtm mode not allowed
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
    iConnectionAvailability->PhoneNetworkModeStatus( RMobilePhone::ENetworkModeUnknown );
    EUNIT_ASSERT( iConnectionAvailability->iState ==  MMusAvaObserver::EMusAvaNetworkType );
    iConnectionAvailability->PhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    MMusAvaObserver::TAvailabilityStatus state = MMusAvaObserver::EMusAvaStatusNotExecuted;
    state = iConnectionAvailability->State();
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && state == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( state ==  MMusAvaObserver::EMusAvaStatusAvailable );
    }
                          
void UT_CMusAvaConnectionAvailability::UT_CMusAvaConnectionAvailability_NetworkRegistrationStatusL()
    {
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EOperatorSpecific );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    iConnectionAvailability->iState = MMusAvaObserver::EMusAvaStatusAvailable;
    iConnectionAvailability->NetworkRegistrationStatus( RMobilePhone::ERegistrationUnknown );
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iConnectionAvailability->iState 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iConnectionAvailability->iState ==  MMusAvaObserver::EMusAvaRoamingError );
    iConnectionAvailability->iState = MMusAvaObserver::EMusAvaStatusAvailable;
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
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
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::ENever );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusActivationError );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
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
    == MMusAvaObserver::EMusActivationError );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationAllowed);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaStatusAvailable );
    
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
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
    == MMusAvaObserver::EMusActivationError );
    
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationNotExecuted);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
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
    == MMusAvaObserver::EMusActivationError );
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationNotExecuted);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    EUNIT_ASSERT( iConnectionAvailability->NetworkRegistrationAndSettingsL()
    == MMusAvaObserver::EMusAvaManualActivation );
    
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    iStorage->SetRegistrationStatus( RMobilePhone::ERegisteredRoaming );
    iConcreteSettings->SetManualActivation( MMusAvaSettings::EActivationAllowed);
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
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
