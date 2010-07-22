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
* Description:   Utility class to add sip headers. This class will just have 
*                static utility functions.
*
*/


#include "musavasipheaderutil.h"
#include "muscleanupresetanddestroy.h"
#include <escapeutils.h>
#include <sipconnection.h>
#include <sipprofile.h>
#include <sipclienttransaction.h>
#include <sipmessageelements.h>
#include <siprequestelements.h>
#include <sipextensionheader.h>
#include <sipacceptcontactheader.h>
#include <sipcontactheader.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sipresponseelements.h>
#include <sipacceptencodingheader.h>
#include <sipacceptlanguageheader.h>
#include <sipcontenttypeheader.h>
#include <sipunsupportedheader.h>
#include <sdpdocument.h>
#include "sipextensionheader.h"
#include <sdporiginfield.h>
#include <sdpconnectionfield.h>
#include <sdpattributefield.h>
#include <sdpmediafield.h>
#include <sipacceptheader.h>
#include <sipaddress.h>
#include <sdpcodecstringpool.h>

#include "muslogger.h"

// --------------------------------------------------------------------------
// CMusAvaSipheaderUtil::AddAcceptContactHeaderL
// --------------------------------------------------------------------------
//
void CMusAvaSipheaderUtil::AddAcceptContactHeaderL( 
                            RPointerArray<CSIPHeaderBase>& aRequestHeaders,
                            const RStringF& aFeatureTag )
    {
    MUS_LOG("mus: [MUSAVA] -> CMusAvaSipheaderUtil::AddAcceptContactHeaderL" )    
    CSIPAcceptContactHeader* header = CSIPAcceptContactHeader::NewLC();    
    header->SetParamL( aFeatureTag );
    header->SetParamL( SIPStrings::StringF( SipStrConsts::EExplicit ) );
	aRequestHeaders.AppendL( header ); // ownership transferred.
    CleanupStack::Pop( header );    
    MUS_LOG("mus: [MUSAVA] <- CMusAvaSipheaderUtil::AddAcceptContactHeaderL" )
    }


// --------------------------------------------------------------------------
// CMusAvaSipheaderUtil::AddAcceptEncodingHeaderL
// --------------------------------------------------------------------------
//
void CMusAvaSipheaderUtil::AddAcceptEncodingHeaderL( 
                            RPointerArray<CSIPHeaderBase>& aRequestHeaders )
    {
    MUS_LOG("mus: [MUSAVA] -> CMusAvaSipheaderUtil::AddAcceptEncodingHeaderL" )    
    _LIT8(KHeader,"gzip");	
    RPointerArray<CSIPAcceptEncodingHeader> 
                   headers(CSIPAcceptEncodingHeader::DecodeL(KHeader));
    MusCleanupResetAndDestroyPushL(headers);
    
    TInt count = headers.Count();
    CSIPHeaderBase* header = NULL;;
    TInt status = NULL;
    
    for (TInt i=0; i < count; i++)
		{
		header = headers[i];
		status = aRequestHeaders.Append(header);
		if ( status != KErrNone )
		    {
		    header = NULL;
		    }
		else
		    {
		    headers.Remove( i );
		    }
		}
    CleanupStack::PopAndDestroy( &headers ); 
    MUS_LOG("mus: [MUSAVA] <- CMusAvaSipheaderUtil::AddAcceptEncodingHeaderL" )
    }


// --------------------------------------------------------------------------
// CMusAvaSipheaderUtil::AddAcceptLanguageHeaderL
// --------------------------------------------------------------------------
//
void CMusAvaSipheaderUtil::AddAcceptLanguageHeaderL( 
                            RPointerArray<CSIPHeaderBase>& aRequestHeaders )
    {
    MUS_LOG("mus: [MUSAVA] -> CMusAvaSipheaderUtil::AddAcceptLanguageHeaderL" )    
    _LIT8(KHeader,"en");
    RPointerArray<CSIPAcceptLanguageHeader> 
                headers(CSIPAcceptLanguageHeader::DecodeL(KHeader));
    MusCleanupResetAndDestroyPushL(headers);
    
    TInt count = headers.Count();
    CSIPHeaderBase* header = NULL;;
    TInt status = NULL;
    
    for (TInt i=0; i < count; i++)
		{
		header = headers[i];
		status = aRequestHeaders.Append(header);
		if ( status != KErrNone )
		    {
		    header = NULL;
		    }
		else
		    {
		    headers.Remove( i );
		    }
		}
    CleanupStack::PopAndDestroy( &headers ); 
    MUS_LOG("mus: [MUSAVA] <- CMusAvaSipheaderUtil::AddAcceptLanguageHeaderL" )
    }

// --------------------------------------------------------------------------
// CMusAvaSipheaderUtil::AddSdpL
// --------------------------------------------------------------------------
//
void CMusAvaSipheaderUtil::AddSdpL( CSIPRequestElements* aRequest,
                                    HBufC8* aContent )
    {
    MUS_LOG("mus: [MUSAVA] -> CMusAvaSipheaderUtil::AddSdpL" )        
    _LIT8(contentType,"application/sdp");
    CSIPContentTypeHeader* type = CSIPContentTypeHeader::DecodeL(
															contentType);
    CleanupStack::PushL( type );																		
    CSIPMessageElements& messageElements = aRequest->MessageElements();	             		    							    
	messageElements.SetContentL(aContent,type);	
	CleanupStack::Pop( type );//Ownership transferred		  	
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaSipheaderUtil::AddSdpL" )
    }

// --------------------------------------------------------------------------
// CMusAvaSipheaderUtil::AddPreferredIdentityHeaderL
// --------------------------------------------------------------------------
//
void CMusAvaSipheaderUtil::AddPreferredIdentityHeaderL( 
                            RPointerArray<CSIPHeaderBase>& aRequestHeaders,
                            const TDesC8& aVal )
    {    
    MUS_LOG_TDESC8( " mus: [MUSAVA] -> CMusAvaSipheaderUtil::\
    AddPreferredIdentityHeaderL", aVal);
    _LIT8(KHeader,"P-Preferred-Identity");  
	CSIPExtensionHeader* header = CSIPExtensionHeader::NewLC(
										                       KHeader,aVal) ;    
    aRequestHeaders.AppendL( header );
    CleanupStack::Pop( header );//Ownership transferred.
    MUS_LOG("mus: [MUSAVA] <- CMusAvaSipheaderUtil::AddPreferredIdentityHeaderL")
    }

// ----------------------------------------------------------------------------
// MusAvaCapabilityContext::LocalHostL
// ----------------------------------------------------------------------------
//
void CMusAvaSipheaderUtil::LocalHostL(const CSIPConnection& aConn, 
                                      TPtrC8& aLocalHost)
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaSipheaderUtil::LocalHostL" )
    RStringPool strPool = SdpCodecStringPool::StringPoolL();
    TInetAddr aAddr(0);
    aConn.GetLocalAddrL(aAddr);   
	TBuf<50> ipAddress;	
	aAddr.Output(ipAddress);
	MUS_LOG_TDESC( "mus: [MUSAVA] ipAddress",ipAddress )	
	HBufC8 * ipaddr8=HBufC8::NewLC(ipAddress.Length());	
	ipaddr8->Des().Copy(ipAddress);
	aLocalHost.Set( *ipaddr8 );
	CleanupStack::PopAndDestroy(1);//ipaddr8
	MUS_LOG( "mus: [MUSAVA] <- CMusAvaSipheaderUtil::LocalHostL" )
    }


// --------------------------------------------------------------------------
// CMusAvaSipheaderUtil::AddAcceptSdpHeaderL
// --------------------------------------------------------------------------
//
void CMusAvaSipheaderUtil::AddAcceptSdpHeaderL( 
                            RPointerArray<CSIPHeaderBase>& aRequestHeaders )
    {
    MUS_LOG("mus: [MUSAVA] -> CMusAvaSipheaderUtil::AddAcceptSDPHeaderL" )    
    _LIT8(KHeader,"application/sdp");
    RPointerArray<CSIPAcceptHeader> headers(CSIPAcceptHeader::DecodeL(KHeader));
    MusCleanupResetAndDestroyPushL(headers);
    
    TInt count = headers.Count();
    CSIPHeaderBase* header = NULL;;
    TInt status = NULL;
    
    for (TInt i=0; i < count; i++)
		{
		header = headers[i];
		status = aRequestHeaders.Append(header);
		if ( status != KErrNone )
		    {
		    header = NULL;
		    }
		else
		    {
		    headers.Remove( i );
		    }
		}
    CleanupStack::PopAndDestroy( &headers ); 
    MUS_LOG( 
    "mus: [MUSAVA] <- CMusAvaSipheaderUtil::AddAcceptSDPHeaderL" )
    }




