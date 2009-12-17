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


#ifndef __UT_CMusTsyPropertyMonitor_H__
#define __UT_CMusTsyPropertyMonitor_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusCommonObserverStub;
class CMusTsyPropertyMonitor;

// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files
// include this file. Directive can be removed when 
// EUnit is fixed.  
#pragma warn_illtokenpasting off

//  CLASS DEFINITION
/**
 * Tester class for CMusTsyPropertyMonitor. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusTsyPropertyMonitor ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusTsyPropertyMonitor* NewL();
        static UT_CMusTsyPropertyMonitor* NewLC();
                
        /**
         * Destructor
         */
        ~UT_CMusTsyPropertyMonitor();

    private: // Constructors and destructors

        UT_CMusTsyPropertyMonitor();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();
        void Setup2L();
        void Teardown();

    private: // Test methdods
    
        void UT_CMusTsyPropertyMonitor_NewLL();
        void UT_CMusTsyPropertyMonitor_RunLL();
        void UT_CMusTsyPropertyMonitor_DoCancelL();
        void UT_CMusTsyPropertyMonitor_RunErrorL(); 
        void UT_CMusTsyPropertyMonitor_AddCallMonitorLL();
        void UT_CMusTsyPropertyMonitor_RemoveCallMonitorL();
        void UT_CMusTsyPropertyMonitor_MonitorCallLL();
        void UT_CMusTsyPropertyMonitor_MonitorCallL_2L();
        void UT_CMusTsyPropertyMonitor_RemoveUnUsedCallMonitorsL();

    private:    // Data
        CMusTsyPropertyMonitor* iMusTsyPropertyMonitor;
        RMobilePhone iPhone;
        
        

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMusTsyPropertyMonitor_H__

// End of file
