/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __UT_CMUSAVACONNECTIONMONITOR_H__
#define __UT_CMUSAVACONNECTIONMONITOR_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

#include "rconnmon.h"
//  FORWARD DECLARATIONS
class CSipSseTestTls;
class CMusAvaConnectionMonitor;
class CMusAvaSharedObject;
        
//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaConnectionMonitor
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaConnectionMonitor* NewL();
        static UT_CMusAvaConnectionMonitor* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaConnectionMonitor();

    private:    // Constructors and destructors

        UT_CMusAvaConnectionMonitor();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaConnectionMonitor_NewLL();   

        void UT_CMusAvaConnectionMonitor_EventLL();
        
        void UT_CMusAvaConnectionMonitor_AddAdapterLL();
        
        void UT_CMusAvaConnectionMonitor_RemoveAdapterL();
      
        void UT_CMusAvaConnectionMonitor_ConnectionCountL();
       
        void UT_CMusAvaConnectionMonitor_ConnectionIDLL();

        void UT_CMusAvaConnectionMonitor_MultimediaSharingAccessPointIDL();
        

	EUNIT_DECLARE_TEST_TABLE; 
		
        CMusAvaConnectionMonitor* iAvaConnectionMonitor;
        
        CMusAvaSharedObject* iSharedObject; 
        		
        CSipSseTestTls* iStorage;
        
        TInt iProfileId;
	
    };

#endif      //  __UT_CMUSAVACONNECTIONMONITOR_H__

// End of file
