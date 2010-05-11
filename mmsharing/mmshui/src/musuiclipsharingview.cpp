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



#include "musuiclipsharingview.h"
#include "musuiclipsharingviewcontainer.h"
#include "musuibackgroundviewcontainer.h"
#include "musuiclipsharingcontroller.h"
#include "musuiactivitymanger.h"
#include "musuiappui.h"
#include "musuistatuspanehandler.h"
#include "musui.hrh"
#include "musuidefinitions.h"
#include "muslogger.h" // debug logging
#include <musui.rsg>
#include <musui.mbg>

#include <aknViewAppUi.h>
#include <avkon.hrh>
#include <eikmenup.h>
#include <eikbtgpc.h>
#include <akntoolbar.h>


const TInt KMusUiContainerHeight = 50;
const TInt KDefaultResolutionHeight = 640;

// -----------------------------------------------------------------------------
// EPOC two-phased constructor
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingView::ConstructL" );

    CMusUiSendView::ConstructL();

    Toolbar()->SetToolbarObserver( this );
    Toolbar()->UpdateBackground();

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingView::ConstructL" );
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMusUiClipSharingView::~CMusUiClipSharingView()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingView::~CMusUiClipSharingView" );
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    delete iContainer;
    
    delete iController;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingView::~CMusUiClipSharingView" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TUid CMusUiClipSharingView::Id() const
    {
    return KMusUidClipSharingView;
    }

// -----------------------------------------------------------------------------
// From MEikMenuObserver, Called by framework before constructing menupane
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::DynInitMenuPaneL( TInt aResourceId,
                                              CEikMenuPane* aMenuPane )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingView::DynInitMenuPaneL" );
    
 
    // Base class initiated initialization first
    CMusUiGeneralView::DynInitMenuPaneL( aResourceId, aMenuPane );
    
    if(! MUS_NO_TOOLBAR )
        {
            SetInitialFocusedItemL();        
        }
    
    if ( aResourceId == R_MUSUI_VIEW_MENU )
        {
        
        // Delete items not related to clip sharing:
        aMenuPane->DeleteMenuItem( EMusuiCmdRecord );
        aMenuPane->DeleteMenuItem( EMusuiCmdStopRecording );        

        aMenuPane->DeleteMenuItem( EMusuiCmdToolbarZoom );
        aMenuPane->DeleteMenuItem( EMusuiCmdToolbarBrightness );
        aMenuPane->DeleteMenuItem( EMusuiCmdViewZoom );
        aMenuPane->DeleteMenuItem( EMusuiCmdViewBrightness );
        aMenuPane->DeleteMenuItem( EMusuiCmdViewFullScreen );       
        
        // Delete EMusuiCmdViewAudioNormal or EMusuiCmdViewAudioMute
        // depending on the audio state of the clip:
        if ( iController->ClipContainsAudioL() )
            {
            aMenuPane->DeleteMenuItem( iController->ClipMutedL() ? 
                                        EMusuiCmdViewAudioMute : 
                                        EMusuiCmdViewAudioNormal );
            }
         else
            {
            aMenuPane->DeleteMenuItem( EMusuiCmdViewAudioMute );
            aMenuPane->DeleteMenuItem( EMusuiCmdViewAudioNormal );
            }
        
        // Delete "Replay" item, if the end of the clip not reached:
        if ( !iController->ClipEnded() )
            {
            aMenuPane->DeleteMenuItem( EMusuiCmdViewReplay );
            }

        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingView::DynInitMenuPaneL" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::HandleToolbarCommandL( TInt aCommand )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingView::HandleToolbarCommandL" );

     __ASSERT_ALWAYS( iController, User::Leave( KErrNotReady ) );

    switch ( aCommand )
        {
        case EKeyLeftArrow:
            {
            HandleCommandL(
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeDown :
                    iController->ToolbarFFRevSelected() ? EMusuiCmdViewFastReverse :
                    EMusUiCmdToolbarLastIndex );
            break;
            }
        case EKeyRightArrow:
            {
            HandleCommandL(
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeUp :
                    iController->ToolbarFFRevSelected() ? EMusuiCmdViewFastForward :
                    EMusUiCmdToolbarLastIndex );
            break;
            }
        case EKeyUpArrow:
            {
            HandleCommandL(
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeUp :
                    iController->ToolbarFFRevSelected() ? EMusuiCmdViewFastForward :
                    EMusUiCmdToolbarLastIndex );
            break;
            }
        case EKeyDownArrow:
            {
            HandleCommandL(
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeDown :
                    iController->ToolbarFFRevSelected() ? EMusuiCmdViewFastReverse :
                    EMusUiCmdToolbarLastIndex );
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
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingView::HandleToolbarCommandL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::HandleKeyUpEvent( TInt /*aKeyCode*/ )
    {
    TRAP_IGNORE( iController->StopWindingL() )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::SetInitialFocusedItemL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingView::SetInitialFocusedItemL" );

    __ASSERT_ALWAYS( iController, User::Leave( KErrNotReady ) );

    if ( iOperatorSpecificFunctionality && !AknLayoutUtils::PenEnabled() )
        {
        MUS_LOG( "Focus changed in OPERATOR mode !!!!!!!!!!!!!" );
  
        if ( !( iController->DeviceHasDedicatedVolumeKeys() ) )
            {
            Toolbar()->SetFocusedItemL( EMusuiCmdToolbarVolume );
            }
        else
            {
      
	        Toolbar()->SetFocusedItemL( EventControllerL().IsMicMutedL() ?
                                                    EMusuiCmdToolbarUnmute :
                                                    EMusuiCmdToolbarMute );
    		}        
        
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingView::SetInitialFocusedItemL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::DynInitToolbarL( TInt aResourceId,
                                             CAknToolbar* aToolbar )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingView::DynInitToolbarL" );

    // Items to be removed in Clip Sharing View:
    

 	if (!AknLayoutUtils::PenEnabled())//(aResourceId == R_MUS_TOOLBAR_NONTOUCH)//
	{
   		if ( iController->DeviceHasDedicatedVolumeKeys() )
        	{
        	aToolbar->RemoveItem( EMusuiCmdToolbarVolume );
        	}
    	else
        	{
        	aToolbar->SetFocusedItemL( EMusuiCmdToolbarVolume );
        	}        	
	    
		aToolbar->RemoveItem( EMusuiCmdToolbarZoom );
    	aToolbar->RemoveItem( EMusuiCmdToolbarBrightness );
	}

    Toolbar()->RemoveItem( EMusuiCmdToolbarPauseLive );
    Toolbar()->RemoveItem( EMusuiCmdToolbarUnPauseLive );
    Toolbar()->RemoveItem( EMusuiCmdToolbarFullScreen );
    
    
    if ( iOperatorSpecificFunctionality )
        {
        // Remove pause/unpause functionality in operator variant case
        Toolbar()->RemoveItem( EMusuiCmdToolbarPause );
        Toolbar()->RemoveItem( EMusuiCmdToolbarUnPause );
        
        if ( aToolbar->FocusedItem() == KErrNotFound &&
             !AknLayoutUtils::PenEnabled() )
            {
            aToolbar->SetFocusedItemL( EventControllerL().IsMicMutedL() ?
                                                    EMusuiCmdToolbarUnmute :
                                                    EMusuiCmdToolbarMute );
            }                

        }
    else        
        {
        // Toggled items to be hidden:
        Toolbar()->HideItem( iPauseSelected ?
                                EMusuiCmdToolbarPause :
                                EMusuiCmdToolbarUnPause,
                             ETrue,
                             ETrue );
        if ( aToolbar->FocusedItem() == KErrNotFound &&
             !AknLayoutUtils::PenEnabled() )
            {
            aToolbar->SetFocusedItemL( EMusuiCmdToolbarPause );
            }                               
        }
            
    // Last generic base class functionality
    CMusUiGeneralView::DynInitToolbarL( aResourceId, aToolbar );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingView::DynInitToolbarL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::RunFetchDialogL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingView::RunFetchDialogL" );
    iContainer->RunFetchDialogL();
    if ( iContainer->FileSelected() )
        {
        iController->SetFileNameL( iContainer->FetchedFileName() );
		RefreshView();
        }
    else
        {
        MusAppUi()->HandleExit();
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingView::RunFetchDialogL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::ShowTranscodingProgressDialogL()
    {
    iContainer->ShowTranscodingProgressDialogL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::UpdateClipTranscodingPercentageL( TInt aPercetage )
    {
    iContainer->UpdateClipTranscodingPercentageL( aPercetage );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::CancelTranscodingDialogL()
    {
    iContainer->CancelTranscodingL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::UpdateClipPositionL( 
                                        TInt aPositionInSeconds,
                                        const TDesC& aFormattedPosition )
    {
    MUS_LOG_TDESC( "mus: [MUSUI ] -> CMusUiClipSharingView::UpdateClipPositionL: ",
                   aFormattedPosition );
    
    MusAppUi()->MusStatusPane()->SetRightLabelL( aFormattedPosition );
    iContainer->SetPositionValueL( aPositionInSeconds );
        
    MUS_LOG( "mus: [MUSUI ] <- CMusUiClipSharingView::UpdateClipPositionL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::SetDurationValue( TInt aDurationValue )
    {
    iContainer->SetDurationValue( aDurationValue );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::SetPositionValueL( TInt aDurationValue )
    {
    iContainer->SetPositionValueL( aDurationValue );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::SetDurationVisible()
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiLiveSharingView::SetDurationVisible: %d",
                            iToolbarItemSelected );
    if ( !iToolbarItemSelected )
        {
        iContainer->SetDurationIndicatorVisible( ETrue );
        }
    else
        {
        iContainer->SetDurationIndicatorVisible( EFalse );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingView::SetDurationVisible" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::ReplaceToolbarCommand( TInt aOldCommand, 
                                                   TInt aNewCommand,
                                                   TBool aSetNewCommandFocused )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiClipSharingView::ReplaceToolbarCommand: %d",
                            aOldCommand );
    switch ( aOldCommand )
        {
        case EMusuiCmdToolbarPause:
            {
            iPauseSelected = ETrue;
            break;
            }
        case EMusuiCmdToolbarUnPause:
            {
            iPauseSelected = EFalse;
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
    
    MUS_LOG1( "mus: [MUSUI ]  <- CMusUiClipSharingView::ReplaceToolbarCommand: %d",
                            aOldCommand );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::RefreshView( TBool aLayoutChange )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingView::RefreshView" );
    

    if ( iContainer )
        {            
        TInt height = MusAppUi()->ApplicationRect().Height();
        MUS_LOG1( "mus: [MUSUI ]  -> application rect height: %d", height );
        TInt containerHeight = (KMusUiContainerHeight * height) / KDefaultResolutionHeight;

        MUS_LOG1( "mus: [MUSUI ]  -> container rect height: %d", containerHeight );
        TRect containerRect( 
                        ClientRect().iTl.iX, 
                        ClientRect().iBr.iY - ToolbarPlaceHolderHeight() 
                                            - containerHeight,//KMusUiContainerHeight,
                        ClientRect().iBr.iX,
                        ClientRect().iBr.iY - ToolbarPlaceHolderHeight() );

        if (!AknLayoutUtils::PenEnabled())
        	{
        	containerRect = TRect(
        			ClientRect().iTl.iX,
        			ClientRect().iTl.iY,
        			ClientRect().iBr.iX,
        			ClientRect().iTl.iY +  containerHeight );
        	}
        iContainer->SetRect( containerRect );
   
        if ( iController )
            {
            
            TRect videoRect( ClientRect().iTl.iX,
                     ClientRect().iTl.iY,
                     ClientRect().iBr.iX,
                     ClientRect().iBr.iY - ToolbarPlaceHolderHeight()
                                         - iContainer->Size().iHeight );

             if (!AknLayoutUtils::PenEnabled())
             	{
             	TInt dy = iContainer->Size().iHeight;
             	videoRect.Move(0, dy);
            	}
            iController->SetRect( videoRect );

            }
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingView::RefreshView" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::CancelTranscodingL()
    {
    if ( iController )
        {
        iController->CancelTranscodingL();
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiSendController* CMusUiClipSharingView::SendController() const
    {
    return iController;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiSendViewContainer* CMusUiClipSharingView::SendContainer() const
    {
    return iContainer;
    }


// -----------------------------------------------------------------------------
// From CAknView, Gets called from framework when activating this view
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::DoActivateL( const TVwsViewId& aPrevViewId,
                                         TUid aCustomMessageId,
                                         const TDesC8& aCustomMessage )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingView::DoActivateL" );

    // Base class activation first
    CMusUiSendView::DoActivateL( aPrevViewId,
                                 aCustomMessageId,
                                 aCustomMessage );
        
    TInt height = MusAppUi()->ApplicationRect().Height();
    MUS_LOG1( "mus: [MUSUI ]  -> application rect height: %d", height );
    TInt containerHeight = (KMusUiContainerHeight * height) / KDefaultResolutionHeight;

    MUS_LOG1( "mus: [MUSUI ]  -> container rect height: %d", containerHeight );  
    TRect containerRect( 
        ClientRect().iTl.iX, 
        ClientRect().iBr.iY - ToolbarPlaceHolderHeight() 
                            - containerHeight,//KMusUiContainerHeight,
        ClientRect().iBr.iX,
        ClientRect().iBr.iY - ToolbarPlaceHolderHeight() );
                         
   if (!AknLayoutUtils::PenEnabled())
        {
    	containerRect = TRect(
    			ClientRect().iTl.iX,
    			ClientRect().iTl.iY,
    			ClientRect().iBr.iX,
    			ClientRect().iTl.iY +  containerHeight );
    	}
 
    if ( !iContainer )
        {
        MUS_LOG( "mus: [MUSUI ]  CMusUiClipSharingView::DoActivateL: !iContainer" );
        iContainer = new ( ELeave ) CMusUiClipSharingViewContainer();
        iContainer->SetMopParent( this );
        iContainer->ConstructL( this, containerRect );
        AppUi()->AddToViewStackL( *this, iContainer );
        }

    /* Increase the ordinal position of background container
     * So that other control will be drawn first.     
     */
    UpdateBackgroundOrdinalPosition( ETrue );
        
    TRect videoRect( ClientRect().iTl.iX,
                     ClientRect().iTl.iY,
                     ClientRect().iBr.iX,
                     ClientRect().iBr.iY - ToolbarPlaceHolderHeight() 
                                         - iContainer->Size().iHeight );

    if (!AknLayoutUtils::PenEnabled())
    	{
    	TInt dy = iContainer->Size().iHeight;
    	videoRect.Move(0, dy);
    	}
    if ( !iController )
        {
        MUS_LOG( "mus: [MUSUI ]  CMusUiClipSharingView::DoActivateL: !iController" );
        iController = CMusUiClipSharingController::NewL( *MusAppUi(),
                                                         *this, 
                                                         *this,
                                                         *this,
                                                         videoRect );
        }
    
    iContainer->SetController( iController );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingView::DoActivateL" );
    }


// -----------------------------------------------------------------------------
// From AknView, Gets called from framework when deactivating this view
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingView::DoDeactivate()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingView::DoDeactivate" );
    
    CMusUiSendView::DoDeactivate(); // Base class deactivation first
    
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingView::DoDeactivate" );
    }


// end of file
