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


#ifndef __UT_MUSENGSESSIONDURATIONTIMER_H__
#define __UT_MUSENGSESSIONDURATIONTIMER_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusEngSessionDurationTimer;
class CMusEngObserverStub;


//  CLASS DEFINITION
/**
 * Tester class for CMusEngSessionDurationTimer. 
 * 
 * 
 */
NONSHARABLE_CLASS( UT_CMusEngSessionDurationTimer ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngSessionDurationTimer* NewL();
        static UT_CMusEngSessionDurationTimer* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngSessionDurationTimer();

    private: // Constructors and destructors

        UT_CMusEngSessionDurationTimer();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methods
        
        void UT_CMusEngSessionDurationTimer_StartL();
        void UT_CMusEngSessionDurationTimer_RunLL();
        void UT_CMusEngSessionDurationTimer_DoCancelL();

    private:    // Data

        CMusEngSessionDurationTimer* iDurationTimer;
        CMusEngObserverStub* iObserver;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGSESSIONDURATIONTIMER_H__

// End of file
