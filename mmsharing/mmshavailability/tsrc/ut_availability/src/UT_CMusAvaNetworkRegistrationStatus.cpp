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
#include "UT_CMusAvaNetworkRegistrationStatus.h"
#include "musavatelephonystatusobserverimp.h"
#include "musavaavailability.h"
#include "muslogger.h"
#include "musavacapabilitytesthelper.h" //remove compilation warning



//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
//#include <telephonyinternalpskeys.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include "CSipSseTestTls.h"

//  INTERNAL INCLUDES
#include "musavanetworkregistrationstatus.h"



// CONSTRUCTION
UT_CMusAvaNetworkRegistrationStatus* UT_CMusAvaNetworkRegistrationStatus::NewL()
    {
    UT_CMusAvaNetworkRegistrationStatus* self = UT_CMusAvaNetworkRegistrationStatus::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaNetworkRegistrationStatus* UT_CMusAvaNetworkRegistrationStatus::NewLC()
    {
    UT_CMusAvaNetworkRegistrationStatus* self = new( ELeave ) UT_CMusAvaNetworkRegistrationStatus();
    CleanupStack::PushL( self );
	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaNetworkRegistrationStatus::~UT_CMusAvaNetworkRegistrationStatus()
    {
    }

// Default constructor
UT_CMusAvaNetworkRegistrationStatus::UT_CMusAvaNetworkRegistrationStatus()
    {
    }

// Second phase construct
void UT_CMusAvaNetworkRegistrationStatus::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMusAvaNetworkRegistrationStatus::SetupL(  )
    {
    SIPStrings::OpenL();
    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
    iObserver = CMusAvaTelephonyStatusObserverImp::NewL();
    iNetworkRegistrationStatus = CMusAvaNetworkRegistrationStatus::NewL( );	
    } 

void UT_CMusAvaNetworkRegistrationStatus::Teardown(  )
    {
    if ( iStorage )
        {
        iStorage->Clear();
        CSipSseTestTls::Close();
        iStorage = NULL;
        }
    delete iObserver;
    delete iNetworkRegistrationStatus;
    SIPStrings::Close();
    }

void UT_CMusAvaNetworkRegistrationStatus::UT_CMusAvaNetworkRegistrationStatus_NewLL(  )
    {
    EUNIT_ASSERT( iNetworkRegistrationStatus );
    }
void  UT_CMusAvaNetworkRegistrationStatus::UT_CMusAvaNetworkRegistrationStatus_AddAdapterLL()
    {
    iNetworkRegistrationStatus->AddAdapterL( *iObserver );
    }

void UT_CMusAvaNetworkRegistrationStatus::UT_CMusAvaNetworkRegistrationStatus_NetworkRegistrationL()
    {
    RMobilePhone::TMobilePhoneRegistrationStatus regStatus = RMobilePhone::ERegistrationUnknown;
    RMobilePhone::TMobilePhoneRegistrationStatus regStatusUpdate = RMobilePhone::ERegistrationUnknown;
    iStorage->SetRegistrationStatus( regStatus );
    iNetworkRegistrationStatus->NetworkRegistrationStatus( regStatusUpdate );
    EUNIT_ASSERT( regStatus == regStatusUpdate );
    }

void UT_CMusAvaNetworkRegistrationStatus::UT_CMusAvaNetworkRegistrationStatus_RemoveAdapterL()
    {
    iNetworkRegistrationStatus->RemoveAdapter( *iObserver );
    }

void UT_CMusAvaNetworkRegistrationStatus::UT_CMusAvaNetworkRegistrationStatus_TelephonyStatusL()
    {
    /*iNetworkRegistrationStatus->TelephonyStatus();
    EUNIT_ASSERT( iNetworkRegistrationStatus->iRegistrationStatus == iStorage->iPhoneRegistrationStatus );
    User::RequestComplete(KErrNone, iObserver->iRequestStatus );*/
    }

void UT_CMusAvaNetworkRegistrationStatus::UT_CMusAvaNetworkRegistrationStatus_NetworkRegistrationStatusL()
    {
    iNetworkRegistrationStatus->NetworkRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    EUNIT_ASSERT( RMobilePhone::ERegisteredOnHomeNetwork != iStorage->iPhoneRegistrationStatus );
    
    iNetworkRegistrationStatus->AddAdapterL( *iObserver );
    iNetworkRegistrationStatus->NetworkRegistrationStatus( RMobilePhone::ERegisteredOnHomeNetwork );
    EUNIT_ASSERT( iObserver->iPhoneRegistrationStatus == RMobilePhone::ERegisteredOnHomeNetwork );
    }
        		
void UT_CMusAvaNetworkRegistrationStatus::UT_CMusAvaNetworkRegistrationStatus_RunL()
    {
    CActiveScheduler::Start();
    }
  
void UT_CMusAvaNetworkRegistrationStatus::UT_CMusAvaNetworkRegistrationStatus_StatusL()
    {
   // NetworkRegistrationStatus->RunL();
    }

void UT_CMusAvaNetworkRegistrationStatus::UT_CMusAvaNetworkRegistrationStatus_DoCancelL()
    {
    
    }

void UT_CMusAvaNetworkRegistrationStatus::UT_CMusAvaNetworkRegistrationStatus_RunErrorLL()
    {
    
    } 

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaNetworkRegistrationStatus,
    "CMusAvaNetworkRegistrationStatus",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaNetworkRegistrationStatus",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkRegistrationStatus_NewLL, Teardown)
    
EUNIT_TEST(
    "AddAdapterL - test ",
    "CMusAvaNetworkRegistrationStatus",
    "AddAdapterL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkRegistrationStatus_AddAdapterLL, Teardown)


EUNIT_TEST(
    "NetworkRegistrationL - test ",
    "CMusAvaNetworkRegistrationStatus",
    "NetworkRegistrationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkRegistrationStatus_NetworkRegistrationL, Teardown)
  
EUNIT_TEST(
    "RemoveAdapter - test ",
    "CMusAvaNetworkRegistrationStatus",
    "RemoveAdapter",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkRegistrationStatus_RemoveAdapterL, Teardown)

    
EUNIT_TEST(
    "TelephonyStatus - test ",
    "CMusAvaNetworkRegistrationStatus",
    "TelephonyStatus",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkRegistrationStatus_TelephonyStatusL, Teardown)
 

EUNIT_TEST(
    "NetworkRegistrationStatus - test ",
    "CMusAvaNetworkRegistrationStatus",
    "NetworkRegistrationStatus",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkRegistrationStatus_NetworkRegistrationStatusL, Teardown) 
    
EUNIT_TEST(
    "StatusL - test ",
    "CMusAvaNetworkRegistrationStatus",
    "StatusL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkRegistrationStatus_StatusL, Teardown)  
  
EUNIT_TEST(
    "DoCancel - test ",
    "CMusAvaNetworkRegistrationStatus",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkRegistrationStatus_DoCancelL, Teardown)  
    
EUNIT_TEST(
    "RunErrorL - test ",
    "CMusAvaNetworkRegistrationStatus",
    "RunErrorL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkRegistrationStatus_RunErrorLL, Teardown) 
        
    
    

EUNIT_END_TEST_TABLE

//  END OF FILE
