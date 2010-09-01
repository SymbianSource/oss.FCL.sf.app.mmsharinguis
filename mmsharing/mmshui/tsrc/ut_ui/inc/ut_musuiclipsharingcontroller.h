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


#ifndef __UT_MUSUICLIPSHARINGCONTROLLER_H__
#define __UT_MUSUICLIPSHARINGCONTROLLER_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusUiClipSharingObserverAdapter;
class CMusUiClipSharingController;
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
NONSHARABLE_CLASS( UT_CMusUiClipSharingController ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusUiClipSharingController* NewL();
        static UT_CMusUiClipSharingController* NewLC();

        /**
         * Destructor
         */
        ~UT_CMusUiClipSharingController();

    private: // Constructors and destructors

        UT_CMusUiClipSharingController();
        void ConstructL();

    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methods


        void UT_CMusUiClipSharingController_StartInvitationLL();
        void UT_CMusUiClipSharingController_AddressFoundLL();
        void UT_CMusUiClipSharingController_InviteLL();
        void UT_CMusUiClipSharingController_InviteL2L();
        void UT_CMusUiClipSharingController_SetRectL();
        void UT_CMusUiClipSharingController_CancelInvitationLL();
        void UT_CMusUiClipSharingController_HandleCommandLL();
        void UT_CMusUiClipSharingController_PlayingL();
        void UT_CMusUiClipSharingController_SetFileNameLL();
        void UT_CMusUiClipSharingController_ClipEndedL();
        void UT_CMusUiClipSharingController_ZoomInLL();
        void UT_CMusUiClipSharingController_ZoomOutLL();
        void UT_CMusUiClipSharingController_SplitLL();
        void UT_CMusUiClipSharingController_ShowErrorDialogL();
        void UT_CMusUiClipSharingController_DismissWaitDialogL();
        void UT_CMusUiClipSharingController_EndOfClipL();
        void UT_CMusUiClipSharingController_SessionRejectedL();
        void UT_CMusUiClipSharingController_SessionBadRequestL();
        void UT_CMusUiClipSharingController_SessionUnauthorizedL();
        void UT_CMusUiClipSharingController_SessionPaymentRequiredL();
        void UT_CMusUiClipSharingController_SessionRecipientNotFoundL();
        void UT_CMusUiClipSharingController_SessionProxyAuthenticationRequiredL();
        void UT_CMusUiClipSharingController_SessionRequestTimeOutL();
        void UT_CMusUiClipSharingController_SessionUnsupportedMediaTypeL();
        void UT_CMusUiClipSharingController_SessionRequestCancelledL();
        void UT_CMusUiClipSharingController_SessionEstablishedL();
        void UT_CMusUiClipSharingController_SessionTerminatedL();
        void UT_CMusUiClipSharingController_SessionConnectionLostL();
        void UT_CMusUiClipSharingController_SessionFailedL();
        void UT_CMusUiClipSharingController_StreamIdleL();
        void UT_CMusUiClipSharingController_StreamStreamingL();
        void UT_CMusUiClipSharingController_SessionTimeChangedL();
        void UT_CMusUiClipSharingController_InactivityTimeoutL();


    private:    // Data

	    CMusUiClipSharingObserverAdapter* iClipSharingObserverAdapter;
	    CMusUiEventObserverAdapter* iEventObserver;

        CMusUiClipSharingController* iClipSharingController;
        CMusUiEventController* iEventController;


        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGSESSION_H__

// End of file
