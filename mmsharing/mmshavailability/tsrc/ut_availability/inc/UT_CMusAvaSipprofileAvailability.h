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


#ifndef __UT_CMUSAVASIPPROFILEAVAILABILITY_H__
#define __UT_CMUSAVASIPPROFILEAVAILABILITY_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CMusAvaAvailabilityObserverImp;
class CMusAvaSharedObject;
class CMusAvaSipprofileAvailability;        
//  CLASS DEFINITION

// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files // include this file. Directive can be removed when // EUnit is fixed.  
#pragma warn_illtokenpasting off
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaSipprofileAvailability
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaSipprofileAvailability* NewL();
        static UT_CMusAvaSipprofileAvailability* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaSipprofileAvailability();

    private:    // Constructors and destructors

        UT_CMusAvaSipprofileAvailability();
        void ConstructL();

	
    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaSipprofileAvailability_NewLL();   

        void UT_CMusAvaSipprofileAvailability_IsProfileExistL();
		
        void UT_CMusAvaSipprofileAvailability_DoExecuteLL();
         		
        void UT_CMusAvaSipprofileAvailability_NameL();
          
        void UT_CMusAvaSipprofileAvailability_StopL();
        

	EUNIT_DECLARE_TEST_TABLE; 
		
	private:			
        CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
        CMusAvaSipprofileAvailability* iSipprofileAvailability;	
    };

#endif      //  __UT_CMUSAVASIPPROFILEAVAILABILITY_H__

// End of file
