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
#include "ut_muscalleventmonitor.h"
#include "muscalleventmonitor.h"


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>

_LIT(KTelNo,"1234");

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallEventMonitor* UT_CMusCallEventMonitor::NewL()
    {
    UT_CMusCallEventMonitor* self = UT_CMusCallEventMonitor::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallEventMonitor* UT_CMusCallEventMonitor::NewLC()
    {
    UT_CMusCallEventMonitor* self = new( ELeave ) UT_CMusCallEventMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallEventMonitor::~UT_CMusCallEventMonitor()
    {
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusCallEventMonitor::UT_CMusCallEventMonitor()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusCallEventMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    
// -----------------------------------------------------------------------------
// UT_CMusCallEventMonitor::NotifyCallStateChanged() 
// -----------------------------------------------------------------------------
//
void UT_CMusCallEventMonitor::NotifyCallStateChanged(
									NMusSessionInformationApi::TMusCallEvent /*aVal*/,
									TName& /*aCallName*/ )
    {          
    }
    
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallEventMonitor::SetupL()
    {    
    
    //setup
    /*
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMusCallCount,
                    1 );
    */
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     0 );
    
    iCallEventMonitor = CMusCallEventMonitor::NewL( iCall, *this );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallEventMonitor::Teardown()
    {
    delete iCallEventMonitor;    
    PropertyHelper::Close();
    iCall.Close();
    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallEventMonitor::UT_CMusCallEventMonitor_RunLL()
    {
    /* Test configurations */
    iCallEventMonitor->iRemoteCallEvent = RMobileCall::ERemoteResume;
    iCallEventMonitor->iLocalCallEvent = RMobileCall::ELocalResume;
    
    // Test : Local is on hold
    TInt val = RMobileCall::EStatusUnknown;
    EUNIT_ASSERT( iCallEventMonitor );
    iCallEventMonitor->Cancel();
    RTelHelper::SetCallEvent( RMobileCall::ELocalHold );
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     1 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallHold);

	// Test : Local resumed from hold
    iCallEventMonitor->Cancel();
    RTelHelper::SetCallEvent( RMobileCall::ELocalResume );
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallConnected);

	// Test : Local Transfered. Means no call.
    iCallEventMonitor->Cancel();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMusCallEvent,
                    val );
    TInt callEnt = val;
    RTelHelper::SetCallEvent( RMobileCall::ELocalTransfer );    
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
                     
    EUNIT_ASSERT( callEnt==val );
    
	// Test : Remote is on hold
    iCallEventMonitor->Cancel();
    RTelHelper::SetCallEvent( RMobileCall::ERemoteHold );
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallHold);

    // Test : Remote resumed
    iCallEventMonitor->Cancel();
    RTelHelper::SetCallEvent( RMobileCall::ERemoteResume );
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallConnected);

    /*. When call goes to Nocall state , call info not needed to
        to be checked.Bug exist in both 5.0 and MCL . Should be fixed in
        both the branches. Else this unit case will fail 19.12.2008
     
	 //Test : Remote terminated
    iCallEventMonitor->Cancel();
    RTelHelper::SetCallEvent( RMobileCall::ERemoteTerminated );
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);
     */   
    
	// Test : Remote is on conference
    iCallEventMonitor->Cancel();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMusCallEvent,
                    val );
    callEnt = val;
    RTelHelper::SetCallEvent( RMobileCall::ERemoteConferenceCreate );
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( callEnt==val );

	// Test : Remote conference transfer
    iCallEventMonitor->Cancel();
    RTelHelper::SetCallEvent( RMobileCall::ERemoteTransferring );
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==callEnt );

    // remote is connected state and local resume
    iCallEventMonitor->Cancel();
    RTelHelper::SetCallEvent( RMobileCall::ELocalResume );
    iCallEventMonitor->iRemoteCallEvent = RMobileCall::ERemoteResume;
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallConnected);  

    // remote is in hold state and local resume.
    iCallEventMonitor->Cancel();
    RTelHelper::SetCallEvent( RMobileCall::ELocalResume );
    iCallEventMonitor->iRemoteCallEvent = RMobileCall::ERemoteResume;
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallConnected );

    //local hold state and remote resumed
    iCallEventMonitor->Cancel();
    RTelHelper::SetCallEvent( RMobileCall::ERemoteResume );
    iCallEventMonitor->iLocalCallEvent = RMobileCall::ELocalHold;
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallHold);

     //local connected state and remote resumed
    iCallEventMonitor->Cancel();
    RTelHelper::SetCallEvent( RMobileCall::ERemoteResume );
    iCallEventMonitor->iLocalCallEvent = RMobileCall::ELocalResume;
    iCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallConnected);
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallEventMonitor::UT_CMusCallEventMonitor_DoCancelL()
    {
    EUNIT_ASSERT( iCallEventMonitor );
    iCallEventMonitor->DoCancel();
    EUNIT_ASSERT( RTelHelper::GetCalledFunction() == 
                                        RTelSubSessionBase::ECancelAsyncRequest )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallEventMonitor::UT_CMusCallEventMonitor_RunErrorL()
    {
    EUNIT_ASSERT( iCallEventMonitor );
    TInt error = KErrNotFound;
    EUNIT_ASSERT_EQUALS(iCallEventMonitor->RunError(error), KErrNone);
    EUNIT_ASSERT_EQUALS(iCallEventMonitor->RunError(KErrNone), KErrNone);
    }


   


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusCallEventMonitor,
    "UT_CMusCallEventMonitor",
    "UNIT" )

EUNIT_TEST(
    "RunL - test",
    "CMusCallStatusMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallEventMonitor_RunLL, Teardown)

EUNIT_TEST(
    "DoCancel - test",
    "CMusCallStatusMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallEventMonitor_DoCancelL, Teardown)

EUNIT_TEST(
    "RunError - test",
    "CMusCallStatusMonitor",
    "RunErrorL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallEventMonitor_RunErrorL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE


