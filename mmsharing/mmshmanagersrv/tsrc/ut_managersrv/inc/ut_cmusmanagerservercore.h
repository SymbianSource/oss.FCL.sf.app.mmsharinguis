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
* Description:  Unit tests for CMusManagerServerCore class.
*
*/


#ifndef UT_CMUSMANAGERSERVERCORE_H
#define UT_CMUSMANAGERSERVERCORE_H

#include "musmanagerservercoreobserver.h"
#include "musMonitorAvailabilityObserver.h"
#include <digia/eunit/ceunittestsuiteclass.h>

class CMusManagerServerCore;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

/**
 *  Unit test class for CMusManagerServerCore.
 *  Implements unit tests for CMusManagerServerCore.
 *
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( UT_CMusManagerServerCore ) : public CEUnitTestSuiteClass,
    MMusMonitorAvailabilityObserver,
    MMusManagerServerCoreObserver
    {
public:

    static UT_CMusManagerServerCore* NewL();

    static UT_CMusManagerServerCore* NewLC();

    /**
     * Destructor.
     */
    ~UT_CMusManagerServerCore();

// from baseclass MMusManagerServerCoreObserver.

    /**
     * From MMusManagerServerCoreObserver.
     * Stops server.
     *
     * @since S60 v3.2
     */
    virtual void StopServer();

    
    virtual TUint SessionCount();

// from baseclass MMusMonitorAvailabilityObserver.

    /**
     * From MMusMonitorAvailabilityObserver.
     * Informs of a change in multimediasharing availability.
     *
     * @since S60 v3.2
     * @param aStatus New state of availability.
     */
    virtual void AvailabilityChangedL( MultimediaSharing::TMusAvailabilityStatus aStatus );
    
    virtual void RequestComplete();
    					
private:

    UT_CMusManagerServerCore();

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
    void UT_CMusManagerServerCore_NewLL();
    void UT_CMusManagerServerCore_NewLCL();
    void UT_CMusManagerServerCore_AvailabilityQueryLL();
    void UT_CMusManagerServerCore_InvestigateAvailabilityLL();
    void UT_CMusManagerServerCore_InvitationReceivedLL();
    void UT_CMusManagerServerCore_OptionsReceivedLL();
    void UT_CMusManagerServerCore_StartMultimediaSharingLL();
    void UT_CMusManagerServerCore_StopMultimediaSharingLL();
    void UT_CMusManagerServerCore_EventNoSessionsL();
    void UT_CMusManagerServerCore_PluginStoppedL();
    void UT_CMusManagerServerCore_AvailabilityChangedLL();
    void UT_CMusManagerServerCore_MonitorAvailabilityLL();
    void UT_CMusManagerServerCore_RegisterObserverL();
    void UT_CMusManagerServerCore_IsMusEnginePluginL();


private: // data

    /**
     * Tested class.
     * Own.
     */
    CMusManagerServerCore* iCore;

    /**
     * Tells if observer function is called.
     */
    TBool iStopServerCalled;

    /**
     * Tells if observer function is called.
     */
    TBool iAvailabilityChangedLCalled;

    /**
     * Stub count of sessions for MMusManagerServerCoreObserver::SessionCount.
     */
    TUint iSessionCount;

    EUNIT_DECLARE_TEST_TABLE;
    };

#endif // UT_CMUSMANAGERSERVERCORE_H
