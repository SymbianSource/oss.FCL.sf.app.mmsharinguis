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
* Description:   Sent SWIS capability query (OPTIONS)
*
*/


#include "musavacapabilityquery.h"
#include "muslogger.h"
#include "mussettings.h"
#include "mussettingskeys.h"
#include "musavacapability.h"
#include "musavacapabilitycontext.h"
#include "mussesseioninformationapi.h"
#include "musavacapabilityexchange.h"
#include "mmusavacapabilityqueryobserver.h"

#include <sipconnection.h>
#include <sipclienttransaction.h>
#include <sipmessageelements.h>
#include <sipresponseelements.h>
#include <sdpdocument.h>
#include <sipcontenttypeheader.h>
#include <sdpattributefield.h>
#include <sipcontactheader.h>
#include <sipstrings.h>
#include <sipaddress.h>
#include <e32property.h>
#include <escapeutils.h>
#include <badesca.h>


// --------------------------------------------------------------------------
// C++ constructor
// --------------------------------------------------------------------------
//
CMusAvaCapabilityQuery::CMusAvaCapabilityQuery(
                                        CMusAvaCapability& aCapability,
                                        CSIPConnection& aSIPConnection,
                                        CSIPProfile& aProfile )
 
    : CMusAvaCapabilityQueryBase( aCapability, aSIPConnection, aProfile ),
    iRetrying( EFalse )
    {
    }
 
// --------------------------------------------------------------------------
// Symbian two-phase constructor
// --------------------------------------------------------------------------
//
CMusAvaCapabilityQuery* CMusAvaCapabilityQuery::NewL( 
                                        CMusAvaCapability& aCapability,
                                        CSIPConnection& aSIPConnection,
                                        CSIPProfile& aProfile,
                                        const TDesC& aSipAddress )

    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityQuery::NewL" )
    
    CMusAvaCapabilityQuery* self = NULL;
    
    self = new (ELeave) CMusAvaCapabilityQuery( aCapability, 
                                            aSIPConnection,
                                            aProfile );
    CleanupStack::PushL( self );
    self->ConstructL( aSipAddress );
    CleanupStack::Pop( self );
  
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityQuery::NewL" )
    return self;
    }

// --------------------------------------------------------------------------
// Symbian second-phase constructor
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQuery::ConstructL( const TDesC& aSipAddress )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityQuery::ConstructL" )

    CMusAvaCapabilityQueryBase::ConstructL( aSipAddress );
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityQuery::ConstructL" )
    }

// --------------------------------------------------------------------------
// C++ destructor
// --------------------------------------------------------------------------
//
CMusAvaCapabilityQuery::~CMusAvaCapabilityQuery()
    {
    MUS_LOG( 
        "mus: [MUSAVA] -> CMusAvaCapabilityQuery::~CMusAvaCapabilityQuery" )

    delete iTimer;
            
    MUS_LOG( 
        "mus: [MUSAVA] <- CMusAvaCapabilityQuery::~CMusAvaCapabilityQuery" )
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQuery::Prepare
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQuery::Prepare( 
                        RPointerArray<CSIPHeaderBase>& /*aRequestHeaders*/ )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityQuery::Prepare" )
    
    if ( !Retrying() )
        {
        MUS_LOG( "mus: [MUSAVA] query prepared" )
        SetState( ECapabilityQueryPrepared );
        }
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityQuery::Prepare" )
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQuery::DoCompletedL
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQuery::DoCompletedL( 
                                    const CSIPClientTransaction& aResponse )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityQuery::DoCompletedL" )
    
    const CSIPResponseElements* response = aResponse.ResponseElements();
    User::LeaveIfError( !response ? KErrGeneral : KErrNone );
    
    TUint statusCode = response->StatusCode();

    MUS_LOG1( "mus: [MUSAVA] result %d", statusCode )
    
    switch ( statusCode )
        {
        case KMUSAVASIP200:
            {
            DoCompleted200OKL( aResponse ); 
            break;
            }
        case KMUSAVASIPNotImplemented:
            {
            MUS_LOG( "mus: [MUSAVA] => OK" )
            SetResult( KCapabilityCapabilitesReady );
            SetState( ECapabilityQueryCompleted );
            break;
            }
        case KMUSAVASIPForbidden:
            {
            MUS_LOG( "mus: [MUSAVA] => FAILED" )
            if ( MultimediaSharingSettings::OperatorVariantSettingL() ==
                  MusSettingsKeys::EOperatorSpecific )
                {
                SetResult( KCapabilityCapabilitiesForbidden );
                SetState( ECapabilityQueryCompleted );
                // set forbidden property
                TInt result = 
                    RProperty::Set( 
                        NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMUSForbidden,
                        ( TInt ) NMusSessionInformationApi::EMUSForbidden );
                MUS_LOG1( "mus: [MUSAVA]    Property::Ser( KMUSForbidden )\
                            returns %d", result )                
                }
            else
                {
                SetResult( KCapabilityCapabilitesNotFound );
                SetState( ECapabilityQueryCompleted );
                }
            break;
            }
        case  KMUSAVASIPUnsupportedURIScheme:
        MUS_LOG( "mus: [MUSAVA] Unsupported URI Schem" )
            SetState( ECapabilityQueryCompleted );
            break;
        case KMUSAVASIPNotFound:
        case KMUSAVASIPTimeout:
        case KMUSAVASIPTemporarilyUnavailable:
            {
            DoRetryL();
            break;
            }
        default:
            {
            MUS_LOG( "mus: [MUSAVA] => FAILED" )
            SetResult( KCapabilityCapabilitesNotFound );
            SetState( ECapabilityQueryCompleted );
            break;
            }
        }
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityQuery::DoCompletedL" )
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQuery::DoCompleted200OKL
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQuery::DoCompleted200OKL( 
//                        const CSIPResponseElements& aResponse )
                                    const CSIPClientTransaction& aResponse )
    {
    SetState( ECapabilityQueryCompleted );

    // check if P-Asserted-Identity header exists
    _LIT8( KHeader,"P-Asserted-Identity" );
    RStringF p = SIPStrings::Pool().OpenFStringL( KHeader() );
    CleanupClosePushL( p );
	
    TInt count = MusAvaCapabilityContext::HeaderCount( p, 
                    aResponse.ResponseElements()->MessageElements() );
            
    if ( count > 0 )
        {
        TUint index = 0;
        const CSIPHeaderBase* pAssertedId =  MusAvaCapabilityContext::Header(
                        p, 
                        aResponse.ResponseElements()->MessageElements(), 
                        index );
                        
        if ( pAssertedId )
            { // get SIP uri
            HBufC8* value = pAssertedId->ToTextValueLC();
            CSIPAddress* address = CSIPAddress::DecodeL( *value );
            CleanupStack::PushL( address );
                 
            const TDesC8& uriInPAssertedId = address->Uri8().Uri().UriDes();
        	if ( uriInPAssertedId.Length() > 0 )
    	        {
    		    HBufC* uri = EscapeUtils::ConvertToUnicodeFromUtf8L( 
    		                uriInPAssertedId );
        		CleanupStack::PushL( uri );   
        		Capability().Exchange().QueryObserver().
        		CapabilitiesResolvedL( *uri );
        		CleanupStack::PopAndDestroy( uri );
        		}
            CleanupStack::PopAndDestroy( 2 ); //address, value
            }
        }
    CleanupStack::PopAndDestroy( 1 ); //p
    
    const CSIPContentTypeHeader* contentTypeHeader = NULL;
        
    const CSIPResponseElements* response = aResponse.ResponseElements();

    contentTypeHeader = response->MessageElements().ContentType();
    
    if ( contentTypeHeader &&
         contentTypeHeader->
         MediaType().Compare( KMUSAVASIPMediaTypeApplication ) == 0 &&
         contentTypeHeader->
         MediaSubtype().Compare( KMUSAVASIPMediaSubTypeSDP ) == 0 )
        {
        CSdpDocument* sdp = 
            CSdpDocument::DecodeL( response->MessageElements().Content() );
        CleanupStack::PushL( sdp );
        
        TBool codec = ValidateAndStoreCodecsL( *sdp );
        TBool attributes = ValidateAttributesL( *sdp );
        TBool featureTag = ValidateContactL( aResponse );
        TBool operatorVariant = 
            MultimediaSharingSettings::OperatorVariantSettingL() ==
            MusSettingsKeys::EOperatorSpecific;
       
        if ( ( codec && attributes && featureTag  ) ||
            ( codec && !operatorVariant && featureTag ) )
        
            {
            MUS_LOG( "mus: [MUSAVA] => OK" )
            SetResult( KCapabilityCapabilitesReady );
            }
        
        else
            {
            MUS_LOG( "mus: [MUSAVA] => FAILED due SDP validation" )
            SetResult( KCapabilityCapabilitesNotFound );
            }
        
        CleanupStack::PopAndDestroy( sdp );
        
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA] => FAILED due wrong content type" )
        SetResult( KCapabilityCapabilitesNotFound );
        }
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQuery::ValidateCodecL
// --------------------------------------------------------------------------
//
TBool CMusAvaCapabilityQuery::ValidateAndStoreCodecsL( CSdpDocument& aSDP )
    {
    MUS_LOG("mus: [MUSAVA]  -> CMusAvaCapabilityQuery::ValidateAndStoreCodecsL" )

    TBool valid = EFalse;
    
    CDesCArrayFlat* codecs = CMusAvaCapability::ResolveCodecsL( aSDP );
    CleanupStack::PushL( codecs );
    
    Capability().Exchange().QueryObserver().VideoCodecsResolvedL( *codecs );
    MUS_LOG1("mus: [MUSAVA]  -> codecs count = %d", codecs->MdcaCount() )

    if ( codecs->MdcaCount() > 0 )
        {
        valid = ETrue;
        }

    codecs->Reset();
    CleanupStack::PopAndDestroy( codecs );
    
    MUS_LOG("mus: [MUSAVA]  <- CMusAvaCapabilityQuery::ValidateAndStoreCodecsL" )
    return valid;
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQuery::ValidateAttributesL
// --------------------------------------------------------------------------
//
TBool CMusAvaCapabilityQuery::ValidateAttributesL( CSdpDocument& aSDP )
    {
    TBool valid = EFalse;
        
    const CSdpAttributeField* application = NULL;
    const CSdpAttributeField* type = NULL;
    RStringF attrTypeName = MusAvaCapabilityContext::SDPStringL( 
                                        KCapabilitySDPAttributeNameType );
    CleanupClosePushL( attrTypeName );
    
    // "a=application:comgsma.rts"
    application = CMusAvaCapability::Attribute( 
            MusAvaCapabilityContext::SDPStringL( 
                SdpCodecStringConstants::EMediaApplication ),
            aSDP );

    // "a=type:videolive"
    type = CMusAvaCapability::Attribute( attrTypeName, aSDP );
    CleanupStack::PopAndDestroy();//attrTypeName
    
    if ( MultimediaSharingSettings::OperatorVariantSettingL() ==
                MusSettingsKeys::EOperatorSpecific )
        {
        valid = application &&
            application->Value().Compare( KCapabilitySwisApplication ) == 0 &&
            type &&
            type->Value().Compare( KCapabilitySDPAttributeType ) == 0;
        }
    else
        {
        valid = ( application &&
            application->Value().Compare( KCapabilitySwisApplication ) == 0 ) ||
            ( type &&
            type->Value().Compare( KCapabilitySDPAttributeType ) == 0 ) ;
        
        }
    
    return valid;        
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQuery::ValidateContactL
// --------------------------------------------------------------------------
//
TBool CMusAvaCapabilityQuery::ValidateContactL( 
                                    const CSIPClientTransaction& aResponse )
    {
    TBool valid = EFalse;
    RStringF falseParam = MusAvaCapabilityContext::SIPStringL( 
                                                    KMUSAVASIPParamFalse );
    CleanupClosePushL( falseParam );

    const CSIPContactHeader* contact = 
        MusAvaCapabilityContext::ContactHeaderL( aResponse );
                                             
    if ( !contact )
        {
        MUS_LOG( "STCE: => FAILED due lack of contact header" )
        }
    else
        {
        if ( contact->HasParam( Capability().Feature() ) && 
            !contact->HasParam( falseParam ) )
            {
            valid = ETrue;
            }
        else
            {
            MUS_LOG( "STCE: => FAILED due wrong contact header" )
            }        
        }
    CleanupStack::PopAndDestroy();//falseParam
    
    return valid;
    }
    
// --------------------------------------------------------------------------
// CMusAvaCapabilityQuery::DoRetryL
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQuery::DoRetryL()
    {

    if ( !iTimer )
        {
        MUS_LOG( "mus: [MUSAVA] => RE-TRYING" )
        
        iTimer = CMusAvaCapabilityTimer::NewL( *this );
        iTimer->After( KCapabilityTimervalue );
        Retrying() = ETrue;
        SetResult( KCapabilityQueryNotReady );
        SetState( ECapabilityQueryCreated );
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA] => FAILED due failing retry" )
        Retrying() = EFalse;
        SetResult( KCapabilityCapabilitesNotFound );
        SetState( ECapabilityQueryCompleted );
        delete iTimer;
        iTimer = NULL;
        }
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQuery::OnExpired
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQuery::OnExpiredL( TInt /*aStatus*/ )
    {
    Retrying() = EFalse;
        
    ExecuteL();
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQuery::Retrying
// --------------------------------------------------------------------------
//
TBool& CMusAvaCapabilityQuery::Retrying()
    {
    return iRetrying;
    }
