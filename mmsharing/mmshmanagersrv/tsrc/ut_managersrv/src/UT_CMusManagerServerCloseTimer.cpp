/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit tests for CMusManagerServerCloseTimer class.
*
*/


#include "UT_CMusManagerServerCloseTimer.h"
#include "musmanagerserverclosetimer.h"
#include <digia/eunit/eunitmacros.h>


// ======== MEMBER FUNCTIONS ========


UT_CMusManagerServerCloseTimer* UT_CMusManagerServerCloseTimer::NewL()
    {
    UT_CMusManagerServerCloseTimer* self = UT_CMusManagerServerCloseTimer::NewLC();
    CleanupStack::Pop();
    return self;
    }


UT_CMusManagerServerCloseTimer* UT_CMusManagerServerCloseTimer::NewLC()
    {
    UT_CMusManagerServerCloseTimer* self = new( ELeave ) UT_CMusManagerServerCloseTimer();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


UT_CMusManagerServerCloseTimer::~UT_CMusManagerServerCloseTimer()
    {
    }


UT_CMusManagerServerCloseTimer::UT_CMusManagerServerCloseTimer()
    {
    }


// ---------------------------------------------------------------------------
// The ConstructL from the base class CEUnitTestSuiteClass must be called.
// It generates the test case table.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCloseTimer::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// ---------------------------------------------------------------------------
// Setups a test by instantiating tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCloseTimer::SetupL(  )
    {
    iTimer = CMusManagerServerCloseTimer::NewL();
    }


// ---------------------------------------------------------------------------
// Finalizes a test by deleting instance of tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCloseTimer::Teardown(  )
    {
    delete iTimer;
    iTimer = NULL;
    }


// ======== TEST METHODS ========


// ---------------------------------------------------------------------------
// Asserts a successful instantiation.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCloseTimer::UT_CMusManagerServerCloseTimer_NewLL(  )
    {
    EUNIT_ASSERT( iTimer );
    }


// ---------------------------------------------------------------------------
// Asserts a successful instantiation.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCloseTimer::UT_CMusManagerServerCloseTimer_NewLCL(  )
    {
    delete iTimer;
    iTimer = NULL;
    iTimer = CMusManagerServerCloseTimer::NewLC();
    CleanupStack::Pop( iTimer );
    EUNIT_ASSERT( iTimer );
    }


void UT_CMusManagerServerCloseTimer::UT_CMusManagerServerCloseTimer_StopActiveSchedulerAfterL(  )
    {
    iTimer->StopActiveSchedulerAfter( 1000000 );
    iTimer->Cancel();
    EUNIT_ASSERT( !iTimer->IsActive() );
    }


void UT_CMusManagerServerCloseTimer::UT_CMusManagerServerCloseTimer_DoCancelL(  )
    {
    iTimer->StopActiveSchedulerAfter( 1000000 );
    iTimer->Cancel();
    EUNIT_ASSERT( !iTimer->IsActive() );
    }


// ======== EUNIT TEST TABLE ========


EUNIT_BEGIN_TEST_TABLE(
    UT_CMusManagerServerCloseTimer,
    "CMusManagerServerCloseTimer",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusManagerServerCloseTimer",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCloseTimer_NewLL, Teardown )

EUNIT_TEST(
    "NewLC - test ",
    "CMusManagerServerCloseTimer",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCloseTimer_NewLCL, Teardown )

EUNIT_TEST(
    "StopActiveSchedulerAfter - test ",
    "CMusManagerServerCloseTimer",
    "StopActiveSchedulerAfter",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCloseTimer_StopActiveSchedulerAfterL, Teardown )

EUNIT_TEST(
    "DoCancel - test ",
    "CMusManagerServerCloseTimer",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCloseTimer_DoCancelL, Teardown )


EUNIT_END_TEST_TABLE
