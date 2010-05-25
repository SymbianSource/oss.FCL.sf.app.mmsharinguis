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
* Description: 
*
*/



#include "musuinavimediadecorator.h"
#include "musuinavimediaanimator.h"
#include "musuidefinitions.h"
#include "muslogger.h" // debug logging
#include <musuiicons.mbg>
//#include <musui.mbg>
//#include <musui_temp_icons.mbg>
#include <musui.rsg>
#include "musresourcefinderutil.h"

#include <data_caging_path_literals.hrh>    // KDC_APP_BITMAP_DIR
#include <AknsUtils.h>
#include <aknnavi.h>           // CAknNavigationControlContainer
#include <eikspane.h>          // CEikStatusPaneBase
#include <StringLoader.h>
#include <applayout.cdl.h>     // scalable UI
#include <AknIconUtils.h>      // SetSize for the icons
#include <AknsDrawUtils.h>
#include <AknIconArray.h>
#include <gulicon.h>
#include <applayout.cdl.h>
#include <AknUtils.h>

using namespace AppLayout;


const TInt KMusDecoratorStability = 0;
const TInt KIconArraySize = 15;

_LIT( KMyBitmapRomFile, "Z:\\resource\\apps\\musuiicons.mif" );
_LIT( KMyBitmapRamFile, "C:\\resource\\apps\\musuiicons.mif" );


#ifndef __MIN
#define __MIN(A, B) ((A) < (B) ? (A): (B)) 
#endif
//macro 
//_LIT( KMyMbmFileName, "z:Camcorder.mbm");


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiNaviMediaDecorator* CMusUiNaviMediaDecorator::NewL()
    {
    CMusUiNaviMediaDecorator* self = new( ELeave ) CMusUiNaviMediaDecorator();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiNaviMediaDecorator::~CMusUiNaviMediaDecorator()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiNaviMediaDecorator::~CMusUiNaviMediaDecorator" );
    delete iLeftLabel;
    delete iRightLabel;
    delete iIconArray;
    delete iAnimation;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiNaviMediaDecorator::~CMusUiNaviMediaDecorator" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiNaviMediaDecorator::CMusUiNaviMediaDecorator()
    {
    iMiddle = 0;
    iLeftTextSize = TSize( 0, 0 );
    iRightTextSize = TSize( 0, 0 );
    iIcon = NULL; 
    iIconLeft = 50; // Only defaylt valut, it's overrided right away
    iImageSize = TSize( 18, 18 ); // Only default value, it's overrided right avay
    }


// -----------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------
//
void CMusUiNaviMediaDecorator::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiNaviMediaDecorator::ConstructL" );
    iLeftLabel = HBufC16::NewL(0);
    iRightLabel = HBufC16::NewL(0);
    

    iIconLayout.LayoutRect( Rect(), R_MUS_NAVI_MEDIA_ICON_LAYOUT_SIZE );
    iImageSize.iWidth = iIconLayout.Rect().Width();
    iImageSize.iHeight = iIconLayout.Rect().Height();
    
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiNaviMediaDecorator::ConstructL(): iImageSize.iWidth %d",
                            iImageSize.iWidth )
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiNaviMediaDecorator::ConstructL(): iImageSize.iHeight %d",
                            iImageSize.iHeight )
    iLeftTextLayout.LayoutText( Rect(),
                                R_MUS_NAVI_MEDIA_TEXT_LEFT_LAYOUT );
    iRightTextLayout.LayoutText( Rect(),
                                 R_MUS_NAVI_MEDIA_TEXT_RIGHT_LAYOUT );
                                 

    if( MusResourceFinderUtil::IsExistL( KMyBitmapRomFile ) ) 
        {
        iResourceRam = EFalse;
        }
    else if( MusResourceFinderUtil::IsExistL( KMyBitmapRamFile ) ) 
        {
        iResourceRam = ETrue;
        }
    else
        {
        MUS_LOG( "mus: [MUSUI ]  Icon File Not Found " );
        User::Leave(KErrNotFound);
        }    

    LoadImagesL();

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiNaviMediaDecorator::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------
//
void CMusUiNaviMediaDecorator::SetLeftLabelL( const TDesC& aLabel )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiNaviMediaDecorator::SetLabelL" );
    HBufC* newLabel = aLabel.AllocL();
    delete iLeftLabel; // after the AllocL succeeds
    iLeftLabel = NULL;
    iLeftLabel = newLabel;
    TPtr ptr( iLeftLabel->Des() );
    AknTextUtils::LanguageSpecificNumberConversion( ptr );
    DoLayoutText();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiNaviMediaDecorator::SetLabelL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiNaviMediaDecorator::SetRightLabelL( const TDesC& aLabel )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiNaviMediaDecorator::SetLabelL" );
    HBufC* newLabel = aLabel.AllocL();
    delete iRightLabel; // after the AllocL succeeds
    iRightLabel = NULL;
    iRightLabel = newLabel;
    TPtr ptr( iRightLabel->Des() );
    AknTextUtils::LanguageSpecificNumberConversion( ptr );
    DoLayoutText();
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiNaviMediaDecorator::SetLabelL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiNaviMediaDecorator::ShowIcon( TMusUiNaviMediaDecorator aIcon )
    {
    MUS_LOG1( "mus: [MUSUI ]     -> CMusUiNaviMediaDecorator::ShowIcon( \
              TMusMediaNaviIcons aIcon ) #%d", aIcon );
              
    delete iAnimation;
    iAnimation = NULL;
                  
    if( aIcon == EMusUiNaviIconNone )
        {
        iIcon = NULL;
        return;
        }
    else if( aIcon == EMusUiNaviIconWaitAnim ) 
        {
        iIcon = NULL;
        iIconLayout.LayoutRect( Rect(), R_MUS_NAVI_MEDIA_ICON_LAYOUT_SIZE );
        iImageSize.iWidth = iIconLayout.Rect().Width();
        iImageSize.iHeight = iIconLayout.Rect().Height();
        TRAPD( err, iAnimation = CMusUiNaviMediaAnimator::NewL( *this, iImageSize ); )
        if( err == KErrNone )
            {
            //iAnimation->SetInterval( 50000 );
            }
        }
    else if( iIconArray->Count() > aIcon )
    		{
    		iIcon = static_cast< CGulIcon* > ( iIconArray->At( aIcon) );
    		}
    else
        {
        iIcon = NULL;        
        }
    if( !iAnimation ) 
        {
        DrawNow();
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiNaviMediaDecorator::ShowIcon( \
             TMusMediaNaviIcons aIcon )" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiNaviMediaDecorator::SetIcon( CGulIcon* aIcon )
    {
    iIcon = aIcon; 
    DrawNow();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiNaviMediaDecorator::SizeChanged()
    {
    AknsUtils::RegisterControlPosition( this );
    DoLayoutText();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiNaviMediaDecorator::Draw( const TRect& aRect ) const
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiNaviMediaDecorator::Draw" );

    CWindowGc& gc = SystemGc();
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    // Get skinned bitmap
    CFbsBitmap* skinBitmap =
                    AknsUtils::GetCachedBitmap( skin, KAknsIIDQsnIconColors );
    if( iIcon )
        {
        MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );
        AknsDrawUtils::Background( skin, cc,this, gc , aRect , KAknsDrawParamDefault );        
        gc.BitBltMasked( TPoint( iIconLeft,1),
                         iIcon->Bitmap(),
                         iIconLayout.Rect(),
                         iIcon->Mask(),
                         EFalse );
        }
    // draw time indicator

    //TRgb color = iLeftTextLayout.Color();

	TRgb color;

	AknsUtils::GetCachedColor( skin, color, KAknsIIDQsnTextColors, EAknsCIQsnTextColorsCG2 );

    gc.SetPenColor( color );
    gc.UseFont( iLeftTextLayout.Font() );
    

    TInt flTop = ( ( aRect.Height() >> 1 ) 
                 - ( iLeftTextLayout.Font()->AscentInPixels() >> 1 ) ) 
                 + aRect.iTl.iY - 1; // -1 because some rounding things

    gc.DrawText( *iLeftLabel,
                 TRect( aRect.iTl.iX+1, flTop, aRect.iBr.iX, aRect.iBr.iY  ),
                 iLeftTextLayout.Font()->AscentInPixels() + 1,
                 iLeftTextLayout.Align() );

    TInt frTop = ( ( aRect.Height() >> 1 ) 
                 - ( iRightTextLayout.Font()->AscentInPixels() >> 1 ) ) 
                 + aRect.iTl.iY - 1; // -1 because some rounding things
      
    TInt frLeft;  
    TInt frRight;
                 
    if ( AknLayoutUtils::LayoutMirrored() )
        {
        frLeft  = aRect.iTl.iX;
        frRight = iRightTextSize.iWidth + 1;
        }
    else
        {
        frLeft  = aRect.iBr.iX - iRightTextSize.iWidth - 1;
        frRight = aRect.iBr.iX;
        }
                 
    gc.DrawText( *iRightLabel,
                 TRect( frLeft, frTop, frRight, aRect.iBr.iY  ),
                 iRightTextLayout.Font()->AscentInPixels() + 1,
                 iRightTextLayout.Align() );


    MUS_LOG( "mus: [MUSUI ]  <- CMusUiNaviMediaDecorator::Draw " );
    }


// -----------------------------------------------------------------------------
// Layouts the text depending on current icon situation.
// -----------------------------------------------------------------------------
//
void CMusUiNaviMediaDecorator::DoLayoutText()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiNaviMediaDecorator::DoLayoutText" );
    TRect parent( Rect() );
    if ( parent.IsEmpty() )
        {
        MUS_LOG( "mus: [MUSUI ] <- CMusUiNaviMediaDecorator::DoLayoutText: return" );
        return;
        }

    TInt minsize = __MIN( parent.Width(), parent.Height() );
    iImageSize = TSize( minsize-2, minsize-2 );
     

    // Reassign text sizes
    TAknTextLineLayout txtLayout = AppLayout::Navipanetexts_Line_2();
    const TInt spaceBetweenIconsAndText = 1;
    txtLayout.il = spaceBetweenIconsAndText;

    iLeftTextLayout.LayoutText( parent, txtLayout );
    iRightTextLayout.LayoutText( parent, txtLayout );

    TSize parentSize = TSize( parent.iBr.iX - parent.iTl.iX,
                              parent.iBr.iY - parent.iTl.iY );

    //TInt clientWidth = parent.iBr.iX - parent.iTl.iX;

    iLeftTextSize.iWidth =
            iLeftTextLayout.Font()->TextWidthInPixels( iLeftLabel->Des() );
    iRightTextSize.iWidth =
            iRightTextLayout.Font()->TextWidthInPixels( iRightLabel->Des() );

    // First testing elements total width
    if( iLeftTextSize.iWidth + iRightTextSize.iWidth + iIconLayout.Rect().iBr.iX
        > parentSize.iWidth )
        {
        // Whole stuff is too large
        _LIT( KMusStringThreeDots, "...");
            TRAP_IGNORE( SetRightLabelL( KMusStringThreeDots ) );
        }

    // Middle point of Icon box in current text parameters
    TInt middle = parent.iTl.iX + ( parentSize.iWidth >> 1 );

    // If there collision, calculates middle of the text fields
    iIconLeft = middle - parent.iTl.iX - ( iIconLayout.Rect().iBr.iX >> 1 );
    TInt iconRight =
            middle - parent.iTl.iX + ( iIconLayout.Rect().iBr.iX >> 1 );

    if( iLeftTextSize.iWidth > iIconLeft ||
        ( parentSize.iWidth - iRightTextSize.iWidth ) < iconRight )
        {
        middle = (( parentSize.iWidth
                 - ( iLeftTextSize.iWidth + iRightTextSize.iWidth )) >> 1 )
                 + parent.iTl.iX + iLeftTextSize.iWidth;
        }


    // Move only if middle place moves enough eq. ignoring jumping
    if( middle < ( iMiddle - KMusDecoratorStability )
        || middle > ( iMiddle + KMusDecoratorStability ) )
        {
        iMiddle = middle;
        }

    iLeftTextSize.iHeight = iLeftTextLayout.Font()->HeightInPixels();
    iRightTextSize.iHeight = iRightTextLayout.Font()->HeightInPixels();

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiNaviMediaDecorator::DoLayoutText" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void  CMusUiNaviMediaDecorator::LoadImagesL() 
    {
    
    delete iIconArray;
    iIconArray = NULL;
    iIconArray = new ( ELeave ) CAknIconArray( KIconArraySize );

    TParse parse;
    if( iResourceRam ) 
        {
        parse.Set( KMyBitmapRamFile, &KDC_APP_RESOURCE_DIR, NULL );
        }
    else 
        {
        parse.Set( KMyBitmapRomFile, &KDC_APP_RESOURCE_DIR, NULL );
        }


    MAknsSkinInstance* skin = AknsUtils::SkinInstance();

    TAknsItemID item;
    item.Set( 0xFFCC, 0xC ); // It's a kind a magic
    

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_forw,
                     EMbmMusuiiconsQgn_indi_video_forw_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_pause,
                     EMbmMusuiiconsQgn_indi_video_pause_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_play,
                     EMbmMusuiiconsQgn_indi_video_play_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_record,
                     EMbmMusuiiconsQgn_indi_video_record_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_record_not,
                     EMbmMusuiiconsQgn_indi_video_record_not_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_rew,
                     EMbmMusuiiconsQgn_indi_video_rew_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_stop,
                     EMbmMusuiiconsQgn_indi_video_stop_mask,
                     parse );

    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void  CMusUiNaviMediaDecorator::ConstructImageL( MAknsSkinInstance* aSkin,
                                                const TAknsItemID& aId,
                                                TInt aBitmapId,
                                                TInt aMaskId,
                                                TParse parse )

    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiNaviMediaDecorator::ConstructImageL()" );

    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;
   
    AknsUtils::CreateColorIconL(aSkin,
        		                aId,
        		                KAknsIIDQsnIconColors,
        		                EAknsCIQsnIconColorsCG13,
        		                bitmap,
    							mask,
    							parse.FullName(),
    							aBitmapId,
    							aMaskId,
    							KRgbBlack);

    AknIconUtils::SetSize( bitmap, iImageSize );
    AknIconUtils::SetSize( mask, iImageSize );
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiNaviMediaDecorator::ConstructIconL(): iImageSize.iWidth %d",
                            iImageSize.iWidth )
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiNaviMediaDecorator::ConstructIconL(): iImageSize.iHeight %d",
                            iImageSize.iHeight )
    
    
    CGulIcon* icon = CGulIcon::NewL();
    icon->SetBitmap( bitmap );
    icon->SetMask( mask );
    
    CleanupStack::PushL( icon );
    iIconArray->AppendL( icon );
    CleanupStack::Pop( icon );

    MUS_LOG( "mus: [MUSUI ] <- CMusUiNaviMediaDecorator::ConstructIconL()" );

    }

// end of file
