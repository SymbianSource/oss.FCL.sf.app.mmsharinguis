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


#ifndef __UT_CMUSAVACALLEVENTMONITOR_H__
#define __UT_CMUSAVACALLEVENTMONITOR_H__

#ifdef __MWERKS__
#pragma warn_emptydecl off
#pragma warn_illtokenpasting off
#endif

//  EXTERNAL INCLUDES
#include <CEUnitTestSuiteClass.h>

//  INTERNAL INCLUDES

//  FORWARD DECLARATIONS
class CMusAvaCallEventMonitor;
class CMusAvaCallMonitorObserverStub;


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
NONSHARABLE_CLASS( UT_CMusAvaCallEventMonitor )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaCallEventMonitor* NewL();
        static UT_CMusAvaCallEventMonitor* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaCallEventMonitor();

    private:    // Constructors and destructors

        UT_CMusAvaCallEventMonitor();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaCallEventMonitor_GetTelNumberLL();


    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 
        
        // Owned
        CMusAvaCallEventMonitor* iCallEventMonitor;
        
        // Owned
        CMusAvaCallMonitorObserverStub* iObserver;
    };

#endif      //  __UT_CMUSAVACALLEVENTMONITOR_H__

// End of file
