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
#include <AknIconArray.h>
#include <gulicon.h>
#include <applayout.cdl.h>
#include <e32std.h>
#include <e32base.h>

using namespace AppLayout;


// CONSTANTS
const TInt KMusUiAnimatorTimer = 50000;  // Default 50 ms, 20 fps

const TInt KIconArraySize = 15;

_LIT( KMyBitmapRomFile, "Z:\\resource\\apps\\musuiicons.mif" );
_LIT( KMyBitmapRamFile, "C:\\resource\\apps\\musuiicons.mif" );


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiNaviMediaAnimator* CMusUiNaviMediaAnimator::NewL( 
        MMusUiNaviMediaAnimatorObserver& aObserver, TSize aImageSize )
    {
    CMusUiNaviMediaAnimator* self = 
                new( ELeave ) CMusUiNaviMediaAnimator( aObserver, aImageSize );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiNaviMediaAnimator::~CMusUiNaviMediaAnimator()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiNaviMediaAnimator::~CMusUiNaviMediaAnimator" );

    if ( iAnimationTimer->IsActive() )
        {
        iAnimationTimer->Cancel();
        }
    delete iAnimationTimer;        
    delete iBitmapArray;

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiNaviMediaAnimator::~CMusUiNaviMediaAnimator" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiNaviMediaAnimator::CMusUiNaviMediaAnimator( 
                MMusUiNaviMediaAnimatorObserver& aObserver, TSize aImageSize )
                : iObserver( aObserver )
    {
    iSize = aImageSize;
    MUS_LOG1( "mus: [MUSUI ] -> \
          CMusUiNaviMediaAnimator::CMusUiNaviMediaAnimator(): iSize.iWidth %d",
                            iSize.iWidth )
    MUS_LOG1( "mus: [MUSUI ] -> \
          CMusUiNaviMediaAnimator::CMusUiNaviMediaAnimator(): iSize.iHeight %d",
                            iSize.iHeight )
    iBitmapArray = NULL;
    iInterval = KMusUiAnimatorTimer;
    iIndex = 0;



    
/*    iAnimationTimer->Start( iInterval,
                            iInterval,
                            TCallBack( AnimationInterval, this ) );
                            */
    }


// -----------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------
//
void CMusUiNaviMediaAnimator::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiNaviMediaAnimator::ConstructL" );

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
    
    iAnimationTimer = CPeriodic::NewL( CActive::EPriorityStandard );
    
    iAnimationTimer->Start( iInterval,
                            iInterval,
                            TCallBack( AnimationInterval, this ) );


    MUS_LOG( "mus: [MUSUI ]  <- CMusUiNaviMediaAnimator::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
//
// -----------------------------------------------------------------------------
//
void CMusUiNaviMediaAnimator::SetInterval( const TInt aAnimationInterval )
    {
    iInterval = aAnimationInterval;


    if ( iAnimationTimer->IsActive() )
        {
        iAnimationTimer->Cancel();
        }
    
    iAnimationTimer->Start( iInterval,
                            iInterval,
                            TCallBack( AnimationInterval, this ) );
    }


// -----------------------------------------------------------------------------
// Callback for disable level indicator.
// -----------------------------------------------------------------------------
//
TInt CMusUiNaviMediaAnimator::AnimationInterval( TAny* aThis )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiNaviMediaAnimator::AnimationInterval()" );
    CMusUiNaviMediaAnimator* animator =
                            static_cast< CMusUiNaviMediaAnimator* > ( aThis );
                            
    
    if ( animator->iBitmapArray ) 
        {
        animator->iObserver.SetIcon( animator->iBitmapArray->At( animator->iIndex ) );
        if( animator->iIndex < ( animator->iBitmapArray->Count() - 1 ) ) 
            {
            animator->iIndex++;
            }
        else 
            {
            animator->iIndex = 0;
            }
        }
        
    MUS_LOG( "mus: [MUSUI ] <- CMusUiNaviMediaAnimator::AnimationInterval()" );
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// Layouts the text depending on current icon situation.
// -----------------------------------------------------------------------------
//
void CMusUiNaviMediaAnimator::LoadImagesL()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiNaviMediaAnimator::DoLayoutText" );

    delete iBitmapArray;
    iBitmapArray = NULL;
    iBitmapArray = new ( ELeave ) CAknIconArray( KIconArraySize );

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
                     EMbmMusuiiconsQgn_indi_video_wait1,
                     EMbmMusuiiconsQgn_indi_video_wait1_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_wait2,
                     EMbmMusuiiconsQgn_indi_video_wait2_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_wait3,
                     EMbmMusuiiconsQgn_indi_video_wait3_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_wait4,
                     EMbmMusuiiconsQgn_indi_video_wait4_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_wait5,
                     EMbmMusuiiconsQgn_indi_video_wait5_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_wait6,
                     EMbmMusuiiconsQgn_indi_video_wait6_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_wait7,
                     EMbmMusuiiconsQgn_indi_video_wait7_mask,
                     parse );

    ConstructImageL( skin,
                     item,
                     EMbmMusuiiconsQgn_indi_video_wait8,
                     EMbmMusuiiconsQgn_indi_video_wait8_mask,
                     parse );

                     
    
    MUS_LOG( "mus: [MUSUI ] <- CMusUiNaviMediaAnimator::DoLayoutText" );
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void  CMusUiNaviMediaAnimator::ConstructImageL( MAknsSkinInstance* aSkin,
                                                const TAknsItemID& aId,
                                                TInt aBitmapId,
                                                TInt aMaskId,
                                                TParse parse )

    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiNaviMediaAnimator::ConstructImageL()" );

    CFbsBitmap* bitmap = NULL;
    CFbsBitmap* mask = NULL;


    AknsUtils::CreateIconL(aSkin,
                           aId,
                           bitmap,
                           mask,
                           parse.FullName(),
                           aBitmapId,
                           aMaskId);

    AknIconUtils::SetSize( bitmap, iSize );
    AknIconUtils::SetSize( mask, iSize );
    
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiNaviMediaAnimator::ConstructImageL(): iSize.iWidth %d",
                            iSize.iWidth )
    MUS_LOG1( "mus: [MUSUI ] -> CMusUiNaviMediaAnimator::ConstructImageL(): iSize.iHeight %d",
                            iSize.iHeight )
    
    CGulIcon* icon = CGulIcon::NewL();
    icon->SetBitmap( bitmap );
    icon->SetMask( mask );
    
    CleanupStack::PushL( icon );
    iBitmapArray->AppendL( icon );
    CleanupStack::Pop( icon );

    //aImage->SetPicture(bitmap, mask); // Owership transferred to CEikImage

    MUS_LOG( "mus: [MUSUI ] <- CMusUiNaviMediaAnimator::ConstructImageL()" );

    }

