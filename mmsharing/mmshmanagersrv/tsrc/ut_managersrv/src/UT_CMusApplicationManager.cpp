/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit tests for CMusApplicationManager class
*
*/


#include "ut_cmusapplicationmanager.h"
#include "musapplicationmanager.h"
#include "musmanagercommon.h"
#include "mussessionproperties.h"
#include "musunittesting.h"
#include "musmanagerservercommon.h"
#include "mussessionproperties.h"
#include "mustesthelp.h"
#include <apgcli.h>
#include <apgtask.h>
#include <e32std.h>
#include <e32property.h>
#include <digia/eunit/eunitmacros.h>
#include <APACMDLN.H>



// ======== MEMBER FUNCTIONS ========


UT_CMusApplicationManager* UT_CMusApplicationManager::NewL()
    {
    UT_CMusApplicationManager* self = UT_CMusApplicationManager::NewLC();
    CleanupStack::Pop();
    return self;
    }


UT_CMusApplicationManager* UT_CMusApplicationManager::NewLC()
    {
    UT_CMusApplicationManager* self = new( ELeave ) UT_CMusApplicationManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


UT_CMusApplicationManager::~UT_CMusApplicationManager()
    {
    }


UT_CMusApplicationManager::UT_CMusApplicationManager()
    {
    }


// ---------------------------------------------------------------------------
// The ConstructL from the base class CEUnitTestSuiteClass must be called.
// It generates the test case table.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// ---------------------------------------------------------------------------
// Setups a test by instantiating tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::SetupL()
    {
    iManager = CMusApplicationManager::NewL();
    }


// ---------------------------------------------------------------------------
// Finalizes a by deleting instance of tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::Teardown()
    {
    delete iManager;
    iManager = NULL;
    PropertyHelper::Close(); 
    }


// ======== TEST METHODS ========


// ---------------------------------------------------------------------------
// Asserts that instantiation is successful.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_NewLL()
    {
    EUNIT_ASSERT( iManager );
    EUNIT_ASSERT( iManager->iApaSession.iConnected );
    }


// ---------------------------------------------------------------------------
// Asserts that instantiation is successful.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_NewLCL()
    {
    CMusApplicationManager* manager = CMusApplicationManager::NewLC();
    EUNIT_ASSERT( manager );
    EUNIT_ASSERT( manager->iApaSession.iConnected );
    CleanupStack::PopAndDestroy( manager );
    }


// ---------------------------------------------------------------------------
// Asserts that correct functions from TApaTask are called.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_ApplicationRunningL()
    {
    TApaTask::iApplicationExist = EFalse;
    EUNIT_ASSERT( !iManager->ApplicationRunning() );
    EUNIT_ASSERT( TApaTask::iApaTaskCalledFunction == TApaTask::EExists )
    TApaTask::iApplicationExist = ETrue;
    EUNIT_ASSERT( iManager->ApplicationRunning() );
    EUNIT_ASSERT( TApaTask::iApaTaskCalledFunction == TApaTask::EExists )
    }


// ---------------------------------------------------------------------------
// Else branch of tested method can't be tested since it tries to start real
// application.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_StartApplicationLL()
    {
    TApaTask::iApplicationExist = EFalse;
    iManager->StartApplicationL();
    EUNIT_ASSERT( TApaTask::iApaTaskCalledFunction == TApaTask::EExists )
    EUNIT_ASSERT_EQUALS( PropertyHelper::GetCalledFunction(), RProperty::ENone ) 

    TApaTask::iApplicationExist = ETrue;
    iManager->StartApplicationL();
    EUNIT_ASSERT_EQUALS( PropertyHelper::GetCalledFunction(), RProperty::ENone ) 

    }


// ---------------------------------------------------------------------------
// Asserts that tested method calls TApaTask::Exists and if task exists it
// should also call TApaTask::KillTask.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_StopApplicationLL()
    {
    TApaTask::iApplicationExist = EFalse;
    iManager->StopApplicationL();
    EUNIT_ASSERT( TApaTask::iApaTaskCalledFunction == TApaTask::EExists )    
    
    
    TApaTask::iApplicationExist = ETrue;
    iManager->StopApplicationL();
    TInt availability = MultimediaSharing::EMultimediaSharingAvailable;
    TUint key(NMusSessionApi::KStatus);
    RProperty::Get( key,availability);
    EUNIT_ASSERT_EQUALS( PropertyHelper::GetCalledFunction(), RProperty::EDefine ) 
    EUNIT_ASSERT_EQUALS( availability, 
                         MultimediaSharing::EMultimediaSharingNotAvailable )
    }


// ---------------------------------------------------------------------------
// Asserts that tested method calls TApaTask::Exists and if task exists it
// calls TApaTask::BringToForeground.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_ShowApplicationLL()
    {
    TApaTask::iApplicationExist = EFalse;
    iManager->ShowApplicationL();
    EUNIT_ASSERT( TApaTask::iApaTaskCalledFunction == TApaTask::EExists )
    TApaTask::iApplicationExist = ETrue;
    iManager->ShowApplicationL();
    EUNIT_ASSERT( TApaTask::iApaTaskCalledFunction == TApaTask::EBringToForeground )
    }


// ---------------------------------------------------------------------------
// Writes session properties. Asserts that RProperty leaves are passed
// correctly to user.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_WriteSessionPropertiesL()
    {
    CDesCArrayFlat* array = new( ELeave ) CDesCArrayFlat(4);
    CleanupStack::PushL( array );

    array->AppendL( _L( "a" ) );
    array->AppendL( _L( "b" ) );
    array->AppendL( _L( "c" ) );
    array->AppendL( _L( "d" ) );
    array->AppendL( _L( "e" ) );
    array->AppendL( _L( "f" ) );
    array->AppendL( _L( "g" ) );
    array->AppendL( _L( "h" ) );

    iManager->WriteSessionPropertiesL(
        MultimediaSharing::EMusLiveVideo,
        (MultimediaSharing::TMusAvailabilityStatus) KErrNone,
        *array );

    PropertyHelper::SetErrorCode( KErrGeneral );

    EUNIT_ASSERT_SPECIFIC_LEAVE(
        iManager->WriteSessionPropertiesL(
            MultimediaSharing::EMusLiveVideo,
            (MultimediaSharing::TMusAvailabilityStatus) KErrNone,
            *array ), KErrGeneral );

    PropertyHelper::SetErrorCode(KErrNone);

    CleanupStack::PopAndDestroy( array );
    }


// ---------------------------------------------------------------------------
// Sets a videosharing P/S property. Asserts that RProperty leaves are passed
// correctly to user and RProperty::Define is called. Tests literal
// properties specifically.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_SetPropertyLL()
    {
    _LIT( KTest, "unit_test");
    PropertyHelper::SetErrorCode( KErrNotSupported );
    MUS_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iManager->SetPropertyL( 1, KTest ),
        KErrNotSupported );

    EUNIT_ASSERT( PropertyHelper::GetCalledFunction() == RProperty::EDefine )

    PropertyHelper::SetErrorCode( KErrNone );
    EUNIT_ASSERT_NO_LEAVE( iManager->SetPropertyL( 1, KTest ) );
    }


// ---------------------------------------------------------------------------
// Sets a videosharing P/S property. Asserts that RProperty leaves are passed
// correctly to user and RProperty::Define is called. Tests integral
// properties specifically.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_SetPropertyL2L()
    {
    PropertyHelper::SetErrorCode( KErrNotSupported );
    MUS_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iManager->SetPropertyL( 1, 1 ),
        KErrNotSupported );

    EUNIT_ASSERT( PropertyHelper::GetCalledFunction() == RProperty::EDefine )

    PropertyHelper::SetErrorCode(KErrNone);
    MUS_EUNIT_ASSERT_NO_LEAVE( iManager->SetPropertyL( 1, 1 ) );
    }


// ---------------------------------------------------------------------------
// Asserts that TApaTaskList::FindApp is called by tested method.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_GetApaTaskL()
    {
    TApaTask::iApplicationExist = EFalse;
    iManager->GetApaTask();
    EUNIT_ASSERT( TApaTaskList::iCalledFunction == TApaTaskList::EFindApp )
    }


// ---------------------------------------------------------------------------
// Sets a videosharing status. Asserts that RProperty leaves are passed
// correctly to user and RProperty::Define is called.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_SetStatusLL()
    {
    PropertyHelper::SetErrorCode( KErrGeneral );
    MUS_EUNIT_ASSERT_SPECIFIC_LEAVE( iManager->SetStatusL(
        MultimediaSharing::EMultimediaSharingNotAvailable ), KErrGeneral );

    EUNIT_ASSERT( PropertyHelper::GetCalledFunction() == RProperty::EDefine );

    PropertyHelper::SetErrorCode(KErrNone);
    MUS_EUNIT_ASSERT_NO_LEAVE( iManager->SetStatusL(
        MultimediaSharing::EMultimediaSharingAvailable ) );
    }


// ======== EUNIT TEST TABLE ========


EUNIT_BEGIN_TEST_TABLE(
    UT_CMusApplicationManager,
    "CMusApplicationManager",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusApplicationManager",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_NewLL, Teardown )

EUNIT_TEST(
    "NewLC - test ",
    "CMusApplicationManager",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_NewLCL, Teardown )

EUNIT_TEST(
    "ApplicationRunning - test ",
    "CMusApplicationManager",
    "ApplicationRunning",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_ApplicationRunningL, Teardown )

EUNIT_TEST(
    "StartApplicationL - test ",
    "CMusApplicationManager",
    "StartApplicationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_StartApplicationLL, Teardown )

EUNIT_TEST(
    "StopApplicationL - test ",
    "CMusApplicationManager",
    "StopApplicationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_StopApplicationLL, Teardown )

EUNIT_TEST(
    "ShowApplicationL - test ",
    "CMusApplicationManager",
    "ShowApplicationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_ShowApplicationLL, Teardown )

EUNIT_TEST(
    "WriteSessionProperties - test ",
    "CMusApplicationManager",
    "WriteSessionProperties",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_WriteSessionPropertiesL, Teardown )

EUNIT_TEST(
    "SetPropertyL (string) - test ",
    "CMusApplicationManager",
    "SetPropertyL",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_SetPropertyLL, Teardown )

EUNIT_TEST(
    "SetPropertyL (integer) - test ",
    "CMusApplicationManager",
    "SetPropertyL",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_SetPropertyL2L, Teardown )

EUNIT_TEST(
    "GetApaTask - test ",
    "CMusApplicationManager",
    "GetApaTask",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_GetApaTaskL, Teardown )

EUNIT_TEST(
    "SetStatusL - test ",
    "CMusApplicationManager",
    "SetStatusL",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_SetStatusLL, Teardown )

EUNIT_END_TEST_TABLE
