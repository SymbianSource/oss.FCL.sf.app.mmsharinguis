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



#ifndef UT_MUSAOCALLEVENTMONITOR_H
#define UT_MUSAOCALLEVENTMONITOR_H

#include "muscallmonitorobserver.h"

#include <etelmm.h>
#include <digia/eunit/ceunittestsuiteclass.h>


class CMusAoCallEventMonitor;

/**
 * Monitor, which observes status of current cs call
 * Observers will be notified, when call status is idle
 * i.e. call has ended.
 *
 * @lib ut_mushaoplugin.dll
 */
NONSHARABLE_CLASS( UT_CMusAoCallEventMonitor )
    : public CEUnitTestSuiteClass, public MMusCallMonitorObserver
    {

public: // constructors and destructor

    /**
     * Destructor.
     */
    ~UT_CMusAoCallEventMonitor();

    /**
     * Two-phased constructor. Leaves on failure.
     * @return The constructed CMusAoCallMonitorTest object.
     */
    static UT_CMusAoCallEventMonitor* NewLC();

private:

    /**
     * C++ constructor.
     */
    UT_CMusAoCallEventMonitor();

    /**
     * Symbian 2nd-phase constructor.
     */
    void ConstructL();
    

private: // test MMusCallMonitorObserver      
    
    virtual void CallConnectedL( const TDesC& aTelNumber );
    
    virtual void CallHoldL( const TDesC& aTelNumber );
    
    virtual void ConferenceCallL();
    
    virtual void NoActiveCallL();
    

public: 

    void UT_ConnectedCallCountLL();
    
    void UT_RunLL();    

    void UT_ExamineEventLL();

private:

    void SetupL();
    
    void Teardown();
    
    void ResetObserverFlags();

private: // data

    CMusAoCallEventMonitor* iMonitor;
    
    EUNIT_DECLARE_TEST_TABLE; 
    
    TBool iCallConnected;
    
    TBool iCallHold;
    
    TBool iConferenceCall;
    
    TBool iNoActiveCall;
    
    };

#endif // UT_MUSAOCALLEVENTMONITOR_H
