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
#include "e32property.h"
#include "ut_muscallmonitor.h"
#include "muscallmonitor.h"


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>

_LIT(KTelNo,"1234");

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallMonitor* UT_CMusCallMonitor::NewL()
    {
    UT_CMusCallMonitor* self = UT_CMusCallMonitor::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallMonitor* UT_CMusCallMonitor::NewLC()
    {
    UT_CMusCallMonitor* self = new( ELeave ) UT_CMusCallMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
// UT_CMusCallMonitor::NotifyCallStateChanged() 
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitor::NotifyCallStateChanged(
									NMusSessionInformationApi::TMusCallEvent /*aVal*/,
									TName& /*aCallName*/ )
    {          
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallMonitor::~UT_CMusCallMonitor()
    {
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusCallMonitor::UT_CMusCallMonitor()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitor::SetupL()
    {    
    TName callName = KTelNo();
    TBool pttCallExist = EFalse;
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    iCallMonitor = CMusCallMonitor::NewL( callName, pttCallExist );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitor::Teardown()
    {
    delete iCallMonitor;
    iLine.Close(); 
    PropertyHelper::Close();

    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitor::UT_CMusCallMonitor_StartMonitorLL()
    {

    EUNIT_ASSERT( !iCallMonitor->iCallStatusMonitor );
    EUNIT_ASSERT( !iCallMonitor->iCallEventMonitor );
    iCallMonitor->StartMonitorL( iLine, *this, *this );

    EUNIT_ASSERT( iCallMonitor->iCallStatusMonitor );
    EUNIT_ASSERT( iCallMonitor->iCallEventMonitor );

    EUNIT_ASSERT_LEAVE( iCallMonitor->StartMonitorL( iLine, *this , *this) );
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitor::UT_CMusCallMonitor_StopMonitorLL()
    {
    
    EUNIT_ASSERT( !iCallMonitor->iCallStatusMonitor );
    EUNIT_ASSERT( !iCallMonitor->iCallEventMonitor );
    iCallMonitor->StopMonitor();
    EUNIT_ASSERT( !iCallMonitor->iCallStatusMonitor );
    EUNIT_ASSERT( !iCallMonitor->iCallEventMonitor );

    iCallMonitor->StartMonitorL( iLine, *this, *this );

    EUNIT_ASSERT( iCallMonitor->iCallStatusMonitor );
    EUNIT_ASSERT( iCallMonitor->iCallEventMonitor );
    iCallMonitor->StopMonitor();
    EUNIT_ASSERT( !iCallMonitor->iCallStatusMonitor );
    EUNIT_ASSERT( !iCallMonitor->iCallEventMonitor );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitor::UT_CMusCallMonitor_IsEqualL()
    {
    TName test = KNullDesC();
    
    EUNIT_ASSERT( !iCallMonitor->IsEqual( test ) );

    test = iCallMonitor->Name();
    EUNIT_ASSERT( iCallMonitor->IsEqual( test ) );

    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitor::UT_CMusCallMonitor_IsMonitoredL()
    {
    
    EUNIT_ASSERT( !iCallMonitor->IsMonitored() );
    
    iCallMonitor->StartMonitorL( iLine, *this, *this );
    EUNIT_ASSERT( iCallMonitor->IsMonitored() );
    
    
    }


// -----------------------------------------------------------------------------
//  MusCallStateChanged from the MusCallStateObserver 
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitor::MusCallStateChanged()
    {
    // NOP
    }   
   


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusCallMonitor,
    "UT_CMusCallMonitor",
    "UNIT" )

EUNIT_TEST(
    "StartMonitorL - test",
    "CMusCallStatusMonitor",
    "StartMonitorL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallMonitor_StartMonitorLL, Teardown)

EUNIT_TEST(
    "StopMonitorL - test",
    "CMusCallStatusMonitor",
    "StopMonitorL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallMonitor_StopMonitorLL, Teardown)

EUNIT_TEST(
    "IsEqual - test",
    "CMusCallStatusMonitor",
    "IsEqual",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallMonitor_IsEqualL, Teardown)

EUNIT_TEST(
    "IsMonitored - test",
    "CMusCallStatusMonitor",
    "IsMonitored",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallMonitor_IsMonitoredL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE


