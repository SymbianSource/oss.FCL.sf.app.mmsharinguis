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
* Description:  Mus Applications Event Observer Interface
*
*/



#include "musuigeneralview.h"     
#include "musui.hrh"
#include "musuiappui.h"         // Application UI class
#include "musuistatuspanehandler.h"
#include "musuieventcontroller.h"
#include "musuiviewcontainer.h"
#include "musuidefinitions.h"
#include "mussettings.h"
#include "mussettingskeys.h"
#include "muslogger.h" // debug logging
#include "musuiactivetimer.h"
#include "musuibackgroundviewcontainer.h"
#include <musui.rsg>
#include <musui.mbg>

#include <eikenv.h>               // Eikon enviroment
#include <aknViewAppUi.h>
#include <avkon.hrh>
#include <eikmenup.h>
#include <akntoolbar.h>
#include <aknbutton.h>
#include <AknIncallBubbleNotify.h>
#include <featmgr.h>

const TInt KBackgroundIntervalForClose = 10 * 1000 * 1000; // 10s
const TInt KMusFgBgEventFiltering = 200 * 1000; // 200 ms

// -----------------------------------------------------------------------------
// EPOC two-phased constructor
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::ConstructL" );

    BaseConstructL( R_MUSUI_VIEW );
    TInt toolbarId = R_MUS_TOOLBAR;
    if (!AknLayoutUtils::PenEnabled())
	    {
	    toolbarId = R_MUS_TOOLBAR_NONTOUCH;
	    }
    CreateAndSetToolbarL(toolbarId);    
    
    // check if operator specific functionality is needed  
    iOperatorSpecificFunctionality = 
        ( MultimediaSharingSettings::OperatorVariantSettingL() == 
                                        MusSettingsKeys::EOperatorSpecific );
    
    iIncallBubble = CAknIncallBubble::NewL();
    iSessionEndTimer = CMusUiActiveTimer::NewL( this );
    
    iFgBgTimer = CMusUiActiveTimer::NewL( this );
                                                    
    if( MUS_NO_TOOLBAR )
        {
        MUS_LOG( "mus: [MUSUI ] Operator variant hides toolbar!" );
        Toolbar()->SetToolbarVisibility( EFalse );
        Toolbar()->MakeVisible(EFalse);
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::ConstructL" );
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMusUiGeneralView::~CMusUiGeneralView()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::~CMusUiGeneralView" );
            
    delete iSessionEndTimer; 
    
    delete iFgBgTimer;          
    
    if ( iIncallBubble )
        {
        TRAP_IGNORE( iIncallBubble->SetIncallBubbleAllowedInUsualL( ETrue ) )    
        }
    delete iIncallBubble; 
    
    /* Remove and delete background container */
    if ( iBackgroundContainer )
      {
      AppUi()->RemoveFromViewStack( *this, iBackgroundContainer );
      }
    delete iBackgroundContainer;
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::~CMusUiGeneralView" );
    }


// -----------------------------------------------------------------------------
// Fetches a pointer to the application's AppUi class ( CMusUiAppUi )
// -----------------------------------------------------------------------------
//
CMusUiAppUi* CMusUiGeneralView::MusAppUi() const
    {
    MUS_LOG( "mus: [MUSUI ]     CMusUiGeneralView::MusAppUi()" );
    return static_cast<CMusUiAppUi*>( iEikonEnv->AppUi() );
    }



// -----------------------------------------------------------------------------
// To be overridden in subclasses
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::HandleKeyUpEvent( TInt /*aKeyCode*/ )
    {
    // NOP
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::SetToolbarVisibility( TBool aVisible )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiGeneralView::SetToolbarVisibility: %d",
                            aVisible );
    
    if( MUS_NO_TOOLBAR )
        {
        MUS_LOG( "mus: [MUSUI ] Operator variant hides toolbar!" );
        Toolbar()->SetToolbarVisibility( EFalse );
        Toolbar()->MakeVisible(EFalse);
        return;
        }
    
    // Show toolbar only after established session and before of exit procedure 
    if ( aVisible &&
         EventController() &&
         !EventController()->ExitOccured() &&
         EventController()->ConnectionEstablished() )
        {
        MUS_LOG( "mus: [MUSUI ] setting toolbar visible" );
        
        if ( SharingContainer().WaitDialogShown() )
            {
            // Cannot show toolbar at the moment as it would go over the dialog.
            // Instead, request container to set visibility once dialog
            // is dismissed.
            SharingContainer().RequestToolbarVisibilityOnceDialogDismissed( this );
            }
        else
            {
            Toolbar()->SetToolbarVisibility( ETrue );
			Toolbar()->MakeVisible(ETrue);
            }
        }
    else if ( !aVisible && EventController()->ExitOccured() )
        {
        Toolbar()->SetToolbarVisibility( EFalse );
		Toolbar()->MakeVisible(EFalse);
        }
    else
        {
        MUS_LOG( "mus: [MUSUI ] We do not set fixed toolbar invisible" );
        if (!AknLayoutUtils::PenEnabled())
        	{
        	Toolbar()->SetToolbarVisibility( EFalse );
			Toolbar()->MakeVisible(EFalse);
        	}
        
        SharingContainer().RequestToolbarVisibilityOnceDialogDismissed( NULL );
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::SetToolbarVisibility" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::AsyncQueryDialogAcceptedL( TBool aAccepted )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::AsyncQueryDialogAcceptedL" );
    
    EventControllerL().AsyncQueryDialogAcceptedL( aAccepted );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::AsyncQueryDialogAcceptedL" );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CMusUiGeneralView::FocusedToolbarItem() const
    {
    MUS_LOG( "mus: [MUSUI ]  <-> CMusUiGeneralView::FocusedToolbarItem" );
    return Toolbar()->FocusedItem();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::DynInitToolbarL( TInt /*aResourceId*/,
                                         CAknToolbar* /*aToolbar*/ )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::DynInitToolbarL" );

    Toolbar()->SetWithSliding( EFalse );

    // Hide items according to states of speaker and microphone:
    
    Toolbar()->HideItem( EventControllerL().IsMicMutedL() ?
                            EMusuiCmdToolbarMute :
                            EMusuiCmdToolbarUnmute,
                        ETrue,
                        ETrue );
    
    TBool loudSpeakerEnabled( EventControllerL().IsLoudSpeakerEnabled() );
      
    Toolbar()->HideItem( loudSpeakerEnabled ?
                                EMusuiCmdToolbarSpeakerOn :
                                EMusuiCmdToolbarSpeakerOff,
                         ETrue,
                         ETrue );
    
    // Dim audio routing button if audio routing cannot be changed
    // or when some button is selected
    TBool dimAudioRouting( iToolbarItemSelected || 
                           !EventControllerL().AudioRoutingCanBeChanged() );
                                                 
    Toolbar()->SetItemDimmed( loudSpeakerEnabled ?
                                    EMusuiCmdToolbarSpeakerOff :
                                    EMusuiCmdToolbarSpeakerOn,
                              dimAudioRouting,
                              ETrue );                 
    
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::DynInitToolbarL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::OfferToolbarEventL( TInt aCommand )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiGeneralView::OfferToolbarEventL: %d",
                            aCommand );
    EventControllerL().OfferToolbarEventL( aCommand );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::OfferToolbarEventL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiGeneralView::ShowWaitDialogL( const TDesC& aPrompt, 
                                       TInt aTimerInSeconds,
                                       TInt aExpirationMessageResourceId )
    {
    MUS_LOG_TDESC( "mus: [MUSUI ]    -> CMusUiGeneralView::ShowWaitDialogL: ",
                            aPrompt );
                            
    TBool retVal = SharingContainer().RunWaitDialogL( 
                                                aPrompt, 
                                                aTimerInSeconds,
                                                aExpirationMessageResourceId );
    
    MUS_LOG1( "mus: [MUSUI ]  <- CMusUiGeneralView::ShowWaitDialogL: retVal = %d",
              retVal )
    
    return retVal;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::DismissWaitDialogL( TBool aReturnValue )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::DismissWaitDialogL" );
 
    SharingContainer().DismissWaitDialogL( aReturnValue );       
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::DismissWaitDialogL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::SetWaitDialogCallbackL( MProgressDialogCallback* aCallback )
    {
    SharingContainer().SetWaitDialogCallbackL( aCallback );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::ShowStatusPaneTitleL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::ShowStatusPaneTitleL" );

    MusAppUi()->MusStatusPane()->ActivateNaviPaneMediaL();

    // Status Pane Title:
    if ( EventControllerL().MusContactName().Length() > 0 )
        {
        MusAppUi()->MusStatusPane()->SetTitleL( 
                                        EventControllerL().MusContactName() );
        }
    else if ( EventControllerL().MusTelNumberValue().Length() > 0 )
        {
        MusAppUi()->MusStatusPane()->SetTitleL(
                                    EventControllerL().MusTelNumberValue() );
        }
    else
        {
        MusAppUi()->MusStatusPane()->SetTitleFromResourceL(
                                R_MUS_LIVE_SHARING_VIEW_TITLE );
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::ShowStatusPaneTitleL" );
    }
        

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::EnableMiddleSoftkeyL( TInt aResourceId )
    {
    // Update Middle softkey
    CEikButtonGroupContainer* cba = Cba();
    if ( cba )
        {
        cba->SetCommandL( CEikButtonGroupContainer::EMiddleSoftkeyPosition,
                          aResourceId );
        cba->DrawDeferred();
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::ActivateVolumeControlL( TInt aValue, TBool aOnlyIfVisible )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::ActivateVolumeControlL" );
    MusAppUi()->MusStatusPane()->ActivateVolumeControlL( aValue, aOnlyIfVisible );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::ActivateVolumeControlL" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::HighlightSelectedToolbarItem( TInt aCommand )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::HightlightSelectedToolbarItem" );
    iToolbarItemSelected = !iToolbarItemSelected;
    CAknButton* button = static_cast<CAknButton*>( Toolbar()->ControlOrNull(
                                                                aCommand ) );
    button->SetCurrentState( iToolbarItemSelected ? 1 : 0, ETrue );

    TBool dimAudioRouting( iToolbarItemSelected || 
                           !( EventController() &&
                              EventController()->AudioRoutingCanBeChanged() ) );
                           
    for ( TInt i = EMusuiCmdToolbarVolume; i < EMusUiCmdToolbarLastIndex; i++ )
        {
        // Undim audio routing buttons only if changing the routing is allowed                  
        if ( i == EMusuiCmdToolbarSpeakerOn )
            {
            Toolbar()->SetItemDimmed( 
                                EMusuiCmdToolbarSpeakerOn,
                                dimAudioRouting,
                                ETrue );  
            }
        else if ( i == EMusuiCmdToolbarSpeakerOff )
            {
            Toolbar()->SetItemDimmed( 
                                EMusuiCmdToolbarSpeakerOff,
                                dimAudioRouting,
                                ETrue );  
            }
        else if ( i != aCommand )
            {
            Toolbar()->SetItemDimmed( i, iToolbarItemSelected, ETrue );
            }
        else
            {
            // NOP
            }
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::HightlightSelectedToolbarItem" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::ReplaceToolbarCommand( TInt aOldCommand,
                                               TInt aNewCommand,
                                               TBool aSetNewCommandFocused )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiGeneralView::ReplaceToolbarCommand: %d",
                            aNewCommand );  
    
    // Hide the old item:
    Toolbar()->HideItem( aOldCommand, ETrue, EFalse );
    // Show new item:
    Toolbar()->HideItem( aNewCommand, EFalse, ETrue );
    
    TBool dimAudioRouting( iToolbarItemSelected || 
                           !( EventController() &&
                              EventController()->AudioRoutingCanBeChanged() ) );
    
    if ( aNewCommand == EMusuiCmdToolbarSpeakerOn ||
         aNewCommand == EMusuiCmdToolbarSpeakerOff )
        {
        // Dim audio routing button if audio routing cannot be changed                     
        Toolbar()->SetItemDimmed( aNewCommand,
                                  dimAudioRouting,
                                  ETrue );    
        }
        
    if ( aSetNewCommandFocused &&
         !AknLayoutUtils::PenEnabled() )
        {
        TRAP_IGNORE( Toolbar()->SetFocusedItemL( aNewCommand ) );
        }
        
    MUS_LOG1( "mus: [MUSUI ]  <- CMusUiGeneralView::ReplaceToolbarCommand: %d",
                            aNewCommand );    
    }   


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::DismissMenuBar()
	{
	 MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::DismissMenuBar" );
	 StopDisplayingMenuBar();
	 MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::DismissMenuBar" );
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::RefreshAudioRoutingToolbarButton()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::RefreshAudioRoutingToolbarButton" )
    TBool loudSpeakerEnabled( EventController() &&
                          EventController()->IsLoudSpeakerEnabled() );
    if ( MUS_NO_TOOLBAR )
        {
        TRAP_IGNORE( EnableMiddleSoftkeyL( loudSpeakerEnabled ?
                                               R_MUSUI_VIEW_OPS_MSK_IHF_OFF :
                                               R_MUSUI_VIEW_OPS_MSK_IHF_ON ) );
        } 
    else if ( Toolbar()->IsShown() )
        {
        // If toolbar is not shown, it will be refreshed automatically when shown.
        TInt focusedItem( Toolbar()->FocusedItem() );
        TBool focusInAudioRouting( focusedItem == EMusuiCmdToolbarSpeakerOff ||
                                   focusedItem == EMusuiCmdToolbarSpeakerOn );
                                   
        TBool loudSpeakerEnabled( EventController() &&
                                  EventController()->IsLoudSpeakerEnabled() );
        
        // Hide old audio routing buttton
        Toolbar()->HideItem( loudSpeakerEnabled ?
                                EMusuiCmdToolbarSpeakerOn :
                                EMusuiCmdToolbarSpeakerOff,
                             ETrue,
                             EFalse );
                                  
        // Show new audio routing button                              
        Toolbar()->HideItem( loudSpeakerEnabled ?
                                    EMusuiCmdToolbarSpeakerOff :
                                    EMusuiCmdToolbarSpeakerOn,
                             EFalse,
                             ETrue );                     
        
        // Dim new audio routing button if audio routing cannot be changed
        // or when some button is selected        
        TBool dimAudioRouting( iToolbarItemSelected || 
                               !( EventController() &&
                               EventController()->AudioRoutingCanBeChanged() ) );
                                                     
        Toolbar()->SetItemDimmed( loudSpeakerEnabled ?
                                        EMusuiCmdToolbarSpeakerOff :
                                        EMusuiCmdToolbarSpeakerOn,
                                  dimAudioRouting,
                                  ETrue );
        
        // If audio routing button has been focused, focus also the new button
        // if it is not dimmed
        if ( focusInAudioRouting && 
             !dimAudioRouting && 
             !AknLayoutUtils::PenEnabled() )
            {
            TRAP_IGNORE( Toolbar()->SetFocusedItemL( loudSpeakerEnabled ?
                                                EMusuiCmdToolbarSpeakerOff :
                                                EMusuiCmdToolbarSpeakerOn ) )
            }  
        
         
        }
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::RefreshAudioRoutingToolbarButton" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::UpdateSessionTime( const TDesC& aSessionTime )
    {
    MUS_LOG_TDESC( "mus: [MUSUI ]  -> CMusUiGeneralView::UpdateSessionTime: ",
                   aSessionTime );
    
    TRAPD( err, MusAppUi()->MusStatusPane()->SetLeftLabelL( aSessionTime ) );
    if ( err != KErrNone )
        {
        EventController()->HandleError( err );
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::UpdateSessionTime" );
    }
        

// -----------------------------------------------------------------------------
// takes care of view command handling
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::HandleCommandL( TInt aCommand )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::HandleCommandL" );
    
    EventControllerL().HandleCommandL( aCommand );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::HandleCommandL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiEventController& CMusUiGeneralView::EventControllerL() const
    {
    __ASSERT_ALWAYS( EventController(), User::Leave( KErrNotFound ) );
    
    return *EventController();
    }


// -----------------------------------------------------------------------------
// Called when focus event occurs. Timer is started once coming to foreground
// as often in such case we receive burst of foreground/background events.
// Timer is used to filter that burst and to determine the final condition.
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::HandleForegroundEventL( TBool aForeground, TBool aExit )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiGeneralView::HandleForegroundEventL: %d",
                            aForeground );
    
    SetCurrentFgBgEvent( ( aForeground ? EMusFgEvent : EMusBgEvent ) );
    iCurrentExitSetting = aExit;
        
    if ( !iFgBgTimer->IsActive() )
        {
        if ( aForeground )
            {
            MUS_LOG( "mus: [MUSUI ] delayed to fg" );
            }
        else
            {
            DoBackgroundEventL( iCurrentExitSetting );
            MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::HandleForegroundEventL,\
immediately to bg" );
            return;
            }
        }

    iFgBgTimer->After( KMusFgBgEventFiltering );    

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::HandleForegroundEventL" );
    }
 
 
// -----------------------------------------------------------------------------
// From MMusUiActiveTimerObserver
// -----------------------------------------------------------------------------
//    
void CMusUiGeneralView::TimerComplete( CMusUiActiveTimer* aTimer )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiGeneralView::TimerComplete" );
                          
    if ( aTimer == iSessionEndTimer )
        {
        MUS_LOG( "mus: [MUSUI ]     Close application because of going \
in background" );
                  
        TRAP_IGNORE( EventControllerL().HandleExitL() )
        }
    else
        {        
        TRAP_IGNORE( CompleteForegroundEventL() )
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::TimerComplete" );
    }


// -----------------------------------------------------------------------------
// From MEikMenuObserver, Called by framework before constructing menupane.
// Initializes menu items common to all sharing types.
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::DynInitMenuPaneL( TInt aResourceId,
                                          CEikMenuPane* aMenuPane )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiGeneralView::DynInitMenuPaneL [%d]",
                            aResourceId );

    if ( aResourceId == R_MUSUI_VIEW_MENU )
        {

        // Delete Single session items:
        aMenuPane->DeleteMenuItem( EMusuiCmdViewShareVideo );
        aMenuPane->DeleteMenuItem( EMusuiCmdViewShareImage );
        aMenuPane->DeleteMenuItem( EMusuiCmdViewShareLive );        
        if ( MUS_NO_TOOLBAR )
            {
            aMenuPane->DeleteMenuItem( EventControllerL().IsMicMutedL() ?
                                           EMusuiCmdViewMicrophoneMute :
                                           EMusuiCmdViewMicrophoneUnmute );
            }
        else
            {
            aMenuPane->DeleteMenuItem( EMusuiCmdViewMicrophoneMute );
            aMenuPane->DeleteMenuItem( EMusuiCmdViewMicrophoneUnmute );
            }
        
        if ( !FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            aMenuPane->DeleteMenuItem( EAknCmdHelp );
            }
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::DynInitMenuPaneL" );
    }


// -----------------------------------------------------------------------------
// CMusUiGeneralView::DoActivateL
// From CAknView, Gets called from framework when activating this view
// -----------------------------------------------------------------------------
void CMusUiGeneralView::DoActivateL( const TVwsViewId& /*aPrevViewId*/,
                                     TUid /*aCustomMessageId*/,
                                     const TDesC8& /*aCustomMessage*/ )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiGeneralView::DoActivateL()" )
    
    
    MusAppUi()->MusStatusPane()->ClearNaviPaneL();
    MusAppUi()->MusStatusPane()->SetTitleFromResourceL(
        R_MUS_LIVE_SHARING_VIEW_TITLE );
    
    iIncallBubble->SetIncallBubbleAllowedInUsualL( EFalse );
    
    /* Draw the skin background in the client rectangle area.*/
    if ( !iBackgroundContainer )
       {
       /* Construct background container with clientrect area and
        * Ordinal position of 0 ie highest so that it draws the initial 
        * skin background first.
        */       
       iBackgroundContainer = 
           CMusUiBackgroundViewContainer::NewL( ClientRect(), 0 );
       /* Add container to view stack so that it get view events */
       AppUi()->AddToViewStackL( *this, iBackgroundContainer );
       }
    else
       {
       /* Set the windows ordinal position to highest ie 0,
        * so that background gets priority and drawn first.        
        */
       iBackgroundContainer->SetOrdinalPosition( 0 );       
       }
    /* Draw the skin background now */
    iBackgroundContainer->DrawNow();
                            
    MUS_LOG( "mus: [MUSUI ] <- CMusUiGeneralView::DoActivateL()" )
    }


// -----------------------------------------------------------------------------
// CMusUiGeneralView::DoDeactivate
// From AknView, Gets called from framework when deactivating this view
// -----------------------------------------------------------------------------
void CMusUiGeneralView::DoDeactivate()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiGeneralView::DoDeactivate()" )
    if ( iBackgroundContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iBackgroundContainer );
        delete iBackgroundContainer;
        iBackgroundContainer = NULL;
        }
    MUS_LOG( "mus: [MUSUI ] <- CMusUiGeneralView::DoDeactivate()" )
    }


// -----------------------------------------------------------------------------
// Asynchronous completion of last fg/bg condition.
// -----------------------------------------------------------------------------
// 
void CMusUiGeneralView::CompleteForegroundEventL()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiGeneralView::CompleteForegroundEventL()" )
    
    if ( iCurrentFgBgEvent == EMusFgEvent )
        {
        DoForegroundEventL();
        }
    else if ( iCurrentFgBgEvent == EMusBgEvent && EventControllerL().IsForeground() )
        {
        // Bg handling when already at bg would cause problems in state
        // restoring phase when coming back to fg. Easiest to deal with that
        // at this level.
        DoBackgroundEventL( iCurrentExitSetting );
        }

    SetCurrentFgBgEvent( EMusFgBgEventNone );   
    
    MUS_LOG( "mus: [MUSUI ] <- CMusUiGeneralView::CompleteForegroundEventL()" )
    }


// -----------------------------------------------------------------------------
// Complete foreground event.
// -----------------------------------------------------------------------------
// 
void CMusUiGeneralView::DoForegroundEventL()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiGeneralView::DoForegroundEventL()" )
    
    RefreshView();
    
    iSessionEndTimer->Cancel();    
        
    EventControllerL().HandleForegroundEventL( ETrue );
    iIncallBubble->SetIncallBubbleAllowedInUsualL( EFalse );
    
    MUS_LOG( "mus: [MUSUI ] <- CMusUiGeneralView::DoForegroundEventL()" )
    }

    
// -----------------------------------------------------------------------------
// Complete background event.
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::DoBackgroundEventL( TBool aExit )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiGeneralView::DoBackgroundEventL()" )
    
    if ( aExit )
        {
        MUS_LOG( "mus: [MUSUI ]     Go to background, \
                  wait some seconds before closing..." );
        iSessionEndTimer->After( KBackgroundIntervalForClose );
        }
    if ( !MusAppUi()->ExitValue() )
        {
        MUS_LOG( "mus: [MUSUI ]     Calling EventControllerL()" );
        EventControllerL().HandleForegroundEventL( EFalse );
        // If exiting, next will be called by destructor    
    	iIncallBubble->SetIncallBubbleAllowedInUsualL( ETrue );  
        } 
        
    SetCurrentFgBgEvent( EMusFgBgEventNone );
    
    MUS_LOG( "mus: [MUSUI ] <- CMusUiGeneralView::DoBackgroundEventL()" )
    }

// -----------------------------------------------------------------------------
// Set current fg/bg condition.
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::SetCurrentFgBgEvent( TMusFgBgEventType aEventType )
    {
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiGeneralView::SetCurrentFgBgEvent() event:%d",
              aEventType )
             
    iCurrentFgBgEvent = aEventType;         
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiGeneralView::OperatorSpecificFunctionality() const
    {
    return iOperatorSpecificFunctionality;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusUiGeneralView::ToolbarPlaceHolderHeight() const
    {
    TInt toolbarPlaceHolderHeight( 0 );
    CAknAppUiBase::TAppUiOrientation orientation = MusAppUi()->AppOrientation();
    if ( !Toolbar()->IsShown() && 
         orientation == CAknAppUiBase::EAppUiOrientationPortrait &&                        
         AknLayoutUtils::PenEnabled() )
        {
        // In touch UI in portrait mode the toolbar 
        // fills the whole lower part of the screen
        toolbarPlaceHolderHeight = Toolbar()->Size().iHeight;
        }
    return toolbarPlaceHolderHeight;       
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusUiGeneralView::ToolbarPlaceHolderWidth() const 
    { 
    TInt toolbarPlaceHolderWidth( 0 ); 
    CAknAppUiBase::TAppUiOrientation orientation = MusAppUi()->AppOrientation(); 
    if ( !Toolbar()->IsShown() && 
         orientation == CAknAppUiBase::EAppUiOrientationLandscape &&                        
         AknLayoutUtils::PenEnabled() ) 
        { 
        // In touch UI in EAppUiOrientationLandscape mode the toolbar 
        // fills the whole lower part of the screen 
        toolbarPlaceHolderWidth = Toolbar()->Size().iWidth; 
        } 
    return toolbarPlaceHolderWidth;       
    } 

// -----------------------------------------------------------------------------
// Increase/Decrease background container ordinal so that other controls
// can preceed background
// -----------------------------------------------------------------------------
//
void CMusUiGeneralView::UpdateBackgroundOrdinalPosition( TBool aUp )
    {
    // If there is no background container then do not care
    if ( iBackgroundContainer )
        {
        iBackgroundContainer->UpdateOrdinalPositionValue( aUp );              
        }    
    }
// end of file






