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


#ifndef __MUSAVANETWORKAVAILABILITY_H__
#define __MUSAVANETWORKAVAILABILITY_H__

#include "musavaavailability.h"
#include "muscallmonitorobserver.h"
#include <sipprofileregistryobserver.h>

#include <e32base.h>

class MMusAvaAvailabilityObserver;
class MMusAvaObserver;
class CMusAvaSharedObject;
class CMusAvaCallEventMonitor;
class CMusAvaSettingsImp;

/**
 *  Implemets register vailability.
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */



class CMusAvaNetworkAvailability : public CMusAvaAvailability,
                                   public MMusCallMonitorObserver
	{
public:
	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @param aModules is data container
     * @return Returns pointer to CMusAvaInterface object
     */
	static CMusAvaNetworkAvailability* NewL(
        MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aSettings );


	static CMusAvaNetworkAvailability* NewLC(
        MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aSettings );


	/**
     * Destructor
     */
	~CMusAvaNetworkAvailability();

protected:

	/**
     * Constructor
     *
     * @param aObserver Pointer to observer intercase
     * @param aName availability name
     * @param aModules is data container
     */

	CMusAvaNetworkAvailability( MMusAvaAvailabilityObserver& aObserver,
                                CMusAvaSettingsImp& aSettings );

	void ConstructL();


// from base class CMusAvaAvailability

public:

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
     *
     * @since S60 v3.2
     * @return KErrNone if stop operation was successfully executed;
     *         system wide error otherwise
     */
    virtual void Stop();

    /**
     * From CMusAvaAvailability.
     * Returns availability name.
     *
     * @return Name of *this* availability.
     */
    virtual MMusAvaObserver::TAvailabilityName Name();

public: // from MMusCallMonitorObserver

    /**
     * Call connected
     *
     * @since  S60 v3.2
     * @return
     */
    void CallConnectedL( const TDesC& aTelNumber, TBool aIsSipUri );

     /**
     * Call on hold
     *
     * @since  S60 v3.2
     * @return
     */
    virtual void CallHoldL( const TDesC& aTelNumber, TBool aIsSipUri );

    /**
     * Call disconnected
     *
     * @since  S60 v3.2
     * @return
     */
    virtual void NoActiveCallL();

    /**
     * Call on hold
     *
     * @since  S60 v3.2
     * @return
     */
    virtual void ConferenceCallL();

private:

	void SetRemoteHostL( const TDesC& aTelNumber, TBool aIsSipUri );

private: // data

   /**
    *
    */
    CMusAvaCallEventMonitor* iPhoneStatus;

   /**
    * Pointer to CMusAvaSharedObject object
    */
    CMusAvaSharedObject* iSharedObj;

   /**
    * Reference to availability settings object.
    */
    CMusAvaSettingsImp& iSettings;
    
    MUS_UNITTEST( UT_CMusAvaNetworkAvailability )
    };


#endif // __MUSAVANETWORKAVAILABILITY_H__
