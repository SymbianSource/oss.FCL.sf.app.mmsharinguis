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
#include "UT_availability.h"
#include "CSipSseTestTls.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>


//  INTERNAL INCLUDES


// CONSTRUCTION
T_availability* T_availability::NewL()
    {
    T_availability* self = T_availability::NewLC();
    CleanupStack::Pop();

    return self;
    }

T_availability* T_availability::NewLC()
    {
    T_availability* self = new( ELeave ) T_availability();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
T_availability::~T_availability()
    {
    }

// Default constructor
T_availability::T_availability()
    {
    }

// Second phase construct
void T_availability::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void T_availability::SetupL(  )
    {
    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
    }

void T_availability::Teardown(  )
    {
    if ( iStorage )
        {
        iStorage->Clear();
        CSipSseTestTls::Close();
        iStorage = NULL;
        }
    }

void T_availability::TestL(  )
    {
    EUNIT_ASSERT( EFalse );
    }
    
void UT_CMusAvaController_StartAvailibilityLL()
    {
    
    }
  
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    T_availability,
    "Add test suite description here.",
    "MODULE" )

EUNIT_TEST(
    "Empty test skeleton",
    "Add tested class name here",
    "Add tested function name here",
    "FUNCTIONALITY",
    SetupL, TestL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
