/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit tests for CMusAvailabilityManager class.
*
*/


#ifndef UT_CMUSMANAGERSERVERCLOSETIMER_H
#define UT_CMUSMANAGERSERVERCLOSETIMER_H

#include <digia/eunit/ceunittestsuiteclass.h>

class CMusManagerServerCloseTimer;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

/**
 *  Unit test class for CMusManagerServerCloseTimer.
 *  Implements unit tests for CMusManagerServerCloseTimer.
 *
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( UT_CMusManagerServerCloseTimer )
    : public CEUnitTestSuiteClass
    {
public:

    static UT_CMusManagerServerCloseTimer* NewL();

    static UT_CMusManagerServerCloseTimer* NewLC();

    /**
     * Destructor.
     */
    ~UT_CMusManagerServerCloseTimer();

private:

    UT_CMusManagerServerCloseTimer();

    void ConstructL();

    /**
    * Sets up a test case.
    */
    void SetupL();

    /**
    * Tears down a test case.
    */
    void Teardown();

    /**
    * Invidual unit test methods.
    */
    void UT_CMusManagerServerCloseTimer_NewLL();
    void UT_CMusManagerServerCloseTimer_NewLCL();
    void UT_CMusManagerServerCloseTimer_StopActiveSchedulerAfterL();
    void UT_CMusManagerServerCloseTimer_DoCancelL();

private: // data

    /**
     * Instance of tested class.
     * Own.
     */
    CMusManagerServerCloseTimer* iTimer;

    EUNIT_DECLARE_TEST_TABLE;

    };

#endif // UT_CMUSMANAGERSERVERCLOSETIMER_H
