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
* Description:  Unit tests for CMusApplicationManager class.
*
*/


#ifndef UT_CMUSAPPLICATIONMANAGER_H
#define UT_CMUSAPPLICATIONMANAGER_H

#include <digia/eunit/ceunittestsuiteclass.h>

class CMusApplicationManager;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

/**
 *  Unit test class for CMusApplicationManager class.
 *  Implements unit testing of CMusApplicationManager class.
 *
  *  @since S60 v3.2
 */
NONSHARABLE_CLASS( UT_CMusApplicationManager ) : public CEUnitTestSuiteClass
    {
public:

    static UT_CMusApplicationManager* NewL();

    static UT_CMusApplicationManager* NewLC();

    /**
    * Destructor.
    */
    ~UT_CMusApplicationManager();

private:

    UT_CMusApplicationManager();

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
    void UT_CMusApplicationManager_NewLL();
    void UT_CMusApplicationManager_NewLCL();
    void UT_CMusApplicationManager_ApplicationRunningL();
    void UT_CMusApplicationManager_StartApplicationLL();
    void UT_CMusApplicationManager_StopApplicationLL();
    void UT_CMusApplicationManager_ShowApplicationLL();
    void UT_CMusApplicationManager_WriteSessionPropertiesL();
    void UT_CMusApplicationManager_SetPropertyLL();
    void UT_CMusApplicationManager_SetPropertyL2L();
    void UT_CMusApplicationManager_MapAvailabilitiesL();
    void UT_CMusApplicationManager_SetStatusLL();
    void UT_CMusApplicationManager_CallProviderLL();
    void UT_CMusApplicationManager_ResolvePluginNameLL();
    void UT_CMusApplicationManager_CreateCommandLineArgsLCL();

private: // data

    /**
     * Instance of tested class.
     * Own.
     */
    CMusApplicationManager* iManager;

    EUNIT_DECLARE_TEST_TABLE;

    };

#endif // UT_CMUSAPPLICATIONMANAGER_H
