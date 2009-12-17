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


#ifndef __UT_CMUSAVASHAREDOBJECT_H__
#define __UT_CMUSAVASHAREDOBJECT_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CSipSseTestTls;
class CMusAvaSharedObject;
        
//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaSharedObject
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaSharedObject* NewL();
        static UT_CMusAvaSharedObject* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaSharedObject();
        
   private:    // Constructors and destructors

        UT_CMusAvaSharedObject();
        void ConstructL();


    public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaSharedObject_NewLL();
	         
        void UT_CMusAvaSharedObject_ConnectionMonitorL();
	
        void UT_CMusAvaSharedObject_MusAvaSipL();
        
        void UT_CMusAvaSharedObject_SipL();         

	EUNIT_DECLARE_TEST_TABLE; 
        		
        CSipSseTestTls* iStorage;
        
        CMusAvaSharedObject* iSharedObject; 
    };

#endif      //  __UT_CMUSAVASHAREDOBJECT_H__

// End of file
