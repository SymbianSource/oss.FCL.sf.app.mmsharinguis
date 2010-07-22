/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __UT_MUSAIWRESOURCEFINDER_H__
#define __UT_MUSAIWRESOURCEFINDER_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS


//  CLASS DEFINITION

NONSHARABLE_CLASS( UT_MusAiwResourceFinder )
     : public CEUnitTestSuiteClass
    {
    public: // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_MusAiwResourceFinder* NewL();
        static UT_MusAiwResourceFinder* NewLC();
        /**
         * Destructor
         */
        ~UT_MusAiwResourceFinder();

    private: // Constructors and destructors

        UT_MusAiwResourceFinder();
        void ConstructL();

    private: // Test case setup and teardown

         void SetupL();
        
         void Teardown();
    
    public: // Test cases
    
        void TestL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif      //  __UT_MUSAIWRESOURCEFINDER_H__

// End of file
