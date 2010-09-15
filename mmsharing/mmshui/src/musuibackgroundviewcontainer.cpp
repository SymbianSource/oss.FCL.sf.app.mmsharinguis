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

#include "musuibackgroundviewcontainer.h"
#include "musuiappui.h"
#include "muslogger.h" // debug logging
#include "musuieventcontroller.h"

#include <AknsControlContext.h>
#include <AknsBasicBackgroundControlContext.h>
#include <AknsDrawUtils.h>
#include <StringLoader.h>                // StringLoader
#include <aknnotewrappers.h>
#include <AknWaitDialog.h>
#include <AknGlobalNote.h>
#include <AknDef.h>


// Hidden window group. magic number 32 hiddens window
_LIT( KMusUiBackgroundWindowGroupName,"32MusUiBackground" );


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiBackgroundViewContainer* CMusUiBackgroundViewContainer::NewL( 
    const TRect& aRect, 
    TInt aOrdinalPosition )
    {
    CMusUiBackgroundViewContainer* self =
        new( ELeave )CMusUiBackgroundViewContainer( aOrdinalPosition );
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiBackgroundViewContainer::CMusUiBackgroundViewContainer( 
    TInt aOrdinalPosition ) : iOrdinalPosition( aOrdinalPosition ),
            iController( NULL )
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiBackgroundViewContainer::ConstructL( const TRect& aRect )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiBackgroundViewContainer::ConstructL()" );
    
    RWindowGroup iMyWindowGroup = RWindowGroup( iCoeEnv->WsSession() );
    User::LeaveIfError( iMyWindowGroup.Construct( ( TUint32 )&iMyWindowGroup) );

    iMyWindowGroup.SetName( KMusUiBackgroundWindowGroupName );
    iMyWindowGroup.SetOrdinalPosition( iOrdinalPosition, ECoeWinPriorityNormal );
    iMyWindowGroup.EnableReceiptOfFocus( EFalse );
	iMyWindowGroup.AutoForeground( EFalse );
    iMyWindowGroup.SetNonFading( ETrue );

    CreateWindowL(&iMyWindowGroup);

    iGroupIdentifier = iMyWindowGroup.Identifier();

    iBackGround = CAknsBasicBackgroundControlContext::NewL(
        KAknsIIDQsnBgAreaMain,
        aRect,
        EFalse );

    SetRect( aRect );

    ActivateL();

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBackgroundViewContainer::ConstructL()" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiBackgroundViewContainer::~CMusUiBackgroundViewContainer()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiBackgroundViewContainer::~CMusUiBackgroundViewContainer()" );
    delete iBackGround;
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBackgroundViewContainer::~CMusUiBackgroundViewContainer()" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiBackgroundViewContainer::RefreshView()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiBackgroundViewContainer::RefreshView()" );

    DrawNow();

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBackgroundViewContainer::RefreshView()" );
    }


// -----------------------------------------------------------------------------
// Set's this group right ordinal position
// -----------------------------------------------------------------------------
//
void CMusUiBackgroundViewContainer::SetOrdinalPosition()
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiBackgroundViewContainer::SetOrdinalPosition(): %d",
                iOrdinalPosition );
    MakeVisible( ETrue );

    iCoeEnv->WsSession().SetWindowGroupOrdinalPosition( iGroupIdentifier,
                                                        iOrdinalPosition );

    DrawNow();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBackgroundViewContainer::SetOrdinalPosition()" );
    }

// -----------------------------------------------------------------------------
// Set's this group right ordinal position
// -----------------------------------------------------------------------------
//
void CMusUiBackgroundViewContainer::SetOrdinalPosition( TInt aOrdianlPosition)
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiBackgroundViewContainer::SetOrdinalPosition(): %d",
            aOrdianlPosition );
    MakeVisible( ETrue );

    iCoeEnv->WsSession().SetWindowGroupOrdinalPosition( iGroupIdentifier,
                                                        aOrdianlPosition );

    DrawNow();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBackgroundViewContainer::SetOrdinalPosition()" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiBackgroundViewContainer::UpdateOrdinalPositionValue( TBool aUp )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiBackgroundViewContainer::UpdateOrdinalPositionValue(): %d",
                aUp );
    if ( aUp )
        {
        ++iOrdinalPosition;
        }
    else
        {
        --iOrdinalPosition;
        }
    SetOrdinalPosition();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBackgroundViewContainer::UpdateOrdinalPositionValue()" );
    }


// -----------------------------------------------------------------------------
// Sets the event controller
// -----------------------------------------------------------------------------
//
void CMusUiBackgroundViewContainer::SetUiEventController( CMusUiEventController* aController )
	{
	iController = aController;
	}


// -----------------------------------------------------------------------------
// Called by framework when the view size is changed
// -----------------------------------------------------------------------------
//
void CMusUiBackgroundViewContainer::SizeChanged()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiBackgroundViewContainer::SizeChanged()" );
    TRect parentRect(Rect());
    if (iBackGround)
        {
        (static_cast<CAknsBasicBackgroundControlContext*>(iBackGround))->SetRect(parentRect);
        }

    SetOrdinalPosition();
    //MakeVisible( EFalse );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBackgroundViewContainer::SizeChanged()" );
    }


// -----------------------------------------------------------------------------
// From base class CCoeControl.  Handles a change to the application's
// resources which are shared across the environment.
// -----------------------------------------------------------------------------
//
void CMusUiBackgroundViewContainer::HandleResourceChange( TInt aType )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiBackgroundViewContainer::HandleResourceChange" );
    CCoeControl::HandleResourceChange( aType );
    
    TBool displayEnabled( EFalse );
    if ( iController )
        {
        TRAP_IGNORE( displayEnabled = iController->IsDisplayEnabledL() )
        }
        
    if ( aType == KEikDynamicLayoutVariantSwitch && !displayEnabled )
        {
        TRect mainPaneRect;
        AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane,
            mainPaneRect );
        SetRect( mainPaneRect );
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBackgroundViewContainer::HandleResourceChange" );
    }


// -----------------------------------------------------------------------------
// Called by framework when to retrieve the no. of controls.
// -----------------------------------------------------------------------------
//
TInt CMusUiBackgroundViewContainer::CountComponentControls() const
    {
    return 0; // return nbr of controls inside this container
    }


// -----------------------------------------------------------------------------
// Called by framework to retrieve the control specified by index.
// -----------------------------------------------------------------------------
//
CCoeControl* CMusUiBackgroundViewContainer::ComponentControl( TInt /*aIndex*/ ) const
    {
    return NULL;
    }


// -----------------------------------------------------------------------------
// Called by framework to redraw the screen area.
// -----------------------------------------------------------------------------
//
void CMusUiBackgroundViewContainer::Draw( const TRect& aRect ) const
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiBackgroundViewContainer::Draw()" );

    CWindowGc& gc = SystemGc();

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );
    if(!AknsDrawUtils::Background( skin, cc,this,gc,aRect,KAknsDrawParamDefault ))
        {
        MUS_LOG( "mus: [MUSUI ]     Skin not valid or not found" );
        gc.SetBrushColor( iEikonEnv->ControlColor( EColorWindowBackground, *this ) );
        gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
        gc.Clear( aRect );
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiBackgroundViewContainer::Draw()" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiBackgroundViewContainer::HandleControlEventL(
    CCoeControl* /*aControl*/,TCoeEvent /*aEventType*/)
    {
    // TODO: Add your control event handler code here
    }


// -----------------------------------------------------------------------------
// Called by framework to act on key events if required.
// -----------------------------------------------------------------------------
//
TKeyResponse CMusUiBackgroundViewContainer::OfferKeyEventL(
                        const TKeyEvent& /*aKeyEvent*/,
                        TEventCode /*aType*/ )
    {
    return EKeyWasNotConsumed;
    }


// -----------------------------------------------------------------------------
// Supply the Mob Object for background skin drawing
// -----------------------------------------------------------------------------
//
TTypeUid::Ptr CMusUiBackgroundViewContainer::MopSupplyObject( TTypeUid aId )
    {
    if (aId.iUid == MAknsControlContext::ETypeId && iBackGround )
        {
        return MAknsControlContext::SupplyMopObject( aId, iBackGround );
        }
    return CCoeControl::MopSupplyObject( aId );
    }



// end of file
