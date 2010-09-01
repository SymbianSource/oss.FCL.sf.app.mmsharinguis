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
* Description:   SIP event handling class
*
*/


#include "musavacapabilitysipagent.h"
#include <e32svr.h>
#include <rconnmon.h>
#include <sipdialog.h>
#include <sipservertransaction.h>
#include <sipclienttransaction.h>
#include <sipdialogassocbase.h>
#include <sipinvitedialogassoc.h>
#include <sipregistrationbinding.h>
#include <siprefresh.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <sipresponseelements.h>

#include "mussettings.h"
#include "muslogger.h"
#include "musavacapabilityexchange.h"
#include "musavacapabilityquery.h"
#include "musavaterminal.h"
#include "musavasharedobject.h"
#include "musavasip.h"
#include "musavacapabilitycontext.h"
#include "mmusavacapabilityqueryobserver.h"

// --------------------------------------------------------------------------
// C++ constructor
// --------------------------------------------------------------------------
//
CMusAvaCapabilitySipAgent::CMusAvaCapabilitySipAgent( 
                            CMusAvaCapabilityExchange& aCapabilityExchange,
                            CMusAvaSharedObject& aSharedObject,
                            MMusAvaAvailabilityObserver& aAvaObserver)
    : iCapabilityExchange( aCapabilityExchange ),
      iSharedObj( aSharedObject ),
      iAvailabilityObserver( aAvaObserver )
    {
    }


// --------------------------------------------------------------------------
// Symbian two-phase constructor
// --------------------------------------------------------------------------
//
CMusAvaCapabilitySipAgent* CMusAvaCapabilitySipAgent::NewL( 
                            CMusAvaCapabilityExchange& aCapabilityExchange,
                            CMusAvaSharedObject& aSharedObject,
                            MMusAvaAvailabilityObserver& aAvaObserver)
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilitySipAgent::NewL" )
    CMusAvaCapabilitySipAgent* self = 
        new (ELeave) CMusAvaCapabilitySipAgent( aCapabilityExchange, 
                                                aSharedObject,
                                                aAvaObserver );
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilitySipAgent::NewL" )
    return self;
    }


// --------------------------------------------------------------------------
// C++ destructor
// --------------------------------------------------------------------------
//
CMusAvaCapabilitySipAgent::~CMusAvaCapabilitySipAgent()
    {
    MUS_LOG( 
    "mus: [MUSAVA] -> CMusAvaCapabilitySipAgent::~CMusAvaCapabilitySipAgent" )

    delete iSipConnection;
    
    MUS_LOG( 
    "mus: [MUSAVA] <- CMusAvaCapabilitySipAgent::~CMusAvaCapabilitySipAgent" )
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilitySipAgent::ExecuteCapabilityQueryL
// --------------------------------------------------------------------------
// 
void CMusAvaCapabilitySipAgent::ExecuteCapabilityQueryL( 
                                           CMusAvaCapability& aCapability, 
                                           const TDesC& aSipAddress )
    {
    MUS_LOG( 
    "mus: [MUSAVA] -> CMusAvaCapabilitySipAgent::ExecuteCapabilityQueryL" )

    CSIPProfile* profile = iSharedObj.MusAvaSip().Profile();
    if ( !profile )
        {
        User::Leave( KErrNotFound );
        }

    // Fetching connection leaves if profile is not registered for some reason,
    // Thats what we want, forced creation of connection would activate PDP
    // context although there's no reason to do that without registration.
    CMusAvaCapabilityQuery* query = CMusAvaCapabilityQuery::NewL( 
                                        aCapability,
                                        *iSharedObj.MusAvaSip().ConnectionL( EFalse ), 
                                        *profile, 
                                        aSipAddress );
    
    CleanupStack::PushL( query );
    
    query->Terminal().ExecuteQueryL( query );
    
    CleanupStack::Pop( query );
    
    MUS_LOG( 
    "mus: [MUSAVA] <- CMusAvaCapabilitySipAgent::ExecuteCapabilityQueryL" )
    }

// --------------------------------------------------------------------------
// From MSIPObserver, incoming SIP request outside dialog is handled here
// already.
// --------------------------------------------------------------------------
//

TInt CMusAvaCapabilitySipAgent::IncomingRequest( TUint32 /*aIapId*/,
                                    CSIPServerTransaction* /*aTransaction*/ )
    {
    MUS_LOG( 
        "mus: [MUSAVA]  -> CMusAvaCapabilitySipAgent::IncomingRequest()" )
    // If this method was called, there's no connection for this IAP
    MUS_LOG( "mus: [MUSAVA]  if this method was called,\
    	there's no capability" )
    MUS_LOG( 
        "mus: [MUSAVA]  <- CMusAvaCapabilitySipAgent::IncomingRequest()" )
    return KErrNotFound;
    }

// -------------------------------------------------------------------------
// SIP request timed-out and has been destroyed.
// -------------------------------------------------------------------------
//
TInt CMusAvaCapabilitySipAgent::TimedOut(
                                    CSIPServerTransaction& /*aTransaction*/ )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaCapabilitySipAgent::ConstructL()" )
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaCapabilitySipAgent::ConstructL()" )
    return KErrNotSupported;
    }

// --------------------------------------------------------------------------
//  A SIP request outside a dialog has been received from the network.
// --------------------------------------------------------------------------
//
TInt CMusAvaCapabilitySipAgent::IncomingRequest(
                                        CSIPServerTransaction* aTransaction )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilitySipAgent::IncomingRequest" )
    TInt retval = KErrNone;
    MusSettingsKeys::TOperatorVariant variantValue = MusSettingsKeys::EStandard;
    TRAPD( error, variantValue = 
                  MultimediaSharingSettings::OperatorVariantSettingL() );
    if( error )
    	{
         MUS_LOG("CMusAvaCapabilitySipAgent::IncomingRequest\
        		  Get OperatorVariantSettingL worng");
    	}
    
    if ( aTransaction->Type() == 
                        SIPStrings::StringF( SipStrConsts::EOptions ) )
        {  
        // Answer 200OK when CS call remain at Availability States
        //or at EMusAvaOptionNotAvailable State in operatorspecific mode
        if ( MMusAvaObserver::EMusAvaStatusNotExecuted <= 
             iAvailabilityObserver.AvailabilityPluginState()  
             ||( ( iAvailabilityObserver.AvailabilityPluginState() ==
              MMusAvaObserver::EMusAvaOptionNotAvailable ) 
              && ( variantValue == MusSettingsKeys::EOperatorSpecific  )))
            {
            retval = 
             iCapabilityExchange.QueryObserver().CapabilityQueryAnswered() ?
             KErrNotFound : KErrNone;
        
            if ( !retval )
                {
                MUS_LOG( "mus: [MUSAVA] Responding to OPTIONS" )
                iCapabilityExchange.QueryReceived( aTransaction );
                }
            }
        else
            {
            retval = KErrNotFound;
            }
        }
    else        
        {
        MUS_LOG( "mus: [MUSAVA] Default response" )
        TRAP_IGNORE( HandleIncomingRequestL( *aTransaction ) )
        delete aTransaction;
        MUS_LOG( "mus: [MUSAVA] Default response sent" )
        }
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilitySipAgent::IncomingRequest" )
    return retval;
    }

// --------------------------------------------------------------------------
// From MSIPConnectionObserver, incoming SIP request outside dialog is 
// handled here already.
// --------------------------------------------------------------------------
//
void CMusAvaCapabilitySipAgent::HandleIncomingRequestL( 
                                        CSIPServerTransaction& aTransaction )
    {
    MUS_LOG( 
    "mus: [MUSAVA] -> CMusAvaCapabilitySipAgent::HandleIncomingRequestL" )
    RStringF phrase = 
                SIPStrings::StringF( SipStrConsts::EPhraseNotImplemented );
    CSIPResponseElements* response = 
        CSIPResponseElements::NewLC( KMUSAVASIPNotImplemented, phrase );
    aTransaction.SendResponseL( response );
    CleanupStack::Pop( response );
    MUS_LOG( 
    "mus: [MUSAVA] <- CMusAvaCapabilitySipAgent::HandleIncomingRequestL" )
    }

// --------------------------------------------------------------------------
// CMusAvaCapabilitySipAgent::IncomingResponse
// --------------------------------------------------------------------------
//
TInt CMusAvaCapabilitySipAgent::IncomingResponse( 
                                        CSIPClientTransaction& aTransaction )
    {
    MUS_LOG( 
        "mus: [MUSAVA] -> CMusAvaCapabilitySipAgent::IncomingResponse" )
    iCapabilityExchange.AnswerToQueryReceived( aTransaction );
    MUS_LOG( 
        "mus: [MUSAVA] <- CMusAvaCapabilitySipAgent::IncomingResponse" )
    return KErrNone;
    }
    
// --------------------------------------------------------------------------
// CMusAvaCapabilitySipAgent::ErrorOccured
// --------------------------------------------------------------------------
//
TInt CMusAvaCapabilitySipAgent::ErrorOccured( TInt aError,
                                        CSIPTransactionBase& aTransaction )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilitySipAgent:.ErrorOccured" )
    
    if ( aTransaction.IsSIPClientTransaction() )
        {
        MUS_LOG( "mus: [MUSAVA] Canceling query" )
        
        iCapabilityExchange.CancelQuery( 
                aError, 
                static_cast<CSIPClientTransaction&>( aTransaction ) );
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA] Server transaction, nothing to do" )
        }
        
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilitySipAgent:.ErrorOccured" )
    return KErrNone;
    }

