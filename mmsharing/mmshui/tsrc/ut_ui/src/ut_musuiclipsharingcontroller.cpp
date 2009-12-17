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
#include "ut_musuiclipsharingcontroller.h"
#include "musuiclipsharingcontroller.h"
#include "musuiclipsharingobserveradapter.h"
#include "musuieventobserveradapter.h"
#include "musuidefinitions.h"
#include "musengclipsession.h"
#include <e32property.h>
//#include "musestubs.h"
//#include "musengtestdefs.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
//#include <centralrepository.h>
//#include <mcedisplaysink.h>




// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiClipSharingController* UT_CMusUiClipSharingController::NewL()
    {
    UT_CMusUiClipSharingController* self = UT_CMusUiClipSharingController::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiClipSharingController* UT_CMusUiClipSharingController::NewLC()
    {
    UT_CMusUiClipSharingController* self = new( ELeave ) UT_CMusUiClipSharingController();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiClipSharingController::~UT_CMusUiClipSharingController()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusUiClipSharingController::UT_CMusUiClipSharingController()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusUiClipSharingController::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiClipSharingController::SetupL()
    {
    iEventObserver = new (ELeave) CMusUiEventObserverAdapter();
    EUNIT_ASSERT( iEventObserver );

    iClipSharingObserverAdapter = new (ELeave) CMusUiClipSharingObserverAdapter();
    EUNIT_ASSERT( iClipSharingObserverAdapter );


    RProperty::iIntProperty = 0;
    RProperty::iErrorCode = KErrNone;


    iClipSharingController = CMusUiClipSharingController::NewL( iClipSharingObserverAdapter, TRect( 0,0,100,100));
    iEventController = static_cast<CMusUiEventController*>
                        ( iClipSharingController );

    EUNIT_ASSERT( iEventController );
    iEventController->SetEventObserver( iEventObserver );
    EUNIT_ASSERT( iEventController->iObserver == iEventObserver );
    
    // Need fake active object event...
    _LIT( KFileName , "clip.3gp" );
    iClipSharingController->iFileName = KFileName().AllocL();
    iClipSharingController->HandleAsyncEventL( EMusUiAsyncStartMediaGallery );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiClipSharingController::Teardown()
    {
    delete iClipSharingController;
    iEventController = NULL;
    delete iEventObserver;
    delete iClipSharingObserverAdapter;
    }



// TEST CASES


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_StartInvitationLL()
    {
    // TODO:
    EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_AddressFoundLL()
    {
    EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_InviteLL()
    {
    // Precondititions
    iClipSharingObserverAdapter->iErrorCode = KErrNone; 
    iEventController->iCameraAvailable = EAvailable;
    iEventController->iSipRegistrationPending = EFalse;
    
    // sip address length > 0 and no separator
    _LIT( KRes, "address" );
    delete iEventController->iRemoteSipAddress;
    iEventController->iRemoteSipAddress = KRes().AllocL();
    iClipSharingController->InviteL();
    
    EUNIT_ASSERT( iClipSharingController->iSession->iInvited );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EShowWaitDialogL );


    // sip address length > 0 and separator
    _LIT( KRes2, "Address1, Address2, Address3" );
    delete iEventController->iRemoteSipAddress;
    iEventController->iRemoteSipAddress = KRes2().AllocL();
    iClipSharingController->iSession->iInvited = EFalse;
    iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::ENone;

    iClipSharingController->InviteL();
    EUNIT_ASSERT( iClipSharingController->iSession->iInvited );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EShowWaitDialogL );
    
    
    // sip addres length = 0
    _LIT( KRes3, "" );
    delete iEventController->iRemoteSipAddress;
    iEventController->iRemoteSipAddress = KRes3().AllocL();
    iClipSharingController->iSession->iInvited = EFalse;
    iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::ENone;

    iClipSharingController->InviteL();
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EManualAddressEntryL );
   
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_InviteL2L()
    {
    // Precondititions
    iClipSharingObserverAdapter->iErrorCode = KErrNone; 

    // registration not pending and triedinvitations > 1
    iEventController->iSipRegistrationPending = EFalse;
    iClipSharingController->iSession->iInvited = EFalse;
    iClipSharingController->iManualAddressTyped = EFalse;
    iClipSharingController->iTriedInvitations = 2;

    _LIT( KRes, "address" );
    iClipSharingController->InviteL( KRes );
    EUNIT_ASSERT( iClipSharingController->iSession->iInvited );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EShowWaitDialogL );


    // registration not pending and triedinvitations > 1
    iClipSharingController->iSession->iErrorCode = KErrArgument; 

    iEventController->iSipRegistrationPending = EFalse;
    iClipSharingController->iSession->iInvited = EFalse;
    iClipSharingController->iManualAddressTyped = EFalse;
    iClipSharingController->iTriedInvitations = 2;

    iClipSharingController->InviteL( KRes );
    EUNIT_ASSERT( iClipSharingController->iSession->iInvited );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleExitL );


    // registration not pending and triedinvitations = 0 
    iClipSharingController->iSession->iErrorCode = KErrArgument; 

    iEventController->iSipRegistrationPending = EFalse;
    iClipSharingController->iSession->iInvited = EFalse;
    iClipSharingController->iManualAddressTyped = EFalse;
    iClipSharingController->iTriedInvitations = 0;

    _LIT( KProposal, "proposal_address" );
    delete iEventController->iRemoteSipAddressProposal;
    iEventController->iRemoteSipAddressProposal = KProposal().AllocL();
    
    // Manual address entry
    iClipSharingController->InviteL( KRes );
    EUNIT_ASSERT( iClipSharingController->iSession->iInvited );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EShowWaitDialogL );
    EUNIT_ASSERT( iClipSharingController->iTriedInvitations == 1 );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iAddress->Des().Compare( KProposal ) == 0 );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SetRectL()
    {
	TRect rect = TRect( 0,0, 100,100 );
	
    iClipSharingController->iSession->iErrorCode = KErrArgument;
    iClipSharingController->SetRect( rect );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleError );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iError == KErrArgument );

    iClipSharingController->iSession->iErrorCode = KErrNone;
    iClipSharingController->SetRect( rect );
    EUNIT_ASSERT( iClipSharingController->iSession->iRect == rect );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_CancelInvitationLL()
    {
    // Connection established cancel	
	iClipSharingController->iSession->iInvited = ETrue;
    iClipSharingController->iConnectionEstablished = ETrue;
    iClipSharingController->iManualAddressTyped = EFalse;

    iClipSharingController->CancelInvitationL();
    EUNIT_ASSERT( iClipSharingController->iSession->iInvited );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleExitL );


    // No connection cancel	
	iClipSharingController->iSession->iInvited = ETrue;
    iClipSharingController->iConnectionEstablished = EFalse;
    iClipSharingController->iManualAddressTyped = EFalse;

    iClipSharingController->CancelInvitationL();
    EUNIT_ASSERT( !iClipSharingController->iSession->iInvited  );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleExitL );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_HandleCommandLL()
    {
    EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_PlayingL()
    {
    iClipSharingController->iPlaying = ETrue;
    EUNIT_ASSERT( iClipSharingController->Playing() );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SetFileNameLL()
    {
    _LIT( KRes, "filename" );
    delete iClipSharingController->iFileName;
    iClipSharingController->SetFileNameL( KRes );
    EUNIT_ASSERT( iClipSharingController->iFileName->Des().Compare( KRes ) == 0 );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_ClipEndedL()
    {
    iClipSharingController->iClipEnded = ETrue;
    EUNIT_ASSERT( iClipSharingController->ClipEnded() );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_ZoomInLL()
    {
    iClipSharingController->ZoomInL();
    EUNIT_ASSERT( ETrue );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_ZoomOutLL()
    {
    iClipSharingController->ZoomOutL();
    EUNIT_ASSERT( ETrue );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SplitLL()
    {
    EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_ShowErrorDialogL()
    {
	iClipSharingObserverAdapter->iErrorCode = KErrNone;
	iClipSharingController->ShowErrorDialog( 1 );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EShowErrorDialogL );

	iClipSharingObserverAdapter->iErrorCode = KErrNone;
	iClipSharingObserverAdapter->iShowErrorDialogErrorCode = KErrArgument;
	iClipSharingController->ShowErrorDialog( 1 );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleError );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_DismissWaitDialogL()
    {
	iClipSharingObserverAdapter->iErrorCode = KErrNone;
	iClipSharingController->DismissWaitDialog();
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EDismissWaitDialogL );

	iClipSharingObserverAdapter->iErrorCode = KErrArgument;
	iClipSharingController->DismissWaitDialog();
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleError );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iError == KErrArgument );
    }

void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_EndOfClipL()
    {
    iClipSharingController->iClipEnded = EFalse;
    iClipSharingController->EndOfClip();
    EUNIT_ASSERT( iClipSharingController->iClipEnded );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionRejectedL()
    {
	iClipSharingController->SessionRejected();
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleExitL );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionBadRequestL()
    {
	iClipSharingController->SessionBadRequest();
    EUNIT_ASSERT( ETrue );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionUnauthorizedL()
    {
	iClipSharingController->SessionUnauthorized();
    EUNIT_ASSERT( ETrue );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionPaymentRequiredL()
    {
	iClipSharingController->SessionPaymentRequired();
    EUNIT_ASSERT( ETrue );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionRecipientNotFoundL()
    {
    EUNIT_ASSERT( EFalse );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionProxyAuthenticationRequiredL()
    {
	iClipSharingController->SessionProxyAuthenticationRequired();
    EUNIT_ASSERT( ETrue );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionRequestTimeOutL()
    {
    iClipSharingController->SessionRequestTimeOut();
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleExitL );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionUnsupportedMediaTypeL()
    {
    iClipSharingController->SessionUnsupportedMediaType();
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleExitL );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionRequestCancelledL()
    {
    iClipSharingController->SessionRequestCancelled();
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleExitL );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionEstablishedL()
    {
    iClipSharingController->SessionEstablished();
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EShowClipSharingPanesL );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionTerminatedL()
    {
    iClipSharingController->SessionTerminated();
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleExitL );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionConnectionLostL()
    {
    iClipSharingController->SessionConnectionLost();
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleExitL );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionFailedL()
    {
    iClipSharingController->SessionFailed();
	EUNIT_ASSERT( ETrue );	
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_StreamIdleL()
    {
    iClipSharingController->StreamIdle();
	EUNIT_ASSERT( ETrue );	
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_StreamStreamingL()
    {
    iClipSharingController->StreamStreaming();
	EUNIT_ASSERT( ETrue );	
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_SessionTimeChangedL()
    {
    iClipSharingController->SessionTimeChanged( TTimeIntervalSeconds( 1  ) );
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EUpdateSessionTime );
    }


void UT_CMusUiClipSharingController::UT_CMusUiClipSharingController_InactivityTimeoutL()
    {
    iClipSharingController->InactivityTimeout();
    EUNIT_ASSERT( iClipSharingObserverAdapter->iCalledObserver == CMusUiClipSharingObserverAdapter::EHandleExitL );
    }




//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusUiClipSharingController,
    "UT_CMusUiClipSharingController",
    "UNIT" )

EUNIT_TEST(
    "StartInvitationL - test ",
    "CMusUiClipSharingController",
    "StartInvitationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_StartInvitationLL, Teardown)

EUNIT_TEST(
    "AddressFoundL - test ",
    "CMusUiClipSharingController",
    "AddressFoundL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_AddressFoundLL, Teardown)

EUNIT_TEST(
    "InviteL - test ",
    "CMusUiClipSharingController",
    "InviteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_InviteLL, Teardown)

EUNIT_TEST(
    "InviteL2 - test ",
    "CMusUiClipSharingController",
    "InviteL2",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_InviteL2L, Teardown)

EUNIT_TEST(
    "SetRect - test ",
    "CMusUiClipSharingController",
    "SetRect",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SetRectL, Teardown)

EUNIT_TEST(
    "CancelInvitationL - test ",
    "CMusUiClipSharingController",
    "CancelInvitationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_CancelInvitationLL, Teardown)

EUNIT_TEST(
    "HandleCommandL - test ",
    "CMusUiClipSharingController",
    "HandleCommandL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_HandleCommandLL, Teardown)

EUNIT_TEST(
    "Playing - test ",
    "CMusUiClipSharingController",
    "Playing",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_PlayingL, Teardown)

EUNIT_TEST(
    "SetFileNameL - test ",
    "CMusUiClipSharingController",
    "SetFileNameL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SetFileNameLL, Teardown)

EUNIT_TEST(
    "ClipEnded - test ",
    "CMusUiClipSharingController",
    "ClipEnded",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_ClipEndedL, Teardown)

EUNIT_TEST(
    "ZoomInL - test ",
    "CMusUiClipSharingController",
    "ZoomInL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_ZoomInLL, Teardown)

EUNIT_TEST(
    "ZoomOutL - test ",
    "CMusUiClipSharingController",
    "ZoomOutL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_ZoomOutLL, Teardown)

EUNIT_TEST(
    "SplitL - test ",
    "CMusUiClipSharingController",
    "SplitL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SplitLL, Teardown)

EUNIT_TEST(
    "ShowErrorDialog - test ",
    "CMusUiClipSharingController",
    "ShowErrorDialog",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_ShowErrorDialogL, Teardown)

EUNIT_TEST(
    "DismissWaitDialog - test ",
    "CMusUiClipSharingController",
    "DismissWaitDialog",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_DismissWaitDialogL, Teardown)

EUNIT_TEST(
    "EndOfClip - test ",
    "CMusUiClipSharingController",
    "EndOfClip",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_EndOfClipL, Teardown)

EUNIT_TEST(
    "SessionRejected - test ",
    "CMusUiClipSharingController",
    "SessionRejected",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionRejectedL, Teardown)

EUNIT_TEST(
    "SessionBadRequest - test ",
    "CMusUiClipSharingController",
    "SessionBadRequest",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionBadRequestL, Teardown)

EUNIT_TEST(
    "SessionUnauthorized - test ",
    "CMusUiClipSharingController",
    "SessionUnauthorized",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionUnauthorizedL, Teardown)

EUNIT_TEST(
    "SessionPaymentRequired - test ",
    "CMusUiClipSharingController",
    "SessionPaymentRequired",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionPaymentRequiredL, Teardown)

EUNIT_TEST(
    "SessionRecipientNotFound - test ",
    "CMusUiClipSharingController",
    "SessionRecipientNotFound",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionRecipientNotFoundL, Teardown)

EUNIT_TEST(
    "SessionProxyAuthenticationRequired - test ",
    "CMusUiClipSharingController",
    "SessionProxyAuthenticationRequired",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionProxyAuthenticationRequiredL, Teardown)

EUNIT_TEST(
    "SessionRequestTimeOut - test ",
    "CMusUiClipSharingController",
    "SessionRequestTimeOut",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionRequestTimeOutL, Teardown)

EUNIT_TEST(
    "SessionUnsupportedMediaType - test ",
    "CMusUiClipSharingController",
    "SessionUnsupportedMediaType",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionUnsupportedMediaTypeL, Teardown)

EUNIT_TEST(
    "SessionRequestCancelled - test ",
    "CMusUiClipSharingController",
    "SessionRequestCancelled",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionRequestCancelledL, Teardown)

EUNIT_TEST(
    "SessionEstablished - test ",
    "CMusUiClipSharingController",
    "SessionEstablished",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionEstablishedL, Teardown)

EUNIT_TEST(
    "SessionTerminated - test ",
    "CMusUiClipSharingController",
    "SessionTerminated",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionTerminatedL, Teardown)

EUNIT_TEST(
    "SessionConnectionLost - test ",
    "CMusUiClipSharingController",
    "SessionConnectionLost",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionConnectionLostL, Teardown)

EUNIT_TEST(
    "SessionFailed - test ",
    "CMusUiClipSharingController",
    "SessionFailed",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionFailedL, Teardown)

EUNIT_TEST(
    "StreamIdle - test ",
    "CMusUiClipSharingController",
    "StreamIdle",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_StreamIdleL, Teardown)

EUNIT_TEST(
    "StreamStreaming - test ",
    "CMusUiClipSharingController",
    "StreamStreaming",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_StreamStreamingL, Teardown)

EUNIT_TEST(
    "SessionTimeChanged - test ",
    "CMusUiClipSharingController",
    "SessionTimeChanged",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_SessionTimeChangedL, Teardown)

EUNIT_TEST(
    "InactivityTimeout - test ",
    "CMusUiClipSharingController",
    "InactivityTimeout",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiClipSharingController_InactivityTimeoutL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
