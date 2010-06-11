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
* Description:  Implementation of unit tests for CMusManagerImpl class.
*
*/



#include "musmanager.h"
#include "musmanagerimpl.h"
#include "ut_cmusmanagerimpl.h"
#include "musmanagerserverstarter.h"

// To avoid warning about "invalid token pasting" when generating test table
#pragma warn_illtokenpasting off


// ======== MEMBER FUNCTIONS ========


UT_CMusManagerImpl::UT_CMusManagerImpl()
    : CEUnitTestSuiteClass()
    {
    }


// ---------------------------------------------------------------------------
// The ConstructL from the base class CEUnitTestSuiteClass must be called.
// It generates the test case table.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


UT_CMusManagerImpl* UT_CMusManagerImpl::NewL()
    {
    UT_CMusManagerImpl* self = UT_CMusManagerImpl::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


UT_CMusManagerImpl* UT_CMusManagerImpl::NewLC()
    {
    UT_CMusManagerImpl* self = new( ELeave ) UT_CMusManagerImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


UT_CMusManagerImpl::~UT_CMusManagerImpl()
    {
    }


// ---------------------------------------------------------------------------
// Sets up a test case by instantiating tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::SetupL(  )
    {
    iImpl = CMusManagerImpl::NewL();
    }


// ---------------------------------------------------------------------------
// Finalizes a test case by freeing resources.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::Teardown(  )
    {
    delete iImpl;
    }


// ======== UNIT TEST METHODS ========


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::UT_CMusManagerImpl_StartServerL()
    {
    iImpl->StartServer();
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::UT_CMusManagerImpl_ServerStartedL()
    {
    iImpl->ServerStarted();
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::UT_CMusManagerImpl_ExamineAvailabilityLL()
    {
    iImpl->ExamineAvailabilityL();
    } 


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::UT_CMusManagerImpl_AvailabilityLL()
    {
    iImpl->AvailabilityL();
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::UT_CMusManagerImpl_HandleSipRequestLL()
    {
    iImpl->HandleSipRequestL( 1 );
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::UT_CMusManagerImpl_StartApplicationLL()
    {
    iImpl->StartApplicationL( MultimediaSharing::EMusLiveVideo );
    iImpl->StartApplicationL( MultimediaSharing::EMusClipVideo );
    iImpl->StartApplicationL( MultimediaSharing::EMusStillImage );
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::UT_CMusManagerImpl_StopApplicationLL()
    {
    iImpl->StopApplicationL();
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::UT_CMusManagerImpl_QueryAvailabilityLL()
    {
    iImpl->QueryAvailabilityL();
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::UT_CMusManagerImpl_InitSessionLL()
    {
    iImpl->InitSessionL();
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerImpl::UT_CMusManagerImpl_ConnectLL()
    {
    iImpl->ConnectL();
    }

void UT_CMusManagerImpl::UT_CMusManagerImpl_HandleCommandLL()
    {
    MultimediaSharing::TCommandType commandType = MultimediaSharing::ECommandNotDefined;
    iImpl->HandleCommandL( commandType );
    commandType = MultimediaSharing::ECommandManualActivation;
    iImpl->HandleCommandL( commandType );
    
    }


// ======== EUNIT TEST TABLE ========


EUNIT_BEGIN_TEST_TABLE(
    UT_CMusManagerImpl,
    "CMusManagerImpl",
    "CMusManagerImpl" )

EUNIT_TEST(
    "ConnectL - test",
    "CMusManagerImpl",
    "ConnectL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerImpl_ConnectLL, Teardown )

EUNIT_TEST(
    "InitSessionL - test",
    "CMusManagerImpl",
    "InitSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerImpl_InitSessionLL, Teardown )

EUNIT_TEST(
    "QueryAvailabilityL - test",
    "CMusManagerImpl",
    "QueryAvailabilityL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerImpl_QueryAvailabilityLL, Teardown )

EUNIT_TEST(
    "StopApplicationL - test",
    "CMusManagerImpl",
    "StopApplicationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerImpl_StopApplicationLL, Teardown )

EUNIT_TEST(
    "StartApplicationL - test",
    "CMusManagerImpl",
    "StartApplicationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerImpl_StartApplicationLL, Teardown )

EUNIT_TEST(
    "HandleSipRequestL - test",
    "CMusManagerImpl",
    "HandleSipRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerImpl_HandleSipRequestLL, Teardown )

EUNIT_TEST(
    "AvailabilityL - test",
    "CMusManagerImpl",
    "AvailabilityL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerImpl_AvailabilityLL, Teardown )

EUNIT_TEST(
    "ExamineAvailabilityL - test",
    "CMusManagerImpl",
    "ExamineAvailabilityL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerImpl_ExamineAvailabilityLL, Teardown )

EUNIT_TEST(
    "ServerStarted - test",
    "CMusManagerImpl",
    "ServerStarted",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerImpl_ServerStartedL, Teardown )

EUNIT_TEST(
    "StartServer - test",
    "CMusManagerImpl",
    "StartServer",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerImpl_StartServerL, Teardown )

EUNIT_TEST(
    "HandleCommandL - test",
    "CMusManager",
    "HandleCommandL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerImpl_HandleCommandLL, Teardown )    

EUNIT_END_TEST_TABLE
