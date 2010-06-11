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


#ifndef __UT_MUSENGURIPARSER_H__
#define __UT_MUSENGURIPARSER_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS


#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 * Tester class for TMusEngUriParser. 
 * 
 */
NONSHARABLE_CLASS( UT_TMusEngUriParser ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_TMusEngUriParser* NewL();
        static UT_TMusEngUriParser* NewLC();
                
        /**
         * Destructor
         */
        ~UT_TMusEngUriParser();

    private: // Constructors and destructors

        UT_TMusEngUriParser();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methdods
    
        void UT_TMusEngUriParser_TMusEngUriParserL();
        void UT_TMusEngUriParser_GetUri8LL();
        void UT_TMusEngUriParser_GetUri16LL();
        void UT_TMusEngUriParser_ParseUriLL();
        
    private:    // Data

        

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGURIPARSER_H__

// End of file
