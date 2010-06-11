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
#include "musenglivesession.h"
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
CMusEngLiveSession* CMusEngLiveSession::NewL()
    {
    CMusEngLiveSession* self = new( ELeave )CMusEngLiveSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession::~CMusEngLiveSession()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::~CMusEngLiveSession()" )
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::~CMusEngLiveSession()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::EnableDisplayL( TBool aEnable )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngLiveSession::EnableDisplayL() %d", 
              aEnable )
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::EnableDisplayL()")
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//        
void CMusEngLiveSession::SetOrientationL( TDisplayOrientation aOrientation )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngLiveSession::SetOrientationL() %d", 
              aOrientation )
                  

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::SetOrientationL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::SetRectsL( 
    const TRect& aRemoteRect,
    const TRect& aLocalRect )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::SetRectsL()" )  
         
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::SetRectsL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession::CMusEngLiveSession() 
    : CMusEngMceSession()
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::ConstructL()" )
    
    CMusEngMceSession::ConstructL();
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::ConstructL()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::EstablishLcSessionL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::EstablishLcSessionL" )

    iTimer = CSVPTimer::NewL( *this, 1 ); 
    iTimer->SetTime(2000);
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::EstablishLcSessionL" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::TerminateLcSessionL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::TerminateLcSessionL" )
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::TerminateLcSessionL" )
    }


// End of file
