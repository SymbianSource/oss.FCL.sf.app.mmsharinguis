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
* Description: 
*
*/



#include "musuiactivitymanger.h"
#include "muslogger.h" // debug logging


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiActivityManager* CMusUiActivityManager::NewL( TInt aTimeout )

    {
    CMusUiActivityManager* self = 
                                 new (ELeave) CMusUiActivityManager( aTimeout );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiActivityManager::~CMusUiActivityManager()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActivityManager::~CMusUiActivityManager" );
    Cancel();
    iTimer.Close();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActivityManager::~CMusUiActivityManager" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiActivityManager::CMusUiActivityManager( TInt aTimeout )
                : CActive( CActive::EPriorityHigh ),
                  iTimeout( aTimeout )

    {
    CActiveScheduler::Add(this);
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiActivityManager::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActivityManager::ConstructL" );
    iTimer.CreateLocal();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActivityManager::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiActivityManager::SetTimeout( TInt aTimeout )
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiActivityManager::SetTimeout [%d]", aTimeout );
    iTimeout = aTimeout;
    Reset();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActivityManager::SetTimeout" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiActivityManager::Start()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActivityManager::Start" );
    if (!IsActive())
        {
        iTimer.Inactivity( iStatus, iTimeout );
        SetActive();
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActivityManager::Start" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiActivityManager::Reset()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActivityManager::Reset" );
    Cancel();
    Start();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActivityManager::Reset" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiActivityManager::DoCancel()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActivityManager::DoCancel" );
    iTimer.Cancel();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActivityManager::DoCancel" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiActivityManager::RunL()
    {

    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActivityManager::RunL" );
    
    if ( iStatus == KErrNone )
        {
        
        TInt inactivity = User::InactivityTime().Int();

        if ( inactivity >= iTimeout )
            {
            User::ResetInactivityTime();
            Reset();  
            }
        else
            {
            iTimer.Inactivity( iStatus, iTimeout );
            }
               
        if ( !IsActive() )
            {
            SetActive();
            }

        }   

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActivityManager::RunL" );
    }

// -------------------------------------------------------------------------
//  If RunL() leaves,It should be handled here.
// -------------------------------------------------------------------------
//
TInt CMusUiActivityManager::RunError( TInt aError )
    {
	MUS_LOG( "mus: [MUSUI ]  -> CMusUiActivityManager::RunError" );
    // Nothing can be done here.
    aError = KErrNone;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActivityManager::RunError" );
    return aError;
    }


// end of file
