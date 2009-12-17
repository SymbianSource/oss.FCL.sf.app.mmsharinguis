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
#include <sipstrings.h>
#include <sipstrconsts.h>
#include "UT_CMusAvaSharedObject.h"
#include "muslogger.h"
#include "CSipSseTestTls.h"
#include "musavasipdefaulthandler.h"
#include "sipconnectionobserver.h"
#include "musavasipdefaulthandler.h"
#include "musavasipconnectionadapter.h"
#include "musavasettingsimp.h"
#include "musavaregisteravailability.h"
#include "musavasipdefaulthandler.h"
#include "musavacapabilitytesthelper.h" //remove compilation warning

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
//#include <telephonyinternalpskeys.h>
#include <sipprofileregistry.h>

//  INTERNAL INCLUDES
#include "musavasharedobject.h"



// CONSTRUCTION
UT_CMusAvaSharedObject* UT_CMusAvaSharedObject::NewL()
    {
    UT_CMusAvaSharedObject* self = UT_CMusAvaSharedObject::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaSharedObject* UT_CMusAvaSharedObject::NewLC()
    {
    UT_CMusAvaSharedObject* self = new( ELeave ) UT_CMusAvaSharedObject();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaSharedObject::~UT_CMusAvaSharedObject()
    {
    }

// Default constructor
UT_CMusAvaSharedObject::UT_CMusAvaSharedObject()
    {
    }

// Second phase construct
void UT_CMusAvaSharedObject::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaSharedObject::SetupL(  )
    {
    SIPStrings::OpenL();
    iSharedObject = CMusAvaSharedObject::GetSingletonL();
    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
 
    } 

void UT_CMusAvaSharedObject::Teardown(  )
    {
    CMusAvaSharedObject::DeleteSingleton();
  
    if ( iStorage )
        {
        iStorage->Clear();
        CSipSseTestTls::Close();
        iStorage = NULL;
        }
    SIPStrings::Close();
    }


void UT_CMusAvaSharedObject::UT_CMusAvaSharedObject_NewLL()
    {
    EUNIT_ASSERT( iSharedObject->iSelfCounter == 1 );
    CMusAvaSharedObject* sharedObject = CMusAvaSharedObject::GetSingletonL();
    EUNIT_ASSERT( iSharedObject->iSelfCounter == 2 );
    CMusAvaSharedObject::DeleteSingleton();
    EUNIT_ASSERT( iSharedObject->iSelfCounter == 1 );
    EUNIT_ASSERT( &(iSharedObject->MusAvaSip()) != NULL );
    EUNIT_ASSERT( &(iSharedObject->MusAvaTelephonyStatus()) != NULL );
    EUNIT_ASSERT( &(iSharedObject->NetworkModeStatus()) != NULL );
    }  
 	
void UT_CMusAvaSharedObject::UT_CMusAvaSharedObject_ConnectionMonitorL()
    {
    CMusAvaConnectionMonitor& connectionMonitor = iSharedObject->ConnectionMonitor();
    EUNIT_ASSERT( &connectionMonitor != NULL );
    }
    	
void UT_CMusAvaSharedObject::UT_CMusAvaSharedObject_MusAvaSipL()
    {
    CMusAvaSip& avaSip = iSharedObject->MusAvaSip();
    EUNIT_ASSERT( &avaSip != NULL );
    }

void UT_CMusAvaSharedObject::UT_CMusAvaSharedObject_SipL()
    {
    CSIP& sip = iSharedObject->Sip();
    EUNIT_ASSERT( &sip != NULL );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaSharedObject,
    "CMusAvaSharedObject",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaSip",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSharedObject_NewLL, Teardown)
    
EUNIT_TEST(
    "ConnectionMonitor - test ",
    "CMusAvaSharedObjectp",
    "ConnectionMonitor",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSharedObject_ConnectionMonitorL, Teardown)
    
    
EUNIT_TEST(
    "MusAvaSip - test ",
    "CMusAvaSharedObjectp",
    "MusAvaSip",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSharedObject_MusAvaSipL, Teardown)

EUNIT_TEST(
    "Sip - test ",
    "CMusAvaSharedObjectp",
    "Sip",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSharedObject_SipL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
