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


#ifndef __UT_MUSENGCLIPSESSION_H__
#define __UT_MUSENGCLIPSESSION_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusEngClipSession;
class CMusEngObserverStub;

// CONSTANTS
_LIT8( KMusSipUri, "sip:user@domain.com" );

//  CLASS DEFINITION
/**
 * Tester class for CMusEngClipSession. 
 * Tests only implementations that are not tested in ancestor classes.
 * 
 */
NONSHARABLE_CLASS( UT_CMusEngClipSession ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngClipSession* NewL();
        static UT_CMusEngClipSession* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngClipSession();

    private: // Constructors and destructors

        UT_CMusEngClipSession();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();
        void Setup2L();

        void Teardown();

    private: // Test methdods
    
        void UT_NewLL();
        void UT_SetClipLL();
        void UT_FastForwardLL();
        void UT_FastRewindLL();
        void UT_PositionLL();
        void UT_DurationLL();
        void UT_SetPositionLL();
        void UT_TranscodeLToAvcL();
        void UT_TranscodeLToH263L();
        void UT_CancelTranscodeLL();
        void UT_PlayLL();
        void UT_PauseLL();
        void UT_IsPlayingLL();
        void UT_CompleteSessionStructureLL();
        void UT_StreamStateChangedL();
        void UT_StreamStateChangedWithSourceL();
        void UT_StreamStateChangedWithSinkL();
        void UT_AddAmrCodecLL();
        void UT_AddVideoCodecLL();
        void UT_HasClipEndedL();
        void UT_ConstructAudioStructureLL();
        void UT_ConstructAudioStructureL_OperatorVariantL();
        void UT_EstablishSessionLL();
        void UT_IsRewindFromEndL();
        
    private:    // Data

        CMusEngClipSession* iClipSession;
        CMusEngObserverStub* iObserver;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGCLIPSESSION_H__

// End of file
