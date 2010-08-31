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


#ifndef __UT_CMUSAVASETTINGSIMP_H__
#define __UT_CMUSAVASETTINGSIMP_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CMusAvaSettingsImp;	

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaSettingsImp
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaSettingsImp* NewL();
        static UT_CMusAvaSettingsImp* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaSettingsImp();

    private:    // Constructors and destructors

        UT_CMusAvaSettingsImp();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaSettingsImp_NewLL();
	
        void UT_CMusAvaSettingsImp_DestructorL();
	
        void UT_CMusAvaSettingsImp_TelNumberL();    	

        void UT_CMusAvaSettingsImp_ContactNameL();
        
        void UT_CMusAvaSettingsImp_SipAddressProposalL();    	

        void UT_CMusAvaSettingsImp_SipAddressesL();

        void UT_CMusAvaSettingsImp_ContactIdL();

        void UT_CMusAvaSettingsImp_VideoCodecsL();

        void UT_CMusAvaSettingsImp_AudioCodecsL();

        void UT_CMusAvaSettingsImp_SipProfileIdL();

        void UT_CMusAvaSettingsImp_SetObserverL();
        
        void UT_CMusAvaSettingsImp_ManualActivationL();
         
        void UT_CMusAvaSettingsImp_ApplicationStateL();
        
        void UT_CMusAvaSettingsImp_CallDirectionL();
        
        void UT_CMusAvaSettingsImp_SetTelNumberLL();    

        void UT_CMusAvaSettingsImp_SetSipAddressProposalLL( );    
            
        void UT_CMusAvaSettingsImp_ReleseTelNumberL();	

        void UT_CMusAvaSettingsImp_SetSipAddressesLL();

        void UT_CMusAvaSettingsImp_SetContactIdL();

        void UT_CMusAvaSettingsImp_SetVideoCodecsLL();

        void UT_CMusAvaSettingsImp_SetAudioCodecsLL();

        void UT_CMusAvaSettingsImp_SetSipProfileIdL();
             
        void UT_CMusAvaSettingsImp_CopyDescArrayLL();


		EUNIT_DECLARE_TEST_TABLE; 
				
		CMusAvaSettingsImp* iSettingsImp;
	  
    };

#endif      //  __UT_CMUSAVASETTINGSIMP_H__

// End of file
