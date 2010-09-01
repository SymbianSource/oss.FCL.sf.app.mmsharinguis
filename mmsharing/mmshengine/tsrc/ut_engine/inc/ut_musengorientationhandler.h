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


#ifndef __UT_MUSENGORIENTATIONHANDLER_H__
#define __UT_MUSENGORIENTATIONHANDLER_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusEngLiveSession;
class CMusEngObserverStub;
class CMusEngOrientationHandler;


//  CLASS DEFINITION
/**
 * Tester class for CMusEngLiveSession. 
 * Tests only implementations that are not tested in ancestor classes.
 * 
 */
NONSHARABLE_CLASS( UT_CMusEngOrientationHandler ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngOrientationHandler* NewL();
        static UT_CMusEngOrientationHandler* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngOrientationHandler();

    private: // Constructors and destructors

        UT_CMusEngOrientationHandler();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methdods
    
        void UT_NewLL();
        void UT_RefreshOrientationL();

    private:    // Data

        CMusEngLiveSession* iLiveSession;
        CMusEngObserverStub* iObserver;
        CMusEngOrientationHandler* iHandler;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGORIENTATIONHANDLER_H__

// End of file
