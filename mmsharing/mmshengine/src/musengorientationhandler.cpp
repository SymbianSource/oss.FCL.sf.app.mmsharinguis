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
* Description:
*
*/


// USER
#include "musengorientationhandler.h"
#include "muslogger.h"
#include "musenglivesession.h"
#include "musenglivesessionobserver.h"

// SYSTEM

const TInt KMusEngDelayedResume = 300000; // 300 ms

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngOrientationHandler* CMusEngOrientationHandler::NewL( 
    CMusEngLiveSession& aSession, MMusEngLiveSessionObserver& aSessionObserver )
    {
    CMusEngOrientationHandler* self = 
        new (ELeave) CMusEngOrientationHandler( aSession, aSessionObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self);
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngOrientationHandler::~CMusEngOrientationHandler()
    {
    MUS_LOG( "mus: [ENGINE]     CMusEngOrientationHandler::~CMusEngOrientationHandler()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngOrientationHandler::RefreshOrientationL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngOrientationHandler::RefreshOrientationL()" )
    if ( !IsActive() )
        {
        TBool refreshBeginStateCamera = iSession.IsPlayingL();
        TBool refreshBeginStateDisplay = iSession.IsDisplayEnabledL();
            
        if ( refreshBeginStateCamera )
             {
             MUS_LOG( "mus: [MUSENG]  -> Playing, pause/stop to restart camera" );
             iSession.PauseL();
             } 
         
        if ( refreshBeginStateDisplay )
             {
             MUS_LOG( "mus: [MUSENG]  -> display is enabled, disable/enable it");
             iSession.EnableDisplayL(EFalse);
             }
        iRefreshBeginStateCamera = refreshBeginStateCamera;
        iRefreshBeginStateDisplay = refreshBeginStateDisplay;
        }
    
    DoDelayedResumeL();
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngOrientationHandler::RefreshOrientationL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngOrientationHandler::UpdateL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngOrientationHandler::UpdateL()" )
    iRefreshBeginStateCamera = iSession.IsPlayingL();
    iRefreshBeginStateDisplay = iSession.IsDisplayEnabledL();
    MUS_LOG2( "mus: [ENGINE]  <- CMusEngOrientationHandler::UpdateL(), cam:%d disp:%d", 
        iRefreshBeginStateCamera, iRefreshBeginStateDisplay )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngOrientationHandler::RunL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngOrientationHandler::RunL()" )

    TBool refreshBeginStateCamera = iRefreshBeginStateCamera;
    TBool refreshBeginStateDisplay = iRefreshBeginStateDisplay;
       
    iSessionObserver.OrientationRefreshEnded();
    
    if ( refreshBeginStateDisplay )
        {
        MUS_LOG( "mus: [ENGINE] enable display after delayed orientation" )
        iSession.EnableDisplayL(ETrue);
        }
    
    if ( refreshBeginStateCamera )
        {
        MUS_LOG( "mus: [ENGINE] enable camera after delayed orientation" )
        iSession.PlayL();
        }
    MUS_LOG( "mus: [ENGINE]  <- CMusEngOrientationHandler::RunL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusEngOrientationHandler::RunError( TInt aError )
    {
    MUS_LOG( "mus: [ENGINE]  <-> CMusEngOrientationHandler::RunError()" )
    aError = KErrNone;
    return aError;
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngOrientationHandler::DoDelayedResumeL()
    {
    if ( IsActive() )
        {
        Cancel();
        }
    After( KMusEngDelayedResume );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngOrientationHandler::CMusEngOrientationHandler( 
    CMusEngLiveSession& aSession, MMusEngLiveSessionObserver& aSessionObserver )
    : CTimer( CActive::EPriorityStandard ), iSession( aSession ), 
      iSessionObserver( aSessionObserver )
    {
    CActiveScheduler::Add(this);
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngOrientationHandler::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngOrientationHandler::ConstructL()" )
        
    CTimer::ConstructL();
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngOrientationHandler::ConstructL()" )
    }

