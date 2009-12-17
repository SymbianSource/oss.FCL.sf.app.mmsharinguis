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
* Description:  SIP event handling class
*
*/


#ifndef CMUSAVACAPABILITYSIPAGENT_H
#define CMUSAVACAPABILITYSIPAGENT_H


#include <e32base.h>    // CBase
#include "musunittesting.h"
#include "musavasipadapter.h"
#include "musavasipconnectionadapter.h"
#include "musavaavailabilityobserver.h"


class CMusAvaSharedObject;
class CSIPConnection;
class CSIPDialog;
class CSIPProfile;
class CSIPProfileRegistry;
class CSIPServerTransaction;
class CMusAvaCapabilityExchange;
class CMusAvaCapability;

/**
 * SIP event handling class
 *
 * @lib AlwaysOnlineSwisPlugin.dll
 */
class CMusAvaCapabilitySipAgent : 
    public CBase,
    public MMusAvaSipAdapter, 
    public MMusAvaSipConnectionAdapter
    {

private:

    /** SIP connection state */

    enum TSipConnectionState
        {
        /** Connection state unknown */
        ESipConnUnknown = 0,
        /** Connection initializing */
        ESipConnInit,
        /** Connection active */
        ESipConnActive,
        /** Connection suspended */
        ESipConnSuspended,
        /** Connection is inactive */
        ESipConnInactive,
        /** Connection is permanently unavailable */
        ESipConnUnavailable
        };


public: // constructors and destructor

    static CMusAvaCapabilitySipAgent* NewL( 
        CMusAvaCapabilityExchange& aCapabilityExchange, 
        CMusAvaSharedObject& aSharedObject,
        MMusAvaAvailabilityObserver& aAvaObserver );

    virtual ~CMusAvaCapabilitySipAgent();


private: // constructors

    CMusAvaCapabilitySipAgent( CMusAvaCapabilityExchange& aCapabilityExchange,
                            CMusAvaSharedObject& aSharedObject,
                            MMusAvaAvailabilityObserver& aAvaObserver );


public: // new functions

    /**
     * Executes capability query
     *
     *@param aCapability capability to query
     *@param aSipAddress remote part address
     */
    void ExecuteCapabilityQueryL( CMusAvaCapability& aCapability,
                                const TDesC& aSipAddress );


public: // functions from base class MSIPObserver

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
    TInt IncomingRequest( TUint32 aIapId,
                          CSIPServerTransaction* aTransaction );

    /**
    * The received SIP request time-outed and it has been destroyed.
    * This will be called if the user fails to create a SIP
    * connection and does not send an appropriate SIP response.
    * @param aTransaction The time-outed transaction.
    */
    TInt TimedOut( CSIPServerTransaction& /* aTransaction */ );


public: // functions from base class MMusAvaSipConnectionAdapter

    /**
    * A SIP request outside a dialog has been received from the
    * network.
    */
    TInt IncomingRequest( CSIPServerTransaction* aTransaction );

	/**
    * A SIP response received from the network.
	*
	* @param aTransaction contains response elements.
	*/
	TInt IncomingResponse (CSIPClientTransaction& aTransaction);

	/**
	* An asynchronous error has occurred in the stack related to the
	* request indicated by the given transaction.
	*
	* @param aError system wide or SIP error code
	* @param aTransaction failed transaction
	* @param aSIPConnection a SIP connection        
	*/
	TInt ErrorOccured (TInt aError, CSIPTransactionBase& aTransaction );

private:
    /**
    * Handle incoming SIP request
    * @param aTransaction incoming transaction
    */
    void HandleIncomingRequestL( CSIPServerTransaction& aTransaction );

private: // data

    CSIPConnection*          iSipConnection;
    CMusAvaCapabilityExchange& iCapabilityExchange;
    CMusAvaSharedObject&     iSharedObj;
    MMusAvaAvailabilityObserver& iAvailabilityObserver;
    
    MUS_UNITTEST ( UT_CMusAvaCapabilitySipAgent )

    };


#endif // CMUSAVACAPABILITYSIPAGENT_H
