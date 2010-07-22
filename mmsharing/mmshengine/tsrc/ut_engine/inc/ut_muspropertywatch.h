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


#ifndef __UT_MUSPROPERYWATCH_H__
#define __UT_MUSPROPERYWATCH_H__


//  EXTERNAL INCLUDES
#include <ceunittestsuiteclass.h>
#include <eunitmacros.h>

#include "musengstubs.h"

//  FORWARD DECLARATIONS
class CMusPropertyWatch;
class TMusUiPropertyObserverStub;

//  CLASS DEFINITION
/**
 * Tester class for CMusEngSession.
 * Since CMusEngSession is abstract, it is tested via object of its'
 * concrete descendant class CMusEngLiveSession. Only concrete
 * implementations are tested in this tester class.
 *
 */
NONSHARABLE_CLASS( UT_CMusPropertyWatch ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusPropertyWatch* NewL();
        static UT_CMusPropertyWatch* NewLC();

        /**
         * Destructor
         */
        ~UT_CMusPropertyWatch();

    private: // Constructors and destructors

        UT_CMusPropertyWatch();
        void ConstructL();

    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methods

	    void UT_CMusPropertyWatch_ReadIntPropertyLL();
	    void UT_CMusPropertyWatch_ReadDescPropertyLL();
	    void UT_CMusPropertyWatch_DoCancelL();
	    void UT_CMusPropertyWatch_RunLL();
	    void UT_CMusPropertyWatch_RunErrorL();


    private:    // Data


	    TMusPropertyObserverStub iPropertyObserver;
        CMusPropertyWatch* iPropertyWatch;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSPROPERYWATCH_H__

// End of file
