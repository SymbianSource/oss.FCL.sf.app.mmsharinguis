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
#include "ut_musuilivesharingcontroller.h"
#include "musuilivesharingcontroller.h"
#include "musuilivesharingobserveradapter.h"
//#include "musestubs.h"
//#include "musengtestdefs.h"
#include "musuieventobserveradapter.h"
#include "musuidefinitions.h"
#include <musenglivesession.h>

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32property.h>

//#include <centralrepository.h>
//#include <mcedisplaysink.h>

using namespace NMusResourceApi;


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiLiveSharingController* UT_CMusUiLiveSharingController::NewL()
    {
    UT_CMusUiLiveSharingController* self = UT_CMusUiLiveSharingController::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiLiveSharingController* UT_CMusUiLiveSharingController::NewLC()
    {
    UT_CMusUiLiveSharingController* self = new( ELeave ) UT_CMusUiLiveSharingController();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiLiveSharingController::~UT_CMusUiLiveSharingController()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::SetupL()
    {
    
    iEventObserver = new (ELeave) CMusUiEventObserverAdapter();
    EUNIT_ASSERT( iEventObserver );

    iLiveSharingObserverAdapter = new (ELeave) CMusUiLiveSharingObserverAdapter();
    EUNIT_ASSERT( iLiveSharingObserverAdapter );


    RProperty::iIntProperty = 0;
    RProperty::iErrorCode = KErrNone;


    iLiveSharingController = CMusUiLiveSharingController::NewL( iLiveSharingObserverAdapter, TRect( 0,0,100,100));
    iEventController = static_cast<CMusUiEventController*>
                        ( iLiveSharingController );

    EUNIT_ASSERT( iEventController );
    iEventController->SetEventObserver( iEventObserver );
    EUNIT_ASSERT( iEventController->iObserver == iEventObserver );

    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::Teardown()
    {
    delete iLiveSharingController;
    iEventController = NULL;
    delete iEventObserver;
    delete iLiveSharingObserverAdapter;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_StartInvitationLL()
	{
	
    //iLiveSharingController->StartInvitationL();
    //EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleAsyncEventL );
    //EUNIT_ASSERT( iLiveSharingObserverAdapter->iHandleAsyncEventId == EMusUiAsyncStartInvitation );
	EUNIT_ASSERT( EFalse );	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_AddressFoundLL()
	{
	EUNIT_ASSERT( EFalse );	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_InviteLL()
	{
	
    // Precondititions
    iLiveSharingObserverAdapter->iErrorCode = KErrNone; 
    iEventController->iCameraAvailable = EAvailable;
    iEventController->iSipRegistrationPending = EFalse;
    
    // sip address length > 0 and no separator
    _LIT( KRes, "address" );
    delete iEventController->iRemoteSipAddress;
    iEventController->iRemoteSipAddress = KRes().AllocL();
    iLiveSharingController->InviteL();
    
    EUNIT_ASSERT( iLiveSharingController->iSession->iInvited );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EShowWaitDialogL );


    // sip address length > 0 and separator
    _LIT( KRes2, "Address1, Address2, Address3" );
    delete iEventController->iRemoteSipAddress;
    iEventController->iRemoteSipAddress = KRes2().AllocL();
    iLiveSharingController->iSession->iInvited = EFalse;
    iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::ENone;

    iLiveSharingController->InviteL();
    EUNIT_ASSERT( iLiveSharingController->iSession->iInvited );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EShowWaitDialogL );
    
    
    // sip addres length = 0
    _LIT( KRes3, "" );
    delete iEventController->iRemoteSipAddress;
    iEventController->iRemoteSipAddress = KRes3().AllocL();
    iLiveSharingController->iSession->iInvited = EFalse;
    iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::ENone;

    iLiveSharingController->InviteL();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EManualAddressEntryL );
   
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_InviteL2L()
	{
    // Precondititions
    iLiveSharingObserverAdapter->iErrorCode = KErrNone; 

    // registration not pending and triedinvitations > 1
    iEventController->iSipRegistrationPending = EFalse;
    iLiveSharingController->iSession->iInvited = EFalse;
    iLiveSharingController->iManualAddressTyped = EFalse;
    iLiveSharingController->iTriedInvitations = 2;

    _LIT( KRes, "address" );
    iLiveSharingController->InviteL( KRes );
    EUNIT_ASSERT( iLiveSharingController->iSession->iInvited );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EShowWaitDialogL );


    // registration not pending and triedinvitations > 1
    iLiveSharingController->iSession->iErrorCode = KErrArgument; 

    iEventController->iSipRegistrationPending = EFalse;
    iLiveSharingController->iSession->iInvited = EFalse;
    iLiveSharingController->iManualAddressTyped = EFalse;
    iLiveSharingController->iTriedInvitations = 2;

    iLiveSharingController->InviteL( KRes );
    EUNIT_ASSERT( iLiveSharingController->iSession->iInvited );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleExitL );


    // registration not pending and triedinvitations = 0 
    iLiveSharingController->iSession->iErrorCode = KErrArgument; 

    iEventController->iSipRegistrationPending = EFalse;
    iLiveSharingController->iSession->iInvited = EFalse;
    iLiveSharingController->iManualAddressTyped = EFalse;
    iLiveSharingController->iTriedInvitations = 0;

    _LIT( KProposal, "proposal_address" );
    delete iEventController->iRemoteSipAddressProposal;
    iEventController->iRemoteSipAddressProposal = KProposal().AllocL();
    
    // Manual address entry
    iLiveSharingController->InviteL( KRes );
    EUNIT_ASSERT( iLiveSharingController->iSession->iInvited );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EShowWaitDialogL );
    EUNIT_ASSERT( iLiveSharingController->iTriedInvitations == 1 );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iAddress->Des().Compare( KProposal ) == 0 );

	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SetRectL()
	{
	
	TRect rect = TRect( 0,0, 100,100 );
	
    iLiveSharingController->iSession->iErrorCode = KErrArgument;
    iLiveSharingController->SetRect( rect );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleError );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iError == KErrArgument );

    iLiveSharingController->iSession->iErrorCode = KErrNone;
    iLiveSharingController->SetRect( rect );
    EUNIT_ASSERT( iLiveSharingController->iSession->iRect == iLiveSharingController->CenteredRect( rect ) );
    
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_CancelInvitationLL()
	{

    // Connection established cancel	
	iLiveSharingController->iSession->iInvited = ETrue;
    iLiveSharingController->iConnectionEstablished = ETrue;
    iLiveSharingController->iManualAddressTyped = EFalse;

    iLiveSharingController->CancelInvitationL();
    EUNIT_ASSERT( iLiveSharingController->iSession->iInvited );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleExitL );


    // No connection cancel	
	iLiveSharingController->iSession->iInvited = ETrue;
    iLiveSharingController->iConnectionEstablished = EFalse;
    iLiveSharingController->iManualAddressTyped = EFalse;

    iLiveSharingController->CancelInvitationL();
    EUNIT_ASSERT( !iLiveSharingController->iSession->iInvited  );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleExitL );
    
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_HandleCommandLL()
	{
	// TODO: 
	
	EUNIT_ASSERT( EFalse );	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_PlayingL()
	{
    iLiveSharingController->iPlaying = ETrue;
    EUNIT_ASSERT( iLiveSharingController->Playing() );
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_CurrentZoomLL()
	{
    iLiveSharingController->iSession->iCurrentZoom = 14;
	EUNIT_ASSERT( iLiveSharingController->CurrentZoomL() == 14 );	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_MinZoomLL()
	{
    iLiveSharingController->iSession->iCurrentZoom = 13;
	EUNIT_ASSERT( iLiveSharingController->CurrentZoomL() == 13 );	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_MaxZoomLL()
	{
    iLiveSharingController->iSession->iCurrentZoom = 12;
	EUNIT_ASSERT( iLiveSharingController->CurrentZoomL() == 12 );	
	}
    
//private:	// new functions:
    


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_CenteredRectL()
	{
	EUNIT_ASSERT( EFalse );	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_ZoomInLL()
	{
    iLiveSharingController->iSession->iCurrentZoom = 12;
	EUNIT_ASSERT( iLiveSharingController->CurrentZoomL() == 13 );	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_ZoomOutLL()
	{
    iLiveSharingController->iSession->iCurrentZoom = 12;
	EUNIT_ASSERT( iLiveSharingController->CurrentZoomL() == 11 );	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SplitLL()
	{
	EUNIT_ASSERT( EFalse );	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_ShowErrorDialogL()
	{
	iLiveSharingObserverAdapter->iErrorCode = KErrNone;
	iLiveSharingController->ShowErrorDialog( 1 );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EShowErrorDialogL );

	iLiveSharingObserverAdapter->iErrorCode = KErrNone;
	iLiveSharingObserverAdapter->iShowErrorDialogErrorCode = KErrArgument;
	iLiveSharingController->ShowErrorDialog( 1 );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleError );

	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_DismissWaitDialogL()
	{
	iLiveSharingObserverAdapter->iErrorCode = KErrNone;
	iLiveSharingController->DismissWaitDialog();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EDismissWaitDialogL );

	iLiveSharingObserverAdapter->iErrorCode = KErrArgument;
	iLiveSharingController->DismissWaitDialog();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleError );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iError == KErrArgument );
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionRejectedL()
	{
	iLiveSharingController->SessionRejected();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleExitL );
	}                // e.g. 603 DECLINE


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionBadRequestL()
	{
	iLiveSharingController->SessionBadRequest();
	EUNIT_ASSERT( ETrue );	
	}                   // 400 


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionUnauthorizedL()
	{
	iLiveSharingController->SessionUnauthorized();
	EUNIT_ASSERT( ETrue );	
	}                 // 401 


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionPaymentRequiredL()
	{
	iLiveSharingController->SessionPaymentRequired();
	EUNIT_ASSERT( ETrue );	
	}              // 402


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionRecipientNotFoundL()
	{
	EUNIT_ASSERT( EFalse );	
	}            // 404


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionProxyAuthenticationRequiredL()
	{
	iLiveSharingController->SessionProxyAuthenticationRequired();
	EUNIT_ASSERT( ETrue );	
	}  // 407


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionRequestTimeOutL()
	{
    iLiveSharingController->SessionRequestTimeOut();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleExitL );
	}               // 408


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionUnsupportedMediaTypeL()
	{
    iLiveSharingController->SessionUnsupportedMediaType();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleExitL );
	}         // 415


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionRequestCancelledL()
	{
    iLiveSharingController->SessionRequestCancelled();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleExitL );
	}             // 487

//public:	// from MMusEngSessionObserver



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionEstablishedL()
	{

    iLiveSharingController->SessionEstablished();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleExitL );


	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionTerminatedL()
	{
    iLiveSharingController->SessionTerminated();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleExitL );
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionConnectionLostL()
	{
    iLiveSharingController->SessionConnectionLost();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleExitL );
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionFailedL()
	{
    iLiveSharingController->SessionFailed();
	EUNIT_ASSERT( ETrue );	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_StreamIdleL()
	{
    iLiveSharingController->StreamIdle();
	EUNIT_ASSERT( ETrue );	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_StreamStreamingL()
	{
    iLiveSharingController->StreamStreaming();
	EUNIT_ASSERT( ETrue );	
	}	


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_SessionTimeChangedL()
	{
    iLiveSharingController->SessionTimeChanged( TTimeIntervalSeconds( 1  ) );
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EUpdateSessionTime );
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiLiveSharingController::UT_CMusUiLiveSharingController_InactivityTimeoutL()
	{
    iLiveSharingController->InactivityTimeout();
    EUNIT_ASSERT( iLiveSharingObserverAdapter->iCalledObserver == CMusUiLiveSharingObserverAdapter::EHandleExitL );
	}



// TEST CASES


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusUiLiveSharingController,
    "UT_CMusUiLiveSharingController",
    "UNIT" )

EUNIT_TEST(
    "StartInvitationL - test ",
    "UT_CMusUiLiveSharingController",
    "StartInvitationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_StartInvitationLL, Teardown)

EUNIT_TEST(
    "AddressFoundL - test ",
    "UT_CMusUiLiveSharingController",
    "AddressFoundL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_AddressFoundLL, Teardown)

EUNIT_TEST(
    "InviteL - test ",
    "UT_CMusUiLiveSharingController",
    "InviteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_InviteLL, Teardown)

EUNIT_TEST(
    "InviteL2 - test ",
    "UT_CMusUiLiveSharingController",
    "InviteL2",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_InviteL2L, Teardown)

EUNIT_TEST(
    "SetRect - test ",
    "UT_CMusUiLiveSharingController",
    "SetRect",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SetRectL, Teardown)

EUNIT_TEST(
    "CancelInvitationL - test ",
    "UT_CMusUiLiveSharingController",
    "CancelInvitationL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_CancelInvitationLL, Teardown)

EUNIT_TEST(
    "HandleCommandL - test ",
    "UT_CMusUiLiveSharingController",
    "HandleCommandL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_HandleCommandLL, Teardown)

EUNIT_TEST(
    "PlayingL - test ",
    "UT_CMusUiLiveSharingController",
    "PlayingL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_PlayingL, Teardown)

EUNIT_TEST(
    "CurrentZoomL - test ",
    "UT_CMusUiLiveSharingController",
    "CurrentZoomL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_CurrentZoomLL, Teardown)

EUNIT_TEST(
    "MinZoomL - test ",
    "UT_CMusUiLiveSharingController",
    "MinZoomL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_MinZoomLL, Teardown)

EUNIT_TEST(
    "MaxZoomL - test ",
    "UT_CMusUiLiveSharingController",
    "MaxZoomL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_MaxZoomLL, Teardown)

EUNIT_TEST(
    "CenteredRect - test ",
    "UT_CMusUiLiveSharingController",
    "CenteredRect",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_CenteredRectL, Teardown)

EUNIT_TEST(
    "ZoomInL - test ",
    "UT_CMusUiLiveSharingController",
    "ZoomInL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_ZoomInLL, Teardown)

EUNIT_TEST(
    "ZoomOutL - test ",
    "UT_CMusUiLiveSharingController",
    "ZoomOutL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_ZoomOutLL, Teardown)

EUNIT_TEST(
    "SplitL - test ",
    "UT_CMusUiLiveSharingController",
    "SplitL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SplitLL, Teardown)

EUNIT_TEST(
    "ShowErrorDialog - test ",
    "UT_CMusUiLiveSharingController",
    "ShowErrorDialog",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_ShowErrorDialogL, Teardown)

EUNIT_TEST(
    "DismissWaitDialog - test ",
    "UT_CMusUiLiveSharingController",
    "DismissWaitDialog",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_DismissWaitDialogL, Teardown)

EUNIT_TEST(
    "SessionRejected - test ",
    "UT_CMusUiLiveSharingController",
    "SessionRejected",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionRejectedL, Teardown)

EUNIT_TEST(
    "SessionUnauthorized - test ",
    "UT_CMusUiLiveSharingController",
    "SessionUnauthorized",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionUnauthorizedL, Teardown)

EUNIT_TEST(
    "SessionPaymentRequired - test ",
    "UT_CMusUiLiveSharingController",
    "SessionPaymentRequired",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionPaymentRequiredL, Teardown)

EUNIT_TEST(
    "SessionRecipientNotFound - test ",
    "UT_CMusUiLiveSharingController",
    "SessionRecipientNotFound",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionRecipientNotFoundL, Teardown)

EUNIT_TEST(
    "SessionProxyAuthenticationRequired - test ",
    "UT_CMusUiLiveSharingController",
    "SessionProxyAuthenticationRequired",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionProxyAuthenticationRequiredL, Teardown)

EUNIT_TEST(
    "SessionRequestTimeOut - test ",
    "UT_CMusUiLiveSharingController",
    "SessionRequestTimeOut",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionRequestTimeOutL, Teardown)

EUNIT_TEST(
    "SessionUnsupportedMediaType - test ",
    "UT_CMusUiLiveSharingController",
    "SessionUnsupportedMediaType",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionUnsupportedMediaTypeL, Teardown)

EUNIT_TEST(
    "SessionRequestCancelled - test ",
    "UT_CMusUiLiveSharingController",
    "SessionRequestCancelled",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionRequestCancelledL, Teardown)

EUNIT_TEST(
    "SessionEstablished - test ",
    "UT_CMusUiLiveSharingController",
    "SessionEstablished",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionEstablishedL, Teardown)

EUNIT_TEST(
    "SessionTerminated - test ",
    "UT_CMusUiLiveSharingController",
    "SessionTerminated",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionTerminatedL, Teardown)

EUNIT_TEST(
    "SessionConnectionLost - test ",
    "UT_CMusUiLiveSharingController",
    "SessionConnectionLost",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionConnectionLostL, Teardown)

EUNIT_TEST(
    "SessionFailed - test ",
    "UT_CMusUiLiveSharingController",
    "SessionFailed",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionFailedL, Teardown)

EUNIT_TEST(
    "StreamIdle - test ",
    "UT_CMusUiLiveSharingController",
    "StreamIdle",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_StreamIdleL, Teardown)

EUNIT_TEST(
    "StreamStreaming - test ",
    "UT_CMusUiLiveSharingController",
    "StreamStreaming",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_StreamStreamingL, Teardown)

EUNIT_TEST(
    "SessionTimeChanged - test ",
    "UT_CMusUiLiveSharingController",
    "SessionTimeChanged",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_SessionTimeChangedL, Teardown)

EUNIT_TEST(
    "InactivityTimeout - test ",
    "UT_CMusUiLiveSharingController",
    "InactivityTimeout",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiLiveSharingController_InactivityTimeoutL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE
