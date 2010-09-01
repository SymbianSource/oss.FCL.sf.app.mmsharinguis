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


#ifndef __UT_CMUSAVATERMINAL_H__
#define __UT_CMUSAVATERMINAL_H__

#ifdef __MWERKS__
#pragma warn_emptydecl off
#pragma warn_illtokenpasting off
#endif

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMusAvaTerminal;
class CMusAvaCapabilityExchange;
class CSIP;
class CSIPConnection;
class CSIPProfile;
class CMusAvaCapability;
class CMusAvaOptionHandler;
class CMusAvaObserverImp;
class CMusAvaSettingsImp;
class CMusAvaAvailabilityObserverImp;	

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
NONSHARABLE_CLASS( UT_CMusAvaTerminal )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaTerminal* NewL();
        static UT_CMusAvaTerminal* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaTerminal();

    private:    // Constructors and destructors

        UT_CMusAvaTerminal();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

        void SetupL();
        
        void Teardown();
        
        void UT_CMusAvaTerminal_NewLL();

        void UT_CMusAvaTerminal_MatchLL();

        void UT_CMusAvaTerminal_ExecuteQueryLL();

        void UT_CMusAvaTerminal_QueryCompletedLL();

        void UT_CMusAvaTerminal_QueryCanceledL();
        
        void UT_CMusAvaTerminal_QueryRequestedLL();
        
        void UT_CMusAvaTerminal_IsDiscoveredL();
        

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

    CMusAvaCapabilityExchange* iExchange;
    
    
    CSIP* iSIP;
    CSIPConnection* iSIPConnection;
    CSIPProfile* iProfile;
    CMusAvaTerminal* iTerminal;

    CMusAvaOptionHandler* iOptionHandler;
	CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
	CMusAvaSettingsImp* iConcreteSettings;

    private: // NOT owned
    
    CMusAvaCapability* iCapability;

    };

#endif      //  __UT_CSTCETERMINAL_H__

// End of file
