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
* Description:  Capability Test helper
*
*/


#include "musavacapabilitytesthelper.h"



CSIPServerTransaction* CapabilityTestHelper::OptionsRequestL( 
                                           const TDesC8& aRequestUri,
                                           const TDesC8& aFrom,
                                           const TDesC8& aAcceptContact, 
                                           const TDesC8& aContact,
                                           const TDesC8& aAccept,
                                           const TDesC8& aUserAgent )
    {
    
    CSIPServerTransaction* srvtransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions )); 
    CleanupStack::PushL( srvtransaction );
    
    TUriParser8 Uri;
    User::LeaveIfError( Uri.Parse( aRequestUri ) );
    
  	CUri8* uri = CUri8::NewLC( Uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );

//headers

    RPointerArray<CSIPHeaderBase> sipHdrs;
    CSIPHeaderBase::PushLC( &sipHdrs );

    if ( aAcceptContact.Length() > 0 )
        {
        
        RStringF value = MusAvaCapabilityContext::SIPStringL( aAcceptContact );
        CleanupClosePushL( value );
        
        CSIPAcceptContactHeader* acceptContact = 
            MusAvaCapabilityContext::AcceptContactHeaderLC( value );
        sipHdrs.AppendL( acceptContact );
        CleanupStack::Pop( acceptContact );
        
        CleanupStack::PopAndDestroy();//value
        
        }
        
    if ( aAccept.Length() > 0 )
        {
        CSIPAcceptHeader* accept = 
            CSIPAcceptHeader::NewLC( KMUSAVASIPMediaTypeApplication, 
                            KMUSAVASIPMediaSubTypeSDP );
        sipHdrs.AppendL( accept );
        CleanupStack::Pop( accept );
        }
        
    if ( aContact.Length() > 0 )
        {
        RStringF value = MusAvaCapabilityContext::SIPStringL( aContact );
        CleanupClosePushL( value );
        
        TUriParser8 uriValue;
        User::LeaveIfError( uriValue.Parse( aFrom ) );
        CUri8* uri = CUri8::NewLC( uriValue );
        
        CSIPContactHeader* contact = 
            MusAvaCapabilityContext::ContactHeaderLC( *uri, value );
        sipHdrs.AppendL( contact );
        CleanupStack::Pop( contact );
        CleanupStack::PopAndDestroy( uri );

        CleanupStack::PopAndDestroy();//value
        
        }
        
    if ( aUserAgent.Length() > 0 )
        {
        
        CSIPExtensionHeader* userAgent = 
            MusAvaCapabilityContext::UserAgentHeaderLC( aUserAgent );
        sipHdrs.AppendL( userAgent );
        CleanupStack::Pop( userAgent );
        }
        
    requestelements->MessageElements().SetUserHeadersL( sipHdrs );
    CleanupStack::PopAndDestroy( &sipHdrs );
    
//to, from

    CSIPToHeader* toheader = CSIPToHeader::DecodeL( aRequestUri );
    CleanupStack::PushL( toheader );
    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL( aFrom ); 
    CleanupStack::PushL( fromheader );
    
    requestelements->SetFromHeaderL(fromheader); 
    CleanupStack::Pop( fromheader );
    requestelements->SetToHeaderL(toheader); 
    CleanupStack::Pop( toheader );

    
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    
    srvtransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );
    CleanupStack::Pop( srvtransaction );

    return srvtransaction;
    
    }


CSIPServerTransaction* CapabilityTestHelper::OptionsRequestL( 
                                           const TDesC8& aRequestUri,
                                           const TDesC8& aFrom,
                                           const TDesC8& aAcceptContact, 
                                           const TDesC8& aContact,
                                           const TDesC8& aAccept,
                                           const TDesC8& aUserAgent,
                                           const TDesC8& aAssertId )
    {
    
    CSIPServerTransaction* srvtransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions )); 
    CleanupStack::PushL( srvtransaction );
    
    TUriParser8 Uri;
    User::LeaveIfError( Uri.Parse( aRequestUri ) );
    
  	CUri8* uri = CUri8::NewLC( Uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );

//headers

    RPointerArray<CSIPHeaderBase> sipHdrs;
    CSIPHeaderBase::PushLC( &sipHdrs );

    if ( aAcceptContact.Length() > 0 )
        {
        
        RStringF value = MusAvaCapabilityContext::SIPStringL( aAcceptContact );
        CleanupClosePushL( value );
        
        CSIPAcceptContactHeader* acceptContact = 
            MusAvaCapabilityContext::AcceptContactHeaderLC( value );
        sipHdrs.AppendL( acceptContact );
        CleanupStack::Pop( acceptContact );
        
        CleanupStack::PopAndDestroy();//value
        
        }
        
    if ( aAccept.Length() > 0 )
        {
        CSIPAcceptHeader* accept = 
            CSIPAcceptHeader::NewLC( KMUSAVASIPMediaTypeApplication, 
                            KMUSAVASIPMediaSubTypeSDP );
        sipHdrs.AppendL( accept );
        CleanupStack::Pop( accept );
        }
        
    if ( aContact.Length() > 0 )
        {
        RStringF value = MusAvaCapabilityContext::SIPStringL( aContact );
        CleanupClosePushL( value );
        
        TUriParser8 uriValue;
        User::LeaveIfError( uriValue.Parse( aFrom ) );
        CUri8* uri = CUri8::NewLC( uriValue );
        
        CSIPContactHeader* contact = 
            MusAvaCapabilityContext::ContactHeaderLC( *uri, value );
        sipHdrs.AppendL( contact );
        CleanupStack::Pop( contact );
        CleanupStack::PopAndDestroy( uri );

        CleanupStack::PopAndDestroy();//value
        
        }
        
    if ( aUserAgent.Length() > 0 )
        {
        
        CSIPExtensionHeader* userAgent = 
            MusAvaCapabilityContext::UserAgentHeaderLC( aUserAgent );
        sipHdrs.AppendL( userAgent );
        CleanupStack::Pop( userAgent );
        }

     if ( aAssertId.Length() > 0 )
        {
        
        CSIPExtensionHeader* assertId = 
            MusAvaCapabilityContext::PAssertedIdentityHeaderLC( aAssertId );
        sipHdrs.AppendL( assertId );
        CleanupStack::Pop( assertId );
        }
        
    requestelements->MessageElements().SetUserHeadersL( sipHdrs );
    CleanupStack::PopAndDestroy( &sipHdrs );
    
//to, from

    CSIPToHeader* toheader = CSIPToHeader::DecodeL( aRequestUri );
    CleanupStack::PushL( toheader );
    CSIPFromHeader* fromheader = CSIPFromHeader::DecodeL( aFrom ); 
    CleanupStack::PushL( fromheader );
    
    requestelements->SetFromHeaderL(fromheader); 
    CleanupStack::Pop( fromheader );
    requestelements->SetToHeaderL(toheader); 
    CleanupStack::Pop( toheader );

    
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    
    srvtransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );
    CleanupStack::Pop( srvtransaction );

    return srvtransaction;
    
    }
CSIPClientTransaction* CapabilityTestHelper::OptionsResponseL( TUint aResponse,
                                            TInt aPhrase,
                                            const TDesC8& aFrom,
                                            const TDesC8& aContact,
                                            const TDesC8& aUserAgent,
                                            const TDesC8& aPAssertedId,
                                            const TDesC8& aSdp )
    {
        
    CSIPClientTransaction* clitransaction = 
      CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ));
    CleanupStack::PushL( clitransaction );
    
    SetResponseL( *clitransaction, aResponse, aPhrase, aFrom, aContact, 
                aUserAgent, aPAssertedId, aSdp );

    CleanupStack::Pop( clitransaction );
    
    return clitransaction;
    
    
    }


void CapabilityTestHelper::SetResponseL( CSIPClientTransaction& aTrx,
                                   TUint aResponse, 
                                   TInt aPhrase,
                                   const TDesC8& aFrom,
                                   const TDesC8& aContact,
                                   const TDesC8& aUserAgent,
                                   const TDesC8& aPAssertedId,
                                   const TDesC8& aSdp,
                                   const TDesC8& aContentType )
    {

    CSIPResponseElements* response = 
        CSIPResponseElements::NewLC( aResponse, 
                                     SIPStrings::StringF( aPhrase ) );

    CSIPMessageElements& message = response->MessageElements();
    
//headers

    RPointerArray<CSIPHeaderBase> sipHdrs;
    CSIPHeaderBase::PushLC( &sipHdrs );

    if ( aContact.Length() > 0 )
        {

        RStringF value = MusAvaCapabilityContext::SIPStringL( aContact );
        CleanupClosePushL( value );
        
        TUriParser8 uriValue;
        User::LeaveIfError( uriValue.Parse( aFrom ) );
        CUri8* uri = CUri8::NewLC( uriValue );
        
        CSIPContactHeader* contact = 
            MusAvaCapabilityContext::ContactHeaderLC( *uri, value );
        sipHdrs.AppendL( contact );
        CleanupStack::Pop( contact );
        CleanupStack::PopAndDestroy( uri );
        CleanupStack::PopAndDestroy();//value
        
        }
        
    if ( aUserAgent.Length() > 0 )
        {
        
        CSIPExtensionHeader* userAgent = 
            MusAvaCapabilityContext::UserAgentHeaderLC( aUserAgent );
        sipHdrs.AppendL( userAgent );
        CleanupStack::Pop( userAgent );
        }
        
    if ( aPAssertedId.Length() > 0 )
        {
        
        CSIPExtensionHeader* pAssertedId = 
            MusAvaCapabilityContext::PAssertedIdentityHeaderLC( aPAssertedId );
        sipHdrs.AppendL( pAssertedId );
        CleanupStack::Pop( pAssertedId );
        
        }
        
    message.SetUserHeadersL( sipHdrs );
    CleanupStack::PopAndDestroy( &sipHdrs );

    if ( aSdp.Length() > 0 )
        {
        CSIPContentTypeHeader* contenttype = NULL;
        if ( aContentType.Length() == 0 )
            {
            contenttype = 
            CSIPContentTypeHeader::NewLC( KMUSAVASIPMediaTypeApplication,
                                         KMUSAVASIPMediaSubTypeSDP );
            }
        else
            {
            contenttype = 
            CSIPContentTypeHeader::DecodeL( aContentType );
            CleanupStack::PushL( contenttype );

            }
        HBufC8* content = aSdp.AllocLC();
        
        message.SetContentL( content, contenttype );
        CleanupStack::Pop( content );
        CleanupStack::Pop( contenttype );
        
        }
        
    aTrx.SetResponseElements( response );
    CleanupStack::Pop( response );
    
    }

void CapabilityTestHelper::SetResponse2L(CSIPClientTransaction& aTrx,
                                   TUint aResponse, 
                                   TInt aPhrase,
                                   const TDesC8& aFrom,
                                   const TDesC8& aContact,
                                   const TDesC8& aUserAgent,
                                   const TDesC8& aPAssertedId,
                                   const TDesC8& aPAssertedId2,
                                   const TDesC8& aSdp,
                                   const TDesC8& aContentType )  
    {
    CSIPResponseElements* response = 
        CSIPResponseElements::NewLC( aResponse, 
                                     SIPStrings::StringF( aPhrase ) );

    CSIPMessageElements& message = response->MessageElements();
    
//headers

    RPointerArray<CSIPHeaderBase> sipHdrs;
    CSIPHeaderBase::PushLC( &sipHdrs );

    if ( aContact.Length() > 0 )
        {

        RStringF value = MusAvaCapabilityContext::SIPStringL( aContact );
        CleanupClosePushL( value );
        
        TUriParser8 uriValue;
        User::LeaveIfError( uriValue.Parse( aFrom ) );
        CUri8* uri = CUri8::NewLC( uriValue );
        
        CSIPContactHeader* contact = 
            MusAvaCapabilityContext::ContactHeaderLC( *uri, value );
        sipHdrs.AppendL( contact );
        CleanupStack::Pop( contact );
        CleanupStack::PopAndDestroy( uri );
        CleanupStack::PopAndDestroy();//value
        
        }
        
    if ( aUserAgent.Length() > 0 )
        {
        
        CSIPExtensionHeader* userAgent = 
            MusAvaCapabilityContext::UserAgentHeaderLC( aUserAgent );
        sipHdrs.AppendL( userAgent );
        CleanupStack::Pop( userAgent );
        }
        
    if ( aPAssertedId.Length() > 0 )
        {
        
        CSIPExtensionHeader* pAssertedId = 
            MusAvaCapabilityContext::PAssertedIdentityHeaderLC( aPAssertedId );
        sipHdrs.AppendL( pAssertedId );
        CleanupStack::Pop( pAssertedId );
        
        }
        
    if ( aPAssertedId2.Length() > 0 )
        {
        
        CSIPExtensionHeader* pAssertedId2 = 
            MusAvaCapabilityContext::PAssertedIdentityHeaderLC( aPAssertedId2 );
        sipHdrs.AppendL( pAssertedId2 );
        CleanupStack::Pop( pAssertedId2 );
        
        }

    message.SetUserHeadersL( sipHdrs );
    CleanupStack::PopAndDestroy( &sipHdrs );

    if ( aSdp.Length() > 0 )
        {
        CSIPContentTypeHeader* contenttype = NULL;
        if ( aContentType.Length() == 0 )
            {
            contenttype = 
            CSIPContentTypeHeader::NewLC( KMUSAVASIPMediaTypeApplication,
                                         KMUSAVASIPMediaSubTypeSDP );
            }
        else
            {
            contenttype = 
            CSIPContentTypeHeader::DecodeL( aContentType );
            CleanupStack::PushL( contenttype );

            }
        HBufC8* content = aSdp.AllocLC();
        
        message.SetContentL( content, contenttype );
        CleanupStack::Pop( content );
        CleanupStack::Pop( contenttype );
        
        }
        
    aTrx.SetResponseElements( response );
    CleanupStack::Pop( response );
    
    }

CSIPAddress* HelpFlags::iTestSipAddress = NULL;
TBool HelpFlags::iUseTestSipAddress = EFalse;
TBool HelpFlags::iEmptyContactHeader = EFalse;

