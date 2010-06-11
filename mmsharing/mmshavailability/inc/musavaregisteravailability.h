/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*  Version     : %version: 15.1.5 % << Don't touch! Updated by Synergy at check-out.
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Provide interface for the client requestin register availability.
*
*/



#ifndef __MUSAVAREGISTERAVAILABILITY_H__
#define __MUSAVAREGISTERAVAILABILITY_H___


#include "musunittesting.h"
#include "musavaavailability.h"
#include "musavasipconnectionadapter.h"
#include "musavasipprofileregistryadapter.h"
#include <sipprofileregistryobserver.h>
#include <e32base.h>


class CMusAvaSettingsImp;
class CSIPProfileRegistry;
class CSIPProfile;
class MMusAvaObserver;
class CMusAvaSharedObject;
class CSIPProfile;
class CSIPProfileRegistry;
class CSIPConnection;


/**
 *  Implemets register vailability.
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaRegisterAvailability : public CMusAvaAvailability,
                                    public MMusAvaSipProfileRegistryAdapter
	{
public:

	/**
     * Two-phased constructor
     *
     * @param aObserver Pointer to observer intercase
     * @param aModules is data container
     * @return Returns pointer to CMusAvaInterface object
     */
	static CMusAvaRegisterAvailability* NewL(
        MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aSettings );


	static CMusAvaRegisterAvailability* NewLC(
	    MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aSettings );

    /**
     * Destructor
     */

    ~CMusAvaRegisterAvailability();

    protected:

    /**
     * Constructor
     *
     * @param aObserver Pointer to observer intercase
     * @param aName availability name
     * @param aModules is data container
     * @return
     */
    CMusAvaRegisterAvailability(
        MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aSettings );

    void ConstructL();

// from base class CMusAvaAvailability

public:

   /**
    * Executes for the availability.
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

// from base class MSIPProfileRegistryObserver

public:

   /**
    * An event related to SIP Profile has accorred
    * @param aProfileId a profile Id
    * @param aEvent an occurred event
    **/
    virtual void ProfileRegistryEventOccurred(
        TUint32 aProfileId,
        MSIPProfileRegistryObserver::TEvent aEvent );

   /**
    * An asynchronous error has occurred related to SIP profile
    * Event is send to those observers, who have the
    * corresponding profile instantiated.
    * @param aProfileId the id of failed profile
    * @param aError an occurred error
    */
    virtual void ProfileRegistryErrorOccurred(
        TUint32 aProfileId,
        TInt aError );

// from base class MMusAvaSipConnectionAdapter

public:

   /**
	* Connection state has changed.
    * If connection state has changed to EInactive or EUnavailable,
	* SIP stack has removed all stand-alone SIP refreshes, registrations
	* and dialog associations that client requested to refresh. Client may
	* re-issue refresh requests (stand-alone, registration or dialog
	* association related) when connection becomes EActive again.
	* SIP stack also terminates all pending SIP client transactions and no
    * errors are reported back to the client about the terminated
    * transactions nor about removed refreshes in order to avoid event
    * flood.
	*
	* @param aState indicates the current connection state
	*/
	TInt ConnectionStateChanged (CSIPConnection::TState aState );


private: // Helpers

   /**
	* Creates proposal sip address and send event.
	*
	*/
    void CreateSipAddressProposalL();
    
    /**
	* Enables SIP Register.
	*
	*/
    void EnableRegisterL();

   /**
	* Initialize SIP Register request.
	*
	* @return ETrue if Register request was initialized
    *         successfully; EFalse otherwise
	*/
    TBool InitializeRegister();

   /**
	* Send SIP Register request.
	*
	* @return ETrue if Register operation was
    *         successfully executed; EFalse otherwise
	*/
    TBool RegisterL();
    
    /**
	* Update SIP profile for general data.
	*
	* @return ETrue if SIP Profile was found; 
	* EFalse otherwise
	*/
    TBool UpdateSIPProfile();
    
     /**
	* Current SIP profile status.
	*
	* @return ETrue if SIP Profile is registred; 
	* EFalse otherwise
	*/
    TBool RegistrationStatusL();

private: // Data

    /**
     * Pointer to CMusAvaSharedObject object
     */
     CMusAvaSharedObject* iSharedObj;

    /**
     * Pointer to CSIPProfileRegistry object
     */
     CSIPProfileRegistry* iRegistry;

	/**
     * Reference to CMusAvaSettingsImp object
     */
     CMusAvaSettingsImp& iSettings;

    /**
     * Pointer to CSIPConnection object
     */
     CSIPConnection* iConnection;


    MUS_UNITTEST( UT_CMusAvaRegisterAvailability )
    };


#endif // __MUSAVAREGISTERAVAILABILITY_H__