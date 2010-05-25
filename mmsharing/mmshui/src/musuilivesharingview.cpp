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
* Description:  The MUS application's UI class.
*
*/



#include "musenglivesession.h"
#include "musuilivesharingview.h"
#include "musuilivesharingviewcontainer.h"
#include "musuibackgroundviewcontainer.h"
#include "musuilivesharingcontroller.h"
#include "musuiactivitymanger.h"
#include "musuiappui.h"
#include "musuistatuspanehandler.h"
#include "musui.hrh"
#include "musuidefinitions.h"
#include "muslogger.h" // debug logging
#include <musui.rsg>
#include <musui.mbg>
#include "musengmcesession.h"

#include <aknViewAppUi.h>
#include <avkon.hrh>
#include <eikmenup.h>
#include <akntoolbar.h>


const TInt KMusUiContainerWidth = 30;


// -----------------------------------------------------------------------------
// EPOC two-phased constructor
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::ConstructL" );

    CMusUiSendView::ConstructL();

    Toolbar()->SetToolbarObserver( this );
    Toolbar()->UpdateBackground();
/*
    if( MUS_NO_TOOLBAR )
        {
        MUS_LOG( "mus: [MUSUI ]  Operator variant hides toolbar!" );
        Toolbar()->SetToolbarVisibility( EFalse );
        Toolbar()->MakeVisible(EFalse);
        }
*/
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::ConstructL" );
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMusUiLiveSharingView::~CMusUiLiveSharingView()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::~CMusUiLiveSharingView" );
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    delete iContainer;
    delete iController;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::~CMusUiLiveSharingView" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TUid CMusUiLiveSharingView::Id() const
    {
    MUS_LOG( "mus: [MUSUI ]     CMusUiLiveSharingView::Id()" );
    return KMusUidLiveSharingView;
    }


// -----------------------------------------------------------------------------
// Handle EKeyUpArrow key presses 
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::HandleKeyUpArrowL()
    {
    HandleCommandL(
            iController->ZoomSelected() ? EMusuiCmdViewZoomIn :
            iController->BrightnessSelected() ? EMusuiCmdViewIncreaseBrightness :
            EMusUiCmdToolbarLastIndex );
    }


// -----------------------------------------------------------------------------
// Handle EKeyDownArrow key presses
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::HandleKeyDownArrowL()
    {
    HandleCommandL(
            iController->ZoomSelected() ? EMusuiCmdViewZoomOut :
            iController->BrightnessSelected() ? EMusuiCmdViewDecreaseBrightness :
            EMusUiCmdToolbarLastIndex );
    }




// -----------------------------------------------------------------------------
// From MEikMenuObserver, Called by framework before constructing menupane
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::DynInitMenuPaneL( TInt aResourceId,
                                              CEikMenuPane* aMenuPane )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::DynInitMenuPaneL [%d]" );
    SetZoomVisible(EFalse);
    SetBrightnessVisible(EFalse);
    // Base class initiated initialization first
    CMusUiGeneralView::DynInitMenuPaneL( aResourceId, aMenuPane );
    if( !MUS_NO_TOOLBAR )
        {
        SetInitialFocusedItemL();
        }
    
    if ( aResourceId == R_MUSUI_VIEW_MENU )
        {
        // Delete items not related to live sharing:
        aMenuPane->DeleteMenuItem( EMusuiCmdViewReplay );
        aMenuPane->DeleteMenuItem( EMusuiCmdViewAudioMute );
        aMenuPane->DeleteMenuItem( EMusuiCmdViewAudioNormal );        
        aMenuPane->DeleteMenuItem( EMusuiCmdViewFullScreen );
        
        if ( iController->ConnectionEstablished()
                     && !iController->IsDiskFull() )
            {
            CMusEngLiveSession *session = dynamic_cast <CMusEngLiveSession*>
                ( iController->EngineOutSession() );
            
            if ( session->IsPlayingL() )
                {
                aMenuPane->DeleteMenuItem( session->IsRecording() ?
                                           EMusuiCmdRecord :
                                           EMusuiCmdStopRecording );
                }
            else
                { 
                // when camera source was paused, hide the command
                aMenuPane->DeleteMenuItem( EMusuiCmdRecord );
                aMenuPane->DeleteMenuItem( EMusuiCmdStopRecording );
                }
            }
        else
            {
            // Hide recording commands if the session is not established
            // TODO: check autorecord on/off
            aMenuPane->DeleteMenuItem( EMusuiCmdRecord );
            aMenuPane->DeleteMenuItem( EMusuiCmdStopRecording );                    
            }

        __ASSERT_ALWAYS( iController, User::Leave( KErrNotReady ) );
    
        if( MUS_NO_TOOLBAR )
            {
            aMenuPane->DeleteMenuItem( EMusuiCmdToolbarBrightness );
            aMenuPane->DeleteMenuItem( EMusuiCmdToolbarZoom );            
            if ( !iController->IsBrightnessSupported() ||
                 !SendController()->IsPlayingL() )
                {
                aMenuPane->DeleteMenuItem( EMusuiCmdViewBrightness );
                }
            
            if ( !SendController()->IsPlayingL() )
                {
                aMenuPane->DeleteMenuItem( EMusuiCmdViewZoom );
                }
            }
        else
            {
            aMenuPane->DeleteMenuItem( EMusuiCmdViewBrightness );
            aMenuPane->DeleteMenuItem( EMusuiCmdViewZoom );            
            if ( !iController->IsBrightnessSupported() ||
                 !SendController()->IsPlayingL() || 
                 !AknLayoutUtils::PenEnabled() )
                {
                aMenuPane->DeleteMenuItem( EMusuiCmdToolbarBrightness );
                }
            
            if ( !SendController()->IsPlayingL() ||
                 !AknLayoutUtils::PenEnabled() )
                {
                aMenuPane->DeleteMenuItem( EMusuiCmdToolbarZoom );
                }
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::DynInitMenuPaneL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::HandleToolbarCommandL( TInt aCommand )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiLiveSharingView::HandleToolbarCommandL: %d",
                            aCommand );
    
     __ASSERT_ALWAYS( iController, User::Leave( KErrNotReady ) );

    switch ( aCommand )
        {
        case EKeyLeftArrow:
            {
            if( MUS_NO_TOOLBAR )
                {
            HandleCommandL(
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeDown :
                    iController->BrightnessSelected() ? EMusuiCmdViewDecreaseBrightness :
                    iController->ZoomSelected() ? EMusuiCmdViewZoomOut :
                    EMusUiCmdToolbarLastIndex );
                }
            else
                {
                HandleCommandL(
                        iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeDown :
                        iController->ToolbarBrightnessSelected() ? EMusuiCmdViewDecreaseBrightness :
                        iController->ToolbarZoomSelected() ? EMusuiCmdViewZoomOut :
                        EMusUiCmdToolbarLastIndex );
                }
            break;
            }
        case EKeyRightArrow:
            {
            if( MUS_NO_TOOLBAR )
                {
            HandleCommandL(
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeUp :
                    iController->BrightnessSelected() ? EMusuiCmdViewIncreaseBrightness :
                    iController->ZoomSelected() ? EMusuiCmdViewZoomIn :
                    EMusUiCmdToolbarLastIndex );
                }
            else
                {
                HandleCommandL(
                        iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeUp :
                        iController->ToolbarBrightnessSelected() ? EMusuiCmdViewIncreaseBrightness :
                        iController->ToolbarZoomSelected() ? EMusuiCmdViewZoomIn :
                        EMusUiCmdToolbarLastIndex );
                }
            break;
            }
        case EKeyUpArrow:
            {
            if( MUS_NO_TOOLBAR )
                {
            HandleCommandL(
                    iController->ZoomSelected() ? EMusuiCmdViewZoomIn :
                    iController->BrightnessSelected() ? EMusuiCmdViewIncreaseBrightness :
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeUp :
                    EMusUiCmdToolbarLastIndex );
                }
            else
                {
                HandleCommandL(
                        iController->ToolbarZoomSelected() ? EMusuiCmdViewZoomIn :
                        iController->ToolbarBrightnessSelected() ? EMusuiCmdViewIncreaseBrightness :
                        iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeUp :
                        EMusUiCmdToolbarLastIndex );
                }
            break;
            }
        case EKeyDownArrow:
            {
            if( MUS_NO_TOOLBAR )
                {
            HandleCommandL(
                    iController->ZoomSelected() ? EMusuiCmdViewZoomOut :
                    iController->BrightnessSelected() ? EMusuiCmdViewDecreaseBrightness :
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeDown :
                    EMusUiCmdToolbarLastIndex );
                }
            else
                {
                HandleCommandL(
                        iController->ToolbarZoomSelected() ? EMusuiCmdViewZoomOut :
                        iController->ToolbarBrightnessSelected() ? EMusuiCmdViewDecreaseBrightness :
                        iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeDown :
                        EMusUiCmdToolbarLastIndex );
                }
            break;
            }
        case EMusuiCmdViewVolumeDown:
            {
            HandleCommandL( EMusuiCmdViewVolumeDown );
            break;
            }
        case EMusuiCmdViewVolumeUp:
            {
            HandleCommandL( EMusuiCmdViewVolumeUp );
            break;
            }
        default:
            {
            break;
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::HandleToolbarCommandL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::SetInitialFocusedItemL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> MusUiLiveSharingView::SetInitialFocusedItemL" );

    __ASSERT_ALWAYS( iController, User::Leave( KErrNotReady ) );

    if ( iOperatorSpecificFunctionality  && !AknLayoutUtils::PenEnabled() )
        {
        MUS_LOG( "Focus changed in OPERATOR mode !!!!!!!!!!!!!" );
        Toolbar()->SetFocusedItemL( EMusuiCmdToolbarZoom );
        
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::SetInitialFocusedItemL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::DynInitToolbarL( TInt aResourceId,
                                             CAknToolbar* aToolbar )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::DynInitToolbarL" );

    // Items to be removed in Live Sharing View:    
 	if (!AknLayoutUtils::PenEnabled())//(aResourceId == R_MUS_TOOLBAR_NONTOUCH)//
	{
    	Toolbar()->RemoveItem( EMusuiCmdToolbarFFRev );
  		
		// Items to be removed in Live Sharing View:
    	if ( iController->DeviceHasDedicatedVolumeKeys() )
        	{
       		Toolbar()->RemoveItem( EMusuiCmdToolbarVolume );
        	}
    	else
        	{
        	aToolbar->SetFocusedItemL( EMusuiCmdToolbarVolume );
        	}

        
   		if ( !iController->IsBrightnessSupported() )
        	{
        	Toolbar()->HideItem( EMusuiCmdToolbarBrightness, ETrue, ETrue );
        	}
   	 	else
        	{
        	if ( aToolbar->FocusedItem() == KErrNotFound )
            	{
            	aToolbar->SetFocusedItemL( EMusuiCmdToolbarBrightness );
            	}
        	}                      
	}
    Toolbar()->RemoveItem( EMusuiCmdToolbarPause );
    Toolbar()->RemoveItem( EMusuiCmdToolbarUnPause );
    Toolbar()->RemoveItem( EMusuiCmdToolbarFullScreen );

    if ( iOperatorSpecificFunctionality )
        {
        // Remove pause/unpause functionality in operator variant case
        Toolbar()->RemoveItem( EMusuiCmdToolbarPauseLive );
        Toolbar()->RemoveItem( EMusuiCmdToolbarUnPauseLive );            
        if ( (aToolbar->FocusedItem() == KErrNotFound ) &&
			 !AknLayoutUtils::PenEnabled() )
            {
            aToolbar->SetFocusedItemL( EMusuiCmdToolbarZoom );
            }
        }
    else        
        {
        // Toggled items to be hidden:
        Toolbar()->HideItem( iPauseSelected ?
                                EMusuiCmdToolbarPauseLive :
                                EMusuiCmdToolbarUnPauseLive,
                             ETrue,
                             ETrue ); 
        if ( aToolbar->FocusedItem() == KErrNotFound &&
             !AknLayoutUtils::PenEnabled() )
            {
            aToolbar->SetFocusedItemL( EMusuiCmdToolbarPauseLive );
            }                                
        }
                        
    // Last generic base class functionality
    CMusUiGeneralView::DynInitToolbarL( aResourceId, aToolbar );

    MUS_LOG1( "aToolbar Focused Item: %d", aToolbar->FocusedItem() );
    MUS_LOG1( "toolbar() Focused Item: %d", Toolbar()->FocusedItem() );
                  
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::DynInitToolbarL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::SetZoomValues( TInt aMinZoom, TInt aMaxZoom )
    {
    iContainer->SetZoomValues( aMinZoom, aMaxZoom );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::SetBrightnessValues( TInt aMinBrightness,
                                                 TInt aMaxBrightness )
    {
    iContainer->SetBrightnessValues( aMinBrightness, aMaxBrightness );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::SetZoomL( TInt aZoomLevel )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::SetZoomL" );
    iContainer->SetZoomL( aZoomLevel );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::SetZoomL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::SetBrightnessL( TInt aBrightnessLevel )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::SetBrightnessL" );
    iContainer->SetBrightnessL( aBrightnessLevel );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::SetBrightnessL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::SetZoomVisible( TBool aVisible )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::SetZoomVisible" );
    
    iContainer->SetZoomVisible( aVisible );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::SetZoomVisible" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::SetBrightnessVisible( TBool aVisible )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::SetBrightnessVisible" );
    
    iContainer->SetBrightnessVisible( aVisible );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::SetBrightnessVisible" );
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::LevelIndicatorDismissed()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::LevelIndicatorDismissed" );
        iController->LevelIndicatorDismissed();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::LevelIndicatorDismissed" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::RefreshAudioRoutingButton()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::RefreshAudioRoutingButton" );
    //We ignore audio routing change event in case there is Zoom/Brightness
    //adjustment ongoing in operator variant. Middle softkey will be
    //updated once adjustment is done    
    if ( !MUS_NO_TOOLBAR ||
         ( !iController->ZoomSelected() &&
         !iController->BrightnessSelected() ) )
        {
        CMusUiGeneralView::RefreshAudioRoutingToolbarButton();        
        }
    
   MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::RefreshAudioRoutingButton" );
   }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::ReplaceToolbarCommand( TInt aOldCommand, 
                                                   TInt aNewCommand,
                                                   TBool aSetNewCommandFocused )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiLiveSharingView::ReplaceToolbarCommand: %d",
                            aOldCommand );
    switch ( aOldCommand )
        {
        case EMusuiCmdToolbarPauseLive:
            {            
            iPauseSelected = ETrue;
            /* Hide pause related toolbar items */
            HideToolbarCommandForPause(ETrue);
            break;
            }
        case EMusuiCmdToolbarUnPauseLive:
            {
            iPauseSelected = EFalse;
            /* Unhide pause related toolbar items */
            HideToolbarCommandForPause(EFalse);
            break;
            }
        default:
            {
            break;
            }
        }
    
    // Call base class version for generic part
    
    CMusUiGeneralView::ReplaceToolbarCommand( aOldCommand, 
                                              aNewCommand,
                                              aSetNewCommandFocused );
    
    MUS_LOG1( "mus: [MUSUI ]  <- CMusUiLiveSharingView::ReplaceToolbarCommand: %d",
                            aNewCommand );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::RefreshView( TBool aLayoutChange )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::RefreshView" );
    if ( iController )
        {
		if ( aLayoutChange && iController->IsSessionEstablished() ) 
			{
			iController->AsyncRefreshView();
			}
		else
			{
			DoRefreshView();
			} 

        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::RefreshView" );
    }

 
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::DoRefreshView()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::DoRefreshView" );
    if ( iContainer )
        {
        TRect containerRect( ClientRect().iBr.iX - KMusUiContainerWidth, 
                             ClientRect().iTl.iY, 
                             ClientRect().iBr.iX,
                             ClientRect().iBr.iY - ToolbarPlaceHolderHeight() );
        iContainer->SetRect( containerRect );    
        }
    
    if ( iController )
        {
        TRect videoRect( ClientRect().iTl.iX,
                         ClientRect().iTl.iY,
                         ClientRect().iBr.iX,
                         ClientRect().iBr.iY - ToolbarPlaceHolderHeight() );        
        // if this function leaves we can nothing to do for camera orientation.
        // Alteast we can try further to set right rectangle for display
        TRAPD(err,RefreshCameraOrientationL(videoRect));       
        if( err )
            {
            MUS_LOG1( "mus: [MUSUI ]  -> Orientation can not be changed =%d",err );
            }                        
        iController->SetRect( videoRect );
        }
            
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::DoRefreshView" );
    } 


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiSendController* CMusUiLiveSharingView::SendController() const
    {
    return iController;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiSendViewContainer* CMusUiLiveSharingView::SendContainer() const
    {
    return iContainer;
    }


// -----------------------------------------------------------------------------
// From CAknView, Gets called from framework when activating this view
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::DoActivateL( const TVwsViewId& aPrevViewId,
                                         TUid aCustomMessageId,
                                         const TDesC8& aCustomMessage )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::DoActivateL" );

    // Base class activation first
    CMusUiSendView::DoActivateL( aPrevViewId, 
                                 aCustomMessageId,
                                 aCustomMessage );
    
    TRect clientRect = ClientRect(); 
    clientRect.iBr.iX -= ToolbarPlaceHolderWidth(); 
    clientRect.iBr.iY -= ToolbarPlaceHolderHeight(); 

    TRect containerRect( clientRect.iBr.iX - KMusUiContainerWidth, 
    		             clientRect.iTl.iY, 
    		             clientRect.iBr.iX,
    		             clientRect.iBr.iY );

    if ( !iContainer )
        {
        MUS_LOG( "mus: [MUSUI ]  CMusUiLiveSharingView::DoActivateL: !iContainer" );
        iContainer = new ( ELeave ) CMusUiLiveSharingViewContainer();
        iContainer->SetMopParent( this );
        iContainer->ConstructL( this,
                                containerRect );
        AppUi()->AddToViewStackL( *this, iContainer );
        }

    /* Increase the ordinal position of background container
     * So that other control will be drawn first.     
     */
    UpdateBackgroundOrdinalPosition( ETrue );
    
    TRect videoRect( ClientRect().iTl.iX,
                     ClientRect().iTl.iY,
                     ClientRect().iBr.iX,
                     ClientRect().iBr.iY - ToolbarPlaceHolderHeight() );

    if ( !iController )
        {
        MUS_LOG( "mus: [MUSUI ]  CMusUiLiveSharingView::DoActivateL: !iController" );
        iController = CMusUiLiveSharingController::NewL( *MusAppUi(),
                                                         *this, 
                                                         *this,
                                                         *this,
                                                         videoRect );
        iController->StartInvitationL();
        }

    iContainer->SetController( iController );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::DoActivateL" );
    }


// -----------------------------------------------------------------------------
// From AknView, Gets called from framework when deactivating this view
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::DoDeactivate()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::DoDeactivate" );

    CMusUiSendView::DoDeactivate(); // Base class deactivation first

    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::DoDeactivate" );
    }

// -----------------------------------------------------------------------------
// When orientation changed , Camera instances has to be recreated inorder
// to receive proper orientated frames.
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::RefreshCameraOrientationL(TRect& aNewRect)
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingView::RefreshCameraOrientation" );
    if ( !iController || !iController->EngineSession() )
        {        
        MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::RefreshCameraOrientation, no session" );
        return;
        }
        TRect oldRect = iController->EngineSession()->Rect();
        MUS_LOG2( "mus: [MUSUI ]  -> Old Rect (width = %d,height=%d)",oldRect.Width(),oldRect.Height() );
        MUS_LOG2( "mus: [MUSUI ]  -> New Rect (width = %d,height=%d)",aNewRect.Width(),aNewRect.Height() );
        if ( !( (  ( oldRect.Width() > oldRect.Height() ) && //check lanscape
                   ( aNewRect.Width() > aNewRect.Height() )
                )
                ||
                (  
                   ( oldRect.Width() < oldRect.Height() ) && // check portrait
                   ( aNewRect.Width() < aNewRect.Height() )
                )
              )
           )
           {
           MUS_LOG( "mus: [MUSUI ]  -> Orientation Changed.Restart Camera" );
           iController->RefreshCameraOrientationL();
           }  
        else
           {
           MUS_LOG( "mus: [MUSUI ]  -> Orientation Not changed. dont restart camera");    
           }          
       


    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::RefreshCameraOrientation" );
    }

// -----------------------------------------------------------------------------
// When Live sharing paused , camera source will be unavilable, hence 
// it is inappropriate to show the zoom and brightness
// so hide those item. And unhide it again when unpaused.
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingView::HideToolbarCommandForPause( TBool aIsPaused )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiLiveSharingView::HideToolbarCommandForPause() %d",aIsPaused); 
    /* Hide brightness toolbar item if paused else unhide. Draw immediately.*/
    Toolbar()->HideItem( EMusuiCmdToolbarBrightness, aIsPaused, ETrue );
    /* Hide zoom toolbar item if paused else unhide. Draw immediately.*/
    Toolbar()->HideItem( EMusuiCmdToolbarZoom, aIsPaused, ETrue );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::HideToolbarCommandForPause");
    }

// end of file
