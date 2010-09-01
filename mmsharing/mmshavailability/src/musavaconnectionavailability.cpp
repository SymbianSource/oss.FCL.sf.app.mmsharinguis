/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Provide interface for the client requestin availability class.
*
*/



#include "musavaconnectionavailability.h"
#include "musavaavailabilityobserver.h"
#include "musavasettingsimp.h"
#include "musavaobserver.h"
#include "musavaavailability.h"
#include "musavasharedobject.h"
#include "musavaconnectionmonitor.h"
#include "musavanetworkregistrationstatus.h"
#include "musavanetworkmodestatus.h"
#include "musavasip.h"
#include "mussettings.h"
#include "mussettingskeys.h"
#include "muslogger.h"

#include <e32base.h>
#include <rconnmon.h>
#include <e32cmn.h>
#include <etelmm.h>
#include <nifvar.h>


// --------------------------------------------------------------------------
// Symbian two-phase constructor
// --------------------------------------------------------------------------
//
CMusAvaConnectionAvailability* CMusAvaConnectionAvailability::NewL(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings  )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionAvailability::NewL()" )
    CMusAvaConnectionAvailability* self =
        CMusAvaConnectionAvailability::NewLC(
            aObserver, aSettings );
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::NewL()" )
    return self;
    }


// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaConnectionAvailability* CMusAvaConnectionAvailability::NewLC(
    MMusAvaAvailabilityObserver& aObserver, 
    CMusAvaSettingsImp& aSettings  )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionAvailability::NewLC()" )
    CMusAvaConnectionAvailability* self =
        new( ELeave ) CMusAvaConnectionAvailability(
            aObserver, aSettings );
    CleanupStack::PushL( self );
    self->ConstructL();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::NewLC()" )
    return self;
    }


// -------------------------------------------------------------------------
//  Destructor.
// -------------------------------------------------------------------------
//
CMusAvaConnectionAvailability::~CMusAvaConnectionAvailability()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionAvailability::\
        ~CMusAvaConnectionAvailability()" )
    if( iSharedObj )
        {
        iSharedObj->DeleteSingleton();
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::\
        ~CMusAvaConnectionAvailability()" )
    }


// -------------------------------------------------------------------------
//  Constructor.
// -------------------------------------------------------------------------
//
CMusAvaConnectionAvailability::CMusAvaConnectionAvailability(
    MMusAvaAvailabilityObserver& aObserver, 
    CMusAvaSettingsImp& aSettings  )
    :CMusAvaAvailability( aObserver ),
    iSettings( aSettings )
    {
    }


// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMusAvaConnectionAvailability::ConstructL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionAvailability::ConstructL()" )
    iSharedObj = CMusAvaSharedObject::GetSingletonL();
    iNetworkRegistrationStatus =  &iSharedObj->MusAvaTelephonyStatus();
    iNetworkModeStatus = &iSharedObj->NetworkModeStatus();
    iConnectionMonitor = &iSharedObj->ConnectionMonitor();
    iNetworkRegistrationStatus->AddAdapterL( *this );
	iNetworkModeStatus->AddAdapterL( *this );
	iConnectionMonitor->AddAdapterL( *this );
	iConnectionMonitor->NotifyEventL();
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::ConstructL()" )
    }


// ---------------------------------------------------------------------------
// From CMusAvaAvailability.
// Executes for the availability.
// ---------------------------------------------------------------------------
//
void CMusAvaConnectionAvailability::DoExecuteL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionAvailability::DoExecuteL()" )
	MMusAvaObserver::TAvailabilityStatus status; 
    MUS_LOG( "mus: [MUSAVA]  Check network mode" )
    RMobilePhone::TMobilePhoneNetworkMode networkmode = 
                                        iNetworkModeStatus->PhoneNetworkMode();
    if ( networkmode == RMobilePhone::ENetworkModeWcdma||
         networkmode == RMobilePhone::ENetworkModeTdcdma ||
         networkmode == RMobilePhone::ENetworkModeCdma95 ||
         networkmode == RMobilePhone::ENetworkModeCdma2000 )
        {
        status = NetworkRegistrationAndSettingsL();
    	if ( status == MMusAvaObserver::EMusAvaStatusAvailable )
	        {
	        MUS_LOG( "mus: [MUSAVA]  Network available" )
	        SetState( status );
	        }
        else
            {
            MUS_LOG1( "mus: [MUSAVA] Activation error: %d", status )
            SetState( status );
            }        
        }
    else if( networkmode == RMobilePhone::ENetworkModeGsm)
    	{
    	TInt edgeDtmSupport = KErrNone;
        TRAPD( err, edgeDtmSupport = 
                       MultimediaSharingSettings::EdgeDtmSupportSettingL() );
        if( err != KErrNone )
            {
            MUS_LOG1( "mus: [MUSAVA]  Reading activation setting error:%d", err )
            SetState( MMusAvaObserver::EMusActivationError );
            }
        else if( edgeDtmSupport == MusSettingsKeys::EDtmModeAllowed )
            { 
            MUS_LOG( "mus: [MUSAVA]  Edge Network Mode allowed in settings " )     
            if( iConnectionMonitor->ConnectionCount() > 0)       
                {
                MUS_LOG( "mus: [MUSAVA]  Atlease one packect connection exists " )     
                SetState( NetworkRegistrationAndSettingsL() );  
                }
            else
                {
                MUS_LOG( "mus: [MUSAVA]  No Packet connection exists now." )     
                SetState( MMusAvaObserver::EMusAvaEdgeDtmStatusUnknown );                    
                }            
            }
        else
            {
            MUS_LOG( "mus: [MUSAVA]  Edge Network Mode Not allowed in settings " )     
            SetState( MMusAvaObserver::EMusAvaNetworkType );    
            }
    	}
    else
        {
        MUS_LOG( "mus: [MUSAVA] Network not available" )
        SetState( MMusAvaObserver::EMusAvaNetworkType );
        }
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::DoExecuteL()" )
    }


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TBool CMusAvaConnectionAvailability::OperatorVariant()
    {
    MusSettingsKeys::TOperatorVariant variantValue = MusSettingsKeys::EStandard;
    TRAPD( error, variantValue = 
                  MultimediaSharingSettings::OperatorVariantSettingL() );
    
    return ( variantValue == MusSettingsKeys::EOperatorSpecific && 
             error == KErrNone );
     
    }

// ---------------------------------------------------------------------------
// From CMusAvaAvailability.
// Stops execution.
// ---------------------------------------------------------------------------
//
void CMusAvaConnectionAvailability::Stop()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionAvailability::Stop()" )
    
    iNetworkRegistrationStatus->RemoveAdapter( *this );
    iNetworkModeStatus->RemoveAdapter( *this );
    iConnectionMonitor->RemoveAdapter( *this );
    
    SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::Stop()" )
    }


// ---------------------------------------------------------------------------
// From CMusAvaAvailability.
// Returns name of *this* availability module.
// ---------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityName CMusAvaConnectionAvailability::Name()
    {
    return MMusAvaObserver::EMusAvaBearerStatus;
    }


// ---------------------------------------------------------------------------
// CMusAvaConnectionAvailability::PhoneNetworkModeStatus
// 
// ---------------------------------------------------------------------------
//
void CMusAvaConnectionAvailability::PhoneNetworkModeStatus(
                            RMobilePhone::TMobilePhoneNetworkMode aStatus )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionAvailability::\
    PhoneNetworkModeStatus()" )
    MUS_LOG1( "mus: [MUSAVA] Mobile Phone Network Status = ", aStatus )

    switch ( aStatus )
        {
        case RMobilePhone::ENetworkModeWcdma :
        case RMobilePhone::ENetworkModeTdcdma :
        case RMobilePhone::ENetworkModeCdma95:
        case RMobilePhone::ENetworkModeCdma2000:
            {
            MMusAvaObserver::TAvailabilityStatus status = 
                                        MMusAvaObserver::EMusActivationError;
            TRAP_IGNORE( status = NetworkRegistrationAndSettingsL() );
            SetState( status );
            break;
            }
        case RMobilePhone::ENetworkModeGsm : // For EDGE /DTM support
            {
            TInt edgeDtmSupport = 0;
            TRAPD( err, edgeDtmSupport = 
                       MultimediaSharingSettings::EdgeDtmSupportSettingL() );
            if( err != KErrNone )
                {
                MUS_LOG1( "mus: [MUSAVA]  Reading activation setting error:%d", err )
                SetState( MMusAvaObserver::EMusActivationError );
                }
            else if( edgeDtmSupport == MusSettingsKeys::EDtmModeAllowed )
                { 
                MUS_LOG( "mus: [MUSAVA]  Edge Network supported " )            
                SetState( MMusAvaObserver::EMusAvaEdgeDtmStatusUnknown );                               
                }
            else
                {
                SetState( MMusAvaObserver::EMusAvaNetworkType );    
                }
            break;
            }
         default :
            {
            SetState( MMusAvaObserver::EMusAvaNetworkType );
            }
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::\
    PhoneNetworkModeStatus()" )
    }


// ---------------------------------------------------------------------------
// CMusAvaConnectionAvailability::NetworkRegistrationStatus
// 
// ---------------------------------------------------------------------------
//
void CMusAvaConnectionAvailability::NetworkRegistrationStatus(
                    RMobilePhone::TMobilePhoneRegistrationStatus aRegStatus )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionAvailability::\
NetworkRegistrationStatus()" )
    MUS_LOG1( "Mobile Phone Network Status = ", aRegStatus )
    
    if( OperatorVariant() &&
        aRegStatus != RMobilePhone::ERegisteredOnHomeNetwork )
        {
        MUS_LOG( "mus: [MUSAVA]  Roaming not allowed" )
        SetState( MMusAvaObserver::EMusAvaRoamingError );
        }

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::\
NetworkRegistrationStatus()" )
    }


// ---------------------------------------------------------------------------
// CMusAvaConnectionAvailability::EventL
//
// ---------------------------------------------------------------------------
//
void CMusAvaConnectionAvailability::EventL(
                                        const CConnMonEventBase& aConnMonEvent)
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionAvailability::EventL()" )
    TInt connectionId = 0;
    TInt event = aConnMonEvent.EventType();
    TUint connId = aConnMonEvent.ConnectionId();
    CMusAvaConnectionMonitor& connectionMonitor = 
                                            iSharedObj->ConnectionMonitor();
    TUint musConnID = connectionMonitor.CreatedConnectionID();
    MUS_LOG1( "mus: [MUSAVA] Current connection ID %d = ", musConnID )
    MUS_LOG1( "mus: [MUSAVA] Event connection ID %d = ", connId )

    switch ( event )
        {
        case EConnMonCreateConnection:
            {
            MUS_LOG( "mus: [MUSAVA] Event type = EConnMonCreateConnection" )
            if( musConnID == 0 )
                {
                MUS_LOG( "mus: [MUSAVA] Store Connection ID" )
                TRAPD( err, connectionId = 
                                        iConnectionMonitor->ConnectionIDL() );
                if ( err == KErrNotFound )
                    {
                    SetState( MMusAvaObserver::EMusAvaConnectionErr );
                    }
               	else
                    {
                    iConnectionMonitor->SetConnectionID( connectionId );
                    }
                }
            break;
            }
        case EConnMonDeleteConnection:
            {
            MUS_LOG( "mus: [MUSAVA] Event type = EConnMonDeleteConnection" )
            if( connId == musConnID )
                {
                SetState( MMusAvaObserver::EMusAvaConnectionErr );
                }
                break;
                }
         case EConnMonConnectionStatusChange:
            {
            MUS_LOG( "mus: [MUSAVA] Event type = \
EConnMonConnectionStatusChange" )
            const CConnMonConnectionStatusChange* connectionStatusChange
                 = (const CConnMonConnectionStatusChange*)(&aConnMonEvent);
            TInt connStatus = connectionStatusChange->ConnectionStatus();
            MUS_LOG1( "mus: [MUSAVA] ConnectionStatus =  %d", connStatus )
            if ( connId == musConnID )
                {
                if( connStatus == KConnectionFailure ||
                    connStatus == KConnectionClosed ||
                    connStatus == KDataTransferTemporarilyBlocked ||
                    connStatus == KLinkLayerClosed ||
                    connStatus == KPsdSuspended ||
                    connStatus == KPsdFinishedDeactivation )
                    {
                    SetState( MMusAvaObserver::EMusAvaConnectionErr );
                    }
                if( connStatus == KConnectionOpen ||
                    connStatus == KLinkLayerOpen )
                    {
                    SetState( NetworkRegistrationAndSettingsL() );
                    }
                }
            break;
            }
        case EConnMonNetworkStatusChange:
            {
            MUS_LOG( "mus: [MUSAVA] Event type = EConnMonNetworkStatusChange" )
            const CConnMonNetworkStatusChange* networkStatusChange
                 = (const CConnMonNetworkStatusChange*)(&aConnMonEvent);
			TInt networkStatus = networkStatusChange->NetworkStatus();
            MUS_LOG1( "mus: [MUSAVA] NetworkStatus =  %d", networkStatus )
            if( connId == EBearerIdWCDMA || connId == EBearerIdWcdmaCSD )
            	{
            	if( EConnMonStatusAttached == networkStatus ||
            	    EConnMonStatusActive == networkStatus )
                    {
                    SetState( NetworkRegistrationAndSettingsL() );
                    }
             	else
                    {
                    SetState( MMusAvaObserver::EMusAvaConnectionErr );
                    }
            	}
            break;
            }
        case EConnMonNetworkRegistrationChange:
            {
            MUS_LOG( "mus: [MUSAVA] Event type = \
EConnMonNetworkRegistrationChange" )
            const CConnMonNetworkRegistrationChange* registrationChange
                 = (const CConnMonNetworkRegistrationChange*)(&aConnMonEvent);
            TInt registrationStatus = registrationChange->RegistrationStatus();
            MUS_LOG1( "mus: [MUSAVA] RegistrationStatus =  %d", 
                    registrationStatus )
            if( connId == EBearerIdWCDMA || connId == EBearerIdWcdmaCSD )
                {
                if ( ENetworkRegistrationRoaming == registrationStatus )
                {
                SetState( MMusAvaObserver::EMusAvaRoamingError );
                }
            else
                {
                SetState( NetworkRegistrationAndSettingsL() );
                }
            }
            break;
            }
        case EConnMonBearerChange:
            {
            MUS_LOG( "mus: [MUSAVA] Event type = EConnMonBearerChange" )
            MUS_LOG1( "mus: [MUSAVA] Bearer =  %d",connId )
            if( connId == EBearerIdGPRS ||
                connId == EBearerIdWCDMA || 
                connId == EBearerIdWcdmaCSD )
                {
                SetState( NetworkRegistrationAndSettingsL() );
                }
            else
                {
                SetState( MMusAvaObserver::EMusAvaNetworkType );
                }
            break;
            }

        case EConnMonPacketDataAvailable:
            {
            MUS_LOG( "mus: [MUSAVA] EConnMonPacketDataAvailable" )
            /* This is for EDGE /DTM support , This is the only place we can be 
               sure that packet data available along with CS call. Here we have to
               check again for activation settings and based on that availability 
               state has to be set
            */
            MMusAvaObserver::TAvailabilityStatus status
                                                = NetworkRegistrationAndSettingsL();
            SetState( status );            
            break;
            }

        case EConnMonPacketDataUnavailable:
            {
            SetState( MMusAvaObserver::EMusAvaNetworkType );
            MUS_LOG( "mus: [MUSAVA] EConnMonPacketDataUnavailable" )
            break;
            }

        case EConnMonBearerInfoChange:
            {
            MUS_LOG( "mus: [MUSAVA] EConnMonBearerInfoChange" )           
            break;
            }
        default :
            {
            MUS_LOG( "mus: [MUSAVA] Event type is not implemented" )
            MUS_LOG1( "mus: [MUSAVA] Event type %d = ", event )
            break;
            }
        }

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::EventL()" )
    }

// -------------------------------------------------------------------------
// CMusAvaConnectionAvailability::MusAvaManualActivation
//
// -------------------------------------------------------------------------
//    
MMusAvaObserver::TAvailabilityStatus CMusAvaConnectionAvailability::
    ManualActivationL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionAvailability::\
ManualActivationL()" )

    if( OperatorVariant() )
    	{
    	MUS_LOG( "mus: [MUSAVA] Operator Specific No Manual query" )
    	MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::\
NetworkRegistrationAndSettingsL()" )
    	return MMusAvaObserver::EMusActivationError;	
    	}
    else
        {
        if ( iSettings.ManualActivation() == 
            MMusAvaSettings::EActivationAllowed )
            {
            MUS_LOG( "mus: [MUSAVA] Manual activation allowed" )
            MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::\
ManualActivationL()" )
            return MMusAvaObserver::EMusAvaStatusAvailable;	
            }
        else if ( iSettings.ManualActivation() == 
            MMusAvaSettings::EActivationNotExecuted )
            {
            MUS_LOG( "mus: [MUSAVA] Manual activation is not executed" )
            MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::\
ManualActivationL()" )
            return MMusAvaObserver::EMusAvaManualActivation;	
            }
        else
            {
            MUS_LOG( "mus: [MUSAVA] Activation error" )
            MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::\
ManualActivationL()" )
            return MMusAvaObserver::EMusActivationError;	
            }
        }
    }

// -------------------------------------------------------------------------
// CMusAvaConnectionAvailability::NetworkRegistrationAndSettingsL
//
// -------------------------------------------------------------------------
//
MMusAvaObserver::TAvailabilityStatus CMusAvaConnectionAvailability::
    NetworkRegistrationAndSettingsL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionAvailability::\
NetworkRegistrationAndSettingsL()" )
    MUS_LOG( "mus: [MUSAVA]  Check registration network status" )
    MUS_LOG( "mus: [MUSAVA]  Check roaming" )

    RMobilePhone::TMobilePhoneRegistrationStatus regStatus;
    iNetworkRegistrationStatus->NetworkRegistration( regStatus );

    MUS_LOG1( "mus: [MUSAVA]     NetworkRegistration returned %d", regStatus )

    MMusAvaObserver::TAvailabilityStatus status;
    MusSettingsKeys::TActivation activation = MusSettingsKeys::EAlwaysActive;
  
    activation = MultimediaSharingSettings::ActivationSettingL();
    TBool operatorVariant = OperatorVariant();
    
    MUS_LOG1( "mus: [MUSAVA]     ActivationSetting returned %d", 
        activation )
    
    //VS off
    if ( activation != MusSettingsKeys::EAlwaysActive )
        {
        status = MMusAvaObserver::EMusActivationError;
        }
    //VS on
    else
        {
        //roaming
        if( regStatus == RMobilePhone::ERegisteredRoaming )
            {
            status = ManualActivationL();        
            }
        //other states including also not registered states, thus
        //failing of PDP context activation will be detected elsewhere
        else
            {
            status = MMusAvaObserver::EMusAvaStatusAvailable;   
            }
        }
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionAvailability::\
NetworkRegistrationAndSettingsL()" )
    return status;
    }


