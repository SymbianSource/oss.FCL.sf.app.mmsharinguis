/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit test class implementation
*
*/


//  INTERNAL INCLUDES
#include "ut_cmusavaclientresolverutil.h"
#include "musavaclientresolverutil.h"
#include "CSipSseTestTls.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <centralrepository.h>
#include <sipstrings.h>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusAvaClientResolverUtil* UT_CMusAvaClientResolverUtil::NewL()
    {
    UT_CMusAvaClientResolverUtil* self = UT_CMusAvaClientResolverUtil::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusAvaClientResolverUtil* UT_CMusAvaClientResolverUtil::NewLC()
    {
    UT_CMusAvaClientResolverUtil* self = new( ELeave ) UT_CMusAvaClientResolverUtil();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusAvaClientResolverUtil::~UT_CMusAvaClientResolverUtil()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusAvaClientResolverUtil::UT_CMusAvaClientResolverUtil()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusAvaClientResolverUtil::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAvaClientResolverUtil::SetupL()
    {
    TRAP_IGNORE( SIPStrings::OpenL() )
    CSipSseTestTls::OpenL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAvaClientResolverUtil::Teardown()
    {
    CSipSseTestTls::Close();
    SIPStrings::Close();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAvaClientResolverUtil::Setup_SawfishL()
    {
    TRAP_IGNORE( SIPStrings::OpenL() )
    CSipSseTestTls::OpenL();
    CRepository::iEmulateSawfishRepository = ETrue;
    iResolver = CMusAvaClientResolverUtil::NewL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAvaClientResolverUtil::Teardown_Sawfish()
    {
    delete iResolver;
    CRepository::iEmulateSawfishRepository = EFalse;
    CSipSseTestTls::Close();
    SIPStrings::Close();
    }


// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAvaClientResolverUtil::UT_NewL_SucceedsInSawfishL()
    {
    EUNIT_ASSERT( iResolver )
                     
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAvaClientResolverUtil::UT_NewL_FailsInOthersThanSawfishL()
    {
    TRAPD( error, iResolver = CMusAvaClientResolverUtil::NewL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error != KErrNone );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusAvaClientResolverUtil,
    "CMusAvaClientResolverUtil",
    "UNIT" )

EUNIT_TEST(
    "UT_NewL_SucceedsInSawfishL",
    "CMusAvaClientResolverUtil",
    "NewL",
    "FUNCTIONALITY",
    Setup_SawfishL, UT_NewL_SucceedsInSawfishL, Teardown_Sawfish)

EUNIT_TEST(
    "UT_NewL_FailsInOthersThanSawfishL",
    "CMusAvaClientResolverUtil",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewL_FailsInOthersThanSawfishL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE


