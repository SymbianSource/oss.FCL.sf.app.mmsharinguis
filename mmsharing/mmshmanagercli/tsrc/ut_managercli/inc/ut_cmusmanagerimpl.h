/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit tests for CMusManagerImpl class.
*
*/



#ifndef UT_CMUSMANAGERIMPL_H
#define UT_CMUSMANAGERIMPL_H


#include "musmanagerimpl.h"
#include <e32base.h>
#include <digia/eunit/ceunittestsuiteclass.h>


/**
 *  This class implements unit tests for CMusManagerImpl class.
 */
class UT_CMusManagerImpl : public CEUnitTestSuiteClass
    {
public:

    static UT_CMusManagerImpl* NewL();
    static UT_CMusManagerImpl* NewLC();
    virtual ~UT_CMusManagerImpl();

    /**
     * Unit test methods.
     */
    void UT_CMusManagerImpl_ExamineAvailabilityLL();
    void UT_CMusManagerImpl_AvailabilityLL();
    void UT_CMusManagerImpl_HandleSipRequestLL();
    void UT_CMusManagerImpl_StartApplicationLL();
    void UT_CMusManagerImpl_StopApplicationLL();
    void UT_CMusManagerImpl_HandleCommandLL();
private:

    UT_CMusManagerImpl();
    void ConstructL();

    /**
     * Sets up a test case by instantiating tested class.
     */
    void SetupL();

    /**
     * Finalizes a test case by freeing resources.
     */
    void Teardown();

    void UT_CMusManagerImpl_StartServerL();
    void UT_CMusManagerImpl_ServerStartedL();
    void UT_CMusManagerImpl_ConnectLL();
    void UT_CMusManagerImpl_InitSessionLL();
    void UT_CMusManagerImpl_QueryAvailabilityLL();

private: // data

    /**
     * Pointer to instance of tested class.
     * Own.
     */
    CMusManagerImpl* iImpl;

    EUNIT_DECLARE_TEST_TABLE;
    };


#endif // UT_CMUSMANAGERIMPL_H
