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
* Description:  MusIndicatorApi provides means to show notes and indicate
*                availability of videosharing to user.
*
*/


#include "musindicatordsa.h"
#include "muslogger.h"
#include "musresourceutil.h"
#include "musresourcefinderutil.h"
#include <e32cmn.h>
#include <e32std.h>
#include <musindicator.rsg>
#include <eikenv.h>
#include <coefontprovider.h>
#include <AknsDrawUtils.h>
#include <eikappui.h>
#include <musindicatoricons.mbg>
#include <AknUtils.h>

// constants
// Size and positions

const TInt KMusIndicatorWindowHeight = 50;
const TInt KMusIndicatorWindowShrink = 2;
const TInt KMusSpaceBetweenImageAndText = 5;

// appereance
const TInt KMusIndicatorBorderRoundedCorners = 6;
const TInt KMusIndicatorBorderSize = 1;
//const TInt KMusIndicatorFontPenSize = 1;




CMusIndicatorDsa* CMusIndicatorDsa::NewL()
    {
    MUS_LOG( "mus: [MUSIND] -> CMusIndicatorDsa::NewL" )
    CMusIndicatorDsa* self = new (ELeave) CMusIndicatorDsa();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSIND] <- CMusIndicatorDsa::NewL" )
    return self;
    }


CMusIndicatorDsa::CMusIndicatorDsa()
    : iActivateCallBack( CMusIndicatorDsa::TryActivate, this ),
      iRedrawCallBack( CMusIndicatorDsa::DoHandleResourceChange, this )
    {
    MUS_LOG( "mus: [MUSIND] -> CMusIndicatorDsa::CMusIndicatorDsa" )
        
    iBorderColor = KRgbBlack;
    iBorderBgColor = KRgbWhite;
    iFontColor = KRgbBlack;
    
    MUS_LOG( "mus: [MUSIND] <- CMusIndicatorDsa::CMusIndicatorDsa" )
    }


CMusIndicatorDsa::~CMusIndicatorDsa()
    {
    MUS_LOG( "mus: [MUSIND] -> CMusIndicatorDsa::~CMusIndicatorDsa" )
    
    StopPeriodicTimer();    
    delete iPeriodic;

    delete iIcon;
    delete iIconMask;
    
    delete iText;
    
    MUS_LOG( "mus: [MUSIND] <- CMusIndicatorDsa::~CMusIndicatorDsa" )
    }


const CFont& CMusIndicatorDsa::Font() const
    {
    TCoeFont coeFont = TCoeFont::NormalFont();
    TZoomFactor zoomFactor = AccumulatedZoom();
    return FindFontProvider().Font(coeFont, zoomFactor);
    }

//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
void CMusIndicatorDsa::ConstructL()
    {
    MUS_LOG( "mus: [MUSIND] -> CMusIndicatorDsa::ConstructL" )

    iPeriodic = CPeriodic::NewL( CActive::EPriorityStandard );
    
    LoadResourceStringL();
    LoadImageL();
    CreateWindowL();
    
    Initialize();
    TryActivateL();
    
    MUS_LOG( "mus: [MUSIND] <- CMusIndicatorDsa::ConstructL" )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusIndicatorDsa::TryActivateL()
    {
    MAknsControlContext* cc = 
        AknsDrawUtils::ControlContext( ControlEnv()->AppUi()->TopFocusedControl() );

    if ( cc )
        {
        StopPeriodicTimer();
        ActivateL();
        }
    else
        {
        StartPeriodicTimer( iActivateCallBack );
        }
    }


// -----------------------------------------------------------------------------
// Called by framework to redraw the screen area.
// -----------------------------------------------------------------------------
//
void CMusIndicatorDsa::Draw( const TRect& aRect ) const
    {
    MUS_LOG( "mus: [MUSIND] -> CMusIndicatorDsa::Draw" )
    MUS_LOG2( "mus: [MUSIND] : x=%d,y=%d", aRect.iTl.iX, aRect.iTl.iY )
    MUS_LOG2( "mus: [MUSIND] : x=%d,y=%d", aRect.iBr.iX, aRect.iBr.iY )
    
    CWindowGc& gc = SystemGc();
    
    TSize screenSizeNow = ControlEnv()->ScreenDevice()->SizeInPixels();
    
    if ( screenSizeNow != iScreenSize )
        {
        StartPeriodicTimer( iRedrawCallBack );
        }
    
    MAknsControlContext* cc = 
        AknsDrawUtils::ControlContext( ControlEnv()->AppUi()->TopFocusedControl() );
    MUS_LOG1( "mus: [MUSIND] : cc = %d", cc )

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    MUS_LOG1( "mus: [MUSIND] : skin = %d", skin )

    TRgb fontColor;
    AknsUtils::GetCachedColor( skin, fontColor, 
                               KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG6 );
    TRgb borderColor = fontColor;
    
    TBool backgroundWithBitmap = 
                ( skin && !cc ) ? 
                 EFalse :
                 AknsDrawUtils::Background( skin, cc,this,gc,aRect,KAknsDrawParamDefault );
    
    if( !backgroundWithBitmap )
        {
        MUS_LOG( "mus: [MUSIND] : Clearing rect with null brush") 
        gc.SetBrushStyle( CGraphicsContext::ENullBrush );
        gc.Clear( aRect );
        }
    else
        {
        MUS_LOG( "mus: [MUSIND] : rect was cleared with bitmap") 
        }
    
    gc.UseFont( &Font() );
    
    // Border initialization                   
    gc.SetBrushStyle( CGraphicsContext::ENullBrush );

    // Draw border
    MUS_LOG( "mus: [MUSIND] : Draw border") 
    gc.SetPenStyle( CGraphicsContext::ESolidPen );
    gc.SetPenColor( borderColor );
    gc.SetPenSize(
        TSize( KMusIndicatorBorderSize, KMusIndicatorBorderSize ) );
    gc.DrawRoundRect( iScreenRect,
                        TSize( KMusIndicatorBorderRoundedCorners,
                               KMusIndicatorBorderRoundedCorners ) );
    
    /* 
    // Draw smaller border with other color
    MUS_LOG( "mus: [MUSIND] : Draw smaller border with other color") 
    gc.SetPenColor( iBorderBgColor );
    TRect smallerRect = iScreenRect;
    smallerRect.Shrink( KMusIndicatorBorderSize, KMusIndicatorBorderSize );
    gc.DrawRoundRect( smallerRect,
                        TSize( KMusIndicatorBorderRoundedCorners,
                               KMusIndicatorBorderRoundedCorners ) );
                              
    // Font outline initialization
    
    MUS_LOG( "mus: [MUSIND] : Draw text outlines") 
    gc.SetPenColor( iBorderBgColor );
    gc.SetPenSize( TSize( KMusIndicatorFontPenSize, KMusIndicatorFontPenSize ) );

    // Draw text outlines
    TPoint outlineTextPos = iTextpos;
    outlineTextPos -= TPoint( 1, 1 );
    gc.DrawText( *iText, outlineTextPos );
    */
   
    // Font initialization
    gc.SetPenColor( fontColor );

    // Draw text
    MUS_LOG( "mus: [MUSIND] : Draw text") 
    gc.DrawText( *iText, iTextpos );
    
    gc.BitBltMasked( iImagepos, 
                       iIcon, 
                       TRect( 0, 0, iImageSize.iWidth, iImageSize.iHeight ),
                       iIconMask, 
                       EFalse );


    
    MUS_LOG( "mus: [MUSIND] <- CMusIndicatorDsa::Draw" )
    }


//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
//
void CMusIndicatorDsa::HandleResourceChange( TInt aType )
    {
    
    if ( aType == KEikDynamicLayoutVariantSwitch )
        {
        Initialize();
        DrawDeferred();
        }
    
    StopPeriodicTimer();
    
    }



//------------------------------------------------------------------------------
// Calculates topleft position of drawn text.
//------------------------------------------------------------------------------
//
void CMusIndicatorDsa::Initialize()
    {
    MUS_LOG( "mus: [MUSIND] -> CMusIndicatorDsa::Initialize" )
    
    TRect clientRect = CEikonEnv::Static()->EikAppUi()->ClientRect();
    MUS_LOG2( "mus: [MUSIND] clientrect : x=%d,y=%d", clientRect.iTl.iX, clientRect.iTl.iY )
    MUS_LOG2( "mus: [MUSIND] clientrect : x=%d,y=%d", clientRect.iBr.iX, clientRect.iBr.iY )
    
    iScreenSize = ControlEnv()->ScreenDevice()->SizeInPixels();

    //test (indicator goes inside options menu area)  
    //clientRect.iTl.iY = 150;
    
    SetPosition( clientRect.iTl );
    SetSize(
        TSize( iScreenSize.iWidth, KMusIndicatorWindowHeight ) );
    
    iScreenRect = TRect(
        TPoint( 0, 0 ),
        TSize( iScreenSize.iWidth, KMusIndicatorWindowHeight ) );
    iScreenRect.Shrink( KMusIndicatorWindowShrink, KMusIndicatorWindowShrink );
    
    
    TInt imagePlusTextWidth = Font().TextWidthInPixels( *iText ) + 
            iImageSize.iWidth + KMusSpaceBetweenImageAndText;

    iImagepos.iX = ( iScreenRect.Width() - imagePlusTextWidth )  >> 1;
    iImagepos.iY = ( iScreenRect.Height() - iImageSize.iHeight ) >> 1;
    
    // Text position
    iTextpos.iX = iImagepos.iX + iImageSize.iWidth + KMusSpaceBetweenImageAndText;

    iTextpos.iY = ( iScreenRect.Height() >> 1 ) +
                  ( Font().HeightInPixels() >> 1 ) +
                    iScreenRect.iTl.iY;
    
    MUS_LOG( "mus: [MUSIND] <- CMusIndicatorDsa::Initialize" )
    }



//------------------------------------------------------------------------------
// Loads displayed image.
//------------------------------------------------------------------------------
//
void CMusIndicatorDsa::LoadImageL()
    {
    MUS_LOG( "mus: [MUSIND] -> CMusIndicatorDsa::LoadImageL" )
    
    if ( iIcon || iIconMask )
        {
        MUS_LOG( "mus: [MUSIND] <- CMusIndicatorDsa::LoadImageL" )
        return;
        }
    
    _LIT( KMyBitmapRomFile, "Z:\\resource\\apps\\musindicatoricons.mbm" );
    _LIT( KMyBitmapRamFile, "C:\\resource\\apps\\musindicatoricons.mbm" );

    TFileName imageFileName;
    if ( MusResourceFinderUtil::IsExistL( KMyBitmapRomFile ) ) 
        {
        imageFileName = KMyBitmapRomFile;
        }
    else if ( MusResourceFinderUtil::IsExistL( KMyBitmapRamFile ) ) 
        {
        imageFileName = KMyBitmapRamFile;
        }
    else
        {
        MUS_LOG( "mus: [MUSIND]  Icon File Not Found!" );
        User::Leave( KErrNotFound );
        } 
        
    iIcon = new ( ELeave ) CFbsBitmap();
    iIconMask = new ( ELeave ) CFbsBitmap();
    
    // Enumerations of mbg file are usable only when vector images
    // in mif file are used. Those cannot be loaded without UI components.
    const TInt KMusIndicatorIconIndex = 0;
    const TInt KMusIndicatorIconMaskIndex = 1;
    User::LeaveIfError( 
        iIcon->Load( imageFileName, KMusIndicatorIconIndex ) );
    User::LeaveIfError( 
        iIconMask->Load( imageFileName, KMusIndicatorIconMaskIndex ) );
    
    iImageSize = iIcon->SizeInPixels();
    
    // Crop icon if it is too big
    if ( iImageSize.iHeight > KMusIndicatorWindowHeight )
        {
        TInt newHeight = KMusIndicatorWindowHeight >> 1;
        TInt newWidth = 
            TReal( iImageSize.iHeight / iImageSize.iWidth  ) * newHeight;
        iIcon->Resize( TSize( newHeight, newWidth ) );
        iIconMask->Resize( TSize( newHeight, newWidth ) );
        }
    
    
    MUS_LOG( "mus: [MUSIND] <- CMusIndicatorDsa::LoadImageL" )
    }


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
void CMusIndicatorDsa::LoadResourceStringL()
    {
    MUS_LOG( "mus: [MUSIND] -> CMusIndicatorDsa::LoadResourceStringL" )
    delete iText;
    iText = NULL;
    iText = MusResourceUtil::ReadResourceString16L(
        R_MUSINDICATOR_NOTE_VSREADY_TXT,
        KMusIndicatorResource );
    MUS_LOG( "mus: [MUSIND] <- CMusIndicatorDsa::LoadResourceStringL" )
    }

//------------------------------------------------------------------------------
// Periodic timer completion
//------------------------------------------------------------------------------
//
TInt CMusIndicatorDsa::TryActivate( TAny* aThis )
    {
    if ( !aThis )
        {
        return KErrArgument;
        }

    CMusIndicatorDsa* handle = static_cast< CMusIndicatorDsa* > ( aThis );
    
    TInt error = KErrNone;
    TRAP( error, handle->TryActivateL() );

    return error;
    }
    

//------------------------------------------------------------------------------
// Periodic timer completion
//------------------------------------------------------------------------------
//
TInt CMusIndicatorDsa::DoHandleResourceChange( TAny* aThis )
    {

    if ( !aThis )
        {
        return KErrArgument;
        }

    CMusIndicatorDsa* handle = static_cast< CMusIndicatorDsa* > ( aThis );

    handle->HandleResourceChange( KEikDynamicLayoutVariantSwitch );
    
    return KErrNone;
    
    }



//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
void CMusIndicatorDsa::StartPeriodicTimer( const TCallBack& aCallBack) const
    {
    MUS_LOG( "mus: [MUSIND] -> CMusIndicatorDsa::StartPeriodicTimer" )
    
    if ( iPeriodic->IsActive() )
        {
        return;
        }
    
    TTimeIntervalMicroSeconds32 delay = 
            aCallBack.iFunction == CMusIndicatorDsa::TryActivate ?
                                  KMusIndicatorDelay : KMusRedrawDelay;
    
    iPeriodic->Start( delay, delay, aCallBack );
    
    MUS_LOG( "mus: [MUSIND] <- CMusIndicatorDsa::StartPeriodicTimer" )
    }


//------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------
//
void CMusIndicatorDsa::StopPeriodicTimer()
    {
    MUS_LOG( "mus: [MUSIND] -> CMusIndicatorDsa::StopPeriodicTimer" )
    if( iPeriodic )
        {
        iPeriodic->Cancel();
        }
    MUS_LOG( "mus: [MUSIND] <- CMusIndicatorDsa::StopPeriodicTimer" )
    }

    



// End of file

