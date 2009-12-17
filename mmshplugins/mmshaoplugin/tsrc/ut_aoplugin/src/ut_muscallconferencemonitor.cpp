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
#include "etel.h"
#include "etelmm.h"
#include "ut_muscallconferencemonitor.h"
#include "muscallconferencemonitor.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallConferenceMonitor* UT_CMusCallConferenceMonitor::NewL()
    {
    UT_CMusCallConferenceMonitor* self = UT_CMusCallConferenceMonitor::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallConferenceMonitor* UT_CMusCallConferenceMonitor::NewLC()
    {
    UT_CMusCallConferenceMonitor* self = new( ELeave ) UT_CMusCallConferenceMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallConferenceMonitor::~UT_CMusCallConferenceMonitor()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusCallConferenceMonitor::UT_CMusCallConferenceMonitor()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceMonitor::SetupL()
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
    
    iConferenceCallMonitor = 
                    CMusCallConferenceMonitor::NewL(  iMobilePhone,
                                                      iLine ,
                                                      iCallMonitorArray );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceMonitor::Teardown()
    {
    delete iConferenceCallMonitor;
    PropertyHelper::Close();
    iMobilePhone.Close();
    iLine.Close();
    iCallMonitorArray.ResetAndDestroy();
    
    }



// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceMonitor::UT_CMusCallConferenceMonitor_NewLL()
    {
    EUNIT_ASSERT( iConferenceCallMonitor );
    EUNIT_ASSERT( iConferenceCallMonitor->IsActive() );    
    EUNIT_ASSERT( iConferenceCallMonitor->iConfCallEvent );        
    }

 
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceMonitor::UT_CMusCallConferenceMonitor_RunLL()
    {
    TInt val = RMobileCall::EStatusUnknown;
    EUNIT_ASSERT( iConferenceCallMonitor );

    iConferenceCallMonitor->Cancel(); // Cancel the active object        
    RTelHelper::SetConfStatus( RMobileConferenceCall::EConferenceIdle );
    iConferenceCallMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val == RMobileCall::EStatusUnknown );

    RTelHelper::SetConfStatus( RMobileConferenceCall::EConferenceActive );
    iConferenceCallMonitor->Cancel(); // Cancel the active object    
    iConferenceCallMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::EConferenceCall);

    RTelHelper::SetConfStatus( RMobileConferenceCall::EConferenceHold );
    iConferenceCallMonitor->Cancel(); // Cancel the active object    
    iConferenceCallMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallHold);

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceMonitor::UT_CMusCallConferenceMonitor_DoCancelL()
    {
    EUNIT_ASSERT( iConferenceCallMonitor );
    iConferenceCallMonitor->DoCancel();
    EUNIT_ASSERT( RTelHelper::GetCalledFunction() == 
                                        RTelSubSessionBase::ECancelAsyncRequest )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceMonitor::UT_CMusCallConferenceMonitor_RunErrorL()
    {
    EUNIT_ASSERT( iConferenceCallMonitor );
    TInt error = KErrNotFound;
    iConferenceCallMonitor->RunError(error);
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceMonitor::UT_CMusCallConferenceMonitor_MonitorCallLL()
    {
    EUNIT_ASSERT( iConferenceCallMonitor );
    EUNIT_ASSERT( iConferenceCallMonitor->iCallMonitorArray.Count() == 0 );
    
    iConferenceCallMonitor->MonitorCallL();
    EUNIT_ASSERT( iConferenceCallMonitor->iCallMonitorArray.Count() == 0 );
    
    RTelHelper::SetCallStatus( RMobileCall::EStatusConnected );
    iConferenceCallMonitor->MonitorCallL();
    EUNIT_ASSERT( iConferenceCallMonitor->iCallMonitorArray.Count() == 0 );
    
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusCallConferenceMonitor,
    "UT_CMusCallConferenceMonitor",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusConferenceCallMonitor",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallConferenceMonitor_NewLL, Teardown)

EUNIT_TEST(
    "RunL - test ",
    "CMusConferenceCallMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallConferenceMonitor_RunLL, Teardown)

EUNIT_TEST(
    "DoCancel - test ",
    "CMusConferenceCallMonitor",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallConferenceMonitor_DoCancelL, Teardown)

EUNIT_TEST(
    "RunError - test ",
    "CMusConferenceCallMonitor",
    "RunError",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallConferenceMonitor_RunErrorL, Teardown)
    
EUNIT_TEST(
    "MonitorCallL - test ",
    "CMusConferenceCallMonitor",
    "MonitorCallL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallConferenceMonitor_MonitorCallLL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE


