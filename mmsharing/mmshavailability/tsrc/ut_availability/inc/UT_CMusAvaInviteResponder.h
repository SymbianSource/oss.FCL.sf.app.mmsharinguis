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


#ifndef __UT_CMUSAVAINVITERESPONDER_H__
#define __UT_CMUSAVAINVITERESPONDER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CSipSseTestTls;
class CMusAvaInviteResponder;
class CMusAvaInviteResponderObserverImp;
class CSIPServerTransaction;
        
//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaInviteResponder
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaInviteResponder* NewL();
        static UT_CMusAvaInviteResponder* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaInviteResponder();
        
   private:    // Constructors and destructors

        UT_CMusAvaInviteResponder();
        void ConstructL();


    public: 	// From observer interface
		
		

    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_CMusAvaInviteResponder_NewLL();
        
        void UT_CMusAvaInviteResponder_AnswerLL();
	
	    void UT_CMusAvaInviteResponder_SendResponseLL();
	
        void UT_CMusAvaInviteResponder_ResponseCompleteL();
        
        void UT_CMusAvaInviteResponder_ResponseLL();
       

	EUNIT_DECLARE_TEST_TABLE; 
        		
        CSipSseTestTls* iStorage;
        CMusAvaInviteResponder* iInviteResponder;
        CMusAvaInviteResponderObserverImp* iResponderObserverImp;
        CSIPServerTransaction* iServerTransaction;
       
    };

#endif      //  __UT_CMUSAVAINVITERESPONDER_H__

// End of file
