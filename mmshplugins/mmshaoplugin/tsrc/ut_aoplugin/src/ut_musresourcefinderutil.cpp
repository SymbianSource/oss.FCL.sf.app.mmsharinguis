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
#include "ut_musresourcefinderutil.h"
#include "musresourcefinderutil.h"
 

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_MusResourceFinderUtil* UT_MusResourceFinderUtil::NewL()
    {
    UT_MusResourceFinderUtil* self = UT_MusResourceFinderUtil::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_MusResourceFinderUtil* UT_MusResourceFinderUtil::NewLC()
    {
    UT_MusResourceFinderUtil* self = new( ELeave ) UT_MusResourceFinderUtil();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_MusResourceFinderUtil::~UT_MusResourceFinderUtil()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_MusResourceFinderUtil::UT_MusResourceFinderUtil()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_MusResourceFinderUtil::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusResourceFinderUtil::SetupL()
    {

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusResourceFinderUtil::Teardown()
    {

    }



// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusResourceFinderUtil::UT_MusResourceFinderUtil_ResourcePathLL()
    {
    _LIT( reourceFile, "noway");
    HBufC* name = MusResourceFinderUtil::ResourcePathL( reourceFile );
    CleanupStack::PushL( name );
    EUNIT_ASSERT( name );
    CleanupStack::PopAndDestroy( name );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusResourceFinderUtil::UT_MusResourceFinderUtil_AppResourcePathLL()
    {
    _LIT( reourceFile, "noway");
    HBufC* name = MusResourceFinderUtil::AppResourcePathL( reourceFile );
    CleanupStack::PushL( name );
    EUNIT_ASSERT( name );
    CleanupStack::PopAndDestroy( name );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_MusResourceFinderUtil::UT_MusResourceFinderUtil_IsExistLL()
    {
    _LIT( reourceFile, "noway");
    EUNIT_ASSERT( !MusResourceFinderUtil::IsExistL( reourceFile ) );
    } 
    


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_MusResourceFinderUtil,
    "UT_MusResourceFinderUtil",
    "UNIT" )

EUNIT_TEST(
    "ResourcePathL - test ",
    "MusResourceFinderUtil",
    "ResourcePathL",
    "FUNCTIONALITY",
    SetupL, UT_MusResourceFinderUtil_ResourcePathLL, Teardown)

EUNIT_TEST(
    "AppResourcePathL - test ",
    "MusResourceFinderUtil",
    "AppResourcePathL",
    "FUNCTIONALITY",
    SetupL, UT_MusResourceFinderUtil_AppResourcePathLL, Teardown)

EUNIT_TEST(
    "IsExistL - test ",
    "MusResourceFinderUtil",
    "IsExistL",
    "FUNCTIONALITY",
    SetupL, UT_MusResourceFinderUtil_IsExistLL, Teardown)



EUNIT_END_TEST_TABLE

//  END OF FILE


