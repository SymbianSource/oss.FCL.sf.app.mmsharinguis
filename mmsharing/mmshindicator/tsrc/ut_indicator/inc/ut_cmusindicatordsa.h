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
* Description:  Unit tests for CMusIndicatorApi class.
*
*/



#ifndef UT_CMUSINDICATORDSA_H
#define UT_CMUSINDICATORDSA_H


#include <digia/eunit/ceunittestsuiteclass.h>
#include <e32def.h>


#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif


class CMusIndicatorDsa;


/**
 *  Unit test class for CMusIndicatorApi class.
 *  Implements unit tests for CMusIndicatorApi class.
 */
NONSHARABLE_CLASS( UT_CMusIndicatorDsa ) : public CEUnitTestSuiteClass
    {
public:

    static UT_CMusIndicatorDsa* NewL();
    static UT_CMusIndicatorDsa* NewLC();

    /**
     * Destructor.
     */
    ~UT_CMusIndicatorDsa();

private:

    UT_CMusIndicatorDsa();

    void ConstructL();

    /**
     * Sets up a new test.
     */
    void SetupL();

    /**
     * Tears down a test.
     */
    void Teardown();

    /**
     * Actual unit test methods.
     */
    void UT_CMusIndicatorDsa_NewLL();
    void UT_CMusIndicatorDsa_DrawL();
    void UT_CMusIndicatorDsa_LoadImageLL();
    void UT_CMusIndicatorDsa_LoadResourceStringLL();
    void UT_CMusIndicatorDsa_InitializeL();
    void UT_CMusIndicatorDsa_TryActivateL();
    void UT_CMusIndicatorDsa_StartPeriodicTimerL();
    void UT_CMusIndicatorDsa_StopPeriodicTimerL();
    void UT_CMusIndicatorDsa_HandleResourceChangeL();
    void UT_CMusIndicatorDsa_DoHandleResourceChangeL();
    
    
    /**
    * Helpers
    */
    void FakeVisibilityL();

private: // data

    /**
     * Tested class.
     * Own.
     */
    CMusIndicatorDsa* iIndicatorDsa;

    EUNIT_DECLARE_TEST_TABLE;

    };

#endif // UT_CMUSINDICATORDSA_H
