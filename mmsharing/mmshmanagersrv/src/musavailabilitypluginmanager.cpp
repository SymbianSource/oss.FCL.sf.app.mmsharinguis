/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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


#include "musavailabilitypluginmanager.h"
#include "musavasettings.h"
#include "musunittesting.h"
#include "musavainterface.h"
#include "muslogger.h"
#include "musavailabilitypluginmanagerobserver.h"
#include "musmanagerservercommon.h"
#include "musindicatorapi.h"
#include "musapplicationmanager.h"

#include <badesca.h>
#include <bamdesca.h>
#include <e32property.h>


// CONSTANTS

// Max number of digits in unsigned 32 bit integer (TInt32)
const TUint KMaxInt32Length = 16;


using namespace MultimediaSharing;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusAvailabilityPluginManager* CMusAvailabilityPluginManager::NewL(
                          MMusAvailabilityPluginManagerObserver& aObserver,
                          CMusApplicationManager& aApplicationManager )
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::NewL()" )

    CMusAvailabilityPluginManager* self =
                          CMusAvailabilityPluginManager::NewLC( aObserver,
                                                                aApplicationManager );

    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::NewL()" )
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusAvailabilityPluginManager* CMusAvailabilityPluginManager::NewLC(
                          MMusAvailabilityPluginManagerObserver& aObserver,
                          CMusApplicationManager& aApplicationManager )
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::NewLC()" )

    CMusAvailabilityPluginManager* self =
                   new( ELeave ) CMusAvailabilityPluginManager( aObserver, 
                                                                aApplicationManager );

    CleanupStack::PushL( self );
    self->ConstructL();
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::NewLC()" )
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusAvailabilityPluginManager::~CMusAvailabilityPluginManager()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
             ~CMusAvailabilityPluginManager()" )
    delete iPlugin;
    delete iSessionParameters;
    delete iIndicator;
    iIndicator = NULL;
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
             ~CMusAvailabilityPluginManager()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::ConstructL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::ConstructL()" )
    iSessionParameters = new( ELeave ) CDesCArrayFlat( KParameterCount );
    iPlugin = CMusAvaInterface::NewL();
    iPlugin->SetObserver( *this );
   	iPlugin->SetSettingsObserver( *this );
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::ConstructL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusAvailabilityPluginManager::CMusAvailabilityPluginManager(
                          MMusAvailabilityPluginManagerObserver& aObserver,
                          CMusApplicationManager& aApplicationManager )
    : iObserver( aObserver ), 
      iApplicationManager( aApplicationManager )
    {
    iPluginStarted = EFalse;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusAvailabilityPluginManager::ApplicationAllowed()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        ApplicationAllowed()" )
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        ApplicationAllowed()" )
    return ( iPlugin->MandatoryAvailability()
        || iPlugin->AvailabilityState( EMusAvaNameRegistration ) == 
            EMusAvaStatusInProgress
        || iPlugin->AvailabilityState( EMusAvaNameRegistration ) == 
            EMusAvaStatusAvailable );
    }
// -----------------------------------------------------------------------------
// If minumum availability is available returning mandaroty level status
// and if mandatory level status is available returning extension level status
// -----------------------------------------------------------------------------
//
TMusAvailabilityStatus CMusAvailabilityPluginManager::Availability()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        Availability()" )
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        	Availability()" )
    return MapAvailabilities( iPlugin->AvailabilityStatus() );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::InvestigateAvailabilityL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
             InvestigateAvailabilityL()" )
    if( !iPluginStarted )
        {
        iPlugin->StartL();
        iPluginStarted = ETrue;
        }


    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
             InvestigateAvailabilityL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::PrepareForReceivedInviteL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
                 PrepareForReceivedInviteL()" )
                 
    iPlugin->PrepareForReceivedInviteL();
    
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
                     PrepareForReceivedInviteL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::InvitationReceivedL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        InvitationReceivedL()" )

    if( !iPluginStarted )
        {
        iPlugin->StartL();
        iPluginStarted = ETrue;
        }

    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        InvitationReceivedL()" )
    }

// -----------------------------------------------------------------------------
// CMusAvailabilityPluginManager::OptionsReceived
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::OptionsReceivedL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        OptionsReceivedL()" )

    if( !iPluginStarted )
        {
        iPlugin->StartL();
        iPluginStarted = ETrue;
        }

    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        OptionsReceivedL()" )
    }

// -----------------------------------------------------------------------------
// CMusAvailabilityPluginManager::LoadPluginL
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::LoadPluginL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        LoadPluginL()" )
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        LoadPluginL()" )
    }

// -----------------------------------------------------------------------------
// CMusAvailabilityPluginManager::UnloadPluginL
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::StopPluginL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        StopPluginL()" )
    if( iPluginStarted )
        {
        iPlugin->Stop();
        iPluginStarted = EFalse;
        }
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        StopPluginL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MDesCArray& CMusAvailabilityPluginManager::SessionParametersL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        SessionParametersL()" )
    // update the values
    UpdateSessionParametersL();

    // then pass them
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        SessionParametersL()" )
    return *iSessionParameters;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::ManualQueryL()
    {

    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        ManualQueryL()" )
  	MMusAvaSettings& settings = iPlugin->Settings();
  	
    MUS_LOG( "mus: [MUSSRV]    Status available" )
    if( !iIndicator )
        {
        MUS_LOG( "mus: [MUSSRV]    Create indicator" )
        iIndicator = CMusIndicatorApi::NewL( *this );
        if( iIndicator->ConfirmationQueryL( CMusIndicatorApi::EVsRoamingActivationQuery ) )
        	{
        	settings.SetManualActivation( MMusAvaSettings::EActivationAllowed );
        	iPlugin->StartL();
        	}
        else
        	{
        	settings.SetManualActivation( MMusAvaSettings::EActivationNotAllowed );
        	}
        delete iIndicator;
        iIndicator = NULL;
        }
     else
     	{
     	if( iIndicator->ConfirmationQueryL( CMusIndicatorApi::EVsRoamingActivationQuery ) )
        	{
        	settings.SetManualActivation( MMusAvaSettings::EActivationAllowed );
        	iPlugin->StartL();
        	}
        else
        	{
        	settings.SetManualActivation( MMusAvaSettings::EActivationNotAllowed );	
        	}	
     	}
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        ManualQueryL()" )  
    }

TBool CMusAvailabilityPluginManager::PossibleToLaunch()
    {
    return iPlugin->ExtensionAvailability() && 
    	iPlugin->AvailabilityStatus() == 
    		MMusAvaObserver::EMusAvaStatusAvailable;
    }
void CMusAvailabilityPluginManager::AvailabilityChanged( 
                                            TAvailabilityName /*aName*/, 
                                            TAvailabilityStatus aStatus )
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        AvailabilityChanged()" )
	TRAPD( err , iObserver.AvailabilityChangedL( MapAvailabilities( aStatus ) ) );
    if ( err )
        {
        // Not sure about what to do. Very rare situation.
        MUS_LOG1( "mus: [MUSSRV] AvailabilityChangedL() leave code: %d",err )
        }
    // Do some indicator things...
    TRAP( err, IndicateAvailabilityL() );
    if ( err )
        {
        // Not sure about what to do. Very rare situation.
        MUS_LOG1( "mus: [MUSSRV] IndicateAvailabilityL() leave code: %d",err )
        }

    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        AvailabilityChanged()" )	
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::AvailabilityError( 
                                            TAvailabilityName /*aName*/, 
                                            TAvailabilityStatus aStatus )
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        AvailabilityError()" )
    TRAPD( err , iObserver.AvailabilityChangedL( MapAvailabilities( aStatus ) ) );
    if ( err )
        {
        // Not sure about what to do. Very rare situation.
        MUS_LOG1( "mus: [MUSSRV] AvailabilityChangedL() leave code: %d",err )
        }

    // Do some indicator things...
    TRAP( err, IndicateAvailabilityL() );
    if ( err )
        {
        MUS_LOG1( "mus: [MUSSRV] IndicateAvailabilityL() leave code: %d",err )
        }

    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        AvailabilityError()" )	
    }

// -----------------------------------------------------------------------------
// From MMusAvaSettingsObserver
// -----------------------------------------------------------------------------
//
MMusAvaSettingsObserver::TApplicationState CMusAvailabilityPluginManager::
    ApplicationState()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        ApplicationState()" )
    MMusAvaSettingsObserver::TApplicationState ret = EApplicationStateNotDefined;   
    if ( iApplicationManager.ApplicationRunning() )
        {
        ret = EApplicationIsRunning;
        }
    else
        {
        ret = EApplicationNotRunning;
        }
    
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        ApplicationState()" )
    return ret;
    }

// -----------------------------------------------------------------------------
// From MMusAvaSettingsObserver
// -----------------------------------------------------------------------------
//
TBool CMusAvailabilityPluginManager::OptionAllowed()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        OptionAllowed()" )
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        OptionAllowed()" )
    // TBD: Check it out
    return iPlugin->AvailabilityState( MMusAvaObserver::EMusAvaOptionHandler )
        >= EMusAvaStatusNotExecuted;
    }

// -----------------------------------------------------------------------------
// From MMusAvaSettingsObserver
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::StartApplicationL( 
        MultimediaSharing::TMusUseCase aUseCase )
    {
    iObserver.StartSharingWithUseCaseL( aUseCase );        
    }

// -----------------------------------------------------------------------------
// From MMusIndicatorObserver.
// Starts live video sharing.
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::StartLiveSharingL()
    {
    iObserver.StartSharingWithUseCaseL( MultimediaSharing::EMusLiveVideo );
    }

// -----------------------------------------------------------------------------
// This functions cross-links TAvailabilityStatus and
// MultimediaSharing::TMusAvailabilityStatus
// -----------------------------------------------------------------------------
//
MultimediaSharing::TMusAvailabilityStatus
        CMusAvailabilityPluginManager::MapAvailabilities(
                            TAvailabilityStatus aStatus )
    {
    MultimediaSharing::TMusAvailabilityStatus status;
    switch( aStatus )
        {
        case EMusAvaStatusNotExecuted:
            {
            MUS_LOG( "mus: [MUSSRV]     that is \
                EMultimediaSharingNotAvailable" )
            status = EMultimediaSharingNotAvailable;
            break;
            }
        case EMusAvaFailureCode:
            {
            MUS_LOG( "mus: [MUSSRV]     that is \
                EMusAvaFailureCode" )
            status = EMultimediaFailureCode;
            break;
            }
        case EMusAvaStatusInProgress: // EMusAvaStatusRegistering:
            {
            MUS_LOG( "mus: [MUSSRV]     that is ESipRegistrationPending" )
            if ( iPlugin->CurrentAvailability() > EMusAvaNameRegistration )
                {
                // SIP registration already performed, we're available
                MUS_LOG( "mus: [MUSSRV]     that is \
                    EMultimediaSharingAvailable - a" )
                status = EMultimediaSharingAvailable;
                }
            else if ( 
                iPlugin->CurrentAvailability() == EMusAvaNameRegistration )
                {
                // SIP register pending
                MUS_LOG( "mus: [MUSSRV]     that is \
                    ESipRegistrationPending - a" )
                status = ESipRegistrationPending;
                }
            else
                {
                MUS_LOG( "mus: [MUSSRV]     that is \
                    EMultimediaSharingNotAvailable - a" )
                status = EMultimediaSharingNotAvailable;
                }
            break;
            }
        case EMusAvaStatusAvailable:
            {
            MUS_LOG( "mus: [MUSSRV]     that is EMultimediaSharingAvailable" )
            status = EMultimediaSharingAvailable;
            break;
            }
        case EMusAvaStatusNoSipProfile:
            {
            MUS_LOG( "mus: [MUSSRV]     that is EErrNoSipProfile" )
            status = EErrNoSipProfile;
            break;
            }
        case EMusAvaStatusNotRegistered:
            {
            MUS_LOG( "mus: [MUSSRV]     that is EErrSipRegistration" )
            status = EErrSipRegistration;
            break;
            }
        case EMusAvaStatusNoCall:
            {
            MUS_LOG( "mus: [MUSSRV]     that is EErrNoActiveCall" )
            status = EErrNoActiveCall;
            break;
            }
        case EMusAvaStatusCallOnHold:
            {
            MUS_LOG( "mus: [MUSSRV]     that is EErrCallOnHold" )
            status = EErrCallOnHold;
            break;
            }
        case EMusAvaOptionNotAvailable:
            {
            MUS_LOG( "mus: [MUSSRV]     that is EErrOptionAvailable" )
            status = ESipOptionsNotCapable;
            break;
            }
        case EMusAvaStatusConferenceCall:
            {
            MUS_LOG( "mus: [MUSSRV]     that is EErrConferenceCall" )
            status = EErrConferenceCall;
            break;
            }
        case EMusAvaStatusOptionsSent:
            {
            MUS_LOG( "mus: [MUSSRV]     that is ESipOptionsCapable" )
            status = ESipOptionsSent;
            break;
            }
        case EMusAvaStatusOptionsNotSent:
            {
            MUS_LOG( "mus: [MUSSRV]     that is ESipOptionsNotSent" )
            status = ESipOptionsNotSent;
            break;
            }
        case EMusAvaNetworkType:
            {
            MUS_LOG( "mus: [MUSSRV]     that is EErrNetwork" )
            status = EErrNetwork;
            break;
            }
        case EMusAvaRoamingError:
            {
            MUS_LOG( "mus: [MUSSRV]     that is EErrRoaming" )
            status = EErrRoaming;
            break;
            }
        case EMusAvaConnectionErr:
            {
            MUS_LOG( "mus: [MUSSRV]     that is EErrConnection" )
            status = EErrConnection;
            break;
            }
        case EMusActivationError:
            {
            MUS_LOG( "mus: [MUSSRV]     that is EErrActivation" )
            status = EErrActivation;
            break;
            }
        
        case EMusAvaManualActivation:
            {
            MUS_LOG( "mus: [MUSSRV]     that is Manual Activation" )
            status = EManualActivation;
            break;
            }

        case EMusAvaEdgeDtmStatusUnknown:
            {
            MUS_LOG( "mus: [MUSSRV]     Edge/DTM support" )
            status = EErrNetwork;
            break;
            }
        default:
            MUS_LOG( "mus: [MUSSRV]     that is unknown; returning \
                KErrUnknown" )
            status = ( TMusAvailabilityStatus ) KErrUnknown;
        }
    return status;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::UpdateSessionParametersL()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
             UpdateSessionParametersL()" )
    // clear the table
    iSessionParameters->Delete( 0, iSessionParameters->Count() );

    HBufC* telNumber = TelNumberLC();
    iSessionParameters->AppendL( *telNumber );
    CleanupStack::PopAndDestroy( telNumber );

    HBufC* sipAddresses = SipAddressesLC();
    iSessionParameters->AppendL( *sipAddresses );
    CleanupStack::PopAndDestroy( sipAddresses );

    MMusAvaSettings& settings = iPlugin->Settings();

    // append sip address proposal
    iSessionParameters->AppendL( settings.SipAddressProposal() );

    TBuf<KMaxInt32Length> contactIdBuf;
    contactIdBuf.Num( settings.ContactId() );

    // append contact id
    iSessionParameters->AppendL( contactIdBuf );
    // append video codec
    HBufC* videoCodecs = VideoCodecsLC();
    iSessionParameters->AppendL( *videoCodecs );
    CleanupStack::PopAndDestroy( videoCodecs );

    TBuf<KMaxInt32Length> sipProfileIdBuf;
    sipProfileIdBuf.Num( settings.SipProfileId() );

    MUS_LOG1("iSessionParameters -> SIP Profile ID = %d ",
        settings.SipProfileId() )

    // append sip profile id
    iSessionParameters->AppendL( sipProfileIdBuf );

    HBufC* contactName = ContactNameLC();
    iSessionParameters->AppendL( *contactName );
    CleanupStack::PopAndDestroy( contactName );

    TBuf<KMaxInt32Length> fastModeBuf;
    fastModeBuf.Num( settings.FastMode() );
    MUS_LOG1("iSessionParameters -> fast mode = %d ",
        settings.FastMode() )

    // append fast mode
    iSessionParameters->AppendL( fastModeBuf );
    
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
             UpdateSessionParametersL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC* CMusAvailabilityPluginManager::SipAddressesLC()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        SipAddressesLC()" )
    HBufC* sipAddresses = NULL;

    // We have to keep up with KMaxPropertySize since it is advertised through
    // RProperty to UI process.
    TBuf<RProperty::KMaxPropertySize> tbuf;
    _LIT( KComma, "," );

    MMusAvaSettings& settings = iPlugin->Settings();
    const MDesCArray& addresses = settings.SipAddresses();
    MUS_LOG1( "mus: [MUSSRV]    addresses.MdcaCount() %d", 
        addresses.MdcaCount() )        
    
    for( TInt i=0;i<addresses.MdcaCount();i++ )
        {             
         // Check enough space available in tbuf
        if ( tbuf.MaxSize()-tbuf.Size() >
             addresses.MdcaPoint(i).Size()+KComma().Size())
            {
            if( i > 0) tbuf.Append( KComma() );                
            tbuf.Append( addresses.MdcaPoint( i ) );                
            }
        else
            {
            // if no space just break and do not continue further.
            break;
            }            
        }
    sipAddresses = tbuf.AllocLC();

    MUS_LOG_TDESC( "mus: [MUSSRV]       SIP Address: ", (*sipAddresses) );
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        SipAddressesLC()" )

    return sipAddresses;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC* CMusAvailabilityPluginManager::VideoCodecsLC()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        VideoCodecsLC()" )
    HBufC* videoCodecs = NULL;

    // We have to keep up with KMaxPropertySize since it is advertised through
    // RProperty to UI process.
    TBuf<RProperty::KMaxPropertySize> tbuf;
    _LIT( KComma, "," );

    MMusAvaSettings& settings = iPlugin->Settings();
    const MDesCArray& codecs = settings.VideoCodecs();
    MUS_LOG1( "mus: [MUSSRV]    addresses.MdcaCount() %d", 
        codecs.MdcaCount() )        
    
    for( TInt i=0;i<codecs.MdcaCount();i++ )
        {             
         // Check enough space available in tbuf
        if ( tbuf.MaxSize()-tbuf.Size() >
             codecs.MdcaPoint(i).Size()+KComma().Size())
            {
            if( i > 0) tbuf.Append( KComma() );                
            tbuf.Append( codecs.MdcaPoint( i ) );                
            }
        else
            {
            // if no space just break and do not continue further.
            break;
            }            
        }
    videoCodecs = tbuf.AllocLC();

    MUS_LOG_TDESC( "mus: [MUSSRV]       Video codecs: ", (*videoCodecs) );
    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        VideoCodecsLC()" )

    return videoCodecs;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC* CMusAvailabilityPluginManager::TelNumberLC()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        TelNumberLC()" )

    MMusAvaSettings& settings = iPlugin->Settings();

    HBufC* telNumber = settings.TelNumber().AllocLC();

    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        TelNumberLC()" )

    return telNumber;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC* CMusAvailabilityPluginManager::ContactNameLC()
    {
    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        ContactNameLC()" )

    MMusAvaSettings& settings = iPlugin->Settings();

    HBufC* contactName = settings.ContactName().AllocLC();

    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        ContactNameLC()" )

    return contactName;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusAvailabilityPluginManager::IndicateAvailabilityL()
    {

    MUS_LOG( "mus: [MUSSRV]  -> CMusAvailabilityPluginManager::\
        IndicateAvailabilityL()" )

    if( PossibleToLaunch() )
        {
        MUS_LOG( "mus: [MUSSRV]    Status available" )
        if( !iIndicator )
            {
            MUS_LOG( "mus: [MUSSRV]    Create indicator" )
            iIndicator = CMusIndicatorApi::NewL( *this );
            iIndicator->IndicateAvailabilityL();
            }
        iApplicationManager.SetIndicatorStatusL( ETrue );
        }
    else
        {
        MUS_LOG( "mus: [MUSSRV]    Not available" )
        iApplicationManager.SetIndicatorStatusL( EFalse );
        delete iIndicator;
        iIndicator = NULL;
        }

    MUS_LOG( "mus: [MUSSRV]  <- CMusAvailabilityPluginManager::\
        IndicateAvailabilityL()" )
    }

