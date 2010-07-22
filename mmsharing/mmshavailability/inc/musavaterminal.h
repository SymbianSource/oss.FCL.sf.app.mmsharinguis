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
* Description:  Discovered terminal
*
*/

#ifndef CMUSAVATERMINAL_H
#define CMUSAVATERMINAL_H

#include <e32base.h>    // CBase
#include "musunittesting.h"


class CMusAvaCapabilityQueryBase;
class CMusAvaCapability;
class CSIPClientTransaction;
class CSIPServerTransaction;
class CMusAvaCapabilityExchange;
class CSdpDocument;
class CSIPHeaderBase;
class CSIPResponseElements;

/**
 * Discovered terminal
 *
 * @lib musavailabilityplugin.dll
 */
class CMusAvaTerminal : public CBase
    {

public: // constructors and destructor


    static CMusAvaTerminal* NewL( CMusAvaCapabilityExchange& aExchange,
                                const TDesC8& aTerminalUri,
                                const TDesC8& aTerminalId );

    virtual ~CMusAvaTerminal();


private: // constructors

    CMusAvaTerminal( CMusAvaCapabilityExchange& aExchange );

    void ConstructL( const TDesC8& aTerminalUri ,
                     const TDesC8& aTerminalId );


public: // new functions

    /**
     * Returns ID
     *
     * @return ID
     */
    const TDesC8& Id() const;

    /**
     * Returns (remote) URI
     *
     * @return (remote) URI
     */
    const TDesC8& Uri() const;

    /**
     * Checks if terminal matches given condition
     *
     * @param aTerminalUri terminal uri
     * @param aTerminalId terminal uri
     * @return ETrue, if there is a match
     */
    TBool MatchL( const TDesC8& aTerminalUri, 
                  const TDesC8& aTerminalId = KNullDesC8 );

    /**
     * Returns requested query (query from peer), if any
     *
     * @return executed query or NULL
     */
    const CMusAvaCapability* RequestedQuery() const;
    
    /**
     * Executes capability query.
     *
     * @param aQuery query to execute, ownership is transferred
     */
    void ExecuteQueryL( CMusAvaCapabilityQueryBase* aQuery );

    /**
     * Called when final response to OPTIONS has been received
     *
     * @param aResponse final response to OPTIONS 
     * @return EFalse, if response was not consumed
     */
    TBool QueryCompletedL( const CSIPClientTransaction& aResponse );

    /**
     * Called when query has been canceled.
     *
     * @return aTransaction the transaction, which caused cancelation
     * @return EFalse, if not consumed
     */
    TBool QueryCanceled( const CSIPClientTransaction& aTransaction );

    /**
     * Query state executing.
     *
     * @return ETrue if query executing, otherwise EFalse 
     */
    TBool QueryExecuting();

    /**
     * Called when OPTIONS request has been received
     *
     * @param aQuery query received 
     */
    void QueryRequestedL( CSIPServerTransaction& aQuery );

    /**
     * Attach query
     *
     * @param aQuery a query to be attached
     * @return number of attached queries
     */
    TInt AttachQuery( CMusAvaCapabilityQueryBase& aQuery );
    
    /**
     * Removes query
     *
     * @param aQuery a query to be detached
     * @return number of attached queries
     */
    TInt DetachQuery( CMusAvaCapabilityQueryBase& aQuery );
    
    /**
     * Returns ETrue, if terminal is fully discovered
     *
     * @return ETrue, if terminal is fully discovered
     */
    TBool IsDiscovered();
 
     
private: // new functions

    void AttachIdL( const TDesC8& aTerminalId );
    void AttachUriL( const TDesC8& aUri );
    void ResetAndDestroyQuery();
    TBool PopulateResponseL( CSIPServerTransaction& aQuery,
                        RPointerArray<CSIPHeaderBase>& aResponseHeaders,
                             CSdpDocument& aSdpContent );
    void AddResponseHeadersL( CSIPResponseElements& aResponse,
                        RPointerArray<CSIPHeaderBase>& aResponseHeaders );
    void AddResponseContentL( CSIPResponseElements& aResponse,
                              CSdpDocument& aSdpContent );
    
    
private: // NOT owned data

    CMusAvaCapabilityExchange& iExchange;
    CMusAvaCapability* iRequestedQuery;

private: // data

    HBufC8* iTerminalId;
    HBufC8* iUri;
    CMusAvaCapabilityQueryBase* iQuery;

    TInt iQueries;
    
    MUS_UNITTEST ( UT_CMusAvaTerminal )
    MUS_UNITTEST ( UT_CMusAvaCapabilityExchange )
    MUS_UNITTEST ( UT_CMusAvaCapabilitySipAgent )
    MUS_UNITTEST ( UT_CMusAvaCapabilityQuery )
    };


#endif // CMUSAVATERMINAL_H
