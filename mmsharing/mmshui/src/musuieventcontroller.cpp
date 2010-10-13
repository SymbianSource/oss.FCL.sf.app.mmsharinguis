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
* Description:  Application's UI class.
*
*/



#include "musuieventcontroller.h"
#include "musuieventobserver.h"
#include "musuisharingobserver.h"
#include "musuipropertywatch.h"
#include "musuicallbackservice.h"
#include "musuiactivitymanger.h"
#include "musuifileutil.h"
#include "musuidialogutil.h"
#include "musuicontactutil.h"
#include "mussessionproperties.h"
#include "musuibitmapdecoder.h"
#include "muscommon.h"
#include "mussettings.h"
#include "musenguriparser.h"
#include "musuimmcmonitor.h"
#include "musresourceproperties.h"
#include "musuiresourcehandler.h"
#include "musui.hrh"
#include "muslogger.h" // debug logging
#include "musuigeneralview.h"

#include "musengmcesession.h"

#include <musui.rsg>

#include <fbs.h>
#include <e32property.h>

#include <StringLoader.h>
#include <pathinfo.h>
#include <featmgr.h>
#include <bldvariant.hrh> // for feature definitions
#include <pathconfiguration.hrh>

#include <hlplch.h> // HlpLauncher
#include <AknUtils.h>
#include <telmicmutestatuspskeys.h>

using namespace MusSettingsKeys;
using namespace NMusSessionApi;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiEventController::~CMusUiEventController()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::~CMusUiEventController" );
    delete iStatusPropertyWatch;    
    delete iContactName;
    delete iTelNumber;
    delete iVideoCodec;
    delete iDialogPrompt;
    delete iCallbackService;
    delete iMmcMonitor;
    delete iActivityManager;
    delete iMicMuteStatusPropertyWatch;
    FeatureManager::UnInitializeLib();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::~CMusUiEventController" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::ConstructL()
    {
   MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::ConstructL" );

    TInt error( KErrNotFound );

    FeatureManager::InitializeLibL();

    iCallbackService = CMusUiCallbackService::NewL( *this );

    // read ps properties (no need to read again later)

    TRAP( error, iVideoCodec = CMusUiPropertyWatch::ReadDescPropertyL(
                            KVideoCodecs ) );
    if ( error != KErrNone )
        {
        iVideoCodec = KNullDesC().AllocL();
        }

    TRAP( error, iTelNumber = CMusUiPropertyWatch::ReadDescPropertyL(
                            KTelNumber ) );
    if ( error != KErrNone )
        {
        iTelNumber = KNullDesC().AllocL();
        }

    TRAP( error, iSipProfileId = CMusUiPropertyWatch::ReadIntPropertyL(
                            NMusSessionApi::KSipProfileId ) );
    if ( error != KErrNone )
        {
        iSipProfileId = KErrNotFound;
        }

    TRAP( error,
          iContactId = CMusUiPropertyWatch::ReadIntPropertyL( KContactId ) );
    if ( error != KErrNone )
        {
        iContactId = KErrNotFound;
        }

    TRAP( error, iContactName = CMusUiPropertyWatch::ReadDescPropertyL(
                            KContactName ) );

    if ( error != KErrNone )
        {
        iContactName = KNullDesC().AllocL();
        }
    
    // follow session status property
    iStatusPropertyWatch = CMusUiPropertyWatch::NewL(
                            *this,
                            NMusSessionApi::KCategoryUid,
                            KStatus );

    iMmcMonitor = CMusUiMmcMonitor::NewL( *this );

    iResourceHandler = iEventObserver.ResourceHandler();
    __ASSERT_ALWAYS( iResourceHandler, User::Leave( KErrNotFound ) );
    
    //Mic mute status property
    iMicMuteStatusPropertyWatch = CMusUiPropertyWatch::NewL(
							*this,
							KPSUidTelMicrophoneMuteStatus,
							KTelMicrophoneMuteState );

    // start monitoring activity
    iActivityManager = CMusUiActivityManager::NewL( KMusBacklightTimeOut );
    iActivityManager->Reset();

    // check if operator specific functionality is needed  
    iOperatorSpecificFunctionality = 
        ( MultimediaSharingSettings::OperatorVariantSettingL() == 
                                                    EOperatorSpecific );

    iShutdownState = EMusUiShutdownNotDefined;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiEventController::CMusUiEventController(
                            MMusUiEventObserver& aEventObserver,
                            MMusUiSharingObserver& aSharingObserver )
    : iEventObserver( aEventObserver ),
      iSharingObserver( aSharingObserver ),
	  iForeground( ETrue ),
	  iShowDialog( EFalse ),
	  iActiveCall( ETrue )
	  
    {
    }


// -----------------------------------------------------------------------------
// Called by CMusUiPropertyWatch, when resource key values change in P&S
// Sets the member values common to all sharing types.
// -----------------------------------------------------------------------------
//
void CMusUiEventController::PropertyChanged( const TUint aKey,
                                                     const TInt aValue )
    {
    MUS_LOG2( "mus: [MUSUI ] -> CMusUiEventController::ResourcePropertyChanged:\
              aKey: [%u] aValue: [%d]", aKey, aValue );
    switch( aKey )
        {
    	case KTelMicrophoneMuteState:
    		{
		  if ( aValue ==  EPSTelMicMuteOff)
			  {
			  iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarUnmute,
												   EMusuiCmdToolbarMute,ETrue );
			  }
		  else if ( aValue == EPSTelMicMuteOn )
			  {
			  iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarMute,
												 EMusuiCmdToolbarUnmute,ETrue );
			  }
			break;
    		}
        case KStatus:
            {
            TRAP_IGNORE(
                    HandleChangedStatusL( ( TMusAvailabilityStatus ) aValue ) );
            break;
            }
        default:
            {
            break;
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::ResourcePropertyChanged" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::HandlePropertyError( const TInt aReason )
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiEventController::HandleError [%d]",
                            aReason );
    
    HandleError( aReason );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::HandleError" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::HandleAsyncEventL( TMusUiAsyncEvent aEventId )
    {
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiEventController::HandleAsyncEventL [%d]",
              aEventId );
    
    switch ( aEventId )
        {
        case EMusUiAsyncHandleExit:
            {
            MUS_LOG( "mus: [MUSUI ]   CMusUiEventController::\
                                    HandleAsyncEventL: EMusUiAsyncHandleExit" );
            HandleExitL();
            break;
            }
        default:
            {
            break;
            }
        }
    MUS_LOG( "mus: [MUSUI ] <- CMusUiEventController::HandleAsyncEventL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusUiEventController::HandleAsyncError( TInt aError )
    {
    iEventObserver.HandleError( aError ); // Stop the execution
    
    return KErrNone;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::MmcRemoved()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::MmcRemoved" );
    TRAP_IGNORE( MusUiDialogUtil::ShowInformationDialogL( 
                            R_MUS_LIVE_SHARING_VIEW_NOTE_SHARING_ENDED ) )
                            
    iEventObserver.HandleExit();
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::MmcRemoved" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::SessionEstablished()
    {
    TRAPD( error, iSharingObserver.ShowStatusPaneTitleL() )
    
    if ( error != KErrNone )    
        {
        iEventObserver.HandleError( error );
        }

    if ( MUS_NO_TOOLBAR )
        {
        TRAPD( error, iSharingObserver.EnableMiddleSoftkeyL( IsLoudSpeakerEnabled() ? 
                            R_MUSUI_VIEW_OPS_MSK_IHF_OFF :
                            R_MUSUI_VIEW_OPS_MSK_IHF_ON ));
        if ( error != KErrNone )    
            {
            iEventObserver.HandleError( error );
            }        
        }
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::AudioRoutingChanged( TBool aShowNote )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::AudioRoutingChanged" );
    
    if ( aShowNote )
        {
        if ( IsLoudSpeakerEnabled() )
            {
            MUS_LOG( "mus: [MUSUI ]     show activated note" );
            TRAP_IGNORE( 
            	MusUiDialogUtil::ShowGlobalInformationDialogL( R_MUS_NOTE_HF_ACTIVATED ) )
            }
        }
    
    iSharingObserver.RefreshAudioRoutingToolbarButton();
      
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::AudioRoutingChanged" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::AudioRouteChangeAllowed() const
    {
    // Do not allow audio routing if call has ended
    return iActiveCall;
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::VolumeChanged( TInt aVolume, TBool aAudioRouteChanged )
    {
    // Set Volume:
    //If volume changed due to audio route change, we want to update volume
    //popup control only if it is already visible
    TBool onlyIfVisible = aAudioRouteChanged;
    TRAP_IGNORE( iSharingObserver.ActivateVolumeControlL(aVolume, onlyIfVisible) );
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::SetRect(const TRect& aRect)
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::SetRect" );
    
    if ( EngineSession() )
        {
        TRAPD( err, EngineSession()->SetRectL( aRect ) );
        if ( err != KErrNone )
            {
            iEventObserver.HandleError( err );
            }
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::SetRect" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusUiEventController::MusContactName()
    {
    return *iContactName;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusUiEventController::MusVideoCodecValue()
    {
    return *iVideoCodec;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusUiEventController::MusTelNumberValue()
    {
    MUS_LOG_TDESC("mus: [MUSUI ] <> CMusUiEventController::MusTelNumberValue()",
                   iTelNumber->Des() )
    return *iTelNumber;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusUiEventController::SessionTimeFormatted(
                        const TTimeIntervalSeconds& aSeconds )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::SessionTimeFormatted" );
    iTimeBuffer.SetLength( 0 );
    TLocale locale;
    _LIT( KZeroSymbol, "0" );
    TInt sessionHours( 0 );
    TInt sessionMinutes( 0 );
    TInt sessionSeconds( aSeconds.Int() );
    if ( aSeconds.Int() > ( KMusOneMinuteInSeconds - 1 ) )
        {
        sessionMinutes = (int)(aSeconds.Int() / KMusOneMinuteInSeconds );
        sessionSeconds = sessionSeconds - ( sessionMinutes * KMusOneMinuteInSeconds );
        }
    if ( sessionMinutes > ( KMusOneMinuteInSeconds - 1 ) )
        {
        sessionHours = (int)( sessionMinutes / KMusOneMinuteInSeconds );
        sessionMinutes = sessionMinutes - ( sessionHours * KMusOneMinuteInSeconds );
        }
    if ( sessionHours != 0 )
        {
        iTimeBuffer.AppendNum( sessionHours );
        iTimeBuffer.Append( locale.TimeSeparator( KMusLocaleHoursAndMinutesSeparator ) );
        if ( sessionMinutes < KMusNumberValueTen )
            {
            iTimeBuffer.Append( KZeroSymbol );
            }
        }

    iTimeBuffer.AppendNum( sessionMinutes );
    iTimeBuffer.Append( locale.TimeSeparator( KMusLocaleMinutesAndSecondsSeparator ) );
    if ( sessionSeconds < KMusNumberValueTen )
        {
        iTimeBuffer.Append( KZeroSymbol );
        }
    iTimeBuffer.AppendNum( sessionSeconds );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::SessionTimeFormatted" );
    return iTimeBuffer;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::SaveContactQueryL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::SaveContactQueryL" );
    if ( !MusUiContactUtil::ShareViewFieldExistsL( *iEventObserver.EikonEnv(), 
                                                   iContactId ) )
        {
        delete iDialogPrompt;
        iDialogPrompt = NULL;
        iDialogPrompt = StringLoader::LoadL( R_MUS_VIEW_SAVE_ADDRESS_QUERY_TXT );
        iEventObserver.ShowAsyncConfirmationQueryDialogL( *iDialogPrompt );
        }
    else
        {
        AsyncQueryDialogAcceptedL( EFalse );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::SaveContactQueryL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::HandleError( const TInt aError )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiEventController::HandleError %d", 
              aError );

    iEventObserver.HandleError( aError );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::HandleError" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::ExitOccured()
    {
    return ( iShutdownState != EMusUiShutdownNotDefined );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::AudioRoutingCanBeChanged()
    {
    return ( EngineSession() && EngineSession()->AudioRoutingCanBeChanged() );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::IsLoudSpeakerEnabled()
    {
    return ( EngineSession() && EngineSession()->IsLoudSpeakerEnabled() );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::DeviceHasDedicatedVolumeKeys()
    {
    return FeatureManager::FeatureSupported( KFeatureIdSideVolumeKeys );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::HandleExitL( TBool aTerminateCall )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::HandleExitL" );
    if ( iShutdownState == EMusUiShutdownNotDefined )
        {
        iShutdownState = EMusUiShutdownStarted;
        
        if ( iStatusPropertyWatch )
            {
            iStatusPropertyWatch->Cancel();    
            }
        
        if ( iMmcMonitor )
            {
            iMmcMonitor->Cancel();    
            }
        
        if ( iActivityManager )
            {
            iActivityManager->Cancel();    
            }
        
        if ( iCallbackService )
            {
            iCallbackService->Cancel();    
            }

        iEventObserver.SetToolbarVisibility( EFalse );
        
        // This must be done before showing the end note, because showing of
        // note may cause the session to continue for extra few seconds.
        DeleteEngineSession();
        if ( iShowDialog )
        	{
            MusUiDialogUtil::ShowGlobalInformationDialogL( 
            		                 R_MUS_NOTE_HF_DEACTIVATED );
        	}
        // Sharing Ended note
        MusUiDialogUtil::ShowGlobalInformationDialogL( 
                                R_MUS_LIVE_SHARING_VIEW_NOTE_SHARING_ENDED );
        
        if( aTerminateCall )
			{
			iEventObserver.TerminateCall();
			}

        // TODO: Continue Recording query, if sharing live video and receiving end
        //    terminates the sharing session

        ExitProcedureL( EFalse );

        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::HandleExitL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::ExitProcedureL( TBool aUserAcceptance )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::ExitProcedureL" );
    
    iSharingObserver.DismissMenuBar();
    
    switch ( iShutdownState )
        {
        case EMusUiShutdownStarted: 
            {
            MUS_LOG( "mus: [MUSUI ]     ExitProcedureL: EMusUiShutdownStarted" );
            // No general functionality, skip the state
            iShutdownState = EMusUiShutdownVideoSaved;
            ExitProcedureL( EFalse );
            break;
            }
        case EMusUiShutdownVideoSaved:
            {
            MUS_LOG( "mus: [MUSUI ]     ExitProcedureL: EMusuiShutdownVideoSaved" );
            iEventObserver.HandleExit();
            break;
            }

        case EMusUiShutdownContactSavingQueried:
            {
            MUS_LOG( "mus: [MUSUI ]     ExitProcedureL: EMusUiShutdownContactSavingQueried" );
            // Stop app execution
            if ( aUserAcceptance )
                {
                if ( iResourceHandler->RequestKeypadL( EFalse ) )
                	{
                		MusUiContactUtil::ActivateContactViewL( 
                                                    *iEventObserver.EikonEnv(), 
                                                    iContactId, 
                                                    TypedAddress(), 
                                                    MusTelNumberValue() );
                	}
                }
            iEventObserver.HandleExit();
            break;
            }
        case EMusUiShutdownOver:
            {
            iEventObserver.HandleExit();
            break;
            }
        case EMusUiShutdownNotDefined:
        default:
            {
            break;
            }
        }
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::ExitProcedureL" );
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::AsyncQueryDialogAcceptedL( TBool aAccepted )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::AsyncQueryDialogAcceptedL" );
    
    if ( ExitOccured() )
        {
        ExitProcedureL( aAccepted );
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::AsyncQueryDialogAcceptedL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::ConnectionEstablished() const
    {
    return iConnectionEstablished;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::SetConnectionInitialized( 
    TBool aConnectionInitialized )
    {
    MUS_LOG1( 
        "mus: [MUSUI ]  -> CMusUiEventController::SetConnectionInitialized ( %d )", 
        aConnectionInitialized );
    
    iConnectionInitialized = aConnectionInitialized;
    
    if ( !iForeground && !ExitOccured() )
        {
        TRAP_IGNORE( EnableDisplayL(EFalse) )
        }
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::ConnectionInitialized() const
    {
    return iConnectionInitialized;
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::EnableDisplayL( TBool aEnable )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::EnableDisplayL" );
    if ( EngineSession() )
        {
        EngineSession()->EnableDisplayL( aEnable );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::EnableDisplayL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::ChangeOrientationL( 
                        CAknAppUiBase::TAppUiOrientation aOrientation )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::ChangeOrientationL" );
    iEventObserver.SwitchOrientationL( aOrientation );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::ChangeOrientationL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::IsDisplayEnabledL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::IsDisplayEnabledL" );
    TBool ret = EFalse;
    if ( EngineSession() )
        {
        ret = EngineSession()->IsDisplayEnabledL();
        }
    MUS_LOG1( "mus: [MUSUI ]  <- CMusUiEventController::IsDisplayEnabledL, %d", ret );
    return ret;   
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::IsAudioRoutingLoudSpeaker()
	{
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::IsAudioRoutingLoudSpeaker" );
    TBool ret = EFalse;
    if ( EngineSession() )
        {
        ret = EngineSession()->IsAudioRoutingLoudSpeaker();
        }
    MUS_LOG1( "mus: [MUSUI ]  <- CMusUiEventController::IsAudioRoutingLoudSpeaker, %d", ret );
    return ret;   
	}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::ToolbarVolumeSelected()
    {
    return iToolbarVolumeSelected;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::IsMicMutedL()
    {
    __ASSERT_ALWAYS( EngineSession(), User::Leave( KErrNotReady ) );
    
    return EngineSession()->IsMicMutedL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::ManualAddressTyped() const
    {
    return EFalse;
    }    


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::HandleForegroundEventL( TBool aForeground )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::HandleForegroundEventL" );
    
    iForeground = aForeground;

    if ( ConnectionInitialized() &&  !ExitOccured() )
        {
        EnableDisplayL( aForeground );
        
        // Change toolbar according to a real state. Other items than speaker
        // and mic states cannot be changed from phone app and thus there is
        // no need to check them
        if ( aForeground )
            {
            if ( MUS_NO_TOOLBAR )
                {
                iSharingObserver.EnableMiddleSoftkeyL( IsLoudSpeakerEnabled() ?
                                            R_MUSUI_VIEW_OPS_MSK_IHF_OFF :
                                            R_MUSUI_VIEW_OPS_MSK_IHF_ON );
                }
            else
                {
                if ( IsMicMutedL() )
                    {
                    iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarMute,
                                                            EMusuiCmdToolbarUnmute,
                                                            EFalse );
                    }
                else
                    {
                    iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarUnmute,
                                                            EMusuiCmdToolbarMute,
                                                            EFalse );
                    }
                
                if ( IsLoudSpeakerEnabled() )
                    {
                    iSharingObserver.ReplaceToolbarCommand( 
                                                        EMusuiCmdToolbarSpeakerOn,
                                                        EMusuiCmdToolbarSpeakerOff,
                                                        EFalse );
                    }
                else
                    {
                    iSharingObserver.ReplaceToolbarCommand( 
                                                        EMusuiCmdToolbarSpeakerOff,
                                                        EMusuiCmdToolbarSpeakerOn,
                                                        EFalse );
                    }          
               if ( ConnectionEstablished() )
                    {
                    iEventObserver.SetToolbarVisibility( aForeground );
                    }
               
                }            
            }
            
        }
    
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::HandleForegroundEventL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiEventController::IsForeground() const
    {
    return iForeground;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiEventController::OfferToolbarEventL( TInt aCommand )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiEventController::OfferToolbarEventL [%d]",
              aCommand );
    
    __ASSERT_ALWAYS( EngineSession(), User::Leave( KErrNotReady ) );

    switch ( aCommand )
        {
        case EMusuiCmdToolbarVolume:
            {
            iToolbarVolumeSelected = !iToolbarVolumeSelected;
            iSharingObserver.HighlightSelectedToolbarItem( 
                                                EMusuiCmdToolbarVolume );
            iSharingObserver.ActivateVolumeControlL( 
                                            EngineSession()->VolumeL() );
            break;
            }  
        case EMusuiCmdToolbarMute:
            {       
            EngineSession()->MuteMicL( ETrue );
            iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarMute,
                                                    EMusuiCmdToolbarUnmute,
                                                    ETrue );
            break;
            }
        case EMusuiCmdToolbarUnmute:
            {
            EngineSession()->MuteMicL( EFalse );
            iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarUnmute,
                                                    EMusuiCmdToolbarMute,
                                                    ETrue );
            break;
            }
        case EMusuiCmdToolbarSpeakerOff:
            {
            EngineSession()->EnableLoudspeakerL( ELoudspeakerDisabled, EFalse );
            // Toolbar will be changed when asynchronous request is completed
            break;
            }
        case EMusuiCmdToolbarSpeakerOn:
            {
            EngineSession()->EnableLoudspeakerL( ELoudspeakerEnabled, EFalse );
            // Toolbar will be changed when asynchronous request is completed
            break;     
            }
        default:    // Never should be the case
            {
            HandleExitL();
            break;
            }
                
        }
    
    MUS_LOG1( "mus: [MUSUI ]  <- CMusUiEventController::OfferToolbarEventL [%d]",
              aCommand );
                                                            
    }


// -----------------------------------------------------------------------------
// Handles commands common to all sharing types.
// -----------------------------------------------------------------------------
//
void CMusUiEventController::HandleCommandL( TInt aCommand )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::HandleCommandL" );
    
    switch ( aCommand )
        {
        case EMusuiCmdViewVolumeUp:
            {
            // Set Volume:
            __ASSERT_ALWAYS( EngineSession(), User::Leave( KErrNotReady ) );
            EngineSession()->VolumeUpL();
            // Status pane update will be done later. When we get 
			// VolumeChanged() callback
            iSharingObserver.ActivateVolumeControlL( 
                                            EngineSession()->VolumeL() );
            break;
            }
        case EMusuiCmdViewVolumeDown:
            {
            // Set Volume:
            __ASSERT_ALWAYS( EngineSession(), User::Leave( KErrNotReady ) );
            EngineSession()->VolumeDownL();
            // Status pane update will be done later. When we get 
			// VolumeChanged() callback
            iSharingObserver.ActivateVolumeControlL( 
                                            EngineSession()->VolumeL() );            
            break;
            }
        case EMusuiCmdViewVolumeChanged:
            {
            __ASSERT_ALWAYS( EngineSession(), User::Leave( KErrNotReady ) );
            TInt volumeFromEngine = EngineSession()->VolumeL();
            TInt volumeFromUi = iEventObserver.GetUiVolumeValue();
            if ( volumeFromUi != volumeFromEngine )
                {
                EngineSession()->SetVolumeL( volumeFromUi );
                }
            break;
            }
           
        case EMusuiCmdViewLoadspeaker:
            {
            EngineSession()->EnableLoudspeakerL( ELoudspeakerEnabled, ETrue );
            break;
            }
            
        case EMusuiCmdViewHandset:
            {
            EngineSession()->EnableLoudspeakerL( ELoudspeakerDisabled, ETrue );
            break;
            }

        case EMusuiCmdViewMicrophoneMute:
            {
            EngineSession()->MuteMicL( ETrue );
            break;            
            }
            
        case EMusuiCmdViewMicrophoneUnmute:
            {
            EngineSession()->MuteMicL( EFalse );
            break;
            }
           
        case EMusuiCmdViewEndActiveCall:
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiReceiveController::ConstructL:\
                                    EMusuiCmdViewEndActiveCall" );
            HandleExitL( ETrue );
            break;
            }
        case EMusuiGenCmdExit:
        case EAknSoftkeyExit:
            {
            HandleExitL();
            break;
            }

        case EAknCmdHelp:
            {
            if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                        iEventObserver.EikonEnv()->WsSession(),
                        iEventObserver.EikonEnv()->AppUi()->AppHelpContextL() );
                }
            break;
            }

        default:
            {
            //NOP
            break;
            }
        }
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::HandleCommandL" );
    }
    

// -----------------------------------------------------------------------------
// Handles status changes common to all sharing types.
// -----------------------------------------------------------------------------
//
void CMusUiEventController::HandleChangedStatusL(
                        TMusAvailabilityStatus aStatus )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiEventController::HandleChangedStatusL" );
    switch( aStatus )
        {
        case EErrNoActiveCall:
            {
            MUS_LOG( "mus: [MUSUI ]     EErrNoActiveCall" );
            iActiveCall = EFalse;
            HandleExitL();
            break;
            }
        case EErrCallOnHold:
        case EErrConferenceCall: 
        case EMultimediaSharingNotAvailable:
            {
            MUS_LOG( "mus: [MUSUI ]\
    EErrCallOnHold, EErrConferenceCall, EMultimediaSharingNotAvailable" );
            HandleExitL();
            break;
            }
        case EErrSipRegistration:
            {
            MUS_LOG( "mus: [MUSUI ]     EErrSipRegistration" );
            MusUiDialogUtil::ShowInformationDialogL( 
                                    R_MUS_LIVE_SHARING_VIEW_NOTE_SERVICE_N_A );
            HandleExitL();
            break;
            }
        case EErrRoaming:
            {
            MUS_LOG( "mus: [MUSUI ]     EErrRoaming" );
            HandleExitL();
            break;
            }
        case EErrNetwork:
            {
            MUS_LOG( "mus: [MUSUI ]     EErrNetwork" );
            MusUiDialogUtil::ShowInformationDialogL( 
                                    R_MUS_LIVE_SHARING_VIEW_NOTE_SERVICE_N_A );
            HandleExitL();
            break;
            }
        case EErrConnection:
            {
            MUS_LOG( "mus: [MUSUI ]     EErrConnection" );
            MusUiDialogUtil::ShowInformationDialogL( 
                                    R_MUS_LIVE_SHARING_VIEW_NOTE_ONLY_3G );
            HandleExitL();
            break;
            }

        case EErrServerShutDown:
            {
            MUS_LOG( "mus: [MUSUI ]     EErrServerShutDown" );
            HandleExitL();
            break;
            }

        default:
            {
            MUS_LOG( "mus: [MUSUI ]     Default" );
            // TODO
            break;
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiEventController::HandleChangedStatusL" );
    }


