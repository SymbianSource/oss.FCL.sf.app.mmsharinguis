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
* Description:   helper SIP related methods
*
*/


#include <e32math.h>
#include <uri8.h>
#include <uriutils.h>
#include <sip.h>
#include <sipservertransaction.h>
#include <sipclienttransaction.h>
#include <sipacceptcontactheader.h>
#include <sipcontactheader.h>
#include <sipextensionheader.h>
#include <siprequestelements.h>
#include <sipresponseelements.h>
#include <sipmessageelements.h>
#include <sipfromheader.h>
#include <sipaddress.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sdpdocument.h>
#include <sdpcodecstringconstants.h>
#include <sdpcodecstringpool.h>
#include <escapeutils.h>

#include "muslogger.h"
#include "musavacapabilitycontext.h"

_LIT8( KMusAssertedIndentity,"P-Asserted-Identity" );

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
RStringF MusAvaCapabilityContext::SDPStringL( TInt aStringIndex )
    {
    return SdpCodecStringPool::StringPoolL().
        StringF( aStringIndex, SdpCodecStringPool::StringTableL() );
    }


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
//
RStringF MusAvaCapabilityContext::SDPStringL( const TDesC8& aString )
    {
    return SdpCodecStringPool::StringPoolL().OpenFStringL( aString );
    }


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
//
RStringF MusAvaCapabilityContext::SIPStringL( const TDesC8& aString )
    {
    return SIPStrings::Pool().OpenFStringL( aString );
    }


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
//
void MusAvaCapabilityContext::LocalHostL( 
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


// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
//
CUri8* MusAvaCapabilityContext::ToUriL( const TDesC8& aUri )
    {
    TUriParser8 uri;
    User::LeaveIfError( uri.Parse( aUri ) );
    
    return CUri8::NewL( uri );
    }

    
// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
//
HBufC8* MusAvaCapabilityContext::ToTextL( CSdpDocument& aSdpContent )
    {
    HBufC8* content = NULL;
    
    CBufFlat* encBuf = CBufFlat::NewL( KMUSAVAMaxLengthOfSDPMessage );
    CleanupStack::PushL( encBuf );
    RBufWriteStream writeStream( *encBuf, 0 );
	writeStream.PushL();
    aSdpContent.EncodeL( writeStream );
    CleanupStack::PopAndDestroy();//writeStream
    
    content = encBuf->Ptr( 0 ).AllocL();
    CleanupStack::PopAndDestroy( encBuf );

    return content;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
const CSIPMessageElements& MusAvaCapabilityContext::MessageElementsL( 
                                            const CSIPTransactionBase& aTrx )
    {
    const CSIPMessageElements* message = NULL;
    
    if ( aTrx.IsSIPClientTransaction() )
        {
        
        const CSIPResponseElements* response = 
            static_cast<const CSIPClientTransaction&>
                ( aTrx ).ResponseElements();
        User::LeaveIfError( !response ? KErrGeneral : KErrNone );
        
        message = &response->MessageElements();
        }
    else
        {
        const CSIPRequestElements* request = 
            static_cast<const CSIPServerTransaction&>( aTrx ).RequestElements();
        User::LeaveIfError( !request ? KErrGeneral : KErrNone );
        
        message = &request->MessageElements();
        }
        
    return *message;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
CSIPAcceptContactHeader* MusAvaCapabilityContext::AcceptContactHeaderLC( 
                                                            RStringF aValue )
    {
    CSIPAcceptContactHeader* header = CSIPAcceptContactHeader::NewLC();
    
    header->SetParamL( aValue );
    header->SetParamL( SIPStrings::StringF( SipStrConsts::EExplicit ) );
    
    return header;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
const CSIPAcceptContactHeader* MusAvaCapabilityContext::AcceptContactHeaderL( 
                                            const CSIPTransactionBase& aTrx )
    {
    return static_cast<const CSIPAcceptContactHeader*>
        ( Header( SIPStrings::StringF( SipStrConsts::EAcceptContactHeader ),
          MessageElementsL( aTrx ) ) );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
CSIPContactHeader* MusAvaCapabilityContext::ContactHeaderLC( 
                                                    const CUri8& aAddress, 
                                                    RStringF aValue )
    {
    CUri8* uri = CUri8::NewLC( aAddress.Uri() );
    
    /*
    if ( !uri->Uri().IsPresent( EUriHost ) )
        {
        uri->SetComponentL( 
        SIPStrings::StringF( SipStrConsts::ELocalHost ).DesC(), EUriHost );
        }
    */
                       
    CSIPAddress* address = CSIPAddress::NewL( uri );
    CleanupStack::Pop( uri );
    CleanupStack::PushL( address );
    
    CSIPContactHeader* header = CSIPContactHeader::NewL( address );
    CleanupStack::Pop( address );
    CleanupStack::PushL( header );

    if ( aValue != SIPStrings::StringF( SipStrConsts::EEmpty ) )
        {
        TInt separator = aValue.DesC().Find( KMUSAVASemicolon );
        if ( separator < KErrNone )
            {
            header->SetParamL( aValue );
            }
        else
            {
            TPtrC8 head = aValue.DesC().Left( separator );
            TPtrC8 tail = aValue.DesC().Mid( 
                        separator + KMUSAVASemicolon().Length() );
        
            RStringF theHead = SIPStringL( head );
            CleanupClosePushL( theHead );
            RStringF theTail = SIPStringL( tail );
            CleanupClosePushL( theTail );
        
            header->SetParamL( theHead );
            header->SetParamL( theTail );

            CleanupStack::PopAndDestroy();//theTail
            CleanupStack::PopAndDestroy();//theHead
            }
        }
        
    return header;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
const CSIPContactHeader* MusAvaCapabilityContext::ContactHeaderL( 
                                            const CSIPTransactionBase& aTrx )
    {
    return static_cast<const CSIPContactHeader*>
        ( Header( SIPStrings::StringF( SipStrConsts::EContactHeader ), 
                  MessageElementsL( aTrx ) ) );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
CSIPExtensionHeader* MusAvaCapabilityContext::UserAgentHeaderLC( 
                                                    const TDesC8& aValue )
    {
    return CSIPExtensionHeader::NewLC( KMUSAVASIPHeaderUserAgent, aValue );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
const CSIPExtensionHeader* MusAvaCapabilityContext::UserAgentHeaderL( 
                                            const CSIPTransactionBase& aTrx )
    {
    RStringF userAgentHeaderName = SIPStringL( KMUSAVASIPHeaderUserAgent );
    CleanupClosePushL( userAgentHeaderName );
    
    const CSIPExtensionHeader* header = 
        static_cast<const CSIPExtensionHeader*>
        ( Header( userAgentHeaderName, MessageElementsL( aTrx ) ) );
        
    CleanupStack::PopAndDestroy();//userAgentHeaderName

    return header;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
CSIPExtensionHeader* MusAvaCapabilityContext::PAssertedIdentityHeaderLC( 
                                                    const TDesC8& aValue )
    {
    return CSIPExtensionHeader::NewLC( KMUSAVASIPHeaderPAssertedIdentity, 
                                    aValue );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
const CUri8& MusAvaCapabilityContext::OriginatorL( 
                                        const CSIPServerTransaction& aTrx )
    {
    const CSIPRequestElements* request = aTrx.RequestElements();
    User::LeaveIfError( !request ? KErrGeneral : KErrNone );
    
    const CSIPFromHeader* from = request->FromHeader();
    
    User::LeaveIfError( from ? KErrNone : KErrGeneral );

    return from->SIPAddress().Uri8();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
const CSIPHeaderBase* MusAvaCapabilityContext::Header( 
                                    RStringF aHeaderName,
                                    const CSIPMessageElements& aMessage )
    {
    TBool found = EFalse;
    CSIPHeaderBase* header = NULL;
    const RPointerArray<CSIPHeaderBase>& headers = aMessage.UserHeaders();
    
    for ( TInt i=0; i < headers.Count() && !found; i++ )
        {
        header = headers[i];
        if ( header->Name() == aHeaderName )
            {
            found = ETrue;
            }
        else
            {
            header = NULL;
            }
        }
    return header;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
const CSIPHeaderBase* MusAvaCapabilityContext::Header( 
                                    RStringF aHeaderName,
                                    const CSIPMessageElements& aMessage,
                                    TUint aIndex )
    {
    TInt found = -1;
    CSIPHeaderBase* header = NULL;
    const RPointerArray<CSIPHeaderBase>& headers = aMessage.UserHeaders();
    
    for ( TInt i=0; i < headers.Count() && found != aIndex; i++ )
        {
        header = headers[i];
        if ( header->Name() == aHeaderName )
            {
            found++;
            }
        else
            {
            header = NULL;
            }
        }
    return header;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
TUint MusAvaCapabilityContext::HeaderCount( 
                                    RStringF aHeaderName,
                                    const CSIPMessageElements& aMessage )
    {
    TUint found = 0;
    CSIPHeaderBase* header = NULL;
    const RPointerArray<CSIPHeaderBase>& headers = aMessage.UserHeaders();
    
    for ( TInt i=0; i < headers.Count(); i++ )
        {
        header = headers[i];
        if ( header->Name() == aHeaderName )
            {
            found++;
            }
        }
    return found;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
CDesCArrayFlat* MusAvaCapabilityContext::PAssertedIdentityHeaderL(
                                    const CSIPMessageElements& aMessage )
    { 
    MUS_LOG( "mus: [MUSAVA] -> MusAvaCapabilityContext::PAssertedIdentityHeaderL" )
    
    RStringF p = SIPStrings::Pool().OpenFStringL( KMusAssertedIndentity() );
    CleanupClosePushL( p );	
    TInt headerCount = HeaderCount( p,aMessage); 
    
    CDesCArrayFlat* headers = new( ELeave ) CDesCArrayFlat( 2 );
    CleanupStack::PushL( headers );

    for ( TInt i=0; i < headerCount; i++ )
        {        
        const CSIPHeaderBase* pAssertedId = 
        		MusAvaCapabilityContext::Header( p, aMessage, i );      
                        
        if ( pAssertedId )
            {
            HBufC8* value = pAssertedId->ToTextValueLC();
            CSIPAddress* address = CSIPAddress::DecodeL( *value );
            CleanupStack::PushL( address );                 
            const TDesC8& uriInPAssertedId = address->Uri8().Uri().UriDes();
        	if ( uriInPAssertedId.Length() > 0 )
    	        {
    	        HBufC* uriBuf = 
    	            EscapeUtils::ConvertToUnicodeFromUtf8L( uriInPAssertedId );
    	        CleanupStack::PushL( uriBuf );  
    	        headers->AppendL( *uriBuf );
    	        CleanupStack::PopAndDestroy( uriBuf );
        		}
            CleanupStack::PopAndDestroy( address );
            CleanupStack::PopAndDestroy( value );
            }
        }	
    
    CleanupStack::Pop( headers );
    CleanupStack::PopAndDestroy( &p );

    MUS_LOG( "mus: [MUSAVA] <- MusAvaCapabilityContext::PAssertedIdentityHeaderL" )
    return headers;
    }

