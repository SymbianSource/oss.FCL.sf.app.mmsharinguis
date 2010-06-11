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
* Description:  Unit tests for CMusManager class.
*
*/



#ifndef UT_CMUSMANAGER_H
#define UT_CMUSMANAGER_H


#include "musmanagercommon.h"
#include <e32base.h>
#include <digia/eunit/ceunittestsuiteclass.h>
#include <musavailabilityobserver.h>

#pragma warn_illtokenpasting off

class CMusManager;


/**
 *  UT_CMusManager implements unit tests for CMusManager class.
 */
class UT_CMusManager : public CEUnitTestSuiteClass,
                       public MMusAvailabilityObserver
    {
public:

    static UT_CMusManager* NewL();
    static UT_CMusManager* NewLC();

   /**
    * Destructor.
    */
    virtual ~UT_CMusManager();

    /**
     * Sets up a test case by instantiating tested class.
     */
    void SetupL();
    void SetupDummyL();

    /**
     * Finalizes a test case by freeing resources.
     */
    void Teardown();
    void TeardownDummy();

    /**
     * Unit test methods.
     */
    void UT_CMusManager_ExamineAvailabilityLL();
    void UT_CMusManager_AvailabilityLL();
    void UT_CMusManager_HandleSipRequestLL();
    void UT_CMusManager_HandleCommandLL();
    void UT_CMusManager_StartApplicationLL();
    void UT_CMusManager_StopApplicationLL();
    void UT_CMusManager_NewLL();
    
public: // from MMusAvailabilityObserver
    void MusAvailabilityChangedL( 
                MultimediaSharing::TMusAvailabilityStatus aAvailabilityStatus );        

private:

    UT_CMusManager();
    void ConstructL();

private: // data

    /**
     * Pointer to instance of tested class.
     * Own.
     */
    CMusManager* iImpl;

    EUNIT_DECLARE_TEST_TABLE;

    };


#endif // UT_CMUSMANAGER_H
