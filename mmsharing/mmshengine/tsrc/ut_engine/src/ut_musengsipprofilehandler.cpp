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
#include "ut_musengsipprofilehandler.h"
#include "mussipprofilehandler.h"
#include "musengstubs.h"
#include "musengtestdefs.h"
#include "sipprofilealrcontroller.h"


//  SYSTEM INCLUDES
#include <sipprofile.h>
#include <digia/eunit/eunitmacros.h>
#include <uri8.h>

const TUint KSipProfileId( 1 );


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSipProfileHandler* UT_CMusEngSipProfileHandler::NewL()
    {
    UT_CMusEngSipProfileHandler* self = UT_CMusEngSipProfileHandler::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSipProfileHandler* UT_CMusEngSipProfileHandler::NewLC()
    {
    UT_CMusEngSipProfileHandler* self = new( ELeave ) UT_CMusEngSipProfileHandler();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSipProfileHandler::~UT_CMusEngSipProfileHandler()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngSipProfileHandler::UT_CMusEngSipProfileHandler()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngSipProfileHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
// From MMusSipProfileUser
// -----------------------------------------------------------------------------
//
TBool UT_CMusEngSipProfileHandler::IsRoamingBetweenAPsAllowed()
    {
    return iRoamingBetweenAPsAllowed;
    }
    
   
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSipProfileHandler::SetupL()
    {
    iProfileHandler = CMusSipProfileHandler::NewL( *this );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSipProfileHandler::Teardown()
    {
    delete iProfileHandler;
    iRoamingBetweenAPsAllowed = EFalse;
    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSipProfileHandler::UT_CMusEngSipProfileHandler_CreateSipProfileLL()
    {
    // SIP profile ID zero
    iProfileHandler->CreateProfileL( 0 );
    EUNIT_ASSERT( iProfileHandler->iSipProfile != NULL )
    EUNIT_ASSERT( iProfileHandler->iSipProfile == iProfileHandler->Profile() )
    EUNIT_ASSERT( iProfileHandler->iSipProfile->iIsDefaultProfile )
    
    // SIP profile ID non-zero 
    delete iProfileHandler->iSipProfile;
    iProfileHandler->iSipProfile = NULL;
    iProfileHandler->CreateProfileL( KSipProfileId );
    EUNIT_ASSERT( iProfileHandler->iSipProfile != NULL )
    EUNIT_ASSERT( iProfileHandler->iSipProfile == iProfileHandler->Profile() )
    EUNIT_ASSERT( !iProfileHandler->iSipProfile->iIsDefaultProfile )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSipProfileHandler::UT_CMusEngSipProfileHandler_ProfileIdL()
    {
    // SIP profile does not yet exist
    EUNIT_ASSERT_EQUALS( 0, iProfileHandler->ProfileId() )
    
    // SIP profile exists
    iProfileHandler->CreateProfileL( KSipProfileId );
    EUNIT_ASSERT_EQUALS( KSipProfileId, iProfileHandler->ProfileId() )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSipProfileHandler::UT_CMusEngSipProfileHandler_AlrEventL()
    {
    const TUint32 KSnapId( 1 );
    const TUint32 KIapId( 1 );
    
    // Event is other that EIapAvailable
    iProfileHandler->AlrEvent( 
        MSipProfileAlrObserver::EMigrationCompleted, 
        KSipProfileId, KSnapId, KIapId );
    EUNIT_ASSERT( 
        !iProfileHandler->iProfileAlrController->iAllowMigrationCalled )     
    EUNIT_ASSERT( 
        !iProfileHandler->iProfileAlrController->iDisallowMigrationCalled )
    
    // Event is EIapAvailable, SIP profile does not exist
    iProfileHandler->AlrEvent( 
        MSipProfileAlrObserver::EIapAvailable, 
        KSipProfileId, KSnapId, KIapId );
    if ( !iProfileHandler->iProfileAlrController->iAllowMigrationCalled )
        {
        User::Leave( KErrNoMemory );
        }    
    EUNIT_ASSERT( 
        iProfileHandler->iProfileAlrController->iAllowMigrationCalled ) 
    iProfileHandler->iProfileAlrController->iAllowMigrationCalled = EFalse;
    EUNIT_ASSERT( 
        !iProfileHandler->iProfileAlrController->iDisallowMigrationCalled )
    EUNIT_ASSERT_EQUALS( 
        KIapId, iProfileHandler->iProfileAlrController->iLastUsedIap )
    iProfileHandler->iProfileAlrController->iLastUsedIap = 0;
        
    // Event is EIapAvailable, SIP profile ID does not match
    iProfileHandler->CreateProfileL( KSipProfileId );
    iProfileHandler->AlrEvent( 
        MSipProfileAlrObserver::EIapAvailable, 
        KSipProfileId+1, KSnapId, KIapId );
    if ( !iProfileHandler->iProfileAlrController->iAllowMigrationCalled )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( 
        iProfileHandler->iProfileAlrController->iAllowMigrationCalled )
    iProfileHandler->iProfileAlrController->iAllowMigrationCalled = EFalse;
    EUNIT_ASSERT( 
        !iProfileHandler->iProfileAlrController->iDisallowMigrationCalled )
    EUNIT_ASSERT_EQUALS( 
        KIapId, iProfileHandler->iProfileAlrController->iLastUsedIap )
    iProfileHandler->iProfileAlrController->iLastUsedIap = 0;
    
    // Event is EIapAvailable, SIP profile ID matches, roaming not allowed
    iProfileHandler->AlrEvent( 
        MSipProfileAlrObserver::EIapAvailable, 
        KSipProfileId, KSnapId, KIapId );
    if ( !iProfileHandler->iProfileAlrController->iDisallowMigrationCalled )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( 
        !iProfileHandler->iProfileAlrController->iAllowMigrationCalled ) 
    EUNIT_ASSERT( 
        iProfileHandler->iProfileAlrController->iDisallowMigrationCalled )        
    iProfileHandler->iProfileAlrController->iDisallowMigrationCalled = EFalse;   
    EUNIT_ASSERT_EQUALS( 
        KIapId, iProfileHandler->iProfileAlrController->iLastUsedIap )
    iProfileHandler->iProfileAlrController->iLastUsedIap = 0;
    
    // Event is EIapAvailable, SIP profile ID matches, roaming not allowed
    iRoamingBetweenAPsAllowed = ETrue;
    iProfileHandler->AlrEvent( 
        MSipProfileAlrObserver::EIapAvailable, 
        KSipProfileId, KSnapId, KIapId );
    if ( !iProfileHandler->iProfileAlrController->iAllowMigrationCalled )
        {
        User::Leave( KErrNoMemory );
        }    
    EUNIT_ASSERT( 
        iProfileHandler->iProfileAlrController->iAllowMigrationCalled )
    iProfileHandler->iProfileAlrController->iAllowMigrationCalled = EFalse;
    EUNIT_ASSERT( 
        !iProfileHandler->iProfileAlrController->iDisallowMigrationCalled )
    EUNIT_ASSERT_EQUALS( 
            KIapId, iProfileHandler->iProfileAlrController->iLastUsedIap )
    iProfileHandler->iProfileAlrController->iLastUsedIap = 0;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSipProfileHandler::UT_CMusEngSipProfileHandler_NullTestsL()
    {
    // Dummies
    CSIPTransactionBase transactionBase;
    CSIPServerTransaction serverTransaction;
    CSIPDialog dialog;
    CSIPClientTransaction clientTransaction;
    CSIPDialogAssocBase dialogAssocBase;
    CSIPRegistrationBinding registrationBinding;
    CSIPRefresh refresh;
    
    // Tests
    iProfileHandler->ProfileRegistryEventOccurred( 0,
                                MSIPProfileRegistryObserver::EProfileCreated );
    iProfileHandler->ProfileRegistryErrorOccurred( 0, 0 );
    iProfileHandler->IncomingRequest( 0, NULL );
    iProfileHandler->TimedOut( serverTransaction );
    iProfileHandler->IncomingRequest( NULL );
    iProfileHandler->IncomingRequest( NULL, dialog );
    iProfileHandler->IncomingResponse( clientTransaction );
    iProfileHandler->IncomingResponse( clientTransaction, dialogAssocBase );
    iProfileHandler->IncomingResponse( clientTransaction, NULL );
    iProfileHandler->IncomingResponse( clientTransaction, registrationBinding );
    iProfileHandler->ErrorOccured( 0, transactionBase );
    iProfileHandler->ErrorOccured( 0, clientTransaction, registrationBinding );
    iProfileHandler->ErrorOccured( 0, transactionBase, dialogAssocBase );
    iProfileHandler->ErrorOccured( 0, refresh );
    iProfileHandler->ErrorOccured( 0, registrationBinding );
    iProfileHandler->ErrorOccured( 0, dialogAssocBase );
    iProfileHandler->InviteCompleted( clientTransaction );
    iProfileHandler->InviteCanceled( serverTransaction );
    iProfileHandler->ConnectionStateChanged( CSIPConnection::EInit );
    iProfileHandler->AlrError( KErrGeneral, 0, 0, 0 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSipProfileHandler::UT_CMusEngSipProfileHandler_UserFromProfileLCL()
    {
    iProfileHandler->CreateProfileL( KSipProfileId );
    CSIPProfile* profile = iProfileHandler->Profile();
    delete profile->iArray;
    profile->iArray = NULL;
    
    // no aors -> leave with KErrArgument
    TRAPD( err, iProfileHandler->UserFromProfileLC() )
    EUNIT_ASSERT_EQUALS( KErrArgument, err )
    
    // create aors -> user found
    _LIT8( KUserUri, "sip:user@domain.com" );
    profile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
    profile->iArray->AppendL( KUserUri );

    CUri8* user = iProfileHandler->UserFromProfileLC();
    
    EUNIT_ASSERT_EQUALS( KUserUri(), user->Uri().UriDes() )
    
    CleanupStack::PopAndDestroy( user );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngSipProfileHandler,
    "UT_CMusEngSipProfileHandler",
    "UNIT" )

EUNIT_TEST(
    "CreateSipProfileL - test ",
    "CMusSipProfileHandler",
    "CreateSipProfileL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngSipProfileHandler_CreateSipProfileLL, Teardown)

EUNIT_TEST(
    "ProfileId - test ",
    "CMusSipProfileHandler",
    "ProfileId",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngSipProfileHandler_ProfileIdL, Teardown)
    
EUNIT_TEST(
    "AlrEventL - test ",
    "CMusEngSipProfileHandler",
    "AlrEventL",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngSipProfileHandler_AlrEventL, Teardown)

EUNIT_TEST(
    "Null tests for NOP functions",
    "CMusEngSipProfileHandler",
    "Several NOP functions",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngSipProfileHandler_NullTestsL, Teardown)

EUNIT_TEST(
    "UserFromProfileLC - test ",
    "CMusEngOutSession",
    "UserFromProfileLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusEngSipProfileHandler_UserFromProfileLCL, Teardown)
        
EUNIT_END_TEST_TABLE

//  END OF FILE


