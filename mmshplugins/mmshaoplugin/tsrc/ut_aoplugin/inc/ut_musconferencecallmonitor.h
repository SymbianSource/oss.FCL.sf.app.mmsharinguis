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



#ifndef UT_MUSCONFERENCECALLMONITOR_H
#define UT_MUSCONFERENCECALLMONITOR_H

#include "muscallstatusmonitorobserver.h"

#include <etelmm.h>
#include <digia/eunit/ceunittestsuiteclass.h>


class CMusConferenceCallMonitor;

/**
 * Monitor, which observes status of current cs call
 * Observers will be notified, when call status is idle
 * i.e. call has ended.
 *
 * @lib ut_mushaoplugin.dll
 */
NONSHARABLE_CLASS( UT_CMusConferenceCallMonitor )
    : public CEUnitTestSuiteClass, public MMusCallStatusMonitorObserver
    {

public: // constructors and destructor

    /**
     * Destructor.
     */
    ~UT_CMusConferenceCallMonitor();

    /**
     * Two-phased constructor. Leaves on failure.
     * @return The constructed CMusAoCallMonitorTest object.
     */
    static UT_CMusConferenceCallMonitor* NewLC();

private:

    /**
     * C++ constructor.
     */
    UT_CMusConferenceCallMonitor();

    /**
     * Symbian 2nd-phase constructor.
     */
    void ConstructL();
    

private: // test MMusCallStatusMonitorObserver      
    
    virtual void StatusChanged();
    

public: 

    void UT_IsEqual();

    void UT_GetLastEventL();
    
    void UT_RunL();    

    void UT_DoCancel();

    void UT_RunError();


private:
    void SetupL();
    
    void Teardown();

private: // data

    CMusConferenceCallMonitor* iMonitor;
    
    EUNIT_DECLARE_TEST_TABLE; 
    
    TBool iStatusChanged;

    };

#endif // UT_MUSCONFERENCECALLMONITOR_H
