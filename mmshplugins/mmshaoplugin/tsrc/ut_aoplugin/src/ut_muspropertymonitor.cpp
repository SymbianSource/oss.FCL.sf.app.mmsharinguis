/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


//  INTERNAL INCLUDES
#include "etelmm.h"
#include "e32property.h"
#include "ut_muspropertymonitor.h"
#include "muspropertymonitor.h"
#include "mussesseioninformationapi.h"


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>

_LIT(KTelNo,"1234");


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusPropertyMonitor* UT_CMusPropertyMonitor::NewL()
    {
    UT_CMusPropertyMonitor* self = UT_CMusPropertyMonitor::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusPropertyMonitor* UT_CMusPropertyMonitor::NewLC()
    {
    UT_CMusPropertyMonitor* self = new( ELeave ) UT_CMusPropertyMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusPropertyMonitor::~UT_CMusPropertyMonitor()
    {
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusPropertyMonitor::UT_CMusPropertyMonitor()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyMonitor::SetupL()
    {        
    iMusPropertyMonitor = CMusPropertyMonitor::NewL();
    iMusPropertyMonitor->iPropertyEvent.iPValue = NMusSessionInformationApi::ENoCall;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyMonitor::Teardown()
    {
    delete iMusPropertyMonitor;    
    PropertyHelper::Close();
    }






// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyMonitor::UT_CMusPropertyMonitor_RunLL()
    {
    EUNIT_ASSERT( iMusPropertyMonitor);   
    EUNIT_ASSERT( !iMusPropertyMonitor->iManager );   
    
    iMusPropertyMonitor->iPropertyEvent.iPValue = NMusSessionInformationApi::ENoCall;
    iMusPropertyMonitor->Cancel();
    iMusPropertyMonitor->RunL();
    EUNIT_ASSERT( !iMusPropertyMonitor->iManager );   

    iMusPropertyMonitor->iPropertyEvent.iPValue = NMusSessionInformationApi::ECallConnected;
    iMusPropertyMonitor->Cancel();
    iMusPropertyMonitor->RunL();
    EUNIT_ASSERT( iMusPropertyMonitor->iManager );
    
    iMusPropertyMonitor->iPropertyEvent.iPValue = NMusSessionInformationApi::ECallConnected;
    iMusPropertyMonitor->Cancel();
    iMusPropertyMonitor->RunL();
    EUNIT_ASSERT( iMusPropertyMonitor->iManager );
    
    iMusPropertyMonitor->iPropertyEvent.iPValue = NMusSessionInformationApi::ECallHold;
    iMusPropertyMonitor->Cancel();
    iMusPropertyMonitor->RunL();
    EUNIT_ASSERT( iMusPropertyMonitor->iManager );
    
    iMusPropertyMonitor->iPropertyEvent.iPValue = NMusSessionInformationApi::EConferenceCall;
    iMusPropertyMonitor->Cancel();
    iMusPropertyMonitor->RunL();
    EUNIT_ASSERT( iMusPropertyMonitor->iManager );
    
    iMusPropertyMonitor->iPropertyEvent.iPValue = NMusSessionInformationApi::ENoCall;
    iMusPropertyMonitor->Cancel();
    iMusPropertyMonitor->RunL();
    EUNIT_ASSERT( !iMusPropertyMonitor->iManager );  
    
    iMusPropertyMonitor->iPropertyEvent.iPValue = 555;
    iMusPropertyMonitor->Cancel();
    iMusPropertyMonitor->RunL();
    EUNIT_ASSERT( !iMusPropertyMonitor->iManager );  
    
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyMonitor::UT_CMusPropertyMonitor_DoCancelL()
    {
    EUNIT_ASSERT( iMusPropertyMonitor );
    iMusPropertyMonitor->DoCancel();
    EUNIT_ASSERT( PropertyHelper::GetCalledFunction() == 
                                        RProperty::ECancel )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyMonitor::UT_CMusPropertyMonitor_RunErrorL()
    {
    EUNIT_ASSERT( iMusPropertyMonitor );
    TInt error = KErrNotFound;
    iMusPropertyMonitor->RunError(error);
    }



   


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusPropertyMonitor,
    "UT_CMusPropertyMonitor",
    "UNIT" )

EUNIT_TEST(
    "RunL - test",
    "CMusCallStatusMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMusPropertyMonitor_RunLL, Teardown)

EUNIT_TEST(
    "DoCancel - test",
    "CMusCallStatusMonitor",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_CMusPropertyMonitor_DoCancelL, Teardown)

EUNIT_TEST(
    "RunError - test",
    "CMusCallStatusMonitor",
    "RunErrorL",
    "FUNCTIONALITY",
    SetupL, UT_CMusPropertyMonitor_RunErrorL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE


