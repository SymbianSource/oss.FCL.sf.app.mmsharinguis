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
* Description:   Capability Exchange
*
*/


#include "musavacapabilityexchange.h"
#include <uri8.h>
#include <sipservertransaction.h>
#include <sipclienttransaction.h>
#include <sipresponseelements.h>
#include <sipextensionheader.h>
#include <sipstrings.h>
#include <sipstrconsts.h>


#include "muslogger.h"
#include "musavaterminal.h"
#include "musavacapability.h"
#include "musavacapabilitycontext.h"

// --------------------------------------------------------------------------
// Symbian two-phase constructor
// --------------------------------------------------------------------------
//
CMusAvaCapabilityExchange* CMusAvaCapabilityExchange::NewL( 
                            const TDesC8& aTerminalId,
                            MMusAvaCapabilityQueryObserver& aQueryObserver )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityExchange::NewL" )
    CMusAvaCapabilityExchange* self = 
        new (ELeave) CMusAvaCapabilityExchange( aQueryObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aTerminalId );
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityExchange::NewL" )
    return self;
    }


// --------------------------------------------------------------------------
// C++ destructor
// --------------------------------------------------------------------------
//
CMusAvaCapabilityExchange::~CMusAvaCapabilityExchange()
    {
    MUS_LOG( 
    "mus: [MUSAVA] -> CMusAvaCapabilityExchange::~CMusAvaCapabilityExchange" )

    delete iTerminalId;
    iTerminals.ResetAndDestroy();
    iCapabilities.ResetAndDestroy();
    
    MUS_LOG( 
    "mus: [MUSAVA] <- CMusAvaCapabilityExchange::~CMusAvaCapabilityExchange" )
    }


// --------------------------------------------------------------------------
// C++ constructor
// --------------------------------------------------------------------------
//
CMusAvaCapabilityExchange::CMusAvaCapabilityExchange( 
    MMusAvaCapabilityQueryObserver& aQueryObserver )
    : iQueryObserver( aQueryObserver )
    {
    }


// --------------------------------------------------------------------------
// Symbian second-phase constructor
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityExchange::ConstructL( const TDesC8& aTerminalId )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityExchange::ConstructL" )

    iTerminalId = aTerminalId.AllocL();

    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityExchange::ConstructL" )
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::QueryObserver
// --------------------------------------------------------------------------
//
MMusAvaCapabilityQueryObserver& 
                            CMusAvaCapabilityExchange::QueryObserver() const
    {
    return iQueryObserver;
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::TerminalId
// --------------------------------------------------------------------------
//
const TDesC8& CMusAvaCapabilityExchange::TerminalId() const
    {
    return *iTerminalId;
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::AnswerToQueryReceived
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityExchange::AnswerToQueryReceived( 
                                    const CSIPClientTransaction& aResponse )
    {
    MUS_LOG( 
    "mus: [MUSAVA] -> CMusAvaCapabilityExchange::AnswerToQueryReceived" )
    
    TInt error = KErrNone;
    CMusAvaTerminal* terminal = NULL;
    TInt index = 0;
    
    while( !terminal && index < iTerminals.Count() )
        {
        terminal = iTerminals[ index++ ];
        TRAP( error, terminal = terminal->QueryCompletedL( aResponse ) ? 
                     terminal : NULL );
        terminal = error != KErrNone ? NULL : terminal;
        }
    
    MUS_LOG( 
    "mus: [MUSAVA] <- CMusAvaCapabilityExchange::AnswerToQueryReceived" )
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::QueryReceived
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityExchange::QueryReceived( 
                                            CSIPServerTransaction* aQuery )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityExchange::QueryReceived" )
    
    TRAPD( error, DoQueryReceivedL( *aQuery ) );

    if ( error != KErrNone )
        {
        TRAP_IGNORE( RejectQueryL( *aQuery, error ) )
        }
        
    delete aQuery;
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityExchange::QueryReceived" )
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::CancelQuery
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityExchange::CancelQuery( TInt /*aError*/, 
                                const CSIPClientTransaction& aTransaction )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityExchange::CancelQuery" )

    CMusAvaTerminal* terminal = NULL;
    TInt index = 0;
    
    while( !terminal && index < iTerminals.Count() )
        {
        terminal = iTerminals[ index++ ];
        terminal = terminal->QueryCanceled( aTransaction ) ? 
            terminal : NULL;
        }
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityExchange::CancelQuery" )
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::AddCapabilityL
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityExchange::AddCapabilityL( 
                                        CMusAvaCapability* aCapability )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityExchange::AddCapabilityL" )
    
    User::LeaveIfError( iCapabilities.Find( aCapability ) < KErrNone ?
                        KErrNone : KErrArgument );

    User::LeaveIfError( FindByFeature( *aCapability ) < KErrNone ?
                        KErrNone : KErrArgument );
                        
    iCapabilities.AppendL( aCapability );
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityExchange::AddCapabilityL" )
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::Terminals
// --------------------------------------------------------------------------
//
const RPointerArray<CMusAvaTerminal>& CMusAvaCapabilityExchange::Terminals()
    {
    return iTerminals;
    }

    
// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::Capabilities
// --------------------------------------------------------------------------
//
const RPointerArray<CMusAvaCapability>& 
                                    CMusAvaCapabilityExchange::Capabilities()
    {
    return iCapabilities;
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::TerminalL
// --------------------------------------------------------------------------
//
CMusAvaTerminal& CMusAvaCapabilityExchange::TerminalL( 
                                                const TDesC8& aRemoteUri )
    {
    CMusAvaTerminal* terminal = NULL;
    TInt index = 0;

    while ( !terminal && index < Terminals().Count() )
        {
        terminal = Terminals()[ index++ ];
        terminal = terminal->MatchL( aRemoteUri ) ? terminal : NULL;
        }
    
    if ( !terminal )
        {
        terminal = CMusAvaTerminal::NewL( *this, aRemoteUri, KNullDesC8 );
        CleanupStack::PushL( terminal );
        iTerminals.AppendL( terminal );
        CleanupStack::Pop( terminal );
        }

    return *terminal;
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::TerminalL
// --------------------------------------------------------------------------
//
CMusAvaTerminal& CMusAvaCapabilityExchange::TerminalL( 
                                        const CSIPServerTransaction& aQuery )
    {
    CMusAvaTerminal* terminal = NULL;
    
    const CSIPExtensionHeader* userAgent = 
                        MusAvaCapabilityContext::UserAgentHeaderL( aQuery );
    const CUri8& originator = MusAvaCapabilityContext::OriginatorL( aQuery );
    
    const TDesC8& terminalUri = originator.Uri().UriDes();
    const TDesC8& terminalId = userAgent ? userAgent->Value() : KNullDesC8;
    
    TInt index = 0;
    while( !terminal && index < Terminals().Count() )
        {
        terminal = Terminals()[ index++ ];
        terminal = terminal->MatchL( terminalUri, terminalId )
                   ? terminal : NULL;
        }
    
    if ( !terminal )
        {
        terminal = CMusAvaTerminal::NewL( *this, terminalUri, terminalId );
        CleanupStack::PushL( terminal );
        iTerminals.AppendL( terminal );
        CleanupStack::Pop( terminal );
        }
        
    return *terminal;
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::DoQueryReceivedL
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityExchange::DoQueryReceivedL( 
                                            CSIPServerTransaction& aQuery )
    {
    MUS_LOG( 
        "mus: [MUSAVA] -> CMusAvaCapabilityExchange::DoQueryReceivedL" )
    
    CMusAvaTerminal& terminal = TerminalL( aQuery );
    
    terminal.QueryRequestedL( aQuery );
    
    MUS_LOG( 
        "mus: [MUSAVA] <- CMusAvaCapabilityExchange::DoQueryReceivedL" )
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::RejectQueryL
// --------------------------------------------------------------------------
//
void CMusAvaCapabilityExchange::RejectQueryL( CSIPServerTransaction& aQuery,
                                            TInt aError )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaCapabilityExchange::RejectQueryL" )
    TInt statusCode = aError < 0 ? KMUSAVASIPServerInternalError : aError;
    RStringF reasonPhrase = 
            statusCode == KMUSAVASIPServerInternalError ?
            SIPStrings::StringF( SipStrConsts::EPhraseServerInternalError ):
            SIPStrings::StringF( SipStrConsts::EEmpty);

    CSIPResponseElements* response = 
        CSIPResponseElements::NewLC( statusCode, reasonPhrase );
        
    aQuery.SendResponseL( response );
    
    CleanupStack::Pop( response );
    
    MUS_LOG( "mus: [MUSAVA] <- CMusAvaCapabilityExchange::RejectQueryL" )
    }


// --------------------------------------------------------------------------
// CMusAvaCapabilityExchange::FindByFeature
// --------------------------------------------------------------------------
//
TInt CMusAvaCapabilityExchange::FindByFeature( 
                                        CMusAvaCapability& aCapability )
    {
    TInt index = KErrNotFound;
    CMusAvaCapability* capability = NULL;
    
    for ( TInt i=0; i < iCapabilities.Count() && index == KErrNotFound; i++ )
        {
        capability = iCapabilities[i];
        if ( capability->Feature() == aCapability.Feature() )
            {
            index = i;
            }
        }
    return index;
    }
