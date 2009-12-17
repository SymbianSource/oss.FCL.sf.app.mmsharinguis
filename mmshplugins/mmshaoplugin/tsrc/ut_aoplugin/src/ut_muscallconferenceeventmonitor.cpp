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
#include "ut_muscallconferenceeventmonitor.h"
#include "muscallconferenceeventmonitor.h"
#include "e32property.h"
//#include "muscommonstubs.h" 

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallConferenceEventMonitor* UT_CMusCallConferenceEventMonitor::NewL()
    {
    UT_CMusCallConferenceEventMonitor* self = UT_CMusCallConferenceEventMonitor::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallConferenceEventMonitor* UT_CMusCallConferenceEventMonitor::NewLC()
    {
    UT_CMusCallConferenceEventMonitor* self = new( ELeave ) UT_CMusCallConferenceEventMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallConferenceEventMonitor::~UT_CMusCallConferenceEventMonitor()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusCallConferenceEventMonitor::UT_CMusCallConferenceEventMonitor()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceEventMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceEventMonitor::SetupL()
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
    
    iConferenceCallEventMonitor = 
            CMusCallConferenceEventMonitor::NewL( iMobilePhone,
                                                  iLine ,
                                                  iCallMonitorArray );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceEventMonitor::Teardown()
    {
    delete iConferenceCallEventMonitor;
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
void UT_CMusCallConferenceEventMonitor::UT_CMusCallConferenceEventMonitor_NewLL()
    {
    EUNIT_ASSERT( iConferenceCallEventMonitor )
    }

 
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceEventMonitor::UT_CMusCallConferenceEventMonitor_RunLL()
    {
    TInt val = RMobileConferenceCall::EConferenceCallRemoved ;
    EUNIT_ASSERT( iConferenceCallEventMonitor );

    RTelHelper::SetConfEvent( RMobileConferenceCall::EConferenceCallRemoved );
    iConferenceCallEventMonitor->SetStateL( NMusSessionInformationApi::ENoCall );                               
    iConferenceCallEventMonitor->Cancel(); // Cancel the active object            
    iConferenceCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);

    RTelHelper::SetConfEvent ( RMobileConferenceCall::EConferenceBuilt);
    iConferenceCallEventMonitor->Cancel(); // Cancel the active object    
    iConferenceCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::EConferenceCall);

    RTelHelper::SetConfEvent( RMobileConferenceCall::EConferenceTerminated );
    iConferenceCallEventMonitor->SetStateL( NMusSessionInformationApi::ENoCall );                               
    iConferenceCallEventMonitor->Cancel(); // Cancel the active object    
    iConferenceCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);

    RTelHelper::SetConfEvent( RMobileConferenceCall::EConferenceSwapped );
    iConferenceCallEventMonitor->SetStateL( NMusSessionInformationApi::ENoCall );                               
    iConferenceCallEventMonitor->Cancel(); // Cancel the active object    
    iConferenceCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::EConferenceCall);

    RTelHelper::SetConfEvent( RMobileConferenceCall::EConferenceSplit);
    iConferenceCallEventMonitor->SetStateL( NMusSessionInformationApi::ENoCall );                               
    iConferenceCallEventMonitor->Cancel(); // Cancel the active object    
    iConferenceCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::EConferenceCall );
    
    RTelHelper::SetConfEvent( (RMobileConferenceCall::TMobileConferenceEvent)152090 );
    iConferenceCallEventMonitor->SetStateL( NMusSessionInformationApi::ENoCall );                               
    iConferenceCallEventMonitor->SetStateL( NMusSessionInformationApi::ENoCall );                               
    iConferenceCallEventMonitor->Cancel(); // Cancel the active object    
    iConferenceCallEventMonitor->RunL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceEventMonitor::UT_CMusCallConferenceEventMonitor_DoCancelL()
    {
    EUNIT_ASSERT( iConferenceCallEventMonitor );
    iConferenceCallEventMonitor->DoCancel();
    EUNIT_ASSERT( RTelHelper::GetCalledFunction() == 
                                        RTelSubSessionBase::ECancelAsyncRequest )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceEventMonitor::UT_CMusCallConferenceEventMonitor_RunErrorL()
    {
    EUNIT_ASSERT( iConferenceCallEventMonitor );
    TInt error = KErrNotFound;
    iConferenceCallEventMonitor->RunError(error);
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallConferenceEventMonitor::UT_CMusCallConferenceEventMonitor_MonitorCallLL()
    {
    EUNIT_ASSERT( iConferenceCallEventMonitor );
    EUNIT_ASSERT( iConferenceCallEventMonitor->iCallMonitorArray.Count() == 0 );
    
    iConferenceCallEventMonitor->MonitorCallL();
    EUNIT_ASSERT( iConferenceCallEventMonitor->iCallMonitorArray.Count() == 0 );
    
    RTelHelper::SetCallStatus( RMobileCall::EStatusConnected );
    iConferenceCallEventMonitor->MonitorCallL();
    EUNIT_ASSERT( iConferenceCallEventMonitor->iCallMonitorArray.Count() == 0 );
    
    }




//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusCallConferenceEventMonitor,
    "UT_CMusCallConferenceEventMonitor",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusCallConferenceEventMonitor",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallConferenceEventMonitor_NewLL, Teardown)

EUNIT_TEST(
    "RunL - test ",
    "CMusCallConferenceEventMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallConferenceEventMonitor_RunLL, Teardown)

EUNIT_TEST(
    "DoCancel - test ",
    "CMusCallConferenceEventMonitor",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallConferenceEventMonitor_DoCancelL, Teardown)

EUNIT_TEST(
    "RunError - test ",
    "CMusCallConferenceEventMonitor",
    "RunError",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallConferenceEventMonitor_RunErrorL, Teardown)

EUNIT_TEST(
    "MonitorCallL - test ",
    "CMusCallConferenceEventMonitor",
    "MonitorCallL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallConferenceEventMonitor_MonitorCallLL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE


