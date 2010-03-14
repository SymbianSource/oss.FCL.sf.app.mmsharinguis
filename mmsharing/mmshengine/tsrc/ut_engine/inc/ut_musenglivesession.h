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


#ifndef __UT_MUSENGLIVESESSION_H__
#define __UT_MUSENGLIVESESSION_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusEngLiveSession;
class CMusEngObserverStub;


//  CLASS DEFINITION
/**
 * Tester class for CMusEngLiveSession. 
 * Tests only implementations that are not tested in ancestor classes.
 * 
 */
NONSHARABLE_CLASS( UT_CMusEngLiveSession ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngLiveSession* NewL();
        static UT_CMusEngLiveSession* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngLiveSession();

    private: // Constructors and destructors

        UT_CMusEngLiveSession();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methdods
    
        void UT_NewLL();
        void UT_CurrentZoomLL();
        void UT_CMusEngLiveSession_SetZoomLL();
        void UT_MaxZoomLL();
        void UT_ZoomInLL();
        void UT_ZoomOutLL();
        void UT_ZoomDefaultLL();
        void UT_RecordL_and_IsRecordingLL();
        void UT_PlayLL();
        void UT_PauseLL();
        void UT_IsPlayingLL();
        void UT_CompleteSessionStructureLL();
        void UT_HandleSessionStateChangedL();
        void UT_AdjustVideoCodecLL();
        void UT_AdjustAudioCodecLL();
        void UT_DoCodecSelectionLL();
        void UT_StreamStateChangedL();
        void UT_StreamStateChangedWithSourceL();
        void UT_StreamStateChangedWithSinkL();
        void UT_SetBrightnessLL();
        void UT_CurrentBrightnessLL();
        void UT_MaxBrightnessLL();
        void UT_MinBrightnessLL();
        void UT_IncreaseBrightnessLL();
        void UT_DecreaseBrightnessLL();
        void UT_BrightnessDefaultLL();
        void UT_BrightnessAutoLL();
        void UT_SetEncodingDeviceLL();
        void UT_SetCodecConfigKeyLL();
        void UT_ReadCodecConfigKeyLL();
        void UT_StoreEncoderConfigInfoLL();
        void UT_InitializeZoomStepSizeL();
        
    private:    // Data

        CMusEngLiveSession* iLiveSession;
        CMusEngLiveSession* iRecordedLiveSession;
        CMusEngObserverStub* iObserver;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGLIVESESSION_H__

// End of file
