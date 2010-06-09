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


// USER INCLUDES 
#include "musengsessiondurationtimer.h"
#include "musengsessiondurationtimerobserver.h"



// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusEngSessionDurationTimer* CMusEngSessionDurationTimer::NewL(  
						MMusEngSessionDurationTimerObserver& aObserver )
    {
    CMusEngSessionDurationTimer* self = 
    		new (ELeave) CMusEngSessionDurationTimer( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self) ;
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusEngSessionDurationTimer::~CMusEngSessionDurationTimer()
    {
    Cancel();
    iTimer.Close();
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngSessionDurationTimer::Start(TInt aDelay )
    {    
    if ( !IsActive() )
        {
        iTimer.After( iStatus, aDelay );
        SetActive();    	  	   	    
        }
    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusEngSessionDurationTimer::CMusEngSessionDurationTimer( 
						MMusEngSessionDurationTimerObserver& aObserver ) 
	: CActive( CActive::EPriorityStandard ), 
	  iObserver( aObserver)
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngSessionDurationTimer::ConstructL()
    {
    CActiveScheduler::Add( this );
    User::LeaveIfError( iTimer.CreateLocal() );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngSessionDurationTimer::RunL()
    {
    if ( iStatus.Int() == KErrNone )
        {
		iObserver.UpdateTimerEvent();
        }
    }

// -------------------------------------------------------------------------
//  If RunL() leaves,It should be handled here.
// -------------------------------------------------------------------------
//
TInt CMusEngSessionDurationTimer::RunError( TInt aError )
    {    
    // Nothing can be done here.
    aError = KErrNone;

    return aError;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngSessionDurationTimer::DoCancel()
    {
    iTimer.Cancel();
    }


