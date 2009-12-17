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


#ifndef __UT_CMUSAVASIPPROFILEREGISTRYOBSERVER_H__
#define __UT_CMUSAVASIPPROFILEREGISTRYOBSERVER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CMusAvaAvailabilityObserverImp;
class CMusAvaSharedObject;
class CMusAvaSipProfileRegistryObserver;        
//  CLASS DEFINITION

// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files // include this file. Directive can be removed when // EUnit is fixed.  
#pragma warn_illtokenpasting off
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaSipProfileRegistryObserver
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaSipProfileRegistryObserver* NewL();
        static UT_CMusAvaSipProfileRegistryObserver* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaSipProfileRegistryObserver();

    private:    // Constructors and destructors

        UT_CMusAvaSipProfileRegistryObserver();
        void ConstructL();

	
    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaSipProfileRegistryObserver_NewLL();   

        void UT_CMusAvaSipProfileRegistryObserver_AddAdapterLL();
		
        void UT_CMusAvaSipProfileRegistryObserver_RemoveAdapterL();
         		
        void UT_CMusAvaSipProfileRegistryObserver_ProfileRegistryEventOccurredL();
          
        void UT_CMusAvaSipProfileRegistryObserver_ProfileRegistryErrorOccurredL();
        

	EUNIT_DECLARE_TEST_TABLE; 
		
	private:			
        CMusAvaSipProfileRegistryObserver* iSipprofileRegistryObserver;	
    };

#endif      //  __UT_CMUSAVASIPPROFILEREGISTRYOBSERVER_H__

// End of file
