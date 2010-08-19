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



#include "musuilivesharingcontroller.h"
#include "musuicallbackservice.h"
#include "musuieventobserver.h"
#include "musuisharingobserver.h"
#include "musuisendobserver.h"
#include "musuilivesharingobserver.h"
#include "musuidialogutil.h"
#include "musenglivesession.h"
#include "musuiresourcehandler.h"
#include "musui.hrh"
#include "mussettings.h"
#include "mussettingskeys.h"
#include "muslogger.h" // debug logging
#include "musuigeneralview.h"
#include "musuiactivetimer.h"

#include <musui.rsg>
#include <avkon.hrh>
#include <StringLoader.h>

#include <AknUtils.h>

using namespace NMusResourceApi;
using namespace MusSettingsKeys;

const TInt KMusUiPauseResumeGuardPeriod = 500000;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiLiveSharingController* CMusUiLiveSharingController::NewL(
                        MMusUiEventObserver& aEventObserver,
                        MMusUiSharingObserver& aSharingObserver,
	                    MMusUiSendObserver& aSendObserver,
                        MMusUiLiveSharingObserver& aLiveObserver,
                        const TRect& aRect )
    {
    CMusUiLiveSharingController* self =
             new( ELeave ) CMusUiLiveSharingController( aEventObserver,
                                                        aSharingObserver,
                                                        aSendObserver,
                                                        aLiveObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiLiveSharingController::~CMusUiLiveSharingController()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::~CMusUiLiveSharingController" );
    delete iSession;
    delete iPauseResumeGuardTimer;
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::~CMusUiLiveSharingController" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiLiveSharingController::CMusUiLiveSharingController(
                            MMusUiEventObserver& aEventObserver,    
                        	MMusUiSharingObserver& aSharingObserver,
	                        MMusUiSendObserver& aSendObserver,
	                        MMusUiLiveSharingObserver& aLiveObserver )
	: CMusUiSendController( aEventObserver, aSharingObserver, aSendObserver ),
	  iLiveObserver( aLiveObserver )
    {
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::ConstructL( const TRect& aRect )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::ConstructL" );
    
    CMusUiSendController::ConstructL(); // Base class construction

    iSession = CMusEngLiveSession::NewL( VideoFileNameL(),
                                         aRect,
                                         *this,
                                         *this,
                                         *this,
                                         iSipProfileId );

    iSession->SetAudioRoutingObserver( this );
    iSession->SetVolumeChangeObserver( this );
        
    if ( iSession->AudioRoutingCanBeChanged() && !iSession->IsAudioRoutingHeadset() )
        {
        iSession->EnableLoudspeakerL( ELoudspeakerEnabled, ETrue );
        }
        
    if ( iVideoCodec && iVideoCodec->Length() > 0 )
        {
        iSession->SetSupportedVideoCodecListL( *iVideoCodec );
        }
    
    iPauseResumeGuardTimer = CMusUiActiveTimer::NewL( NULL );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::ConstructL" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::PlayL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::PlayL" );
    
    CMusUiSendController::PlayL();
    iEventObserver.ShowNaviPaneIconL( 
                    (!iDiskFull) ? RecordIcon() : EMusUiNaviIconRecordNot );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::PlayL" );
    }


// -----------------------------------------------------------------------------
// When orientation changed , Camera instances has to be recreated inorder
// to receive proper orientated frames.
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::RefreshCameraOrientationL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::RefreshCameraOrientationL" );

    if ( EngineSession() ){
        EngineSession()->RefreshOrientationL();
    }
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::RefreshCameraOrientationL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::PauseL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::PauseL" );
    
    CMusUiSendController::PauseL();
    iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconPause );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::PauseL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::DeleteEngineSession()
    {
    CMusUiSendController::DeleteEngineSession();
    if ( iSession && iSession->IsAudioRoutingLoudSpeaker() )
	    {
	    iShowDialog = ETrue; 
	    }
    delete iSession;
    iSession = NULL;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceOutSession* CMusUiLiveSharingController::EngineOutSession()
    {
    return iSession; // Live session is CMusEngMceOutSession
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::InviteL( const TDesC& aRecipient )
    {
    MUS_LOG_TDESC( "mus: [MUSUI ]    -> CMusUiLiveSharingController::InviteL:",
                            aRecipient );

    // Registration must not be pending in this phase
    __ASSERT_ALWAYS( !iSipRegistrationPending, 
                     iEventObserver.HandleError( KErrNotReady ) );
    
    delete iAddress;
    iAddress = NULL;
    iAddress = aRecipient.AllocL();

    
    TRAPD( err, iSession->InviteL( aRecipient ) );
    MUS_LOG( "mus: [MUSUI ]     CMusUiLiveSharingController::InviteL: After TRAPD" );

    // If address is in wrong format, Manual Address Entry Dialog
    // is displayed
    if ( err != KErrNone )
        {
        // Display the error message
        DismissWaitDialog();
        if ( !ExitOccured() )
            {
            MusUiDialogUtil::ShowGlobalErrorDialogL( 
                                R_MUS_LIVE_SHARING_VIEW_NOTE_INVITE_ERROR );
            }
        if ( ( ++iTriedInvitations < 2 )  && ( err == KErrArgument ) )
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiLiveSharingController::InviteL: iTriedInvitations < 2" );
            iManualAddressTyped = ETrue;
            iResourceHandler->RequestKeypadL( ETrue );
            iSendObserver.ManualAddressEntryL( *iRemoteSipAddressProposal );
            return;
            }
        else
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiLiveSharingController::InviteL: ++iTriedInvitations > 1" );
            HandleExitL();
            return;
            }
        }
    
    SetConnectionInitialized( ETrue );
        
    ShowInvitingWaitDialogL();
    
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::InviteL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::HandleSliderValueChangeL( TInt aNewLevel )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiLiveSharingController::HandleSliderValueChangeL(%d)",
              aNewLevel )

    __ASSERT_ALWAYS( !ExitOccured(), User::Leave( KErrDied ) );
    __ASSERT_ALWAYS( iConnectionEstablished, User::Leave( KErrNotReady ) );
    
    if ( iToolbarZoomSelected )
        {
        iSession->SetZoomL( aNewLevel );
        iLiveObserver.SetZoomL( iSession->CurrentZoomL() );
        }
    else
        {
        iSession->SetBrightnessL( aNewLevel );
        iLiveObserver.SetBrightnessL( iSession->CurrentBrightnessL() );
        }
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::HandleSliderValueChangeL" )
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::OfferToolbarEventL( TInt aCommand )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiLiveSharingController::OfferToolbarEventL [%d]",
                            aCommand );
    
    iLiveObserver.SetZoomVisible( EFalse );
    iLiveObserver.SetBrightnessVisible( EFalse );
                                                    
    switch ( aCommand )
        {
        case EMusuiCmdToolbarPauseLive:
            {
            HandleCommandL( EMusuiCmdViewPause );
            HandlePauseResumeInToolbar();
            break;
            }
            
        case EMusuiCmdToolbarUnPauseLive:
            {
            HandleCommandL( EMusuiCmdViewContinue );
            HandlePauseResumeInToolbar();
            break;
            }
        case EMusuiCmdToolbarZoom:
            {
            iToolbarZoomSelected = !iToolbarZoomSelected;
            
            if ( iToolbarZoomSelected )
                {
                iLiveObserver.SetZoomValues( iSession->MinZoomL(), 
                                             iSession->MaxZoomL() );
                iLiveObserver.SetZoomL( iSession->CurrentZoomL() );
                }
            
            iLiveObserver.SetZoomVisible( iToolbarZoomSelected );
            iSharingObserver.HighlightSelectedToolbarItem( EMusuiCmdToolbarZoom );
            
            break;
            }
        case EMusuiCmdToolbarBrightness:
            {
            iToolbarBrightnessSelected = !iToolbarBrightnessSelected;
            
            if ( iToolbarBrightnessSelected )
                {
                MUS_LOG1( "mus: [MUSUI ]     CMusUiLiveSharingController::OfferToolbarEventL: Brightness %d",
                                iSession->CurrentBrightnessL() );
                                
                iLiveObserver.SetBrightnessValues( iSession->MinBrightnessL(),
                                                   iSession->MaxBrightnessL() );
                
                iLiveObserver.SetBrightnessL( iSession->CurrentBrightnessL() );
                }
            
            iLiveObserver.SetBrightnessVisible( iToolbarBrightnessSelected );
            iSharingObserver.HighlightSelectedToolbarItem( EMusuiCmdToolbarBrightness );
            
            break;
            }        
        default:    // Not live specific, let the base class handle
            {
            CMusUiEventController::OfferToolbarEventL( aCommand );
            break;
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::OfferToolbarEventL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::HandleCommandL( TInt aCommand )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiLiveSharingController::HandleCommandL [%d]",
                            aCommand );

    __ASSERT_ALWAYS( !ExitOccured(), User::Leave( KErrDied ) );

    switch ( aCommand )
        {
        case EMusuiCmdViewPause:
            {            
            UserInitiatedCameraStateChangeL(EFalse);
            break;
            }
        case EMusuiCmdViewContinue:
            {
            UserInitiatedCameraStateChangeL(ETrue);
            break;
            }

        case EMusuiCmdViewShareVideo:
            {
            PauseL();
            // TODO: Change to video sharing
            break;
            }
        case EMusuiCmdViewShareImage:
            {
            PauseL();
            // TODO: Change to image sharing
            break;
            }

        case EMusuiCmdToolbarZoom:
            {
            iToolbarZoomSelected = ETrue;
            iToolbarBrightnessSelected = EFalse;
            iLiveObserver.SetZoomValues( iSession->MinZoomL(), 
                                         iSession->MaxZoomL() );
            iLiveObserver.SetZoomL( iSession->CurrentZoomL() );
            
            iLiveObserver.SetZoomVisible( iToolbarZoomSelected );
            break;
            }
        case EMusuiCmdViewMiddleSoftkeyOk:
            {
            HandleMiddleSoftkeyOkL();
            break;
            }            

        case EMusuiCmdViewZoom:
            {
            ZoomL();
            break;
            }
            
        case EMusuiCmdViewZoomIn:
            {
            iLiveObserver.SetZoomVisible( MUS_NO_TOOLBAR ? iZoomSelected : iToolbarZoomSelected ); 
            iSession->ZoomInL();
            iLiveObserver.SetZoomL( iSession->CurrentZoomL() );
            break;
            }
        case EMusuiCmdViewZoomOut:
            {
            iLiveObserver.SetZoomVisible( MUS_NO_TOOLBAR ? iZoomSelected : iToolbarZoomSelected ); 
            iSession->ZoomOutL();
            iLiveObserver.SetZoomL( iSession->CurrentZoomL() );
            break;
            }

        case EMusuiCmdToolbarBrightness:
            {
            iToolbarZoomSelected = EFalse;
            iToolbarBrightnessSelected = ETrue;
            iLiveObserver.SetBrightnessValues( iSession->MinBrightnessL(),
                                               iSession->MaxBrightnessL() );
            iLiveObserver.SetBrightnessL( iSession->CurrentBrightnessL() );
            
            iLiveObserver.SetBrightnessVisible( iToolbarBrightnessSelected );
            break;
            }
        case EMusuiCmdViewBrightness:
            {
            BrightnessL();
            break;
            }            
            
        case EMusuiCmdViewIncreaseBrightness:
            {
			iLiveObserver.SetBrightnessVisible( MUS_NO_TOOLBAR ? iBrightnessSelected : iToolbarBrightnessSelected ); 
            iSession->IncreaseBrightnessL();
            iLiveObserver.SetBrightnessL( iSession->CurrentBrightnessL() );
            break;
            }
        case EMusuiCmdViewDecreaseBrightness:
            {
			iLiveObserver.SetBrightnessVisible( MUS_NO_TOOLBAR ? iBrightnessSelected : iToolbarBrightnessSelected ); 
            iSession->DecreaseBrightnessL();
            iLiveObserver.SetBrightnessL( iSession->CurrentBrightnessL() );
            break;
            }            

        case EMusuiCmdRecord:
            {
            // Show toolbar hidden while showing options menu
            iEventObserver.SetToolbarVisibility( ETrue );
            
            if ( iDiskFull )
            	{
            	DiskFull();
            	}
            else
            	{
	            iSession->RecordL( ETrue );
	            iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconRecord );
            	}
            break;
            }
        case EMusuiCmdStopRecording:
            {
            // Show toolbar hidden while showing options menu
            iEventObserver.SetToolbarVisibility( ETrue );
            
            iSession->RecordL( EFalse );
            iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconRecordNot );
            break;
            }
            
        default:
            {
            // Try more general handling
            CMusUiEventController::HandleCommandL( aCommand );
            break;
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::HandleCommandL" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::LevelIndicatorDismissed()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::LevelIndicatorDismissed" );
    if ( MUS_NO_TOOLBAR )
        {
        iZoomSelected = EFalse;
        iBrightnessSelected = EFalse;
        TRAP_IGNORE( iSharingObserver.EnableMiddleSoftkeyL( IsLoudSpeakerEnabled() ?
                                                R_MUSUI_VIEW_OPS_MSK_IHF_OFF :
                                                R_MUSUI_VIEW_OPS_MSK_IHF_ON ) );
        }
    else
        {
        if ( iToolbarZoomSelected )
            {
            iToolbarZoomSelected = EFalse;
            if ( !AknLayoutUtils::PenEnabled() )
                {
                TRAP_IGNORE( iSharingObserver.HighlightSelectedToolbarItem(EMusuiCmdToolbarZoom) );
                }
            }
        else if( iToolbarBrightnessSelected )
            {
            iToolbarBrightnessSelected = EFalse;            
            if ( !AknLayoutUtils::PenEnabled() )
                {
                TRAP_IGNORE( iSharingObserver.HighlightSelectedToolbarItem(EMusuiCmdToolbarBrightness) );
                }
            }
            
        }
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::LevelIndicatorDismissed" );
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TMusUiNaviMediaDecorator CMusUiLiveSharingController::RecordIcon()
    {
    MUS_LOG( "mus: [MUSUI ]     CMusUiLiveSharingController::RecordIcon" );
        
    return (iSession->IsRecording() == EFalse) ?
                            EMusUiNaviIconRecordNot :
                            EMusUiNaviIconRecord;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiLiveSharingController::IsBrightnessSupported()
    {
    if ( !iSession )
        {
        return EFalse;
        }
    TRAPD( error, iSession->SetBrightnessL( iSession->CurrentBrightnessL() ) )
    return ( error == KErrNotSupported ) ? EFalse : ETrue;
    }

    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiLiveSharingController::IsDiskFull()
    {
    return iDiskFull;	
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiLiveSharingController::ToolbarZoomSelected()
    {
    return iToolbarZoomSelected;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiLiveSharingController::ToolbarBrightnessSelected()
    {
    return iToolbarBrightnessSelected;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiLiveSharingController::ZoomSelected()
    {
    return iZoomSelected;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiLiveSharingController::BrightnessSelected()
    {
    return iBrightnessSelected;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::ZoomL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::ZoomL" );
    iZoomSelected = ETrue;
    iLiveObserver.SetZoomValues( iSession->MinZoomL(), iSession->MaxZoomL() );
    iLiveObserver.SetZoomL( iSession->CurrentZoomL() );
    iLiveObserver.SetZoomVisible( ETrue );
    
    if ( MUS_NO_TOOLBAR )
        {
        iSharingObserver.EnableMiddleSoftkeyL( R_MUSUI_VIEW_MSK_OK );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::ZoomL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::BrightnessL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::BrightnessL" );
    iBrightnessSelected = ETrue;
    iLiveObserver.SetBrightnessValues( iSession->MinBrightnessL(), 
            iSession->MaxBrightnessL() );
    iLiveObserver.SetBrightnessL( iSession->CurrentBrightnessL() );
    iLiveObserver.SetBrightnessVisible( ETrue );

    if ( MUS_NO_TOOLBAR )
        {
        iSharingObserver.EnableMiddleSoftkeyL( R_MUSUI_VIEW_MSK_OK );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::BrightnessL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::HandleMiddleSoftkeyOkL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::HandleMiddleSoftkeyOkL" );

    if ( iZoomSelected )
        {
        iZoomSelected = EFalse;
        iLiveObserver.SetZoomVisible( EFalse );
        }
    else if ( iBrightnessSelected )
        {
        iZoomSelected = EFalse;
        iLiveObserver.SetBrightnessVisible( EFalse );
        }
    
    iSharingObserver.EnableMiddleSoftkeyL( IsLoudSpeakerEnabled() ?
                                                R_MUSUI_VIEW_OPS_MSK_IHF_OFF :
                                                R_MUSUI_VIEW_OPS_MSK_IHF_ON );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::HandleMiddleSoftkeyOkL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::ZoomInL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::ZoomInL" );
    iSession->ZoomInL();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::ZoomInL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::ZoomOutL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::ZoomOutL" );
    iSession->ZoomOutL();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::ZoomOutL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::IncreaseBrightnessL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::IncreaseBrightnessL" );
    iSession->IncreaseBrightnessL();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::IncreaseBrightnessL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::DecreaseBrightnessL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::DecreaseBrightnessL" );
    iSession->DecreaseBrightnessL();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::DecreaseBrightnessL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::DiskFull()
    {
    // Update the icon:
    TRAP_IGNORE( iEventObserver.ShowNaviPaneIconL( EMusUiNaviIconRecordNot );
    DismissWaitDialog();
    // Saving ends when disk is full but streaming continues (user is notified)
    TRAP_IGNORE( MusUiDialogUtil::ShowGlobalErrorDialogL( R_MUS_VIEW_NOTE_MEMORY_LOW ) ) );
    iDiskFull = ETrue;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::OrientationRefreshEnded()  
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::OrientationRefreshEnded" );
    iLiveObserver.DoRefreshView();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::OrientationRefreshEnded" );
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::SessionEstablished()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::SessionEstablished" );
    
    iSessionEstablished = ETrue;
    
    TRAPD( error, iResourceHandler->RequestCameraL( ETrue ) );
    if ( error != KErrNone )
        {
        MUS_LOG1( "mus: [MUSUI ]     RequestCameraL leave code: %d", error )
        }
        
    CMusUiSendController::SessionEstablished(); // Base class handling
    
    iVideoToBeSaved = ETrue;
    
    MusSettingsKeys::TAutoRecord autoRecordValue = EAutoRecordOff; 
        TRAPD ( errAutoRecord, autoRecordValue = 
                    MultimediaSharingSettings::AutoRecordSettingL() );
    if ( errAutoRecord != KErrNone )
        {
        MUS_LOG1( "mus: [MUSUI ]     AutoRecordSettingL leave code: %d", errAutoRecord )
        HandleError( errAutoRecord );
        }
    
    if ( autoRecordValue != EAutoRecordOff )
        {
        TRAPD ( errRecordL, iSession->RecordL( ETrue ) );
        if ( errRecordL != KErrNone )
            {
            MUS_LOG1( "mus: [MUSUI ]     RecordL leave code: %d", errRecordL )
            HandleError( errRecordL );
            }
        }

    TRAP_IGNORE( iEventObserver.ShowNaviPaneIconL( RecordIcon() ) )
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::SessionEstablished" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::SessionTerminated()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::SessionTerminated" );    
    iSessionEstablished = EFalse;
    if ( !ExitOccured() )
        {
        DismissWaitDialog();
        
        TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::SessionTerminated" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::SessionConnectionLost()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::SessionConnectionLost" );
    iSessionEstablished = EFalse;
    DismissWaitDialog();
    if ( !ExitOccured() )
        {
        TRAP_IGNORE( MusUiDialogUtil::ShowGlobalErrorDialogL( 
                            R_MUS_LIVE_SHARING_VIEW_NOTE_CONNECTION_LOST ) );
        }
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::SessionConnectionLost" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::SessionFailed()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::SessionFailed" );
    iSessionEstablished = EFalse;
    if ( !ExitOccured() )
        {
        DismissWaitDialog();
       
        TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::SessionFailed" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::StreamIdle()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::StreamIdle" );
    DismissWaitDialog();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::StreamIdle" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::StreamStreaming()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingController::StreamStreaming" );
    //DismissWaitDialog();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::StreamStreaming" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::SessionTimeChanged(
                        const TTimeIntervalSeconds& aSeconds )
    {
    if ( aSeconds.Int() > -1 )
        {
        iSharingObserver.UpdateSessionTime( SessionTimeFormatted( aSeconds ) );
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::InactivityTimeout()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiLiveSharingController::InactivityTimeout" );
    DismissWaitDialog();
    if ( !ExitOccured() )
        {
        TRAP_IGNORE( MusUiDialogUtil::ShowGlobalErrorDialogL( 
                            R_MUS_LIVE_SHARING_VIEW_NOTE_SERVICE_N_A ) );
        }
    
    TRAP_IGNORE( iCallbackService->AsyncEventL( EMusUiAsyncHandleExit ) );
        
    MUS_LOG( "mus: [MUSUI ] <- CMusUiLiveSharingController::InactivityTimeout" );
    }

// -----------------------------------------------------------------------------
// Determines whether Session established or not
// -----------------------------------------------------------------------------
//
TBool CMusUiLiveSharingController::IsSessionEstablished()
	{
	return iSessionEstablished;
	}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::HandlePauseResumeInToolbar()
    {
    TBool isPlaying( EFalse );
    TRAP_IGNORE( isPlaying = IsPlayingL() )
    if ( isPlaying )
        {
        iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarUnPauseLive,
                                                EMusuiCmdToolbarPauseLive,
                                                ETrue );
        }
    else
        {
        iSharingObserver.ReplaceToolbarCommand( EMusuiCmdToolbarPauseLive,
                                                EMusuiCmdToolbarUnPauseLive,
                                                ETrue );
        }
    }

// -----------------------------------------------------------------------------
// Workaround for problem at lower level (encoder side) which causes crash
// if several sequential pause/resumes are done too rapidly. Discard state change
// attempt if it occurs too quickly after previous state change.
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingController::UserInitiatedCameraStateChangeL( TBool aEnable )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiLiveSharingController::UserInitiatedCameraStateChangeL, enable:", 
              aEnable );
    
    if ( iPauseResumeGuardTimer->IsActive() ){
        MUS_LOG( "mus: [MUSUI ]  <- State change ignored as guard timer is running!" );
        return;
    }
    
    if ( aEnable )
        {
        PlayL();
        }
    else
        {
        PauseL();
        }
    
    iPauseResumeGuardTimer->After( KMusUiPauseResumeGuardPeriod );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingController::UserInitiatedCameraStateChangeL" );
    }


// End of file

