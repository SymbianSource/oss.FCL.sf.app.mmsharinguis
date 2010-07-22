/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Monitors the Kodiak Phone Call
*
*/



#include <e32property.h>
#include <CTSYDomainPSKeys.h>
#include <digia/eunit/eunitmacros.h>
#include "muspttcallmonitor.h"
#include "ut_muspttcallmonitor.h"

#pragma warn_illtokenpasting off

// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
UT_CMusPTTCallMonitor* UT_CMusPTTCallMonitor::NewLC()
    {    
    UT_CMusPTTCallMonitor* self = new (ELeave) UT_CMusPTTCallMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();    
    return self;
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void UT_CMusPTTCallMonitor::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
UT_CMusPTTCallMonitor::UT_CMusPTTCallMonitor() 
    : CEUnitTestSuiteClass()
    {
    // nothing
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
UT_CMusPTTCallMonitor::~UT_CMusPTTCallMonitor()
    {
    }
 
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPTTCallMonitor::UT_RunLL()
    { 
                
    iMonitor->RunL();
    iMonitor->Cancel();
    EUNIT_ASSERT( iMonitor->IsPTTCallExist());  
    
    // case 2.
    iMonitor->iPropertyEvent.iPValue = EPSCTsyKodiakPttNotActive;
    iMonitor->RunL();
    iMonitor->Cancel();
    EUNIT_ASSERT( !iMonitor->IsPTTCallExist() );
    
    // case 3
    iMonitor->iPropertyEvent.iPValue = EPSCTsyKodiakPttActive;
    iMonitor->RunL();
    iMonitor->Cancel();
    EUNIT_ASSERT( iMonitor->IsPTTCallExist() );           
   
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPTTCallMonitor::UT_IsPTTCallExistL()
    {
    iMonitor->iPropertyEvent.iPValue = EPSCTsyKodiakPttNotActive;
    EUNIT_ASSERT( !iMonitor->IsPTTCallExist());
    
    iMonitor->iPropertyEvent.iPValue = EPSCTsyKodiakPttActive;
    EUNIT_ASSERT( iMonitor->IsPTTCallExist());    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPTTCallMonitor::UT_DoCancelL()
    {        
    iMonitor->Cancel();
    EUNIT_ASSERT(!iMonitor->IsActive());
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPTTCallMonitor::SetupL()
    {        
    iMonitor = CMusPttCallMonitor::NewL(*this);  
    iMonitor->Cancel();      
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPTTCallMonitor::Teardown()
    {        
    delete iMonitor;    
    }

// test table

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusPTTCallMonitor,
    "UT_CMusPTTCallMonitor",
    "UNIT" )

EUNIT_TEST(
    "RunL - test",
    "CMusPTTCallMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_RunLL, Teardown)

EUNIT_TEST(
    "IsPTTCallExist - test",
    "CMusPTTCallMonitor",
    "IsPTTCallExist",
    "FUNCTIONALITY",
    SetupL, UT_IsPTTCallExistL, Teardown)

EUNIT_TEST(
    "DoCancel - test",
    "CMusPTTCallMonitor",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_DoCancelL, Teardown)

EUNIT_END_TEST_TABLE

