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
* Description:  Unit tests for CMusAvailabilityManager class.
*
*/


#ifndef UT_CMUSAVAILABILITYPLUGINMANAGER_H
#define UT_CMUSAVAILABILITYPLUGINMANAGER_H

#include "musavailabilitypluginmanagerobserver.h"
#include <digia/eunit/ceunittestsuiteclass.h>
#include <e32def.h>

#pragma warn_illtokenpasting off

class CMusAvailabilityPluginManager;
class CMusApplicationManager;

#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

/**
 *  Unit test class for CMusAvailabilityPluginManager.
 *  Implements unit tests for CMusAvailabilityPluginManager.
 *
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( UT_CMusAvailabilityPluginManager )
    : public CEUnitTestSuiteClass, MMusAvailabilityPluginManagerObserver
    {
public:

    static UT_CMusAvailabilityPluginManager* NewL();

    static UT_CMusAvailabilityPluginManager* NewLC();

    /**
     * Destructor.
     */
    ~UT_CMusAvailabilityPluginManager();

// from baseclass MMusAvailabilityPluginManagerObserver.

    /**
     * From MMusAvailabilityPluginManagerObserver.
     * Informs that observed plugin is stopped.
     *
     * @since S60 v3.2
     */
    virtual void PluginStopped();

    /**
     * From MMusAvailabilityPluginManagerObserver.
     * Informs a change in multimedia sharing availability.
     *
     * @param aAvailability Represents new state of availability.
     * @since S60 v3.2
     */
    virtual void AvailabilityChangedL( MultimediaSharing::TMusAvailabilityStatus aAvailability );
 

    /**
     * From MMusAvailabilityPluginManagerObserver.     
     *     
     * @since S60 v3.2
     */
    virtual void StartSharingWithUseCaseL( MultimediaSharing::TMusUseCase aUseCase );

private:

    UT_CMusAvailabilityPluginManager();

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
    void UT_CMusAvailabilityPluginManager_NewLL();
    void UT_CMusAvailabilityPluginManager_NewLCL();
    void UT_CMusAvailabilityPluginManager_InvestigateAvailabilityLL();
    void UT_CMusAvailabilityPluginManager_InvitationReceivedLL();
    void UT_CMusAvailabilityPluginManager_OptionsReceivedLL();
    void UT_CMusAvailabilityPluginManager_StopPluginLL();
    void UT_CMusAvailabilityPluginManager_SessionParametersL();
    void UT_CMusAvailabilityPluginManager_OptionAllowedL();
    void UT_CMusAvailabilityPluginManager_ApplicationStateL();
    void UT_CMusAvailabilityPluginManager_AvailabilityL();
    void UT_CMusAvailabilityPluginManager_StartLiveSharingLL();
    void UT_CMusAvailabilityPluginManager_MapAvailabilitiesL();
    void UT_CMusAvailabilityPluginManager_UpdateSessionParametersLL();
    void UT_CMusAvailabilityPluginManager_SipAddressesLCL();
    void UT_CMusAvailabilityPluginManager_TelNumberLCL();
    void UT_CMusAvailabilityPluginManager_ContactNameLCL();
    void UT_CMusAvailabilityPluginManager_IndicateAvailabilityLL();
    void UT_CMusAvailabilityPluginManager_PrepareForReceivedInviteLL();

private: // data

    /**
     * Instance of tested class.
     * Own.
     */
    CMusAvailabilityPluginManager* iManager;

    /**
     * Tells if observer method
     * MMusAvailabilityPluginManagerObserver::AvailabilityChangedL has been
     * called.
     */
    TBool iCalledAvailabilityChangedL;

    /**
     * Tells if observer method
     * MMusAvailabilityPluginManagerObserver::PluginStoppedL has been called.
     */
    TBool iCalledPluginStoppedL;

    /**
     * Tells if observer method
     * MMusAvailabilityPluginManagerObserver::StartLiveSharingL has been called.
     */
    TBool iCalledAStartLiveSharing;
    
    /**
     * Application manager instance. Owned.
     */
    CMusApplicationManager* iAppManager;

    EUNIT_DECLARE_TEST_TABLE;

    };

#endif // UT_CMUSAVAILABILITYPLUGINMANAGER_H
