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


#ifndef __UT_CMusAvaSettingAvailability_H__
#define __UT_CMusAvaSettingAvailability_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CMusAvaAvailabilityObserverImp;
class CMusAvaSharedObject;
class CMusAvaSettingAvailability;        
//  CLASS DEFINITION

// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files // include this file. Directive can be removed when // EUnit is fixed.  
#pragma warn_illtokenpasting off
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaSettingAvailability
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaSettingAvailability* NewL();
        static UT_CMusAvaSettingAvailability* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaSettingAvailability();

    private:    // Constructors and destructors

        UT_CMusAvaSettingAvailability();
        void ConstructL();

	
    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaSettingAvailability_NewLL();   

        void UT_CMusAvaSettingAvailability_IsForbiddenByOptionsResponseL();

        void UT_CMusAvaSettingAvailability_CheckActivationStateL();
		
        void UT_CMusAvaSettingAvailability_DoExecuteLL();
         		
        void UT_CMusAvaSettingAvailability_NameL();
          
        void UT_CMusAvaSettingAvailability_StopL();
        

	EUNIT_DECLARE_TEST_TABLE; 
		
	private:			
        CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
        CMusAvaSettingAvailability* iSettingAvailability;	
    };

#endif      //  __UT_CMusAvaSettingAvailability_H__

// End of file
