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
* Description:  Capability Exchange 
*
*/

#ifndef CMUSAVACAPABILITYEXCHANGE_H
#define CMUSAVACAPABILITYEXCHANGE_H

#include <e32base.h>    // CBase
#include "musunittesting.h"

class MMusAvaCapabilityQueryObserver;
class CMusAvaCapabilityQueryBase;
class CSIPClientTransaction;
class CSIPServerTransaction;
class CMusAvaTerminal;
class CMusAvaCapability;

/**
 * Capability Exchange
 *
 * @lib musavailabilityplugin.dll
 */
class CMusAvaCapabilityExchange : public CBase
    {

public: // constructors and destructor

    static CMusAvaCapabilityExchange* NewL( const TDesC8& aTerminalId,
                            MMusAvaCapabilityQueryObserver& aQueryObserver );

    virtual ~CMusAvaCapabilityExchange();


private: // constructors

    CMusAvaCapabilityExchange( 
                            MMusAvaCapabilityQueryObserver& aQueryObserver );

    void ConstructL( const TDesC8& aTerminalId );


public: // new functions


    /**
     * Returns query observer
     *
     * @return query observer
     */
    MMusAvaCapabilityQueryObserver& QueryObserver() const;
    
    /**
     * Returns terminal ID
     *
     * @return terminal ID
     */
    const TDesC8& TerminalId() const;

    /**
     * Called when answer to query has been received
     *
     * @param aResponse SIP client transaction representing 
     * the response to query
     */
    void AnswerToQueryReceived( const CSIPClientTransaction& aResponse );

    /**
     * Called when capability query has been received
     *
     * @param aQuery SIP server transaction representing
     * a capability query, ownership is transferred
     */
    void QueryReceived( CSIPServerTransaction* aQuery );

    /**
     * Called when capability query has been canceled
     *
     * @param aError system wide or SIP error code
     * @param aTransaction SIP client transaction 
     */
    void CancelQuery( TInt aError, const CSIPClientTransaction& aTransaction );


public: // new functions

    
    /**
     * Adds capability
     *
     * @param aCapability a capability, ownership is transferred
     */
    void AddCapabilityL( CMusAvaCapability* aCapability );

    /**
     * Returns all discovered terminals
     * 
     * @return all discovered terminals
     */
    const RPointerArray<CMusAvaTerminal>& Terminals();

    /**
     * Returns all capabilities
     * 
     * @return all capabilities
     */
    const RPointerArray<CMusAvaCapability>& Capabilities();

    
public: // for internal use only

    /**
     * Returns terminal based on remote URI
     *
     * @return terminal based on remote URI
     */
    CMusAvaTerminal& TerminalL( const TDesC8& aRemoteUri );
    
    
private: // new functions

    CMusAvaTerminal& TerminalL( const CSIPServerTransaction& aQuery );
    void DoQueryReceivedL( CSIPServerTransaction& aQuery );
    void RejectQueryL( CSIPServerTransaction& aQuery,
                       TInt aError );
    TInt FindByFeature( CMusAvaCapability& aCapablity );
    
    
private: // data

    MMusAvaCapabilityQueryObserver& iQueryObserver;
    HBufC8* iTerminalId;
    RPointerArray<CMusAvaTerminal> iTerminals;
    RPointerArray<CMusAvaCapability> iCapabilities;

    MUS_UNITTEST ( UT_CMusAvaCapabilityExchange )
    MUS_UNITTEST ( UT_CMusAvaCapabilitySipAgent )
    };


#endif // CMUSAVACAPABILITYEXCHANGE_H
