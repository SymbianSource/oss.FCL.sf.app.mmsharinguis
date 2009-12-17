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


#ifndef __MUSAVAINVITEHANDLER_H__
#define __MUSAVAINVITEHANDLER_H__

#include "musavaavailability.h"
#include "musavaobserver.h"
#include "musavainviteresponderobserver.h"
#include "musavasipconnectionadapter.h"
#include "musavasipadapter.h"
#include "musunittesting.h"

#include <e32base.h>

class CSIPServerTransaction;
class CMusAvaInviteResponder;
class CMusAvaSharedObject;
class MMusAvaAvailabilityObserver;
class CMusAvaSettingsImp;

/**
 *  Implements handling of non-desired SIP invites.
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaInviteHandler : public CMusAvaAvailability,
                             public MMusAvaInviteResponderObserver,
                             public MMusAvaSipConnectionAdapter,
                             public MMusAvaSipAdapter
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

	static CMusAvaInviteHandler* NewL(
        MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aSettings );

	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @param aModules is data container
     * @return Returns pointer to CMusAvaInterface object
     */

	static CMusAvaInviteHandler* NewLC(
        MMusAvaAvailabilityObserver& aObserver,
        CMusAvaSettingsImp& aSettings );

	/**
     * Destructor
     */
	~CMusAvaInviteHandler();

public:

	/**
	* A SIP request outside a dialog has been received from the network.
    *
    * @pre aTransaction != 0
	* @param aTransaction SIP server transaction. The ownership is
    *   transferred.
    */
	virtual TInt IncomingRequest(
	    CSIPServerTransaction* aTransaction );

	/**
	* A SIP request within a dialog has been received from the network.
	* The client must resolve the actual dialog association to which
	* this request belongs.
	*
    * @pre aTransaction != 0
	* @param aTransaction SIP server transaction. The ownership is
    *   transferred.
	* @param aDialog the dialog that this transaction belongs to.
	*/
/*	virtual TInt IncomingRequest (CSIPServerTransaction* aTransaction,
				                  CSIPDialog& aDialog);
*/

public:

    /**
    * A SIP request has been received from the network.
    * This function is called when the SIP request was received using such
    * an IAP, for which the application has not created a CSIPConnection
    * object.
    * @pre aTransaction != 0
    * @param aIapId The IapId from which the SIP request was received.
    * @param aTransaction contains local address, remote address of a SIP
    *        message, as well as optional SIP message method, headers and
    *        body. The ownership is transferred.
    */
    TInt IncomingRequest(
        TUint32 aIapId,
        CSIPServerTransaction* aTransaction );

    /**
    * The received SIP request time-outed and it is invalid i.e. cannot be used
    * anymore.
    * This will be called if the user fails to create a SIP connection and
    * does not send an appropriate SIP response.
    * @param aTransaction The time-outed transaction.
    */
    TInt TimedOut( CSIPServerTransaction& aTransaction );

	protected:

	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @param aName availability name
     * @param aModules is data container
     * @return
     */
	CMusAvaInviteHandler(
	    MMusAvaAvailabilityObserver& aObserver,
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

public: 

    /**
     * Handles a received invitation
     *
     * @param aTransaction A SIP Transaction of the received invitation.
     */
    void InvitationReceivedL( CSIPServerTransaction* aTransaction );


public: // MMusAvaInviteResponderObserver

    MMusAvaSettingsObserver::TApplicationState ApplicationState();

    /**
     * Deletes a invitation responder instance after answering.
     *
     */
    void InvitationAnsweredLD();

private:

    /**
     * Array of invitation responder instances
     */
    RPointerArray<CMusAvaInviteResponder> iResponders;

    /**
     * Reference to availability settings (data store)
     */
    CMusAvaSettingsImp& iSettings;

	/**
     * Pointer to CMusAvaSharedObject object
     */
	CMusAvaSharedObject* iSharedObj;

	MUS_UNITTEST ( UT_CMusAvaInviteHandler )
    };


#endif // __MUSAVAINVITEHANDLER_H__