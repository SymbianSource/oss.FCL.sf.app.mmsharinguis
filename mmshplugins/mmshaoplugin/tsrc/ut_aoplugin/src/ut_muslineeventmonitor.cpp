/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Monitor that observes the status of the current CS call
*
*/


#include "ut_muslineeventmonitor.h"
#include "muslineeventmonitor.h"
#include "muscalleventmonitor.h"
#include "muspttcallmonitor.h"

//#include "musmanagerstub.h"    
//#include "muscallmonitorobserver.h"
 
#include <etelmm.h>
#include <e32svr.h>
#include <mmtsy_names.h>
#include <digia/eunit/eunitmacros.h>

#ifdef RD_KODIAK_POC 
    #include <CTSYDomainPSKeys.h>
#else
    //just for compilation . we never use this.
    const TUid KPSUidCtsyCallInformation = {0x102029AC};
    const TUint32 KCTsyKodiakPtt = 0x00000003;    
    enum TPSCTsyKodiakPtt
     {
     EPSCTsyKodiakPttNotActive,
     EPSCTsyKodiakPttActive
     };
#endif

#pragma warn_illtokenpasting off

// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
UT_CMusLineEventMonitor* UT_CMusLineEventMonitor::NewLC()
    {    
    UT_CMusLineEventMonitor* self = new (ELeave) UT_CMusLineEventMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();    
    return self;
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void UT_CMusLineEventMonitor::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusLineEventMonitor::StatusChanged()
    {
    }
    

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
UT_CMusLineEventMonitor::UT_CMusLineEventMonitor() 
    : CEUnitTestSuiteClass()
    {
    // nothing
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
UT_CMusLineEventMonitor::~UT_CMusLineEventMonitor()
    {
    }
   
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusLineEventMonitor::UT_GetLineL()
    {
    RMobileLine& line = iMonitor->GetLine();

    // Just check something is returned..
    RLine::TCallInfo callInfo;
    User::LeaveIfError( line.GetCallInfo( 0, callInfo ) );
    EUNIT_ASSERT( callInfo.iStatus );
    }
    
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusLineEventMonitor::UT_RemoteHoldCountL()
    {

    // without any calls
    EUNIT_ASSERT( iMonitor->RemoteHoldCount() == 0 )
    
    // Local hold
    _LIT( callname, "MyCall" );
    CMusCallEventMonitor* callEventMonitor = 
                                CMusCallEventMonitor::NewL( iMonitor->GetLine(), 
                                                            callname,
                                                            *this );             
    CleanupStack::PushL( callEventMonitor );

    callEventMonitor->iCallEvent = RMobileCall::ELocalHold;        
    iMonitor->iCallEventArray.AppendL( callEventMonitor );
    CleanupStack::Pop( callEventMonitor );
    EUNIT_ASSERT( iMonitor->RemoteHoldCount() == 0 )

    // Remote hold
    callEventMonitor = CMusCallEventMonitor::NewL( iMonitor->GetLine(), 
                                                   callname,
                                                   *this );             

    CleanupStack::PushL( callEventMonitor );
    callEventMonitor->iCallEvent = RMobileCall::ERemoteHold;        
    iMonitor->iCallEventArray.AppendL( callEventMonitor );
    CleanupStack::Pop( callEventMonitor );
    EUNIT_ASSERT( iMonitor->RemoteHoldCount() == 1 )

    // Cleanup
    iMonitor->iCallEventArray.ResetAndDestroy();
    
    }
   

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusLineEventMonitor::UT_RunLL()
    {
    
    //RCall::whatever
    RTelHelper::SetCallStatus( RMobileCall::EStatusHold );    
    iMonitor->RunL();
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 0 );
    iMonitor->Cancel();
    
    //RCall::EStatusConnected
    RTelHelper::SetCallStatus( RMobileCall::EStatusConnected );
    iMonitor->RunL();
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 1 );
    iMonitor->Cancel();

    //RCall::EStatusIdle
    RTelHelper::SetCallStatus( RMobileCall::EStatusIdle );
    iMonitor->RunL();
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 0 );
    iMonitor->Cancel();

    //RCall::EStatusConnected
    RTelHelper::SetCallStatus( RMobileCall::EStatusConnected );
    iMonitor->RunL();
    iMonitor->Cancel();

    //RCall::EStatusHangingUp
    RTelHelper::SetCallStatus( RMobileCall::EStatusDisconnecting );
    iMonitor->RunL();
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 0 );
    iMonitor->Cancel();
    
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusLineEventMonitor::UT_DoCancelL()
    {
    iMonitor->DoCancel();
    EUNIT_ASSERT( RTelHelper::GetCalledFunction()==
                         RTelSubSessionBase::ECancelAsyncRequest )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusLineEventMonitor::UT_RunErrorL()
    {
    EUNIT_ASSERT_EQUALS( iMonitor->RunError( KErrNone ), KErrNone );
    // Make sure RunError handles the error compleatly and returns
    // KErrNone
    EUNIT_ASSERT( iMonitor->RunError( KErrGeneral ) == KErrNone );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusLineEventMonitor::UT_AddCallEventMonitorLL()
    {
    RProperty::Set(KPSUidCtsyCallInformation, KCTsyKodiakPtt,                       
                       EPSCTsyKodiakPttActive );
    iMonitor->iPTTCallMonitor->Cancel();
    iMonitor->iPTTCallMonitor->RunL();
    iMonitor->iPTTCallMonitor->Cancel();
    // Add first call
    _LIT( KCallName, "FirstCall" );
    iMonitor->AddCallEventMonitorL( KCallName() );
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 1 );

    // Trying add same call again 
    iMonitor->AddCallEventMonitorL( KCallName() );
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 1 );

    // Add second call
    _LIT( KCallName2, "SecondCall" );
    iMonitor->AddCallEventMonitorL( KCallName2() );
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 2 );

    RProperty::Set(KPSUidCtsyCallInformation, KCTsyKodiakPtt,                       
                       EPSCTsyKodiakPttNotActive );
    iMonitor->iPTTCallMonitor->RunL();
    iMonitor->iPTTCallMonitor->Cancel();
    // Add first call    
    iMonitor->AddCallEventMonitorL( KCallName() );
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 0 );  
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusLineEventMonitor::UT_RemoveCallEventMonitorLL()
    {

    // Precondition add call
    _LIT( KCallName, "FirstCall" );
    iMonitor->AddCallEventMonitorL( KCallName() );
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 1 );
    
    _LIT( KCallName2, "SecondCall" );
    iMonitor->AddCallEventMonitorL( KCallName2() );
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 2 );

    // Remove call with wrong name
    _LIT( KCallName3, "ThirdCall" );
    iMonitor->RemoveCallEventMonitorL( KCallName3() );
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 2 );
    
    // Remove second call remove 
    iMonitor->RemoveCallEventMonitorL( KCallName2() );
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 1 );

    // Remove added call
    iMonitor->RemoveCallEventMonitorL( KCallName() );
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 0 );

    // Trying overload remove 
    iMonitor->RemoveCallEventMonitorL( KCallName() );
    EUNIT_ASSERT_EQUALS( iMonitor->iCallEventArray.Count(), 0 );
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusLineEventMonitor::SetupL()
    {
    RMobilePhone phone;
    iMonitor = CMusLineEventMonitor::NewL( phone, *this );
    iMonitor->Cancel();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void UT_CMusLineEventMonitor::Teardown()
    {
    delete iMonitor;
    }

// test table

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusLineEventMonitor,
    "UT_CMusLineEventMonitor",
    "UNIT" )

EUNIT_TEST(
    "GetLine - test",
    "CMusAoCallMonitor",
    "GetLine",
    "FUNCTIONALITY",
    SetupL, UT_GetLineL, Teardown)

EUNIT_TEST(
    "RemoteHoldCount - test",
    "CMusAoCallMonitor",
    "RemoteHoldCount",
    "FUNCTIONALITY",
    SetupL, UT_RemoteHoldCountL, Teardown)

EUNIT_TEST(
    "RunL - test",
    "CMusAoCallMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_RunLL, Teardown)

EUNIT_TEST(
    "DoCancel - test",
    "CMusAoCallMonitor",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_DoCancelL, Teardown)

EUNIT_TEST(
    "RunError - test",
    "CMusAoCallMonitor",
    "RunError",
    "FUNCTIONALITY",
    SetupL, UT_RunErrorL, Teardown)

EUNIT_TEST(
    "AddCallEventMonitorL - test",
    "CMusAoCallMonitor",
    "AddCallEventMonitorL",
    "FUNCTIONALITY",
    SetupL, UT_AddCallEventMonitorLL, Teardown)

EUNIT_TEST(
    "RemoveCallEventMonitorL - test",
    "CMusAoCallMonitor",
    "RemoveCallEventMonitorL",
    "FUNCTIONALITY",
    SetupL, UT_RemoveCallEventMonitorLL, Teardown)





EUNIT_END_TEST_TABLE
