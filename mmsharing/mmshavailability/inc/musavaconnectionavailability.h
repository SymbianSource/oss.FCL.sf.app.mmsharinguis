/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provide interface for the client requestin network availability.
*
*/



#ifndef __MUSAVACONNECTIONAVAILABILITY_H__
#define __MUSAVACONNECTIONAVAILABILITY_H__


#include "musavaavailability.h"
#include "musavatelephonystatusobserver.h"
#include "musavaconnectionmonitorobserver.h"
#include "musunittesting.h"

#include <e32base.h>
#include <etelmm.h>

class MMusAvaObserver;
class CMusAvaSharedObject;
class CMusAvaConnectionMonitor;
class CConnMonEventBase;
class CMusAvaNetworkRegistrationStatus;
class CMusAvaNetworkModeStatus;
class TAvailabilityStatus;
class CMusAvaSettingsImp;
class TMobilePhoneRegistrationStatus;


/**
 *  Implemets register vailability.
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaConnectionAvailability : public CMusAvaAvailability,
                                      public MMusAvaTelephonyStatusObserver,
                                      public MMusAvaConnectionMonitorObserver
    {
public:
    
    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @param aName availability name
     * @param aModules is data container
     * @return Returns pointer to CMusAvaInterface object
     */
    static CMusAvaConnectionAvailability* NewL( MMusAvaAvailabilityObserver& aObserver,
                                                CMusAvaSettingsImp& aSettings );

    static CMusAvaConnectionAvailability* NewLC( MMusAvaAvailabilityObserver& aObserver,
                                                 CMusAvaSettingsImp& aSettings );

    /**
     * Destructor
     */
    ~CMusAvaConnectionAvailability();

protected:

    CMusAvaConnectionAvailability( MMusAvaAvailabilityObserver& aObserver,
                                   CMusAvaSettingsImp& aSettings );

    void ConstructL();


public:// from base class CMusAvaAvailability


    /**
     * Executes for the availability.
     *
     * @since S60 v3.2
     * @return KErrNone if ExecutePatternL operation was
     *         successfully executed; system wide error otherwise
     */
    virtual void DoExecuteL();

     /**
     * Stop the execution.
     */
    virtual void Stop();

    /**
     * From CMusAvaAvailability.
     * Returns availability name.
     *
     * @return Name of *this* availability.
     */
    virtual MMusAvaObserver::TAvailabilityName Name();

public: // from MMusAvaTelephonyStatusObserver

    /**
    * Current Network Mode Status
    *
    * @since  S60 v3.2
    * @return
    */
    void PhoneNetworkModeStatus(
            RMobilePhone::TMobilePhoneNetworkMode aStatus );
    
    /**
     * Current Network Registration Status
     *
     * @since  S60 v3.2
     * @return
     */
    void NetworkRegistrationStatus(
            RMobilePhone::TMobilePhoneRegistrationStatus aRegStatus );

public: //from MMusAvaConnectionMonitorObserver

    /**
    * Event from connection monitor server
    *
    */
    void EventL(const CConnMonEventBase& aConnMonEvent);

private:

    /**
    * Check needs to have manual activation 
    *
    * @since  S60 v3.2
    * @return TAvailabilityStatus is returned
    */
    MMusAvaObserver::TAvailabilityStatus ManualActivationL();

    /**
     * Current test network registration agains the settings
     *
     * @since  S60 v3.2
     * @return TAvailabilityStatus is returned
     */
    MMusAvaObserver::TAvailabilityStatus NetworkRegistrationAndSettingsL();

    static TBool OperatorVariant();
    
private: // data

    /**
     * Pointer to CMusAvaNetworkRegistrationStatus object
     */
    CMusAvaNetworkRegistrationStatus* iNetworkRegistrationStatus;

    /**
     * Pointer to CMusAvaNetworkModeStatus object
     */
    CMusAvaNetworkModeStatus* iNetworkModeStatus;

    /**
    * Pointer to CMusAvaConnectionMonitor object
    */
    CMusAvaConnectionMonitor* iConnectionMonitor;

     /**
     * Pointer to CMusAvaSharedObject
     */
    CMusAvaSharedObject* iSharedObj;

     /**
     * Connection ID
     */
    TUint iConnectionID;
    
     /**
     * Reference to availability settings 
     */
    CMusAvaSettingsImp& iSettings;

    
    MUS_UNITTEST( UT_CMusAvaConnectionAvailability )
    };


#endif // __MUSAVACONNECTIONAVAILABILITY_H__
