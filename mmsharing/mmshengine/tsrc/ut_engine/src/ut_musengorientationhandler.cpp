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
#include "ut_musengorientationhandler.h"
#include "musengorientationhandler.h"
#include "musengstubs.h"
#include "musengtestdefs.h"
#include "musenglivesession.h"
#include "musengmceutils.h"
#include <mcecamerasource.h>
#include <mcedisplaysink.h>

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngOrientationHandler* UT_CMusEngOrientationHandler::NewL()
    {
    UT_CMusEngOrientationHandler* self = UT_CMusEngOrientationHandler::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngOrientationHandler* UT_CMusEngOrientationHandler::NewLC()
    {
    UT_CMusEngOrientationHandler* self = new( ELeave ) UT_CMusEngOrientationHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngOrientationHandler::~UT_CMusEngOrientationHandler()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngOrientationHandler::UT_CMusEngOrientationHandler()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngOrientationHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngOrientationHandler::SetupL()
    {
    iObserver = new( ELeave ) CMusEngObserverStub;
    
    iLiveSession = CMusEngLiveSession::NewL( TRect(0, 0, 100, 100),
                                             *iObserver,
                                             *iObserver,
                                             *iObserver );   

    iLiveSession->iOperatorVariant = EFalse;
    
    iHandler = CMusEngOrientationHandler::NewL( *iLiveSession, *iObserver );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngOrientationHandler::Teardown()
    {
    delete iHandler;
    delete iLiveSession;
    delete iObserver;
    }



// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngOrientationHandler::UT_NewLL()
    {
    EUNIT_ASSERT( iHandler );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngOrientationHandler::UT_RefreshOrientationL()
    { 
    ESTABLISH_OUT_SESSION( iLiveSession )
        
    // Refresh when both camera and display are enabled
    CMceCameraSource* camera = 
        MusEngMceUtils::GetCameraL( *(iLiveSession->iSession) );
    camera->iIsEnabled = ETrue;
    CMceDisplaySink* display = 
        MusEngMceUtils::GetDisplayL( *(iLiveSession->iSession) );
    display->iIsEnabled = ETrue;
    EUNIT_ASSERT( !iHandler->IsActive() );
    iHandler->RefreshOrientationL();
    EUNIT_ASSERT( iHandler->IsActive() );
    EUNIT_ASSERT( !camera->iIsEnabled );
    EUNIT_ASSERT( !display->iIsEnabled );
    iHandler->Cancel();
    iHandler->RunL(); // Simulate completion
    EUNIT_ASSERT( !iHandler->IsActive() );
    EUNIT_ASSERT( camera->iIsEnabled );
    EUNIT_ASSERT( display->iIsEnabled );
       
    // Refresh when camera is disabled, display disabled
    camera->iIsEnabled = EFalse;
    display->iIsEnabled = ETrue;
    iHandler->RefreshOrientationL();
    EUNIT_ASSERT( iHandler->IsActive() );
    EUNIT_ASSERT( !camera->iIsEnabled );
    EUNIT_ASSERT( !display->iIsEnabled );
    iHandler->Cancel();
    iHandler->RunL(); // Simulate completion
    EUNIT_ASSERT( !iHandler->IsActive() );
    EUNIT_ASSERT( !camera->iIsEnabled );
    EUNIT_ASSERT( display->iIsEnabled );
    
    // Display and camera gets resumed while delayed orientation handling is ongoing, pausing
    // display after handling completes should not happen
    camera->iIsEnabled = ETrue;
    display->iIsEnabled = EFalse;
    iHandler->RefreshOrientationL();
    EUNIT_ASSERT( iHandler->IsActive() );
    EUNIT_ASSERT( !camera->iIsEnabled );
    EUNIT_ASSERT( !display->iIsEnabled );
    display->iIsEnabled = ETrue;
    camera->iIsEnabled = ETrue;
    iHandler->UpdateL();
    iHandler->Cancel();
    iHandler->RunL(); // Simulate completion
    EUNIT_ASSERT( !iHandler->IsActive() );
    EUNIT_ASSERT( camera->iIsEnabled );
    EUNIT_ASSERT( display->iIsEnabled );     
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngOrientationHandler,
    "UT_CMusEngLiveSesssion",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusEngOrienationHandler",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewLL, Teardown)
    
EUNIT_TEST(
    "UT_RefreshOrientationL - test ",
    "CMusEngOrienationHandler",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_RefreshOrientationL, Teardown)
      
EUNIT_END_TEST_TABLE

//  END OF FILE


