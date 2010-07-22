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


#ifndef __UT_MUSENGCAMERAHANDLER_H__
#define __UT_MUSENGCAMERAHANDLER_H__


//  EXTERNAL INCLUDES
#include <ceunittestsuiteclass.h>
#include <eunitmacros.h>

#include "musengcamerahandler.h"

//  FORWARD DECLARATIONS
class CMceManager;
class CMusSipProfileHandler;
class CMceSession;
class CMusEngObserverStub;
class CMceDisplaySink;


//  CLASS DEFINITION
/**
 * Tester class for CMusEngLiveSession. 
 * Tests only implementations that are not tested in ancestor classes.
 * 
 */
NONSHARABLE_CLASS( UT_TMusEngCameraHandler ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TMusEngCameraHandler* NewL();
        static UT_TMusEngCameraHandler* NewLC();
        
        /**
         * Destructor
         */
        ~UT_TMusEngCameraHandler();

    private: // Constructors and destructors

        UT_TMusEngCameraHandler();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();
        void Teardown();

    private: // Test methdods
    
        void UT_LcCameraCountL();
        void UT_ToggleLcCameraL();
        void UT_SetLcBrightnessL();
        void UT_LcBrightnessL();
        void UT_MaxLcBrightnessL();
        void UT_MinLcBrightnessL();
        void UT_IncreaseLcBrightnessL();
        void UT_DecreaseLcBrightnessL();        
        void UT_MinLcZoomL();
        void UT_MaxLcZoomL();
        void UT_LcZoomValueL();
        void UT_SetLcZoomValueL();
        void UT_LcZoomInL();
        void UT_LcZoomOutL();
        void UT_PlayL();
        void UT_PauseL();
        void UT_IsPlayingL();
        void UT_InitializeZoomStepSizeL();
        void UT_ChangeCameraL();
        void UT_ChangeActiveCameraL();
        
    private:    // Data

        TMusEngCameraHandler iCameraHandler;
        CMceManager* iManager;
        CMusSipProfileHandler* iProfileHandler;
        CMusEngObserverStub* iObserver;
        CMceSession* iSession;

        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_MUSENGLIVESESSION_H__

// End of file
