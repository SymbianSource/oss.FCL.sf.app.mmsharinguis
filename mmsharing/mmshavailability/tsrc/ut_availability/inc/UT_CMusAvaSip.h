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


#ifndef __UT_CMUSAVASIP_H__
#define __UT_CMUSAVASIP_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CSipSseTestTls;
class CMusAvaSip;
        
//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaSip
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaSip* NewL();
        static UT_CMusAvaSip* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaSip();
        
   private:    // Constructors and destructors

        UT_CMusAvaSip();
        void ConstructL();


    public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaSip_NewLL();
	
        void UT_CMusAvaSip_AddAdapterLL();
        
        void UT_CMusAvaSip_RemoveAdapterL();
        
        void UT_CMusAvaSip_CreateProfileL(); 
        
        void UT_CMusAvaSip_CreateSipProfileL();
     
        void UT_CMusAvaSip_ProfileL();

        void UT_CMusAvaSip_ProfileRegistryL();
        	 
    	void UT_CMusAvaSip_ConnectionL();
    	 
    	void UT_CMusAvaSip_ConnectionObserverL();
    		
        void UT_CMusAvaSip_SipL();

        void UT_CMusAvaSip_CreateSIPConnectionLL();
        
        void UT_CMusAvaSip_OwnDomainL();
            

	EUNIT_DECLARE_TEST_TABLE; 
		
        		
       CSipSseTestTls* iStorage;
	   CMusAvaSip* iMusAvaSip;
       TInt iProfileId;	    
    };

#endif      //  __UT_CMusAvaSip_H__

// End of file
