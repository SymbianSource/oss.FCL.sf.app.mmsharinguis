/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The MUS application's UI class.
*
*/


#include "musuiinvitationquery.h"
#include "musuiinvitationqueryobserver.h"
#include "musuidefinitions.h"
#include "musuiactivetimer.h"
#include "muslogger.h"
#include "musunittesting.h"

#include <AknGlobalConfirmationQuery.h>
#include <avkon.rsg>
#include <musui.rsg>
#include <StringLoader.h>


// -----------------------------------------------------------------------------
// Two phase constructor
// -----------------------------------------------------------------------------
//
CMusUiInvitationQuery* CMusUiInvitationQuery::NewL(
                                MMusUiInvitationQueryObserver& aView )
    {
    CMusUiInvitationQuery* self
            = new( ELeave ) CMusUiInvitationQuery( aView );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMusUiInvitationQuery::~CMusUiInvitationQuery()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiInvitationQuery::~CMusUiInvitationQuery" );

    delete iGlobalQuery;
    delete iPrompt;
    
    delete iActiveTimer;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiInvitationQuery::~CMusUiInvitationQuery" );
    }


// -----------------------------------------------------------------------------
// Constructor
// -----------------------------------------------------------------------------
//
CMusUiInvitationQuery::CMusUiInvitationQuery( MMusUiInvitationQueryObserver& aView )
    : CActive( EPriorityNormal ), iObserver( aView )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// Second phase constructor
// -----------------------------------------------------------------------------
//
void CMusUiInvitationQuery::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiInvitationQuery::ConstructL" );

    iGlobalQuery = CAknGlobalConfirmationQuery::NewL();
    iPrompt = NULL;
    
    iActiveTimer = CMusUiActiveTimer::NewL( this );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiInvitationQuery::ConstructL" );
    }


// -----------------------------------------------------------------------------
// Shows the confirmation query dialog
// -----------------------------------------------------------------------------
//
void CMusUiInvitationQuery::ShowL( const TDesC& aContact )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiInvitationQuery::ShowL" );
    if ( iPrompt )
        {
        delete iPrompt;
        iPrompt = NULL;
        }

    iPrompt = StringLoader::LoadL(
                            R_MUS_RECEIVE_VIEW_NOTE_INCOMING_VIDEO_REQUEST,
                            aContact );
	if ( iActiveTimer->IsActive() )
	    {
	    // If Timer is active already , dont know what to do
	    // It might mean a invitation ongoing.
	    }
	else
	    {
		iActiveTimer->After( KMusInviteExpireTimeout * KMusOneSecond );
        iGlobalQuery->ShowConfirmationQueryL( 
                                iStatus,
                                *iPrompt,
                                R_AVKON_SOFTKEYS_YES_NO );
        SetActive();    
	    }
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiInvitationQuery::ShowL" );
    }


// -----------------------------------------------------------------------------
// Timer Expiry event.
// -----------------------------------------------------------------------------
//
void CMusUiInvitationQuery::TimerComplete( CMusUiActiveTimer* /*aTimer*/ )
    {
    Cancel();
    TRAP_IGNORE( iObserver.InvitationRejectedL() );
    }


// -----------------------------------------------------------------------------
// CActive RunL implementation.
// -----------------------------------------------------------------------------
//
void CMusUiInvitationQuery::RunL()
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiInvitationQuery::RunL [%d]", iStatus.Int() );
    iActiveTimer->Cancel();
    if ( iStatus == EAknSoftkeyYes || iStatus == EEikBidOk )
        {
        MUS_LOG( "mus: [MUSUI ]     CMusUiInvitationQuery::RunL: if" );
        iObserver.InvitationAcceptedL();
        }
    else    // EAknSoftkeyNo
        {
        MUS_LOG( "mus: [MUSUI ]     CMusUiInvitationQuery::RunL: else" );
        iObserver.InvitationRejectedL();
        }
    Cancel();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiInvitationQuery::RunL" );
    }


// -----------------------------------------------------------------------------
// Cancel() will call this
// -----------------------------------------------------------------------------
//
void CMusUiInvitationQuery::DoCancel()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiInvitationQuery::DoCancel" );
    iGlobalQuery->CancelConfirmationQuery();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiInvitationQuery::DoCancel" );
    }


// -----------------------------------------------------------------------------
// If RunL() leaves should be handled here.
// -----------------------------------------------------------------------------
//
TInt CMusUiInvitationQuery::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiInvitationQuery::RunError [%d]", aError );

    // Handle the error fully here. No cleanup needed in this case.
    // default active scheduler will panic by calling Error() if 
    // it not handled here. so return KErrNone.
    aError = KErrNone;

    return aError;
    }


// end of file
