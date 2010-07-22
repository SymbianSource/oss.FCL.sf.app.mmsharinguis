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


#ifndef __UT_MUSENGTWOWAYSESSION_H__
#define __UT_MUSENGTWOWAYSESSION_H__


//  EXTERNAL INCLUDES
#include <ceunittestsuiteclass.h>
#include <eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusEngTwoWaySession;
class CLcSessionObserverStub;
class CLcUiProviderStub;


//  CLASS DEFINITION
/**
 * Tester class for CMusEngLiveSession. 
 * Tests only implementations that are not tested in ancestor classes.
 * 
 */
NONSHARABLE_CLASS( UT_CMusEngTwoWaySession ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngTwoWaySession* NewL();
        static UT_CMusEngTwoWaySession* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngTwoWaySession();

    private: // Constructors and destructors

        UT_CMusEngTwoWaySession();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methdods
    
        void UT_NewLL();
        void UT_EstablishLcSessionLL();
        void UT_EnableDisplayLL();
        void UT_SetOrientationLL();
        void UT_SetLocalRectLL();
        void UT_SetRectsLL();
        void UT_CompleteSessionStructureLL();
        void UT_HandleSessionStateChangedL();
        void UT_AdjustVideoCodecLL();
        void UT_AdjustAudioCodecLL();
        void UT_DoCodecSelectionLL();
        void UT_StreamStateChangedL();
        void UT_StreamStateChangedWithSourceL();
        void UT_SetEncodingDeviceLL();
        void UT_SetCodecConfigKeyLL();
        void UT_ReadCodecConfigKeyLL();
        void UT_StoreEncoderConfigInfoLL();
        void UT_RemoteVideoPlayerL();
        void UT_EnableLcFileL();
        void UT_InactivityTimeoutL();
        
    private:    // Data

        CMusEngTwoWaySession* iLiveSession;
        CLcSessionObserverStub* iLcSessionObserver;
        CLcUiProviderStub* iLcUiProvider;

        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_MUSENGTWOWAYSESSION_H__

// End of file
