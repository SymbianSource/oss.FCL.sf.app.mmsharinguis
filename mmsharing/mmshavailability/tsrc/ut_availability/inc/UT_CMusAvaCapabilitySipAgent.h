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


#ifndef __UT_CMUSAVACAPABILITYSIPAGENT_H__
#define __UT_CMUSAVACAPABILITYSIPAGENT_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CSIPConnection;
class CSIPProfile;
class CMusAvaCapability;
class CSipSseTestTls;
class CMusAvaOptionHandler;
class CMusAvaObserverImp;
class CMusAvaSettingsImp;	
class CMusAvaCapabilityExchange;
class CMusAvaCapabilitySipAgent;
class CMusAvaSharedObject;
class CMusAvaAvailabilityObserverImp;

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaCapabilitySipAgent
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaCapabilitySipAgent* NewL();
        static UT_CMusAvaCapabilitySipAgent* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaCapabilitySipAgent();

    private:    // Constructors and destructors

        UT_CMusAvaCapabilitySipAgent();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaCapabilitySipAgent_NewLL();
        
        void UT_CMusAvaCapabilitySipAgent_ExecuteCapabilityQueryLL();
        
        void UT_CMusAvaCapabilitySipAgent_IncomingRequestL();
        
        void UT_CMusAvaCapabilitySipAgent_HandleIncomingRequestLL();
        
        void UT_CMusAvaCapabilitySipAgent_IncomingResponseL();
        
        void UT_CMusAvaCapabilitySipAgent_ErrorOccuredL();
        
        void UT_CMusAvaCapabilitySipAgent_TimedOutL();

        
		EUNIT_DECLARE_TEST_TABLE; 
				
		CMusAvaOptionHandler* iOptionHandler;
	    
	    CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
		
		CMusAvaSettingsImp* iConcreteSettings;
		
		CMusAvaCapabilitySipAgent* iSipAgent;
		
    	CMusAvaCapabilityExchange* iExchange;
        
        CSIPConnection* iSIPConnection;
        CSIPProfile* iProfile;

    private: // NOT owned
    
		CSipSseTestTls* iStorage;
        CMusAvaCapability* iCapability;
        
        CMusAvaSharedObject* iSharedObject; 
        
        TInt iProfileId;
  
    };

#endif      //  __UT_CMUSAVACAPABILITYSIPAGENT_H__

// End of file
