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


#ifndef __UT_MUSENGSIPPROFILEHANDLER_H__
#define __UT_MUSENGSIPPROFILEHANDLER_H__


// INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusSipProfileHandler;
class CMusEngObserverStub;


//  CLASS DEFINITION
/**
 * Tester class for CMusSipProfileHandler. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusEngSipProfileHandler ): 
    public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngSipProfileHandler* NewL();
        static UT_CMusEngSipProfileHandler* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngSipProfileHandler();
        
    private: // Constructors and destructors

        UT_CMusEngSipProfileHandler();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methdods
    
        void UT_CreateSipProfileLL();
        void UT_ProfileIdL();
        void UT_AlrEventL();
        void UT_NullTestsL();
        void UT_UserFromProfileLCL();
        void UT_IsRegisteredL();
        void UT_ProfileRegistryEventOccurredL();
        
    private:    // Data

        CMusSipProfileHandler* iProfileHandler;
        CMusEngObserverStub* iObserver;
        
        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_MUSENGSIPPROFILEHANDLER_H__

// End of file
