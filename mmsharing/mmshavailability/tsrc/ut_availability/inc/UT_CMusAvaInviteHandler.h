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


#ifndef __UT_CMUSAVAINVITEHANDLER_H__
#define __UT_CMUSAVAINVITEHANDLER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CSipSseTestTls;
class CMusAvaInviteHandler;
class CMusAvaAvailabilityObserverImp;
class CMusAvaSettingsImp;	

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaInviteHandler
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaInviteHandler* NewL();
        static UT_CMusAvaInviteHandler* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaInviteHandler();

    private:    // Constructors and destructors

        UT_CMusAvaInviteHandler();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaInviteHandler_NewLL();

        void UT_CMusAvaInviteHandler_IncomingRequestL();

        void UT_CMusAvaInviteHandler_DoExecuteLL();

        void UT_CMusAvaInviteHandler_NameL();

        void UT_CMusAvaInviteHandler_StopL();

        void UT_CMusAvaInviteHandler_InvitationReceivedLL();

        void UT_CMusAvaInviteHandler_InvitationAnsweredLDL();


		EUNIT_DECLARE_TEST_TABLE; 
				
		CMusAvaInviteHandler* iInviteHandler;
	    
	    CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
		
		CMusAvaSettingsImp* iConcreteSettings;
	  
    };

#endif      //  __UT_CMUSAVAINVITEHANDLER_H__

// End of file
