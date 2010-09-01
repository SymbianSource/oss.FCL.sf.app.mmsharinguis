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


#ifndef __UT_MUSUIRECEIVECONTROLLER_H__
#define __UT_MUSUIRECEIVECONTROLLER_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusUiReceiveObserverAdapter;
class CMusUiReceiveController;
class CMusUiEventObserverAdapter;
class CMusUiEventController;



//  CLASS DEFINITION
/**
 * Tester class for CMusEngSession.
 * Since CMusEngSession is abstract, it is tested via object of its'
 * concrete descendant class CMusEngLiveSession. Only concrete
 * implementations are tested in this tester class.
 *
 */
NONSHARABLE_CLASS( UT_CMusUiReceiveController ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusUiReceiveController* NewL();
        static UT_CMusUiReceiveController* NewLC();

        /**
         * Destructor
         */
        ~UT_CMusUiReceiveController();

    private: // Constructors and destructors

        UT_CMusUiReceiveController();
        void ConstructL();

    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methods

	    void UT_CMusUiReceiveController_HandleCommandLL();
	    void UT_CMusUiReceiveController_SizeChangedL();
	    void UT_CMusUiReceiveController_ShowErrorDialogLL();
	    void UT_CMusUiReceiveController_HandleIncomingSessionLL();
	    void UT_CMusUiReceiveController_CancelInvitationLL();

	public: // from CMusUiEventController

	    void UT_CMusUiReceiveController_ContinueInvitationLL();
	    void UT_CMusUiReceiveController_HandleAsyncEventLL();
	    void UT_CMusUiReceiveController_ConnectionEstablishedL();
	    void UT_CMusUiReceiveController_PlayLL();
	    void UT_CMusUiReceiveController_PauseLL();
	    void UT_CMusUiReceiveController_EnableDisplayLL();
	    void UT_CMusUiReceiveController_ShowNaviPaneIconLL();
	    void UT_CMusUiReceiveController_ChangeOrientationLL();
	    void UT_CMusUiReceiveController_CenteredRectL();
	    void UT_CMusUiReceiveController_ManualAddressTypedL();
	    void UT_CMusUiReceiveController_ShowInformationDialogLL();
	    void UT_CMusUiReceiveController_ShutdownLL();
	    
	public: // from MMusEngReceiveSessionObserver

	    void UT_CMusUiReceiveController_IncomingSessionL();
	    void UT_CMusUiReceiveController_StreamBufferingL();

	public:	// from MMusEngSessionObserver

	    void UT_CMusUiReceiveController_SessionEstablishedL();
	    void UT_CMusUiReceiveController_SessionTerminatedL();
	    void UT_CMusUiReceiveController_SessionConnectionLostL();
	    void UT_CMusUiReceiveController_SessionFailedL();
	    void UT_CMusUiReceiveController_StreamIdleL();
	    void UT_CMusUiReceiveController_StreamStreamingL();	
	    void UT_CMusUiReceiveController_SessionTimeChangedL();
	    void UT_CMusUiReceiveController_InactivityTimeoutL();
		
	public: // from MMusUiInvitationQueryObserver

	    void UT_CMusUiReceiveController_InvitationAcceptedLL();
	    void UT_CMusUiReceiveController_InvitationRejectedLL();
		
	private:	// new functions:

	    void UT_CMusUiReceiveController_UpdateStatusPaneLL();
	    void UT_CMusUiReceiveController_InspectVideoPlayerLL();
	    void UT_CMusUiReceiveController_ShowWaitDialogLL();
    
	    



    private:    // Data

	    CMusUiReceiveObserverAdapter* iReceiveObserverAdapter;
	    CMusUiEventObserverAdapter* iEventObserver;

        CMusUiReceiveController* iReceiveController;
        CMusUiEventController* iEventController;


        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGSESSION_H__

// End of file
