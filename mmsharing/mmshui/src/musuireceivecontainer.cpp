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


#include "musuireceivecontainer.h"
#include "musuieventcontroller.h"
#include "musuigeneralview.h"
#include "musuireceiveview.h"
#include "musuiinvitationquery.h"
#include "musuiinvitationqueryobserver.h"
#include "musuiappui.h"
#include "musuidialogutil.h"
#include "musuidefinitions.h"
#include "musui.hrh"
#include "muslogger.h" // debug logging
#include <musui.rsg>

#include <AknsControlContext.h>
#include <AknsBasicBackgroundControlContext.h>
#include <AknsDrawUtils.h>
#include <AknWaitDialog.h>
#include <AknDef.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiReceiveContainer* CMusUiReceiveContainer::NewL(
                                CMusUiReceiveView& aView,
                                const TRect aRect )
    {
    CMusUiReceiveContainer* self
            = new( ELeave ) CMusUiReceiveContainer( aView );

    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );

    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiReceiveContainer::~CMusUiReceiveContainer()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveContainer::~CMusUiReceiveContainer" );

    if ( iInvitationQuery )
        {
        iInvitationQuery->Cancel();
        delete iInvitationQuery;
        iInvitationQuery = NULL;
        }

    delete iBackGround;
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveContainer::~CMusUiReceiveContainer" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiReceiveContainer::CMusUiReceiveContainer( CMusUiReceiveView& aView )
    : iView( aView )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveContainer::ConstructL( const TRect& aRect )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveContainer::ConstructL" );
    CreateWindowL();
    iBackGround = CAknsBasicBackgroundControlContext::NewL(
                             KAknsIIDQsnBgAreaMain,
                             aRect,
                             EFalse );
    iFullScreen = EFalse;
    SetRect( aRect );
    ActivateL();
    
    iOperatorSpecificFunctionality = iView.OperatorSpecificFunctionality();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveContainer::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveContainer::ShowInvitationQueryL(
                                    const TDesC& aContact,
                                    MMusUiInvitationQueryObserver& aObserver )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveContainer::ShowInvitationQueryL" );
    if ( !iInvitationQuery )
        {
        iInvitationQuery = CMusUiInvitationQuery::NewL( aObserver );
        }
    iInvitationQuery->ShowL( aContact );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveContainer::ShowInvitationQueryL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveContainer::DismissInvitationQuery( TBool aDelete )
    {
    if ( iInvitationQuery )
        {
        iInvitationQuery->Cancel();
        if ( aDelete )
            {
            delete iInvitationQuery;        
            iInvitationQuery = NULL;
            }
        }
    }
    

// -----------------------------------------------------------------------------
// Fetches a pointer to the view that owns this container.
// -----------------------------------------------------------------------------
//
CMusUiGeneralView* CMusUiReceiveContainer::MusUiView() const
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveContainer::MusUiView" );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveContainer::MusUiView" );
    return &iView;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveContainer::SetFullScreen( TBool aFullScreen, TRect aRect )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiReceiveContainer::SetFullScreen: %d",
                            aFullScreen );
    iFullScreen = aFullScreen;
    
    if ( aFullScreen )
        {
        // In order to capture pointer events this container must be visible.
        // Thus we make it as small as possible.
        SetRect( TRect( 0, 0, 0, 0) );
        MakeVisible( ETrue );
        SetGloballyCapturing( ETrue );
        SetPointerCapture( ETrue );    
        }
    else
        {
        SetRect( aRect );
        SetPointerCapture( EFalse );
        MakeVisible( EFalse );
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveContainer::SetFullScreen" );
    }


// -----------------------------------------------------------------------------
// Called by framework when the view size is changed
// -----------------------------------------------------------------------------
//
void CMusUiReceiveContainer::SizeChanged()
    {
    TRect parentRect(Rect());
    if (iBackGround)
        {
        (static_cast<CAknsBasicBackgroundControlContext*>(iBackGround))->SetRect(parentRect);
        }
    MakeVisible( EFalse );
    }


// -----------------------------------------------------------------------------
// Called by framework when to retrieve the no. of controls.
// -----------------------------------------------------------------------------
//
TInt CMusUiReceiveContainer::CountComponentControls() const
    {
    return 0; // return nbr of controls inside this container
    }


// -----------------------------------------------------------------------------
// Called by framework to retrieve the control specified by index.
// -----------------------------------------------------------------------------
//
CCoeControl* CMusUiReceiveContainer::ComponentControl( TInt aIndex ) const
    {
    switch ( aIndex )
        {
        default:
            return NULL;
        }
    }


// -----------------------------------------------------------------------------
// Called by framework to redraw the screen area.
// -----------------------------------------------------------------------------
//
void CMusUiReceiveContainer::Draw( const TRect& aRect ) const
    {
    CWindowGc& gc = SystemGc();

    gc.Clear( aRect );
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );
    if(!AknsDrawUtils::Background( skin, cc,this,gc,aRect,KAknsDrawParamDefault ))
        {
        gc.SetBrushColor( iEikonEnv->ControlColor( EColorWindowBackground, *this ) );
        gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
        gc.Clear( aRect );
        }
    }


// -----------------------------------------------------------------------------
// Called by framework
// -----------------------------------------------------------------------------
//
TKeyResponse CMusUiReceiveContainer::OfferKeyEventL( const TKeyEvent& aEvent,
                                                     TEventCode aType )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveContainer::OfferKeyEventL" );
    
    if ( iFullScreen && aType == EEventKey )
        {
        // We respond to whatever key by returning from fullscreen
        if( MUS_NO_TOOLBAR )
            {
            (static_cast <CMusUiReceiveView*> 
                (MusUiView()))->HandleCommandL( EMusuiCmdViewFullScreen );            
            }
        else
            {
            (static_cast <CMusUiReceiveView*> 
                (MusUiView()))->HandleCommandL( EMusuiCmdToolbarFullScreen );
            }
         return EKeyWasConsumed;
        }
        
    switch ( aEvent.iCode )
        {
        // Volume Control
        case EKeyLeftArrow:
            {
            (static_cast <CMusUiReceiveView*> 
                    (MusUiView()))->HandleCommandL( EMusuiCmdViewVolumeDown );
            return EKeyWasConsumed;
            }
        case EKeyRightArrow:
            {
            (static_cast <CMusUiReceiveView*> 
                    (MusUiView()))->HandleCommandL( EMusuiCmdViewVolumeUp );
            return EKeyWasConsumed;
            }

        //  TODO:
        case EKeyUpArrow:
            {
            break;
            }
        case EKeyDownArrow:
            {
            break;
            }
        default:
            return EKeyWasNotConsumed;
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveContainer::OfferKeyEventL" );
    return EKeyWasNotConsumed;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveContainer::HandlePointerEventL( const TPointerEvent& )
    {
    if ( iFullScreen )
        {
        // We respond to whatever pointer event by returning from fullscreen
        (static_cast <CMusUiReceiveView*> 
                (MusUiView()))->HandleCommandL( EMusuiCmdToolbarFullScreen );
        }
    }
	
	
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiReceiveContainer::UpdatePointerCapture()
	{
	MUS_LOG( "mus: [MUSUI ]  -> CMusUiReceiveContainer::UpdatePointerCapture" );
	
	if ( iFullScreen )
		{
		// In order to capture pointer events this container must be visible.
		// Thus we make it as small as possible.
		SetRect( TRect( 0, 0, 0, 0 ) );
		MakeVisible( ETrue );
		SetGloballyCapturing( ETrue );
		SetPointerCapture( ETrue );    	
		}
	
	MUS_LOG( "mus: [MUSUI ]  <- CMusUiReceiveContainer::UpdatePointerCapture" );
	}
	
// end of file
