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


#include "ut_musconferencecallmonitor.h"
#include "musconferencecallmonitor.h"

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

// Constants
_LIT( KMusTestCallName , "Voice1");


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
UT_CMusConferenceCallMonitor* UT_CMusConferenceCallMonitor::NewLC()
    {    
    UT_CMusConferenceCallMonitor* self = new (ELeave) UT_CMusConferenceCallMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();    
    return self;
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void UT_CMusConferenceCallMonitor::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
// Observer function
// -----------------------------------------------------------------------------
//
void UT_CMusConferenceCallMonitor::StatusChanged()
    {
    // Function called flag
    iStatusChanged = ETrue;
    }


// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
UT_CMusConferenceCallMonitor::UT_CMusConferenceCallMonitor() 
    : CEUnitTestSuiteClass()
    {
    // nothing
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
UT_CMusConferenceCallMonitor::~UT_CMusConferenceCallMonitor()
    {
    }

   
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusConferenceCallMonitor::UT_RunL()
    {    
    iStatusChanged = EFalse;
    iMonitor->RunL();
    iMonitor->Cancel();
    EUNIT_ASSERT( iStatusChanged );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusConferenceCallMonitor::UT_DoCancel()
    {
    iMonitor->DoCancel();
    EUNIT_ASSERT(RTelHelper::GetCalledFunction() ==
                         RTelSubSessionBase::ECancelAsyncRequest )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusConferenceCallMonitor::UT_RunError()
    {
    EUNIT_ASSERT_EQUALS( iMonitor->RunError( KErrNone ),
                         KErrNone )
    // Make sure RunError handles the error compleatly and returns
    // KErrNone
    EUNIT_ASSERT( iMonitor->RunError( KErrGeneral ) == KErrNone );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusConferenceCallMonitor::SetupL()
    {    
    RMobilePhone phone;
    iMonitor = CMusConferenceCallMonitor::NewL( phone, *this );
    iMonitor->Cancel();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusConferenceCallMonitor::Teardown()
    {    
    delete iMonitor;    
    
    }

// test table

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusConferenceCallMonitor,
    "UT_CMusConferenceCallMonitor",
    "UNIT" )

EUNIT_TEST(
    "UT_RunL",
    "CMusCallEventMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_RunL, Teardown)

EUNIT_TEST(
    "UT_DoCancel",
    "CMusCallEventMonitor",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_DoCancel, Teardown)

EUNIT_TEST(
    "UT_RunError",
    "CMusCallEventMonitor",
    "RunError",
    "FUNCTIONALITY",
    SetupL, UT_RunError, Teardown)


EUNIT_END_TEST_TABLE
