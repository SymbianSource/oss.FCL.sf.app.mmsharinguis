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
#include "ut_muscallstatusmonitor.h"
#include "muscallstatusmonitor.h"


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>

_LIT(KTelNo,"1234");

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallStatusMonitor* UT_CMusCallStatusMonitor::NewL()
    {
    UT_CMusCallStatusMonitor* self = UT_CMusCallStatusMonitor::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallStatusMonitor* UT_CMusCallStatusMonitor::NewLC()
    {
    UT_CMusCallStatusMonitor* self = new( ELeave ) UT_CMusCallStatusMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// UT_CMusCallStatusMonitor::NotifyCallStateChanged() 
// -----------------------------------------------------------------------------
//
void UT_CMusCallStatusMonitor::NotifyCallStateChanged(
    NMusSessionInformationApi::TMusCallEvent aVal,
    TName& /*aCallName*/ )
    {
    if ( aVal ==  NMusSessionInformationApi::ENoCall )
        {
        // Simulate the actual observer in case of no call
        delete iCallStatusMonitor;
        iCallStatusMonitor = NULL;
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallStatusMonitor::~UT_CMusCallStatusMonitor()
    {
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusCallStatusMonitor::UT_CMusCallStatusMonitor()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusCallStatusMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallStatusMonitor::SetupL()
    {    
    RTelHelper::SetCallStatus( RMobileCall::EStatusIdle );
    
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     NMusSessionInformationApi::ENoCall );
    
    iCallStatusMonitor = CMusCallStatusMonitor::NewL( iCall, *this, *this ); 
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallStatusMonitor::Teardown()
    {
    delete iCallStatusMonitor;
    PropertyHelper::Close();
    iCall.Close();   
    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallStatusMonitor::UT_CMusCallStatusMonitor_CheckStateLL()
    {
    User::LeaveIfError( 
        RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMusCallCount,
                        1 ) );    
    User::LeaveIfError(
        RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMusCallEvent,
                        NMusSessionInformationApi::ENoCall ) );     
    TInt callEvent( NMusSessionInformationApi::ENoCall );
    
    // No actions: RMobileCall::EStatusIdle
    iCallStatusMonitor->iCallStatus = RMobileCall::EStatusIdle;
    iCallStatusMonitor->CheckStateL();
    EUNIT_ASSERT_EQUALS( RMobileCall::EStatusIdle, 
                         iCallStatusMonitor->iCallStatus )
    User::LeaveIfError(
        RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMusCallEvent,
                        callEvent ) );
    EUNIT_ASSERT_EQUALS( 
        static_cast< TInt >( NMusSessionInformationApi::ENoCall ),
        callEvent)

    // RMobileCall::EStatusConnected
    iCallStatusMonitor->iCallStatus = RMobileCall::EStatusConnected;   
    iCallStatusMonitor->CheckStateL();
    EUNIT_ASSERT_EQUALS( RMobileCall::EStatusConnected, 
                         iCallStatusMonitor->iCallStatus )
    User::LeaveIfError(
        RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMusCallEvent,
                        callEvent ) );   
    EUNIT_ASSERT_EQUALS( 
        static_cast< TInt >( NMusSessionInformationApi::ECallConnected ), 
        callEvent  )
    
    // RMobileCall::EStatusDisconnecting
    iCallStatusMonitor->iCallStatus = RMobileCall::EStatusDisconnecting;
    iCallStatusMonitor->CheckStateL();
    User::LeaveIfError(
        RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMusCallEvent,
                        callEvent ) );  
    EUNIT_ASSERT_EQUALS( 
        static_cast< TInt >( NMusSessionInformationApi::ENoCall ),
        callEvent )
    EUNIT_ASSERT( iCallStatusMonitor == NULL )
    
    // RMobileCall::EStatusDisconnectingWithInband
    // iCallStatusMonitor was deleted by the previous call
    iCallStatusMonitor = CMusCallStatusMonitor::NewL( iCall, *this, *this );
    iCallStatusMonitor->iCallStatus = RMobileCall::EStatusDisconnectingWithInband;
    iCallStatusMonitor->CheckStateL();
    User::LeaveIfError(
        RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMusCallEvent,
                        callEvent ) );    
    EUNIT_ASSERT_EQUALS( 
        static_cast< TInt >( NMusSessionInformationApi::ENoCall ),
        callEvent )
    EUNIT_ASSERT( iCallStatusMonitor == NULL )
    
    // RMobileCall::EStatusHold
    // iCallStatusMonitor was deleted by the previous call
    iCallStatusMonitor = CMusCallStatusMonitor::NewL( iCall, *this, *this ); 
    iCallStatusMonitor->iCallStatus = RMobileCall::EStatusHold;
    iCallStatusMonitor->CheckStateL();
    User::LeaveIfError(
        RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMusCallEvent,
                        callEvent ) );    
    EUNIT_ASSERT_EQUALS( 
        static_cast< TInt >( NMusSessionInformationApi::ECallHold ),
        callEvent )   
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallStatusMonitor::UT_CMusCallStatusMonitor_RunLL()
    {
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    TInt val = RMobileCall::EStatusUnknown;
    EUNIT_ASSERT( iCallStatusMonitor );
    iCallStatusMonitor->Cancel();
    RTelHelper::SetCallStatus( RMobileCall::EStatusAnswering );
    iCallStatusMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     1 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
	
    iCallStatusMonitor->Cancel();
    RTelHelper::SetCallStatus( RMobileCall::EStatusConnected );
    iCallStatusMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallConnected);

    iCallStatusMonitor->Cancel();
    RTelHelper::SetCallStatus( RMobileCall::EStatusHold );
    iCallStatusMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallHold);

    iCallStatusMonitor->Cancel();
    RTelHelper::SetCallStatus( RMobileCall::EStatusTransferring );
    iCallStatusMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallHold); //previous state.
    
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallStatusMonitor::UT_CMusCallStatusMonitor_DoCancelL()
    {
    EUNIT_ASSERT( iCallStatusMonitor );
    iCallStatusMonitor->DoCancel();
    EUNIT_ASSERT( RTelHelper::GetCalledFunction() == 
                                        RTelSubSessionBase::ECancelAsyncRequest )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallStatusMonitor::UT_CMusCallStatusMonitor_RunErrorL()
    {
    EUNIT_ASSERT( iCallStatusMonitor );
    TInt error = KErrNotFound;
    iCallStatusMonitor->RunError(error);
    }


// -----------------------------------------------------------------------------
//  MusCallStateChanged from the MusCallStateObserver 
// -----------------------------------------------------------------------------
//
void UT_CMusCallStatusMonitor::MusCallStateChanged()
    {
    // NOP
    }   


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusCallStatusMonitor,
    "UT_CMusCallStatusMonitor",
    "UNIT" )

EUNIT_TEST(
    "CheckStateL - test",
    "CMusCallStatusMonitor",
    "CheckStateL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallStatusMonitor_CheckStateLL, Teardown)
    
EUNIT_TEST(
    "RunL - test",
    "CMusCallStatusMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallStatusMonitor_RunLL, Teardown)

EUNIT_TEST(
    "DoCancel - test",
    "CMusCallStatusMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallStatusMonitor_DoCancelL, Teardown)

EUNIT_TEST(
    "RunError - test",
    "CMusCallStatusMonitor",
    "RunErrorL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallStatusMonitor_RunErrorL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE


