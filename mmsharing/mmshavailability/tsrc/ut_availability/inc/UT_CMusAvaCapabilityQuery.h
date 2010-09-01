/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef __UT_CMUSAVACAPABILITYQUERY_H__
#define __UT_CMUSAVACAPABILITYQUERY_H__

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
class CMusAvaCapabilityQuery;
class CMusAvaSharedObject;
class CMusAvaAvailabilityObserverImp;

#include "mussettingskeys.h"

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaCapabilityQuery
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaCapabilityQuery* NewL();
        static UT_CMusAvaCapabilityQuery* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaCapabilityQuery();

    private:    // Constructors and destructors

        UT_CMusAvaCapabilityQuery();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaCapabilityQuery_NewLL();
        
        void UT_CMusAvaCapabilityQuery_ExecuteLL();
        
        void UT_CMusAvaCapabilityQuery_ValidateUriL();
        
        void UT_CMusAvaCapabilityQuery_CanceledL();
        
        void UT_CMusAvaCapabilityQuery_CompletedLL();
        
        void UT_CMusAvaCapabilityQuery_CompletedL1L();
        
        void UT_CMusAvaCapabilityQuery_CompletedL2L();
        
        void UT_CMusAvaCapabilityQuery_ValidateAndStoreCodecsLL();
        
        void UT_CMusAvaCapabilityQuery_ValidateAttributesLL(); 
        
        void UT_CMusAvaCapabilityQuery_ValidateContactLL();
        
        void UT_CMusAvaCapabilityQuery_PrepareL();
        
        void UT_CMusAvaCapabilityQuery_DoCompletedLL();
        
        void UT_CMusAvaCapabilityQuery_DoCompleted200OKLL();
        
        void UT_CMusAvaCapabilityOtherSDPHeadersLL();
        
		EUNIT_DECLARE_TEST_TABLE; 
				
		CMusAvaOptionHandler* iOptionHandler;
	    
	    CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
		
		CMusAvaSettingsImp* iConcreteSettings;
		
		CMusAvaCapabilitySipAgent* iSipAgent;
		
    	CMusAvaCapabilityExchange* iExchange;
        
        //CSIP& iSIP;
        CSIPConnection* iSIPConnection;
        CSIPProfile* iProfile;
        
        MusSettingsKeys::TOperatorVariant iOpVariantSetting;

    private: // NOT owned
    
    	CSipSseTestTls* iStorage;
        CMusAvaCapability* iCapability;
        
        CMusAvaSharedObject* iSharedObject; 
        
        CMusAvaCapabilityQuery* iQuery;
  
    };

#endif      //  __UT_CMUSAVACAPABILITYQUERY_H__

// End of file
