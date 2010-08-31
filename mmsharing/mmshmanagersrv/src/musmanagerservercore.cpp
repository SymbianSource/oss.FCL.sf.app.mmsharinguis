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


#include "musunittesting.h"

#include "musmanagerservercore.h"
#include "musapplicationmanager.h"
#include "musavailabilitypluginmanager.h"
#include "musmonitoravailabilityobserver.h"
#include "muslogger.h"
#include "musmanagerservercoreobserver.h"
#include "musmanager.h"
#include "musmanagerservercommon.h"

using namespace MultimediaSharing;

// CONSTANTS
_LIT8( KMusEngineName, "MultimediaSharing" );


// -----------------------------------------------------------------------------
// CMusManagerServerCore::NewL
// -----------------------------------------------------------------------------
//
CMusManagerServerCore* CMusManagerServerCore::NewL(
    MMusManagerServerCoreObserver& aObserver )
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::NewL()" );

    CMusManagerServerCore* self = CMusManagerServerCore::NewLC( aObserver );
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::NewL()" );
    return self;
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::NewLC
// -----------------------------------------------------------------------------
//
CMusManagerServerCore* CMusManagerServerCore::NewLC(
    MMusManagerServerCoreObserver& aObserver )
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::NewLC()" );

    CMusManagerServerCore* self
        = new( ELeave ) CMusManagerServerCore( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::NewLC()" );
    return self;
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::CMusManagerServerCore
// -----------------------------------------------------------------------------
//
CMusManagerServerCore::CMusManagerServerCore( MMusManagerServerCoreObserver& aObserver )
    : iObserver( aObserver )
    {
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::ConstructL
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::ConstructL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::ConstructL" );
    
    iApplicationManager = CMusApplicationManager::NewL();
    
    iApplicationManager->ResolvePluginNameL( iPluginName );
    
    iPluginManager = CMusAvailabilityPluginManager::NewL( *this, *iApplicationManager );

    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::ConstructL" );
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::~CMusManagerServerCore
// -----------------------------------------------------------------------------
//
CMusManagerServerCore::~CMusManagerServerCore()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::CMusManagerServerCore" );

     // inform ui via application manager (via session api)
    
    delete iPluginManager;

    delete iApplicationManager;
    
     // go through interested client sessions
    StopMonitoring();
    iAvailabilityMonitors.Close();
    
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::CMusManagerServerCore" );
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::~CMusManagerServerCore
// -----------------------------------------------------------------------------
//
MultimediaSharing::TMusAvailabilityStatus CMusManagerServerCore::AvailabilityQueryL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::AvailabilityQueryL" );
    MultimediaSharing::TMusAvailabilityStatus status
            = Availability();
   
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::AvailabilityQueryL" );
    return status;
    }
 // -----------------------------------------------------------------------------
// CMusManagerServerCore::InvestigateAvailability
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::CommandL( MultimediaSharing::TCommandType aCommandType )
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::CommandL" );
     if ( aCommandType == MultimediaSharing::ECommandManualActivation &&
          IsMusEnginePlugin() )
		{
		MUS_LOG( "mus: [MUSSRV]    Manual Activation is started" )
		iPluginManager->ManualQueryL();
		}
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::CommandL" );
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::InvestigateAvailability
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::InvestigateAvailabilityL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::InvestigateAvailabilityL" );
    if ( IsMusEnginePlugin() )
        {
        iPluginManager->InvestigateAvailabilityL();
        }
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::InvestigateAvailabilityL" );
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::InvitationReceived
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::InvitationReceivedL( TUid aChannelId )
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::InvitationReceivedL" );

    TInt uid = aChannelId.iUid;

    if( uid == CMusManager::ESipInviteDesired &&
        ApplicationAllowed() )
        {        
        PrepareForReceivedInviteL();
        
        // write session boundary values
        WriteSessionPropertiesL(
                MultimediaSharing::EMusReceive,
                Availability(),
                iPluginManager->SessionParametersL() );

        iApplicationManager->StartApplicationL();
        }
    else if( uid == CMusManager::ESipInviteDesired2WayVideo &&
            ApplicationAllowed() )
        {
        PrepareForReceivedInviteL();
        
        // write session boundary values
        WriteSessionPropertiesL(
                MultimediaSharing::EMusReceiveTwoWayVideo,
                Availability(),
                iPluginManager->SessionParametersL() );

        iApplicationManager->StartApplicationL();
        }
    else if ( IsMusEnginePlugin() )
        {
        if( uid == CMusManager::ESipInviteNotDesired )
            {
            iPluginManager->InvitationReceivedL();
            }
        else
            {
            User::Leave( KErrNotReady );
            }
        }

    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::InvitationReceivedL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::OptionsReceivedL( TUid /*aChannelId*/ )
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::OptionsReceivedL" );
    if ( IsMusEnginePlugin() )
        {
        iPluginManager->OptionsReceivedL();    
        }    
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::OptionsReceivedL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::StartMultimediaSharingL( MultimediaSharing::TMusUseCase aUseCase )
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::StartMultimediaSharingL" );
    if( !iApplicationManager->ApplicationRunning() &&
        ApplicationAllowed() )
        {
        // write session boundary values
        WriteSessionPropertiesL(
                aUseCase,
                Availability(),
                iPluginManager->SessionParametersL() );

        // start actual application
        iApplicationManager->StartApplicationL();
        }
    else
        {
        iApplicationManager->ShowApplicationL();
        }
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::StartMultimediaSharingL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::StopMultimediaSharingL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::StopMultimediaSharingL" );
    iApplicationManager->StopApplicationL();
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::StopMultimediaSharingL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::EventNoSessions()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::EventNoSessions" );
    if ( IsMusEnginePlugin() ) 
        {
        TRAPD( error, iPluginManager->StopPluginL() );
        if( error )
            {
            // iObserver.StopServer();
            }        
        }
    iObserver.StopServer();
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::EventNoSessions" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::PluginStopped()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::PluginStopped" );
    if( iObserver.SessionCount() == 0 )
        {
        iObserver.StopServer();
        }
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::PluginStopped" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::AvailabilityChangedL(
    MultimediaSharing::TMusAvailabilityStatus aAvailability )
    {
    MUS_LOG1( "mus: [MUSSRV]  -> CMusManagerServerCore::AvailabilityChangedL( %d )",
              aAvailability );
    
    if ( !IsMusEnginePlugin() )
        {
        MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::AvailabilityChangedL - \
                non-MuS plugin");
        return;
        }
    // inform ui via application manager (via session api)

    WriteSessionPropertiesL(
            aAvailability,
            iPluginManager->SessionParametersL() );
    
    if( iAvailabilityMonitors.Count() )
    	{	
    	// go through interested client sessions
	    for( TInt i=0; i<iAvailabilityMonitors.Count(); i++ )
	        {
	        // and inform each about the new status
	       	iAvailabilityMonitors[i]->AvailabilityChangedL( 
	       		aAvailability );
	        }
    	}
    
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::AvailabilityChangedL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::CancelMonitoring( MMusMonitorAvailabilityObserver* aObserver )
	{
	TInt index = iAvailabilityMonitors.Find( aObserver );
		
	if( index != KErrNotFound )
		{
		iAvailabilityMonitors[index]->RequestComplete();
		iAvailabilityMonitors.Remove( index );
		}	
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::RegisterObserverL( 
									MMusMonitorAvailabilityObserver* aObserver )
	{
    if( iAvailabilityMonitors.Find( aObserver ) == KErrNotFound )
		{
		iAvailabilityMonitors.AppendL( aObserver );
		}
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::RegisterObserver" );	
	}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//	
void CMusManagerServerCore::RemoveObserver( MMusMonitorAvailabilityObserver* aObserver )
	{
	MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::RemoveObserver" );
    TInt index = iAvailabilityMonitors.Find( aObserver );
		
	if( index != KErrNotFound )
		{
		iAvailabilityMonitors.Remove( index );
		}
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::RemoveObserver" );	
	}

// -----------------------------------------------------------------------------
// From MMusAvailabilityPluginManagerObserver.
// Starts live video sharing.
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::StartSharingWithUseCaseL( 
    MultimediaSharing::TMusUseCase aUseCase )
    {
    MUS_LOG1( "mus: [MUSSRV]  -> CMusManagerServerCore::StartSharingWithUseCaseL:%d", 
              aUseCase );
    
    StartMultimediaSharingL( aUseCase );
    
    MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::StartSharingWithUseCaseL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::StopMonitoring()
	{
	// go through interested client sessions
    for( TInt i=0; i<iAvailabilityMonitors.Count(); i++ )
        {
        // and inform each about the new status
       	iAvailabilityMonitors[i]->RequestComplete();
       	iAvailabilityMonitors.Remove( i );
        }			
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::IsMusEnginePlugin
// -----------------------------------------------------------------------------
//
TBool CMusManagerServerCore::IsMusEnginePlugin()
    {
    TBool isMus( EFalse );    
    if ( !iPluginName.Compare( KMusEngineName ) )
        {
        isMus = ETrue;
        }
    return isMus;
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::Availability
// -----------------------------------------------------------------------------
//
MultimediaSharing::TMusAvailabilityStatus CMusManagerServerCore::Availability()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusManagerServerCore::Availability" );
    if ( IsMusEnginePlugin() ) 
        {
        MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::Availability - MuS plugin" );
        return iPluginManager->Availability();
        }
    else
        {
        MUS_LOG( "mus: [MUSSRV]  <- CMusManagerServerCore::Availability - non-MuS plugin" );
        return EMultimediaSharingAvailable;
        }
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::ApplicationAllowed
// -----------------------------------------------------------------------------
//
TBool CMusManagerServerCore::ApplicationAllowed()
    {
    if ( IsMusEnginePlugin() )
        {
        return iPluginManager->ApplicationAllowed();
        }
    else
        {
        return ETrue;
        }
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::PrepareForReceivedInviteL
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::PrepareForReceivedInviteL()
    {
    if ( IsMusEnginePlugin() )
        {
        iPluginManager->PrepareForReceivedInviteL();
        }
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::WriteSessionPropertiesL
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::WriteSessionPropertiesL(
        MultimediaSharing::TMusUseCase aUseCase,
        MultimediaSharing::TMusAvailabilityStatus aStatus,
        MDesCArray& aSessionParameters )
    {
    if ( IsMusEnginePlugin() )
        {
        iApplicationManager->WriteSessionPropertiesL(
                                    aUseCase,
                                    aStatus,
                                    aSessionParameters );
        }
    }

// -----------------------------------------------------------------------------
// CMusManagerServerCore::WriteSessionPropertiesL
// -----------------------------------------------------------------------------
//
void CMusManagerServerCore::WriteSessionPropertiesL(
        MultimediaSharing::TMusAvailabilityStatus aStatus,
        MDesCArray& aSessionParameters )
    {
    if ( IsMusEnginePlugin() )
        {
        iApplicationManager->WriteSessionPropertiesL(
                                    aStatus,
                                    aSessionParameters );
        }
    }

//  End of File  
