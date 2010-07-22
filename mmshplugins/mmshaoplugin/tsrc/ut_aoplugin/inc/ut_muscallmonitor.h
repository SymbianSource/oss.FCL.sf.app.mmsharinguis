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


#ifndef __UT_CMusCallMonitor_H__
#define __UT_CMusCallMonitor_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>
#include "etelmm.h"
#include "mussesseioninformationapi.h"
#include "mmustsypropertyobserver.h"
#include "mmuscallstateobserver.h"

// Forward declaration
class CMusCallMonitor;


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
NONSHARABLE_CLASS( UT_CMusCallMonitor ): public CEUnitTestSuiteClass,
											MMusTsyPropertyObserver, 
											 MMusCallStateObserver
    {
    public:
        //Two phase construction
        static UT_CMusCallMonitor* NewL();
        static UT_CMusCallMonitor* NewLC();
        // Destructor                  
        ~UT_CMusCallMonitor();
        
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
    
    
    private: // Constructor
        UT_CMusCallMonitor();
        void ConstructL();

    private: // Test case setup and teardown
        void SetupL();
        void Teardown();

    private: // Test methdods
        void UT_CMusCallMonitor_StartMonitorLL();
        void UT_CMusCallMonitor_StopMonitorLL();
        void UT_CMusCallMonitor_IsEqualL();     
        void UT_CMusCallMonitor_IsMonitoredL();  
    
    private:    // Data        
        CMusCallMonitor* iCallMonitor;
        RMobileLine iLine; 

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMusCallMonitor_H__

// End of file
