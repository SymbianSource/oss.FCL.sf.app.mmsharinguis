/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Monitor that observes the status of the current CS call
*
*/



#ifndef MUSLINEEVENTMONITORTEST_H
#define MUSLINEEVENTMONITORTEST_H

//#include <etelmm.h>
#include <CEUnitTestSuiteClass.h>

//#include "ut_musaocallmonitor.h"
#include "muscallstatusmonitorobserver.h"

class CMusLineEventMonitor;

/**
 * Monitor, which observes status of current cs call
 * Observers will be notified, when call status is idle
 * i.e. call has ended.
 *
 * @lib ut_mushaoplugin.dll
 */
NONSHARABLE_CLASS( UT_CMusLineEventMonitor )
    : public CEUnitTestSuiteClass, public MMusCallStatusMonitorObserver    
    {

public: // constructors and destructor

    /**
     * Destructor.
     */
    ~UT_CMusLineEventMonitor();

    /**
     * Two-phased constructor. Leaves on failure.
     * @return The constructed UT_CMusAoCallMonitorTest object.
     */
    static UT_CMusLineEventMonitor* NewLC();

private:

    /**
     * C++ constructor.
     */
    UT_CMusLineEventMonitor();

    /**
     * Symbian 2nd-phase constructor.
     */
    void ConstructL();
    

private: // test MMusCallStatusMonitorObserver    

    virtual void StatusChanged();

public: // Unit test functions

    void UT_GetLineL();
    
    void UT_RemoteHoldCountL();

    void UT_RunLL();    

    void UT_DoCancelL();

    void UT_RunErrorL();
    
    void UT_AddCallEventMonitorLL();
    
    void UT_RemoveCallEventMonitorLL();

private:
    void SetupL();
    
    void Teardown();

private: // data

    CMusLineEventMonitor* iMonitor;
    
    EUNIT_DECLARE_TEST_TABLE; 

    };

#endif // MUSLINEEVENTMONITORTEST_H
