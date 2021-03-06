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
* Description:  
*
*/


#ifndef MUSSIPCRPLUGIN_H
#define MUSSIPCRPLUGIN_H

#include "musunittesting.h"

#include <apgcli.h>
#include <sipresolvedclient.h>
#include <ecom/implementationproxy.h>

class CMusManager;
class CSdpMediaField;

IMPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount );

/**
 * CMusSipCrPlugin
 */
class CMusSipCrPlugin : public CSIPResolvedClient
    {
    MUS_UNITTEST( CMusSipCrPluginTest )

public:  // constructors and destructor

    static CMusSipCrPlugin* NewL();

    virtual ~CMusSipCrPlugin();

private: // constructors

    CMusSipCrPlugin();

    void ConstructL();

public: // from CSIPResolvedClient

    /**
     * Returns the SIP client's channel UID that indicates client's
     * channel UID for connection with SIP e.g. CSIP::NewL()
     * @param aMethod the method of the SIP request
     * @param aRequestUri the request-URI of the SIP request
     * @param aHeaders all the headers in the SIP request
     * @param aContent SIP request body; zero-length descriptor if not present
     * @param aContentType the content-type of the SIP request.
     *        Zero-pointer if body is not present.
     * @return SIP client's communication channel's UID
     */
    TUid ChannelL( RStringF aMethod,
                  const TDesC8& aRequestUri,
                  const RPointerArray<CSIPHeaderBase>& aHeaders,
                  const TDesC8& aContent,
                  const CSIPContentTypeHeader* aContentType=0 );


    /**
     * Requests the client to connect to SIP with resolved
     * UID in case there's no connection with resolved channel UID.
     * @param aUid previously resolved channel UID
     * @leave KErrNoMemory if out of memory
     * @leave KErrNotFound in case non-existing channel UID was provided
     */
    void ConnectL( TUid aUid );

    /**
     * Requests the implementation to provide capabilities in XML format
     * in case they were not defined in the ECOM resource file. The cabablities
     * to be returned must be defined according to the same DTD.
     * This function will be invoked only if the capabilities are not
     * defined in the ECOM resource file.
     */
    const TDesC8& Capabilities();

private: // New functions
    
    /**
     * Check whether video field has sendrecv attribute
     * @return ETrue if video has sendrecv attribute, EFalse otherwise 
     */
    TBool CheckForSendRecvAttributeL(
        RPointerArray<CSdpMediaField>& aFields ) const;
    
private: // data

    CMusManager* iManager;
    TBool iCloseStringPool;
    
    };


#endif // MUSSIPCRPLUGIN_H
