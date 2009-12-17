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
* Description:  Monitor that observes the calling line identity restriction
*                setting
*
*/

#include "musclirmonitor.h"
#include "ut_musclirmonitor.h"
#include "e32property.h"

#include <etelmm.h>
#include <etel.h>
#include <mmtsy_names.h>
#include <digia/eunit/eunitmacros.h>


#pragma warn_illtokenpasting off

// Constants


// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::NewLC
// -----------------------------------------------------------------------------
//
UT_CMusClirMonitor* UT_CMusClirMonitor::NewLC()
    {    
    UT_CMusClirMonitor* self = new (ELeave) UT_CMusClirMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::ConstructL
// -----------------------------------------------------------------------------
//
void UT_CMusClirMonitor::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::UT_CMusClirMonitor
// -----------------------------------------------------------------------------
//
UT_CMusClirMonitor::UT_CMusClirMonitor() :
    CEUnitTestSuiteClass()
    {
    }


// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::~UT_CMusClirMonitor
// -----------------------------------------------------------------------------
//
UT_CMusClirMonitor::~UT_CMusClirMonitor()
    {
    }

   
// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::UT_ReadClir
// -----------------------------------------------------------------------------
//
void UT_CMusClirMonitor::UT_ReadClir()
    {
    // Cancel initial request
    iMonitor->Cancel();
    
    iMonitor->ReadClir();
    EUNIT_ASSERT( iMonitor->IsActive() );
    
    // Monitor is already reading, new attempts do nothing 
    iMonitor->ReadClir();
    iMonitor->ReadClir();
    
    EUNIT_ASSERT_EQUALS( iMonitor->iErrorCount, 0 );
    }


// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::UT_RunL
// -----------------------------------------------------------------------------
//
void UT_CMusClirMonitor::UT_RunL()
    {
    CTelephony::TIdentityServiceStatus value1 =
        CTelephony::EIdServiceActivePermanent;
    CTelephony::TIdentityServiceStatus value2 =
            CTelephony::EIdServiceNotProvisioned;

    EUNIT_ASSERT( iMonitor->iAllowed );
    iMonitor->Cancel();
    iMonitor->iTIdentityServiceV1.iIdentityStatus = value1;
    iMonitor->iStatus = KErrNone;    
    iMonitor->RunL();
    EUNIT_ASSERT( !iMonitor->iAllowed );
    EUNIT_ASSERT_EQUALS( iMonitor->iErrorCount, 0 );
    
    
    iMonitor->Cancel();
    iMonitor->iTIdentityServiceV1.iIdentityStatus = value2;
    iMonitor->iStatus = KErrGeneral;
    iMonitor->RunL();
    // iAllowed value does not change because request completed with error
    EUNIT_ASSERT( !iMonitor->iAllowed );
    EUNIT_ASSERT_EQUALS( iMonitor->iErrorCount, 1 );
    
   
    iMonitor->Cancel();
    iMonitor->iTIdentityServiceV1.iIdentityStatus = value2;
    iMonitor->iStatus = KErrNone;
    iMonitor->RunL();
    EUNIT_ASSERT( iMonitor->iAllowed );
    // After successful read, error count is reset
    EUNIT_ASSERT_EQUALS( iMonitor->iErrorCount, 0 );
    }


// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::UT_RunLErrorsL
// Test that if reading fails, it is retried few times. 
// -----------------------------------------------------------------------------
//
void UT_CMusClirMonitor::UT_RunLErrorsL()
    {
    // Must have same value as in musclirmonitor.cpp
    const TInt KMaxErrorsBeforeStop = 5;

    EUNIT_ASSERT_EQUALS( iMonitor->iErrorCount, 0 );
    
    for ( TInt i = 1; i <= KMaxErrorsBeforeStop; ++i )
        {
        // Reading fails
        iMonitor->Cancel();
        iMonitor->iStatus = KErrNotSupported;
        iMonitor->RunL();
        EUNIT_ASSERT_EQUALS( iMonitor->iErrorCount, i );
        // Retry started
        EUNIT_ASSERT( iMonitor->IsActive() );
        }
    
    // Reading fails
    iMonitor->Cancel();
    iMonitor->iStatus = KErrNotSupported;
    iMonitor->RunL();
    EUNIT_ASSERT_EQUALS( iMonitor->iErrorCount, KMaxErrorsBeforeStop + 1 );

    // No more retries
    EUNIT_ASSERT( !iMonitor->IsActive() );
    }

// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::UT_DoCancel
// -----------------------------------------------------------------------------
//
void UT_CMusClirMonitor::UT_DoCancel()
    {
    EUNIT_ASSERT( iMonitor->IsActive() );

    // Can't call DoCancel directly, must call it via the CActive::Cancel
    iMonitor->Cancel();
    
    EUNIT_ASSERT( !iMonitor->IsActive() );
    }


// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::UT_RunError
// -----------------------------------------------------------------------------
//
void UT_CMusClirMonitor::UT_RunError()
    {
    EUNIT_ASSERT( iMonitor->RunError( KErrNone ) == KErrNone );
    EUNIT_ASSERT( iMonitor->RunError( KErrGeneral ) == KErrNone );
    EUNIT_ASSERT( iMonitor->RunError( 48 ) == KErrNone );
    EUNIT_ASSERT( iMonitor->RunError( -2092 ) == KErrNone );
    }


// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::UT_TranslateResult
// -----------------------------------------------------------------------------
//
void UT_CMusClirMonitor::UT_TranslateResult()
    {
    iMonitor->iAllowed = EFalse;
    
    iMonitor->TranslateResult( CTelephony::EIdServiceActiveDefaultAllowed );
    EUNIT_ASSERT( iMonitor->iAllowed );
    
    iMonitor->TranslateResult( CTelephony::EIdServiceActivePermanent );
    EUNIT_ASSERT( !iMonitor->iAllowed );
    
    iMonitor->TranslateResult( CTelephony::EIdServiceNotProvisioned );
    EUNIT_ASSERT( iMonitor->iAllowed );
    
    iMonitor->TranslateResult( CTelephony::EIdServiceActiveDefaultRestricted );
    EUNIT_ASSERT( !iMonitor->iAllowed );
    
    iMonitor->TranslateResult( CTelephony::EIdServiceUnknown );
    EUNIT_ASSERT( !iMonitor->iAllowed );
    }

// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::SetupL
// -----------------------------------------------------------------------------
//
void UT_CMusClirMonitor::SetupL()
    {        
    iMonitor = CMusClirMonitor::NewL();
    }


// -----------------------------------------------------------------------------
// UT_CMusClirMonitor::Teardown
// -----------------------------------------------------------------------------
//
void UT_CMusClirMonitor::Teardown()
    {
    delete iMonitor;
    iMonitor = NULL;
    PropertyHelper::Close();
    
    }

// test table

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusClirMonitor,
    "UT_CMusClirMonitor",
    "UNIT" )

EUNIT_TEST(
    "UT_ReadClir",
    "CMusClirMonitor",
    "ReadClir",
    "FUNCTIONALITY",
    SetupL, UT_ReadClir, Teardown)

EUNIT_TEST(
    "UT_RunL",
    "CMusClirMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_RunL, Teardown)

EUNIT_TEST(
    "UT_RunLErrorsL",
    "CMusClirMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_RunLErrorsL, Teardown)

EUNIT_TEST(
    "UT_DoCancel",
    "CMusClirMonitor",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_DoCancel, Teardown)

EUNIT_TEST(
    "UT_RunError",
    "CMusClirMonitor",
    "RunError",
    "FUNCTIONALITY",
    SetupL, UT_RunError, Teardown)

EUNIT_TEST(
    "UT_TranslateResult",
    "CMusClirMonitor",
    "TranslateResult",
    "FUNCTIONALITY",
    SetupL, UT_TranslateResult, Teardown)

EUNIT_END_TEST_TABLE
