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
#include "sipprofile.h"
#include "UT_CMusAvaConnectionMonitor.h"
#include "musavaavailability.h"
#include "muslogger.h"
#include "musavaobserver.h"
#include "musavaobserverimp.h"
#include "musavadefaultimp.h"
#include "musavaobserverimp.h"
#include "musavasettingsimp.h"
#include "musavasharedobject.h"
#include "musavasip.h"
#include "musavaconnectionMonitorobserverimp.h"
#include "mussettingskeys.h"
#include "mussettings.h"
#include "musavacapabilitytesthelper.h" //remove compilation warning
#include "CSipSseTestTls.h"


//  EXTERNAL INCLUDES
#include "sipprofile.h"
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
#include <sipstrings.h>
//#include <telephonyinternalpskeys.h>


//  INTERNAL INCLUDES
#include "musavaconnectionmonitor.h"



// CONSTRUCTION
UT_CMusAvaConnectionMonitor* UT_CMusAvaConnectionMonitor::NewL()
    {
    UT_CMusAvaConnectionMonitor* self = UT_CMusAvaConnectionMonitor::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaConnectionMonitor* UT_CMusAvaConnectionMonitor::NewLC()
    {
    UT_CMusAvaConnectionMonitor* self = new( ELeave ) UT_CMusAvaConnectionMonitor();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaConnectionMonitor::~UT_CMusAvaConnectionMonitor()
    {
    }

// Default constructor
UT_CMusAvaConnectionMonitor::UT_CMusAvaConnectionMonitor()
    {
    }

// Second phase construct
void UT_CMusAvaConnectionMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaConnectionMonitor::SetupL(  )
    {
    SIPStrings::OpenL();
    CSipSseTestTls::OpenL();
    iSharedObject = CMusAvaSharedObject::GetSingletonL();
    iAvaConnectionMonitor  = &iSharedObject->ConnectionMonitor();	
    } 

void UT_CMusAvaConnectionMonitor::Teardown(  )
    {
    CMusAvaSharedObject::DeleteSingleton();
    CSipSseTestTls::Close();
    SIPStrings::Close();
    }

void UT_CMusAvaConnectionMonitor::UT_CMusAvaConnectionMonitor_NewLL(  )
    {
    EUNIT_ASSERT( ETrue );
    }  
    
void UT_CMusAvaConnectionMonitor::UT_CMusAvaConnectionMonitor_EventLL()
    {
    CConnMonNetworkStatusChange* netStatus = 
    new( ELeave ) CConnMonNetworkStatusChange( 1, EBearerWCDMA );
     CleanupStack::PushL( netStatus );
    CMMusAvaConnectionMonitorObserverImp* imp1 = CMMusAvaConnectionMonitorObserverImp::NewL();
     CleanupStack::PushL( imp1 );
    iAvaConnectionMonitor->EventL( *netStatus );
    EUNIT_ASSERT( imp1->iConnMonEventBase == NULL );
    iAvaConnectionMonitor->AddAdapterL( *imp1 );
    iAvaConnectionMonitor->EventL(* netStatus );
    EUNIT_ASSERT( &imp1->iConnMonEventBase != NULL );
    CMMusAvaConnectionMonitorObserverImp* imp2 = CMMusAvaConnectionMonitorObserverImp::NewL();
    CleanupStack::PushL( imp2 );
    iAvaConnectionMonitor->AddAdapterL( *imp2 );
    iAvaConnectionMonitor->EventL( *netStatus );
    EUNIT_ASSERT( &imp1->iConnMonEventBase != NULL );
    CleanupStack::PopAndDestroy( imp2 ); 
    CleanupStack::PopAndDestroy( imp1 ); 
    CleanupStack::PopAndDestroy( netStatus );
    }

void UT_CMusAvaConnectionMonitor::UT_CMusAvaConnectionMonitor_AddAdapterLL()
    {
    CMMusAvaConnectionMonitorObserverImp* imp1 = CMMusAvaConnectionMonitorObserverImp::NewL();
     CleanupStack::PushL( imp1 );
    CMMusAvaConnectionMonitorObserverImp* imp2 = CMMusAvaConnectionMonitorObserverImp::NewL();
     CleanupStack::PushL( imp2 );
    iAvaConnectionMonitor->AddAdapterL( *imp1 );
    iAvaConnectionMonitor->AddAdapterL( *imp2 );
    CleanupStack::PopAndDestroy( imp2 ); 
    CleanupStack::PopAndDestroy( imp1 ); 
    }

void UT_CMusAvaConnectionMonitor::UT_CMusAvaConnectionMonitor_RemoveAdapterL()
    {
    CMMusAvaConnectionMonitorObserverImp* imp1 = CMMusAvaConnectionMonitorObserverImp::NewL();
    CleanupStack::PushL( imp1 );
    CMMusAvaConnectionMonitorObserverImp* imp2 = CMMusAvaConnectionMonitorObserverImp::NewL();
     CleanupStack::PushL( imp2 );
    iAvaConnectionMonitor->AddAdapterL( *imp1 );
    iAvaConnectionMonitor->AddAdapterL( *imp2 );
    iAvaConnectionMonitor->RemoveAdapter( *imp1 );
    iAvaConnectionMonitor->RemoveAdapter( *imp2 );
     CleanupStack::PopAndDestroy( imp2 ); 
    CleanupStack::PopAndDestroy( imp1 ); 
    }
void UT_CMusAvaConnectionMonitor::UT_CMusAvaConnectionMonitor_ConnectionCountL()
    {
    iAvaConnectionMonitor->iConnectionMonitor.iConnectionCounter = 10;
    TUint ret = iAvaConnectionMonitor->ConnectionCount();
    EUNIT_ASSERT( ret == 10 );
    iAvaConnectionMonitor->iConnectionMonitor.iConnectionCounter = 0;
    ret = iAvaConnectionMonitor->ConnectionCount();
    EUNIT_ASSERT( ret == 0 );  
    }

void UT_CMusAvaConnectionMonitor::UT_CMusAvaConnectionMonitor_ConnectionIDLL()
    {
    //No connection
    iAvaConnectionMonitor->iConnectionMonitor.iConnectionCounter = 0;
    EUNIT_ASSERT( iAvaConnectionMonitor->ConnectionIDL() == 0 );
    //Connection  created
    iAvaConnectionMonitor->iConnectionMonitor.iConnectionCounter = 1;
    EUNIT_ASSERT( !iAvaConnectionMonitor->ConnectionIDL() );
    //Access point found
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    EUNIT_ASSERT( iAvaConnectionMonitor->ConnectionIDL() == 0 );
    //ConnectionId leavs
    iAvaConnectionMonitor->iConnectionMonitor.iConnectionInfoError = KErrNotFound;
    TRAPD( error, iAvaConnectionMonitor->ConnectionIDL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNotFound );
    //Connection ID found  
    iAvaConnectionMonitor->iConnectionMonitor.iConnectionId = 1; 
    iAvaConnectionMonitor->iConnectionMonitor.iConnectionInfoError = KErrNone;
    EUNIT_ASSERT( iAvaConnectionMonitor->ConnectionIDL() == 0);
    //IAP doesn't match
    iAvaConnectionMonitor->iConnectionMonitor.iUintAttributeValue = 3;
    EUNIT_ASSERT( iAvaConnectionMonitor->ConnectionIDL() == 0 );
     //IAP found
    iAvaConnectionMonitor->iConnectionMonitor.iUintAttributeValue = 1;
    EUNIT_ASSERT( iAvaConnectionMonitor->ConnectionIDL() );
    }

void UT_CMusAvaConnectionMonitor::UT_CMusAvaConnectionMonitor_MultimediaSharingAccessPointIDL()
    {
    //No profile
    MultimediaSharingSettings::SetSipProfileSettingL( KErrNotFound );
    TInt ret = iAvaConnectionMonitor->MultimediaSharingAccessPointID();
    if ( ret != KErrNotFound )
        {
        if ( iSharedObject->MusAvaSip().Profile() )
            {
            TBool val;
            iSharedObject->MusAvaSip().Profile()->GetParameter( KSIPDefaultProfile, val );
            if ( val )
                {
                User::Leave( KErrNoMemory );
                }
            }
        }
    else
        {
        EUNIT_ASSERT( ret == KErrNotFound );    
        }
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    TRAPD( err, ret = iSharedObject->MusAvaSip().CreateProfileL() );
    if ( err == KErrNoMemory || !ret )
        {
        User::Leave( KErrNoMemory );
        }
    //No profile id found
    iSharedObject->MusAvaSip().Profile()->iTUint32Value = 0;
    EUNIT_ASSERT( iAvaConnectionMonitor->MultimediaSharingAccessPointID() == 0 );
    //Profile id found
    iSharedObject->MusAvaSip().Profile()->iTUint32Value = 1;
    EUNIT_ASSERT( iAvaConnectionMonitor->MultimediaSharingAccessPointID() == 1 );
    }

    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaConnectionMonitor,
    "CMusAvaConnectionMonitor",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaConnectionMonitor",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionMonitor_NewLL, Teardown)

EUNIT_TEST(
    "EventL - test ",
    "CMusAvaConnectionMonitor",
    "EventL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionMonitor_EventLL, Teardown)
    
EUNIT_TEST(
    "AddAdapterL - test ",
    "CMusAvaConnectionMonitor",
    "AddAdapterL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionMonitor_AddAdapterLL, Teardown)
       
    
EUNIT_TEST(
    "RemoveAdapter - test ",
    "CMusAvaConnectionMonitor",
    "RemoveAdapter",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionMonitor_RemoveAdapterL, Teardown)
            

EUNIT_TEST(
    "ConnectionCount - test ",
    "CMusAvaConnectionMonitor",
    "ConnectionCount",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionMonitor_ConnectionCountL, Teardown)
    
EUNIT_TEST(
    "ConnectionIDL - test ",
    "CMusAvaConnectionMonitor",
    "ConnectionIDL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionMonitor_ConnectionIDLL, Teardown)
    
EUNIT_TEST(
    "MultimediaSharingAccessPointID - test ",
    "CMusAvaConnectionMonitor",
    "MultimediaSharingAccessPointID",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaConnectionMonitor_MultimediaSharingAccessPointIDL, Teardown)
    
    

EUNIT_END_TEST_TABLE

//  END OF FILE
