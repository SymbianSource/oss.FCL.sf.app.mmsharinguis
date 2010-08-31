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



#ifndef UT_CMUSINDICATORAPI_H
#define UT_CMUSINDICATORAPI_H


#include "musindicatorobserver.h"
#include <digia/eunit/ceunittestsuiteclass.h>
#include <e32def.h>


#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif


class CMusIndicatorApi;


/**
 *  Unit test class for CMusIndicatorApi class.
 *  Implements unit tests for CMusIndicatorApi class.
 */
NONSHARABLE_CLASS( UT_CMusIndicatorApi ) : public
    CEUnitTestSuiteClass,
    MMusIndicatorObserver
    {
public:

    static UT_CMusIndicatorApi* NewL();
    static UT_CMusIndicatorApi* NewLC();

    /**
     * Destructor.
     */
    ~UT_CMusIndicatorApi();

private:

    UT_CMusIndicatorApi();

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
     * From MMusIndicatorObserver.
     * Instructs to start live video sharing.
     */
    virtual void StartLiveSharingL();

    /**
     * Actual unit test methods.
     */
    void UT_CMusIndicatorApi_NewLL();
    void UT_CMusIndicatorApi_ConfirmationQueryLL();
    void UT_CMusIndicatorApi_IndicateAvailabilityLL();
    void UT_CMusIndicatorApi_RunLL();
    void UT_CMusIndicatorApi_DoCancelL();
    void UT_CMusIndicatorApi_RunErrorL();
    void UT_CMusIndicatorApi_PlaySoundCompleteL();
    void UT_CMusIndicatorApi_PlayToneLL();
    void UT_CMusIndicatorApi_NoteTextLCL();
    void UT_CMusIndicatorApi_ShowLiveSharingQueryLL();
    void UT_CMusIndicatorApi_LiveSharingQueryTimeoutL();
    
    
private: // data

    /**
     * Tested class.
     * Own.
     */
    CMusIndicatorApi* iIndicatorApi;

    /**
     * Tells if observer method MMusIndicatorObserver::StartLiveSharingL has
     * been called.
     */    
    TBool iStartLiveSharingLCalled;

    EUNIT_DECLARE_TEST_TABLE;

    };

#endif // UT_CMUSINDICATORAPI_H
