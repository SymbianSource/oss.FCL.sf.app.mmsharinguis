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


#ifndef __UT_CMusAvaSipConnectionObserver_H__
#define __UT_CMusAvaSipConnectionObserver_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CMusAvaSipConnectionObserver;
class CMusAvaSipAdapterImp;
        
//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaSipConnectionObserver
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaSipConnectionObserver* NewL();
        static UT_CMusAvaSipConnectionObserver* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaSipConnectionObserver();
        
   private:    // Constructors and destructors

        UT_CMusAvaSipConnectionObserver();
        void ConstructL();
        

    private:    // New methods

        void SetupL();

        void Teardown();
        
        void RemoveDefaultHandler();
        void AddObserversL();

        void UT_CMusAvaSipConnectionObserver_AddObserverL();
        void UT_CMusAvaSipConnectionObserver_RemoveObserverL();
        void UT_CMusAvaSipConnectionObserver_ConnectionStateChangedL();
        void UT_CMusAvaSipConnectionObserver_ErrorOccuredL();
        void UT_CMusAvaSipConnectionObserver_IncomingRequestL();
        void UT_CMusAvaSipConnectionObserver_IncomingResponseL();
        void UT_CMusAvaSipConnectionObserver_InviteCanceledL();
        void UT_CMusAvaSipConnectionObserver_InviteCompletedL();

        
	EUNIT_DECLARE_TEST_TABLE; 
		
        		
        CMusAvaSipConnectionObserver* iMusAvaSipConnectionObserver;
	    CMusAvaSipAdapterImp* iAdapterImp;	    
        CMusAvaSipAdapterImp* iAdapterImp2;      
        CMusAvaSipAdapterImp* iAdapterImp3;      
    };

#endif      //  __UT_CMusAvaSipConnectionObserver_H__

// End of file
