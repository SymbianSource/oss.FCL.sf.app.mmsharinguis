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


#ifndef __UT_CMusCallMonitorBase_H__
#define __UT_CMusCallMonitorBase_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>
#include "etelmm.h"
#include "muscallmonitorbase.h"
#include "mussesseioninformationapi.h"
#include "mmustsypropertyobserver.h"

// Forward declaration

// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files
// include this file. Directive can be removed when 
// EUnit is fixed.  
#pragma warn_illtokenpasting off

class CMusCallMonitorMock : public CMusCallMonitorBase
							
    {
public:
    
    CMusCallMonitorMock( const RMobileCall& aCall,
    					MMusTsyPropertyObserver& aObserver );
    ~CMusCallMonitorMock();
    
    private:

    void RunL();
    void DoCancel();
    };



//  CLASS DEFINITION
/**
 * Tester class for CMusCallMonitorBase. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusCallMonitorBase ): public CEUnitTestSuiteClass,
											MMusTsyPropertyObserver
    {
    public:
        //Two phase construction
        static UT_CMusCallMonitorBase* NewL();
        static UT_CMusCallMonitorBase* NewLC();
        // Destructor                  
        ~UT_CMusCallMonitorBase();
        
    public:    // from MMusTsyPropertyObserver.
    
    /**
     * Notify call state changed
     * @param aVal The call status
     * @param aCallName The name of the call
     */
    virtual void NotifyCallStateChanged( NMusSessionInformationApi::TMusCallEvent aVal,
    									 TName& aCallName );
    

    private: // Constructor
        UT_CMusCallMonitorBase();
        void ConstructL();

    private: // Test case setup and teardown
        void SetupL();
        void Setup2L();
        
        void Teardown();

    private: // Test methdods
        void UT_SetStateLL();
        void UT_SetCallInfoLL();
        void UT_RunErrorL();
        
        void UT_SetClirSettingL();
        void UT_SetTerminatingPrivacyL();
        void UT_RemovePrefixL();
        
    
    private:    // Data        
        CMusCallMonitorBase* iCallMonitorBase;
        RMobileCall iCall;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMusCallMonitorBase_H__

// End of file
