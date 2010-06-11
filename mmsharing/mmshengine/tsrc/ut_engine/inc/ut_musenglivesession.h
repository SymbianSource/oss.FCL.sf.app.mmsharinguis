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
class CLcSessionObserverStub;
class CLcUiProviderStub;
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
        void UT_CompleteSessionStructureLL();
        void UT_HandleSessionStateChangedL();
        void UT_AdjustVideoCodecLL();
        void UT_AdjustAudioCodecLL();
        void UT_DoCodecSelectionLL();
        void UT_StreamStateChangedL();
        void UT_StreamStateChangedWithSourceL();
        void UT_StreamStateChangedWithSinkL();
        void UT_EstablishLcSessionL();
        void UT_LocalVideoPlayerL();  
                
    private:    // Data

        CMusEngLiveSession* iLiveSession;
        CMusEngLiveSession* iRecordedLiveSession;
        CLcSessionObserverStub* iLcSessionObserver;
        CLcUiProviderStub* iLcUiProvider;
        CMusEngObserverStub* iAudioRoutingObserver;

        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_MUSENGLIVESESSION_H__

// End of file
