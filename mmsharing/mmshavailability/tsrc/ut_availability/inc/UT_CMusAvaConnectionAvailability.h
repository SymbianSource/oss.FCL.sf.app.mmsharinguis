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


#ifndef __UT_CMUSAVACONNECTIONAVAILABILITY_H__
#define __UT_CMUSAVACONNECTIONAVAILABILITY_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CSipSseTestTls;
class CMusAvaAvailabilityObserverImp;
class CMusAvaSettingsImp;
class CMusAvaConnectionAvailability;
class CMusAvaSharedObject;
        
//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaConnectionAvailability
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaConnectionAvailability* NewL();
        static UT_CMusAvaConnectionAvailability* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaConnectionAvailability();

    private:    // Constructors and destructors

        UT_CMusAvaConnectionAvailability();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaConnectionAvailability_NewLL();   
		
        void UT_CMusAvaConnectionAvailability_DoExecuteLL();
         		
        void UT_CMusAvaConnectionAvailability_NameL();
          
        void UT_CMusAvaConnectionAvailability_StopL();
        
        void UT_CMusAvaConnectionAvailability_PhoneNetworkModeStatusL();
                          
        void UT_CMusAvaConnectionAvailability_NetworkRegistrationStatusL();
        
        void UT_CMusAvaConnectionAvailability_EventLL();
        
        void UT_CMusAvaConnectionAvailability_NetworkRegistrationAndSettingsLL();
        
        void UT_CMusAvaConnectionAvailability_ManualActivationLL();
        
        
        

	EUNIT_DECLARE_TEST_TABLE; 
		
				
        CMusAvaAvailabilityObserverImp* iAvailabilityObserver;

        CMusAvaSettingsImp* iConcreteSettings;

        CMusAvaConnectionAvailability* iConnectionAvailability;
        		
        CSipSseTestTls* iStorage;
        
        CMusAvaSharedObject* iSharedObject;
        
        TInt iProfileId;
	
    };

#endif      //  __UT_CMUSAVACONNECTIONAVAILABILITY_H__

// End of file
