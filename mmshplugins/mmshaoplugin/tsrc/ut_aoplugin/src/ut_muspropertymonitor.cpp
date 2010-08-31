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
    iMusPropertyMonitor = CMusPropertyMonitor::NewL(*this);
    iMusPropertyMonitor->iPropertyEvent.iPValue = NMusSessionInformationApi::ENoCall;
    iMusCallStateObserverInovked = EFalse;
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
    EUNIT_ASSERT( !iMusCallStateObserverInovked );
    iMusPropertyMonitor->iPropertyEvent.iPValue = NMusSessionInformationApi::ENoCall;
    iMusPropertyMonitor->Cancel();
    iMusPropertyMonitor->RunL();
    EUNIT_ASSERT( iMusCallStateObserverInovked );   
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
    EUNIT_ASSERT( !iMusCallStateObserverInovked );
    EUNIT_ASSERT( iMusPropertyMonitor );
    TInt CallState;
    TInt error = KErrNotFound;

    // Call is Established:
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                             NMusSessionInformationApi::KMusCallEvent,
                             NMusSessionInformationApi::ECallConnected ) );

    // Test1: Invoking Error Situation, Ensure Calll is disconected.
    User::LeaveIfError( iMusPropertyMonitor->RunError(error));
    User::LeaveIfError( RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                             NMusSessionInformationApi::KMusCallEvent,
                             CallState ) );
    EUNIT_ASSERT( CallState == NMusSessionInformationApi::ENoCall );
    EUNIT_ASSERT( iMusCallStateObserverInovked );
    }


// -----------------------------------------------------------------------------
// UT_CMusPropertyMonitor::UT_CMusPropertyMonitor_IsCallConnected()
// Checks from the P/S keys if the Call is connected.
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyMonitor::UT_CMusPropertyMonitor_IsCallConnected()
    {
    // Test1: Call Not Connected:
    TBool callConnected = EFalse;
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                             NMusSessionInformationApi::KMusCallEvent,
                             NMusSessionInformationApi::ENoCall ) );
    
    callConnected = iMusPropertyMonitor->IsCallConnected();
    EUNIT_ASSERT( callConnected == EFalse );
    
    // Test2: Call Connected:
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                             NMusSessionInformationApi::KMusCallEvent,
                             NMusSessionInformationApi::ECallConnected ) );

    callConnected = iMusPropertyMonitor->IsCallConnected();
    EUNIT_ASSERT( callConnected == ETrue );
    
    
    // Test3: Hold and Conference are concidered as call connected
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                             NMusSessionInformationApi::KMusCallEvent,
                             NMusSessionInformationApi::ECallHold ) );

    callConnected = iMusPropertyMonitor->IsCallConnected();
    EUNIT_ASSERT( callConnected == ETrue );
    }


// -----------------------------------------------------------------------------
//  MusCallStateChanged from the MusCallStateObserver 
// -----------------------------------------------------------------------------
//
void UT_CMusPropertyMonitor::MusCallStateChanged()
    {
    iMusCallStateObserverInovked = ETrue;
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

EUNIT_TEST(
    "IsCallConnected - test",
    "CMusCallStatusMonitor",
    "IsCallConnected",
    "FUNCTIONALITY",
    SetupL, UT_CMusPropertyMonitor_IsCallConnected, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE


