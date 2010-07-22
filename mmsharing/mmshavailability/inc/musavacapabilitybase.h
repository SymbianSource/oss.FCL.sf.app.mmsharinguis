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
* Description:  Capability
*
*/


#ifndef CMUSAVACAPABILITYBASE_H
#define CMUSAVACAPABILITYBASE_H


#include <e32base.h>    // CBase
#include <sdpcodecstringconstants.h>
#include <sdpcodecstringpool.h>
#include "musunittesting.h"


class CMusAvaCapabilityExchange;
class CSIPServerTransaction;
class CSIPHeaderBase;
class CSdpDocument;
class CSIPAcceptContactHeader;
class CSdpFmtAttributeField;
class CSdpAttributeField;
class CSdpOriginField;
class CSdpMediaField;
class RStringF;

#define CAPABILITY_CONTAINS( tagSet, tag ) tagSet.Find( tag ) >= KErrNone

/**
 * Capability
 *
 * @lib musavailabilityplugin.dll
 */
class CMusAvaCapabilityBase : public CBase
    {

public: // constructors and destructor

    virtual ~CMusAvaCapabilityBase();


protected: // constructors

    CMusAvaCapabilityBase( CMusAvaCapabilityExchange& aExchange );

    void ConstructL( const TDesC8& aFeature );

public: // new functions

    /**
    * Returns exchance
    *
    * @return exchance
    */
    CMusAvaCapabilityExchange& Exchange() const;

    /**
    * Returns feature this capability represents
    *
    * @return feature this capability represents
    */
    const RStringF& Feature() const;

    /**
    * Checks if capability supports feature, found
    * in Accept-Contact header
    *
    * @param acceptContact accept contact
    * @return ETrue, if there is a match
    */
    virtual TBool Supports( const CSIPAcceptContactHeader& acceptContact );

    /**
    * Uses for conrolling capability 
    *
    * @return ETrue, if capability is enabled
    */
    TBool& Enabled();

    /**
    * Populates response to query
    *
    * @param aQuery query as SIP server transaction
    * @param aResponseHeaders SIP headers container for response
    * @param aResponseContent SDP document container for response
    */
    virtual void PopulateResponseL( 
            CSIPServerTransaction& aQuery,
            RPointerArray<CSIPHeaderBase>& aResponseHeaders,
            CSdpDocument& aResponseContent );
            

protected: //abstract methods

    /**
    * Populates response to query
    *
    * @param aQuery query as SIP server transaction
    * @param aResponseHeaders SIP headers container for response
    * @param aResponseContent SDP document container for response
    */
    virtual void DoPopulateResponseL( 
            CSIPServerTransaction& aQuery,
            RPointerArray<CSIPHeaderBase>& aResponseHeaders,
            CSdpDocument& aResponseContent ) = 0;



public: //static API

                       
    /**
     * Creates SDP document
     *
     * @param aQuery received query
     * @return SDP document
     */
    static CSdpDocument* ResponseContentLC( 
                                     const CSIPServerTransaction& aQuery );

    /**
     * Finds codec from SDP document
     *
     * @param aMedia name of the media
     * @param aCodec name of codec
     * @param aSDP SDP document
     * @return the media line
     */
    static const CSdpFmtAttributeField* CodecL( const RStringF& aMedia,
                                        const TDesC8& aCodec,
                                        CSdpDocument& aSDP );

    /**
     * Finds attribute field from SDP document
     *
     * @param aAttribute atrbute name
     * @param aSDP SDP document
     * @return the attribute
     */
    static const CSdpAttributeField* Attribute( const RStringF& aAttribute,
                                                   CSdpDocument& aSDP );

protected://new functions

    /**
    * Adds contact header to response of query
    *
    * @param aQuery query as SIP server transaction
    * @param aResponseHeaders SIP headers container for response
    */
    virtual void AddContactHeaderL(
            CSIPServerTransaction& aQuery,
            RPointerArray<CSIPHeaderBase>& aResponseHeaders );
            
    /**
     * Creates origin field for sdp
     *
     * @param aNetType net type
     * @param aLocalHost local host
     * @param aHostType host type
     * @return origin field 
     */
    static CSdpOriginField* OriginFieldL( const RStringF& aNetType,
                                          const TDesC8& aLocalHost,
                                          const RStringF& aHostType );
                                          
    /**
     * Creates media line 
     *
     * @param aMedia name of the media
     * @param aFormatList format list
     * @return media line
     */
    CSdpMediaField* MediaLineLC( const RStringF& aMedia, 
                                 const TDesC8& aFormatList ) const;
    
    
    /**
     * Creates rtpmap line
     *
     * @param aCodec name of coded
     * @param aPayloadType payload type
     * @param aClockRate clockrate
     * @param aParams optional params
     * @return rtpmap line
     */
    CSdpFmtAttributeField* RtpMapLineLC( const TDesC8& aCodec,
                                 const TUint aPayloadType,
                                 const TUint aClockRate,
                                 const TDesC8& aParams = KNullDesC8 ) const;
    
                                   
private:

    CMusAvaCapabilityExchange& iExchange;
    RStringF iFeature;
    TBool iIsEnabled;

    //for unit testing
    //MUS_UNITTEST ( UT_ )

    };

#endif // CMUSAVACAPABILITYBASE_H
