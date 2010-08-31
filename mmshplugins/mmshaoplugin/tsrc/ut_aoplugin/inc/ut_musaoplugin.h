/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Main plugin class
*
*/


#ifndef MUSAOPLUGINTEST_H
#define MUSAOPLUGINTEST_H


#include <e32base.h>    // CBase
#include <digia/eunit/ceunittestsuiteclass.h>

class CMusAoPlugin;

/**
 * Main plugin class
 *
 * @lib ut_musaoplugin.dll
 */
NONSHARABLE_CLASS( UT_CMusAoPlugin ) : public CEUnitTestSuiteClass
    {
    
public:

    /**
     * Symbian two-phase constructor.
     * @return New UT_CMusAoPlugin instance
     */
    static UT_CMusAoPlugin* NewLC();

    /**
     * C++ destructor.
     */
    virtual ~UT_CMusAoPlugin();
  

private: // constructors

    /**
     * C++ constructor.
     */
    UT_CMusAoPlugin();

    /**
     * Symbian second-phase constructor.
     */
    void ConstructL();

public: // setup functions

    void SetupL();
    
    void Teardown();

public: // test functions 

    void DefinePropertiesLL();
    void DefinePropertyL_IntPropertyL();
    void DefinePropertyL_DesCPropertyL();
    void DeletePropertiesL();
    
    void UT_HandleServerCommandLL();
    void UT_StartMusClientL();
    void UT_StopMusClientL();
    void UT_MusCallStateChangedL();
       
private: // data

    CMusAoPlugin* iPlugin;
    
    EUNIT_DECLARE_TEST_TABLE; 

    };

#endif // MUSAOPLUGINTEST_H
