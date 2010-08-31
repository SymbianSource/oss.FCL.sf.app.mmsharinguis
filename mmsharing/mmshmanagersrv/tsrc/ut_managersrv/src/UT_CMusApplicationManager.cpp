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
#include "mussettings.h"
#include "mussesseioninformationapi.h"
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
    Dll::FreeTls(); // Used by the RProcess and TFindProcess stubs
    }


// ======== TEST METHODS ========


// ---------------------------------------------------------------------------
// Asserts that instantiation is successful.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_NewLL()
    {
    EUNIT_ASSERT( iManager );
    TInt fastMode;
    User::LeaveIfError( RProperty::Get( NMusSessionApi::KCategoryUid, 
                                        NMusSessionApi::KFastMode, 
                                        fastMode ) );
    EUNIT_ASSERT_EQUALS( TInt( MusSettingsKeys::EFastModeOff ), fastMode );
    }


// ---------------------------------------------------------------------------
// Asserts that instantiation is successful.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_NewLCL()
    {
    CMusApplicationManager* manager = CMusApplicationManager::NewLC();
    EUNIT_ASSERT( manager );
    CleanupStack::PopAndDestroy( manager );
    }


// ---------------------------------------------------------------------------
// Asserts that correct functions from TApaTask are called.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_ApplicationRunningL()
    {
    EUNIT_ASSERT( !iManager->ApplicationRunning() )
   
    iManager->StartApplicationL();
    EUNIT_ASSERT( iManager->ApplicationRunning() )
    }


// ---------------------------------------------------------------------------
// Else branch of tested method can't be tested since it tries to start real
// application.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_StartApplicationLL()
    {
    EUNIT_ASSERT( !iManager->ApplicationRunning() )
    iManager->StartApplicationL();
    EUNIT_ASSERT_EQUALS( PropertyHelper::GetCalledFunction(), RProperty::EDefine )
    EUNIT_ASSERT( iManager->ApplicationRunning() )
    
    iManager->StartApplicationL();
    EUNIT_ASSERT( iManager->ApplicationRunning() )
    }


// ---------------------------------------------------------------------------
// Asserts that tested method calls TApaTask::Exists and if task exists it
// should also call TApaTask::KillTask.
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_StopApplicationLL()
    {
    // MuS not running
    iManager->StopApplicationL(); 
    
    // MuS running
    User::LeaveIfError( 
        Dll::SetTls( reinterpret_cast< TAny* >( 1 ) ) );     
    // TLS is used by TFindProcess stub
    iManager->StopApplicationL();
    TInt availability = MultimediaSharing::EMultimediaSharingAvailable;
    TUint key( NMusSessionApi::KStatus );
    RProperty::Get( key, availability );
    EUNIT_ASSERT_EQUALS( PropertyHelper::GetCalledFunction(), RProperty::EDefine ) 
    EUNIT_ASSERT_EQUALS( availability, 
                         MultimediaSharing::EMultimediaSharingNotAvailable )
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_ShowApplicationLL()
    {
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
    array->AppendL( _L( "0" ) ); //EFastModeOn

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
    
    

    MultimediaSharingSettings::iVideoDirection = MusSettingsKeys::ETwoWayVideo;
    iManager->WriteSessionPropertiesL(
            MultimediaSharing::EMusLiveVideo,
            (MultimediaSharing::TMusAvailabilityStatus) KErrNone,
            *array );

    TInt val;
    User::LeaveIfError( RProperty::Get( NMusSessionApi::KCategoryUid, 
                                        NMusSessionApi::KUseCase, 
                                        val ) );
    
    MultimediaSharing::TMusUseCase usecase = 
                static_cast< MultimediaSharing::TMusUseCase >( val );
    
    EUNIT_ASSERT( usecase == MultimediaSharing::EMusTwoWayVideo );
        
    
    //Ensure if fast mode key is "disabled", value from session params
    //won't be published
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid, 
                                        NMusSessionApi::KFastMode, 
                                        MusSettingsKeys::EFastModeDisabled ) );
    iManager->WriteSessionPropertiesL(
            MultimediaSharing::EMusLiveVideo,
            (MultimediaSharing::TMusAvailabilityStatus) KErrNone,
            *array );
    User::LeaveIfError( RProperty::Get( NMusSessionApi::KCategoryUid, 
                                        NMusSessionApi::KFastMode, 
                                        val ) );
    EUNIT_ASSERT_EQUALS( TInt( MusSettingsKeys::EFastModeDisabled ), val );
    
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


// ---------------------------------------------------------------------------
// CallProviderL Test
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_CallProviderLL()
    {
    TInt error = KErrNone;
    
    // Use Publish & Subscribe Keys to set CallProvider Name
    _LIT( KProviderName1, "MultimediaSharing");
    _LIT8( KProviderNameTest1, "MultimediaSharing");
    
    _LIT( KProviderNameEmpty1, "");
    _LIT8( KProviderNameEmptyTest1, "");
    
    
    // Valid Test:
    TRAP( error, RProperty::Set(
           NMusSessionInformationApi::KCategoryUid,
           NMusSessionInformationApi::KMUSCallProvider,
           KProviderName1 ));

    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    
    TBuf8<RProperty::KMaxPropertySize> providerName ;
    iManager->CallProviderL( providerName );
    
    EUNIT_ASSERT( providerName == KProviderNameTest1 );
    

    //Empty Test
    TRAP( error, RProperty::Set(
           NMusSessionInformationApi::KCategoryUid,
           NMusSessionInformationApi::KMUSCallProvider,
           KProviderNameEmpty1 ));

    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    iManager->CallProviderL( providerName );
    
    EUNIT_ASSERT( providerName == KProviderNameEmptyTest1 );
    }


// ---------------------------------------------------------------------------
// ResolvePluginName Test
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_ResolvePluginNameLL()
    {
    // Default Engine Name to be returned
    TInt error = KErrNone;
    _LIT( KEngineName, "MultimediaSharing");
    _LIT8( KEngineTestName, "MultimediaSharing");

    // Test String for the Publish/Subscribe Keys.
    _LIT( KProviderName, "MultimediaSharing");
    _LIT( KProviderName1, "CS");
  
    TBuf8<RProperty::KMaxPropertySize> engineName ;

    // Use Publish & Subscribe Keys to set Engine Name as 
    // 1. MultimediaSharing  [Exact Name]
    // 2. CS                 [No Match Found, Default MultiMediaSharing will be picked]

    // 1. MultimediaSharing  [Exact Name]

    TRAP( error, RProperty::Set(
           NMusSessionInformationApi::KCategoryUid,
           NMusSessionInformationApi::KMUSCallProvider,
           KProviderName ));

    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    
    iManager->ResolvePluginNameL( engineName );
    EUNIT_ASSERT( engineName == KEngineTestName );
  
    // 2. CS [No Match Found, Default MultiMediaSharing will be picked]
    TRAP( error, RProperty::Set(
           NMusSessionInformationApi::KCategoryUid,
           NMusSessionInformationApi::KMUSCallProvider,
           KProviderName1 ));
    
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    iManager->ResolvePluginNameL( engineName );
    EUNIT_ASSERT( engineName == KEngineTestName );
    }

// ---------------------------------------------------------------------------
// Command line arg test
// ---------------------------------------------------------------------------
//
void UT_CMusApplicationManager::UT_CMusApplicationManager_CreateCommandLineArgsLCL()
    {
    _LIT( KEnginePluginName, "MultimediaSharing" );
    HBufC* cmdLineArgs = iManager->CreateCommandLineArgsLC();
    
    EUNIT_ASSERT( cmdLineArgs != NULL )
    EUNIT_ASSERT_EQUALS( KEnginePluginName(), *cmdLineArgs )
    
    CleanupStack::PopAndDestroy( cmdLineArgs );
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
    "SetStatusL - test ",
    "CMusApplicationManager",
    "SetStatusL",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_SetStatusLL, Teardown )

EUNIT_TEST(
    "CallProviderL - test ",
    "CMusApplicationManager",
    "CallProviderL",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_CallProviderLL, Teardown )    
    
EUNIT_TEST(
    "ResolvePluginNameL - test ",
    "CMusApplicationManager",
    "ResolvePluginNameL",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_ResolvePluginNameLL, Teardown ) 
    
EUNIT_TEST(
    "CreateCommandLineLC - test ",
    "CMusApplicationManager",
    "CreateCommandLineLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusApplicationManager_CreateCommandLineArgsLCL, Teardown )  
    
EUNIT_END_TEST_TABLE
