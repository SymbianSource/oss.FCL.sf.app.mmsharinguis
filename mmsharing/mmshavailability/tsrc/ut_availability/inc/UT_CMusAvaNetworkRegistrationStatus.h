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


#ifndef __UT_CMUSAVANETWORKREGISTRATIONYSTATUS_H__
#define __UT_CMUSAVANETWORKREGISTRATIONYSTATUS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CSipSseTestTls;
class CMusAvaTelephonyStatusObserverImp;
class CMusAvaNetworkRegistrationStatus;	

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaNetworkRegistrationStatus
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaNetworkRegistrationStatus* NewL();
        static UT_CMusAvaNetworkRegistrationStatus* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaNetworkRegistrationStatus();

    private:    // Constructors and destructors

        UT_CMusAvaNetworkRegistrationStatus();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaNetworkRegistrationStatus_NewLL();
        
        void UT_CMusAvaNetworkRegistrationStatus_AddAdapterLL();
  
        void UT_CMusAvaNetworkRegistrationStatus_NetworkRegistrationL();

        void UT_CMusAvaNetworkRegistrationStatus_RemoveAdapterL();

        void UT_CMusAvaNetworkRegistrationStatus_TelephonyStatusL();

        void UT_CMusAvaNetworkRegistrationStatus_NetworkRegistrationStatusL();
                		
        void UT_CMusAvaNetworkRegistrationStatus_RunL();
          
        void UT_CMusAvaNetworkRegistrationStatus_StatusL();
        
        void UT_CMusAvaNetworkRegistrationStatus_DoCancelL();
        
        void UT_CMusAvaNetworkRegistrationStatus_RunErrorLL();   
        

		EUNIT_DECLARE_TEST_TABLE; 
		
				
		CSipSseTestTls* iStorage;
		CMusAvaNetworkRegistrationStatus* iNetworkRegistrationStatus;
	    CMusAvaTelephonyStatusObserverImp* iObserver;
	  
    };

#endif      //  __UT_CMUSAVANETWORKREGISTRATIONYSTATUS_H__

// End of file
