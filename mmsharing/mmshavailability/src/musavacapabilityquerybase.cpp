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
* Description:   Sent capability query (OPTIONS)
*
*/


#include "musavacapabilityquerybase.h"
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
#include <sipacceptheader.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sipresponseelements.h>
#include <sipaddress.h>
#include <sipfromheader.h>
#include <sdpdocument.h>
#include <sdporiginfield.h>
#include <sdpconnectionfield.h>
#include <sdpattributefield.h>
#include <sdpmediafield.h>
#include <e32math.h>
#include <e32property.h>

#include "mussettings.h"
#include "muslogger.h"
#include "mmusavacapabilityqueryobserver.h"
#include "musavacapability.h"
#include "musavacapabilityexchange.h"
#include "musavaterminal.h"
#include "musavacapabilitycontext.h"
#include "musavasipheaderutil.h"
#include "mussesseioninformationapi.h"

// --------------------------------------------------------------------------
// C++ constructor
// --------------------------------------------------------------------------
//
CMusAvaCapabilityQueryBase::CMusAvaCapabilityQueryBase( 
                                        CMusAvaCapability& aCapability,
                                        CSIPConnection& aSIPConnection,
                                        CSIPProfile& aProfile  )
    : iCapability( aCapability ),
      iSIPConnection( aSIPConnection ),
      iProfile( aProfile ),
      iState( ECapabilityQueryCreated ),
      iResult( KCapabilityQueryNotReady )
    {
    }

// --------------------------------------------------------------------------
// C++ destructor
// --------------------------------------------------------------------------
//
CMusAvaCapabilityQueryBase::~CMusAvaCapabilityQueryBase()
    {
    MUS_LOG( 
    "mus: [MUSAVA] -> CMusAvaCapabilityQueryBase::~CMusAvaCapabilityQueryBase" )
    
    delete iRegisteredContact;
    delete iOriginator;
    if ( iTerminal )
        {
        iTerminal->DetachQuery( *this );
        }
    ResetAndDestroyTrx();
    MUS_LOG( "mus: [MUSAVA] -> StringPools are getting closed." )
    MUS_LOG( 
    "mus: [MUSAVA] <- CMusAvaCapabilityQueryBase::~CMusAvaCapabilityQueryBase" )
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::ConstructL
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQueryBase::ConstructL( const TDesC& aSipAddress )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityQueryBase::ConstructL" )

    // If string pool is opened already , then dont care.    

    User::LeaveIfError( aSipAddress.Length() > 0 ? KErrNone : KErrArgument );
    
    // If it is not registered , just leave.
    TBool isRegistered = ETrue;
    iProfile.GetParameter( KSIPProfileRegistered, isRegistered );
    User::LeaveIfError( isRegistered ? KErrNone : KErrArgument );
    
    TInt err( KErrNotFound );
    const TDesC8* registeredContact( NULL );
    err = iProfile.GetParameter( KSIPRegisteredContact, registeredContact );
    if( err )
        {
        MUS_LOG1( "mus: [MUSAVA]    iProfile.GetParameter Contact -> %d", err )
        User::LeaveIfError( err );
        }    

    // contactHeader array will only contain one instance of CSIPContactHeader,
    // since only one registered contact is decoded
    RPointerArray<CSIPContactHeader> contactHeader = 
        CSIPContactHeader::DecodeL( *registeredContact );
    MusCleanupResetAndDestroyPushL( contactHeader );

    // leave if there is at least one contact not present
    if ( contactHeader.Count() < 1 )
        {
        MUS_LOG1( "mus: [MUSAVA]    contactHeader has %d instances, instead of 1", 
                contactHeader.Count() );
        User::Leave( KErrNotFound );
        }
    
    // if more contact than pick the first one. Sip assures in idle case
    // it should always return only one registered contact.
    CSIPAddress* sipAddress = contactHeader[0]->SIPAddress();
    if ( !sipAddress )
        {
        MUS_LOG( "mus: [MUSAVA]    name-address not present")
        User::Leave( KErrNotFound );
        }
    iRegisteredContact = CUri8::NewL( sipAddress->Uri8().Uri() );
    MUS_LOG_TDESC8( " mus: [MUSAVA] iRegisteredContact : ", (iRegisteredContact->Uri().UriDes()));        
    CleanupStack::PopAndDestroy( &contactHeader );    
    
    sipAddress = NULL;
    const MDesC8Array* aors = NULL;
    User::LeaveIfError(iProfile.GetParameter( KSIPRegisteredAors, aors ));
    MUS_LOG( "mus: [MUSAVA]    iProfile.GetParameter -> OK  " )
    if( !aors || aors->MdcaCount() <= 0 ) User::Leave(KErrArgument);            
    MUS_LOG_TDESC8( " mus: [MUSAVA] AOR -> 0 : ", aors->MdcaPoint(0));     
    sipAddress =  CSIPAddress::DecodeL( aors->MdcaPoint(0) );
    MUS_LOG( "mus: [MUSAVA]    CSIPAddress::DecodeL -> OK   " )
    CleanupStack::PushL( sipAddress );        
    iOriginator = CUri8::NewL(sipAddress->Uri8().Uri());
    MUS_LOG_TDESC8( " mus: [MUSAVA] iOriginator : ", (iOriginator->Uri().UriDes()));    
    CleanupStack::PopAndDestroy(sipAddress);        
    
    HBufC8* sipAddress8 = EscapeUtils::ConvertFromUnicodeToUtf8L( aSipAddress );
    CleanupStack::PushL( sipAddress8 );
    iTerminal = &Capability().Exchange().TerminalL( sipAddress8->Des() );
    CleanupStack::PopAndDestroy( sipAddress8 ) ;
    iTerminal->AttachQuery( *this );
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityQueryBase::ConstructL" )
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::Capability
// --------------------------------------------------------------------------
//
const CMusAvaCapability& CMusAvaCapabilityQueryBase::Capability() const
    {
    return iCapability;
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::SetState
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQueryBase::SetState( 
                                CMusAvaCapabilityQueryBase::TState aState )
    {
    iState = aState;
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::SetResult
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQueryBase::SetResult( TInt aResult )
    {
    iResult = aResult;
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::Terminal
// --------------------------------------------------------------------------
//
CMusAvaTerminal& CMusAvaCapabilityQueryBase::Terminal()
    {
    return *iTerminal;
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::ExecuteL
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQueryBase::ExecuteL()
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityQueryBase::ExecuteL" )
    
    __ASSERT_ALWAYS( 
        State() != ECapabilityQueryPrepared, User::Leave( KErrGeneral ) );
    
    if ( State() == ECapabilityQueryCreated )
        {
        RPointerArray<CSIPHeaderBase> headers;
        CSIPHeaderBase::PushLC( &headers );
        
        Prepare( headers );
        
        if ( State() == ECapabilityQueryPrepared )
            {
            CMusAvaSipheaderUtil::AddAcceptContactHeaderL( headers,
                                                  Capability().Feature() );
            MUS_LOG( "mus: [MUSAVA]  AddAcceptContactHeaderL -> Success ")            
            CMusAvaSipheaderUtil::AddAcceptEncodingHeaderL(headers);
            MUS_LOG( "mus: [MUSAVA]  AddAcceptEncodingHeaderL -> Success ")                        
            CMusAvaSipheaderUtil::AddAcceptLanguageHeaderL(headers);  
            MUS_LOG( "mus: [MUSAVA]  AddAcceptLanguageHeaderL -> Success ")                        
            CMusAvaSipheaderUtil::AddAcceptSdpHeaderL(headers); 
            MUS_LOG( "mus: [MUSAVA]  AddAcceptSDPHeaderL -> Success ")                            
            CMusAvaSipheaderUtil::AddPreferredIdentityHeaderL(headers,
                                               iOriginator->Uri().UriDes());
            MUS_LOG( "mus: [MUSAVA]  AddPreferredIdentityHeaderL -> Success ")                         
            AddContactHeaderL( headers );
            MUS_LOG( "mus: [MUSAVA]  AddContactHeaderL -> Success ")
            
            TBool usePrivacy = MultimediaSharingSettings::PrivacySetting();
            if ( usePrivacy )
                {
                NMusSessionInformationApi::TMusClirSetting clir =
                    NMusSessionInformationApi::ESendOwnNumber;
                RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                                NMusSessionInformationApi::KMusClirSetting,
                                reinterpret_cast<TInt&>( clir ) );
                usePrivacy = ( clir == NMusSessionInformationApi::EDoNotSendOwnNumber );
                }
                    
            if (usePrivacy)
                {
                CMusAvaSipheaderUtil::AddPrivacyHeaderL( headers );
                MUS_LOG( "mus: [MUSAVA]  AddPrivacyHeaderL -> Success ")
                }
            //terminal ID
            if ( Capability().Exchange().TerminalId().Length() > 0 )
                {
                MUS_LOG( "mus: [MUSAVA] Adding User-Agent header" )
            	CSIPHeaderBase* userAgent = 
               	    MusAvaCapabilityContext::UserAgentHeaderLC( 
               	        Capability().Exchange().TerminalId() );
                headers.AppendL( userAgent );
                CleanupStack::Pop( userAgent );
                }
                
            //remote uri ( will be used as ToHeader )
            CUri8* remoteUri = 
                    MusAvaCapabilityContext::ToUriL( Terminal().Uri() );
            CleanupStack::PushL( remoteUri );
            CSIPRequestElements* request = 
                                CSIPRequestElements::NewL( remoteUri );
            CleanupStack::Pop( remoteUri );
            CleanupStack::PushL( request );
            
            if (usePrivacy)
                {
                _LIT8( KAnonymous,
                       "\"Anonymous\" <sip:anonymous@anonymous.invalid>" );
                CSIPFromHeader* from = CSIPFromHeader::DecodeL( KAnonymous );
                CleanupStack::PushL( from );
                request->SetFromHeaderL( from );
                CleanupStack::Pop( from );
                }
            
            request->SetMethodL( 
                        SIPStrings::StringF( SipStrConsts::EOptions ) );
            
            CSIPMessageElements& message = request->MessageElements();
            message.SetUserHeadersL( headers );
                        
            HBufC8* content = ContentLC();
            CMusAvaSipheaderUtil::AddSdpL(request,content);
            CleanupStack::Pop(content) ;

            ResetAndDestroyTrx();            
            
            MUS_LOG( "mus: [MUSAVA] Sending OPTIONS request ...." )            
            iTrx = iSIPConnection.SendRequestL( request, iProfile );
            CleanupStack::Pop( request );
                      
            iState = ECapabilityQueryExecuting;            
            MUS_LOG( "mus: [MUSAVA] OPTIONS request sent" )
            }
            
        CleanupStack::PopAndDestroy( &headers );
        }
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityQueryBase::ExecuteL" )
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::CompletedL
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQueryBase::CompletedL( 
                                    const CSIPClientTransaction& aResponse )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityQueryBase::CompletedL" )
    
    if ( iTrx && *iTrx == aResponse )
        {
        TUint statusCode = aResponse.ResponseElements()->StatusCode();
        if ( 100 <= statusCode && statusCode < 200 )
            {
            return;
            }
        DoCompletedL( aResponse );
        if ( State() == ECapabilityQueryCompleted )
            {
            MUS_LOG( "mus: [MUSAVA] query completed" )
            Capability().Exchange().QueryObserver().CapabilitiesResolved( 
                                                                    *this );
            }
        }
        
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityQueryBase::CompletedL" )
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::Canceled
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQueryBase::Canceled( 
                                const CSIPClientTransaction& aTransaction )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityQueryBase::Canceled" )
    if ( iTrx && *iTrx == aTransaction )
        {
        MUS_LOG( "mus: [MUSAVA] query canceled" )
        
        SetState( ECapabilityQueryCompleted );
        SetResult( KCapabilityCapabilitesNotFound );
        Capability().Exchange().QueryObserver().CapabilitiesResolved( 
                                                                    *this );
        }
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityQueryBase::Canceled" )
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::AddContactHeaderL
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQueryBase::AddContactHeaderL( 
                            RPointerArray<CSIPHeaderBase>& aRequestHeaders )
    {
    
    MUS_LOG( 
        "mus: [MUSAVA] -> CMusAvaCapabilityQueryBase::AddContactHeaderL" )
	CSIPContactHeader* contact = 
	    MusAvaCapabilityContext::ContactHeaderLC( *iRegisteredContact,
	                                              Capability().Feature() );    
    aRequestHeaders.AppendL( contact );
    CleanupStack::Pop( contact );    
    MUS_LOG( 
        "mus: [MUSAVA] <- CMusAvaCapabilityQueryBase::AddContactHeaderL" )
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::State
// --------------------------------------------------------------------------
//
CMusAvaCapabilityQueryBase::TState CMusAvaCapabilityQueryBase::State() const
    {
    return iState;
    }
    
// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::Result
// --------------------------------------------------------------------------
//
TInt CMusAvaCapabilityQueryBase::Result() const
    {
    return iResult;
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::ResetAndDestroyTrx
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQueryBase::ResetAndDestroyTrx()
    {
    delete iTrx;
    iTrx = NULL;
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::LocalAddress
// --------------------------------------------------------------------------
//
const CUri8& CMusAvaCapabilityQueryBase::LocalAddress() const
    {
    return *iOriginator;    
    }
    
// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::ResponseContentLC
// --------------------------------------------------------------------------
//
HBufC8* CMusAvaCapabilityQueryBase::ContentLC()

    {
    MUS_LOG("mus: [MUSAVA]: -> CMusAvaCapabilityQueryBase::ContentLC" )
   
    CSdpDocument* content = CSdpDocument::NewLC();

    RStringPool strPool = SdpCodecStringPool::StringPoolL();
	RStringF netType = 
	MusAvaCapabilityContext::SDPStringL( SdpCodecStringConstants::ENetType );
    
    _LIT8(KOriginFieldUser1, "-");
    _LIT8(KSessionName, "-");
    content->SetSessionNameL( KSessionName );

    TPtrC8 localHost;
    RStringF hostType;
    
    TTime now;
    now.UniversalTime();
    TInt64 rand = now.Int64();
    TInt64 sessionID = Math::Rand( rand );

    hostType = strPool.StringF( SdpCodecStringConstants::EAddressTypeIP4, 
	                              SdpCodecStringPool::StringTableL() );
	MUS_LOG( "EAddressTypeIP4 from string pool" ) 
	
	TInetAddr aAddr(0);
    iSIPConnection.GetLocalAddrL(aAddr);
    TBuf<50> ipAddress;	
	aAddr.Output(ipAddress);
	MUS_LOG_TDESC( "mus: [MUSAVA]: LocalIP Address",ipAddress )	
	HBufC8 * ipaddr8=HBufC8::NewLC(ipAddress.Length());	
	ipaddr8->Des().Copy(ipAddress);	
    
    MUS_LOG( "mus: [MUSAVA]: SetOriginField" )	                              
    CSdpOriginField* originField =  CSdpOriginField::NewL( KOriginFieldUser1,
                                 sessionID,
                                 sessionID,
                                 netType,
                                 hostType,
                                 *ipaddr8);                                 
    content->SetOriginField( originField );    
    // Connection field
    MUS_LOG( "mus: [MUSAVA]: SetConnectionField" ) 
    CSdpConnectionField* connectionField = 
        CSdpConnectionField::NewL( netType, hostType,*ipaddr8);     
    content->SetConnectionField( connectionField );     
    CleanupStack::PopAndDestroy(ipaddr8);

    OtherSDPHeadersL(*content);    
            
    MUS_LOG( "mus: [MUSAVA]: Constructing the Sdp Content" ) 
    CBufFlat* encBuf = CBufFlat::NewL( KMUSAVAMaxLengthOfSDPMessage );
    CleanupStack::PushL( encBuf );
    RBufWriteStream writeStream( *encBuf, 0 );
	writeStream.PushL();
    content->EncodeL( writeStream );
    CleanupStack::PopAndDestroy();//writeStream    
    HBufC8* textContent = encBuf->Ptr( 0 ).AllocL();
    CleanupStack::PopAndDestroy( encBuf );

    CleanupStack::PopAndDestroy(content);  

    CleanupStack::PushL(textContent);
        
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityQueryBase::ContentLC" )    
    return textContent;
    }    

// --------------------------------------------------------------------------
// CMusAvaCapabilityQueryBase::OtherSDPHeadersL
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityQueryBase::OtherSDPHeadersL( CSdpDocument& aResponseContent )

    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityQueryBase::OtherSDPHeadersL" )

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
    
    MUS_LOG( "mus: [MUSAVA] Adding media line to SDP" )
    //media line
    CSdpMediaField* mediaLine=CSdpMediaField::NewLC( 
                    MusAvaCapabilityContext::SDPStringL
    				(SdpCodecStringConstants::EMediaVideo ),
                     							       NULL,
                     MusAvaCapabilityContext::SDPStringL
                (SdpCodecStringConstants::EProtocolRtpAvp ), 
                        KCapabilitySwisFormatListH263Only );
    
    //adding H.264 codec to SDP
    if( !MultimediaSharingSettings::IsAvcDisabled())
        {
        MUS_LOG( "mus: [MUSAVA]  - Adding avc to supported codec list" )        
        // reconstruct media line with H264 and H263
        CleanupStack::PopAndDestroy(mediaLine);
        mediaLine = CSdpMediaField::NewLC( 
                MusAvaCapabilityContext::SDPStringL
                    (SdpCodecStringConstants::EMediaVideo ),
                                                       NULL,
                         MusAvaCapabilityContext::SDPStringL
                 (SdpCodecStringConstants::EProtocolRtpAvp ), 
                                 KCapabilitySwisFormatList );
        
        CSdpFmtAttributeField* rtpMapH264 = 
            CMusAvaCapability::RtpMapLineLC( KCapabilitySDPAttributeCodecH264,
                        KCapabilitySDPAttributePayloadTypeH264,
                        KCapabilitySDPAttributeClockrate ); 
        
        mediaLine->FormatAttributeFields().AppendL( rtpMapH264 );
        CleanupStack::Pop( rtpMapH264 );
        }

    //H.263 codec
    MUS_LOG( "mus: [ENGINE]  - Adding h263 to supported codec list" )
    CSdpFmtAttributeField* rtpMapH263 = 
        CMusAvaCapability::RtpMapLineLC( KCapabilitySDPAttributeCodecH263,
                    KCapabilitySDPAttributePayloadType,
                    KCapabilitySDPAttributeClockrate ); 

    mediaLine->FormatAttributeFields().AppendL( rtpMapH263 );
    CleanupStack::Pop( rtpMapH263 );
    
    aResponseContent.MediaFields().AppendL( mediaLine );
    CleanupStack::Pop( mediaLine );

    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityQueryBase::OtherSDPHeadersL" )
    }


