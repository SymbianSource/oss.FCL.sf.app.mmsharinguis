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
#include "ut_mustsypropertymonitor.h"
#include "mustsypropertymonitor.h"
#include "mussettings.h"


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <CTSYDomainPSKeys.h>

_LIT(KTelNo,"1234");


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusTsyPropertyMonitor* UT_CMusTsyPropertyMonitor::NewL()
    {
    UT_CMusTsyPropertyMonitor* self = UT_CMusTsyPropertyMonitor::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusTsyPropertyMonitor* UT_CMusTsyPropertyMonitor::NewLC()
    {
    UT_CMusTsyPropertyMonitor* self = new( ELeave ) UT_CMusTsyPropertyMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusTsyPropertyMonitor::~UT_CMusTsyPropertyMonitor()
    {
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::SetupL()
    {
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     0 );
    MultimediaSharingSettings::SetPropertyValueL(
                                                MusSettingsKeys::KPrivacyExchange,
                                                MusSettingsKeys::EPrivacy );                                                   
    
    iMusTsyPropertyMonitor = CMusTsyPropertyMonitor::NewL( iPhone );
    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateUninitialized;
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::Setup2L()
    {
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     0 );
    MultimediaSharingSettings::SetPropertyValueL(
                                                MusSettingsKeys::KPrivacyExchange,
                                                MusSettingsKeys::ENoPrivacy );                                                   
    
    iMusTsyPropertyMonitor = CMusTsyPropertyMonitor::NewL( iPhone );
    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateUninitialized;
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::Teardown()
    {
    delete iMusTsyPropertyMonitor;
    PropertyHelper::Close();
    iPhone.Close();

    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_NewLL()
    {
    EUNIT_ASSERT( iMusTsyPropertyMonitor);
    EUNIT_ASSERT( MultimediaSharingSettings::PrivacySetting() );//feature is on by default
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iClirMonitor );
    
    MultimediaSharingSettings::SetPropertyValueL( MusSettingsKeys::KPrivacyExchange,
                                                  MusSettingsKeys::ENoPrivacy );
    delete iMusTsyPropertyMonitor;
    iMusTsyPropertyMonitor = NULL;
    
    iMusTsyPropertyMonitor = CMusTsyPropertyMonitor::NewL( iPhone );
    EUNIT_ASSERT( !MultimediaSharingSettings::PrivacySetting() );//feature is on by default
    EUNIT_ASSERT( !iMusTsyPropertyMonitor->iClirMonitor );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_RunLL()
    {
    EUNIT_ASSERT( iMusTsyPropertyMonitor);
    TInt val = KErrNone;
    
    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );

    
    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateNone;
    iMusTsyPropertyMonitor->SetStateL( NMusSessionInformationApi::ENoCall );

    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);
    

    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateDisconnecting;
    iMusTsyPropertyMonitor->SetStateL( NMusSessionInformationApi::ENoCall );

    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);

    
    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateAnswering;
    iMusTsyPropertyMonitor->SetStateL( NMusSessionInformationApi::ENoCall );

    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);

    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateConnected;
    iMusTsyPropertyMonitor->SetStateL( NMusSessionInformationApi::ENoCall );

    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);

    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateHold;
    iMusTsyPropertyMonitor->SetStateL( NMusSessionInformationApi::ENoCall );
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                 NMusSessionInformationApi::KMusCallCount,
                 1 ) != KErrNone )
    {
    User::Leave( KErrNoMemory );
    }
    
    //simulate multicall scenario
    TName callName(_L("Voice1"));
    iMusTsyPropertyMonitor->AddCallMonitorL( callName );
    callName.Copy(_L("Voice2"));
    iMusTsyPropertyMonitor->AddCallMonitorL( callName );
    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    //
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallHold);
    
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_DoCancelL()
    {
    EUNIT_ASSERT( iMusTsyPropertyMonitor );
    iMusTsyPropertyMonitor->Cancel();
    EUNIT_ASSERT( PropertyHelper::GetCalledFunction() == 
                                        RProperty::ECancel )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_RunErrorL()
    {
    EUNIT_ASSERT( iMusTsyPropertyMonitor );
    TInt error = KErrNotFound;
    iMusTsyPropertyMonitor->RunError(error);
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_AddCallMonitorLL()
    {
    TName name = KTelNo();
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    iMusTsyPropertyMonitor->AddCallMonitorL( name );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_RemoveCallMonitorL()
    {
    TName name = KTelNo();
    iMusTsyPropertyMonitor->RemoveCallMonitor( name );
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_MonitorCallLL()
    {
    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusIdle );
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    
    iMusTsyPropertyMonitor->MonitorCallL();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    
    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusHangingUp );
    
    iMusTsyPropertyMonitor->MonitorCallL();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    
    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusConnected );
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    iMusTsyPropertyMonitor->MonitorCallL();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 1 );
    
    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusAnswering );
    //already exist
    iMusTsyPropertyMonitor->MonitorCallL();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 1 );
    

    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusUnknown );
    
    iMusTsyPropertyMonitor->MonitorCallL();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );

    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusIdle );
    iMusTsyPropertyMonitor->MonitorCallL();
    
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_MonitorCallL_2L()
    {
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    EUNIT_ASSERT( !iMusTsyPropertyMonitor->iClirMonitor );
    
    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusConnected );
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    iMusTsyPropertyMonitor->MonitorCallL();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 1 );
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_RemoveUnUsedCallMonitorsL()
    {
    TName callName(_L("Voice1"));
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    iMusTsyPropertyMonitor->AddCallMonitorL( callName );
    callName.Copy(_L("Voice2"));
    iMusTsyPropertyMonitor->AddCallMonitorL( callName );
    RTelHelper::SetCallStatus(RMobileCall::EStatusUnknown);
    iMusTsyPropertyMonitor->RemoveUnUsedCallMonitors();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count()==0 );    
    }
   


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusTsyPropertyMonitor,
    "UT_CMusTsyPropertyMonitor",
    "UNIT" )

    
EUNIT_TEST(
    "NewL - test",
    "CMusTsyPropertyMonitor",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_NewLL, Teardown)

EUNIT_TEST(
    "RunL - test",
    "CMusTsyPropertyMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_RunLL, Teardown)

EUNIT_TEST(
    "DoCancel - test",
    "CMusTsyPropertyMonitor",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_DoCancelL, Teardown)

EUNIT_TEST(
    "RunError - test",
    "CMusTsyPropertyMonitor",
    "RunError",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_RunErrorL, Teardown)

EUNIT_TEST(
    "RemoveUnUsedCallMonitorsL",
    "CMusCallStatusMonitor",
    "RemoveUnUsedCallMonitors",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_RemoveUnUsedCallMonitorsL, Teardown)

EUNIT_TEST(
    "AddCallMonitorL - test",
    "CMusTsyPropertyMonitor",
    "AddCallMonitorL",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_AddCallMonitorLL, Teardown)

EUNIT_TEST(
    "RemoveCallMonitor - test",
    "CMusTsyPropertyMonitor",
    "RemoveCallMonitor",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_RemoveCallMonitorL, Teardown)
    
EUNIT_TEST(
    "MonitorCallL - test",
    "CMusTsyPropertyMonitor",
    "MonitorCallL",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_MonitorCallLL, Teardown)

EUNIT_TEST(
    "MonitorCallL no clir - test",
    "CMusTsyPropertyMonitor",
    "MonitorCallL",
    "FUNCTIONALITY",
    Setup2L, UT_CMusTsyPropertyMonitor_MonitorCallL_2L, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE


