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


#include <e32base.h>

#include "musmanagerclientsession.h"
#include "musmanageripccommon.h"
#include "musmanagercommon.h"
#include "muslogger.h"
#include "musmanager.h"

/* Number of outstanding request can be issued by the client 
   For Mush 8 is normal and if needed it has to be increased 
   Or decreased . If request exceeds this number then that particular
   Request will be considered as part of the global pool of request
*/
#define ASYNCHRONOUS_MESSAGE_SLOTS 255

const TInt KMusServerSessionCreationWaitTimeoutInMicrosecs = 10000000; // 10 secs

// ======== LOCAL FUNCTIONS ========
      
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
RMusManagerClientSession::RMusManagerClientSession()
    : RSessionBase ()
    {
    }


// ---------------------------------------------------------------------------
// Use timeout mechanism for session creation as otherwise it can block
// so long that WServ terminates the process. There's no way to cancel
// session creation other than destuction of client session. Therefore, in case
// of timeout, leave and let creation finish silently or get canceled when
// user of client session object deletes us.
// ---------------------------------------------------------------------------
//
void RMusManagerClientSession::ConnectL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::ConnectL(), with timeout" );

    if ( iSessionCreationStatus.Int() == KRequestPending )
        {
        // Previous creation still pending
        User::Leave( KErrNotReady );
        }
    
    RTimer timer;
    CleanupClosePushL( timer );
    User::LeaveIfError( timer.CreateLocal() );
    
    User::LeaveIfError( CreateSession( KMusManagerServerName(), Version(),
                            ASYNCHRONOUS_MESSAGE_SLOTS, &iSessionCreationStatus) );
    TRequestStatus timeoutStatus;
    timer.After( timeoutStatus, KMusServerSessionCreationWaitTimeoutInMicrosecs );
    User::WaitForRequest( iSessionCreationStatus, timeoutStatus );
    if ( timeoutStatus.Int() != KRequestPending )
        {
        MUS_LOG( "mus: [MUSCLI]  RMusManagerClientSession::ConnectL, timeout" );
        User::Leave( KErrTimedOut );
        } 
    else
        {
        MUS_LOG( "mus: [MUSCLI]  RMusManagerClientSession::ConnectL, no timeout" );
        timer.Cancel();
        User::WaitForRequest( timeoutStatus );
        }
    CleanupStack::PopAndDestroy( &timer );
    
    MUS_LOG( "mus: [MUSCLI]  <- RMusManagerClientSession::ConnectL()" );
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void RMusManagerClientSession::Disconnect()
    {
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::Disconnect()" );
    RHandleBase::Close();
    MUS_LOG( "mus: [MUSCLI]  <- RMusManagerClientSession::Disconnect()" );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
TVersion RMusManagerClientSession::Version(void) const
    {
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::Version()" );
    MUS_LOG( "mus: [MUSCLI]  <- RMusManagerClientSession::Version()" );
    return TVersion( KMusManagerServerMajorVersionNumber,
                     KMusManagerServerMinorVersionNumber,
                     KMusManagerServerBuildVersionNumber);
    }


// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void RMusManagerClientSession::InvestigateAvailabilityL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::InvestigateAvailabilityL()" );
    // construct an empty argument
    TIpcArgs ipcArgs( TIpcArgs::ENothing );

    // send
    User::LeaveIfError( SendReceive( EMusManagerIpcInvestigateAvailability, ipcArgs ) );
    MUS_LOG( "mus: [MUSCLI]  <- RMusManagerClientSession::InvestigateAvailabilityL()" );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
MultimediaSharing::TMusAvailabilityStatus RMusManagerClientSession::QueryAvailabilityL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::QueryAvailabilityL()" );
    TInt capability;
    TPckg<TInt> capPckg( capability );

    // construct a single argument
    TIpcArgs ipcArgs( &capPckg );

    // send and wait for answer
    User::LeaveIfError( SendReceive( EMusManagerIpcAvailabilityQuery, ipcArgs ) );

    MUS_LOG( "mus: [MUSCLI]  <- RMusManagerClientSession::QueryAvailabilityL()" );
    return ( MultimediaSharing::TMusAvailabilityStatus ) capability;
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void RMusManagerClientSession::StartL( MultimediaSharing::TMusUseCase aUseCase )
    {
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::StartL()" );
    TPckg<TInt> pckg( ( TInt ) aUseCase );
    TIpcArgs ipcArgs( &pckg );
    // send
    
    User::LeaveIfError( SendReceive( EMusManagerIpcStartMultimediaSharing, ipcArgs ) );
    MUS_LOG( "mus: [MUSCLI]  <- RMusManagerClientSession::StartL()" );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void RMusManagerClientSession::StartObservingAvailabilityL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::\
    		StartObservingAvailabilityL()" );
    // construct an empty argument
    TIpcArgs ipcArgs( TIpcArgs::ENothing );
    
    // send
    User::LeaveIfError( SendReceive( EMusManagerIpcStartObservingAvailability, ipcArgs ) );
    MUS_LOG( "mus: [MUSCLI]  <- RMusManagerClientSession::\
    		StartObservingAvailabilityL()" );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void RMusManagerClientSession::StopL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::StopL()" );
    // construct an empty argument
    TIpcArgs ipcArgs( TIpcArgs::ENothing );

    // send
    User::LeaveIfError( SendReceive( EMusManagerIpcStopMultimediaSharing, ipcArgs ) );
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::StopL()" );
    }
    
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void RMusManagerClientSession::HandleCommandL( 
								MultimediaSharing::TCommandType aCommandType )
	{
	MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::HandleCommandL()" );
    // construct a single argument
    TPckg<TInt> pckg( ( TInt ) aCommandType );
    TIpcArgs ipcArgs( &pckg );
    // send
    User::LeaveIfError( SendReceive( EMusManagerIpcHandleCommand, ipcArgs ) );    
    MUS_LOG( "mus: [MUSCLI]  <- RMusManagerClientSession::HandleCommandL()" );	
	}

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void RMusManagerClientSession::HandleSipRequestL( TInt aUid )
    {
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::HandleSipRequestL()" );
    // construct a single argument
    TPckg<TInt> pckg( aUid );
    TIpcArgs ipcArgs( &pckg );
    // Handle sip options in a different way
    if (aUid == CMusManager::ESipOptions)
        {
        MUS_LOG( "mus: [MUSCLI]  -> This is sip options request sending to server" );
        User::LeaveIfError( SendReceive( EMusManagerIpcOptionsReceived, ipcArgs ) );
        }
    else
        {
        MUS_LOG( "mus: [MUSCLI]  -> This is some invitation request sending to server" );
        User::LeaveIfError( SendReceive( EMusManagerIpcInvitationReceived, ipcArgs ) );
        }
        
    MUS_LOG( "mus: [MUSCLI]  <- RMusManagerClientSession::HandleSipRequestL()" );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void RMusManagerClientSession::MonitorAvailabilityL(
                TRequestStatus& aRequestStatus,
                TIpcArgs& aIpcArgs )
    {
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::MonitorAvailabilityL()" );
    SendReceive( 
    	EMusManagerIpcMonitorAvailability, 
    	aIpcArgs,
    	aRequestStatus );
    MUS_LOG( "mus: [MUSCLI]  <- RMusManagerClientSession::MonitorAvailabilityL()" );
    }

// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void RMusManagerClientSession::CancelMonitoringL()
    {
    MUS_LOG( "mus: [MUSCLI]  -> RMusManagerClientSession::CancelMonitoring()" );
    // construct an empty argument
    TIpcArgs ipcArgs( TIpcArgs::ENothing );

    // send
    User::LeaveIfError( SendReceive( EMusManagerIpcCancelMonitoring, ipcArgs ) );
    MUS_LOG( "mus: [MUSCLI]  <- RMusManagerClientSession::CancelMonitoring()" );
    }
