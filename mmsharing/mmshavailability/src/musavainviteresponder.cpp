/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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



#include "musavainviteresponder.h"
#include "musavainviteresponderobserver.h"
#include "muslogger.h"

#include <e32cmn.h>
#include <sipresponseelements.h>
#include <sipmessageelements.h>
#include <sipstrings.h>
#include <siptransactionbase.h>
#include <sipheaderbase.h>
#include <sipservertransaction.h>

const TUint KMUSAVA486 = 486; // Busy
const TUint KMUSAVA603 = 603; // Decline/Reject

_LIT8( KMusAvaStatusBusy, "Busy Here" );
_LIT8( KMusAvaStatusDecline, "Decline" );


// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaInviteResponder* CMusAvaInviteResponder::NewL(
                                CSIPServerTransaction* aTransaction,
                                MMusAvaInviteResponderObserver& aObserver )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteResponder::NewL()" )

    CMusAvaInviteResponder* self = CMusAvaInviteResponder::NewLC(
                                                aTransaction,
                                                aObserver );
    CleanupStack::Pop( self );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteResponder::NewL()" )
    return self;
    }


// -------------------------------------------------------------------------
//  Destructor.
// -------------------------------------------------------------------------
//
CMusAvaInviteResponder::~CMusAvaInviteResponder()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteResponder::~CMusAvaInviteResponder()" )
    delete iTransaction;
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteResponder::~CMusAvaInviteResponder()" )
    }

// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaInviteResponder* CMusAvaInviteResponder::NewLC(
                                CSIPServerTransaction* aTransaction,
                                MMusAvaInviteResponderObserver& aObserver )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteResponder::NewLC()" )
    CMusAvaInviteResponder* self = new( ELeave ) CMusAvaInviteResponder(
                                                        aTransaction,
                                                        aObserver );
    CleanupStack::PushL( self );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteResponder::NewLC()" )
    return self;
    }

// -------------------------------------------------------------------------
//  Constructor.
// -------------------------------------------------------------------------
//
CMusAvaInviteResponder::CMusAvaInviteResponder(
                                CSIPServerTransaction* aTransAction,
                                MMusAvaInviteResponderObserver& aObserver )
    : iObserver( aObserver ), iTransaction( aTransAction )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteResponder::CMusAvaInviteResponder()" )
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteResponder::CMusAvaInviteResponder()" )
    }

// -------------------------------------------------------------------------
//
// -------------------------------------------------------------------------
//
void CMusAvaInviteResponder::AnswerL()
    {
    if( iObserver.ApplicationState()  ==
        MMusAvaSettingsObserver::EApplicationIsRunning )
        {
        SendResponseL( KMUSAVA486, KMusAvaStatusBusy );
        }
    else
        {
        SendResponseL( KMUSAVA603, KMusAvaStatusDecline );
        }
    }

// ------------------------------------------------------------------------
// Send response to request.
// ------------------------------------------------------------------------
void CMusAvaInviteResponder::SendResponseL( TUint aStatusCode,
                                            const TDesC8& aReasonPhrase )
    {
    MUS_LOG( "mus: [MUSAVA] -> CMusAvaInviteResponder::SendResponseL()" )
    MUS_LOG1( "mus: [MUSAVA]     aStatusCode == <%d>", aStatusCode )

    RStringF responseString = SIPStrings::Pool().OpenFStringL( aReasonPhrase );
    CleanupClosePushL( responseString );
    CSIPResponseElements* respElem =
        CSIPResponseElements::NewL( aStatusCode, responseString );
    CleanupStack::PopAndDestroy(); //responseString
    CleanupStack::PushL( respElem );

    ResponseL( respElem );
    CleanupStack::Pop( respElem );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteResponder::SendResponseL()" )
    }


// ------------------------------------------------------------------------
// Returns response status.
// ------------------------------------------------------------------------
TBool CMusAvaInviteResponder::ResponseComplete()
    {
   
	MUS_LOG( "mus: [MUSAVA]	-> CMusAvaInviteResponder::ResponseComplete()" )
	TBool res = EFalse;

	CSIPTransactionBase::TState state = CSIPTransactionBase::ETerminated;
	TRAPD( err, state = iTransaction->StateL() );
	
	if( err != KErrNone ) 
		{
		MUS_LOG1( "mus: [MUSAVA]	   serverTran.StateL, err #%d", err )
		return res;
		}

	if ( state == CSIPTransactionBase::ECompleted || 
		 state == CSIPTransactionBase::EConfirmed || 
		 state == CSIPTransactionBase::ETerminated && 
	     iTransaction->ResponseAllowed() )	
	    {
	    res = ETrue;
	    }

	MUS_LOG( "mus: [MUSAVA]	<- CMusAvaInviteResponder::ResponseComplete()" )
	return res;
    }

// ------------------------------------------------------------------------
// Send Response to reques.
// ------------------------------------------------------------------------
void CMusAvaInviteResponder::ResponseL(
                                CSIPResponseElements* aResponseElements )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteResponder::ResponseL" )
    CSIPServerTransaction& serverTran =
        static_cast<CSIPServerTransaction&>( *iTransaction );
    
    if ( iTransaction->IsSIPClientTransaction() )
        {
        User::Leave( KErrNotReady );
        }
    
    else if ( ( serverTran.StateL() == CSIPTransactionBase::ETrying ||
         serverTran.StateL() == CSIPTransactionBase::EProceeding ) &&
         serverTran.ResponseAllowed() )
        {
        MUS_LOG( "   Send response" )
        serverTran.SendResponseL( aResponseElements );
        }
    else
        {
        User::Leave( KErrNotReady );
        MUS_LOG( "mus: [MUSAVA]  Not allow to sent response " )
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteResponder::ResponseL" )
        }
    }


