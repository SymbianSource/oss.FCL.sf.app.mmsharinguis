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


#ifndef __UT_CMUSAVACONTACTAVAILABILITY_H__
#define __UT_CMUSAVACONTACTAVAILABILITY_H__

#include <mussettingskeys.h>

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <PbkFields.hrh>

//  FORWARD DECLARATIONS
class CMusAvaAvailabilityObserverImp;
class CMusAvaContactAvailability;
class CSipSseTestTls;
class CMusAvaSettingsImp;
class CContactTextField;
class CPbkContactItem;
class CPbkContactEngine;
//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaContactAvailability
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaContactAvailability* NewL();
        static UT_CMusAvaContactAvailability* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaContactAvailability();

    private:    // Constructors and destructors

        UT_CMusAvaContactAvailability();
        void ConstructL();

	public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();
        
        void SetupOpSpecificL();

        void Teardown();

        void UT_CMusAvaController_NewLL();   
		
        void UT_CMusAvaController_DoExecuteLL();
         		
        void UT_CMusAvaController_DoExecuteL1_L();
         		
        void UT_CMusAvaController_DoExecuteL2_L();
         		
        void UT_CMusAvaController_NameL();
          
        void UT_CMusAvaController_StatusL();
        
        void UT_CMusAvaController_StopL();
        
        void UT_CMusAvaController_ActiveCallLL();   
        
        void UT_CMusAvaController_ActiveCallLOpSpecificL();   
        
        void UT_CMusAvaController_ContactLL();
        
        void UT_CMusAvaController_ContactArrayLL();
        
        void UT_CMusAvaController_ContactItemL();   
        
        void UT_CMusAvaController_PhoneResulutionLL();         
                    
        void UT_CMusAvaController_ContactIdL();
       
        void UT_CMusAvaController_ContactIdSearchL();

        void UT_CMusAvaController_ResolveAddressesLL();
        
        void UT_CMusAvaController_ResolveAddressesLOpSpecificL();
       
        void UT_CMusAvaController_ResolvePhoneContactLL();   
        
        void UT_CMusAvaController_ResolvePhoneContactLOpSpecificL();  

        void UT_CMusAvaController_IsExistAlreadyL();    
        
        void UT_CMusAvaController_UpdateContactNameLL();
        
        void UT_CMusAvaController_CallConnectedLL();
        CContactTextField* EditableContactFieldL( CPbkContactEngine& aContactEngine, 
                                                  CPbkContactItem& aContact, 
                                                  TPbkFieldId aFieldId );
                                                  
        void AddFields(CPbkContactEngine& aContactEngine,
                       CPbkContactItem& contact,
                       const TDesC& aText, 
                       TPbkFieldId aFieldId);
        
        void AddToMyContact( CPbkContactEngine& aContactEngine,
                             const TDesC& aText, 
                             TPbkFieldId aFieldId,
                             const TDesC& aText2, 
                             TPbkFieldId aFieldId2 );
        

    private:    // Data

		EUNIT_DECLARE_TEST_TABLE; 
		
		CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
		
		CMusAvaSettingsImp* iConcreteSettings;
		
		CMusAvaContactAvailability* iContactAvailability;
				
		CSipSseTestTls* iStorage;		
		
		MusSettingsKeys::TOperatorVariant iSaveOperatorVariant;
		
    };

#endif      //  __UT_CMUSAVACONTACTAVAILABILITY_H__

// End of file
