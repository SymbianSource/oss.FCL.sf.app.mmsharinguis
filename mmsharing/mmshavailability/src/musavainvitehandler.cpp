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




#include "musunittesting.h"
#include "musavainviteresponder.h"
#include "musavainvitehandler.h"
#include "musavasharedobject.h"
#include "muslogger.h"
#include "musavasip.h"
#include "musavasettingsimp.h"
#include "sipservertransaction.h"
#include <sipstrings.h>
#include <sipstrconsts.h>


CMusAvaInviteHandler* CMusAvaInviteHandler::NewL(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp&  aSettings )
    {
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::NewL()" )
    CMusAvaInviteHandler* self = CMusAvaInviteHandler::NewLC(
        aObserver,
        aSettings );
    CleanupStack::Pop( self );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::NewL()" )
    return self;
    }


CMusAvaInviteHandler* CMusAvaInviteHandler::NewLC(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    {
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::NewLC()" )
    CMusAvaInviteHandler* self = new( ELeave ) CMusAvaInviteHandler(
        aObserver,
        aSettings );

    CleanupStack::PushL( self );
    self->ConstructL();

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::NewLC()" )
    return self;
    }


CMusAvaInviteHandler::~CMusAvaInviteHandler()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteHandler::~CMusAvaInviteHandler()" )
    iResponders.ResetAndDestroy();
    if ( iSharedObj )
        {
        iSharedObj->DeleteSingleton();
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::~CMusAvaInviteHandler()" )
    }


CMusAvaInviteHandler::CMusAvaInviteHandler(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    :CMusAvaAvailability( aObserver ),
    iSettings( aSettings )
    {
    }


void CMusAvaInviteHandler::ConstructL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteHandler::ConstructL()" )
    iSharedObj = CMusAvaSharedObject::GetSingletonL();
    iSharedObj->MusAvaSip().AddAdapterL( ( MMusAvaSipConnectionAdapter& ) *this );
    iSharedObj->MusAvaSip().AddAdapterL( ( MMusAvaSipAdapter& ) *this );
    // set this only for static availability modules
    iState = MMusAvaObserver::EMusAvaStatusAvailable ; 
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::ConstructL()" )
    }


// -------------------------------------------------------------------------
//  A SIP request outside a dialog has been received from the network.
// -------------------------------------------------------------------------
//
TInt CMusAvaInviteHandler::IncomingRequest (CSIPServerTransaction*
                                            aTransaction)
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteHandler::IncomingRequest (CSIPServerTransaction* aTransaction)" )
    if( aTransaction->Type() == SIPStrings::StringF( SipStrConsts::EInvite ) )
        {
        TRAPD ( err, InvitationReceivedL( aTransaction ) )
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::IncomingRequest()" )
        if ( err != KErrNone )
            {
            delete aTransaction;
            }
        return KErrNone;
        }
    else
        {
        MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::IncomingRequest(KErrNotFound)" )
        return KErrNotFound;
        }
    }


// -------------------------------------------------------------------------
//  Starts the loopy execution.
// -------------------------------------------------------------------------
//
void CMusAvaInviteHandler::DoExecuteL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteHandler::ExecutePatternL()" )
    // set status available
    SetState( MMusAvaObserver::EMusAvaStatusAvailable );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::ExecutePatternL()" )
    }


// -------------------------------------------------------------------------
//  Stops the loop.
// -------------------------------------------------------------------------
//
void CMusAvaInviteHandler::Stop()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteHandler::StopPatternL()" )
    iResponders.ResetAndDestroy();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::StopPatternL()" )
    }


// -------------------------------------------------------------------------
//  CMusAvaInviteHandler::Name
// -------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityName CMusAvaInviteHandler::Name()
    {
    return MMusAvaObserver::EMusAvaInviteHandler;
    }

// -------------------------------------------------------------------------
//  This function is called when a SIP invite is received.
// -------------------------------------------------------------------------
//
void CMusAvaInviteHandler::InvitationReceivedL(
                            CSIPServerTransaction* aTransaction )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteHandler::InvitationReceivedL()" )

    // create a new responder instance
    CMusAvaInviteResponder* responder = CMusAvaInviteResponder::NewLC(
                                            aTransaction, *this );
    TRAPD( err,  responder->AnswerL() );
    if( err != KErrNone )
	  	{
	  	CleanupStack::PopAndDestroy( responder );
	    MUS_LOG1( "mus: [MUSAVA] InvitationReceivedL( aTransaction )\
	    err #%d", err )
    	}
    else
        {
        TRAP( err, iResponders.AppendL( responder ) );
        if ( err != KErrNone )
            {
            CleanupStack::PopAndDestroy( responder );
            }
        else
            {
            CleanupStack::Pop( responder );
            }
        InvitationAnsweredLD();
        }

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::InvitationReceivedL()" )
    }


// -------------------------------------------------------------------------
//  This function is called when SIP invite is properly responded.
// -------------------------------------------------------------------------
//
void CMusAvaInviteHandler::InvitationAnsweredLD()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteHandler::InvitationAnsweredLD()" )

    // look for that responder instance from the array
    if ( iResponders.Count() )
        {
        for ( TInt i = 0; i < iResponders.Count(); i++ )
            {
            CMusAvaInviteResponder* responder = ( iResponders )[ i ];
             if( responder->ResponseComplete() )
                {
                MUS_LOG( "mus: [MUSAVA]  Remove responder" )
                delete responder;
                iResponders.Remove( i );
                }
            }
        }

    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteHandler::InvitationAnsweredLD()" )
    }


// -------------------------------------------------------------------------
// CMusAvaInviteHandler::ApplicationState
// -------------------------------------------------------------------------
//
MMusAvaSettingsObserver::TApplicationState
    CMusAvaInviteHandler::ApplicationState()
    {
    return iSettings.ApplicationState();
    }


// -------------------------------------------------------------------------
// CMusAvaInviteHandler::IncomingRequest
// -------------------------------------------------------------------------
//
TInt CMusAvaInviteHandler::IncomingRequest(
    TUint32 /*aIapId*/,
    CSIPServerTransaction* aTransaction )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteHandler::IncomingRequest()" )
    TInt retval = IncomingRequest( aTransaction );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::IncomingRequest()" )
    return retval;
    }


// -------------------------------------------------------------------------
// CMusAvaInviteHandler::TimedOut
// -------------------------------------------------------------------------
//
TInt CMusAvaInviteHandler::TimedOut(CSIPServerTransaction& /*aTransaction*/ )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaInviteHandler::ConstructL()" )
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaInviteHandler::ConstructL()" )
    return KErrNotSupported;
    }
