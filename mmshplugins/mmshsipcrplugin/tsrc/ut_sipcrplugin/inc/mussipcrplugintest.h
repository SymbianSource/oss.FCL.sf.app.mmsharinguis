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



#include <digia/eunit/ceunittestsuiteclass.h>

class CMusSipCrPlugin;


/**
 * Main plugin class
 *
 * @lib ut_musaoplugin.dll
 */
class CMusSipCrPluginTest 
    : public CEUnitTestSuiteClass
    {
    
public:

    /**
     * Symbian two-phase constructor.
     * @return New CMusSipCrPluginTest instance
     */
    static CMusSipCrPluginTest* NewL();

    /**
     * C++ destructor.
     */
    virtual ~CMusSipCrPluginTest();

private: // constructors

    /**
     * C++ constructor.
     */
    CMusSipCrPluginTest();

    /**
     * Symbian second-phase constructor.
     */
    void ConstructL();


public: // Test functions

    void UT_ConstructL();
    
    void UT_ChannelL();
   
    void UT_ConnectL();
    
    void UT_Capabilities();

    void UT_ImplementationGroupProxy();

    void UT_CheckForSendRecvAttributeL();

public:
    void SetupL();
    void Teardown();
    

private: // data

    CMusSipCrPlugin* iPlugin;
    
    EUNIT_DECLARE_TEST_TABLE; 

    };

#endif // MUSAOPLUGINTEST_H
