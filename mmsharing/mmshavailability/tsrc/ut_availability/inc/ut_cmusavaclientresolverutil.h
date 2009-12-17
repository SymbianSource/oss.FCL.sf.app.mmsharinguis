/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit test class
*
*/



#ifndef __UT_CMUSAVACLIENTRESOLVERUTIL_H__
#define __UT_CMUSAVACLIENTRESOLVERUTIL_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusAvaClientResolverUtil;

#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 * Tester class for CMusAvaClientResolverUtil. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusAvaClientResolverUtil ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaClientResolverUtil* NewL();
        static UT_CMusAvaClientResolverUtil* NewLC();
                
        /**
         * Destructor
         */
        ~UT_CMusAvaClientResolverUtil();

    private: // Constructors and destructors

        UT_CMusAvaClientResolverUtil();
        void ConstructL();
        
        
    private: // Test case setup and teardown

        // Setup and teardown 
        void SetupL();
        void Teardown();

        // Setup and teardown for a test to be run using Sawfish
        void Setup_SawfishL();
        void Teardown_Sawfish();


    private: // Test methods
    
        void UT_NewL_SucceedsInSawfishL();
        void UT_NewL_FailsInOthersThanSawfishL();
        
    private:    // Data

        CMusAvaClientResolverUtil* iResolver;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMUSAVACLIENTRESOLVERUTIL_H__

// End of file
