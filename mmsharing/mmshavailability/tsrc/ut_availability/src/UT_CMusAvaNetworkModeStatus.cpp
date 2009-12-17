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
#include "UT_CMusAvaNetworkModeStatus.h"
#include "musavatelephonystatusobserverimp.h"
#include "musavaavailability.h"
#include "muslogger.h"
#include "musavacapabilitytesthelper.h" //remove compilation warning
#include "CSipSseTestTls.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
//#include <telephonyinternalpskeys.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <digia/eunit/eunitdecorators.h>

//  INTERNAL INCLUDES
#include "musavanetworkmodestatus.h"

// CONSTRUCTION
UT_CMusAvaNetworkModeStatus* UT_CMusAvaNetworkModeStatus::NewL()
    {
    UT_CMusAvaNetworkModeStatus* self = UT_CMusAvaNetworkModeStatus::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaNetworkModeStatus* UT_CMusAvaNetworkModeStatus::NewLC()
    {
    UT_CMusAvaNetworkModeStatus* self = new( ELeave ) UT_CMusAvaNetworkModeStatus();
    CleanupStack::PushL( self );
	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaNetworkModeStatus::~UT_CMusAvaNetworkModeStatus()
    {
    }

// Default constructor
UT_CMusAvaNetworkModeStatus::UT_CMusAvaNetworkModeStatus()
    {
    }

// Second phase construct
void UT_CMusAvaNetworkModeStatus::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    
//  METHODS

void UT_CMusAvaNetworkModeStatus::SetupL(  )
    {
    SIPStrings::OpenL();
    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
    iObserver = CMusAvaTelephonyStatusObserverImp::NewL();
    iNetworkRegistrationStatus = CMusAvaNetworkModeStatus::NewL();
    } 

void UT_CMusAvaNetworkModeStatus::Teardown(  )
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

void UT_CMusAvaNetworkModeStatus::UT_CMusAvaNetworkModeStatus_NewLL(  )
    {
    EUNIT_ASSERT( iNetworkRegistrationStatus );
    }
void  UT_CMusAvaNetworkModeStatus::UT_CMusAvaNetworkModeStatus_AddAdapterLL()
    {
    iNetworkRegistrationStatus->AddAdapterL( *iObserver );
    }

void UT_CMusAvaNetworkModeStatus::UT_CMusAvaNetworkModeStatus_RemoveAdapterL()
    {
    iNetworkRegistrationStatus->RemoveAdapter( *iObserver );
    }

void UT_CMusAvaNetworkModeStatus::UT_CMusAvaNetworkModeStatus_PhoneNetworkModeL()
    {
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeUnknown );
    EUNIT_ASSERT( !iNetworkRegistrationStatus->PhoneNetworkMode( ) );
    iStorage->Clear();
    iStorage->SetPhoneNetworkModeStatus( RMobilePhone::ENetworkModeWcdma );
    EUNIT_ASSERT( iNetworkRegistrationStatus->PhoneNetworkMode() );
    }
    
void UT_CMusAvaNetworkModeStatus::UT_CMusAvaNetworkModeStatus_PhoneNetworkModeStatusL()    
    {
    
    TRAPD( error, iNetworkRegistrationStatus->AddAdapterL( *this ) );
    EUNIT_ASSERT( error == KErrNoMemory || error == KErrNone );
    if ( error == KErrNoMemory )
        {
        User::Leave( error );
        }

    EUNIT_DISABLE_ALLOC_DECORATOR;
    // no error => status updated
    iNetworkRegistrationStatus->iPhone.DoNotifyModeChange( 
                                RMobilePhone::ENetworkModeGsm, KErrNone );
    CActiveScheduler::Start();
    EUNIT_ASSERT_EQUALS( iNetworkStatus, RMobilePhone::ENetworkModeGsm );
    
    // error => status not updated
    iNetworkRegistrationStatus->iPhone.DoNotifyModeChange( 
                                RMobilePhone::ENetworkModeCdma95, KErrGeneral );    

    iPeriodic = CPeriodic::NewL( EPriorityLow );
    iPeriodic->Start( 1000, 1000, TCallBack( UT_CMusAvaNetworkModeStatus::Notify, this ) );
    CActiveScheduler::Start();
    delete iPeriodic;    
    EUNIT_ENABLE_ALLOC_DECORATOR;

    EUNIT_ASSERT_EQUALS( iNetworkStatus, RMobilePhone::ENetworkModeGsm );                                
    
    iNetworkRegistrationStatus->RemoveAdapter( *this );
    }
    
void UT_CMusAvaNetworkModeStatus::PhoneNetworkModeStatus( 
                    RMobilePhone::TMobilePhoneNetworkMode aStatus )
    {
    iNetworkStatus = aStatus;
    CActiveScheduler::Stop();
    }

void UT_CMusAvaNetworkModeStatus::NetworkRegistrationStatus( 
        RMobilePhone::TMobilePhoneRegistrationStatus /*aRegStatus*/ )
    {
    
    }
    
TInt UT_CMusAvaNetworkModeStatus::Notify( TAny* aPtr )
    {
    ( ( UT_CMusAvaNetworkModeStatus* ) aPtr )->iPeriodic->Cancel();
    CActiveScheduler::Stop();
    return KErrNone;
    }
    


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaNetworkModeStatus,
    "CMusAvaNetworkModeStatus",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaNetworkModeStatus",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkModeStatus_NewLL, Teardown)
    
EUNIT_TEST(
    "AddAdapterL - test ",
    "CMusAvaNetworkModeStatus",
    "AddAdapterL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkModeStatus_AddAdapterLL, Teardown)

  
EUNIT_TEST(
    "RemoveAdapter - test ",
    "CMusAvaNetworkModeStatus",
    "RemoveAdapter",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkModeStatus_RemoveAdapterL, Teardown)
 
EUNIT_TEST(
    "PhoneNetworkMode - test ",
    "CMusAvaNetworkModeStatus",
    "PhoneNetworkMode",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkModeStatus_PhoneNetworkModeL, Teardown)
    
EUNIT_TEST(
    "PhoneNetworkModeStatus - test ",
    "CMusAvaNetworkModeStatus",
    "PhoneNetworkModeStatus",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkModeStatus_PhoneNetworkModeStatusL, Teardown)    
  

EUNIT_END_TEST_TABLE

//  END OF FILE
