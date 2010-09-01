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
#include "ut_musuipropertywatch.h"
#include "musuipropertywatch.h"
//#include "musestubs.h"
//#include "musengtestdefs.h"
#include "musuitestdefines.h"

#include "musuipropertyobserveradapter.h"
#include "mussessionproperties.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
//#include <centralrepository.h>
//#include <mcedisplaysink.h>


using namespace NMusSessionApi;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiPropertyWatch* UT_CMusUiPropertyWatch::NewL()
    {
    UT_CMusUiPropertyWatch* self = UT_CMusUiPropertyWatch::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiPropertyWatch* UT_CMusUiPropertyWatch::NewLC()
    {
    UT_CMusUiPropertyWatch* self = new( ELeave ) UT_CMusUiPropertyWatch();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiPropertyWatch::~UT_CMusUiPropertyWatch()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusUiPropertyWatch::UT_CMusUiPropertyWatch()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusUiPropertyWatch::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiPropertyWatch::SetupL()
    {
    // Iniatalize
    iPropertyObserver = new (ELeave) CMusUiPropertyObserverAdapter();

    TUid tmp = { 0 }; // Unit testing dummy
    iPropertyWatch = CMusUiPropertyWatch::NewL( *iPropertyObserver,
                                                tmp,
                                                0 ); // Unit testing dummy
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiPropertyWatch::Teardown()
    {
    delete iPropertyWatch;
    delete iPropertyObserver;
    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiPropertyWatch::UT_CMusUiPropertyWatch_ReadIntPropertyLL()
    {

    // Teesting leave
    RProperty::iErrorCode = KErrNotFound;
    RProperty::iIntProperty = KMusUiTestPropertyReadInteger;
    TInt value;
    TRAPD( err, value = CMusUiPropertyWatch::ReadIntPropertyL( KMusUiTestKey ) );
    EUNIT_ASSERT( err == KErrNotFound );

    // Testing return value
    RProperty::iErrorCode = KErrNone;
    RProperty::iIntProperty = KMusUiTestPropertyReadInteger;
    EUNIT_ASSERT(  CMusUiPropertyWatch::ReadIntPropertyL( KMusUiTestKey ) ==
                   KMusUiTestPropertyReadInteger );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiPropertyWatch::UT_CMusUiPropertyWatch_ReadDescPropertyLL()
    {
    // Teesting leave
    RProperty::iErrorCode = KErrNotFound;
    TRAPD( err,  CMusUiPropertyWatch::ReadDescPropertyL( KMusUiTestKey ) );
    EUNIT_ASSERT( err == KErrNotFound );

    // Testing return value
    RProperty::iErrorCode = KErrNone;
    HBufC* result = CMusUiPropertyWatch::ReadDescPropertyL( KMusUiTestKey );
    EUNIT_ASSERT(  KMusUiTestPropertyReadDesc().Compare( result->Des() ) );
    delete result;

    //EUNIT_ASSERT( EFalse );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiPropertyWatch::UT_CMusUiPropertyWatch_DoCancelL()
    {
    RProperty::iErrorCode = KErrNone;
    iPropertyWatch->Cancel();
    EUNIT_ASSERT( RProperty::iCalledFunction == RProperty::ECancel );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiPropertyWatch::UT_CMusUiPropertyWatch_RunLL()
    {

    // Leave test
    RProperty::iErrorCode = KErrNotFound;
    iPropertyWatch->Cancel();
    TRAPD( err, iPropertyWatch->RunL() );
    EUNIT_ASSERT( err == KErrNotFound );


    // SessionPropertyChanged
    RProperty::iErrorCode = KErrNone;
    iPropertyWatch->iPropertyKey = KStatus;
    iPropertyWatch->Cancel();
    iPropertyWatch->RunL();
    EUNIT_ASSERT( iPropertyObserver->iCalledObserver ==
                  CMusUiPropertyObserverAdapter::ESessionPropertyChanged );


    RProperty::iErrorCode = KErrNone;
    iPropertyWatch->iPropertyKey = KTelNumber;
    iPropertyWatch->Cancel();
    iPropertyWatch->RunL();
    EUNIT_ASSERT( iPropertyObserver->iCalledObserver ==
                  CMusUiPropertyObserverAdapter::EResourcePropertyChanged );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiPropertyWatch::UT_CMusUiPropertyWatch_RunErrorL()
    {

    // Allways returns KErrNone
    RProperty::iErrorCode = KErrNone;
    EUNIT_ASSERT( iPropertyWatch->RunError( KErrNotFound ) == KErrNone );

    // is right observer called
    RProperty::iErrorCode = KErrNone;
    EUNIT_ASSERT( iPropertyObserver->iCalledObserver ==
                  CMusUiPropertyObserverAdapter::EHandleError );
    }



//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusUiPropertyWatch,
    "UT_CMusUiPropertyWatch",
    "UNIT" )

EUNIT_TEST(
    "ReadIntPropertyLL - test ",
    "CMusUiPropertyWatch",
    "ReadIntPropertyLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiPropertyWatch_ReadIntPropertyLL, Teardown)

EUNIT_TEST(
    "ReadDescPropertyLL - test ",
    "CMusUiPropertyWatch",
    "ReadDescPropertyLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiPropertyWatch_ReadDescPropertyLL, Teardown)

EUNIT_TEST(
    "DoCancelL - test ",
    "CMusUiPropertyWatch",
    "DoCancelL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiPropertyWatch_DoCancelL, Teardown)

EUNIT_TEST(
    "RunLL - test ",
    "CMusUiPropertyWatch",
    "RunLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiPropertyWatch_RunLL, Teardown)

EUNIT_TEST(
    "RunErrorL - test ",
    "CMusUiPropertyWatch",
    "RunErrorL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiPropertyWatch_RunErrorL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
