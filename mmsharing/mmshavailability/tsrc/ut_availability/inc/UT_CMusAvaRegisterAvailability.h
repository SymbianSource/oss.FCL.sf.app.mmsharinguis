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


#ifndef __UT_CMUSAVAREGISTERAVAILABILITY_H__
#define __UT_CMUSAVAREGISTERAVAILABILITY_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>

//  FORWARD DECLARATIONS
class CMusAvaAvailabilityObserverImp;
class CSipSseTestTls;
class CMusAvaSettingsImp;
class CMusAvaRegisterAvailability;
class CMusAvaSharedObject;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaRegisterAvailability
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaRegisterAvailability* NewL();
        static UT_CMusAvaRegisterAvailability* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaRegisterAvailability();

    private:    // Constructors and destructors

        UT_CMusAvaRegisterAvailability();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaRegisterAvailability_NewLL();   
		
        void UT_CMusAvaRegisterAvailability_DoExecuteLL();
    	
        void UT_CMusAvaRegisterAvailability_NameL();
           		
        void UT_CMusAvaRegisterAvailability_StatusL();
        
        void UT_CMusAvaRegisterAvailability_Stop();

    	void UT_CMusAvaRegisterAvailability_ProfileRegistryEventOccurredL();

    	void UT_CMusAvaRegisterAvailability_CreateSipAddressProposalLL();
    	
    	void UT_CMusAvaRegisterAvailability_ConnectionStateChangedL();
    	
    	void UT_CMusAvaRegisterAvailability_ProfileRegistryErrorOccurredL();
    	
    	void UT_CMusAvaRegisterAvailability_RegisterLL();

        void UT_CMusAvaRegisterAvailability_InitializeRegisterL();    	

		void UT_CMusAvaRegisterAvailability_EnableRegisterLL();
        
        void UT_CMusAvaRegisterAvailability_UpdateSIPProfileL();
        
        void UT_CMusAvaRegisterAvailability_RegistrationStatusL();

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
		CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
		
		CMusAvaSettingsImp* iConcreteSettings;
		
		CMusAvaRegisterAvailability* iRegisterAvailability;
				
		CSipSseTestTls* iStorage;		
		
        CMusAvaSharedObject* iSharedObj;
        
        TInt iProfileId;
    };

#endif      //  __UT_CMUSAVAREGISTERAVAILABILITY_H__

// End of file
