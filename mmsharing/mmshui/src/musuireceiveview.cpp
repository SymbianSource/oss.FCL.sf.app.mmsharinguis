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



#include "musuireceiveview.h"
#include "musuireceivecontainer.h"
#include "musuibackgroundviewcontainer.h"
#include "musuireceivecontroller.h"
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
#include <akntoolbar.h>


// -----------------------------------------------------------------------------
// EPOC two-phased constructor
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::ConstructL" );
    
    CMusUiGeneralView::ConstructL();

    Toolbar()->SetToolbarObserver( this );
    Toolbar()->UpdateBackground();

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveView::ConstructL" );
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMusUiReceiveView::~CMusUiReceiveView()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::~CMusUiReceiveView" );
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        }
    delete iContainer;

    delete iController;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveView::~CMusUiReceiveView" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TUid CMusUiReceiveView::Id() const
    {
    return KMusUidReceivingView;
    }

// -----------------------------------------------------------------------------
// From MEikMenuObserver, Called by framework before constructing menupane
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::DynInitMenuPaneL( TInt aResourceId,
                                          CEikMenuPane* aMenuPane )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::DynInitMenuPaneL" );
    
    // Base class initiated initialization first
    CMusUiGeneralView::DynInitMenuPaneL( aResourceId, aMenuPane );
    if(!MUS_NO_TOOLBAR)
        {
        SetInitialFocusedItemL();
        }
    
    if ( aResourceId == R_MUSUI_VIEW_MENU )
        {
        // Delete item which is present on toolbar
        if ( !MUS_NO_TOOLBAR )
            {
            aMenuPane->DeleteMenuItem( EMusuiCmdViewFullScreen );
            }

        // Delete items not related to receiving side:
        aMenuPane->DeleteMenuItem( EMusuiCmdViewReplay );
        aMenuPane->DeleteMenuItem( EMusuiCmdToolbarZoom );
        aMenuPane->DeleteMenuItem( EMusuiCmdToolbarBrightness );
        aMenuPane->DeleteMenuItem( EMusuiCmdViewZoom );
        aMenuPane->DeleteMenuItem( EMusuiCmdViewBrightness );
        aMenuPane->DeleteMenuItem( EMusuiCmdRecord );
        aMenuPane->DeleteMenuItem( EMusuiCmdStopRecording );
        
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
        
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveView::DynInitMenuPaneL" );
    }
 
    
// -----------------------------------------------------------------------------
// Called when focus event occurs.
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::DoHandleForegroundEventL( TBool aForeground )
    {
    CMusUiGeneralView::HandleForegroundEventL( aForeground, EFalse );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::HandleToolbarCommandL( TInt aCommand )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::HandleToolbarCommandL" );
    
    __ASSERT_ALWAYS( iController, User::Leave( KErrNotReady ) );
    
    switch ( aCommand )
        {
        case EKeyLeftArrow:
            {
            HandleCommandL( 
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeDown :
                    EMusUiCmdToolbarLastIndex );
            break;
            }
        case EKeyRightArrow:
            {
            HandleCommandL( 
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeUp :
                    EMusUiCmdToolbarLastIndex );
            break;
            }
        case EKeyUpArrow:
            {
            HandleCommandL( 
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeUp :
                    EMusUiCmdToolbarLastIndex );
            break;
            }
        case EKeyDownArrow:
            {
            HandleCommandL( 
                    iController->ToolbarVolumeSelected() ? EMusuiCmdViewVolumeDown :
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
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveView::HandleToolbarCommandL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::SetInitialFocusedItemL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::SetInitialFocusedItemL" );
    
    __ASSERT_ALWAYS( iController, User::Leave( KErrNotReady ) );
    
    if ( iOperatorSpecificFunctionality && !AknLayoutUtils::PenEnabled()  )
        {
        MUS_LOG( "Focus changed in OPERATOR mode !!!!!!!!!!!!!" );
         if ( !( iController->DeviceHasDedicatedVolumeKeys() ) )
            {
            Toolbar()->SetFocusedItemL( EMusuiCmdToolbarVolume );
            }
        else
            {
       
        	Toolbar()->SetFocusedItemL( EMusuiCmdToolbarFullScreen );    
        	}
        
        }
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::SetInitialFocusedItemL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::SetToolbarVisibility( TBool aVisible )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiReceiveView::SetToolbarVisibility: %d",
                            aVisible );
    if ( MUS_NO_TOOLBAR )
        {
        MUS_LOG( "                  Operator variant, no toolbar! Ignoring.." )
        MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveView::SetToolbarVisibility" );
        return;        
        }
    
    // Show toolbar only after established session and before of exit procedure
    if ( iController->FullScreenSelected() )
        {
        MUS_LOG( "mus: [MUSUI ] setting toolbar invisible" );
        
        Toolbar()->SetToolbarVisibility( EFalse );
        
        SharingContainer().RequestToolbarVisibilityOnceDialogDismissed( NULL );
        }
    else
        {
        CMusUiGeneralView::SetToolbarVisibility( aVisible );
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiGeneralView::SetToolbarVisibility" );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::DynInitToolbarL( TInt aResourceId, 
                                         CAknToolbar* aToolbar )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::DynInitToolbarL" );
    
    // Items to be removed in Receive View:  
 	if (!AknLayoutUtils::PenEnabled())//(aResourceId == R_MUS_TOOLBAR_NONTOUCH)//
	{
	    if ( iController->DeviceHasDedicatedVolumeKeys() )
	        {
	        MUS_LOG( "mus: [MUSUI ]     CMusUiReceiveView::DynInitToolbarL\
	                                    DeviceHasDedicatedVolumeKeys" );
	        Toolbar()->RemoveItem( EMusuiCmdToolbarVolume );
	        }
	    else
	        {
	        aToolbar->SetFocusedItemL( EMusuiCmdToolbarVolume );
	        }   

	    Toolbar()->RemoveItem( EMusuiCmdToolbarFFRev );
	    Toolbar()->RemoveItem( EMusuiCmdToolbarZoom );
	    Toolbar()->RemoveItem( EMusuiCmdToolbarBrightness);
	}
             
    Toolbar()->RemoveItem( EMusuiCmdToolbarPauseLive );
    Toolbar()->RemoveItem( EMusuiCmdToolbarUnPauseLive );
    Toolbar()->RemoveItem( EMusuiCmdToolbarPause );
    Toolbar()->RemoveItem( EMusuiCmdToolbarUnPause );
    
    if ( aToolbar->FocusedItem() == KErrNotFound && 
         !AknLayoutUtils::PenEnabled() )
        {
        aToolbar->SetFocusedItemL( EMusuiCmdToolbarFullScreen );
        }
    
    // Last generic base class functionality
    CMusUiGeneralView::DynInitToolbarL( aResourceId, aToolbar );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveView::DynInitToolbarL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::ShowInvitationQueryL( const TDesC& aQuery )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::ShowInvitationQueryL" );
    iContainer->ShowInvitationQueryL( aQuery, *iController );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveView::ShowInvitationQueryL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::DismissInvitationQuery( TBool aDelete )
    {
    iContainer->DismissInvitationQuery( aDelete );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::SetFullScreenL( TBool aFullScreen )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::SetFullScreenL" );

    if ( aFullScreen )
        {
        Cba()->MakeVisible( EFalse );
		SetToolbarVisibility( EFalse );
        MusAppUi()->StatusPane()->MakeVisible( EFalse );
        
		if( MusAppUi()->CurrentFixedToolbar() )
                MusAppUi()->CurrentFixedToolbar()->MakeVisible( EFalse );  
		
        if( MusAppUi()->CurrentPopupToolbar() )
                MusAppUi()->CurrentPopupToolbar()->MakeVisible( EFalse );    
		
        if( MusAppUi()->PopupToolbar() )
                MusAppUi()->PopupToolbar()->MakeVisible( EFalse );        
        iController->ChangeOrientationL( CAknAppUiBase::EAppUiOrientationLandscape );

        iContainer->SetFullScreen( ETrue, MusAppUi()->ApplicationRect() );
        iController->SetRect( MusAppUi()->ApplicationRect() );
        }
    else
        {
        if( MusAppUi()->CurrentFixedToolbar() )
                MusAppUi()->CurrentFixedToolbar()->MakeVisible( ETrue );   
		
        if( MusAppUi()->CurrentPopupToolbar() )
                MusAppUi()->CurrentPopupToolbar()->MakeVisible( ETrue );   
		
        if( MusAppUi()->PopupToolbar() )
                MusAppUi()->PopupToolbar()->MakeVisible( ETrue);
        // First make toolbar, cba and statuspane visible as those affect
        // to client rect
        MusAppUi()->StatusPane()->MakeVisible( ETrue );
        SetToolbarVisibility( ETrue );
        Cba()->MakeVisible( ETrue );
        
        iController->ChangeOrientationL( CAknAppUiBase::EAppUiOrientationAutomatic );
        iContainer->SetFullScreen( EFalse, ClientRect() );
        iController->SetRect( ClientRect() );
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveView::SetFullScreenL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::RefreshView( TBool aLayoutChange )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::RefreshView" );
    
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    
    TRect videoRect( ClientRect().iTl.iX,
                     ClientRect().iTl.iY,
                     ClientRect().iBr.iX,
                     ClientRect().iBr.iY - ToolbarPlaceHolderHeight() );
    
    if ( iController )
        {
        iController->SetRect( videoRect );
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveView::RefreshView" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiEventController* CMusUiReceiveView::EventController() const
    {
    return iController;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiViewContainer& CMusUiReceiveView::SharingContainer() const
    {
    return *iContainer;
    }
        

// -----------------------------------------------------------------------------
// From CAknView, Gets called from framework when activating this view
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::DoActivateL( const TVwsViewId& aPrevViewId,
                                         TUid aCustomMessageId,
                                         const TDesC8& aCustomMessage )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::DoActivateL" );

    // Base class activation first
    CMusUiGeneralView::DoActivateL( aPrevViewId,
                                    aCustomMessageId,
                                    aCustomMessage );
                                    
    if ( !iContainer )
        {
        // create the container and then add myself to the view stack
        iContainer = CMusUiReceiveContainer::NewL( *this, ClientRect() );
        iContainer->SetMopParent( this );
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
        iController = CMusUiReceiveController::NewL( *MusAppUi(),
                                                     *this, 
                                                     *this, 
                                                     videoRect );
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveView::DoActivateL" );
    }


// -----------------------------------------------------------------------------
// From AknView, Gets called from framework when deactivating this view
// -----------------------------------------------------------------------------
//
void CMusUiReceiveView::DoDeactivate()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveView::DoDeactivate" );
    
    CMusUiGeneralView::DoDeactivate(); // Base class deactivation first
    
    if ( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        iContainer = NULL;
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveView::DoDeactivate" );
    }


// end of file
