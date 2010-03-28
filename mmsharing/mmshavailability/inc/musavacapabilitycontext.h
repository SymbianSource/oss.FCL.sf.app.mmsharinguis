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
* Description:  Swis call context
*
*/

#ifndef MUSAVACAPABILITYCONTEXT_H
#define MUSAVACAPABILITYCONTEXT_H

#include <badesca.h>
#include "musunittesting.h"


const TInt KMUSAVASIP200 = 200;
const TInt KMUSAVASIPServerInternalError = 500;
const TInt KMUSAVASIPNotImplemented = 501;
const TInt KMUSAVASIPForbidden = 403;
const TInt KMUSAVASIPNotFound = 404;
const TInt KMUSAVASIPTimeout = 408;
const TInt KMUSAVASIPUnsupportedURIScheme = 416;
const TInt KMUSAVASIPTemporarilyUnavailable = 480;
const TUint KMUSAVASIPDecline = 603;

const TInt KMUSAVAMaxLengthOfSDPMessage = 1000;
_LIT8( KMUSAVASIPMediaTypeApplication, "application" );
_LIT8( KMUSAVASIPMediaSubTypeSDP, "sdp" );
_LIT8( KMUSAVASIPHeaderUserAgent, "User-Agent" );
_LIT8( KMUSAVASIPHeaderPAssertedIdentity, "P-Asserted-Identity" );
_LIT8( KMUSAVASIPParamFalse, "false" );
_LIT8( KMUSAVASemicolon, ";");


class CSIPServerTransaction;
class RStringF;
class CSdpDocument;
class CSIPMessageElements;
class CSIPTransactionBase;
class CSIPHeaderBase;
class CSIPExtensionHeader;
class CSIPAcceptContactHeader;
class CSIPContactHeader;
class CUri8;


class MusAvaCapabilityContext
    {
public: // static API

    /**
    * Returns SDP string from pool
    *
    * @param aStringIndex SDP string index
    * @return SDP String
    */
    static RStringF SDPStringL( TInt aStringIndex );

    /**
    * Creates SDP string to pool
    *
    * @param aString the string as descriptor
    * @return SDP String
    */
    static RStringF SDPStringL( const TDesC8& aString );

    /**
    * Creates SIP string to pool
    *
    * @param aString the string as descriptor
    * @return SIP String
    */
    static RStringF SIPStringL( const TDesC8& aString );
    
    /**
    * Resolves local host
    *
    * @param aQuery query
    * @return feature set
    */
    static void LocalHostL( const CSIPServerTransaction& aRequest,
                            TPtrC8& aLocalHost,
                            RStringF& aHostType);

    /**
     * Helper to create uri from descriptor
     *
     * @param aUri uri as descriptor
     * @return uri
     */
    static CUri8* ToUriL( const TDesC8& aUri );
    
    /**
     * Helper to transform SDP document to text
     *
     * @param aSdpContent SDP document
     * @return SDP document as text
     */
    static HBufC8* ToTextL( CSdpDocument& aSdpContent );


    /**
     * Returns message elemensts from request/response
     *
     * @param aTrx server/client transaction
     * @return message elemensts
     */
    static const CSIPMessageElements& MessageElementsL( 
                                const CSIPTransactionBase& aTrx );

    /**
     * Creates Accept-Contact header
     *
     * @param aValue value of header
     * @return the header
     */
    static CSIPAcceptContactHeader* AcceptContactHeaderLC( RStringF aValue );
    
    /**
     * returns Accept-Contact header from request/response
     *
     * @param aTrx  request/response
     * @return the header
     */
    static const CSIPAcceptContactHeader* AcceptContactHeaderL( 
                                const CSIPTransactionBase& aTrx );

    /**
     * Creates Contact header
     *
     * @param aAddress the address
     * @param aValue value of header
     * @return the header
     */
    static CSIPContactHeader* ContactHeaderLC( const CUri8& aAddress,
                                               RStringF aValue );
    
    /**
     * Returns Contact header from request/response
     *
     * @param aTrx  request/response
     * @return the header
     */
    static const CSIPContactHeader* ContactHeaderL( 
                                const CSIPTransactionBase& aTrx );


    /**
     * Creates User-Agent header
     *
     * @param aValue value of header
     * @return the header
     */
    static CSIPExtensionHeader* UserAgentHeaderLC( const TDesC8& aValue );
    
    /**
     * returns User-Agent header from request/response
     *
     * @param aTrx  request/response
     * @return the header
     */
    static const CSIPExtensionHeader* UserAgentHeaderL( 
                                const CSIPTransactionBase& aTrx );

    /**
     * Creates P-Asserted-Identity header
     *
     * @param aValue value of header
     * @return the header
     */
    static CSIPExtensionHeader* PAssertedIdentityHeaderLC( 
                                const TDesC8& aValue );
    
    /**
     * Returns originator from request
     *
     * @param aTrx request
     * @return originator's URI
     */
    static const CUri8& OriginatorL( const CSIPServerTransaction& aTrx );

    /**
     * Finds header from user headers based on header's name
     *
     * @param aHeaderName name of the header
     * @param aMessage SIP message containg user headers
     * @param aIndex index of the header in message
     * @return the header
     */
    static const CSIPHeaderBase* Header( RStringF aHeaderName,
                                   const CSIPMessageElements& aMessage,
                                   TUint aIndex );
                                   
    /**
     * Finds header from user headers based on header's name
     *
     * @param aHeaderName name of the header
     * @param aMessage SIP message containg user headers
     * @return the header
     */
    static const CSIPHeaderBase* Header( RStringF aHeaderName,
                                   const CSIPMessageElements& aMessage );
    /**
     * Finds header count from user headers based on header's name
     *
     * @param aHeaderName name of the header
     * @param aMessage SIP message containg user headers
     * @return the count
     */
    static TUint HeaderCount( RStringF aHeaderName,
                                   const CSIPMessageElements& aMessage );

    /**
     * Finds and returns all P-Asserted-Identity header .
     *
     * @param aMessage SIP message header.     
     * @return array which has all P-Asserted-Identity header.
     */
    static CDesCArrayFlat* PAssertedIdentityHeaderL(
                                    const CSIPMessageElements& aMessage );
    //MUS_UNITTEST ( UT_ )

    };
    
    
#endif // MUSAVACAPABILITYCONTEXT_H
