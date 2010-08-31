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


#ifndef __UT_CMUSAVANETWORKAVAILABILITY_H__
#define __UT_CMUSAVANETWORKAVAILABILITY_H__

#ifdef __MWERKS__
#pragma warn_emptydecl off
#pragma warn_illtokenpasting off
#endif

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>


//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMusAvaNetworkAvailability;
class CMusAvaAvailabilityObserverImp;
class CMusAvaSettingsImp;


#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CMusAvaNetworkAvailability )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaNetworkAvailability* NewL();
        static UT_CMusAvaNetworkAvailability* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaNetworkAvailability();

    private:    // Constructors and destructors

        UT_CMusAvaNetworkAvailability();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

        void SetupL();

        void Teardown();
        
        void UT_CMusAvaNetworkAvailability_DoExecuteLL();
        
        void UT_CMusAvaNetworkAvailability_DoExecuteL_SipUriL();
        
        void UT_CMusAvaNetworkAvailability_StopL();

        void UT_CMusAvaNetworkAvailability_NameL();

        void UT_CMusAvaNetworkAvailability_CallConnectedLL();

        void UT_CMusAvaNetworkAvailability_ConferenceCallLL();

        void UT_CMusAvaNetworkAvailability_CallHoldLL();

        void UT_CMusAvaNetworkAvailability_NoActiveCallLL();
        
        void UT_CMusAvaNetworkAvailability_SetRemoteHostLL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        CMusAvaNetworkAvailability* iNetworkAvailability;
        CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
        CMusAvaSettingsImp* iSettings;
    };

#endif      //  __UT_CMUSAVANETWORKAVAILABILITY_H__

// End of file
