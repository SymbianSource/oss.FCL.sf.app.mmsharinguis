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


#ifndef __UT_CMUSAVAOPTIONHANDLER_H__
#define __UT_CMUSAVAOPTIONHANDLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CSIPConnection;
class CSIPProfile;
class CSipSseTestTls;
class CMusAvaOptionHandler;
class CMusAvaObserverImp;
class CMusAvaSettingsImp;	
class CMusAvaSharedObject;
class CMusAvaAvailabilityObserverImp;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaOptionHandler
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaOptionHandler* NewL();
        static UT_CMusAvaOptionHandler* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaOptionHandler();

    private:    // Constructors and destructors

        UT_CMusAvaOptionHandler();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaOptionHandler_NewLL();

        void UT_CMusAvaOptionHandler_DoExecuteLL();

        void UT_CMusAvaOptionHandler_NameL();

        void UT_CMusAvaOptionHandler_StatusL();

        void UT_CMusAvaOptionHandler_StopL();

        void UT_CMusAvaOptionHandler_CapabilitiesResolvedL();
        
        void UT_CMusAvaOptionHandler_CapabilitiesResolvedForCingularL();

        void UT_CMusAvaOptionHandler_SipHeadersL();
        
        void UT_CMusAvaOptionHandler_VideoCodecsResolvedLL();

        
		EUNIT_DECLARE_TEST_TABLE; 
		CSipSseTestTls* iStorage;
				
		CMusAvaOptionHandler* iOptionHandler;
	    
	    CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
		
		CMusAvaSettingsImp* iConcreteSettings;
	  
        CMusAvaSharedObject* iSharedObject; 

        CSIPProfile* iProfile;
        
    };

#endif      //  __UT_CMUSAVAOPTIONHANDLER_H__

// End of file
