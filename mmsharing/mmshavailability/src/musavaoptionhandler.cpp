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


#include "musavaoptionhandler.h"

#include <escapeutils.h>
#include <siperr.h>
#include "musunittesting.h"
#include "musavasharedobject.h"
#include "muslogger.h"
#include "musavasip.h"
#include "musavasettingsimp.h"
#include "musavacapabilitysipagent.h"
#include "musavacapabilityexchange.h"
#include "musavacapabilityquery.h"
#include "musavacapability.h"
#include "musavacapabilityquerybase.h"
#include "musavaterminal.h"
#include "mussettingskeys.h"
#include "mussettings.h"
#include "mussesseioninformationapi.h"


_LIT( KMusSipPrefix, "sip:" );
_LIT( KMusTelPrefix, "tel:" );
_LIT( KMusPlusSign, "+" );

const TInt KMaxUriLength = 512;
const TInt KMusMinDigitCountInTelNumber = 7;

const TInt KMusOptionsHandlerIndex = 1;


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusAvaOptionHandler* CMusAvaOptionHandler::NewL(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    {
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaOptionHandler::NewL()" )
    CMusAvaOptionHandler* self = CMusAvaOptionHandler::NewLC(
        aObserver,
        aSettings );
    CleanupStack::Pop( self );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaOptionHandler::NewL()" )
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusAvaOptionHandler* CMusAvaOptionHandler::NewLC(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings  )
    {
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaOptionHandler::NewLC()" )
    CMusAvaOptionHandler* self = new( ELeave ) CMusAvaOptionHandler(
        aObserver,
        aSettings );
    CleanupStack::PushL( self );
    self->ConstructL();

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaOptionHandler::NewLC()" )
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusAvaOptionHandler::~CMusAvaOptionHandler()
    {
    MUS_LOG(
        "mus: [MUSAVA] -> CMusAvaOptionHandler::~CMusAvaOptionHandler()" )
    if (iSharedObj )
        {
        iSharedObj->DeleteSingleton();
        }
    delete iCapabilityExchange;
    iCapabilityExchange = NULL;
    delete iSipAgent;
    iSipAgent = NULL;

    MUS_LOG(
        "mus: [MUSAVA] <- CMusAvaOptionHandler::~CMusAvaOptionHandler()" )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusAvaOptionHandler::CMusAvaOptionHandler(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    :CMusAvaAvailability( aObserver ),
    iSettings( aSettings )
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::ConstructL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaOptionHandler::ConstructL()" )

    //construct capability exchange
    ConstructCapabilityExchangeL();

    iSharedObj = CMusAvaSharedObject::GetSingletonL();
    iSipAgent = CMusAvaCapabilitySipAgent::NewL( *iCapabilityExchange,
                                                *iSharedObj,
                                                iObserver );
    iSharedObj->MusAvaSip().AddAdapterL(
                            ( MMusAvaSipConnectionAdapter& ) *iSipAgent,
                            KMusOptionsHandlerIndex );
    iSharedObj->MusAvaSip().AddAdapterL(
                            ( MMusAvaSipAdapter& ) *iSipAgent, 
                            KMusOptionsHandlerIndex ); 
    
    MUS_LOG( "mus: [MUSAVA]     Check fast mode capability" )
    
    TRAPD( err,
    iFastModeCapable = 
        MultimediaSharingSettings::FastStartupModeL() == MusSettingsKeys::EFastModeOn  && 
        MultimediaSharingSettings::VideoDirectionL() == MusSettingsKeys::ETwoWayVideo &&
        MultimediaSharingSettings::CapabilityQuerySettingL() == MusSettingsKeys::EParallel );
    if ( err == KErrNoMemory )
        {
        User::Leave( err );
        }
    
    MUS_LOG2( "mus: [MUSAVA]     Fast mode check, err:%d, capable:%d", 
              err, iFastModeCapable )
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaOptionHandler::ConstructL()" )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CMusAvaOptionHandler::CapabilityQueryAnswered( TBool aAnswered )
    {
    MUS_LOG1( "mus: [MUSAVA]  <-> CMusAvaOptionHandler::CapabilityQueryAnswered():%d", 
              aAnswered )
    
    iCapabilityQueryAnswered = aAnswered ? aAnswered : iCapabilityQueryAnswered;
    
    HandleFastModeQueryAnswered();
    
    return iCapabilityQueryAnswered;
        
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::ConstructCapabilityExchangeL()
    {
    MUS_LOG(
    "mus: [MUSAVA]  -> CMusAvaOptionHandler::ConstructCapabilityExchangeL()" )
    HBufC8* terminalId = ConstructTerminalIdL();
    CleanupStack::PushL( terminalId );

    //create capability exchange
    iCapabilityExchange =
        CMusAvaCapabilityExchange::NewL( *terminalId, *this );

    //create SWIS capability
    CMusAvaCapability* capability = CMusAvaCapability::NewL(
                                                    *iCapabilityExchange );
    CleanupStack::PushL( capability );
    iCapabilityExchange->AddCapabilityL( capability );
    CleanupStack::Pop( capability );

    iSwisCapability = capability;

    CleanupStack::PopAndDestroy( terminalId );
    MUS_LOG(
    "mus: [MUSAVA]  <- CMusAvaOptionHandler::ConstructCapabilityExchangeL()" )
    }


// -----------------------------------------------------------------------------
// @TODO: Currently terminal id is not in use i.e User-Agent header is not
// sent in request nor response.
// -----------------------------------------------------------------------------
//
HBufC8* CMusAvaOptionHandler::ConstructTerminalIdL()
    {
    return KNullDesC8().AllocL();
    }
       	

// -----------------------------------------------------------------------------
//  Starts the loopy execution.
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::DoExecuteL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaOptionHandler::DoExecuteL()" )
    
    MusSettingsKeys::TOperatorVariant variantSetting = 
        MultimediaSharingSettings::OperatorVariantSettingL();
    
    /*
     Do not resent the OPTIONS request in the below scenario
     i) Options Sent and waiting for Response.
     ii) We already sent OPTIONS and got positive response.So we know that
         other device is VS capable.
     iii) We already sent OPTIONS and got negative response.So we know that
         other device is VS incapable.

    Also OPTIONS should be sent only once if it matches to VS Call criteria.
    */
    MUS_LOG1( "mus: [MUSAVA]  - Current State %d",State() )

    if ( State() == MMusAvaObserver::EMusAvaStatusOptionsSent ||
         State() == MMusAvaObserver::EMusAvaStatusAvailable ||
         State() == MMusAvaObserver::EMusAvaOptionNotAvailable )
        {
        return;
        }
        
    if ( variantSetting == MusSettingsKeys::EOperatorSpecific 
        && iSettings.CallDirection() == 2 && !iCapabilitiesRequestAnswered )
        { // terminated party 
        DoSetState( MMusAvaObserver::EMusAvaStatusOptionsNotSent );
        }
    else if ( MusSettingsKeys::ESequential ==  
     	        MultimediaSharingSettings::CapabilityQuerySettingL()
        && iSettings.CallDirection() == 2 && !iCapabilitiesRequestAnswered )
        { // terminated party 
        DoSetState( MMusAvaObserver::EMusAvaStatusOptionsNotSent );
        }
    else
        {
        if ( MusSettingsKeys::EOperatorSpecific == variantSetting ||
             MusSettingsKeys::EParallel ==  
     	     	MultimediaSharingSettings::CapabilityQuerySettingL() || 
     	     MusSettingsKeys::ESequential ==  
     	        MultimediaSharingSettings::CapabilityQuerySettingL() )
    	    {
    	    const MDesCArray& addresses = iSettings.SipAddresses();

    	    TInt addressesCount = addresses.MdcaCount();
    	    MUS_LOG1( "mus: [MUSAVA]    addresses.MdcaCount() %d", 
    	        addressesCount )
    	    if( addressesCount )
    	        {
    	        const TDesC& sipAddress = addresses.MdcaPoint( 0 );
    	        MUS_LOG_TDESC( "mus: [MUSAVA]       SIP Address: ", 
    	            sipAddress )
    	     	TRAPD( err, iSipAgent->ExecuteCapabilityQueryL( 
    	     	    *iSwisCapability, sipAddress ) );                               
	    	    // set status available and report to the observer
    		    if ( err == KErrNone )
    		    	{
    	    	    HBufC8* sipAddress8 = 
    	    	        EscapeUtils::ConvertFromUnicodeToUtf8L( sipAddress );
                    CleanupStack::PushL( sipAddress8 );
                    if ( iCapabilityExchange->
                        TerminalL( sipAddress8->Des() ).QueryExecuting() )
                        {
                        DoSetState( MMusAvaObserver::EMusAvaStatusOptionsSent );
                        }
    		    	else
    		    	    {
    		    	    DoSetState( MMusAvaObserver::EMusAvaStatusAvailable );	
    		            }
    		        CleanupStack::PopAndDestroy( sipAddress8 );
    		    	}
                // when profile is in when needed mode and registration still on
                // going we get this error and wait untill registration is successful
                // since we did not send any options set the state EMusAvaStatusNotExecuted.
                else if( err == KErrSIPInvalidRegistrationState )
                    {
                    DoSetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
                    }
                // anything other than this should be considered default , means options sent
                // and was not successful.
    		    else
    		        {
    		        DoSetState( MMusAvaObserver::EMusAvaOptionNotAvailable );
    		        }
    	        }
    	    else
    	    	{
    	    	DoSetState( MMusAvaObserver::EMusAvaStatusOptionsNotSent );	
    	    	}
        	}
        else
        	{
        	// option sending not needed 
        	DoSetState( MMusAvaObserver::EMusAvaStatusOptionsNotSent );
        	}  
        }
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaOptionHandler::DoExecuteL()" )
    }


// -----------------------------------------------------------------------------
//  Stops executing availability.
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::Stop()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaOptionHandler::Stop()" )
    // TBD
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaOptionHandler::Stop()" )
    }


// -----------------------------------------------------------------------------
//  Returns name of *this* availability.
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityName CMusAvaOptionHandler::Name()
    {
    return MMusAvaObserver::EMusAvaOptionHandler;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::PrepareForReceivedInviteL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaOptionHandler::PrepareForReceivedInviteL()" )
    
    if ( iFastModeAvailabilityDelayed )
        {
        MUS_LOG( "mus: [MUSAVA]     Set delayed availability" )
        SetState( MMusAvaObserver::EMusAvaStatusAvailable );
        iFastModeAvailabilityDelayed = EFalse;
        }
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaOptionHandler::PrepareForReceivedInviteL()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::CapabilitiesResolved(
                            const CMusAvaCapabilityQueryBase& aSentQuery )
    {
    MUS_LOG(
        "mus: [MUSAVA]  -> CMusAvaOptionHandler::CapabilitiesResolved()" )
    if ( aSentQuery.Result() == KCapabilityCapabilitesReady )
        {
        // tell the upper layer that
        // query was succesfull. VS is available
        DoSetState( MMusAvaObserver::EMusAvaStatusAvailable );
        }
    else if ( aSentQuery.Result() == KCapabilityCapabilitiesForbidden )
        {
        // query returned with response "403 Forbidden". VS is NOT available
        DoSetState( MMusAvaObserver::EMusAvaFailureCode );
        }
    else
        {
        //query failed. VS is NOT available
        DoSetState( MMusAvaObserver::EMusAvaOptionNotAvailable );
        }

    MUS_LOG(
        "mus: [MUSAVA]  <- CMusAvaOptionHandler::CapabilitiesResolved()" )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::CapabilitiesResolvedL( const TDesC& aUri )
    {
    MUS_LOG(
        "mus: [MUSAVA]  -> CMusAvaOptionHandler::CapabilitiesResolvedL()" )
    if ( aUri.Length() > 0 )
        {
        // Set the sip address resolved from succesfull OPTIONS response
        // Old adress(es) are destroyed
        CDesCArrayFlat* sipAddresses = new( ELeave ) CDesCArrayFlat( 1 );
        CleanupStack::PushL( sipAddresses );
         
        sipAddresses->AppendL( aUri );
        iSettings.SetSipAddressesL( *sipAddresses );
        
        MUS_LOG_TDESC( "mus: [MUSAVA]       SIP Address: ", aUri )       
        
        sipAddresses->Reset();
        CleanupStack::PopAndDestroy( sipAddresses );
        }

    MUS_LOG(
        "mus: [MUSAVA]  <- CMusAvaOptionHandler::CapabilitiesResolvedL()" )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::SetCapabilitiesResolvedForCingular()
    {
    MUS_LOG(
        "mus: [MUSAVA]  -> CMusAvaOptionHandler::\
        SetCapabilitiesResolvedForCingular()" )
    iCapabilitiesRequestAnswered = ETrue;
    
    TRAPD( error, DoExecuteL() );
    if ( error )
        {
        MUS_LOG1( "mus: [MUSAVA]  <- CMusAvaOptionHandler::\
        SetCapabilitiesResolvedForCingular() leave code = %d", error )
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaOptionHandler::\
        SetCapabilitiesResolvedForCingular()" )
        }
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CMusAvaOptionHandler::CapabilitiesResolvedForCingular()
    {
    MUS_LOG(
    "mus: [MUSAVA]  CMusAvaOptionHandler::CapabilitiesResolvedForCingular()" )
    return iCapabilitiesRequestAnswered;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::SipHeadersL(
                                   const MDesCArray& aHeaders,
                                   SipStrConsts::TStrings aHeaderType)
    {
    MUS_LOG("mus: [MUSAVA]  -> CMusAvaOptionHandler::SipHeadersL()" )        

    // if header type is to then store in iSettings , so then future
    // request will be send there.
    if(aHeaderType == SipStrConsts::EToHeader)
        {        
        // All addresses received should be valid, so choose just one to avoid
        // situation where user has to choose one from equal addresses.
        // Prefer SIP URI, but accept also TEL URI.
        
        TBuf<KMaxUriLength> sipUri;
        TBuf<KMaxUriLength> telUri;

        for ( TInt i = 0; i < aHeaders.MdcaCount(); ++i )
            {
            if ( aHeaders.MdcaPoint( i ).FindF( KMusSipPrefix ) != 
                 KErrNotFound )
    	    	{
    	    	sipUri.Copy( aHeaders.MdcaPoint( i ) );
    	    	}
    	    else if ( aHeaders.MdcaPoint(i).FindF( KMusTelPrefix ) != 
    	              KErrNotFound )
    	        {
    	    	telUri.Copy( aHeaders.MdcaPoint( i ) );
    	    	}
    	    else
    	        {
    	        // NOP
    	        }
            }
        
        CDesCArray* addresses = new( ELeave ) CDesCArrayFlat( 1 );
        CleanupStack::PushL( addresses );
        
        if ( telUri.Length() > 0 )
            {
            telUri.Trim();
            addresses->AppendL( telUri );
            // Check if TEL URI conforms to phone number currently hold.
            // If they do not match, we replace phone number with one parsed
            // out of received from P-Asserted-Identity header. Since in this 
            // scenario we cannot be sure about validity of contact name either, 
            // we empty the contact name. This is only not to show incorrect 
            // information, but this solution does not show possibly existing 
            // contact name.
            
            // We compare last seven digits, since that is the minimum amount
            // that can make up a valid telephone number. 
            // Variable telUri holds also prefix, but that does not affect the
            // righthand comparison.
            
            MUS_LOG_TDESC( "mus: [MUSUI ]       iSettings.TelNumber(): ",
                           iSettings.TelNumber() )
            MUS_LOG_TDESC( "mus: [MUSUI ]       telUri: ", telUri )
            
            TPtrC16 telUriWithoutPrefix = 
                    telUri.Right( telUri.Length() - KMusTelPrefix().Length() );
            
            TPtrC16 numberPartOfTelUri = 
                    telUriWithoutPrefix.Find( KMusPlusSign ) == 0 ?
                    telUriWithoutPrefix.Right( telUriWithoutPrefix.Length() - 1 ) :
                    telUriWithoutPrefix;
                    
            if  ( !( iSettings.TelNumber().Length() >= KMusMinDigitCountInTelNumber && 
                     numberPartOfTelUri.Length() >= KMusMinDigitCountInTelNumber &&
                     iSettings.TelNumber().Right( KMusMinDigitCountInTelNumber ) ==
                            telUri.Right( KMusMinDigitCountInTelNumber ) ) )
                {
                iSettings.SetTelNumberL( telUriWithoutPrefix );
                iSettings.SetContactNameL( KNullDesC() );
                iSettings.SetContactId( KErrNotFound );              
                }
            }
            
        if ( sipUri.Length() > 0 )
            {
            // Replace possibly existing TEL URI with SIP URI
            addresses->Reset();
            addresses->AppendL( sipUri );
            }
            
        iSettings.SetSipAddressesL( addresses ); // Transfers ownership
        CleanupStack::Pop( addresses );  
        }
    else
        {
        // should go for future need if any.
        }    
    MUS_LOG("mus: [MUSAVA]  <- CMusAvaOptionHandler::SipHeadersL()" )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::VideoCodecsResolvedL( const MDesCArray& aVideoCodecs )
    {
    MUS_LOG("mus: [MUSAVA]  -> CMusAvaOptionHandler::VideoCodecsResolvedL()" )
    
    iSettings.SetVideoCodecsL( aVideoCodecs );
    
    MUS_LOG("mus: [MUSAVA]  <- CMusAvaOptionHandler::VideoCodecsResolvedL()" )
    }


// -----------------------------------------------------------------------------
// If MO side fast mode negotiation has failed, cannot change the value anymore
// as it is very likely that automatic invitation fails as well.
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::FastModeResolved( MusSettingsKeys::TFastMode aMode )
    {
    MUS_LOG1("mus: [MUSAVA]  -> CMusAvaOptionHandler::FastModeResolved():%d", 
        aMode )
    
    if ( FastModeNegotiationFailedMO() )
        {
        MUS_LOG("mus: [MUSAVA]  Ignore setting as failed already" )
        }
    else
        {
        iSettings.SetFastMode( aMode );
        }
    
    MUS_LOG("mus: [MUSAVA]  <- CMusAvaOptionHandler::FastModeResolved()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CMusAvaOptionHandler::DoSetState( MMusAvaObserver::TAvailabilityStatus aNewState )
    {
    MUS_LOG1( "mus: [MUSAVA]  -> CMusAvaOptionHandler::DoSetState() state:%d", 
              aNewState )
    TInt err( KErrNone );
    
    if ( iFastModeCapable )
        {
        TRAP( err, aNewState = HandleFastModeL( aNewState  ) );
        }
    
    SetState( aNewState );
    
    MUS_LOG1("mus: [MUSAVA]  <- CMusAvaOptionHandler::DoSetState(), err:%d", err )
    
    return err;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityStatus CMusAvaOptionHandler::HandleFastModeL(
    MMusAvaObserver::TAvailabilityStatus aNewState )
    {
    MUS_LOG("mus: [MUSAVA]  -> CMusAvaOptionHandler::HandleFastModeL()" )
    
    switch ( aNewState )
        {
        case MMusAvaObserver::EMusAvaOptionNotAvailable:
            {
            aNewState = HandleFastModeOptionNotAvailableL( aNewState );
            break;
            }
        case MMusAvaObserver::EMusAvaStatusOptionsNotSent:
            {
            aNewState = HandleFastModeOptionsNotSentL( aNewState );
            break;
            }
        case MMusAvaObserver::EMusAvaStatusAvailable:
            {
            aNewState = HandleFastModeAvailableL( aNewState );
            break;
            }
        default:
            {
            break;
            }
        }
    
    MUS_LOG("mus: [MUSAVA]  <- CMusAvaOptionHandler::HandleFastModeL()" )
    
    return aNewState;
    }

// -----------------------------------------------------------------------------
// If fast mode and call originator, application is started
// automatically at background with two-way MO use case. If fast mode is
// negotiated, availability publishing is delayed at MT side until invitation
// is received (as MO will send invite automatically).
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityStatus CMusAvaOptionHandler::HandleFastModeAvailableL(
    MMusAvaObserver::TAvailabilityStatus aNewState )
    {
    MUS_LOG("mus: [MUSAVA]  -> CMusAvaOptionHandler::HandleFastModeAvailableL()" )
    
    __ASSERT_ALWAYS( iSettings.Observer(), User::Leave( KErrNotReady ) );
    
    if ( State() != MMusAvaObserver::EMusAvaStatusAvailable &&
         iSettings.FastMode() == MusSettingsKeys::EFastModeOn )
        {
        if ( iSettings.CallDirection() == NMusSessionInformationApi::ECallOrginated )
            {
            MUS_LOG("mus: [MUSAVA]      Starting application at background" )
            iSettings.Observer()->StartApplicationL( MultimediaSharing::EMusTwoWayVideo );
            }
        else
            {
            MUS_LOG("mus: [MUSAVA]      Delay availability publishing" )
            iFastModeAvailabilityDelayed = ETrue;
            aNewState = State();
            }
        }
    
    MUS_LOG("mus: [MUSAVA]  <- CMusAvaOptionHandler::HandleFastModeAvailableL()" )
    
    return aNewState;
    }

// -----------------------------------------------------------------------------
// If fast mode and call terminated side, answering to 200 ok is already enough
// to set us available in delayed manner as other end can start sending
// invitation already after getting fast mode information in answer. 
// -----------------------------------------------------------------------------
//
void CMusAvaOptionHandler::HandleFastModeQueryAnswered()
    {
    if ( FastModeNegotiatedByAnswerMT() )
        {
        MUS_LOG( "mus: [MUSAVA]     Set fastmode available already as answered" )
        
        // Will result delayed availability handling
        DoSetState( MMusAvaObserver::EMusAvaStatusOptionsNotSent );
        }
    }

// -----------------------------------------------------------------------------
// If options wasn't sent because of missing information, state can be set to
// available immediately as MT cannot start waiting for invite unless it is
// sure that other end is about to send it.
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityStatus 
CMusAvaOptionHandler::HandleFastModeOptionsNotSentL(
    MMusAvaObserver::TAvailabilityStatus aNewState )
    {
    MUS_LOG("mus: [MUSAVA]  -> CMusAvaOptionHandler::HandleFastModeOptionsNotSentL()" )
    
    if ( FastModeNegotiatedByAnswerMT() )
        {
        MUS_LOG("mus: [MUSAVA]      Answered already, set available" )
        // Make available but start waiting for invitation
        aNewState = HandleFastModeAvailableL( MMusAvaObserver::EMusAvaStatusAvailable );
        }
    
    MUS_LOG("mus: [MUSAVA]  <- CMusAvaOptionHandler::HandleFastModeOptionsNotSentL()" )
    
    return aNewState;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityStatus 
CMusAvaOptionHandler::HandleFastModeOptionNotAvailableL(
    MMusAvaObserver::TAvailabilityStatus aNewState )
    {
    MUS_LOG("mus: [MUSAVA]  -> CMusAvaOptionHandler::HandleFastModeOptionNotAvailableL()" )
    
    if ( iFastModeAvailabilityDelayed || 
         State() == MMusAvaObserver::EMusAvaStatusAvailable )
        {
        MUS_LOG("mus: [MUSAVA]  In delayed mode or available, ignore" )
        aNewState = State();
        }
    
    MUS_LOG("mus: [MUSAVA]  <- CMusAvaOptionHandler::HandleFastModeOptionNotAvailableL()" )
    
    return aNewState;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusAvaOptionHandler::FastModeNegotiatedByAnswerMT()
    {
    return ( iFastModeCapable && 
             iCapabilityQueryAnswered && 
             iSettings.FastMode() == MusSettingsKeys::EFastModeOn &&
             iSettings.CallDirection() == NMusSessionInformationApi::ECallTerminated );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusAvaOptionHandler::FastModeNegotiationFailedMO()
    {
    return ( iFastModeCapable && 
             iSettings.FastMode() != MusSettingsKeys::EFastModeOn &&
             iSettings.CallDirection() == NMusSessionInformationApi::ECallOrginated &&
             State() != MMusAvaObserver::EMusAvaStatusAvailable && 
             State() != MMusAvaObserver::EMusAvaStatusOptionsSent );
    }

// End of file
