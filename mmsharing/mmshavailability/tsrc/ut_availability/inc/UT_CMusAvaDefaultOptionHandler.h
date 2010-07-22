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


#ifndef __UT_CMUSAVADEFAULTOPTIONHANDLER_H__
#define __UT_CMUSAVADEFAULTOPTIONHANDLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

#include "mussettingskeys.h"

//  FORWARD DECLARATIONS
class CSipSseTestTls;
class CMusAvaDefaultOptionHandler;
class CMusAvaAvailabilityObserverImp;
class CMusAvaSettingsImp;
class CMusAvaSettingsObserverImp;	

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaDefaultOptionHandler
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaDefaultOptionHandler* NewL();
        static UT_CMusAvaDefaultOptionHandler* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaDefaultOptionHandler();

    private:    // Constructors and destructors

        UT_CMusAvaDefaultOptionHandler();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();
        
        void SetupOpSpecificL();

        void Teardown();

        void UT_CMusAvaDefaultOptionHandler_NewLL();
        
        void UT_CMusAvaDefaultOptionHandler_IncomingRequestL();

        void UT_CMusAvaDefaultOptionHandler_DoExecuteLL();

        void UT_CMusAvaDefaultOptionHandler_NameL();

        void UT_CMusAvaDefaultOptionHandler_StatusL();

        void UT_CMusAvaDefaultOptionHandler_StopL();
        
        void UT_CMusAvaDefaultOptionHandler_IncomingOptionRequestLL();
        
        void UT_CMusAvaDefaultOptionHandler_IncomingOptionRequestLOpSpecificL();        
        
        void UT_CMusAvaDefaultOptionHandler_ResponseLL();
        
        void UT_CMusAvaDefaultOptionHandler_NoCapabilityResponseLL();
        
        void UT_CMusAvaDefaultOptionHandler_NoCapabilityResponseLOpSpecificL(); 
        
        void UT_CMusAvaDefaultOptionHandler_CreateResponseLL();
        
        void UT_CMusAvaDefaultOptionHandler_TimedOutL();      


		EUNIT_DECLARE_TEST_TABLE; 
		CSipSseTestTls* iStorage;
				
		CMusAvaDefaultOptionHandler* iOptionDefaultHandler;
	    
	    CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
		
		CMusAvaSettingsImp* iConcreteSettings;
		
		CMusAvaSettingsObserverImp* iSettingsObserverImp;
		
    };

#endif      //  __UT_CMUSAVADEFAULTOPTIONHANDLER_H__

// End of file
