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
* Description:   Capability (answer to OPTIONS)
*
*/



#include "musavacapability.h"
#include "muslogger.h"
#include "musavacapabilityexchange.h"
#include "musavacapabilitycontext.h"
#include "mmusavacapabilityqueryobserver.h"
#include "mussettings.h"

#include <e32math.h>
#include <uri8.h>
#include <escapeutils.h>
#include <sipservertransaction.h>
#include <sipacceptcontactheader.h>
#include <sipcontactheader.h>
#include <siprequestelements.h>
#include <sdpdocument.h>
#include <sdpmediafield.h>
#include <sdpconnectionfield.h>
#include <sdporiginfield.h>
#include <sdpfmtattributefield.h>
#include <sdpattributefield.h>
#include <sdprtpmapvalue.h>
#include <sipstrings.h>
#include <sdpcodecstringpool.h>
#include <sipstrconsts.h>

_LIT8(KSessionName, "-");
_LIT8(KOriginFieldUser, "-");

const TInt KMaxNumAsStringSize = 16;

// --------------------------------------------------------------------------
// Symbian two-phase constructor
// --------------------------------------------------------------------------
//
    CMusAvaCapability* CMusAvaCapability::NewL( 
                                        CMusAvaCapabilityExchange& aExchange )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapability::NewL" )
    
    CMusAvaCapability* self = new (ELeave) CMusAvaCapability( aExchange );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapability::NewL" )
    
    return self;
    }

// --------------------------------------------------------------------------
// C++ destructor
// --------------------------------------------------------------------------
//
CMusAvaCapability::~CMusAvaCapability()
    {
    MUS_LOG( 
        "mus: [MUSAVA]: -> CMusAvaCapability::~CMusAvaCapability" )
    
    iFeature.Close();
    SIPStrings::Close();
    SdpCodecStringPool::Close();
    
    MUS_LOG( 
        "mus: [MUSAVA]: <- CMusAvaCapability::~CMusAvaCapability" )
    }


// --------------------------------------------------------------------------
// C++ constructor
// --------------------------------------------------------------------------
//
CMusAvaCapability::CMusAvaCapability( 
                                    CMusAvaCapabilityExchange& aExchange )
    : iExchange( aExchange ),
      iIsEnabled( ETrue )
    {
    }


// --------------------------------------------------------------------------
// Symbian second-phase constructor
// --------------------------------------------------------------------------
//
void CMusAvaCapability::ConstructL( )
    {
    MUS_LOG( "mus: [MUSAVA]: -> CMusAvaCapability::ConstructL" )
    
    MUS_LOG( "mus: [MUSAVATEST]: SIPStrings::OpenL()" )
    TRAP_IGNORE( SIPStrings::OpenL() )
    TRAP_IGNORE( SdpCodecStringPool::OpenL() ) 
    
    MUS_LOG( "mus: [MUSAVATEST]: MusAvaCapabilityContext::SIPStringL( KCapabilitySwisFeature )" )
    iFeature = MusAvaCapabilityContext::SIPStringL( KCapabilitySwisFeature );
    
    MUS_LOG( "mus: [MUSAVA]: <- CMusAvaCapability::ConstructL" )
    }


// --------------------------------------------------------------------------
// CMusAvaCapability::Exchange
// --------------------------------------------------------------------------
//
CMusAvaCapabilityExchange& CMusAvaCapability::Exchange() const
    {
    return iExchange;
    }


// --------------------------------------------------------------------------
// CMusAvaCapability::Feature
// --------------------------------------------------------------------------
//
const RStringF& CMusAvaCapability::Feature() const
    {
    return iFeature;
    }


// --------------------------------------------------------------------------
// CMusAvaCapability::Supports
// --------------------------------------------------------------------------
//
TBool CMusAvaCapability::Supports( 
                            const CSIPAcceptContactHeader& acceptContact )
    {
    return acceptContact.HasParam( Feature() );
    }
    
    
// --------------------------------------------------------------------------
// CMusAvaCapability::Enabled
// --------------------------------------------------------------------------
//
TBool& CMusAvaCapability::Enabled()
    {
    return iIsEnabled;
    }


// --------------------------------------------------------------------------
// CMusAvaCapability::PopulateResponseL
// --------------------------------------------------------------------------
//
void CMusAvaCapability::PopulateResponseL( 
                            CSIPServerTransaction& aQuery,
                            RPointerArray<CSIPHeaderBase>& aResponseHeaders,
                            CSdpDocument& aResponseContent )

    {
    MUS_LOG( "mus: [MUSAVA]: -> CMusAvaCapability::PopulateResponseL" )

    AddContactHeaderL( aQuery, aResponseHeaders );
    
    DoPopulateResponseL( aResponseContent );
    
    MUS_LOG( "mus: [MUSAVA]: <- CMusAvaCapability::PopulateResponseL" )
    }

// --------------------------------------------------------------------------
// CMusAvaCapability::DoPopulateResponseL
// --------------------------------------------------------------------------
//
void CMusAvaCapability::DoPopulateResponseL( CSdpDocument& aResponseContent )

    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapability::DoPopulateResponseL" )

    MUS_LOG( "mus: [MUSAVA] Adding application attribute to SDP" )
    //application
    CSdpAttributeField* application = CSdpAttributeField::NewLC(
       MusAvaCapabilityContext::SDPStringL( 
            SdpCodecStringConstants::EMediaApplication ), 
       KCapabilitySwisApplication );
                                      
    aResponseContent.AttributeFields().AppendL( application );
    CleanupStack::Pop( application );

    MUS_LOG( "mus: [MUSAVA] Adding type attribute to SDP" )
    //type
    RStringF typeStr = MusAvaCapabilityContext::SDPStringL( 
                                        KCapabilitySDPAttributeNameType );
    CleanupClosePushL( typeStr );
    CSdpAttributeField* type = 
        CSdpAttributeField::NewL( typeStr, KCapabilitySDPAttributeType );
    CleanupStack::PopAndDestroy();//typeStr 
    CleanupStack::PushL( type );
                                         
    aResponseContent.AttributeFields().AppendL( type );
    CleanupStack::Pop( type );
    
    //Add fast startup mode, if supported
    AddFastModeL( aResponseContent );

    MUS_LOG( "mus: [MUSAVA] Adding media line to SDP" )
    //media line    
    CSdpMediaField* mediaLine = MediaLineLC( 
                MusAvaCapabilityContext::SDPStringL( 
                    SdpCodecStringConstants::EMediaVideo ), 
                    KCapabilitySwisFormatListH263Only );
    
    //adding H.264 codec to SDP
    if( !MultimediaSharingSettings::IsAvcDisabled())
        {
        MUS_LOG( "mus: [MUSAVA]  - Adding avc to supported codec list" )
        // reconstruct media line with H264 and H263
        CleanupStack::PopAndDestroy(mediaLine);
        mediaLine = MediaLineLC( 
                 MusAvaCapabilityContext::SDPStringL( 
                    SdpCodecStringConstants::EMediaVideo ), 
                    KCapabilitySwisFormatList );
        
        //adding H.264 codec to SDP
        CSdpFmtAttributeField* rtpMapH264 = 
                                RtpMapLineLC( KCapabilitySDPAttributeCodecH264,
                                KCapabilitySDPAttributePayloadTypeH264,
                                KCapabilitySDPAttributeClockrate ); 
    
        mediaLine->FormatAttributeFields().AppendL( rtpMapH264 );
        CleanupStack::Pop( rtpMapH264 );
        }    
    //H.263 codec
    CSdpFmtAttributeField* rtpMapH263 = 
        RtpMapLineLC( KCapabilitySDPAttributeCodecH263,
                    KCapabilitySDPAttributePayloadType,
                    KCapabilitySDPAttributeClockrate ); 

    mediaLine->FormatAttributeFields().AppendL( rtpMapH263 );
    CleanupStack::Pop( rtpMapH263 );

    aResponseContent.MediaFields().AppendL( mediaLine );
    CleanupStack::Pop( mediaLine );

    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapability::DoPopulateResponseL" )
    }


// --------------------------------------------------------------------------
// CMusAvaCapability::ResponseContentLC
// --------------------------------------------------------------------------
//
CSdpDocument* CMusAvaCapability::ResponseContentLC( 
                            const CSIPServerTransaction& aQuery )

    {
    MUS_LOG( 
        "mus: [MUSAVA]: -> CMusAvaCapability::CreateResponseContentL" )
    
	RStringF netType = 
	MusAvaCapabilityContext::SDPStringL( SdpCodecStringConstants::ENetType );
    
    CSdpDocument* responseContent = CSdpDocument::NewLC();
    
    responseContent->SetSessionNameL( KSessionName );

    TPtrC8 localHost;
    RStringF hostType;
    
    MusAvaCapabilityContext::LocalHostL( aQuery, localHost, hostType );
    
    CSdpOriginField* originField = OriginFieldL( 
                                            netType, localHost, hostType );
    responseContent->SetOriginField( originField );
    
    // Connection field
    CSdpConnectionField* connectionField = 
        CSdpConnectionField::NewL( netType, hostType, localHost );
    responseContent->SetConnectionField( connectionField );         
    
    MUS_LOG( 
        "mus: [MUSAVA]: <- CMusAvaCapability::CreateResponseContentL" )
    
    return responseContent;
    }
    

// -----------------------------------------------------------------------------
// CMusAvaCapability::Codec
// -----------------------------------------------------------------------------
// 
CDesCArrayFlat* CMusAvaCapability::ResolveCodecsL( CSdpDocument& aSDP )
    {
    // TODO: use array of 2 elements, as we assume that max 2 codecs are supported (H.263 and H.264)
    CDesCArrayFlat* codecs =  new( ELeave ) CDesCArrayFlat( 2 );
    CleanupStack::PushL( codecs );
    
    RStringF media = MusAvaCapabilityContext::SDPStringL( 
                                        SdpCodecStringConstants::EMediaVideo );
    
	RStringF rtpmap =  MusAvaCapabilityContext::SDPStringL( 
	                            SdpCodecStringConstants::EAttributeRtpmap );
    
    RPointerArray<CSdpMediaField>& mediaFields = aSDP.MediaFields();
    
    for ( TInt i=0; i < mediaFields.Count(); i++ )
        {
        if ( mediaFields[ i ]->Media() == media )
            {
        	RPointerArray<CSdpFmtAttributeField>& fmtLines =  
                                mediaFields[ i ]->FormatAttributeFields();
            
            for ( TInt j=0; j < fmtLines.Count(); j++ )
                {
                if ( fmtLines[ j ]->Attribute() == rtpmap ) 
                    {
                    if ( CAPABILITY_CONTAINS( fmtLines[ j ]->Value(), KCapabilitySDPAttributeCodecH263 )
                            || ( CAPABILITY_CONTAINS( fmtLines[ j ]->Value(), KCapabilitySDPAttributeCodecH264 ) && 
                                 !MultimediaSharingSettings::IsAvcDisabled() ) )
                        {
                        HBufC* codecBuf = 
                            EscapeUtils::ConvertToUnicodeFromUtf8L( fmtLines[ j ]->Value() );
                        CleanupStack::PushL( codecBuf );  
                        codecs->AppendL( *codecBuf );
                        CleanupStack::PopAndDestroy( codecBuf );
                        }
                    }
                }
            }
        }
    CleanupStack::Pop( codecs );
    
    return codecs;
    }
    
// -----------------------------------------------------------------------------
// CMusAvaCapability::ResolveFastModeL
// -----------------------------------------------------------------------------
// 
MusSettingsKeys::TFastMode CMusAvaCapability::ResolveFastModeL( 
        CSdpDocument& aSDP )
    {
    MUS_LOG( "mus: [MUSAVA]: -> CMusAvaCapability::ResolveFastModeL" )
    MusSettingsKeys::TFastMode mode = MusSettingsKeys::EFastModeOff;
    if ( MultimediaSharingSettings::FastStartupModeL() == 
            MusSettingsKeys::EFastModeOn )
        {
        MUS_LOG( "mus: [MUSAVA] Checking if a=keywds:fastmode present..." )
        const CSdpAttributeField* keywds = CMusAvaCapability::Attribute( 
                    MusAvaCapabilityContext::SDPStringL( 
                        SdpCodecStringConstants::EAttributeKeywds ),
                    aSDP );
        if ( keywds && 
             keywds->Value().Compare( KCapabilitySDPAttributeFastMode ) == 0 )        
            {
            mode = MusSettingsKeys::EFastModeOn;
            }
        }
    MUS_LOG( "mus: [MUSAVA]: <- CMusAvaCapability::ResolveFastModeL" )
    return mode;
    }

// -----------------------------------------------------------------------------
// CMusAvaCapability::ResolveFastModeL
// -----------------------------------------------------------------------------
// 
void CMusAvaCapability::AddFastModeL( CSdpDocument& aSdp )
    {
    MUS_LOG( "mus: [MUSAVA]: -> CMusAvaCapability::AddFastModeL" )
    if ( MultimediaSharingSettings::FastStartupModeL() == 
            MusSettingsKeys::EFastModeOn )
        {
        CSdpAttributeField* fastmode = CSdpAttributeField::NewLC(
             MusAvaCapabilityContext::SDPStringL( 
                 SdpCodecStringConstants::EAttributeKeywds ), 
             KCapabilitySDPAttributeFastMode );
                                           
        aSdp.AttributeFields().AppendL( fastmode );
        CleanupStack::Pop( fastmode );
        }    
    MUS_LOG( "mus: [MUSAVA]: <- CMusAvaCapability::AddFastModeL" )        
    }


// -----------------------------------------------------------------------------
// CMusAvaCapability::Attribute
// -----------------------------------------------------------------------------
// 
const CSdpAttributeField* CMusAvaCapability::Attribute( 
                                                const RStringF& aAttribute,
                                                CSdpDocument& aSDP )
    {
    const CSdpAttributeField* field = NULL;
    
    TBool found = EFalse;
    RPointerArray<CSdpAttributeField>& attributes = aSDP.AttributeFields();
    
    for ( TInt i=0; i < attributes.Count() && !found; i++ )
        {
        if ( attributes[ i ]->Attribute() == aAttribute )
            {
            field = attributes[ i ];
            found = ETrue;
            }
        }
        
    if ( !found )
        {
        TRAP_IGNORE( field = Attribute( MusAvaCapabilityContext::SDPStringL( 
                                    SdpCodecStringConstants::EMediaVideo ),
                                aAttribute,
                                aSDP ) );
        }
    return field;
    }


// -----------------------------------------------------------------------------
// CMusAvaCapability::Attribute
// -----------------------------------------------------------------------------
// 
const CSdpAttributeField* CMusAvaCapability::Attribute( 
                                                const RStringF& aMedia,
                                                const RStringF& aAttribute,
                                                CSdpDocument& aSDP )
    {
    const CSdpAttributeField* field = NULL;
    
    TBool found = EFalse;
    RPointerArray<CSdpMediaField>& mediaFields = aSDP.MediaFields();
    
    for ( TInt i=0; i < mediaFields.Count() && !found; i++ )
        {
        if ( mediaFields[ i ]->Media() == aMedia )
            {
            RPointerArray<CSdpAttributeField>& attributes = 
                                mediaFields[ i ]->AttributeFields();

            for ( TInt j=0; j < attributes.Count() && !found; j++ )
                {
                if ( attributes[ j ]->Attribute() == aAttribute )
                    {
                    field = attributes[ j ];
                    found = ETrue;
                    }
                }
            }
        }
    return field;
    }


// --------------------------------------------------------------------------
// CMusAvaCapability::AddContactHeaderL
// --------------------------------------------------------------------------
//
void CMusAvaCapability::AddContactHeaderL(
                        CSIPServerTransaction& aQuery,
                        RPointerArray<CSIPHeaderBase>& aResponseHeaders )
    {
    MUS_LOG( "mus: [MUSAVA]: -> CMusAvaCapability::AddContactHeaderL" )

    const CUri8& uri = aQuery.RequestElements()->RemoteUri();

    
    TBool cingular = 
                  MultimediaSharingSettings::OperatorVariantSettingL() ==
                  MusSettingsKeys::EOperatorSpecific;
          
    TBool alreadyRequested = iExchange.QueryObserver().
                    CapabilitiesResolvedForCingular() && cingular;

    CSIPContactHeader* contact =  NULL;
    
    if ( alreadyRequested )
        {
        contact = MusAvaCapabilityContext::ContactHeaderLC( uri, 
            SIPStrings::StringF( SipStrConsts::EEmpty ) );
        }
    else
        {
        //Contact header with feature tag
	    contact = MusAvaCapabilityContext::ContactHeaderLC( uri, Feature() );
        }
        
    aResponseHeaders.AppendL( contact );
    CleanupStack::Pop( contact );

    MUS_LOG( "mus: [MUSAVA]: <- CMusAvaCapability::AddContactHeaderL" )
    }
    

// ----------------------------------------------------------------------------
// CMusAvaCapability::OriginFieldL
// ----------------------------------------------------------------------------
//    
CSdpOriginField* CMusAvaCapability::OriginFieldL( 
                                                const RStringF& aNetType,
                                                const TDesC8& aLocalHost,
                                                const RStringF& aHostType )
    {
#ifdef CAPABILITY_UNIT_TESTING    
    TInt64 sessionID = 452027953;
#else    
    TTime now;
    now.UniversalTime();
    TInt64 rand = now.Int64();
    TInt64 sessionID = Math::Rand( rand );
#endif
    
    return CSdpOriginField::NewL( KOriginFieldUser,
                                 sessionID,
                                 sessionID,
                                 aNetType,
                                 aHostType,
                                 aLocalHost );    
    }
  
// -----------------------------------------------------------------------------
// CMusAvaCapability::MediaLineLC
// -----------------------------------------------------------------------------
// 
CSdpMediaField* CMusAvaCapability::MediaLineLC( const RStringF& aMedia,
                                                const TDesC8& aFormatList ) const
    {
	return CSdpMediaField::NewLC( aMedia,
                            NULL,
                            MusAvaCapabilityContext::SDPStringL( 
                                SdpCodecStringConstants::EProtocolRtpAvp ), 
                            aFormatList );
    }

// -----------------------------------------------------------------------------
// CMusAvaCapability::RtpMapLineLC
// ------------------------------------------------ -----------------------------
// 
CSdpFmtAttributeField* CMusAvaCapability::RtpMapLineLC( 
                                                const TDesC8& aCodec,
                                                const TUint64 aPayloadType,
				                                const TUint64 aClockRate,
				                                const TDesC8& aParams )
    {
    
    HBufC8* clockrate  = HBufC8::NewLC( KMaxNumAsStringSize );
    clockrate->Des().Num( aClockRate, EDecimal );
	HBufC8* payload  = HBufC8::NewLC( KMaxNumAsStringSize );
    payload->Des().Num( aPayloadType, EDecimal );
    
    			
	TSdpRtpmapValue rtpmapValue( aCodec, 
								 *clockrate, 
								 aParams );
								 
	
	HBufC8* rtpmap = rtpmapValue.EncodeL();
	CleanupStack::PushL( rtpmap );
	
	CSdpFmtAttributeField* rtpmapAttribute = 
	    CSdpFmtAttributeField::NewL( MusAvaCapabilityContext::SDPStringL( 
	                            SdpCodecStringConstants::EAttributeRtpmap ),
	                        *payload,
	                        *rtpmap );
	    
	CleanupStack::PopAndDestroy( rtpmap ); 
	CleanupStack::PopAndDestroy( payload ); 
	CleanupStack::PopAndDestroy( clockrate ); 
    
    CleanupStack::PushL( rtpmapAttribute );
    
    return rtpmapAttribute;
    }



