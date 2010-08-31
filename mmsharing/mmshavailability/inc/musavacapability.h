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


#ifndef CMUSAVACAPABILITY_H
#define CMUSAVACAPABILITY_H


#include <e32base.h>    // CBase
#include <sdpcodecstringconstants.h>
#include <sdpcodecstringpool.h>
#include <badesca.h>
#include "musunittesting.h"
#include "mussettingskeys.h"

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

#define CAPABILITY_CONTAINS( tagSet, tag ) ( tagSet.Find( tag ) >= KErrNone )
_LIT8( KCapabilitySwisFeature, "+g.3gpp.cs-voice");
_LIT8( KCapabilitySwisApplication, "com.gsma.rts");
_LIT8( KCapabilitySDPAttributeNameType, "type");
_LIT8( KCapabilitySDPAttributeType, "videolive");
_LIT8( KCapabilitySDPAttributeCodecH263, "H263-2000" );
_LIT8( KCapabilitySwisFormatList, "98 96" );
_LIT8( KCapabilitySwisFormatListH263Only, "96" );
_LIT8( KCapabilitySDPAttributeCodecH264, "H264" );
_LIT8( KCapabilitySDPAttributeFastMode, "fastmode" ); //a=keywds:fastmode, see RFC 4566

const TUint64 KCapabilitySDPAttributeClockrate = 90000;
const TUint64 KCapabilitySDPAttributePayloadType = 96;
const TUint64 KCapabilitySDPAttributePayloadTypeH264 = 98;


/**
 * Capability
 *
 * @lib musavailabilityplugin.dll
 */
class CMusAvaCapability : public CBase
    {

public: // constructors and destructor

    static CMusAvaCapability* NewL( CMusAvaCapabilityExchange& aExchange );
    virtual ~CMusAvaCapability();


protected: // constructors

    CMusAvaCapability( CMusAvaCapabilityExchange& aExchange );

    void ConstructL( );

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
            

private: 

    /**
    * Populates response to query
    *
    * @param aResponseContent SDP document container for response
    */
    void DoPopulateResponseL( CSdpDocument& aResponseContent );



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
     * Finds and resolves the codecs from SDP document
     *
     * @return the supported media types
     */
    static CDesCArrayFlat* ResolveCodecsL( CSdpDocument& aSDP );

    /**
     * Finds attribute field from SDP document
     *
     * @param aAttribute attribute name
     * @param aSDP SDP document
     * @return the attribute
     */
    static const CSdpAttributeField* Attribute( const RStringF& aAttribute,
                                                   CSdpDocument& aSDP );

    /**
     * Finds attribute field from media part in SDP document
     *
     * @param aMedia name of the media
     * @param aAttribute attribute name
     * @param aSDP SDP document
     * @return the attribute
     */
    static const CSdpAttributeField* Attribute( const RStringF& aMedia,
                                        const RStringF& aAttribute,
                                        CSdpDocument& aSDP );
    
    /**
     * Finds fast mode value from the SDP document
     */
    static MusSettingsKeys::TFastMode ResolveFastModeL( CSdpDocument& aSdp );
    
    /**
     * Add fast mode related attribute to SDP, if fast mode is supported
     */
    static void AddFastModeL( CSdpDocument& aSdp );
    
    
public://new functions

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
    static CSdpFmtAttributeField* RtpMapLineLC( const TDesC8& aCodec,
                                 const TUint64 aPayloadType,
                                 const TUint64 aClockRate,
                                 const TDesC8& aParams = KNullDesC8 );
    
                                   
private:

    CMusAvaCapabilityExchange& iExchange;
    RStringF iFeature;
    TBool iIsEnabled;

    //for unit testing
    //MUS_UNITTEST ( UT_ )

    };

#endif // CMUSAVACAPABILITY_H
