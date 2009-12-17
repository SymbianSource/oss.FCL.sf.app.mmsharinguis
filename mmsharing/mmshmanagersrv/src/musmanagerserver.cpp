/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/


#include "musmanagerserver.h"
#include "musunittesting.h"
#include "muslogger.h"
#include "musmanageripccommon.h"
#include "musmanagerserverclosetimer.h"
#include "musmanagerservercore.h"
#include "musmanagerserverplatsecpolicy.h"
#include "musmanagerserversession.h"

// CONSTANTS
const TUint KServerCloseWaitTime = 500; // milliseconds

// -----------------------------------------------------------------------------
// CMusManagerServer::NewL
// -----------------------------------------------------------------------------
//
CMusManagerServer* CMusManagerServer::NewL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServer* CMusManagerServer::NewL()" );

    CMusManagerServer* self = CMusManagerServer::NewLC();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServer* CMusManagerServer::NewL()" );
    return self;
    }

// -----------------------------------------------------------------------------
// CMusManagerServer::NewLC
// -----------------------------------------------------------------------------
//
CMusManagerServer* CMusManagerServer::NewLC()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServer* CMusManagerServer::NewLC()" );

    CMusManagerServer* self = new( ELeave ) CMusManagerServer( EPriorityHigh );
    CleanupStack::PushL( self );
    self->ConstructL();
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServer* CMusManagerServer::NewLC()" );
    return self;
    }

// -----------------------------------------------------------------------------
// CMusManagerServer::CMusManagerServer
// -----------------------------------------------------------------------------
//
CMusManagerServer::CMusManagerServer ( TInt aPriority )
    : CPolicyServer( aPriority, KMusManagerServerPlatSecPolicy )
    {
    }

// -----------------------------------------------------------------------------
// CMusManagerServer::ConstructL
// -----------------------------------------------------------------------------
//
void CMusManagerServer::ConstructL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServer::ConstructL()" );

    iActiveScheduler = new( ELeave ) CActiveScheduler ();
    CActiveScheduler::Install( iActiveScheduler );

    StartL( KMusManagerServerName );

    iCloseTimer = CMusManagerServerCloseTimer::NewL();

    iServerCore = CMusManagerServerCore::NewL( *this );
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServer::ConstructL()" );
    }

// -----------------------------------------------------------------------------
// CMusManagerServer::~CMusManagerServer
// -----------------------------------------------------------------------------
//
CMusManagerServer::~CMusManagerServer()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServer::~CMusManagerServer()" );

    delete iCloseTimer;
	//iServerCore.StopMonitoring();
    delete iServerCore;

    delete iActiveScheduler;
    CActiveScheduler::Install( NULL );
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServer::~CMusManagerServer()" );
    }

// -----------------------------------------------------------------------------
// CMusManagerServer::IncrementSessions
// -----------------------------------------------------------------------------
//
void CMusManagerServer::StopServer()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServer::StopServer()" );
    iCloseTimer->StopActiveSchedulerAfter( KServerCloseWaitTime );
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServer::StopServer()" );
    }

// -----------------------------------------------------------------------------
// CMusManagerServer::IncrementSessions
// -----------------------------------------------------------------------------
//
void CMusManagerServer::IncrementSessions()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServer::IncrementSessions()" );

    iSessionCount++;

    if ( iCloseTimer->IsActive() )
        {
        iCloseTimer->Cancel();
        }
    MUS_LOG1( "mus: [MUSSRV]  <- CMusManagerServer::IncrementSessions( %d )",
              iSessionCount );
    }

// -----------------------------------------------------------------------------
// CMusManagerServer::DecrementSessions
// -----------------------------------------------------------------------------
//
void CMusManagerServer::DecrementSessions()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServer::DecrementSessions()" );

    iSessionCount--;

    if( iSessionCount == 0 )
        {
        iServerCore->EventNoSessions();
        }
    MUS_LOG1( "mus: [MUSSRV]  <- CMusManagerServer::DecrementSessions( %d )",
              iSessionCount );
    }

// -----------------------------------------------------------------------------
// CMusManagerServer::RunError
// -----------------------------------------------------------------------------
//
TInt CMusManagerServer::RunError(TInt aError)
    {
    MUS_LOG1( "mus: [MUSSRV]     -> CMusManagerServer::RunError( %d )",
               aError );

    if ( aError == KErrBadDescriptor )
        {
        // A bad descriptor error implies a badly programmed client,
        // so panic it; otherwise report the error to the client
        Message().Panic( KMusManagerServer(), EBadDescriptor );
        }
    else
        {
        Message().Complete( aError );
        }
    //
    // The leave will result in an early return from CServer::RunL(), skipping
    // the call to request another message. So do that now in order to keep the
    // server running.
    ReStart();

    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServer::RunError()" );
    return KErrNone; // handled the error fully
    }

// -----------------------------------------------------------------------------
// CMusManagerServer::NewSessionL
// -----------------------------------------------------------------------------
//
CSession2* CMusManagerServer::NewSessionL(
    const TVersion &aVersion,
    const RMessage2& /*aMessage*/ ) const
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServer::NewSessionL()" );

    // check we're the right version
    if ( !User::QueryVersionSupported( TVersion( KMusManagerServerMajorVersionNumber,
                                                 KMusManagerServerMinorVersionNumber,
                                                 KMusManagerServerBuildVersionNumber),
                                                 aVersion ) )
        {
        User::Leave( KErrNotSupported );
        }
    // make new session
    CMusManagerServerSession* session = CMusManagerServerSession::NewL(
                const_cast<CMusManagerServer&>( *this ),
                *iServerCore );
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServer::NewSessionL()" );
    return session;

    }

// -----------------------------------------------------------------------------
// CMusManagerServer::SessionCount
// -----------------------------------------------------------------------------
//
TUint CMusManagerServer::SessionCount()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServer::SessionCount()" );
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServer::SessionCount()" );
    return iSessionCount;
    }
