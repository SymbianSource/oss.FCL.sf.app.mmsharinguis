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
* Description:  Asynchronous callback service provider.
*
*/



#include "musuicallbackservice.h"
#include "musuicallbackobserver.h" // callback observer interface
#include "muslogger.h"


// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusUiCallbackService* CMusUiCallbackService::NewL(
                        MMusUiCallbackObserver& aCallbackObserver )
    {
    CMusUiCallbackService* self =
                            new ( ELeave ) CMusUiCallbackService(
                                                    aCallbackObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor.
// -----------------------------------------------------------------------------
//
CMusUiCallbackService::~CMusUiCallbackService()
    {
    // Cancel active object
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiCallbackService::~CMusUiCallbackService" );
    Cancel();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiCallbackService::~CMusUiCallbackService" );
    }


// -----------------------------------------------------------------------------
// C++ default constructor.
// -----------------------------------------------------------------------------
//
CMusUiCallbackService::CMusUiCallbackService(
                        MMusUiCallbackObserver& aCallbackObserver )
    : CActive( EPriorityStandard ),
    iCallbackObserver( aCallbackObserver ),
    iEventId( EMusUiAsyncUninitialized )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiCallbackService::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
// Buffer a request to trigger an event.
// -----------------------------------------------------------------------------
//
void CMusUiCallbackService::AsyncEventL( TMusUiAsyncEvent aEventId )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiCallbackService::AsyncEventL" );
    iEventId = aEventId;
    Activate();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiCallbackService::AsyncEventL" );
    }


// -----------------------------------------------------------------------------
// Cancels the active request.
// -----------------------------------------------------------------------------
//
void CMusUiCallbackService::Cancel()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiCallbackService::Cancel" );
    CActive::Cancel();
    iStatus = NULL;
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiCallbackService::Cancel" );
    }

// -----------------------------------------------------------------------------
// Check what event is currently pending.
// -----------------------------------------------------------------------------
//
TMusUiAsyncEvent CMusUiCallbackService::CurrentEvent() const
    {
    if ( IsActive() )
        {
        return iEventId;
        }
    return EMusUiAsyncUninitialized;
    }
    
// -----------------------------------------------------------------------------
// From CActive.  Called when a request completes.
// -----------------------------------------------------------------------------
//
void CMusUiCallbackService::RunL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiCallbackService::RunL" );
    if ( iEventId )
        {
        // Copy the id from member to local so the member can be cleared.
        TMusUiAsyncEvent eventId = iEventId;
        iEventId = EMusUiAsyncUninitialized;
        // Issue the call to the observer interface.
        iCallbackObserver.HandleAsyncEventL( eventId );
        MUS_LOG( "mus: [MUSUI ]  <- CMusUiCallbackService::RunL returns" );
        return;
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiCallbackService::RunL" );
    }


// -----------------------------------------------------------------------------
// From CActive.  Called when a leave occurres in RunL.
// -----------------------------------------------------------------------------
//
TInt CMusUiCallbackService::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSUI ]        CMusUiCallbackService::RunError [%d]",
                            aError );
    iCallbackObserver.HandleAsyncError( aError );
                            
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// From CActive.  Called when a request is cancelled.
// -----------------------------------------------------------------------------
//
void CMusUiCallbackService::DoCancel()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiCallbackService::DoCancel" );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiCallbackService::DoCancel" );
    }


// -----------------------------------------------------------------------------
// Activate the object.
// -----------------------------------------------------------------------------
//
void CMusUiCallbackService::Activate()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiCallbackService::Activate" );
    // Call SetActive() so RunL() will be called by the active scheduler
    if ( IsActive() )
        {
        Cancel();
        }
    SetPriority( EPriorityHigh );
    SetActive();
    iStatus = KRequestPending;
    TRequestStatus* stat = &iStatus;
    User::RequestComplete( stat, KErrNone );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiCallbackService::Activate" );
    }


