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


#ifndef __UT_CMusAvaSipObserver_H__
#define __UT_CMusAvaSipObserver_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <e32property.h>

//  FORWARD DECLARATIONS
class CMusAvaSipObserver;
class CMusAvaSipAdapterImp;
        
//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaSipObserver
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaSipObserver* NewL();
        static UT_CMusAvaSipObserver* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaSipObserver();
        
   private:    // Constructors and destructors

        UT_CMusAvaSipObserver();
        void ConstructL();
        

    private:    // New methods

        void SetupL();

        void Teardown();

        void AddObserversL();
        
        void UT_CMusAvaSipObserver_AddObserverL();    
        
        void UT_CMusAvaSipObserver_RemoveObserverL();
        
        void UT_CMusAvaSipObserver_IncomingRequestL();
        
        void UT_CMusAvaSipObserver_TimedOutL();

	EUNIT_DECLARE_TEST_TABLE; 
		
        		
        CMusAvaSipObserver* iMusAvaSipObserver;
	    CMusAvaSipAdapterImp* iAdapterImp;	    
        CMusAvaSipAdapterImp* iAdapterImp2;
        CMusAvaSipAdapterImp* iAdapterImp3;
    };

#endif      //  __UT_CMusAvaSipObserver_H__

// End of file
