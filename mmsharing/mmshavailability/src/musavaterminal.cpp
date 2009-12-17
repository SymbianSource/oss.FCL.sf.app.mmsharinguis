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
* Description:   Discovered terminal
*
*/


#include "musavaterminal.h"

#include <escapeutils.h>
#include <uri8.h>
#include <sip.h>
#include <sipconnection.h>
#include <sipservertransaction.h>
#include <sipclienttransaction.h>
#include <sipheaderbase.h>
#include <sipextensionheader.h>
#include <sdpdocument.h>
#include <sipcontenttypeheader.h>
#include <sipmessageelements.h>
#include <sipresponseelements.h>
#include <siprequestelements.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sipcontactheader.h>
#include <sipaddress.h>

#include "muslogger.h"
#include "musavacapabilityexchange.h"
#include "musavacapability.h"
#include "musavacapabilityquerybase.h"
#include "musavacapabilitycontext.h"
#include "mmusavacapabilityqueryobserver.h"
#include "mussettings.h"
#include "mussettingskeys.h"
#include "musavasipheaderutil.h"

// --------------------------------------------------------------------------
// Symbian two-phase constructor
// --------------------------------------------------------------------------
//
CMusAvaTerminal* CMusAvaTerminal::NewL( CMusAvaCapabilityExchange& aExchange,
                                    const TDesC8& aTerminalUri,
                                    const TDesC8& aTerminalId )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::NewL" )
    CMusAvaTerminal* self = new (ELeave) CMusAvaTerminal( aExchange );
    CleanupStack::PushL( self );
    self->ConstructL( aTerminalUri, aTerminalId );
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::NewL" )
    return self;
    }


// --------------------------------------------------------------------------
// C++ destructor
// --------------------------------------------------------------------------
//
CMusAvaTerminal::~CMusAvaTerminal()
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::~CMusAvaTerminal" )

    ResetAndDestroyQuery();
    
    delete iUri;
    delete iTerminalId;
    iRequestedQuery = NULL;
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::~CMusAvaTerminal" )
    }

// --------------------------------------------------------------------------
// C++ constructor
// --------------------------------------------------------------------------
//
CMusAvaTerminal::CMusAvaTerminal( CMusAvaCapabilityExchange& aExchange )
    : iExchange( aExchange )
    {
    }


// --------------------------------------------------------------------------
// Symbian second-phase constructor
// --------------------------------------------------------------------------
//
void CMusAvaTerminal::ConstructL( const TDesC8& aTerminalUri,
                                const TDesC8& aTerminalId )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::ConstructL" )

    MUS_LOG_TDESC8( "mus: [MUSAVA] - CMusAvaTerminal::Id ",aTerminalId)
    MUS_LOG_TDESC8( "mus: [MUSAVA] - CMusAvaTerminal::MatchL Uri",aTerminalUri )
    iUri = aTerminalUri.AllocL();
    iTerminalId = aTerminalId.AllocL();

    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::ConstructL" )
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::Id
// --------------------------------------------------------------------------
//
const TDesC8& CMusAvaTerminal::Id() const
    {
    return *iTerminalId;
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::ToUriL
// --------------------------------------------------------------------------
//
const TDesC8& CMusAvaTerminal::Uri() const
    {
    return *iUri;
    }
    

// --------------------------------------------------------------------------
// CMusAvaTerminal::MacthL
// --------------------------------------------------------------------------
//
TBool CMusAvaTerminal::MatchL( const TDesC8& aTerminalUri, 
                             const TDesC8& aTerminalId )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::MatchL" )
    TBool match = EFalse;
    TBool useDoubleKeyMatch = Id().Length() > 0 && aTerminalId.Length() > 0;
    
    MUS_LOG_TDESC8( "mus: [MUSAVA] - CMusAvaTerminal::MatchL aTerminalId ",aTerminalId )
    MUS_LOG_TDESC8( "mus: [MUSAVA] - CMusAvaTerminal::MatchL aTerminalUri ",aTerminalUri )    
    if ( useDoubleKeyMatch )
        {
        match = Uri().Compare( aTerminalUri ) == 0 && 
                Id().CompareF( aTerminalId ) == 0;
        }
    else //not known yet or no terminal id in request i.e. best guess
        {
        match = Uri().Compare( aTerminalUri ) == 0;
        }
        
    if ( match )
        {
        AttachUriL( aTerminalUri );
        AttachIdL( aTerminalId );
        }

    MUS_LOG1( "mus: [MUSAVA] <- CMusAvaTerminal::MatchL = %d",match )
    return match;
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::RequestedQuery
// --------------------------------------------------------------------------
//
const CMusAvaCapability* CMusAvaTerminal::RequestedQuery() const
    {
    return iRequestedQuery;
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::ExecuteQueryL
// --------------------------------------------------------------------------
//
void CMusAvaTerminal::ExecuteQueryL( CMusAvaCapabilityQueryBase* aQuery )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::ExecuteQueryL" )
    
    __ASSERT_ALWAYS( aQuery , User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( 
        aQuery->State() == CMusAvaCapabilityQueryBase::ECapabilityQueryCreated,
        User::Leave( KErrArgument ) );
    __ASSERT_ALWAYS( !iQuery, User::Leave( KErrAlreadyExists ) );

    ResetAndDestroyQuery();
                  
    aQuery->ExecuteL();
    
    iQuery = aQuery;                           
        
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::ExecuteQueryL" )
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::QueryCompletedL
// --------------------------------------------------------------------------
//
TBool CMusAvaTerminal::QueryCompletedL( const CSIPClientTransaction& aResponse )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::QueryCompletedL" )
    TBool consumed = EFalse;
    
    if ( iQuery && iQuery->State() != 
        CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted )
        {
        iQuery->CompletedL( aResponse );
        consumed = iQuery->State() == 
            CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted;
        
        if ( consumed )
            {
            const CSIPExtensionHeader* userAgent = 
                MusAvaCapabilityContext::UserAgentHeaderL( aResponse );
            if ( userAgent )
                {
                AttachIdL( userAgent->Value() );
                }
            }
        }
        
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::QueryCompletedL" )
    
    return consumed;
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::QueryCanceled
// --------------------------------------------------------------------------
//
TBool CMusAvaTerminal::QueryCanceled( 
                                const CSIPClientTransaction& aTransaction )
    {
    TBool consumed = EFalse;
    if ( iQuery && iQuery->State() != 
            CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted )
        {
        iQuery->Canceled( aTransaction );
        consumed = iQuery->State() == 
            CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted;
        }
    return consumed;
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::QueryExecuting
// --------------------------------------------------------------------------
//
TBool CMusAvaTerminal::QueryExecuting()
    {
    TBool executing = EFalse;
    if ( iQuery && iQuery->State() == 
            CMusAvaCapabilityQueryBase::ECapabilityQueryExecuting )
        {
        executing = ETrue;
        }
    return executing;
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::QueryRequestedL
// --------------------------------------------------------------------------
//
void CMusAvaTerminal::QueryRequestedL( CSIPServerTransaction& aQuery )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::QueryRequestedL" )

    CSIPResponseElements* response = 
          CSIPResponseElements::NewLC( 
                      KMUSAVASIP200, 
                      SIPStrings::StringF( SipStrConsts::EPhraseOk ) );
    
    CSdpDocument* sdpContent = 
        CMusAvaCapability::ResponseContentLC( aQuery );

    RPointerArray<CSIPHeaderBase> headers;
    CSIPHeaderBase::PushLC( &headers );
    
    if ( PopulateResponseL( aQuery, headers, *sdpContent ) )
        {
        RStringF feature = 
                 MusAvaCapabilityContext::SIPStringL( KCapabilitySwisFeature );
        CleanupClosePushL( feature );
        CMusAvaSipheaderUtil::AddAcceptContactHeaderL( headers,
                                                        feature );
        CleanupStack::PopAndDestroy(); //feature
        MUS_LOG( "mus: [MUSAVA]  AddAcceptContactHeaderL -> Success ")            

        MUS_LOG( "mus: [MUSAVA]  AddAcceptEncodingHeaderL -> Success ")                        
   
        MUS_LOG( "mus: [MUSAVA]  AddAcceptLanguageHeaderL -> Success ")                        
        CMusAvaSipheaderUtil::AddAcceptSdpHeaderL(headers); 
        MUS_LOG( "mus: [MUSAVA]  AddAcceptSDPHeaderL -> Success ")                                    
        AddResponseHeadersL( *response, headers );
        CleanupStack::Pop( &headers );        
        AddResponseContentL( *response, *sdpContent );        
        }
    else
        {
        CleanupStack::PopAndDestroy( &headers );
        response->SetStatusCodeL( KMUSAVASIPNotImplemented );
        response->SetReasonPhraseL( 
            SIPStrings::StringF( SipStrConsts::EPhraseNotImplemented ) );
        }
    CleanupStack::PopAndDestroy( sdpContent );

    MUS_LOG( "mus: [MUSAVA] -> Sending Response For Options Request ..." )    
    aQuery.SendResponseL( response );
    CleanupStack::Pop( response );
    
    iExchange.QueryObserver().CapabilityQueryAnswered( ETrue );
    
    // Add sipaddress proposal
    if ( response->StatusCode( ) == KMUSAVASIP200 )
        {
                                             
        const CSIPContactHeader* contact = 
            MusAvaCapabilityContext::ContactHeaderL( aQuery );
                                             
        if ( !contact )
            {
            MUS_LOG( "mus: [MUSAVA] => FAILED due lack of contact header" )
            }
        else
            {
    /*
           	const TDesC8& uriInContact =
    		    contact->SIPAddress()->Uri8().Uri().UriDes();
    		if ( uriInContact.Length() > 0 )
    		    {
    		    HBufC* uri = 
    		        EscapeUtils::ConvertToUnicodeFromUtf8L( uriInContact );
    		    CleanupStack::PushL( uri );   
    		    iExchange.QueryObserver().CapabilitiesResolvedL( *uri );
    		    CleanupStack::PopAndDestroy( uri );
    		    }
    */

            // If there is P-Asserted-Identity header then send the OPTIONS
            // to this. Store it in settings data as To header so then future
            // request can be sent there.
            
            CDesCArrayFlat* pAssertedIdentityHeaders =
                     MusAvaCapabilityContext::PAssertedIdentityHeaderL(  
                                 MusAvaCapabilityContext::MessageElementsL(aQuery));
            CleanupStack::PushL( pAssertedIdentityHeaders );
            __ASSERT_ALWAYS(pAssertedIdentityHeaders,User::Leave(KErrArgument));
            if(pAssertedIdentityHeaders->Count()>0)
                {
                iExchange.QueryObserver().SipHeadersL(*pAssertedIdentityHeaders,
                                                      SipStrConsts::EToHeader);
                }
            else
                {
                // P-Asserted-Identity header not exist. So rely on other contacts
                // read from phone book or Tel URI to send request.
                }
            pAssertedIdentityHeaders->Reset();
            CleanupStack::PopAndDestroy( pAssertedIdentityHeaders ); 

   		    TBool sequential = 
                  MultimediaSharingSettings::OperatorVariantSettingL() ==
                  	MusSettingsKeys::EOperatorSpecific || 
                  MusSettingsKeys::ESequential ==  
     	        	MultimediaSharingSettings::CapabilityQuerySettingL();
         
            TBool alreadyRequested = iExchange.QueryObserver().
                   CapabilitiesResolvedForCingular();

            if ( sequential && !alreadyRequested )
  		        {
   		        iExchange.QueryObserver().SetCapabilitiesResolvedForCingular();
   		        }
            }
        }
    
    
    //if pending query
    if ( iQuery )
        {
        MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::QueryRequestedL Executing Pending Query" )
        iQuery->ExecuteL();
        }

    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::QueryRequestedL" )
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::AttachQuery
// --------------------------------------------------------------------------
//
TInt CMusAvaTerminal::AttachQuery( CMusAvaCapabilityQueryBase& /*aQuery*/ )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::AttachQuery" )
    iQueries++; 
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::AttachQuery" )
    return iQueries;
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::DetachQuery
// --------------------------------------------------------------------------
//
TInt CMusAvaTerminal::DetachQuery( CMusAvaCapabilityQueryBase& /*aQuery*/ )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::DetachQuery" )
    iQueries--;
    __ASSERT_ALWAYS( iQueries >= 0, 
                        User::Panic( _L("CMusAvaTerminal"), KErrGeneral ) );
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::DetachQuery" )
    
    return iQueries;
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::AttachIdL
// --------------------------------------------------------------------------
//
void CMusAvaTerminal::AttachIdL( const TDesC8& aTerminalId )
    {
    if ( aTerminalId.Length() > 0 )
        {
        //terminal id might change                                
        HBufC8* tmpTerminalId = aTerminalId.AllocL();
        delete iTerminalId;
        iTerminalId = tmpTerminalId;
        }
    }
    

// --------------------------------------------------------------------------
// CMusAvaTerminal::AttachUriL
// --------------------------------------------------------------------------
//
void CMusAvaTerminal::AttachUriL( const TDesC8& aUri )
    {
    if ( aUri.Length() > 0 )
        {
        delete iUri;
        iUri = NULL;
        iUri = aUri.AllocL();
        }
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::ResetAndDestroyQuery
// --------------------------------------------------------------------------
//
void CMusAvaTerminal::ResetAndDestroyQuery()
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::ResetAndDestroyQuery" )
    CMusAvaCapabilityQueryBase* query = iQuery;
    iQuery = NULL;
    delete query;
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::ResetAndDestroyQuery" )
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::PopulateResponseL
// --------------------------------------------------------------------------
//
TBool CMusAvaTerminal::PopulateResponseL( CSIPServerTransaction& aQuery,
                            RPointerArray<CSIPHeaderBase>& aResponseHeaders,
                            CSdpDocument& aSdpContent )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::PopulateResponseL" )
    
    TBool supported = EFalse;
    
    const CSIPAcceptContactHeader* featureSet = 
        MusAvaCapabilityContext::AcceptContactHeaderL( aQuery );
    
    User::LeaveIfError( !featureSet ? KErrGeneral : KErrNone );
    
    for( TInt i = 0; i < iExchange.Capabilities().Count();i++ )
        {
        CMusAvaCapability* capability = iExchange.Capabilities()[ i ];
        TBool containsFeature = capability->Supports( *featureSet );
        supported = containsFeature ? ETrue : supported;
                           
        if ( containsFeature )
            {
            const CSIPMessageElements& elements = 
                    MusAvaCapabilityContext::MessageElementsL( aQuery );
            const TDesC8& content = elements.Content();
            CSdpDocument* sdp = NULL;
            
            if ( content.Length() > 0 )
                {
                sdp = CSdpDocument::DecodeLC( content );                
                }
            else
                {
                sdp = CSdpDocument::NewLC();
                }

            //store video codecs that came from the other terminal
            CDesCArrayFlat* videoCodecs = CMusAvaCapability::ResolveCodecsL( *sdp );
            CleanupStack::PushL( videoCodecs );
            capability->Exchange().QueryObserver().VideoCodecsResolvedL( *videoCodecs );
            CleanupStack::PopAndDestroy( videoCodecs );                    
        
            CleanupStack::PopAndDestroy( sdp );
            
            capability->PopulateResponseL( aQuery,
                                           aResponseHeaders,
                                           aSdpContent );
            iRequestedQuery = capability;
            }
        }
        
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::PopulateResponseL" )
        
    return supported;
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::AddResponseHeadersL
// --------------------------------------------------------------------------
//
void CMusAvaTerminal::AddResponseHeadersL( CSIPResponseElements& aResponse,
                            RPointerArray<CSIPHeaderBase>& aResponseHeaders )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::AddResponseHeadersL" )

    CSIPMessageElements& message = aResponse.MessageElements();

    if ( iExchange.TerminalId().Length() > 0 )
        {
        //terminal id
    	CSIPHeaderBase* userAgent = 
    	    MusAvaCapabilityContext::UserAgentHeaderLC( 
    	        iExchange.TerminalId() );
        aResponseHeaders.AppendL( userAgent );
        CleanupStack::Pop( userAgent );
        }
        
    //Set user headers
    message.SetUserHeadersL( aResponseHeaders );
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::AddResponseHeadersL" )
    }


// --------------------------------------------------------------------------
// CMusAvaTerminal::AddResponseContentL
// --------------------------------------------------------------------------
//
void CMusAvaTerminal::AddResponseContentL( CSIPResponseElements& aResponse,
                                        CSdpDocument& aSdpContent )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaTerminal::AddResponseContentL" )

    CSIPMessageElements& message = aResponse.MessageElements();

    //create content from SDP document
    HBufC8* content = MusAvaCapabilityContext::ToTextL( aSdpContent );
    CleanupStack::PushL( content );
    
    //create content type
    CSIPContentTypeHeader* contentTypeHeader = NULL;
    contentTypeHeader =
        CSIPContentTypeHeader::NewLC( KMUSAVASIPMediaTypeApplication,
                                      KMUSAVASIPMediaSubTypeSDP );
    
    //set content
    message.SetContentL( content, contentTypeHeader );
    CleanupStack::Pop( contentTypeHeader );
    CleanupStack::Pop( content );

    MUS_LOG( "mus: [MUSAVA] <- CMusAvaTerminal::AddResponseContentL" )
    }
// --------------------------------------------------------------------------
// CMusAvaTerminal::IsDiscovered
// --------------------------------------------------------------------------
//
TBool CMusAvaTerminal::IsDiscovered()
    {
    TBool isDiscovered = EFalse;
    TBool isGarbage = !iQueries && !RequestedQuery();
    
    if ( !isGarbage )
        {
        isDiscovered = 
            iQueries == 1 &&
            iQuery && 
            iQuery->State() == CMusAvaCapabilityQueryBase::ECapabilityQueryCompleted;
        }
    else
        {
        isDiscovered = ETrue;
        }
        
    return isDiscovered;
    
    }
