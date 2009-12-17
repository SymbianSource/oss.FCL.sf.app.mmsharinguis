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


#ifndef __UT_CMUSAVADEFAULTIMP_H__
#define __UT_CMUSAVADEFAULTIMP_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CSipSseTestTls;
class CMusAvaDefaultImp;	
class CMusAvaAvailabilityObserverImp;
class CMusAvaObserverImp;
class CMusAvaSettingsImp;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaDefaultImp
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaDefaultImp* NewL();
        static UT_CMusAvaDefaultImp* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaDefaultImp();

    private:    // Constructors and destructors

        UT_CMusAvaDefaultImp();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void StartL();
        void Teardown();

        void UT_CMusAvaDefaultImp_NewLL();
        
        void UT_CMusAvaDefaultImp_CreateAvailabilityModulesLL();
        
        void UT_CMusAvaDefaultImp_ConstuctHandlerAvailabilityModulesLL();
        
        void UT_CMusAvaDefaultImp_ConstructStaticAvailabilityModulesLL();
        
		void UT_CMusAvaDefaultImp_ConstructDynamicAvailabilityModulesLL();

        void UT_CMusAvaDefaultImp_ExecuteAvailablitityModulesL();

        void UT_CMusAvaDefaultImp_SettingsL();

        void UT_CMusAvaDefaultImp_SetObserverL();

        void UT_CMusAvaDefaultImp_StartLL();

        void UT_CMusAvaDefaultImp_StopL();

        void UT_CMusAvaDefaultImp_AvailabilityChangedL();

        void UT_CMusAvaDefaultImp_AvailabilityErrorL();

        void UT_CMusAvaDefaultImp_AvailableL();
        
        void UT_CMusAvaDefaultImp_AvailabilityStateL();


		EUNIT_DECLARE_TEST_TABLE; 
				
		CMusAvaDefaultImp* iDefaultImp;
		CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
		CMusAvaObserverImp* iMusAvaObserver;
        CSipSseTestTls* iStorage;
   		CMusAvaSettingsImp* iConcreteSettings;
    };

#endif      //  __UT_CMUSAVADEFAULTIMP_H__

// End of file
