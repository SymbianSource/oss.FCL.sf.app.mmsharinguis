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

#ifndef UT_MUSENGSESSIONMANAGER_H
#define UT_MUSENGSESSIONMANAGER_H

//  EXTERNAL INCLUDES
#include <ceunittestsuiteclass.h>
#include <eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusEngSessionManager;
class CLcUiProviderStub;


//  CLASS DEFINITION
/**
 * Tester class for CMusEngSessionManager.
 * 
 */
NONSHARABLE_CLASS( UT_CMusEngSessionManager ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngSessionManager* NewL();
        static UT_CMusEngSessionManager* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngSessionManager();

    private: // Constructors and destructors

        UT_CMusEngSessionManager();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methdods
    
        void UT_NewL();
        void UT_SessionL();
        void UT_IsFeatureSupportedL();
        void UT_CreateMusSessionL();
        void UT_PropertyChangedL();
        
    private:    // Data

        CMusEngSessionManager* iSessionManager;
        CLcUiProviderStub* iLcUiProvider;

        EUNIT_DECLARE_TEST_TABLE;
    };

#endif // UT_MUSENGSESSIONMANAGER_H

// End of file
