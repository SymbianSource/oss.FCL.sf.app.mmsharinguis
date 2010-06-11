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


// USER
#include "musengtwowaysession.h"
#include "lcsessionobserver.h"
#include "musenglogger.h"
#include "musengremotevideoplayer.h"
#include "musenglivevideoplayer.h"
#include "svptimer.h"


const TInt KMusEngJitterBufferLength = 51; //Must be bigger than treshold
// Using following value increases treshold buffer to 1 second from 
// default 100 ms
const TInt KMusEngJitterBufferTreshold = 50;

const TUint32 KMusEngTwoWayReceivingActivityTimeout = 5000; // 5 seconds
// 6 seconds keepalive timer, needs to be more than receiving timeout
const TUint8 KMusEngTwoWayRtpKeepAliveTimer = 6; 

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTwoWaySession* CMusEngTwoWaySession::NewL()
    {
    CMusEngTwoWaySession* self = new( ELeave )CMusEngTwoWaySession();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTwoWaySession::~CMusEngTwoWaySession()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::~CMusEngTwoWaySession()" )
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::~CMusEngTwoWaySession()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::EnableDisplayL( TBool aEnable )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngTwoWaySession::EnableDisplayL() %d", 
              aEnable )
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::EnableDisplayL()")
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//        
void CMusEngTwoWaySession::SetOrientationL( TDisplayOrientation aOrientation )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngTwoWaySession::SetOrientationL() %d", 
              aOrientation )
                  

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::SetOrientationL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::SetRectsL( 
    const TRect& aRemoteRect,
    const TRect& aLocalRect )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::SetRectsL()" )  
         
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::SetRectsL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTwoWaySession::CMusEngTwoWaySession() 
    : CMusEngMceSession()
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::ConstructL()" )
    
    CMusEngMceSession::ConstructL();
    
    iRemoteVideoPlayer = CMusEngRemoteVideoPlayer::NewL( *this, *this ); 
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::ConstructL()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::EstablishLcSessionL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::EstablishLcSessionL" )

    iTimer = CSVPTimer::NewL( *this, 1 ); 
    iTimer->SetTime(2000);
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::EstablishLcSessionL" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::TerminateLcSessionL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::TerminateLcSessionL" )
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::TerminateLcSessionL" )
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngTwoWaySession::RemoteVideoPlayer()
    {
    return iRemoteVideoPlayer;
    }

// End of file
