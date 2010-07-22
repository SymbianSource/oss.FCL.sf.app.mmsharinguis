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


#ifndef __UT_CMUSAVANETWORKMODESTATUS_H__
#define __UT_CMUSAVANETWORKMODESTATUS_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

#include "musavatelephonystatusobserver.h"

//  FORWARD DECLARATIONS
class CSipSseTestTls;
class CMusAvaTelephonyStatusObserverImp;
class CMusAvaNetworkModeStatus;	

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaNetworkModeStatus
     : public CEUnitTestSuiteClass,
       MMusAvaTelephonyStatusObserver
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaNetworkModeStatus* NewL();
        static UT_CMusAvaNetworkModeStatus* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaNetworkModeStatus();

    private:    // Constructors and destructors

        UT_CMusAvaNetworkModeStatus();
        void ConstructL();

	public: 	// From MMusAvaTelephonyStatusObserver observer interface
	
        void PhoneNetworkModeStatus( 
                            RMobilePhone::TMobilePhoneNetworkMode aStatus );
    
        void NetworkRegistrationStatus( 
                RMobilePhone::TMobilePhoneRegistrationStatus aRegStatus );	

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaNetworkModeStatus_NewLL();
        
        void UT_CMusAvaNetworkModeStatus_AddAdapterLL();

        void UT_CMusAvaNetworkModeStatus_RemoveAdapterL();
        
        void UT_CMusAvaNetworkModeStatus_PhoneNetworkModeL();
        
        void UT_CMusAvaNetworkModeStatus_PhoneNetworkModeStatusL();
        
        static TInt Notify( TAny* aPtr );

		EUNIT_DECLARE_TEST_TABLE; 
		
				
		CSipSseTestTls* iStorage;
		CMusAvaNetworkModeStatus* iNetworkRegistrationStatus;
	    CMusAvaTelephonyStatusObserverImp* iObserver;

        RMobilePhone::TMobilePhoneNetworkMode iNetworkStatus;
        CPeriodic* iPeriodic;
    };

#endif      //  __UT_CMUSAVANETWORKREGISTRATIONYSTATUS_H__

// End of file
