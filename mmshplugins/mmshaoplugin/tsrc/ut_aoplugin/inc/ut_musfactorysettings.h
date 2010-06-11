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


#ifndef __UT_CMusFactorySettings_H__
#define __UT_CMusFactorySettings_H__


//  EXTERNAL INCLUDES
#include <ceunittestsuiteclass.h>
#include <eunitmacros.h>


// Forward declaration
class MusFactorySettings;
class CRepository;

// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files
// include this file. Directive can be removed when 
// EUnit is fixed.  
#pragma warn_illtokenpasting off

//  CLASS DEFINITION
/**
 * Tester class for CMusCallMonitorBase. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusFactorySettings ): public CEUnitTestSuiteClass
    {
    public:
        //Two phase construction
        static UT_CMusFactorySettings* NewL();
        static UT_CMusFactorySettings* NewLC();
        // Destructor                  
        ~UT_CMusFactorySettings();

    private: // Constructor
        UT_CMusFactorySettings();
        void ConstructL();

    private: // Test case setup and teardown
        void SetupL();
        void Teardown();

    private: // Test methdods
        void UT_CMusFactorySettings_IsSupportedLL();
        
    private:    // Data
        CRepository* iMusCentralRepository ; 
        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMusFactorySettings_H__

// End of file
