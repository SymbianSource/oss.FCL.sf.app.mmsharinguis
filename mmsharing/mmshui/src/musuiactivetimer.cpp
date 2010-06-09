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



#include "musuiactivetimer.h" 
#include "musuiactivetimerobserver.h"

#include "muslogger.h" // debug logging


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
// 
CMusUiActiveTimer* CMusUiActiveTimer::NewL(  MMusUiActiveTimerObserver* aObserver )
    {
    CMusUiActiveTimer* self = new (ELeave) CMusUiActiveTimer( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
// 
void CMusUiActiveTimer::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActiveTimer::ConstructL" );
    
    User::LeaveIfError( iTimer.CreateLocal() );
    CActiveScheduler::Add( this );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActiveTimer::ConstructL" );
    }


// -----------------------------------------------------------------------------
// .
// -----------------------------------------------------------------------------
// 
void CMusUiActiveTimer::After( TTimeIntervalMicroSeconds32 anInterval )
    {
    // No logging due to huge amount of calls in some use cases
    
    Cancel();
    iTimer.After( iStatus, anInterval );
    SetActive();
    
    // No logging due to huge amount of calls in some use cases
    }


// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
// 
CMusUiActiveTimer::CMusUiActiveTimer( MMusUiActiveTimerObserver* aObserver ) : 
    CActive( EPriorityNormal ),
    iObserver( aObserver )
    {
    // nothing
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
// 
CMusUiActiveTimer::~CMusUiActiveTimer()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActiveTimer::~CMusUiActiveTimer" );
    
    Cancel();
    iTimer.Close();
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActiveTimer::~CMusUiActiveTimer" );
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
void CMusUiActiveTimer::DoCancel()
    {
    // No logging due to huge amount of calls in some use cases
    
    iTimer.Cancel();
    
    // No logging due to huge amount of calls in some use cases
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
// 
void CMusUiActiveTimer::RunL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiActiveTimer::RunL" );
    if (iObserver && iStatus == KErrNone)
        {
        iObserver->TimerComplete( this );
        }
    else
        {
        User::LeaveIfError(iStatus.Int());
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActiveTimer::RunL" );
    }

// -------------------------------------------------------------------------
//  If RunL() leaves,It should be handled here.
// -------------------------------------------------------------------------
//
TInt CMusUiActiveTimer::RunError( TInt aError )
    {
	MUS_LOG( "mus: [MUSUI ]  -> CMusUiActiveTimer::RunError" );
    // Nothing can be done here.
    aError = KErrNone;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiActiveTimer::RunError" );
    return aError;
    }


// end of file
