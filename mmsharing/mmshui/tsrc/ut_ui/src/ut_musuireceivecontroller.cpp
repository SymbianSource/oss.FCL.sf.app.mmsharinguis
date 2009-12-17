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
#include "ut_musuireceivecontroller.h"
#include "musuireceivecontroller.h"
#include "musuireceiveobserveradapter.h"
//#include "musestubs.h"
//#include "musengtestdefs.h"
#include "musuieventobserveradapter.h"
#include "musuidefinitions.h"
#include <musengreceivesession.h>
#include <stringloader.h>

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32property.h>
#include <aknappui.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiReceiveController* UT_CMusUiReceiveController::NewL()
    {
    UT_CMusUiReceiveController* self = UT_CMusUiReceiveController::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiReceiveController* UT_CMusUiReceiveController::NewLC()
    {
    UT_CMusUiReceiveController* self = new( ELeave ) UT_CMusUiReceiveController();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusUiReceiveController::~UT_CMusUiReceiveController()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusUiReceiveController::UT_CMusUiReceiveController()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::SetupL()
    {
      
    iEventObserver = new (ELeave) CMusUiEventObserverAdapter();
    EUNIT_ASSERT( iEventObserver );

    iReceiveObserverAdapter = new (ELeave) CMusUiReceiveObserverAdapter();
    EUNIT_ASSERT( iReceiveObserverAdapter );


    RProperty::iIntProperty = 0;
    RProperty::iErrorCode = KErrNone;


    iReceiveController = CMusUiReceiveController::NewL( *iReceiveObserverAdapter, TRect( 0,0,100,100));
    iEventController = static_cast<CMusUiEventController*>
                        ( iReceiveController );

    EUNIT_ASSERT( iEventController );
    iEventController->SetEventObserver( iEventObserver );
    EUNIT_ASSERT( iEventController->iObserver == iEventObserver );  
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::Teardown()
    {
    delete iReceiveController;
    iEventController = NULL;
    delete iEventObserver;
    delete iReceiveObserverAdapter;
   }



// TEST CASES

    
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_HandleCommandLL()
    {
    EUNIT_ASSERT( EFalse );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_SizeChangedL()
    {
    
    TRect rect = TRect( 0,0, 100, 100 );
    
    iReceiveController->iSession->iRect = rect;
    iReceiveController->SizeChanged( rect );
    EUNIT_ASSERT( iReceiveController->iSession->iRect == iReceiveController->CenteredRect( rect ) );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_ShowErrorDialogLL()
    {
    _LIT( KRes, "res" );
    //StringLoader::iReturnValue = KRes().AllocLC();
    iReceiveController->ShowErrorDialogL( 1 );
    EUNIT_ASSERT( iReceiveObserverAdapter->iCalledObserver == CMusUiReceiveObserverAdapter::EShowErrorDialog );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_HandleIncomingSessionLL()
    {
    _LIT( KRes, "originator" );

    // Not initalized 
    delete iReceiveController->iOriginator;
    iReceiveController->iOriginator = NULL;
    iReceiveController->HandleIncomingSessionL( KRes );
    EUNIT_ASSERT( iReceiveController->iOriginator->Des().Compare( KRes ) == 0 );
    EUNIT_ASSERT( iReceiveObserverAdapter->iCalledObserver == CMusUiReceiveObserverAdapter::EShowInvitationQueryL );


    _LIT( KEmpty, "" );
    // Initalized 
    delete iReceiveController->iOriginator;
    iReceiveController->iOriginator = KEmpty().AllocL();
    iReceiveController->HandleIncomingSessionL( KRes );
    EUNIT_ASSERT( iReceiveController->iOriginator->Des().Compare( KRes ) == 0 );
    EUNIT_ASSERT( iReceiveObserverAdapter->iCalledObserver == CMusUiReceiveObserverAdapter::EShowInvitationQueryL );

    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_CancelInvitationLL()
    {
    iReceiveController->CancelInvitationL();
    EUNIT_ASSERT( !iReceiveController->iSession->iAccepInvitation );
    }

    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_ContinueInvitationLL()
    {
    iReceiveController->ContinueInvitationL();
    EUNIT_ASSERT( ETrue );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_HandleAsyncEventLL()
    {
    EUNIT_ASSERT( EFalse );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_ConnectionEstablishedL()
    {
    iReceiveController->iConnectionEstablished = ETrue;
    EUNIT_ASSERT( iReceiveController->ConnectionEstablished() );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_PlayLL()
    {
    iReceiveController->PlayL();
    EUNIT_ASSERT( ETrue );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_PauseLL()
    {
    iReceiveController->PauseL();
    EUNIT_ASSERT( ETrue );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_EnableDisplayLL()
    {
    iReceiveController->iSession->iDisplayEnabled = EFalse;
    iReceiveController->EnableDisplayL( ETrue );
    EUNIT_ASSERT( iReceiveController->iSession->iDisplayEnabled );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_ShowNaviPaneIconLL()
    {
    iReceiveController->ShowNaviPaneIconL( EMusUiNaviIconPlay );
    EUNIT_ASSERT( iReceiveObserverAdapter->iIcon == EMusUiNaviIconPlay );
    EUNIT_ASSERT( iReceiveObserverAdapter->iVisible );
    EUNIT_ASSERT( iReceiveObserverAdapter->iCalledObserver == CMusUiReceiveObserverAdapter::EShowNaviPaneIconL );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_ChangeOrientationLL()
    {

    iReceiveController->ChangeOrientationL( CAknAppUiBase::EAppUiOrientationPortrait );
    EUNIT_ASSERT( iReceiveObserverAdapter->iOrientation == CAknAppUiBase::EAppUiOrientationPortrait );
    EUNIT_ASSERT( iReceiveObserverAdapter->iCalledObserver == CMusUiReceiveObserverAdapter::EChangeOrientationL );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_CenteredRectL()
    {
    
    
    EUNIT_ASSERT( EFalse );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_ManualAddressTypedL()
    {
    EUNIT_ASSERT( !iReceiveController->ManualAddressTyped() );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_ShowInformationDialogLL()
    {
    iReceiveController->ShowInformationDialogL( 12 );
    EUNIT_ASSERT( iReceiveObserverAdapter->iResourceId == 12 );
    EUNIT_ASSERT( iReceiveObserverAdapter->iCalledObserver == CMusUiReceiveObserverAdapter::EShowInformationDialogL );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_ShutdownLL()
    {
    iReceiveController->HandleExitL();
    EUNIT_ASSERT( iReceiveObserverAdapter->iCalledObserver == CMusUiReceiveObserverAdapter::EHandleExitL );
    }
    
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_IncomingSessionL()
    {
    EUNIT_ASSERT( EFalse );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_StreamBufferingL()
    {
    EUNIT_ASSERT( EFalse );
    }

    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_SessionEstablishedL()
    {
    EUNIT_ASSERT( EFalse );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_SessionTerminatedL()
    {
    EUNIT_ASSERT( EFalse );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_SessionConnectionLostL()
    {
    EUNIT_ASSERT( EFalse );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_SessionFailedL()
    {
    iReceiveController->SessionFailed();
    EUNIT_ASSERT( iReceiveObserverAdapter->iCalledObserver == CMusUiReceiveObserverAdapter::EDismissWaitDialogL );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_StreamIdleL()
    {
    iReceiveController->StreamIdle();
    EUNIT_ASSERT( ETrue );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_StreamStreamingL()
    {
    EUNIT_ASSERT( EFalse );
    }	
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_SessionTimeChangedL()
    {
    iReceiveController->SessionTimeChanged( TTimeIntervalSeconds( 13  ) );
    EUNIT_ASSERT( iReceiveObserverAdapter->iCalledObserver == CMusUiReceiveObserverAdapter::EUpdateSessionTime );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_InactivityTimeoutL()
    {
    iReceiveController->HandleExitL();
    EUNIT_ASSERT( iReceiveObserverAdapter->iCalledObserver == CMusUiReceiveObserverAdapter::EHandleExitL );
    }
	
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_InvitationAcceptedLL()
    {
    EUNIT_ASSERT( EFalse );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_InvitationRejectedLL()
    {
    iReceiveController->InvitationRejectedL();
    EUNIT_ASSERT( !iReceiveController->iSession->iAccepInvitation );
    EUNIT_ASSERT( iReceiveObserverAdapter->iCalledObserver == CMusUiReceiveObserverAdapter::EHandleExitL );
    }
	
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_UpdateStatusPaneLL()
    {
    EUNIT_ASSERT( EFalse );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_InspectVideoPlayerLL()
    {
    EUNIT_ASSERT( EFalse );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusUiReceiveController::UT_CMusUiReceiveController_ShowWaitDialogLL()
    {
    EUNIT_ASSERT( EFalse );
    }
    



//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusUiReceiveController,
    "UT_CMusUiReceiveController",
    "UNIT" )

EUNIT_TEST(
    "HandleCommandLL - test ",
    "CMusUiReceiveController",
    "HandleCommandLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_HandleCommandLL, Teardown)

EUNIT_TEST(
    "SizeChangedL - test ",
    "CMusUiReceiveController",
    "SizeChangedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_SizeChangedL, Teardown)

EUNIT_TEST(
    "ShowErrorDialogLL - test ",
    "CMusUiReceiveController",
    "ShowErrorDialogLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_ShowErrorDialogLL, Teardown)

EUNIT_TEST(
    "HandleIncomingSessionLL - test ",
    "CMusUiReceiveController",
    "HandleIncomingSessionLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_HandleIncomingSessionLL, Teardown)

EUNIT_TEST(
    "CancelInvitationLL - test ",
    "CMusUiReceiveController",
    "CancelInvitationLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_CancelInvitationLL, Teardown)

EUNIT_TEST(
    "ContinueInvitationLL - test ",
    "CMusUiReceiveController",
    "ContinueInvitationLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_ContinueInvitationLL, Teardown)

EUNIT_TEST(
    "HandleAsyncEventLL - test ",
    "CMusUiReceiveController",
    "HandleAsyncEventLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_HandleAsyncEventLL, Teardown)

EUNIT_TEST(
    "ConnectionEstablishedL - test ",
    "CMusUiReceiveController",
    "ConnectionEstablishedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_ConnectionEstablishedL, Teardown)

EUNIT_TEST(
    "PlayLL - test ",
    "CMusUiReceiveController",
    "PlayLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_PlayLL, Teardown)

EUNIT_TEST(
    "PauseLL - test ",
    "CMusUiReceiveController",
    "PauseLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_PauseLL, Teardown)

EUNIT_TEST(
    "EnableDisplayLL - test ",
    "CMusUiReceiveController",
    "EnableDisplayLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_EnableDisplayLL, Teardown)

EUNIT_TEST(
    "ShowNaviPaneIconLL - test ",
    "CMusUiReceiveController",
    "ShowNaviPaneIconLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_ShowNaviPaneIconLL, Teardown)

EUNIT_TEST(
    "ChangeOrientationLL - test ",
    "CMusUiReceiveController",
    "ChangeOrientationLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_ChangeOrientationLL, Teardown)

EUNIT_TEST(
    "CenteredRectL - test ",
    "CMusUiReceiveController",
    "CenteredRectL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_CenteredRectL, Teardown)

EUNIT_TEST(
    "ManualAddressTypedL - test ",
    "CMusUiReceiveController",
    "ManualAddressTypedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_ManualAddressTypedL, Teardown)

EUNIT_TEST(
    "ShowInformationDialogLL - test ",
    "CMusUiReceiveController",
    "ShowInformationDialogLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_ShowInformationDialogLL, Teardown)

EUNIT_TEST(
    "ShowInformationDialogLL - test ",
    "CMusUiReceiveController",
    "ShowInformationDialogLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_ShowInformationDialogLL, Teardown)

EUNIT_TEST(
    "ShutdownLL - test ",
    "CMusUiReceiveController",
    "ShutdownLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_ShutdownLL, Teardown)

EUNIT_TEST(
    "IncomingSessionL - test ",
    "CMusUiReceiveController",
    "IncomingSessionL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_IncomingSessionL, Teardown)

EUNIT_TEST(
    "StreamBufferingL - test ",
    "CMusUiReceiveController",
    "StreamBufferingL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_StreamBufferingL, Teardown)

EUNIT_TEST(
    "SessionEstablishedL - test ",
    "CMusUiReceiveController",
    "SessionEstablishedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_SessionEstablishedL, Teardown)

EUNIT_TEST(
    "SessionTerminatedL - test ",
    "CMusUiReceiveController",
    "SessionTerminatedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_SessionTerminatedL, Teardown)

EUNIT_TEST(
    "SessionConnectionLostL - test ",
    "CMusUiReceiveController",
    "SessionConnectionLostL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_SessionConnectionLostL, Teardown)

EUNIT_TEST(
    "SessionFailedL - test ",
    "CMusUiReceiveController",
    "SessionFailedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_SessionFailedL, Teardown)

EUNIT_TEST(
    "StreamIdleL - test ",
    "CMusUiReceiveController",
    "StreamIdleL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_StreamIdleL, Teardown)

EUNIT_TEST(
    "StreamStreamingL - test ",
    "CMusUiReceiveController",
    "StreamStreamingL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_StreamStreamingL, Teardown)

EUNIT_TEST(
    "SessionTimeChangedL - test ",
    "CMusUiReceiveController",
    "SessionTimeChangedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_SessionTimeChangedL, Teardown)

EUNIT_TEST(
    "InactivityTimeoutL - test ",
    "CMusUiReceiveController",
    "InactivityTimeoutL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_InactivityTimeoutL, Teardown)

EUNIT_TEST(
    "InvitationAcceptedLL - test ",
    "CMusUiReceiveController",
    "InvitationAcceptedLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_InvitationAcceptedLL, Teardown)

EUNIT_TEST(
    "InvitationRejectedLL - test ",
    "CMusUiReceiveController",
    "InvitationRejectedLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_InvitationRejectedLL, Teardown)

EUNIT_TEST(
    "UpdateStatusPaneLL - test ",
    "CMusUiReceiveController",
    "UpdateStatusPaneLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_UpdateStatusPaneLL, Teardown)

EUNIT_TEST(
    "InspectVideoPlayerLL - test ",
    "CMusUiReceiveController",
    "InspectVideoPlayerLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_InspectVideoPlayerLL, Teardown)

EUNIT_TEST(
    "ShowWaitDialogLL - test ",
    "CMusUiReceiveController",
    "ShowWaitDialogLL",
    "FUNCTIONALITY",
    SetupL, UT_CMusUiReceiveController_ShowWaitDialogLL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE
