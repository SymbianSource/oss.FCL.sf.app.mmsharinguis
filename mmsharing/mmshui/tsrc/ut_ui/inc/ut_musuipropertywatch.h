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


#ifndef __UT_MUSUIPROPERYWATCH_H__
#define __UT_MUSUIPROPERYWATCH_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
//class CMusEngLiveSession;
//class CMusEngObserverStub;
class CMusUiPropertyWatch;
class CMusUiPropertyObserverAdapter;

//  CLASS DEFINITION
/**
 * Tester class for CMusEngSession.
 * Since CMusEngSession is abstract, it is tested via object of its'
 * concrete descendant class CMusEngLiveSession. Only concrete
 * implementations are tested in this tester class.
 *
 */
NONSHARABLE_CLASS( UT_CMusUiPropertyWatch ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusUiPropertyWatch* NewL();
        static UT_CMusUiPropertyWatch* NewLC();

        /**
         * Destructor
         */
        ~UT_CMusUiPropertyWatch();

    private: // Constructors and destructors

        UT_CMusUiPropertyWatch();
        void ConstructL();

    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methods

	    void UT_CMusUiPropertyWatch_ReadIntPropertyLL();
	    void UT_CMusUiPropertyWatch_ReadDescPropertyLL();
	    void UT_CMusUiPropertyWatch_DoCancelL();
	    void UT_CMusUiPropertyWatch_RunLL();
	    void UT_CMusUiPropertyWatch_RunErrorL();


    private:    // Data

        /*
        CMusEngLiveSession* iLiveSession;
        CMusEngObserverStub* iObserver;
        */
	    CMusUiPropertyObserverAdapter* iPropertyObserver;
        CMusUiPropertyWatch* iPropertyWatch;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGSESSION_H__

// End of file
