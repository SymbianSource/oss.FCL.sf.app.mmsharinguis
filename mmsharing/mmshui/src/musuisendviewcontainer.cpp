/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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


#include "musuiclipsharingviewcontainer.h"
#include "musuisendview.h"
#include "musuilevelindicator.h"
#include "musuidialogutil.h"
#include "musuidefinitions.h"
#include "musuisendcontroller.h"
#include "muslogger.h" // debug logging
#include <musui.rsg>

#include <AknWaitDialog.h>
#include <AknsBasicBackgroundControlContext.h>
#include <AknsUtils.h>
#include <AknsDrawUtils.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiSendViewContainer::CMusUiSendViewContainer()
    {
    // NOP
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendViewContainer::ConstructL( CMusUiGeneralView* aView,
                                          const TRect& aRect,
                                          TAknOrientation aIndicatorLayout )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendViewContainer::ConstructL" );
    
    CreateWindowL();
    iView = aView;
    iBackGround = CAknsBasicBackgroundControlContext::NewL(
                            KAknsIIDQsnBgAreaMain,
                            aRect,
                            EFalse );

    iIndicator = CMusUiLevelIndicator::NewL( aIndicatorLayout,
                                             0, 
                                             100, 
                                             this,
                                             aRect,
                                             this );
    iIndicator->SetContainerWindowL( *this );
    
    iVideoRect = aRect;
    
    SetRect( aRect );
    
    iIndicator->SetRect( Rect() );

    MakeVisible( EFalse );
    ActivateL();
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendViewContainer::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiSendViewContainer::~CMusUiSendViewContainer()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendViewContainer::~CMusUiSendViewContainer" );

    delete iIndicator;
    delete iBackGround;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendViewContainer::~CMusUiSendViewContainer" );
    }


// -----------------------------------------------------------------------------
// Fetches a pointer to the view that owns this container.
// -----------------------------------------------------------------------------
//
CMusUiGeneralView* CMusUiSendViewContainer::MusUiView() const
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendViewContainer::MusUiView" );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendViewContainer::MusUiView" );
    return iView;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendViewContainer::SetController( CMusUiSendController* aController )
    {
    iSendController = aController;
    }


// -----------------------------------------------------------------------------
// Called by framework to redraw the screen area.
// -----------------------------------------------------------------------------
//
void CMusUiSendViewContainer::Draw( const TRect& aRect ) const
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiSendViewContainer::Draw" );
    CWindowGc& gc = SystemGc();
  
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );
    if(!AknsDrawUtils::Background( skin, 
                                   cc,
                                   this,
                                   gc,
                                   iVideoRect,
                                   KAknsDrawParamDefault ))
        {
        MUS_LOG( "mus: [MUSUI ]     Skin not valid or not found" );
        gc.SetBrushColor( iEikonEnv->ControlColor( EColorWindowBackground, *this ) );
        gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
        gc.Clear( aRect );
        }
       
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendViewContainer::Draw" );
    }
    

// -----------------------------------------------------------------------------
// Called by framework when the view size is changed
// -----------------------------------------------------------------------------
//
void CMusUiSendViewContainer::SizeChanged()
    {
    if (iBackGround)
        {
        static_cast<CAknsBasicBackgroundControlContext*>
                                ( iBackGround )->SetRect( Rect() );
        }
    iIndicator->SetRect( Rect() );    
    }
    

// -----------------------------------------------------------------------------
// Supply the Mob Object for background skin drawing
// -----------------------------------------------------------------------------
//
TTypeUid::Ptr CMusUiSendViewContainer::MopSupplyObject( TTypeUid aId )
    {
    if (aId.iUid == MAknsControlContext::ETypeId && iBackGround )
        {
        return MAknsControlContext::SupplyMopObject( aId, iBackGround );
        }
    return CCoeControl::MopSupplyObject( aId );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendViewContainer::SetLevelIndicatorVisibility( TBool aVisible)
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiSendViewContainer::SetLevelIndicatorVisibility( %d )",
              aVisible )
              
    MakeVisible( aVisible );
    
    if ( !aVisible )
        {
        iView->LevelIndicatorDismissed();
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiSendViewContainer::SetLevelIndicatorVisibility" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiSendViewContainer::IndicatorLevelChanged( TInt aNewLevel )
    {
    if ( iSendController )
        {
        TRAPD( error, iSendController->HandleSliderValueChangeL( aNewLevel ) )

        if ( error != KErrNone )
            {
            MUS_LOG1( "mus: [MUSUI ]     Error during handling of slider value change( %d )",
                      error )
              
            iSendController->HandleError( error );
            }
        }
    }
    


// end of file
