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


#include "musuidefinitions.h"
#include "musuilivesharingviewcontainer.h"
#include "musuieventcontroller.h"
#include "musuigeneralview.h"
#include "musuilivesharingview.h"
#include "musuiappui.h"
#include "musuilevelindicator.h"
#include "musuidialogutil.h"
#include "musui.hrh"
#include "muslogger.h" // debug logging
#include "mussettings.h"
#include <musui.rsg>

#include <AknsDrawUtils.h>
#include <AknDef.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingViewContainer::ConstructL( CMusUiGeneralView* aView,
                                                 const TRect& aRect )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingViewContainer::ConstructL" );
    
    CMusUiSendViewContainer::ConstructL( aView, 
                                         aRect,
                                         EAknOrientationVertical );
    // check if operator specific functionality is needed  
    iOperatorSpecificFunctionality = 
        ( MultimediaSharingSettings::OperatorVariantSettingL() == 
                                        MusSettingsKeys::EOperatorSpecific );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingViewContainer::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiLiveSharingViewContainer::~CMusUiLiveSharingViewContainer()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingViewContainer::~CMusUiLiveSharingViewContainer" );
    // NOP
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingViewContainer::~CMusUiLiveSharingViewContainer" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingViewContainer::SetZoomValues( TInt aMinZoom, TInt aMaxZoom )
    {
    iIndicator->SetRange( aMinZoom, aMaxZoom, ETrue );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingViewContainer::SetBrightnessValues( TInt aMinBrightness, 
                                                          TInt aMaxBrightness )
    {
    iIndicator->SetRange( aMinBrightness, aMaxBrightness, ETrue );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingViewContainer::SetZoomL( TInt aZoomLevel )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingViewContainer::SetZoomL" )

    iIndicator->SetLevelL( EMusUiIndicatorTypeZoom, 
                           aZoomLevel, 
                           ETrue /* use timer */ );
                          
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingViewContainer::SetZoomL" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingViewContainer::SetBrightnessL( TInt aBrightnessLevel )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiLiveSharingViewContainer::SetBrightnessL" )
    
    iIndicator->SetLevelL( EMusUiIndicatorTypeBrightness, 
                           aBrightnessLevel, 
                           ETrue /* use timer */ );
                          
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingViewContainer::SetBrightnessL" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingViewContainer::SetZoomVisible(  TBool aVisible )
    {
    iIndicator->SetIndicatorType( EMusUiIndicatorTypeZoom );
    MakeVisible( aVisible );
    SetGloballyCapturing( aVisible );
    SetPointerCapture( aVisible );  
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingViewContainer::SetBrightnessVisible( TBool aVisible)
    {
    iIndicator->SetIndicatorType(EMusUiIndicatorTypeBrightness);
    MakeVisible( aVisible );
    SetGloballyCapturing( aVisible );
    SetPointerCapture( aVisible );  
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TMusUiIndicatorType CMusUiLiveSharingViewContainer::GetIndicatorType()
	{
	return iIndicator->GetIndicatorType();
	}


// -----------------------------------------------------------------------------
// Called by framework when to retrieve the no. of controls.
// -----------------------------------------------------------------------------
//
TInt CMusUiLiveSharingViewContainer::CountComponentControls() const
    {
    return 1; // return nbr of controls inside this container
    }


// -----------------------------------------------------------------------------
// Called by framework to retrieve the control specified by index.
// -----------------------------------------------------------------------------
//
CCoeControl* CMusUiLiveSharingViewContainer::ComponentControl( TInt aIndex ) const
    {
    CCoeControl* control = NULL;
    switch ( aIndex )
        {
        case 0:
            control = iIndicator;
            break;
        default:
            break;
        }
    return control;
    }


// -----------------------------------------------------------------------------
// Called by framework to act on key events if required.
// -----------------------------------------------------------------------------
//
TKeyResponse CMusUiLiveSharingViewContainer::OfferKeyEventL(
    const TKeyEvent& aKeyEvent,
    TEventCode /*aType*/ )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiLiveSharingViewContainer::OfferKeyEventL: %d",
              aKeyEvent.iCode );
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiLiveSharingViewContainer::OfferKeyEventL: %d",
              aKeyEvent.iScanCode );
            
    TKeyResponse ret( EKeyWasNotConsumed );

    if ( MUS_NO_TOOLBAR )
        {
        CMusUiLiveSharingView* view = 
            static_cast< CMusUiLiveSharingView* >( MusUiView() );      
        switch ( aKeyEvent.iCode )
            {
            case EKeyLeftArrow:
                {
                view->HandleCommandL( EMusuiCmdViewVolumeDown );
                ret = EKeyWasConsumed;
                break;
                }
            case EKeyRightArrow:
                {
                view->HandleCommandL( EMusuiCmdViewVolumeUp );
                ret = EKeyWasConsumed;
                break;
                }
            case EKeyUpArrow:
                {
                view->HandleKeyUpArrowL();
                ret = EKeyWasConsumed;
                break;
                }
            case EKeyDownArrow:
                {
                view->HandleKeyDownArrowL();
                ret = EKeyWasConsumed;
                break;
                }
            default:
                break;
            }
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLiveSharingViewContainer::OfferKeyEventL" );
    return ret;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLiveSharingViewContainer::HandlePointerEventL( 
                                            const TPointerEvent& aPointerEvent)
    {
        // Forward to base class in order to use slider
        CCoeControl::HandlePointerEventL( aPointerEvent );
	    
	if ( !Rect().Contains( aPointerEvent.iPosition ) )
        {
        MakeVisible( EFalse );
        }
    }


// end of file
