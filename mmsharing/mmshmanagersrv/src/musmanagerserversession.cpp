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

#include "muslogger.h"
#include "musmanageripccommon.h"
#include "musmanagerserveritcutility.h"
#include "musmanagerserversession.h"
#include "musmanagerserversessionobserver.h"
#include "musmanagerserver.h"


const TInt KServerRequestPending = 1;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Second phase constructor.
// ---------------------------------------------------------------------------
//
CMusManagerServerSession::CMusManagerServerSession(
    CMusManagerServer& aServer,
    MMusManagerServerSessionObserver& aObserver )
    : CSession2(), iServer( aServer ), iServerCore( aObserver )
    {
    iServer.IncrementSessions();
    }

// ---------------------------------------------------------------------------
// Second phase constructor.
// ---------------------------------------------------------------------------
//
void CMusManagerServerSession::ConstructL()
    {
 
    }


// ---------------------------------------------------------------------------
// First phase constructor.
// ---------------------------------------------------------------------------
//
CMusManagerServerSession* CMusManagerServerSession::NewL(
    CMusManagerServer& aServer,
    MMusManagerServerSessionObserver& aObserver )
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerSession::NewL()" );
    CMusManagerServerSession* self = CMusManagerServerSession::NewLC(
        aServer,
        aObserver );
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerSession::NewL()" );
    return self;
    }


// ---------------------------------------------------------------------------
// First phase constructor.
// ---------------------------------------------------------------------------
//
CMusManagerServerSession* CMusManagerServerSession::NewLC(
    CMusManagerServer& aServer,
    MMusManagerServerSessionObserver& aObserver )
    {
    CMusManagerServerSession* self = new( ELeave ) CMusManagerServerSession(
        aServer,
        aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// ---------------------------------------------------------------------------
// Standard destructor.
// ---------------------------------------------------------------------------
//
CMusManagerServerSession::~CMusManagerServerSession()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerSession::~CMusManagerServerSession()" );
    iServer.DecrementSessions();
    iServerCore.CancelMonitoring( this );
    iAvailabilityStatus.Close();
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerSession::~CMusManagerServerSession()" );
    }
    
// -----------------------------------------------------------------------------
// Start to monitor availability states for this session.
// -----------------------------------------------------------------------------
//
TInt CMusManagerServerSession::MonitorAvailabilityL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerSession::MonitorAvailabilityL" );			
    if ( iAvailabilityStatus.Count() )
    	{
    	MusManagerServerItcUtility::WriteAvailabilityReportL( 
		iMessage, iAvailabilityStatus[0] );                         
	    iAvailabilityStatus.Remove( 0 );
	    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerSession::\
	    	MonitorAvailabilityL" );
	    return KErrNone;	
    	}
   	else
   		{
   		MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerSession::\
   			MonitorAvailabilityL" );
   		return KServerRequestPending;	
   		}
    }

// ---------------------------------------------------------------------------
// Receives incoming messages.
// ---------------------------------------------------------------------------
//
void CMusManagerServerSession::ServiceL( const RMessage2& aMessage )
    {
    MUS_LOG1( "mus: [MUSSRV]     -> CMusManagerServerSession::ServiceL( %d )",
              ( TInt ) aMessage.Function() );
    TMusManagerIpcFunctions function
        = ( TMusManagerIpcFunctions )aMessage.Function();

    TInt retVal( KErrNone );

    switch ( function )
        {
    case EMusManagerIpcAvailabilityQuery:
        {
        TInt capability = iServerCore.AvailabilityQueryL();
        MusManagerServerItcUtility::WriteAvailabilityL(
            aMessage,
            capability );

        iLastReport = ( MultimediaSharing::TMusAvailabilityStatus )capability;
        break;
        }
    case EMusManagerIpcInvestigateAvailability:
        {
        TRAP( retVal, iServerCore.InvestigateAvailabilityL() );
        break;
        }
    case EMusManagerIpcInvitationReceived:
        {
        TUid channelId
            = MusManagerServerItcUtility::ReadChannelIdL( aMessage );

        TRAP( retVal, iServerCore.InvitationReceivedL( channelId ) );

        break;
        }
    case EMusManagerIpcOptionsReceived:
        {
        TUid channelId
            = MusManagerServerItcUtility::ReadChannelIdL( aMessage );

        TRAP( retVal, iServerCore.OptionsReceivedL( channelId ) );
        break;
        }
    case EMusManagerIpcStartMultimediaSharing:
        {
        TInt useCase
            = MusManagerServerItcUtility::ReadUseCaseL( aMessage );

        TRAP( retVal, iServerCore.StartMultimediaSharingL(
                        (MultimediaSharing::TMusUseCase) useCase ) );
        break;
        }
    case EMusManagerIpcStopMultimediaSharing:
        {
        TRAP( retVal, iServerCore.StopMultimediaSharingL() );
        break;
        }
     case EMusManagerIpcStartObservingAvailability:
        {
        iServerCore.RegisterObserverL( this ); 
        break;
        }    
     
    case EMusManagerIpcMonitorAvailability:
        {
        iMessage = aMessage;
        retVal = MonitorAvailabilityL(); 
        }
        break;
    case EMusManagerIpcCancelMonitoring:
        {
        iServerCore.CancelMonitoring( this );
        }
        break;
    case EMusManagerIpcHandleCommand:
        {
        TInt command
            = MusManagerServerItcUtility::ReadCommandL( aMessage );

        TRAP( retVal, iServerCore.CommandL(
                        ( MultimediaSharing::TCommandType ) command ) );
        }
        break;

    default:
        break;
        }
	
    MUS_LOG1( "mus: [MUSSRV]  - Message compleated with code  %d", retVal );
  	aMessage.Complete( retVal );	

	MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerSession::ServiceL()" );
    }

void CMusManagerServerSession::AvailabilityChangedL(
    					MultimediaSharing::TMusAvailabilityStatus aAvailability )
    {
    MUS_LOG1( "mus: [MUSSRV]  -> CMusManagerServerCore::AvailabilityChangedL( %d )",
              aAvailability );
    // inform ui via application manager (via session api)
    if( iAvailabilityStatus.Count() && !iMessage.IsNull()  )
    	{
    	iAvailabilityStatus.AppendL( aAvailability );	
    	MusManagerServerItcUtility::WriteAvailabilityReportL( 
			iMessage, iAvailabilityStatus[0] );                         
	    iAvailabilityStatus.Remove( 0 );
	    iMessage.Complete( KErrNone );
    	}
    else if ( iMessage.IsNull() )
    	{
    	iAvailabilityStatus.AppendL( aAvailability );	                      
    	}
    else
    	{
    	MusManagerServerItcUtility::WriteAvailabilityReportL( 
			iMessage, aAvailability );  
		iMessage.Complete( KErrNone ); 	
    	}
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerSession::AvailabilityChangedL()" );
    }

void CMusManagerServerSession::RequestComplete()
	{
	MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerSession::RequestComplete()" );
 	iMessage.Complete( KErrCancel );
  	MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerSession::RequestComplete()" );	
	}
	
