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


#ifndef __UT_CMusCallStatusMonitor_H__
#define __UT_CMusCallStatusMonitor_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>
#include "etelmm.h"
#include "mussesseioninformationapi.h"
#include "mmustsypropertyobserver.h"

//  FORWARD DECLARATIONS
class CMusCommonObserverStub;
class CMusCallStatusMonitor;

// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files
// include this file. Directive can be removed when 
// EUnit is fixed.  
#pragma warn_illtokenpasting off

//  CLASS DEFINITION
/**
 * Tester class for MusCallMonitor. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusCallStatusMonitor ): public CEUnitTestSuiteClass,
													 MMusTsyPropertyObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusCallStatusMonitor* NewL();
        static UT_CMusCallStatusMonitor* NewLC();
                
        /**
         * Destructor
         */
        ~UT_CMusCallStatusMonitor();
        
    public:    // from MMusTsyPropertyObserver.
    
    /**
     * Notify call state changed
     * @param aVal The call status
     * @param aCallName The name of the call
     */
    virtual void NotifyCallStateChanged( NMusSessionInformationApi::TMusCallEvent aVal,
    									 TName& aCallName );

    private: // Constructors and destructors

        UT_CMusCallStatusMonitor();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();
        void Teardown();

    private: // Test methdods
    
        void UT_CMusCallStatusMonitor_NewLL();
        void UT_CMusCallStatusMonitor_CheckStateLL();
        void UT_CMusCallStatusMonitor_RunLL();
        void UT_CMusCallStatusMonitor_DoCancelL();
        void UT_CMusCallStatusMonitor_RunErrorL();        
    private:    // Data
        CMusCallStatusMonitor* iCallStatusMonitor;
        RMobileCall iCall;
        

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMusCallStatusMonitor_H__

// End of file
