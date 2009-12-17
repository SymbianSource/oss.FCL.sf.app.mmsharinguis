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
* Description:   Provide interface for the client requestin availability class.
*
*/


#include "musavadefaultoptionhandler.h"
#include "musavasharedobject.h"
#include "muslogger.h"
#include "musavasip.h"
#include "musavasettingsimp.h"
#include "sipservertransaction.h"
#include "mussettings.h"
#include "musavacapability.h"
#include "musavacapabilitycontext.h"
#include "musavanetworkmodestatus.h"
#include <sipstrings.h>
#include <siptransactionbase.h>
#include <sipheaderbase.h>
#include <sipservertransaction.h>
#include <sipcontenttypeheader.h>
#include <sdpcodecstringpool.h>
#include <sipresponseelements.h>
#include <siprequestelements.h>
#include <sipmessageelements.h>
#include <sipconnection.h>
#include <sipcontactheader.h>
#include <sdpdocument.h>
#include <sdpattributefield.h>
#include <sdpmediafield.h>
#include <sdpconnectionfield.h>
#include <sdporiginfield.h>
#include <sdprtpmapvalue.h>
#include <sdpfmtattributefield.h>
#include <e32math.h>
#include <uriutils.h>


const TInt KMaxNumAsStringSize = 16;
_LIT8( KMUSAVAOriginFieldUser, "-");


// --------------------------------------------------------------------------
// Symbian two-phase constructor
// --------------------------------------------------------------------------
//
CMusAvaDefaultOptionHandler* CMusAvaDefaultOptionHandler::NewL(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp&  aConcreteSettings  )
    {
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::NewL()" )
    CMusAvaDefaultOptionHandler* self = CMusAvaDefaultOptionHandler::NewLC(
        aObserver,
        aConcreteSettings );
    CleanupStack::Pop( self );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::NewL()" )
    return self;
    }


// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaDefaultOptionHandler* CMusAvaDefaultOptionHandler::NewLC(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aConcreteSettings  )
    {
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::NewLC()" )
    CMusAvaDefaultOptionHandler* self = new( ELeave ) 
        CMusAvaDefaultOptionHandler( aObserver, aConcreteSettings );
    CleanupStack::PushL( self );
    self->ConstructL();

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::NewLC()" )
    return self;
    }


// -------------------------------------------------------------------------
//  Constructor.
// -------------------------------------------------------------------------
//
CMusAvaDefaultOptionHandler::CMusAvaDefaultOptionHandler(
                        MMusAvaAvailabilityObserver& aObserver,
                        CMusAvaSettingsImp& aConcreteSettings  )
    :CMusAvaAvailability( aObserver ),
    iSettings( aConcreteSettings )
    {
    }


// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMusAvaDefaultOptionHandler::ConstructL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultOptionHandler::ConstructL()" )
    iSharedObj = CMusAvaSharedObject::GetSingletonL();
    iSharedObj->MusAvaSip().AddAdapterL( 
                                    ( MMusAvaSipConnectionAdapter& ) *this );
    iSharedObj->MusAvaSip().AddAdapterL( ( MMusAvaSipAdapter& ) *this );
    SIPStrings::OpenL();
    SdpCodecStringPool::OpenL();
    iOperatorVariant = MultimediaSharingSettings::OperatorVariantSettingL(); 
    // set this only for static availability modules
    iState = MMusAvaObserver::EMusAvaStatusAvailable ; 
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::ConstructL()" )
    }


// ---------------------------------------------------------------------------
//  Destructor.
// ---------------------------------------------------------------------------
//
CMusAvaDefaultOptionHandler::~CMusAvaDefaultOptionHandler()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultOptionHandler::\
        ~CMusAvaDefaultOptionHandler()" )
    
    if ( iSharedObj )        
        {
        iSharedObj->DeleteSingleton();    
        }
    SIPStrings::Close();
    SdpCodecStringPool::Close();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::\
        ~CMusAvaDefaultOptionHandler()" )
    }


// ---------------------------------------------------------------------------
//  A SIP request outside a dialog has been received from the network.
// ---------------------------------------------------------------------------
//
TInt CMusAvaDefaultOptionHandler::IncomingRequest(
    CSIPServerTransaction* aTransaction )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultOptionHandler::\
        IncomingRequest()" )
    TInt err = KErrNotFound;
    if( aTransaction->Type() == SIPStrings::StringF( SipStrConsts::EOptions ) 
        //&& !Available( MMusAvaObserver::EMusAvaOptionHandler ) 
        )
        {
        TRAP( err, IncomingOptionRequestL( aTransaction ) )
	    if( !err )
	    	{
	    	delete aTransaction;	
	    	}
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA]  Not OPTIONS request or not handled in defaultOptionHandler" )
        }
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::\
            IncomingRequest()" )
    return err;
    }


   

// ---------------------------------------------------------------------------
//  Starts the loopy execution.
// ---------------------------------------------------------------------------
//
void CMusAvaDefaultOptionHandler::DoExecuteL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultOptionHandler::\
        ExecutePatternL()" )
    // set status available
    SetState( MMusAvaObserver::EMusAvaStatusAvailable );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::\
        ExecutePatternL()" )
    }


// ---------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::Name
//
// ---------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityName CMusAvaDefaultOptionHandler::Name()
    {
    MUS_LOG( "mus: [MUSAVA]  CMusAvaDefaultOptionHandler::Name()" )
    return MMusAvaObserver::EMusAvaDefaultOptionHandler;
    }


// ---------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::IncomingRequest
//
// ---------------------------------------------------------------------------
//
TInt CMusAvaDefaultOptionHandler::IncomingRequest(
                                TUint32 aIapId,
                                CSIPServerTransaction* aTransaction )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultOptionHandler::\
        IncomingRequest()" )
    MUS_LOG1( "mus: [MUSAVA]    Iap ( %d )", aIapId )
    TInt err = KErrNotFound;
    CSIPConnection* connection = NULL;
    if( aTransaction->Type() == SIPStrings::StringF( SipStrConsts::EOptions ) 
        && !Available( MMusAvaObserver::EMusAvaOptionHandler ) )
        {
       	MUS_LOG( "mus: [MUSAVA]  create CSIPConnection" )
       	TRAP( err, connection = CSIPConnection::NewL( 
       	    iSharedObj->MusAvaSip().Sip(),
       	    aIapId,
       	    iSharedObj->MusAvaSip().ConnectionObserver() ) );

        if( err != KErrNone )
            {
            MUS_LOG1( "mus: [MUSAVA]    CSIPConnection error ( %d )", err )
            }
        else
            {
    	    MUS_LOG( "mus: [MUSAVA]  create CSIPConnection" )
    	    TRAP_IGNORE( IncomingOptionRequestL ( aTransaction ) ) 
            delete connection;
            delete aTransaction;
            }
       	}
    else
        {
        MUS_LOG( "mus: [MUSAVA]  Not OPTIONS request or not handled in defaultOptionHandler" )
       	}
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::\
        IncomingRequest()" )
    return err;
    }


// ---------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::TimedOut
//
// ---------------------------------------------------------------------------
//
TInt CMusAvaDefaultOptionHandler::TimedOut(CSIPServerTransaction& 
    /*aTransaction*/ )
    {
    MUS_LOG( "mus: [MUSAVA]  CMusAvaDefaultOptionHandler::ConstructL()" )
    return KErrNotSupported;
    }


// -------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::IncomingOptionRequestL
//
// -------------------------------------------------------------------------
//
void CMusAvaDefaultOptionHandler::IncomingOptionRequestL(
                                CSIPServerTransaction* aTransaction )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultOptionHandler::\
        IncomingOptionRequestL()" )

    MusSettingsKeys::TActivation activation = 
                            MultimediaSharingSettings::ActivationSettingL();
            
    if ( activation == MusSettingsKeys::ENever )
    	{
        MUS_LOG( "mus: [MUSAVA]  Sending 603 Decline" )
        NoCapabilityResponseL( aTransaction, 
                               KMUSAVASIPDecline, 
                               SipStrConsts::EPhraseDecline );    		        	        
    	}
   	else
   		{                        
        MUS_LOG( "mus: [MUSAVA]  Sending 480 Temporarily Unavilable" )
        NoCapabilityResponseL( aTransaction, 
                               KMUSAVASIPTemporarilyUnavailable, 
                               SipStrConsts::EPhraseTemporarilyNotAvailable );	        
        }

    MUS_LOG( "mus: [MUSAVA]  <- \
        CMusAvaDefaultOptionHandler::IncomingOptionRequest()" )
    }

// ------------------------------------------------------------------------
// Send Response to reques.
//
// ------------------------------------------------------------------------
void CMusAvaDefaultOptionHandler::ResponseL(
                                CSIPResponseElements* aResponseElements,
                                CSIPServerTransaction* aTransaction )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaDefaultOptionHandler::ResponseL" )

    if ( ( aTransaction->StateL() == CSIPTransactionBase::ETrying ||
         aTransaction->StateL() == CSIPTransactionBase::EProceeding ) &&
         aTransaction->ResponseAllowed() )
        {
        aTransaction->SendResponseL( aResponseElements );
        MUS_LOG( "   Send response" )
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::ResponseL" )
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA]  Not allow to sent response " )
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::ResponseL" )
        User::Leave( KErrNotReady );
        }
    }


// ------------------------------------------------------------------------
// Send response to request.
//
// ------------------------------------------------------------------------
void CMusAvaDefaultOptionHandler::NoCapabilityResponseL( 
                                        CSIPServerTransaction* aTransaction,
                                        TUint aStatusCode,
                                        SipStrConsts::TStrings aReason )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaDefaultOptionHandler::SendResponseL()" )
    MUS_LOG( "mus: [MUSAVA]  Create response elements " )
    CSIPResponseElements* respElem = NULL;

	if( iOperatorVariant == MusSettingsKeys::EOperatorSpecific )
		{
		MUS_LOG( "mus: [MUSAVA]  Operator Specific response" )
	    respElem = CreateResponseL( *aTransaction );
	    CleanupStack::PushL( respElem );
		}
	else
		{
		MUS_LOG( "mus: [MUSAVA]	Standard response" )
		RStringF reasonPhrase =
	        SIPStrings::StringF( aReason );
	    respElem =
		    CSIPResponseElements::NewLC( aStatusCode, reasonPhrase );
		}	

    ResponseL( respElem, aTransaction );
    MUS_LOG( "mus: [MUSAVA]  Response sent " )
    CleanupStack::Pop( respElem );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaDefaultOptionHandler::SendResponseL()" )
    }


// ------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::CreateResponseL
//
// ----------------------------------------------------------------------------
//
CSIPResponseElements* CMusAvaDefaultOptionHandler::CreateResponseL( 
                                        CSIPServerTransaction& aTransaction )
    {
    HBufC8* sdpBody = NULL;
    CSIPResponseElements* respElem =
        CSIPResponseElements::NewLC( KMUSAVASIP200,
        SIPStrings::StringF(SipStrConsts::EPhraseOk) );

    RPointerArray<CSIPHeaderBase> responseHeaders;
    CSIPHeaderBase::PushLC( &responseHeaders );
    
    const CUri8& uri = aTransaction.RequestElements()->RemoteUri();
    CSIPContactHeader* contact = MusAvaCapabilityContext::ContactHeaderLC( 
            uri, SIPStrings::StringF( SipStrConsts::EEmpty ) );
    responseHeaders.AppendL( contact );
    CleanupStack::Pop( contact );
    
    CSIPMessageElements& messageElements = respElem->MessageElements();
    messageElements.SetUserHeadersL( responseHeaders );
    CleanupStack::Pop( &responseHeaders );
    
    CSIPContentTypeHeader* contenType =
        CSIPContentTypeHeader::NewLC( SIPStrings::StringF(
            SipStrConsts::EApplication).DesC(), 
            KMUSAVASIPMediaSubTypeSDP );

    // Set content
    CSdpDocument* sdpDocument = CreateSDPL( aTransaction );
    CleanupStack::PushL( sdpDocument );
    sdpBody = ToTextL( *sdpDocument );
    CleanupStack::PopAndDestroy( sdpDocument );
    CleanupStack::PushL( sdpBody );
    messageElements.SetContentL( sdpBody, contenType );
    CleanupStack::Pop( sdpBody );
    CleanupStack::Pop( contenType );
    CleanupStack::Pop( respElem );
   	return respElem;
    }


// ------------------------------------------------------------------------
// Create SDP response.
//
// ------------------------------------------------------------------------
CSdpDocument* CMusAvaDefaultOptionHandler::CreateSDPL( 
                                    CSIPServerTransaction& aTransaction  )
    {
	MUS_LOG( "mus: [MUSAVA] -> CMusAvaDefaultOptionHandler::CreateSDPL" )
    // Origin field
	TPtrC8 localHost;
	RStringF hostType;
	RStringF netType = SDPStringL( SdpCodecStringConstants::ENetType );
	CSdpDocument* sdpDocument = CSdpDocument::NewLC();
	LocalHostL( aTransaction,localHost,hostType);
	CSdpOriginField* originField = CreateOriginFieldL(localHost,hostType);
	sdpDocument->SetOriginField(originField);
	// Connection field
	CSdpConnectionField* connectionField =
        CSdpConnectionField::NewL( netType, hostType, localHost );
    sdpDocument->SetConnectionField( connectionField );

    MUS_LOG( "mus: [MUSAVA] Adding application attribute to SDP" )
    //application
    CSdpAttributeField* application =
        CSdpAttributeField::NewLC( 
            SDPStringL( SdpCodecStringConstants::EMediaApplication ),
            KCapabilitySwisApplication );

    sdpDocument->AttributeFields().AppendL( application );
    CleanupStack::Pop( application );

    MUS_LOG( "mus: [MUSAVA] Adding type attribute to SDP" )
    //type
    RStringF typeStr = SDPStringL( KCapabilitySDPAttributeNameType );
    CleanupClosePushL( typeStr );
    CSdpAttributeField* type =
        CSdpAttributeField::NewL( typeStr, KCapabilitySDPAttributeType );
    CleanupStack::PopAndDestroy();//typeStr
    CleanupStack::PushL( type );

    sdpDocument->AttributeFields().AppendL( type );
    CleanupStack::Pop( type );


    MUS_LOG( "mus: [MUSAVA] Adding media line to SDP" )
    //media line
    CSdpMediaField* mediaLine =
        MediaLineLC( SDPStringL( SdpCodecStringConstants::EMediaVideo ),
                     KCapabilitySwisFormatList );

    //rtpmap
    MUS_LOG( "mus: [MUSAVA] Adding rtpmap to SDP" )
    //adding H.264 codec to SDP
    CSdpFmtAttributeField* rtpMapH264 = 
        RtpMapLineLC( KCapabilitySDPAttributeCodecH264,
                    KCapabilitySDPAttributePayloadTypeH264,
                    KCapabilitySDPAttributeClockrate ); 
    
    mediaLine->FormatAttributeFields().AppendL( rtpMapH264 );
    CleanupStack::Pop( rtpMapH264 );

    //H.263 codec
    CSdpFmtAttributeField* rtpMapH263 = RtpMapLineLC( 
                                        KCapabilitySDPAttributeCodecH263,
                                        KCapabilitySDPAttributePayloadType,
                                        KCapabilitySDPAttributeClockrate );

    mediaLine->FormatAttributeFields().AppendL( rtpMapH263 );
    CleanupStack::Pop( rtpMapH263 );

    sdpDocument->MediaFields().AppendL( mediaLine );
    CleanupStack::Pop( mediaLine );
	CleanupStack::Pop( sdpDocument );
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaDefaultOptionHandler::CreateSDPL" )
    return sdpDocument;
	}


// ----------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::CreateOriginFieldL
//
// ----------------------------------------------------------------------------
//
CSdpOriginField* CMusAvaDefaultOptionHandler::CreateOriginFieldL(
    const TDesC8& aLocalHost,
    const RStringF& aHostType) const
    {
    #ifdef UNIT_TESTING
    TInt64 sessionID = 452027953;
#else
    TTime now;
    now.UniversalTime();
    TInt64 rand = now.Int64();
    TInt64 sessionID = Math::Rand( rand );
#endif

    return CSdpOriginField::NewL( 
                            KMUSAVAOriginFieldUser,
                            sessionID,
                            sessionID,
                            SDPStringL( SdpCodecStringConstants::ENetType ),
                            aHostType,
                            aLocalHost);
    }


// ----------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::LocalHostL
//
// ----------------------------------------------------------------------------
//
void CMusAvaDefaultOptionHandler::LocalHostL(
                                    const CSIPServerTransaction& aRequest,
                                    TPtrC8& aLocalHost,
                                    RStringF& aHostType )
    {
    RStringPool strPool = SdpCodecStringPool::StringPoolL();

    const CUri8& uri = aRequest.RequestElements()->RemoteUri();

    aLocalHost.Set( uri.Uri().Extract( EUriHost ) );

    if ( UriUtils::HostType( aLocalHost ) == UriUtils::EIPv6Host )
        {
        aHostType = strPool.StringF( SdpCodecStringConstants::EAddressType,
                                     SdpCodecStringPool::StringTableL() );
        }
    else
        {
        aHostType = strPool.StringF(
                                SdpCodecStringConstants::EAddressTypeIP4,
                                SdpCodecStringPool::StringTableL() );
        }
    }


// ----------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::SDPStringL
//
// ----------------------------------------------------------------------------
//
RStringF CMusAvaDefaultOptionHandler::SDPStringL( const TDesC8& aString )
    {
    return SdpCodecStringPool::StringPoolL().OpenFStringL( aString );
    }


// ----------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::SDPStringL
//
// ----------------------------------------------------------------------------
//
RStringF CMusAvaDefaultOptionHandler::SDPStringL( TInt aStringIndex )
    {
    return SdpCodecStringPool::StringPoolL().
        StringF( aStringIndex, SdpCodecStringPool::StringTableL() );
    }


// -----------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::MediaLineLC
//
// -----------------------------------------------------------------------------
//
CSdpMediaField* CMusAvaDefaultOptionHandler::MediaLineLC( 
                                            const RStringF& aMedia,
                                            const TDesC8& aFormatList ) const
    {
	return CSdpMediaField::NewLC( aMedia,
                            NULL,
                            SDPStringL(
                                SdpCodecStringConstants::EProtocolRtpAvp ),
                            aFormatList );
    }


// ----------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::RtpMapLineLC
//
// ----------------------------------------------------------------------------
//
CSdpFmtAttributeField* CMusAvaDefaultOptionHandler::RtpMapLineLC(
                                                const TDesC8& aCodec,
                                                const TUint64 aPayloadType,
				                                const TUint64 aClockRate,
				                                const TDesC8& aParams ) const
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
	    CSdpFmtAttributeField::NewL( SDPStringL(
	                            SdpCodecStringConstants::EAttributeRtpmap ),
	                            *payload,
	                            *rtpmap );

	CleanupStack::PopAndDestroy( rtpmap );
	CleanupStack::PopAndDestroy( payload );
	CleanupStack::PopAndDestroy( clockrate );

    CleanupStack::PushL( rtpmapAttribute );

    return rtpmapAttribute;
    }


// --------------------------------------------------------------------------
// CMusAvaDefaultOptionHandler::ToTextL
//
// --------------------------------------------------------------------------
//
HBufC8* CMusAvaDefaultOptionHandler::ToTextL( CSdpDocument& aSdpContent )
    {
    HBufC8* content = NULL;

    CBufFlat* encBuf = CBufFlat::NewL( KMUSAVAMaxLengthOfSDPMessage );
    CleanupStack::PushL( encBuf );
    RBufWriteStream writeStream( *encBuf, 0 );
	writeStream.PushL();
	writeStream.Open( *encBuf, 0 );
    aSdpContent.EncodeL( writeStream );
    writeStream.Close();
    CleanupStack::PopAndDestroy();//writeStream

    content = encBuf->Ptr( 0 ).AllocL();
    CleanupStack::PopAndDestroy( encBuf );

    return content;
    }
