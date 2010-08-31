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


#ifndef __UT_CMusPropertyMonitor_H__
#define __UT_CMusPropertyMonitor_H__


// USER INCLUDES

#include "mmuscallstateobserver.h" 

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusPropertyMonitor;

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
NONSHARABLE_CLASS( UT_CMusPropertyMonitor ): public CEUnitTestSuiteClass,
                                                    MMusCallStateObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusPropertyMonitor* NewL();
        static UT_CMusPropertyMonitor* NewLC();
                
        /**
         * Destructor
         */
        ~UT_CMusPropertyMonitor();
        

    public:     // MMusCallStateObserver

        virtual void MusCallStateChanged();


    private: // Constructors and destructors

        UT_CMusPropertyMonitor();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();
        
        void Teardown();

    private: // Test methdods
    
        void UT_CMusPropertyMonitor_RunLL();
        void UT_CMusPropertyMonitor_DoCancelL();
        void UT_CMusPropertyMonitor_RunErrorL();
        void UT_CMusPropertyMonitor_IsCallConnected();

    private:    // Data
        CMusPropertyMonitor* iMusPropertyMonitor;
        TBool iMusCallStateObserverInovked;
       

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMusPropertyMonitor_H__

// End of file
