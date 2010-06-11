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
static TInt stubStatus = KErrNone;
static TRequestStatus* rstatus = NULL;


// ======== CAknGlobalMsgQuery stub ========


CAknGlobalMsgQuery* CAknGlobalMsgQuery::NewL()
    {
    CAknGlobalMsgQuery* self = CAknGlobalMsgQuery::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


CAknGlobalMsgQuery* CAknGlobalMsgQuery::NewLC()
    {
    CAknGlobalMsgQuery* self = new (ELeave) CAknGlobalMsgQuery();
    CleanupStack::PushL( self );
    return self;
    }

CAknGlobalMsgQuery::CAknGlobalMsgQuery()
    {
    }

CAknGlobalMsgQuery::~CAknGlobalMsgQuery()
    {
    CancelMsgQuery();
    iNotify.Close();
    }

void CAknGlobalMsgQuery::ShowMsgQueryL( 
     TRequestStatus& aStatus,
     const TDesC& /*aMsgText*/,
     TInt /*aSoftkeys*/,
     const TDesC& /*HeaderText*/,
     const TDesC& /*HeaderImageFile*/,
     TInt /*aImageId*/,
     TInt /*aImageMaskId*/,
     CAknQueryDialog::TTone /*aTone*/)
    {
    if ( stubStatus == KErrNone )
        {
        aStatus = KRequestPending;
        rstatus = &aStatus;
        }
    else
        {
        User::Leave( stubStatus );
        }
    }

void CAknGlobalMsgQuery::CancelMsgQuery()
    {
    if ( rstatus )
        {
        User::RequestComplete( rstatus, KErrCancel );
        rstatus = NULL;
        }
    }


// ======== CAknGlobalConfirmationQuery stub ========

CAknGlobalConfirmationQuery* CAknGlobalConfirmationQuery::NewL()
    {
    CAknGlobalConfirmationQuery* self = CAknGlobalConfirmationQuery::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


CAknGlobalConfirmationQuery* CAknGlobalConfirmationQuery::NewLC()
    {
    CAknGlobalConfirmationQuery* self = new (ELeave) CAknGlobalConfirmationQuery();
    CleanupStack::PushL( self );
    return self;
    }

CAknGlobalConfirmationQuery::CAknGlobalConfirmationQuery()
    {
    }


CAknGlobalConfirmationQuery::~CAknGlobalConfirmationQuery()
    {
    CancelConfirmationQuery();
    iNotify.Close();
    }
    



void CAknGlobalConfirmationQuery::ShowConfirmationQueryL( 
    TRequestStatus& aStatus,
    const TDesC& /*aPrompt*/,
    TInt /*aSoftkeys*/,
    TInt /*aAnimation*/,
    const TDesC& /*aImageFile*/,
    TInt /*aImageId*/,
    TInt /*aImageMaskId*/,
    CAknQueryDialog::TTone /*aTone*/,
    TBool /*aDismissWithAllKeys*/ )

    {
    if ( stubStatus == KErrNone )
        {
        TRequestStatus* stat = &aStatus;
        User::RequestComplete( stat, EAknSoftkeyYes );
       // rstatus = &aStatus;
        }
    else
        {
        User::Leave( stubStatus );
        }
    
    }

void CAknGlobalConfirmationQuery::CancelConfirmationQuery()
    {
    if ( rstatus )
        {
        User::RequestComplete( rstatus, KErrCancel );
        rstatus = NULL;
        }
    }

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
    rstatus = NULL;
    stubStatus = KErrNone;
    iStartLiveSharingLCalled = EFalse;
    }


// ----------------------------------------------------------------------------
// Finalizes a test by releasing used resources.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::Teardown()
    {
    delete iIndicatorApi;
    iIndicatorApi = NULL;
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
// @TODO: Can this be tested without a dialog?
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_ConfirmationQueryLL()
    {
    TBool ret = iIndicatorApi->ConfirmationQueryL(
         CMusIndicatorApi::EVsRoamingActivationQuery );
    
    EUNIT_ASSERT( ret );
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
    /* NOTE: EUnit reports leak for this test due AVKON resource reservation,
     * which is not released by AVKON at end of test. Resource reservation
     * happend in contruction of indicator(dsa)
     */
    TRAPD( error, RProperty::Define(
                                KPSUidCoreApplicationUIs,
                                KCoreAppUIsVideoSharingIndicator,
                                RProperty::EInt,
                                0 ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
        
    TRAP( error, RProperty::Set(
            KPSUidCoreApplicationUIs,
            KCoreAppUIsVideoSharingIndicator,
            ( TInt ) ECoreAppUIsVideoSharingIndicatorOn ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
        
    iIndicatorApi->Cancel();
    iIndicatorApi->iStatus = KErrNotFound;
    iIndicatorApi->RunL();
    EUNIT_ASSERT( !iStartLiveSharingLCalled );
    
    iIndicatorApi->Cancel();
    iIndicatorApi->iStatus = KErrNone;
    iIndicatorApi->RunL();
    EUNIT_ASSERT( iStartLiveSharingLCalled );
    EUNIT_ASSERT( !iIndicatorApi->iQuery );
    
    delete iIndicatorApi->iLiveSharingQueryTimeout;
    iIndicatorApi->iLiveSharingQueryTimeout = NULL;
    iIndicatorApi->iLiveSharingQueryTimeout = 
                    CPeriodic::NewL( CActive::EPriorityStandard );
    
    iIndicatorApi->RunL();
    EUNIT_ASSERT( !iIndicatorApi->iLiveSharingQueryTimeout );
    
    EUNIT_ASSERT( !iIndicatorApi->IsSubscriber() );
    
    iIndicatorApi->iObserver = NULL;
    EUNIT_ASSERT( iIndicatorApi->IsSubscriber() );
    iIndicatorApi->Indicator( ETrue );
    
    iIndicatorApi->Cancel();
    
    iIndicatorApi->RunL();
    
    TRequestStatus& status = iIndicatorApi->iStatus;
    rstatus = &status;
    //User::RequestComplete( rstatus, KErrCancel ); 
    }


// ----------------------------------------------------------------------------
// Asserts that DoCancel removes current activity and deletes used resources.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_DoCancelL()
    {
    iIndicatorApi->ShowLiveSharingQueryL( EFalse );
    iIndicatorApi->Cancel();
    EUNIT_ASSERT( !iIndicatorApi->iQuery );
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


// ----------------------------------------------------------------------------
// Asserts that a note text is generated.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_NoteTextLCL()
    {
    HBufC* tmp = iIndicatorApi->NoteTextLC(
        CMusIndicatorApi::EVsRoamingActivationQuery );
    EUNIT_ASSERT( tmp );
    EUNIT_ASSERT( tmp->Length() );
    CleanupStack::PopAndDestroy();
    }


// ----------------------------------------------------------------------------
// Asserts that method creates a query and calls CActive::SetActive.
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_ShowLiveSharingQueryLL()
    {
    
    iIndicatorApi->ShowLiveSharingQueryL( EFalse );
    EUNIT_ASSERT( iIndicatorApi->iQuery );
    EUNIT_ASSERT( iIndicatorApi->IsActive() );
    
    iIndicatorApi->Cancel();
    EUNIT_ASSERT( !iIndicatorApi->iQuery );
    EUNIT_ASSERT( !iIndicatorApi->IsActive() );
    
    stubStatus = KErrNotFound;
    iIndicatorApi->ShowLiveSharingQueryL( EFalse );
    EUNIT_ASSERT( !iIndicatorApi->iQuery );
    EUNIT_ASSERT( !iIndicatorApi->IsActive() );
    
    
    }

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
//
void UT_CMusIndicatorApi::UT_CMusIndicatorApi_LiveSharingQueryTimeoutL()
    {
    iIndicatorApi->ShowLiveSharingQueryL( EFalse );
    EUNIT_ASSERT( iIndicatorApi->iQuery );
    EUNIT_ASSERT( iIndicatorApi->IsActive() );
    
    EUNIT_ASSERT( CMusIndicatorApi::LiveSharingQueryTimeout( iIndicatorApi ) == KErrNone );
    
    EUNIT_ASSERT( !iIndicatorApi->iQuery );
    EUNIT_ASSERT( !iIndicatorApi->IsActive() );
    
    
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
    "ConfirmationQueryL - test ",
    "CMusIndicatorApi",
    "ConfirmationQueryL",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorApi_ConfirmationQueryLL, Teardown )

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

EUNIT_TEST(
    "NoteTextLC - test ",
    "CMusIndicatorApi",
    "NoteTextLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorApi_NoteTextLCL, Teardown )

EUNIT_TEST(
    "ShowLiveSharingQueryL - test ",
    "CMusIndicatorApi",
    "ShowLiveSharingQueryL",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorApi_ShowLiveSharingQueryLL, Teardown )

EUNIT_TEST(
    "LiveSharingQueryTimeout - test ",
    "CMusIndicatorApi",
    "LiveSharingQueryTimeout",
    "FUNCTIONALITY",
    SetupL, UT_CMusIndicatorApi_LiveSharingQueryTimeoutL, Teardown )

    

EUNIT_END_TEST_TABLE
