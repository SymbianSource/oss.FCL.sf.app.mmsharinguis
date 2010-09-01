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
* Description:  Class for displaying icons in viewfinder view
*
*/


// USER
#include "musuizoomindicator.h"
#include "muslogger.h"
#include "musuizoomindicatorobserver.h"

// SYSTEM
#include <aknutils.h>
#include <aknsutils.h>
#include <data_caging_path_literals.hrh>
#include <camcorder.mbg>
#include <gulicon.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <AknsDrawUtils.h>



// CONSTANTS
const TInt KMusUiZoomDisableTime = 3000000;


using namespace AknLayoutScalable_Apps;

_LIT( KMyMbmFileName, "z:Camcorder.mbm");


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiZoomIndicator::CMusUiZoomIndicator(
                        TInt aZoomMin,
                        TInt aZoomMax,
                        MMusUiZoomIndicatorObserver* aObserver )
    : iZoomMin( aZoomMin ),
      iZoomMax( aZoomMax ),
      iZoomLevel( aZoomMin ),
      iObserver( aObserver )
    {
    MUS_LOG( "mus: mus: [MUSUI ] -> CMusUiZoomIndicator::CMusUiZoomIndicator()" );
    // pass
    iDisableZoomTimer = NULL;
    iTopMargin =  5;
    iBottomMargin = 5;

    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::CMusUiZoomIndicator()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiZoomIndicator* CMusUiZoomIndicator::NewLC(
                        TInt aZoomMin,
                        TInt aZoomMax,
                        MMusUiZoomIndicatorObserver* aObserver )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::NewLC()" );
    CMusUiZoomIndicator* self = new ( ELeave ) CMusUiZoomIndicator(
                            aZoomMin,
                            aZoomMax,
                            aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();

    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::NewLC()" );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiZoomIndicator* CMusUiZoomIndicator::NewL(
                        TInt aZoomMin,
                        TInt aZoomMax,
                        MMusUiZoomIndicatorObserver* aObserver )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::NewL()" );
    CMusUiZoomIndicator* self = NewLC( aZoomMin,
                                       aZoomMax,
                                       aObserver );
    CleanupStack::Pop( self );

    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::NewL()" );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiZoomIndicator::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::ConstructL()" );

    if( iZoomMin > iZoomMax )
        {
        User::Leave( KErrArgument );
        }

    iIconElevator = new (ELeave) CEikImage();
    iIconElevator->SetContainerWindowL( *this );

    iIconMin = new (ELeave) CEikImage();
    iIconMin->SetContainerWindowL( *this );

    iIconMax = new (ELeave) CEikImage();
    iIconMax->SetContainerWindowL( *this );

    iIconBg = new (ELeave) CEikImage();
    iIconBg->SetContainerWindowL( *this );

    
    ConstructImageL( iIconMin,
                     AknsUtils::SkinInstance(),
                     KAknsIIDQsnIconColors,
                     EMbmCamcorderQgn_indi_cams_zoom_bg_min,
                     EMbmCamcorderQgn_indi_cams_zoom_bg_min);

    ConstructImageL( iIconMax,
                     AknsUtils::SkinInstance(),
                     KAknsIIDQsnIconColors,
                     EMbmCamcorderQgn_indi_cams_zoom_bg_max,
                     EMbmCamcorderQgn_indi_cams_zoom_bg_max);

    ConstructImageL( iIconBg,
                     AknsUtils::SkinInstance(),
                     KAknsIIDQsnIconColors,
                     EMbmCamcorderQgn_indi_cams_zoom_bg,
                     EMbmCamcorderQgn_indi_cams_zoom_bg);

    ConstructImageL( iIconElevator,
                     AknsUtils::SkinInstance(),
                     KAknsIIDQsnIconColors,
                     EMbmCamcorderQgn_indi_cams_zoom_elevator,
                     EMbmCamcorderQgn_indi_cams_zoom_elevator);


    UpdateZoomLayout();

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiZoomIndicator::ConstructL()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiZoomIndicator::~CMusUiZoomIndicator()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::~CMusUiZoomIndicator()" );

    DisableTimer();
    delete iDisableZoomTimer;

    delete iIconElevator;
    delete iIconMin;
    delete iIconMax;
    delete iIconBg;

    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::~CMusUiZoomIndicator()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiZoomIndicator::SetZoomLevel( TInt aZoomLevel )
    {
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiZoomIndicator::SetZoomLevel( %d ) ",
              aZoomLevel );
    if( aZoomLevel >= iZoomMin && aZoomLevel <= iZoomMax )
        {
        iZoomLevel = aZoomLevel;
        }
    DisableTimer();
    UpdateZoomLayout();
    EnableTimer();
    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::SetZoomLevel()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusUiZoomIndicator::GetZoomLevel()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::GetZoomLevel()" );
    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::GetZoomLevel()" );
    return iZoomLevel;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiZoomIndicator::SetZoomMin( TInt aZoomMin )
    {
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiZoomIndicator::SetZoomMin( %d ) ", aZoomMin );
    if( aZoomMin >= 0 && aZoomMin < iZoomMax )
        {
        iZoomMin = aZoomMin;
        }
    DisableTimer();
    UpdateZoomLayout();
    EnableTimer();
    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::SetZoomMin()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiZoomIndicator::SetZoomMax( TInt aZoomMax )
    {
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiZoomIndicator::SetZoomMax( %d ) ", aZoomMax );
    if( aZoomMax >= iZoomMin )
        {
        iZoomMax = aZoomMax;
        }
    DisableTimer();
    UpdateZoomLayout();
    EnableTimer();
    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::SetZoomMax()" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiZoomIndicator::SetFullRect( const TRect aRect  )
    {
    iFullRect = aRect;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiZoomIndicator::SizeChanged()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::SizeChanged()" );
    UpdateZoomLayout();
    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::SizeChanged()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiZoomIndicator::Draw( const TRect& aRect ) const
    {

    CWindowGc& gc = SystemGc();

    TRect rect( 0,0, iFullRect.Width(), iFullRect.Height() );
    //TRect rect( iFullRect.Width()-20, iFullRect.iTl.iY, iFullRect.iBr.iX, iFullRect.iBr.iY ); //TRect( 40,0,380,382 ) );

/*
    AknsUtils::RegisterControlPosition( this, TPoint( iFullRect.Width()-20, iFullRect.iTl.iY ) );
 //   gc.Clear( aRect );
    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    MAknsControlContext* cc = AknsDrawUtils::ControlContext( this );
    if(!AknsDrawUtils::Background( skin, cc,this,gc,rect,KAknsDrawParamDefault ))
        {
*/        
        MUS_LOG( "mus: [MUSUI ]     Skin not valid or not found" );
        gc.SetBrushColor( iEikonEnv->ControlColor( EColorWindowBackground, *this ) );
        gc.SetBrushStyle( CGraphicsContext::ESolidBrush );
        gc.Clear( aRect );
//        }    
  
}


// -----------------------------------------------------------------------------
// Draw all icons and texts to given graphics context.
// -----------------------------------------------------------------------------
//
void CMusUiZoomIndicator::DrawIcons( const TRect& /*aRect*/ )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::DrawIcons()" );
    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::DrawIcons()" );
    }


// -----------------------------------------------------------------------------
// Draw all icons and texts to given graphics context.
// -----------------------------------------------------------------------------
//
void CMusUiZoomIndicator::UpdateZoomLayout()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::UpdateZoomLayout()" );
    
    TSize sz = iIconBg->MinimumSize();
    
    TRect rect = Rect();
    
    // TODO: Remove all magic constants!
    AknLayoutUtils::LayoutImage(iIconMax, Rect(), 0, 0, 0, 0, 0, 20, 20 );
    AknLayoutUtils::LayoutImage(iIconMin, Rect(), 0, 0, 68, 0, 0, 20, 20 );
    AknLayoutUtils::LayoutImage(iIconBg, Rect(), 0, 0, 20, 0, 0, 20, 48 );
    
    // 20 - 68
    iScaleStep = (  48 << 8 ) /
                 ( ( iZoomMax - iZoomMin ) );
    iScaleY = ( 58 ) -
                            ( ( iScaleStep * iZoomLevel ) >> 8 );
    
    
    AknLayoutUtils::LayoutImage(iIconElevator, Rect(), 0, 0, iScaleY, 0, 0, 20, 20 );
    
    DrawNow();
    
    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::UpdateZoomLayout()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt CMusUiZoomIndicator::CountComponentControls() const
    {
    return 4; //iIconElevator ? 1 : 0; // return nbr of controls inside this container
    //return 0; // return nbr of controls inside this container
    }


// --------------------------------------------------------------------------
// Called by framework to retrieve the control specified by index.
// --------------------------------------------------------------------------
//
CCoeControl* CMusUiZoomIndicator::ComponentControl( TInt aIndex ) const
    {
    switch ( aIndex )
        {
        
        case 0:
            return iIconMin;
        case 1:
            return iIconMax;
        case 2:
            return iIconBg;
        case 3:
            return iIconElevator;
        
        default:
            return NULL;
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void  CMusUiZoomIndicator::ConstructImageL( CEikImage* aImage,
                                         MAknsSkinInstance* aSkin,
                                         const TAknsItemID& aId,
                                         TInt aBitmapId,
                                         TInt aMaskId )

    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::ConstructIconL()" );

    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;

    TParse parse;
    parse.Set( KMyMbmFileName, &KDC_APP_RESOURCE_DIR, NULL );

    AknsUtils::CreateIconL(aSkin,
                           aId,
                           bitmap,
                           mask,
                           parse.FullName(),
                           aBitmapId,
                           aMaskId);

    aImage->SetPicture(bitmap, mask); // Owership transferred to CEikImage



    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::ConstructIconL()" );

    }



// -----------------------------------------------------------------------------
// Callback for disable zoom indicator.
// -----------------------------------------------------------------------------
//
TInt CMusUiZoomIndicator::DisableZoomIndicator( TAny* aThis )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::DisableZoomIndicator()" );
    CMusUiZoomIndicator* indicator =
                            static_cast< CMusUiZoomIndicator* > ( aThis );
    indicator->DisableTimer();
    if( indicator->iObserver )
        {
        indicator->iObserver->DisableZoomIndicator();
        }
    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::DisableZoomIndicator()" );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiZoomIndicator::DisableTimer()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::DisableTimer()" );
    if ( iDisableZoomTimer )
        {
        if ( iDisableZoomTimer->IsActive() )
            {
            iDisableZoomTimer->Cancel();
            }
        }
    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::DisableTimer()" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiZoomIndicator::EnableTimer()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiZoomIndicator::EnableTimer()" );

    if ( !iDisableZoomTimer )
        {
        TRAP_IGNORE( iDisableZoomTimer = CPeriodic::NewL( CActive::EPriorityStandard ) );
        }
    else
        {
        DisableTimer();
        }

    // Start disable timer; callback triggers exit.
    iDisableZoomTimer->Start( KMusUiZoomDisableTime,
                             KMusUiZoomDisableTime,
                             TCallBack( DisableZoomIndicator, this ) );
    MUS_LOG( "mus: [MUSUI ] <- CMusUiZoomIndicator::EnableTimer()" );
    }
    

// End of File



