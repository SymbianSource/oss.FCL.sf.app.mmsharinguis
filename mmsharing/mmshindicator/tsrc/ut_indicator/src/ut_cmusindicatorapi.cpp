/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implements unit tests for CMusIndicatorApi class.
*
*/



#include "ut_cmusindicatorapi.h"
#include "musindicatorapi.h"
#include "mussoundplayer.h"
#include "mussettingskeys.h"
#include <digia/eunit/eunitmacros.h>
#include <AknGlobalMsgQuery.h>
#include <AknGlobalConfirmationQuery.h>
#include <coreapplicationuisdomainpskeys.h>

// Next row is to disable warning emerging from EUnit code.
#pragma warn_illtokenpasting off


// ======== MEMBER FUNCTIONS ========


UT_CMusIndicatorApi* UT_CMusIndicatorApi::NewL()
    {
    UT_CMusIndicatorApi* self = UT_CMusIndicatorApi::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


UT_CMusIndicatorApi* UT_CMusIndicatorApi::NewLC()
    {
    UT_CMusIndicatorApi* self = new( ELeave ) UT_CMusIndicatorApi();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


UT_CMusIndicatorApi::~UT_CMusIndicatorApi()
    {
    }


UT_CMusIndicatorApi::UT_CMusIndicatorApi()
    {
    }


// ---------------------------------------------------------------------------
// The ConstructL from the base class CEUnitTestSuiteClass must be called.
// It generates the test case table.
// ---------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// ----------------------------------------------------------------------------
// Prepares a test for execution.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::SetupL()
    {
    iIndicatorApi = CMusIndicatorApi::NewL( *this );
    iStartLiveSharingLCalled = EFalse;
    }


// ----------------------------------------------------------------------------
// Finalizes a test by releasing used resources.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::Teardown()
    {
    delete iIndicatorApi;
    PropertyHelper::Close();
    
    }


// ----------------------------------------------------------------------------
// From MMusIndicatorObserver.
// Instructs to start live video sharing.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::StartLiveSharingL()
    {
    iStartLiveSharingLCalled = ETrue;
    }


// ======== UNIT TEST METHODS ========


// ----------------------------------------------------------------------------
// Asserts that instantiation is successful.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_NewLL()
    {
    EUNIT_ASSERT( iIndicatorApi );
    }


// ----------------------------------------------------------------------------
// Dummy test. @TODO: Can this be tested?
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_IndicateAvailabilityLL()
    {
    iIndicatorApi->IndicateAvailabilityL();
    }


// ----------------------------------------------------------------------------
// Asserts that start livesharing is called only when there is no error.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_RunLL()
    {
    User::LeaveIfError( RProperty::Define(
                            KPSUidCoreApplicationUIs,
                            KCoreAppUIsVideoSharingIndicator,
                            RProperty::EInt,
                            0 ) );
        
    User::LeaveIfError( RProperty::Set(
                            KPSUidCoreApplicationUIs,
                            KCoreAppUIsVideoSharingIndicator,
                            ( TInt ) ECoreAppUIsVideoSharingIndicatorOn ) );
        
    iIndicatorApi->Cancel();
    iIndicatorApi->iStatus = KErrNotFound;
    iIndicatorApi->RunL();
    EUNIT_ASSERT( !iStartLiveSharingLCalled );
    
    iIndicatorApi->Cancel();
    iIndicatorApi->iStatus = KErrNone;
    iIndicatorApi->RunL();
    EUNIT_ASSERT( iStartLiveSharingLCalled ); 
    
    }


// ----------------------------------------------------------------------------
// Asserts that DoCancel removes current activity.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_DoCancelL()
    {
    iIndicatorApi->Cancel();
    EUNIT_ASSERT( !iIndicatorApi->IsActive() );
    
    }


// ----------------------------------------------------------------------------
// Asserts that method returns KErrNone.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_RunErrorL()
    {
    EUNIT_ASSERT( KErrNone == iIndicatorApi->RunError( KErrGeneral ) );
    EUNIT_ASSERT( KErrNone == iIndicatorApi->RunError( KErrNone ) );
    EUNIT_ASSERT( KErrNone == iIndicatorApi->RunError( KErrCancel ) );
    }


// ----------------------------------------------------------------------------
// Asserts that tested method deletes soundplayer.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_PlaySoundCompleteL()
    {
    iIndicatorApi->iSoundPlayer = CMusSoundPlayer::NewL( *iIndicatorApi );
    iIndicatorApi->PlaySoundComplete( KErrNone );
    EUNIT_ASSERT( !iIndicatorApi->iSoundPlayer );
    iIndicatorApi->PlaySoundComplete( KErrGeneral );
    EUNIT_ASSERT( !iIndicatorApi->iSoundPlayer );
    }


// ----------------------------------------------------------------------------
// Asserts that method creates sound player.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_PlayToneLL()
    {
    iIndicatorApi->PlayToneL();
    EUNIT_ASSERT( iIndicatorApi->iSoundPlayer );
    }


// ======== EUNIT TEST TABLE ========


EUNIT_BEGIN_TEST_TABLE(
    UT_CMusIndicatorApi,
    "UT_CMusIndicatorApi",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusIndicatorApi",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorApi_NewLL, Teardown )

EUNIT_TEST(
    "IndicateAvailabilityL - test ",
    "CMusIndicatorApi",
    "IndicateAvailabilityL",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorApi_IndicateAvailabilityLL, Teardown )

EUNIT_TEST(
    "RunL - test ",
    "CMusIndicatorApi",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorApi_RunLL, Teardown )

EUNIT_TEST(
    "DoCancel - test ",
    "CMusIndicatorApi",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorApi_DoCancelL, Teardown )

EUNIT_TEST(
    "RunError - test ",
    "CMusIndicatorApi",
    "RunError",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorApi_RunErrorL, Teardown )

EUNIT_TEST(
    "PlaySoundComplete - test ",
    "CMusIndicatorApi",
    "PlaySoundComplete",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorApi_PlaySoundCompleteL, Teardown )

EUNIT_TEST(
    "PlayToneL - test ",
    "CMusIndicatorApi",
    "PlayToneL",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorApi_PlayToneLL, Teardown )

    

EUNIT_END_TEST_TABLE
