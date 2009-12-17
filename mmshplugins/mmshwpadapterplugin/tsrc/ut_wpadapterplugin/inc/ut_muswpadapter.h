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


#ifndef UT_MUSWPADAPTER_H__
#define UT_MUSWPADAPTER_H__

// Next row is to disable warning emerging from EUnit code.
// Directive can be removed when EUnit is fixed.  
#pragma warn_illtokenpasting off

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusWpAdapter;

//  CLASS DEFINITION
/**
 * Tester class for CMusWpAdapter. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusWpAdapter ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusWpAdapter* NewL();
        static UT_CMusWpAdapter* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusWpAdapter();

    private: // Constructors and destructors

        UT_CMusWpAdapter();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methods
        
        void UT_CMusWpAdapter_NewLL();
        void UT_CMusWpAdapter_ItemCountL();
        void UT_CMusWpAdapter_SummaryTitleL();
        void UT_CMusWpAdapter_SummaryTextL();
        void UT_CMusWpAdapter_SaveLL();
        void UT_CMusWpAdapter_CanSetAsDefaultL();
        void UT_CMusWpAdapter_SetAsDefaultLL();
        void UT_CMusWpAdapter_DetailsLL();
        void UT_CMusWpAdapter_SettingsSavedLL();
        void UT_CMusWpAdapter_SavingFinalizedLL();
        void UT_CMusWpAdapter_VisitCharacteristicLL();
        void UT_CMusWpAdapter_VisitParameterLL();
        void UT_CMusWpAdapter_VisitLinkLL();

    private:    // Data

        CMusWpAdapter* iWpAdapter;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  UT_MUSWPADAPTER_H__

// End of file
