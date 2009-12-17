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


#ifndef __UT_MUSUIEVENTCONTROLLER_H__
#define __UT_MUSUIEVENTCONTROLLER_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>


//  FORWARD DECLARATIONS
//class CMusEngLiveSession;
//class CMusEngObserverStub;
class CMusUiEventController;
class CMusUiLiveSharingController;
class CMusUiEventObserverAdapter;
class CMusUiLiveSharingObserverAdapter;

//  CLASS DEFINITION
/**
 * Tester class for CMusEngSession.
 * Since CMusEngSession is abstract, it is tested via object of its'
 * concrete descendant class CMusEngLiveSession. Only concrete
 * implementations are tested in this tester class.
 *
 */
NONSHARABLE_CLASS( UT_CMusUiEventController ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusUiEventController* NewL();
        static UT_CMusUiEventController* NewLC();

        /**
         * Destructor
         */
        ~UT_CMusUiEventController();

    private: // Constructors and destructors

        UT_CMusUiEventController();
        void ConstructL();

    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methods


    public: // From MMusUiPropertyObserver
    
        void UT_CMusUiEventController_ResourcePropertyChangedL();

        void UT_CMusUiEventController_SessionPropertyChangedL();

        void UT_CMusUiEventController_SessionPropertyChanged2L();

        void UT_CMusUiEventController_HandleErrorL();


    public: // From MMusUiBitmapDecoderObserver

        void UT_CMusUiEventController_UpdateBitmapL();


    public: // New functions

        void UT_CMusUiEventController_SetEventObserverL();

        void UT_CMusUiEventController_HandleForegroundEventLL();

        void UT_CMusUiEventController_SetCameraStatusL();

        void UT_CMusUiEventController_SetKeypadStatusL();

        void UT_CMusUiEventController_SetVideoPlayerStatusL();

        void UT_CMusUiEventController_SetCameraAvailabilityInProgressL();

        void UT_CMusUiEventController_CameraAvailableL();

        void UT_CMusUiEventController_KeypadAvailableL();

        void UT_CMusUiEventController_VideoPlayerAvailableL();

        void UT_CMusUiEventController_CameraAvailibilityInProgressL();

        void UT_CMusUiEventController_MusRemoteSipAddressValueL();

        void UT_CMusUiEventController_MusProposalAddressValueL();

        void UT_CMusUiEventController_MusContactNameL();

        void UT_CMusUiEventController_MusContactBitmapL();

        void UT_CMusUiEventController_MusVideoCodecValueL();

        void UT_CMusUiEventController_MusTelNumberValueL();

        void UT_CMusUiEventController_SipProfileIdL();

        void UT_CMusUiEventController_SipRegistrationPendingL();

        void UT_CMusUiEventController_SessionTimeFormattedL();

        void UT_CMusUiEventController_SaveContactL();

        void UT_CMusUiEventController_HandleExitLL();

        void UT_CMusUiEventController_InspectKeypadLL();

        void UT_CMusUiEventController_UpdateContactLL();

        void UT_CMusUiEventController_UpdateContactNameL();

        void UT_CMusUiEventController_UpdateContactPictureLL();

        void UT_CMusUiEventController_HandleChangedStatusLL();


    private:    // Data

        
	    CMusUiLiveSharingObserverAdapter* iLiveSharingObserverAdapter;
	    CMusUiEventObserverAdapter* iEventObserver;

        CMusUiLiveSharingController* iLiveSharingController;
        CMusUiEventController* iEventController;
        

        HBufC* _iReturnValue;
        HBufC* iVideoFolder;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGSESSION_H__

// End of file
