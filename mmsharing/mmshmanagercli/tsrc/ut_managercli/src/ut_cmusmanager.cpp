/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implements unit tests for CMusManager class.
*
*/



#include "muslogger.h"
#include "musmanager.h"
#include "ut_cmusmanager.h"
#include "musmanagerimpl.h"
#include "FeatMgr.h"
#include <digia/eunit/eunitmacros.h>

// ======== MEMBER FUNCTIONS ========


UT_CMusManager::UT_CMusManager()
    : CEUnitTestSuiteClass()
    {
    }


// ---------------------------------------------------------------------------
// The ConstructL from the base class CEUnitTestSuiteClass must be called.
// It generates the test case table.
// ---------------------------------------------------------------------------
//
void UT_CMusManager::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


UT_CMusManager* UT_CMusManager::NewL()
    {
    UT_CMusManager* self = UT_CMusManager::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


UT_CMusManager* UT_CMusManager::NewLC()
    {
    UT_CMusManager* self = new( ELeave ) UT_CMusManager;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


UT_CMusManager::~UT_CMusManager()
    {
    }


// ---------------------------------------------------------------------------
// Sets up a test case by instantiating tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusManager::SetupL(  )
    {
    iImpl = CMusManager::NewL();
    }


// ---------------------------------------------------------------------------
// Finalizes a test case by freeing resources.
// ---------------------------------------------------------------------------
//
void UT_CMusManager::Teardown(  )
    {
    delete iImpl;
    }


// ======== UNIT TEST METHODS ========


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManager::UT_CMusManager_ExamineAvailabilityLL()
    {
    iImpl->ExamineAvailabilityL();
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManager::UT_CMusManager_AvailabilityLL()
    {
    iImpl->AvailabilityL();
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManager::UT_CMusManager_HandleSipRequestLL()
    {
    iImpl->HandleSipRequestL( CMusManager::ESipOptions );
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManager::UT_CMusManager_StartApplicationLL()
    {
    iImpl->StartApplicationL( MultimediaSharing::EMusLiveVideo );
    iImpl->StartApplicationL( MultimediaSharing::EMusClipVideo );
    iImpl->StartApplicationL( MultimediaSharing::EMusStillImage );
    }


void UT_CMusManager::UT_CMusManager_HandleCommandLL()
    {
    MultimediaSharing::TCommandType commandType = MultimediaSharing::ECommandNotDefined;
    iImpl->HandleCommandL( commandType );
    commandType = MultimediaSharing::ECommandManualActivation;
    iImpl->HandleCommandL( commandType );
    
    }

// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManager::UT_CMusManager_StopApplicationLL()
    {
    iImpl->StopApplicationL();
    }
    
// ---------------------------------------------------------------------------
// Sets up dummy
// ---------------------------------------------------------------------------
//
void UT_CMusManager::SetupDummyL(  )
    {
    }


// ---------------------------------------------------------------------------
// Teardown Dummy
// ---------------------------------------------------------------------------
//
void UT_CMusManager::TeardownDummy(  )
    {
    }  
    
// ---------------------------------------------------------------------------
// NewLL
// ---------------------------------------------------------------------------
//
void UT_CMusManager::UT_CMusManager_NewLL()
    {
	CMusManager *self = NULL;
	
	FeatureManager::MultimediaSharingNotSupported();
    TRAPD(err, self =CMusManager::NewL( this ));
    EUNIT_ASSERT( err == KErrNoMemory||err == KErrNotSupported );
	delete self;
	
    FeatureManager::MultimediaSharingSupported();
    TRAPD(error, self =CMusManager::NewL( this )); 
    EUNIT_ASSERT( error == KErrNoMemory||error == KErrNone ); 
    delete self;
    }   
    
// ---------------------------------------------------------------------------
// MusAvailabilityChangedL
// ---------------------------------------------------------------------------
//    
void UT_CMusManager::MusAvailabilityChangedL( 
            MultimediaSharing::TMusAvailabilityStatus /*aAvailabilityStatus*/ )
    {
    
    }


// ======== EUNIT TEST TABLE ========


EUNIT_BEGIN_TEST_TABLE(
    UT_CMusManager,
    "CMusManager",
    "CMusManager" )

EUNIT_TEST(
    "StopApplicationL - test",
    "CMusManager",
    "StopApplicationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManager_StopApplicationLL, Teardown)

EUNIT_TEST(
    "StartApplicationL - test",
    "CMusManager",
    "StartApplicationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManager_StartApplicationLL, Teardown)

EUNIT_TEST(
    "HandleSipRequestL - test",
    "CMusManager",
    "HandleSipRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManager_HandleSipRequestLL, Teardown)


EUNIT_TEST(
    "AvailabilityL - test",
    "CMusManager",
    "AvailabilityL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManager_AvailabilityLL, Teardown)


EUNIT_TEST(
    "ExamineAvailabilityL - test",
    "CMusManager",
    "ExamineAvailabilityL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManager_ExamineAvailabilityLL, Teardown)
    
EUNIT_TEST(
    "NewL - test",
    "CMusManager",
    "NewL",
    "FUNCTIONALITY",
    SetupDummyL, UT_CMusManager_NewLL, TeardownDummy )    

EUNIT_TEST(
    "HandleCommandL - test",
    "CMusManager",
    "HandleCommandL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManager_HandleCommandLL, Teardown )    

    
EUNIT_END_TEST_TABLE

