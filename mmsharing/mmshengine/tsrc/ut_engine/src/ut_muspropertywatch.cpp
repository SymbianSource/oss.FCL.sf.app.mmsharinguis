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
#include "ut_muspropertywatch.h"
#include "muspropertywatch.h"

#include "mussessionproperties.h"

//  SYSTEM INCLUDES
#include <eunitmacros.h>
#include <e32property.h>

using namespace NMusSessionApi;

const TInt KMusUiTestKey = 1100;
const TInt KMusUiTestPropertyReadInteger = 200;
_LIT( KMusUiTestPropertyReadDesc, "testread" ); 
TUid KMusUiDummyUid = { 1000 }; // Unit testing dummy

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusPropertyWatch* UT_CMusPropertyWatch::NewL()
    {
    UT_CMusPropertyWatch* self = UT_CMusPropertyWatch::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusPropertyWatch* UT_CMusPropertyWatch::NewLC()
    {
    UT_CMusPropertyWatch* self = new( ELeave ) UT_CMusPropertyWatch();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusPropertyWatch::~UT_CMusPropertyWatch()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusPropertyWatch::UT_CMusPropertyWatch()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyWatch::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyWatch::SetupL()
    {
    PropertyHelper::SetErrorCode( KErrNone );
    RProperty::Set( KMusUiDummyUid, KMusUiTestKey, KMusUiTestPropertyReadInteger );
    iPropertyWatch = CMusPropertyWatch::NewL( iPropertyObserver,
                                                KMusUiDummyUid,
                                                KMusUiTestKey );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyWatch::Teardown()
    {
    delete iPropertyWatch;
    PropertyHelper::Close();
    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyWatch::UT_CMusPropertyWatch_ReadIntPropertyLL()
    {

    // Teesting leave
    PropertyHelper::SetErrorCode( KErrNotFound );
    RProperty::Set( KMusUiDummyUid, KMusUiTestKey, KMusUiTestPropertyReadInteger );
    TInt value;
    TRAPD( err, value = CMusPropertyWatch::ReadIntPropertyL( KMusUiTestKey ) );
    EUNIT_ASSERT( err == KErrNotFound );

    // Testing return value
    PropertyHelper::SetErrorCode( KErrNone );
    RProperty::Set( KMusUiDummyUid, KMusUiTestKey, KMusUiTestPropertyReadInteger );
    EUNIT_ASSERT(  CMusPropertyWatch::ReadIntPropertyL( KMusUiTestKey ) ==
                   KMusUiTestPropertyReadInteger );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyWatch::UT_CMusPropertyWatch_ReadDescPropertyLL()
    {
    // Teesting leave
    PropertyHelper::SetErrorCode( KErrNotFound );
    TRAPD( err,  CMusPropertyWatch::ReadDescPropertyL( KMusUiTestKey ) );
    EUNIT_ASSERT( err == KErrNotFound );

    // Testing return value
    RProperty::Set( KMusUiDummyUid, KMusUiTestKey, KMusUiTestPropertyReadDesc );
    PropertyHelper::SetErrorCode( KErrNone );
    HBufC* result = CMusPropertyWatch::ReadDescPropertyL( KMusUiTestKey );
    CleanupStack::PushL( result );
    EUNIT_ASSERT(  KMusUiTestPropertyReadDesc().Compare( result->Des() ) == 0 );
    CleanupStack::PopAndDestroy( result );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyWatch::UT_CMusPropertyWatch_DoCancelL()
    {
    PropertyHelper::SetErrorCode( KErrNone );
    iPropertyWatch->Cancel();
    EUNIT_ASSERT( PropertyHelper::GetCalledFunction() == RProperty::ECancel );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyWatch::UT_CMusPropertyWatch_RunLL()
    {

    // Leave test
    PropertyHelper::SetErrorCode( KErrNotFound );
    iPropertyWatch->Cancel();
    TRAPD( err, iPropertyWatch->RunL() );
    EUNIT_ASSERT( err == KErrNotFound );


    // SessionPropertyChanged
    PropertyHelper::SetErrorCode( KErrNone );
    iPropertyWatch->Cancel();
    iPropertyWatch->RunL();
    EUNIT_ASSERT( iPropertyObserver.iCalledFunction ==
                  TMusPropertyObserverStub::EPropertyChanged );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyWatch::UT_CMusPropertyWatch_RunErrorL()
    {

    // Allways returns KErrNone
    PropertyHelper::SetErrorCode( KErrNone );
    EUNIT_ASSERT( iPropertyWatch->RunError( KErrNotFound ) == KErrNone );

    // is right observer called
    PropertyHelper::SetErrorCode( KErrNone );
    EUNIT_ASSERT( iPropertyObserver.iCalledFunction ==
                  TMusPropertyObserverStub::EHandlePropertyError );
    }



//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusPropertyWatch,
    "UT_CMusPropertyWatch",
    "UNIT" )

EUNIT_TEST(
    "ReadIntPropertyLL - test ",
    "CMusPropertyWatch",
    "ReadIntPropertyLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusPropertyWatch_ReadIntPropertyLL, Teardown)

EUNIT_TEST(
    "ReadDescPropertyLL - test ",
    "CMusPropertyWatch",
    "ReadDescPropertyLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusPropertyWatch_ReadDescPropertyLL, Teardown)

EUNIT_TEST(
    "DoCancelL - test ",
    "CMusPropertyWatch",
    "DoCancelL",
    "FUNCTIONALITY",
    SetupL, UT_CMusPropertyWatch_DoCancelL, Teardown)

EUNIT_TEST(
    "RunLL - test ",
    "CMusPropertyWatch",
    "RunLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusPropertyWatch_RunLL, Teardown)

EUNIT_TEST(
    "RunErrorL - test ",
    "CMusPropertyWatch",
    "RunErrorL",
    "FUNCTIONALITY",
    SetupL, UT_CMusPropertyWatch_RunErrorL, Teardown)


EUNIT_END_TEST_TABLE


//  END OF FILE


