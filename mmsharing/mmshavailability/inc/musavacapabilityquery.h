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
* Description:  Sent SWIS capability query (OPTIONS)
*
*/


#ifndef CMUSAVACAPABILITYQUERY_H
#define CMUSAVACAPABILITYQUERY_H


#include "musavacapabilityquerybase.h"
#include "musavacapabilitytimer.h"

class CSIPResponseElements;
class CMusAvaCapability;
class CSdpDocument;

const TInt KCapabilityTimervalue = 20 * 1000000;

/**
 * Sent SWIS capability query (OPTIONS)
 *
 * @lib AlwaysOnlineSwisPlugin.dll
 */
class CMusAvaCapabilityQuery : public CMusAvaCapabilityQueryBase,
                                 public CMusAvaCapabilityTimer::MTimeOutNotify
    {

public: // constructors and destructor

    static CMusAvaCapabilityQuery* NewL( CMusAvaCapability& aCapability,
                                           CSIPConnection& aSIPConnection,
                                           CSIPProfile& aProfile,
                                           const TDesC& aSipAddress );
                                           

    virtual ~CMusAvaCapabilityQuery();


protected: // constructors

    CMusAvaCapabilityQuery( CMusAvaCapability& aCapability,
                              CSIPConnection& aSIPConnection,
                              CSIPProfile& aProfile );

    void ConstructL( const TDesC& aSipAddress );


public: // from CMusAvaCapabilityTimer::MTimeOutNotify

    /**
     * Called when timer expires
     *
     * @param aStatus status
     */
    void OnExpiredL( TInt aStatus );
    

protected: // from CMusAvaCapabilityQueryBase

    /**
     * Prepares query
     *
     * @param aRequestHeaders headers container for request
     */
    void Prepare( RPointerArray<CSIPHeaderBase>& aRequestHeaders );
    
    /**
     * Complets the query
     *
     * @param aResponse a response to query as SIP client transaction
     */
    void DoCompletedL( const CSIPClientTransaction& aResponse );

    /**
     * Complets the query with 200 OK
     *
     * @param aResponse a response to query as SIP client transaction
     */
    virtual void DoCompleted200OKL( 
                        const CSIPClientTransaction& aResponse );

    /**
     * Validates and store the supported codecs
     *
     * @param aSDP SDP document
     * @return ETrue, if validation is OK
     */
    virtual TBool ValidateAndStoreCodecsL( CSdpDocument& aSDP );


    /**
     * Validates atributes
     *
     * @param aSDP SDP document
     * @return ETrue, if OK
     */
    virtual TBool ValidateAttributesL( CSdpDocument& aSDP );
    
    /**
     * Validates Contact header parameters
     *
     * @param aResponse a response to query as SIP client transaction
     * @return ETrue, if OK
     */
    virtual TBool ValidateContactL( const CSIPClientTransaction& aResponse );
    
    /**
     * Retries query
     *
     */
    void DoRetryL();

    /**
     * Set/Get re-trying status
     *
     * @return re-trying status
     */
    TBool& Retrying();
    

private: // OWNED data


    CMusAvaCapabilityTimer* iTimer;
    
    
private: // data

    TBool iRetrying;

//for unit testing
    CAPABILITY_UT_DEFINITIONS

    };


#endif // CMUSAVACAPABILITYQUERY_H
