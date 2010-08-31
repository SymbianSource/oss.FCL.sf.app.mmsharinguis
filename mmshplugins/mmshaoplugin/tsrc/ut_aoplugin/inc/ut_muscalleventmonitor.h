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


#ifndef __UT_CMusCallEventMonitor_H__
#define __UT_CMusCallEventMonitor_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>
#include "etelmm.h"
#include "mussesseioninformationapi.h"
#include "mmustsypropertyobserver.h"
#include "mmuscallstateobserver.h"

//  FORWARD DECLARATIONS
class CMusCallEventMonitor;

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
NONSHARABLE_CLASS( UT_CMusCallEventMonitor ): public CEUnitTestSuiteClass,
											   MMusTsyPropertyObserver,
											   MMusCallStateObserver
											   
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusCallEventMonitor* NewL();
        static UT_CMusCallEventMonitor* NewLC();
                
        /**
         * Destructor
         */
        ~UT_CMusCallEventMonitor();
        
    public:    // from MMusTsyPropertyObserver.
    
    /**
     * Notify call state changed
     * @param aVal The call status
     * @param aCallName The name of the call
     */
    virtual void NotifyCallStateChanged( NMusSessionInformationApi::TMusCallEvent aVal,
    									 TName& aCallName );
    
    public:    // from MMusCallStateObserver.

        virtual void MusCallStateChanged();
    

    private: // Constructors and destructors

        UT_CMusCallEventMonitor();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();
        void Teardown();

    private: // Test methdods
    
        void UT_CMusCallEventMonitor_NewLL();
        void UT_CMusCallEventMonitor_RunLL();
        void UT_CMusCallEventMonitor_DoCancelL();
        void UT_CMusCallEventMonitor_RunErrorL();        
    private:    // Data
        CMusCallEventMonitor* iCallEventMonitor;
        RMobileCall iCall;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMusCallEventMonitor_H__

// End of file
