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
#include "musengstubs.h"
#include "ut_musengsessionmanager.h"
#include "musengsessionmanager.h"
#include "musengmcesession.h"
#include "musengmceoutsession.h"
#include "mussessionproperties.h"
#include "centralrepository.h"
#include "e32property.h"
#include "musengtestdefs.h"

//  SYSTEM INCLUDES
#include <eunitmacros.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSessionManager* UT_CMusEngSessionManager::NewL()
    {
    UT_CMusEngSessionManager* self = UT_CMusEngSessionManager::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSessionManager* UT_CMusEngSessionManager::NewLC()
    {
    UT_CMusEngSessionManager* self = new( ELeave )UT_CMusEngSessionManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSessionManager::~UT_CMusEngSessionManager()
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngSessionManager::UT_CMusEngSessionManager()
    {
    }

// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionManager::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionManager::SetupL()
    {
    User::LeaveIfError(
        RProperty::Set( 
            NMusSessionApi::KCategoryUid, 
            NMusSessionApi::KUseCase, 
            MultimediaSharing::EMusLiveVideo ) );
    iSessionManager = CMusEngSessionManager::NewL();
    
    iLcUiProvider = new( ELeave )CLcUiProviderStub;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionManager::Teardown()
    {
    delete iSessionManager;
    delete iLcUiProvider;
    // The Central Repository stub is using RProperty stub to store global data. 
    // The global data must be freed here.
    PropertyHelper::Close(); 
    }

// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionManager::UT_NewL()
    {
    EUNIT_ASSERT( iSessionManager->iSession != NULL )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionManager::UT_SessionL()
    {
    EUNIT_ASSERT( iSessionManager->iSession == &( iSessionManager->Session() ) )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionManager::UT_IsFeatureSupportedL()
    {
    EUNIT_ASSERT( 
        iSessionManager->IsFeatureSupported( CLcEngine::ELcShowAcceptQuery ) )
        
    EUNIT_ASSERT( 
        iSessionManager->IsFeatureSupported( CLcEngine::ELcShowInvitingNote ) )
        
    EUNIT_ASSERT( 
        iSessionManager->IsFeatureSupported( CLcEngine::ELcShowWaitingNote ) )        
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionManager::UT_CreateMusSessionL()
    {
    CMusEngMceSession* session = NULL;

    // Live session
    session = 
        iSessionManager->CreateMusSessionL( 
            MultimediaSharing::EMusLiveVideo );
    EUNIT_ASSERT( session != NULL )
    delete session;

    // Clip session
    session =
        iSessionManager->CreateMusSessionL( 
            MultimediaSharing::EMusClipVideo );
    EUNIT_ASSERT( session != NULL )
    delete session;    
    
    // Receive session
    session = 
        iSessionManager->CreateMusSessionL( 
            MultimediaSharing::EMusReceive );   
    EUNIT_ASSERT( session != NULL )
    delete session;

    // Two way session
    session = 
        iSessionManager->CreateMusSessionL( 
            MultimediaSharing::EMusTwoWayVideo );
    EUNIT_ASSERT( session != NULL )
    delete session;
    
    // Two way receive session
    session = 
        iSessionManager->CreateMusSessionL( 
            MultimediaSharing::EMusReceiveTwoWayVideo );   
    EUNIT_ASSERT( session != NULL )
    delete session;    
    
    // Other session types
    EUNIT_ASSERT_SPECIFIC_LEAVE(
        iSessionManager->CreateMusSessionL( MultimediaSharing::EMusStillImage ),
        KErrNotSupported )

    EUNIT_ASSERT_SPECIFIC_LEAVE(
        iSessionManager->CreateMusSessionL( MultimediaSharing::EMusContinue ),
        KErrNotSupported )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSessionManager::UT_PropertyChangedL()
    {
    iSessionManager->Session().SetLcUiProvider( iLcUiProvider );

    // Background startup not active, property change ignored
    MLcSession* origSession = &iSessionManager->Session();
    iSessionManager->PropertyChanged( 
            NMusSessionApi::KUseCase, MultimediaSharing::EMusReceiveTwoWayVideo );
    
    EUNIT_ASSERT( origSession == &iSessionManager->Session() );
    EUNIT_ASSERT( iLcUiProvider->iCalledFunction != CLcUiProviderStub::EHandleForegroundStatus );
    
    // Background startup active, usecase not changed, no effect
    User::LeaveIfError( RProperty::Set( NMusSessionApi::KCategoryUid, 
                    NMusSessionApi::KFastMode, 
                    MusSettingsKeys::EFastModeOn ) );
    iSessionManager->iSession->iBackground = ETrue;
    origSession = &iSessionManager->Session();
    iSessionManager->PropertyChanged( 
            NMusSessionApi::KUseCase, iSessionManager->iCurrentUseCase );
    
    EUNIT_ASSERT( origSession == &iSessionManager->Session() );
    EUNIT_ASSERT( iLcUiProvider->iCalledFunction != CLcUiProviderStub::EHandleForegroundStatus );

    // Background startup active, usecase changed, uiprovider informed and
    // new session replaces old one
    origSession = &iSessionManager->Session();
    iSessionManager->PropertyChanged( 
            NMusSessionApi::KUseCase, MultimediaSharing::EMusClipVideo );
    if ( origSession == &iSessionManager->Session() )
        {
        // Couldn't create new session because running out of memory
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( origSession != &iSessionManager->Session() );
    EUNIT_ASSERT( iLcUiProvider->iCalledFunction == CLcUiProviderStub::EHandleForegroundStatus );
    EUNIT_ASSERT( iLcUiProvider->iForeground );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngSessionManager,
    "UT_CMusEngSessionManager",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusEngSessionManager",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewL, Teardown)

EUNIT_TEST(
    "Session - test ",
    "CMusEngSessionManager",
    "Session",
    "FUNCTIONALITY",
    SetupL, UT_SessionL, Teardown)

EUNIT_TEST(
    "IsFeatureSupported - test ",
    "CMusEngSessionManager",
    "IsFeatureSupported",
    "FUNCTIONALITY",
    SetupL, UT_IsFeatureSupportedL, Teardown)

EUNIT_TEST(
    "CreateMusSessionL - test ",
    "CMusEngSessionManager",
    "CreateMusSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CreateMusSessionL, Teardown)

EUNIT_TEST(
    "PropertyChanged - test ",
    "CMusEngSessionManager",
    "PropertyChanged",
    "FUNCTIONALITY",
    SetupL, UT_PropertyChangedL, Teardown)
    
EUNIT_END_TEST_TABLE


//  END OF FILE


