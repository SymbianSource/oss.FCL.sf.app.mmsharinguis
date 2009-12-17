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


#include "ut_musaocalleventmonitor.h"
#include "musaocalleventmonitor.h"
#include "mussesseioninformationapi.h"

/*
#include "muscallmonitor.h"
#include "muscalleventmonitor.h"
#include "musaoplugin.h"
#include "ut_muscalleventmonitor.h"
#include "mussesseioninformationapi.h"
*/

#include <etelmm.h>
#include <etel.h>
#include <digia/eunit/eunitmacros.h>
#include <e32property.h>


#pragma warn_illtokenpasting off

_LIT( KEmptyTelNumber, "");


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
UT_CMusAoCallEventMonitor* UT_CMusAoCallEventMonitor::NewLC()
    {    
    UT_CMusAoCallEventMonitor* self = new (ELeave) UT_CMusAoCallEventMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();    
    return self;
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void UT_CMusAoCallEventMonitor::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAoCallEventMonitor::CallConnectedL( const TDesC& /*aTelNumber*/ )
    {
    iCallConnected = ETrue;
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAoCallEventMonitor::CallHoldL( const TDesC& /*aTelNumber*/ )
    {
    iCallHold = ETrue;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAoCallEventMonitor::ConferenceCallL()
    {
    iConferenceCall = ETrue;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAoCallEventMonitor::NoActiveCallL()
    {
    iNoActiveCall = ETrue;
    }


// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
UT_CMusAoCallEventMonitor::UT_CMusAoCallEventMonitor() 
    : CEUnitTestSuiteClass()
    {
    // nothing
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
UT_CMusAoCallEventMonitor::~UT_CMusAoCallEventMonitor()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAoCallEventMonitor::UT_ConnectedCallCountLL()
    {
    iMonitor->iCallCount = 2;
    iMonitor->ConnectedCallCountL();
    EUNIT_ASSERT_EQUALS( iMonitor->ConnectedCallCountL(), 2);
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAoCallEventMonitor::UT_RunLL()
    {    
    ResetObserverFlags();
    iMonitor->RunL();
    iMonitor->Cancel();
    EUNIT_ASSERT( iNoActiveCall );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAoCallEventMonitor::UT_ExamineEventLL()
    {

    ResetObserverFlags();
    iMonitor->ExamineEventL( NMusSessionInformationApi::ENoCall );
    EUNIT_ASSERT( iNoActiveCall )

    ResetObserverFlags();
    iMonitor->ExamineEventL( NMusSessionInformationApi::EConferenceCall );
    EUNIT_ASSERT( iConferenceCall )

   // Without telephony number
    ResetObserverFlags();
    iMonitor->ExamineEventL( NMusSessionInformationApi::ECallConnected );
    EUNIT_ASSERT( iNoActiveCall )

    ResetObserverFlags();
    iMonitor->ExamineEventL( NMusSessionInformationApi::ECallHold );
    EUNIT_ASSERT( iNoActiveCall )


    // 6. telephone number
    RProperty::Define( NMusSessionInformationApi::KCategoryUid,
                       NMusSessionInformationApi::KMusTelNumber,
                       RProperty::EText,
                       0 );
    User::LeaveIfError( RProperty::Set(
                        NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMusTelNumber,
                        KEmptyTelNumber ) );


    // With telephony number
    ResetObserverFlags();
    iMonitor->ExamineEventL( NMusSessionInformationApi::ECallConnected );
    EUNIT_ASSERT( iCallConnected )

    ResetObserverFlags();
    iMonitor->ExamineEventL( NMusSessionInformationApi::ECallHold );
    EUNIT_ASSERT( iCallHold )

    // 6. telephone number
    RProperty::Delete( NMusSessionInformationApi::KCategoryUid,
                       NMusSessionInformationApi::KMusTelNumber );

    RProperty::Delete( NMusSessionInformationApi::KCategoryUid,
                       NMusSessionInformationApi::KMusCallCount );
     

    // 7. call count
    RProperty::Define( NMusSessionInformationApi::KCategoryUid,
                       NMusSessionInformationApi::KMusCallCount,
                       RProperty::EInt,
                       0 );
    ResetObserverFlags();
    iMonitor->ExamineEventL( KErrNotFound );
    EUNIT_ASSERT( iNoActiveCall ); 

    // 7. call count
    RProperty::Delete( NMusSessionInformationApi::KCategoryUid,
                       NMusSessionInformationApi::KMusCallCount );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAoCallEventMonitor::SetupL()
    {    
    RMobilePhone phone;
    iMonitor = CMusAoCallEventMonitor::NewL( *this );
    iMonitor->Cancel();

    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAoCallEventMonitor::Teardown()
    {    
    delete iMonitor;    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAoCallEventMonitor::ResetObserverFlags()
    {
    iCallConnected = EFalse; 
    iCallHold = EFalse; 
    iConferenceCall = EFalse; 
    iNoActiveCall = EFalse; 
    }


// test table

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusAoCallEventMonitor,
    "UT_CMusAoCallEventMonitor",
    "UNIT" )

EUNIT_TEST(
    "UT_ConnectedCallCountL",
    "CMusCallEventMonitor",
    "ConnectedCallCountL",
    "FUNCTIONALITY",
    SetupL, UT_ConnectedCallCountLL, Teardown)

EUNIT_TEST(
    "UT_RunL",
    "CMusCallEventMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_RunLL, Teardown)

EUNIT_TEST(
    "UT_ExamineEventL",
    "CMusCallEventMonitor",
    "ExamineEventL",
    "FUNCTIONALITY",
    SetupL, UT_ExamineEventLL, Teardown)


EUNIT_END_TEST_TABLE
