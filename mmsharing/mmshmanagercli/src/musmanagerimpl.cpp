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



#include "musmanagerimpl.h"
#include "musmanager.h"
#include "musmanageripccommon.h"
#include "musmanagerserverstarter.h"
#include "muslogger.h"
#include "musavailabilityobserver.h"

// ---------------------------------------------------------------------------
// CMusManagerImpl::CMusManagerImpl
// ---------------------------------------------------------------------------
//
CMusManagerImpl::CMusManagerImpl()
    : iConnected( EFalse )
    {
    }
    
    
// ---------------------------------------------------------------------------
// CMusManagerImpl::CMusManagerImpl
// ---------------------------------------------------------------------------
//
CMusManagerImpl::CMusManagerImpl( MMusAvailabilityObserver* aObserver )
    : iConnected( EFalse ), iAvailabilityObserver( aObserver )
    {
    }      


// ---------------------------------------------------------------------------
// CMusManagerImpl::ConstructL
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::ConstructL()
    {
    if ( iAvailabilityObserver )
        {
        iListener = CMusManagerImplListener::NewL( this ); 
        }    
    }


// ---------------------------------------------------------------------------
// CMusManagerImpl::NewL
// ---------------------------------------------------------------------------
//
CMusManagerImpl* CMusManagerImpl::NewL()
    {
    CMusManagerImpl* self = CMusManagerImpl::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CMusManagerImpl::NewLC
// ---------------------------------------------------------------------------
//
CMusManagerImpl* CMusManagerImpl::NewLC()
    {
    CMusManagerImpl* self = new( ELeave ) CMusManagerImpl;
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }
    
    
// ---------------------------------------------------------------------------
// CMusManagerImpl::NewL 
// ---------------------------------------------------------------------------
//
CMusManagerImpl* CMusManagerImpl::NewL( MMusAvailabilityObserver* aObserver )
    {
    CMusManagerImpl* self = CMusManagerImpl::NewLC( aObserver );
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// CMusManagerImpl::NewLC
// ---------------------------------------------------------------------------
//
CMusManagerImpl* CMusManagerImpl::NewLC( MMusAvailabilityObserver* aObserver )
    {
    CMusManagerImpl* self = new( ELeave ) CMusManagerImpl( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }      


// ---------------------------------------------------------------------------
// CMusManagerImpl::~CMusManagerImpl
// ---------------------------------------------------------------------------
//
CMusManagerImpl::~CMusManagerImpl()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::~CMusManagerImpl()" );
    delete iListener;
    iSession.Disconnect();
    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::~CMusManagerImpl()" );
    }


// ---------------------------------------------------------------------------
// CMusManagerImpl::~CMusManagerImpl
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::StartServer()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::StartServer()" );
    TInt error = MusManagerServerStarter::Start();
    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::StartServer()" );
    }

// ---------------------------------------------------------------------------
// CMusManagerImpl::~CMusManagerImpl
// ---------------------------------------------------------------------------
//
TBool CMusManagerImpl::ServerStarted()
    {
    return MusManagerServerStarter::Started();
    }

// ---------------------------------------------------------------------------
// CMusManagerImpl::~CMusManagerImpl
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::ExamineAvailabilityL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::ExamineAvailabilityL()" );
    // ensure server and connection are ok
    InitSessionL();

    iSession.InvestigateAvailabilityL();
    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::ExamineAvailabilityL()" );
    }

// ---------------------------------------------------------------------------
// Returns the current availability of MultimediaSharing.
// ---------------------------------------------------------------------------
//
MultimediaSharing::TMusAvailabilityStatus CMusManagerImpl::AvailabilityL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::AvailabilityL()" );

    MultimediaSharing::TMusAvailabilityStatus status;

    if( !ServerStarted() )
        {
        status = ( MultimediaSharing::TMusAvailabilityStatus ) KErrNotReady;
        }
    else
        {
        status = QueryAvailabilityL();
        }
    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::AvailabilityL()" );

    return status;
    }
    
// ---------------------------------------------------------------------------
// Forwards command to executed by MultimediaSharing.
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::HandleCommandL( 
								MultimediaSharing::TCommandType aCommandType )
	{
	MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::HandleCommandL()" );
	    // ensure server and connection are ok
    InitSessionL();
    
    iSession.HandleCommandL( aCommandType );
    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::HandleCommandL()" );	
	}
// ---------------------------------------------------------------------------
// Forwards a SIP request to be handled by MultimediaSharing.
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::HandleSipRequestL( TInt aChannelId )
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::HandleSipRequestL()" );
    // ensure server and connection are ok
    InitSessionL();

    iSession.HandleSipRequestL( aChannelId );
    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::HandleSipRequestL()" );
    }

// ---------------------------------------------------------------------------
// Starts the MultimediaSharing application.
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::StartApplicationL( MultimediaSharing::TMusUseCase aUseCase )
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::StartApplicationL()" );
    // ensure server and connection are ok
    InitSessionL();

    MultimediaSharing::TMusAvailabilityStatus status =
        iSession.QueryAvailabilityL();

    if( status < KErrNone || status >= MultimediaSharing::EErrSipRegistration )
        {
        MUS_LOG1( "mus: [MUSCLI]     <- CMusManagerImpl::StartApplicationL( leave: %d )",
                 status );
        User::Leave( status );
        }

    iSession.StartL( aUseCase );

    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::StartApplicationL()" );
    }

// ---------------------------------------------------------------------------
// Stops the MultimediaSharing application.
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::StopApplicationL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::StopApplicationL()" );
    // ensure server and connection are ok
    InitSessionL();

    iSession.StopL();
    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::StopApplicationL()" );
    }

// ---------------------------------------------------------------------------
// Starts investigating the current availability of MultimediaSharing.
// ---------------------------------------------------------------------------
//
MultimediaSharing::TMusAvailabilityStatus CMusManagerImpl::QueryAvailabilityL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::QueryAvailabilityL()" );
    // ensure server and connection are ok
    InitSessionL();

    MultimediaSharing::TMusAvailabilityStatus status
                            = iSession.QueryAvailabilityL();

    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::QueryAvailabilityL()" );
    return status;
    }

// ---------------------------------------------------------------------------
// Starts to monitor availability
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::MonitorAvailabilityL( TRequestStatus& aRequestStatus )
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::MonitorAvailabilityL()" );
    
    iPcArgs.Set( EMusManagerIpcArgAvailabilityStatus, &iPckg );
    iSession.MonitorAvailabilityL( aRequestStatus, iPcArgs );
 
    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::MonitorAvailabilityL()" );
    }


// ---------------------------------------------------------------------------
// Starts server process, if needed, and then opens a session to it.
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::InitSessionL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::InitSessionL()" );
    // if server not started, start it
    if( !ServerStarted() )
        {
        StartServer();
        }

    // if no session, connect
    if( !iConnected )
        {
        ConnectL();
        }
    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::InitSessionL()" );
    }


// ---------------------------------------------------------------------------
// Connects this client to the server process; opens a session.
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::ConnectL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::ConnectL()" );
    iSession.ConnectL();

    iConnected = ETrue;
    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::ConnectL()" );
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::CancelMonitoring()
    {
    MUS_LOG( "mus: [MUSCLI]  -> CMusManagerImpl::CancelMonitoring()" );
    TRAP_IGNORE( iSession.CancelMonitoringL() );
    MUS_LOG( "mus: [MUSCLI]  <- CMusManagerImpl::CancelMonitoring()" );
    }


// ---------------------------------------------------------------------------
// Creates listener
// ---------------------------------------------------------------------------
//    
CMusManagerImpl::CMusManagerImplListener* 
    CMusManagerImpl::CMusManagerImplListener::NewL( 
        CMusManagerImpl* aMusManagerImpl )
    {
    CMusManagerImplListener* self = 
        new ( ELeave ) CMusManagerImplListener( aMusManagerImpl );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    } 
  
// ---------------------------------------------------------------------------
// Constructs listener
// ---------------------------------------------------------------------------
//    
CMusManagerImpl::CMusManagerImplListener::CMusManagerImplListener( 
        CMusManagerImpl* aMusManagerImpl ) : 
        CActive(0), iMusManagerImpl( aMusManagerImpl )
    {
    CActiveScheduler::Add(this);
    }
    
// ---------------------------------------------------------------------------
// ConstructL
// ---------------------------------------------------------------------------
//    
void CMusManagerImpl::CMusManagerImplListener::ConstructL()
    {
    iMusManagerImpl->InitSessionL();
    iMusManagerImpl->iSession.StartObservingAvailabilityL();
    ListenL();
    }    
    
// ---------------------------------------------------------------------------
// Destructs listener
// ---------------------------------------------------------------------------
//
CMusManagerImpl::CMusManagerImplListener::~CMusManagerImplListener() 
    {
    Cancel();
    }

// ---------------------------------------------------------------------------
// Start listening availability events
// ---------------------------------------------------------------------------
//
void CMusManagerImpl::CMusManagerImplListener::ListenL()
    {
    iMusManagerImpl->MonitorAvailabilityL( iStatus );
    SetActive();
    }

// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//    
void CMusManagerImpl::CMusManagerImplListener::RunL()
    {
    iMusManagerImpl->iAvailabilityObserver->MusAvailabilityChangedL( 
                iMusManagerImpl->iPckg() );  
                
    // restart monitoring                  
    ListenL();
    }
    
// ---------------------------------------------------------------------------
// RunL
// ---------------------------------------------------------------------------
//        
TInt CMusManagerImpl::CMusManagerImplListener::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSCLI]     CMusManagerImplListener::RunError: %d", \
                 aError );
    return aError;
    }

// ---------------------------------------------------------------------------
// Cancel
// ---------------------------------------------------------------------------
//    
void CMusManagerImpl::CMusManagerImplListener::DoCancel()
    {
    iMusManagerImpl->CancelMonitoring();
    }    
