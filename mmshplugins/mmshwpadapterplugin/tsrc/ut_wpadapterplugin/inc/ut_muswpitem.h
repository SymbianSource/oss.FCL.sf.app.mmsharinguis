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


#ifndef UT_MUSWPITEM_H__
#define UT_MUSWPITEM_H__

// Next row is to disable warning emerging from EUnit code.
// Directive can be removed when EUnit is fixed.  
#pragma warn_illtokenpasting off

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusWpItem;

//  CLASS DEFINITION
/**
 * Tester class for CMusWpItem. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusWpItem ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusWpItem* NewL();
        static UT_CMusWpItem* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusWpItem();

    private: // Constructors and destructors

        UT_CMusWpItem();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methods
        
        void UT_CMusWpItem_NewLL();
        void UT_CMusWpItem_ProviderIdL();
        void UT_CMusWpItem_SetProviderIdLL();
        void UT_CMusWpItem_NameL();
        void UT_CMusWpItem_SetNameLL();
        void UT_CMusWpItem_ToAppRefL();
        void UT_CMusWpItem_SetToAppRefLL();
        void UT_CMusWpItem_SetEdgeDtmSupportL();
        void UT_CMusWpItem_SetForceInternetSignalingL();
        void UT_CMusWpItem_SetCapabilityQueryL();
        void UT_CMusWpItem_SetActivationConditionL();
        void UT_CMusWpItem_SaveLL();

    private:    // Data

        CMusWpItem* iWpItem;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  UT_MUSWPITEM_H__

// End of file
