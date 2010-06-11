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


#ifndef __UT_CMUSAVACAPABILITYEXCHANGE_H__
#define __UT_CMUSAVACAPABILITYEXCHANGE_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CSipSseTestTls;
class CMusAvaCapabilityExchange;
class CSIP;
class CSIPConnection;
class CSIPProfile;
class CMusAvaCapability;
class CMusAvaOptionHandler;
class CMusAvaObserverImp;
class CMusAvaSettingsImp;
class CMusAvaAvailabilityObserverImp;	

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaCapabilityExchange
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaCapabilityExchange* NewL();
        static UT_CMusAvaCapabilityExchange* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaCapabilityExchange();

    private:    // Constructors and destructors

        UT_CMusAvaCapabilityExchange();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaCapabilityExchange_NewLL();

        void UT_CMusAvaCapabilityExchange_QueryObserverL();

        void UT_CMusAvaCapabilityExchange_TerminalIdL();

        void UT_CMusAvaCapabilityExchange_QueryReceivedL();
        
        void UT_CMusAvaCapabilityExchange_QueryReceived1L();
        
        void UT_CMusAvaCapabilityExchange_QueryReceived2L();
        
        void UT_CMusAvaCapabilityExchange_CancelQueryL();

        void UT_CMusAvaCapabilityExchange_AddCapabilityLL();
        
        void UT_CMusAvaCapabilityExchange_TerminalLL();

        void UT_CMusAvaCapabilityExchange_RejectQueryLL();

        void UT_CMusAvaCapabilityExchange_FindByFeatureL();
        
        void UT_CMusAvaCapabilityExchange_AnswerToQueryReceivedL();

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
				
		CSipSseTestTls* iStorage;   
		CMusAvaCapabilityExchange* iExchange;
        CSIP* iSIP;
        CSIPConnection* iSIPConnection;
        CSIPProfile* iProfile;

   		CMusAvaOptionHandler* iOptionHandler;
	    CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
		CMusAvaSettingsImp* iConcreteSettings;

    private: // NOT owned
    
        CMusAvaCapability* iCapability;
	  
    };

#endif      //  __UT_CMUSAVACAPABILITYEXCHANGE_H__

// End of file
