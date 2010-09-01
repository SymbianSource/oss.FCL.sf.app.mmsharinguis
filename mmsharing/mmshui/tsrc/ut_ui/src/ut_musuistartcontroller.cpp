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
#include "ut_musuistartcontroller.h"
#include "musuistartcontroller.h"

#include "musuidefinitions.h"
#include "musmanagercommon.h"

#include "musuieventobserveradapter.h"

#include <e32Property.h>


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
//#include <centralrepository.h>
//#include <mcedisplaysink.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiStartController* UT_CMusUiStartController::NewL()
    {
    UT_CMusUiStartController* self = UT_CMusUiStartController::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiStartController* UT_CMusUiStartController::NewLC()
    {
    UT_CMusUiStartController* self = new( ELeave ) UT_CMusUiStartController();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiStartController::~UT_CMusUiStartController()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusUiStartController::UT_CMusUiStartController()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusUiStartController::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiStartController::SetupL()
    {
    iEventObserver = new (ELeave) CMusUiEventObserverAdapter();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiStartController::Teardown()
    {

    delete iEventObserver;

    // Tests are made in construction phase
    // Because class implementation is constructor based

    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiStartController::UT_CMusUiStartController_UseCasePropertyLL()
    {
    EUNIT_ASSERT( iEventObserver );

    // Testing leave
    RProperty::iErrorCode = KErrNotFound;
    TRAPD( err, CMusUiStartController::NewL( *iEventObserver ) );
    EUNIT_ASSERT( err == KErrNotFound );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiStartController::UT_CMusUiStartController_SetStartViewLL()
    {
    EUNIT_ASSERT( iEventObserver );

    // Testing leave
    RProperty::iErrorCode = KErrNotFound;
    RProperty::iIntProperty = -9999;
    iEventObserver->iErrorCode = KErrNone;
    TRAPD( err, CMusUiStartController::NewL( *iEventObserver ) );
    EUNIT_ASSERT( err == KErrNotFound );

    // Testing leave
    RProperty::iErrorCode = KErrNone;
    RProperty::iIntProperty = -9999;
    iEventObserver->iErrorCode = KErrCancel;
    TRAP( err, CMusUiStartController::NewL( *iEventObserver ) );
    EUNIT_ASSERT( err == KErrCancel );

    // case default

    RProperty::iErrorCode = KErrNone;
    RProperty::iIntProperty = -9999;
    iEventObserver->iErrorCode = KErrNone;
    CMusUiStartController* scdefault = CMusUiStartController::NewL( *iEventObserver );
    EUNIT_ASSERT( iEventObserver->iCalledObserver == CMusUiEventObserverAdapter::EHandleExitL );

    delete scdefault;

    // case EMusLiveVideo

    RProperty::iErrorCode = KErrNone;
    RProperty::iIntProperty = EMusLiveVideo;
    iEventObserver->iErrorCode = KErrNone;
    CMusUiStartController* sclive = CMusUiStartController::NewL( *iEventObserver );
    EUNIT_ASSERT( iEventObserver->iCalledObserver == CMusUiEventObserverAdapter::EActivateLocalMusViewL );
    EUNIT_ASSERT( iEventObserver->iViewId == KMusUidLiveSharingView );

    delete sclive;

    // case EMusClipVideo

    RProperty::iErrorCode = KErrNone;
    RProperty::iIntProperty = EMusClipVideo;
    iEventObserver->iErrorCode = KErrNone;
    CMusUiStartController* scclip = CMusUiStartController::NewL( *iEventObserver );
    EUNIT_ASSERT( iEventObserver->iCalledObserver == CMusUiEventObserverAdapter::EActivateLocalMusViewL );
    EUNIT_ASSERT( iEventObserver->iViewId == KMusUidClipSharingView );

    delete scclip;

    // case EMusReceive

    RProperty::iErrorCode = KErrNone;
    RProperty::iIntProperty = EMusReceive;
    iEventObserver->iErrorCode = KErrNone;
    CMusUiStartController* screceive = CMusUiStartController::NewL( *iEventObserver );
    EUNIT_ASSERT( iEventObserver->iCalledObserver == CMusUiEventObserverAdapter::EActivateLocalMusViewL );
    EUNIT_ASSERT( iEventObserver->iViewId == KMusUidReceivingView );

    delete screceive;

    }




//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusUiStartController,
    "UT_CMusUiStartController",
    "UNIT" )

EUNIT_TEST(
    "UseCasePropertyLL - test ",
    "CMusUiStartController",
    "UseCasePropertyLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiStartController_UseCasePropertyLL, Teardown)

EUNIT_TEST(
    "SetStartViewLL - test ",
    "CMusUiStartController",
    "SetStartViewLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiStartController_SetStartViewLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
