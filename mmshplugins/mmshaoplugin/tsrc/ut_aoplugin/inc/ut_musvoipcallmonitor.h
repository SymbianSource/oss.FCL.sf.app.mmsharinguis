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


#ifndef __UT_CMusVoipCallMonitor_H__
#define __UT_CMusVoipCallMonitor_H__

// USER INCLUDE
#include "mmuscallstateobserver.h"

//  EXTERNAL INCLUDES
#include <ceunittestsuiteclass.h>
#include <eunitmacros.h>
#include <etel.h>

//  INTERNAL INCLUDES

// Forward declaration
class CMusVoipCallMonitor;


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
NONSHARABLE_CLASS( UT_CMusVoipCallMonitor ): public CEUnitTestSuiteClass,
                                                    MMusCallStateObserver
    {
    public:
        //Two phase construction
        static UT_CMusVoipCallMonitor* NewL();
        static UT_CMusVoipCallMonitor* NewLC();
        // Destructor                  
        ~UT_CMusVoipCallMonitor();
        
    public: // From MMusCallStateObserver
        
        virtual void MusCallStateChanged();

    private: // Constructor
        UT_CMusVoipCallMonitor();
        void ConstructL();

    private: // Test case setup and teardown
        void SetupL();
        void Teardown();

    private: // Test methdods
        void UT_CMusVoipCallMonitor_IsEqualL();
        void UT_CMusVoipCallMonitor_SetStateLL();
        void UT_CMusVoipCallMonitor_SetCallInfoLL();
        void UT_CMusVoipCallMonitor_SetCallInfoLOverflowL();
        
        void TestSetCallInfoLL( const TDesC& aSipUriFromTelephony,
                                const TDesC& aSipUriPassedToMus );
        void UT_CMusVoipCallMonitor_SetCallProviderInfoLL();
        void UT_CMusVoipCallMonitor_AddDomainFromOwnUsernameLL();

        void UT_CMusVoipCallMonitor_IsDataReadyLL();
        void UT_CMusVoipCallMonitor_ResetCallProviderNameL();
    
    private:    // Data        
        CMusVoipCallMonitor* iVoipCallMonitor;
        RTelServer iTelServer;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMusVoipCallMonitor_H__

// End of file
