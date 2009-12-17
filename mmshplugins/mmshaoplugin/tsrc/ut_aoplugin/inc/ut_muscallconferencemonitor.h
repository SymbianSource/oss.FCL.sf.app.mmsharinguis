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


#ifndef __UT_CMUSCONFERENCECALLMONITOR_H__
#define __UT_CMUSCONFERENCECALLMONITOR_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>
#include <etelmm.h>

//  FORWARD DECLARATIONS
class CMusCallConferenceMonitor;
class CMusCallMonitor;

// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files
// include this file. Directive can be removed when 
// EUnit is fixed.  
#pragma warn_illtokenpasting off

//  CLASS DEFINITION
/**
 * Tester class for MusConferenceCallMonitor. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusCallConferenceMonitor ): public CEUnitTestSuiteClass
    {
    public: // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusCallConferenceMonitor* NewL();
        static UT_CMusCallConferenceMonitor* NewLC();
                
        /**
         * Destructor
         */
        ~UT_CMusCallConferenceMonitor();

    private: // Constructors and destructors

        UT_CMusCallConferenceMonitor();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();
        void Teardown();

    private: // Test methdods
    
        void UT_CMusCallConferenceMonitor_NewLL();
        void UT_CMusCallConferenceMonitor_RunLL();
        void UT_CMusCallConferenceMonitor_DoCancelL();
        void UT_CMusCallConferenceMonitor_RunErrorL();
        void UT_CMusCallConferenceMonitor_MonitorCallLL();
        
    private:    // Data
        
        CMusCallConferenceMonitor* iConferenceCallMonitor;
        RMobilePhone iMobilePhone;
        RMobileLine  iLine;
        RPointerArray<CMusCallMonitor> iCallMonitorArray;
        

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMusCallConferenceMonitor_H__

// End of file
