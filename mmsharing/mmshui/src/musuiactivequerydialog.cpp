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


#include "musuiactivequerydialog.h"
#include "musuiqueryobserver.h"
#include "muslogger.h"
#include "musunittesting.h"

#include <AknGlobalConfirmationQuery.h>
#include <avkon.rsg>
#include <musui.rsg>
#include <StringLoader.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiActiveQueryDialog* CMusUiActiveQueryDialog::NewL(
                                MMusUiQueryObserver& aObserver )
    {
    CMusUiActiveQueryDialog* self
            = new( ELeave ) CMusUiActiveQueryDialog( aObserver );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
CMusUiActiveQueryDialog::~CMusUiActiveQueryDialog()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActiveQueryDialog::~CMusUiActiveQueryDialog" );
    
    Cancel();
    delete iGlobalQuery;
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActiveQueryDialog::~CMusUiActiveQueryDialog" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiActiveQueryDialog::CMusUiActiveQueryDialog( MMusUiQueryObserver& aObserver )
    : CActive( EPriorityNormal ), iObserver( aObserver )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiActiveQueryDialog::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActiveQueryDialog::ConstructL" );

    iGlobalQuery = CAknGlobalConfirmationQuery::NewL();
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActiveQueryDialog::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiActiveQueryDialog::ShowL( const TDesC& aPrompt )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActiveQueryDialog::ShowL" );

    iGlobalQuery->ShowConfirmationQueryL( 
                            iStatus,
                            aPrompt,
                            R_AVKON_SOFTKEYS_YES_NO );
    SetActive();    
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActiveQueryDialog::ShowL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiActiveQueryDialog::RunL()
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiActiveQueryDialog::RunL [%d]", iStatus.Int() );
    if ( iStatus == EAknSoftkeyYes )
        {
        MUS_LOG( "mus: [MUSUI ]     CMusUiActiveQueryDialog::RunL: if" );
        iObserver.QueryAcceptedL( ETrue );
        }
    else    // EAknSoftkeyNo
        {
        MUS_LOG( "mus: [MUSUI ]     CMusUiActiveQueryDialog::RunL: else" );
        iObserver.QueryAcceptedL( EFalse );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActiveQueryDialog::RunL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiActiveQueryDialog::DoCancel()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActiveQueryDialog::DoCancel" );
    iGlobalQuery->CancelConfirmationQuery();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActiveQueryDialog::DoCancel" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusUiActiveQueryDialog::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiActiveQueryDialog::RunError [%d]", 
              aError );
              
    return iObserver.HandleQueryError( aError );
    }


// end of file
