/*
* Copyright (c) 2003, 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class for displaying level indicator icons
*
*/


// USER
#include "musuilevelindicator.h"
#include "musuilevelindicatorobserver.h"
#include "muslogger.h"
#include "musresourcefinderutil.h"
#include "musuiactivetimer.h"
#include <musuiicons.mbg>
#include <musui.rsg>

// SYSTEM
#include <aknslider.h>
#include <barsread.h>
#include <AknUtils.h>
#include <AknsUtils.h>
#include <data_caging_path_literals.hrh>
#include <gulicon.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <AknsDrawUtils.h>


// CONSTANTS
const TInt KMusUiLevelDisableTime = 4000000;
const TInt KMusUiSliderEventDelay = 300000;


_LIT( KMyBitmapRomFile, "Z:\\resource\\apps\\musuiicons.mif" );
//_LIT( KMyBitmapRamFile, "C:\\resource\\apps\\musuiicons.mif" );


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiLevelIndicator::CMusUiLevelIndicator(
        						TInt aMinLevel, 
        						TInt aMaxLevel, 
        						MMusUiLevelIndicatorObserver* aObserver )
    : iMinLevel( aMinLevel ),
      iMaxLevel( aMaxLevel ),
      iObserver( aObserver )
    {
    MUS_LOG( "mus: mus: [MUSUI ] -> CMusUiLevelIndicator::CMusUiLevelIndicator()" );
    // pass    
    MUS_LOG( "mus: [MUSUI ] <- CMusUiLevelIndicator::CMusUiLevelIndicator()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiLevelIndicator* CMusUiLevelIndicator::NewL(
                        TAknOrientation aLayout, 
                        TInt aMinLevel,
                        TInt aMaxLevel,
                        CCoeControl* aParent,
                        const TRect& aRect,
                        MMusUiLevelIndicatorObserver* aObserver )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiLevelIndicator::NewL()" );
    
    CMusUiLevelIndicator* self = NewLC( aLayout,
                                        aMinLevel,
                                        aMaxLevel,
                                        aParent,
                                        aRect,
                                        aObserver );
    CleanupStack::Pop( self );

    MUS_LOG( "mus: [MUSUI ] <- CMusUiLevelIndicator::NewL()" );
    
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiLevelIndicator* CMusUiLevelIndicator::NewLC(
                        TAknOrientation aLayout, 
                        TInt aMinLevel,
                        TInt aMaxLevel,
                        CCoeControl* aParent,
                        const TRect& aRect,
                        MMusUiLevelIndicatorObserver* aObserver )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiLevelIndicator::NewLC()" );
    
    CMusUiLevelIndicator* self = new ( ELeave ) CMusUiLevelIndicator(
                            aMinLevel,
                            aMaxLevel,
                            aObserver );
    CleanupStack::PushL( self );
    self->ConstructL( aLayout, aParent, aRect );

    MUS_LOG( "mus: [MUSUI ] <- CMusUiLevelIndicator::NewLC()" );
    
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiLevelIndicator::~CMusUiLevelIndicator()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiLevelIndicator::~CMusUiLevelIndicator()" );

    delete iSlider;
    delete iSliderEventTimer;
    delete iDisableLevelTimer;
  
    MUS_LOG( "mus: [MUSUI ] <- CMusUiLevelIndicator::~CMusUiLevelIndicator()" );
    }
        

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLevelIndicator::ConstructL( TAknOrientation aLayout,
                                       CCoeControl* aParent,
                                       const TRect& aRect )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiLevelIndicator::ConstructL()" );
    
    SetRect(aRect);
        
    if( iMinLevel > iMaxLevel )
        {
        User::Leave( KErrArgument );
        }
        
    iDisableLevelTimer = CMusUiActiveTimer::NewL( this ); 
    iSliderEventTimer = CMusUiActiveTimer::NewL( this );

    iSlider = new CAknSlider();     
    
    TResourceReader reader;

    if( aLayout == EAknOrientationHorizontal )   
        {
        iEikonEnv->CreateResourceReaderLC( reader, R_MUS_SLIDER_HORIZONTAL );                                        
        MUS_LOG( "mus: [MUSUI ] -> ConstructFromResourceL" );    
        iSlider->ConstructFromResourceL( aParent, 0, reader);            
        CleanupStack::PopAndDestroy(); // reader 
        iSlider->SetRange( iMinLevel, iMaxLevel );          
        iSlider->SetRect( aRect );        
        }
    else
        {
        iEikonEnv->CreateResourceReaderLC( reader, R_MUS_SLIDER_VERTICAL );                    
        iSlider->ConstructFromResourceL( aParent, 0 ,reader);            
        CleanupStack::PopAndDestroy(); // reader          
        TRect rect = TRect( TPoint( (aRect.iBr.iX - iSlider->Size().iWidth),(aRect.iTl.iY) ),TSize( iSlider->Size() ) );             
        iSlider->SetRect(rect);                   
           
        /* Draw the Min and Max Zoom */
        TParse parse;
        parse.Set( KMyBitmapRomFile, &KDC_APP_RESOURCE_DIR, NULL );
        CFbsBitmap* bitmap = NULL;
        CFbsBitmap* mask = NULL;
        AknsUtils::CreateIconL(AknsUtils::SkinInstance(),
                           KAknsIIDQsnBgScreen,
                           bitmap,
                           mask,
                           parse.FullName(),
                           EMbmMusuiiconsQgn_indi_browser_tb_zoom_in,                           
                           EMbmMusuiiconsQgn_indi_browser_tb_zoom_in_mask);   
        iSlider->SetGraphics(CAknSlider::EElemEmptyLeftCap,bitmap,mask);
        AknsUtils::CreateIconL(AknsUtils::SkinInstance(),
                           KAknsIIDQsnBgScreen,                           
                           bitmap,
                           mask,
                           parse.FullName(),
                           EMbmMusuiiconsQgn_indi_browser_tb_zoom_out,
                           EMbmMusuiiconsQgn_indi_browser_tb_zoom_out_mask);           
        iSlider->SetGraphics(CAknSlider::EElemEmptyRightCap,bitmap,mask); 
        }
            
    iSlider->HandleResourceChange( 
                            KEikMessageCaptionedControlEditableStateChange );  
    iSlider->SetObserver( this );     
    iSlider->SetPositionIndicators( CAknSlider::EPosMarker );    
    iSlider->SetTicksEnabled( EFalse );    
    iSlider->EnableDrag();       
    iSlider->SetStepSize( 1 );
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiLevelIndicator::ConstructL()" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLevelIndicator::SetLevelL( TMusUiIndicatorType aType, 
                                      TInt aLevel, 
                                      TBool aUseTimer )
    {
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiLevelIndicator::SetLevelL( %d ) ", 
                            aLevel );
    if ( aLevel >= iMinLevel && aLevel <= iMaxLevel )
        {
            switch(aType)
                {
                case EMusUiIndicatorTypeZoom:
                    {
                    iZoomLevel=aLevel;                    
                    break;
                    }
                case EMusUiIndicatorTypeBrightness:
                    {
                    iBrightnessLevel=aLevel;
                    break;
                    }
                case EMusUiIndicatorTypeDuration:
                    {
                    iDurationLevel=aLevel;
                    break;
                    }
                 }
        }

    // Set it to the slider
    if ( !iSliderEventTimer->IsActive() )
        {
        iSlider->SetValueL( aLevel );
        }
    
    DrawNow();
        
    if ( aUseTimer )
        {
        iDisableLevelTimer->After( KMusUiLevelDisableTime );
        }
        
    MUS_LOG( "mus: [MUSUI ] <- CMusUiLevelIndicator::SetLevelL()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusUiLevelIndicator::GetLevel( TMusUiIndicatorType aType )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiLevelIndicator::GetLevel()" );
    
    TInt ret=KErrNotFound;
    
    switch(aType)
        {
        case EMusUiIndicatorTypeZoom:
            {
            ret=iZoomLevel;
            break;
            }
        case EMusUiIndicatorTypeBrightness:
            {
            ret=iBrightnessLevel;
            break;
            }

        case EMusUiIndicatorTypeDuration:
            {
            ret = iSlider->Value();
            break;
            }
         }
 
    MUS_LOG( "mus: [MUSUI ] <- CMusUiLevelIndicator::GetLevel()" );
    return ret;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLevelIndicator::SetRange( TInt aMinLevel, 
                                     TInt aMaxLevel, 
                                     TBool aUseTimer)
    {
    MUS_LOG2( "mus: [MUSUI ] -> CMusUiLevelIndicator::SetRange( %d - %d ) ",
                            aMinLevel, aMaxLevel );
                            
    if( aMinLevel < aMaxLevel )
        {
        iMinLevel = aMinLevel;
        iMaxLevel = aMaxLevel;
        iSlider->SetRange( iMinLevel, iMaxLevel );
        }
        
    if ( aUseTimer )
       {
       iDisableLevelTimer->After( KMusUiLevelDisableTime );
       }
   
    MUS_LOG( "mus: [MUSUI ] <- CMusUiLevelIndicator::SetRange()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLevelIndicator::SetIndicatorType( TMusUiIndicatorType aType )
    {
    iIndicatorType = aType;
    }
    
  
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TMusUiIndicatorType CMusUiLevelIndicator::GetIndicatorType()
	{
	return iIndicatorType;
	}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiLevelIndicator::SizeChanged()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiLevelIndicator::SizeChanged()" );
    
    if ( iSlider )
        { 
        iSlider->SetRect( Rect() );
        }
        
    DrawNow();
    
    MUS_LOG( "mus: [MUSUI ] <- CMusUiLevelIndicator::SizeChanged()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusUiLevelIndicator::Draw( const TRect& aRect ) const
    {
    CWindowGc& gc = SystemGc();    
    gc.SetBrushColor( iEikonEnv->ControlColor( EColorWindowBackground, *this ) );
    gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
    gc.Clear( aRect );    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusUiLevelIndicator::CountComponentControls() const
    {
    return 1;   // return nbr of controls inside this container
    }


// -----------------------------------------------------------------------------
// Called by framework to retrieve the control specified by index.
// -----------------------------------------------------------------------------
//
CCoeControl* CMusUiLevelIndicator::ComponentControl( TInt aIndex ) const
    {
    switch ( aIndex )
        {        
        case 0:
            {
            return iSlider;
            }
        default:
            {
            return NULL;
            }
        }
    }
 

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusUiLevelIndicator::HandleResourceChange( TInt aType )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiLevelIndicator::HandleResourceChange()" );
    
    if( aType==KEikDynamicLayoutVariantSwitch )
        {
        if ( iSlider ) // Force a re-layout
            {
            iSlider->SetRect( Rect() );        
            }            
        }
        
    DrawNow();
    
    MUS_LOG( "mus: [MUSUI ] <- CMusUiLevelIndicator::HandleResourceChange()" );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusUiLevelIndicator::HandleControlEventL( CCoeControl* aControl,
                                        TCoeEvent aEventType)
    {
    // No logging due to huge amount of events
    
    if ( aControl == iSlider &&
         aEventType == MCoeControlObserver::EEventStateChanged )
        {
        // Restart the disable timer if running
        if ( iDisableLevelTimer->IsActive() )
            {
            iDisableLevelTimer->After( KMusUiLevelDisableTime );
            }
            
        if ( iObserver )
            {
            iSliderEventTimer->After( KMusUiSliderEventDelay );
            }
        }
    
    // No logging due to huge amount of events
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusUiLevelIndicator::TimerComplete( CMusUiActiveTimer* aTimer )
    {    
    if ( aTimer == iSliderEventTimer )
        {
        MUS_LOG( "mus: [MUSUI ]    Slider event timer complete" );

        if ( iObserver )
            {
            iObserver->IndicatorLevelChanged( iSlider->Value() );
            }
        }
    else if ( aTimer == iDisableLevelTimer )
        {
        MUS_LOG( "mus: [MUSUI ]    Slider visibility timer complete" );

        if ( iObserver )
            {
            iObserver->SetLevelIndicatorVisibility( EFalse );
            }
        }
    else
        {
        // NOP
        }
    }
    


// End of File



