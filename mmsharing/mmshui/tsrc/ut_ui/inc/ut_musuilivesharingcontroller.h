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


#ifndef __UT_MUSUILIVESHARINGCONTROLLER_H__
#define __UT_MUSUILIVESHARINGCONTROLLER_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusUiLiveSharingObserverAdapter;
class CMusUiLiveSharingController;
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
NONSHARABLE_CLASS( UT_CMusUiLiveSharingController ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusUiLiveSharingController* NewL();
        static UT_CMusUiLiveSharingController* NewLC();

        /**
         * Destructor
         */
        ~UT_CMusUiLiveSharingController();

    private: // Constructors and destructors

        UT_CMusUiLiveSharingController();
        void ConstructL();

    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methods

		//public:	// new functions:

		void UT_CMusUiLiveSharingController_StartInvitationLL();
		void UT_CMusUiLiveSharingController_AddressFoundLL();
		void UT_CMusUiLiveSharingController_InviteLL();
		void UT_CMusUiLiveSharingController_InviteL2L();
		void UT_CMusUiLiveSharingController_SetRectL();
		void UT_CMusUiLiveSharingController_CancelInvitationLL();
		void UT_CMusUiLiveSharingController_HandleCommandLL();
		void UT_CMusUiLiveSharingController_PlayingL();
		void UT_CMusUiLiveSharingController_CurrentZoomLL();
		void UT_CMusUiLiveSharingController_MinZoomLL();
		void UT_CMusUiLiveSharingController_MaxZoomLL();
		    
		//private:	// new functions:
		    
		void UT_CMusUiLiveSharingController_CenteredRectL();
		void UT_CMusUiLiveSharingController_ZoomInLL();
		void UT_CMusUiLiveSharingController_ZoomOutLL();
		void UT_CMusUiLiveSharingController_SplitLL();
		void UT_CMusUiLiveSharingController_ShowErrorDialogL();
		void UT_CMusUiLiveSharingController_DismissWaitDialogL();

		    
		//public: // From MMusEngLiveSessionObserver

		    // Empty so far
			
		//public: // from MMusEngOutSessionObserver
			
		void UT_CMusUiLiveSharingController_SessionRejectedL();                // e.g. 603 DECLINE
		void UT_CMusUiLiveSharingController_SessionBadRequestL();                   // 400 
		void UT_CMusUiLiveSharingController_SessionUnauthorizedL();                 // 401 
		void UT_CMusUiLiveSharingController_SessionPaymentRequiredL();              // 402
		void UT_CMusUiLiveSharingController_SessionRecipientNotFoundL();            // 404
		void UT_CMusUiLiveSharingController_SessionProxyAuthenticationRequiredL();  // 407
		void UT_CMusUiLiveSharingController_SessionRequestTimeOutL();               // 408
		void UT_CMusUiLiveSharingController_SessionUnsupportedMediaTypeL();         // 415
		void UT_CMusUiLiveSharingController_SessionRequestCancelledL();             // 487

		//public:	// from MMusEngSessionObserver

		void UT_CMusUiLiveSharingController_SessionEstablishedL();
		void UT_CMusUiLiveSharingController_SessionTerminatedL();
		void UT_CMusUiLiveSharingController_SessionConnectionLostL();
		void UT_CMusUiLiveSharingController_SessionFailedL();
		void UT_CMusUiLiveSharingController_StreamIdleL();
		void UT_CMusUiLiveSharingController_StreamStreamingL();	
		void UT_CMusUiLiveSharingController_SessionTimeChangedL();
		void UT_CMusUiLiveSharingController_InactivityTimeoutL();

    private:    // Data

	    CMusUiLiveSharingObserverAdapter* iLiveSharingObserverAdapter;
	    CMusUiEventObserverAdapter* iEventObserver;

        CMusUiLiveSharingController* iLiveSharingController;
        CMusUiEventController* iEventController;
        



        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGSESSION_H__

// End of file
