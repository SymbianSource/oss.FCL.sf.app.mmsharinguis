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
#include "ut_musengsessiondurationtimer.h"
#include "musengsessiondurationtimer.h"
#include "musengstubs.h"
#include "musengtestdefs.h"


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSessionDurationTimer* UT_CMusEngSessionDurationTimer::NewL()
    {
    UT_CMusEngSessionDurationTimer* self = UT_CMusEngSessionDurationTimer::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSessionDurationTimer* UT_CMusEngSessionDurationTimer::NewLC()
    {
    UT_CMusEngSessionDurationTimer* self = 
                            new( ELeave ) UT_CMusEngSessionDurationTimer();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSessionDurationTimer::~UT_CMusEngSessionDurationTimer()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngSessionDurationTimer::UT_CMusEngSessionDurationTimer()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionDurationTimer::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionDurationTimer::SetupL()
    {
    iObserver = new( ELeave ) CMusEngObserverStub;
    iDurationTimer = CMusEngSessionDurationTimer::NewL( *iObserver );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionDurationTimer::Teardown()
    {
    delete iDurationTimer;
    delete iObserver;
    }



// TEST CASES



    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionDurationTimer::UT_CMusEngSessionDurationTimer_StartL()
    {
    TInt delay = 30; // For not calling RunL by itself
    iDurationTimer->Start( delay );
    EUNIT_ASSERT( iDurationTimer->IsActive() );
    
    iDurationTimer->Start( delay ); // Second call does nothing
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionDurationTimer::UT_CMusEngSessionDurationTimer_RunLL()
    {
    // Fail
    iDurationTimer->iStatus = KErrGeneral;
    iDurationTimer->RunL();
    EUNIT_ASSERT( iObserver->IsReseted() );
    
    // Success
    iDurationTimer->iStatus = KErrNone;
    iDurationTimer->RunL();
    EUNIT_ASSERT( iObserver->iUpdateTimerEventCalled );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//        
void UT_CMusEngSessionDurationTimer::UT_CMusEngSessionDurationTimer_DoCancelL()
    {
    // Cannot assert, so dummy case
    iDurationTimer->Cancel();    
    }






//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngSessionDurationTimer,
    "UT_CMusEngSesssionDurationTimer",
    "UNIT" )

EUNIT_TEST(
    "StartL - test ",
    "CMusEngSessionDurationTimer",
    "StartL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngSessionDurationTimer_StartL, Teardown)

EUNIT_TEST(
    "RunL - test ",
    "CMusEngSessionDurationTimer",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngSessionDurationTimer_RunLL, Teardown)

EUNIT_TEST(
    "DoCancel - test ",
    "CMusEngSessionDurationTimer",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngSessionDurationTimer_DoCancelL, Teardown)
   
    
EUNIT_END_TEST_TABLE

//  END OF FILE


