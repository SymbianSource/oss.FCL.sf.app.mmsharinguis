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


#ifndef __UT_MUSENGMCEUTILS_H__
#define __UT_MUSENGMCEUTILS_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>


//  FORWARD DECLARATIONS
class CMceManager;
class CMusSipProfileHandler;
class CMceSession;
class CMusEngObserverStub;
class CMceDisplaySink;

#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 * Tester class for MusEngMceUtils. 
 * 
 */
NONSHARABLE_CLASS( UT_MusEngMceUtils ): 
    public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_MusEngMceUtils* NewL();
        static UT_MusEngMceUtils* NewLC();
                
        /**
         * Destructor
         */
        ~UT_MusEngMceUtils();
        
        
    private: // Constructors and destructors

        UT_MusEngMceUtils();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();
        void Setup2L();

        void Teardown();

    private: // Test methdods
    
        void UT_IsVideoInStreamL();
        void UT_IsVideoOutStreamL();
        void UT_IsAudioInStreamL();
        void UT_GetVideoOutStreamLL();
        void UT_GetVideoInStreamLL();
        void UT_GetRecordingStreamL();
        void UT_GetMediaSinkFromStreamL();
        void UT_GetMediaSinkLFromStreamL();
        void UT_GetMediaSinkFromSessionL();
        void UT_GetMediaSinkLFromSessionL();
        void UT_GetCameraLL();
        void UT_GetFileSourceLL();
        void UT_GetDisplayL();
        void UT_GetDisplayLL();
        void UT_AddDisplayLL();
        void UT_AddSpeakerLL();
        void UT_DisableStreamLL();
        void UT_GetReceivingDisplayL();
        void UT_GetReceivingDisplayLL();
        void UT_GetVfDisplayL();
        void UT_EnableInactivityTimerL();
        
    private:    // Data

        CMceManager* iManager;
        CMusSipProfileHandler* iProfileHandler;
        CMusEngObserverStub* iObserver;
        CMceSession* iEmptySession;
        CMceSession* iVideoOutSession;
        CMceSession* iVideoInSession;
        
        CMceDisplaySink* iReceivingDisplay; // Not owned

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGMCEUTILS_H__

// End of file
