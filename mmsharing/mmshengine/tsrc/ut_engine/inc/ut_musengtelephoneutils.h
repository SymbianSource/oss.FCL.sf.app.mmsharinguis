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


#ifndef __UT_MUSENGTELEPHONEUTILS_H__
#define __UT_MUSENGTELEPHONEUTILS_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusEngTelephoneUtils;
class CMusEngObserverStub;

//  CLASS DEFINITION
/**
 * Tester class for CMusEngTelephoneUtils. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusEngTelephoneUtils ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngTelephoneUtils* NewL();
        static UT_CMusEngTelephoneUtils* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngTelephoneUtils();

    private: // Constructors and destructors

        UT_CMusEngTelephoneUtils();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methods
        
        void UT_NewLL();
        void UT_AudioRoutingCanBeChangedL();
        void UT_AudioOutputIsBTL();
        void UT_LoudspeakerLL();
        void UT_IsLoudSpeakerEnabledL();
        void UT_GetVolumeLL();
        void UT_SetVolumeLL();
        void UT_AvailableOutputsChangedL();
        void UT_OutputChangedL();
        void UT_SetOutputCompleteL();
        void UT_ValidateVolumeL();
        void UT_DestructorL();
        void UT_UpdateCurrentVolumeL();
        void UT_HandleNotifyGenericL();

    private:    // Data

        CMusEngTelephoneUtils* iTelephoneUtils;
        CMusEngObserverStub* iObserver;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGTELEPHONEUTILS_H__

// End of file
