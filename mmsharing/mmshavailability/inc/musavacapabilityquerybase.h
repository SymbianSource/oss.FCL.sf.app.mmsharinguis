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
* Description:  Sent capability query (OPTIONS)
*
*/


#ifndef CMUSAVACAPABILITYQUERYBASE_H
#define CMUSAVACAPABILITYQUERYBASE_H


#include <e32base.h>    // CBase

#ifdef CAPABILITY_UNIT_TESTING
#include "capabilitytestdefs.h"
#else
#define CAPABILITY_UT_DEFINITIONS
#endif



class CMusAvaCapability;
class CSIPConnection;
class CSIPProfile;
class CSdpDocument;

class CSIPClientTransaction;
class CMusAvaTerminal;
class CSIPHeaderBase;
class CUri8;

const TInt KCapabilityQueryNotReady = 0;
const TInt KCapabilityCapabilitesReady = 1;
const TInt KCapabilityCapabilitesNotFound = 2;
const TInt KCapabilityCapabilitiesForbidden = 3;


/**
 * Sent capability query (OPTIONS)
 *
 * @lib AlwaysOnlineSwisPlugin.dll
 */
class CMusAvaCapabilityQueryBase : public CBase
    {

public: 

    enum TState
        {
        ECapabilityQueryCreated,
        ECapabilityQueryPrepared,
        ECapabilityQueryExecuting,
        ECapabilityQueryCompleted
        };

public: // constructors and destructor

    virtual ~CMusAvaCapabilityQueryBase();


protected: // constructors

    CMusAvaCapabilityQueryBase( CMusAvaCapability& aCapability,
                              CSIPConnection& aSIPConnection,
                              CSIPProfile& aProfile );


    void ConstructL( const TDesC& aSipAddress );
    
public: // new functions

    /**
     * Executes OPTIONS query
     *
     */
    virtual void ExecuteL();

    /**
     * Complets the query
     *
     * @param aResponse a response to query as SIP client transaction
     */
    void CompletedL( const CSIPClientTransaction& aResponse );

    /**
     * Cancels query, if transaction is created by this query
     *
     * @param aResponse a response to query as SIP client transaction
     */
    void Canceled( const CSIPClientTransaction& aTransaction );

    /**
     * Returns associated terminal
     *
     * @return returns associated terminal
     */
    CMusAvaTerminal& Terminal();

    /**
     * Returns the sdp content.
     *     
     */
    HBufC8* ContentLC();

    
    
public: // new functions

    /**
     * Returns state of query
     *
     * @return state of query
     */
    TState State() const;
    
    /**
     * Returns result of query
     *
     * @return result of query
     */
    TInt Result() const;
    
    
protected: // abstract functions

    /**
     * Prepares query
     *
     * @param aRequestHeaders headers container for request
     */
    virtual void Prepare( RPointerArray<CSIPHeaderBase>& aRequestHeaders ) = 0;
    
    /**
     * Complets the query
     *
     * @param aResponse a response to query as SIP client transaction
     */
    virtual void DoCompletedL( const CSIPClientTransaction& aResponse ) = 0;


protected: // new functions

    /**
     * Returns capablity, which is beed queried
     *
     * @return capablity
     */
    const CMusAvaCapability& Capability() const;
    
    /**
     * Sets state 
     *
     * @param aState new state
     */
    void SetState( TState aState );
    
    /**
     * Sets result
     *
     * @param aResult the result
     */
    void SetResult( TInt aResult );
   
    /**
     * Adds Contact header to request's user headers
     *
     * @param aRequestHeaders request's user headers
     */
    virtual void AddContactHeaderL( RPointerArray<CSIPHeaderBase>& aRequestHeaders );

    /**
     * Resets and destroys transaction
     */
    void ResetAndDestroyTrx();
    
    /**
     * Returns local address based on profile
     *
     * @return local adderss
     */
    const CUri8& LocalAddress() const;

    /**
     * Some Multimediasharing specific sdp contents are added.
     *     
     */
    void OtherSDPHeadersL( CSdpDocument& aResponseContent );
    

private: // NOT own data

    CMusAvaCapability& iCapability;
    CSIPConnection& iSIPConnection;
    CSIPProfile& iProfile;

    TState iState;
    TInt iResult;

    CMusAvaTerminal* iTerminal;
    
private: // own data
    
    CUri8* iRegisteredContact;
    CUri8* iOriginator;
    CSIPClientTransaction* iTrx;
    
//for unit testing
    CAPABILITY_UT_DEFINITIONS
    
    };


#endif // CMUSAVACAPABILITYQUERYBASE_H
