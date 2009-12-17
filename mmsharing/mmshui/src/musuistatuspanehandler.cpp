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



#include "musuistatuspanehandler.h"
#include "musuidefinitions.h"
#include "muslogger.h" // debug logging
#include <musui.mbg>

#include <eikspane.h>
#include <barsread.h>
#include <StringLoader.h>
#include <akntitle.h>
#include <aknenv.h>
#include <akncontext.h>
#include <AknIndicatorContainer.h>
#include <aknnavi.h> // Navi pane container ( CAknNavigationControlContainer )
#include <AknVolumePopup.h>
#include <eikapp.h>
#include <eikenv.h>
#include <eikappui.h>


// constants
#define KMUSMBMFILENAME iEikonEnvironment.EikAppUi()->Application()->BitmapStoreName()


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiStatusPaneHandler* CMusUiStatusPaneHandler::NewL( CEikonEnv& aEikonEnv )
    {
    CMusUiStatusPaneHandler* self = 
                        new ( ELeave ) CMusUiStatusPaneHandler( aEikonEnv );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiStatusPaneHandler::~CMusUiStatusPaneHandler()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::~CMusUiStatusPaneHandler" );
    
    delete iNaviPaneText;
    delete iMediaControl;
    delete iVolumePopup;
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::~CMusUiStatusPaneHandler" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiStatusPaneHandler::CMusUiStatusPaneHandler( CEikonEnv& aEikonEnv )
    :iEikonEnvironment( aEikonEnv )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiStatusPaneHandler::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::ConstructL" );
    
    CEikStatusPane* statusPane = iEikonEnvironment.AppUiFactory()->StatusPane();
    
    iNaviPane = STATIC_CAST(
                CAknNavigationControlContainer*,
                statusPane->ControlL( TUid::Uid( EEikStatusPaneUidNavi ) ) );
    iTitlePane = STATIC_CAST(
                CAknTitlePane*,
                statusPane->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
    iContextPane = STATIC_CAST(
                CAknContextPane*,
                statusPane->ControlL( TUid::Uid( EEikStatusPaneUidContext ) ) );

    iVolumePopup = CAknVolumePopup::ConstructFromResourceL( 
                                        R_AVKON_NAVI_PANE_VOLUME_INDICATOR );

    iMediaControl = CreateMediaNaviL();
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::ConstructL" );
    }


// -----------------------------------------------------------------------------
// Updates statuspane icons
// -----------------------------------------------------------------------------
//
void CMusUiStatusPaneHandler::SetStatusPaneIconsL( TInt32 aIconId,
                                                   TInt32 aIconMaskId )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::SetStatusPaneIconsL" )

	iContextPane->SetPictureFromFileL( KMUSMBMFILENAME, aIconId, aIconMaskId );
													 
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::SetStatusPaneIconsL" )
    }


// -----------------------------------------------------------------------------
// Updates statuspane icons
// -----------------------------------------------------------------------------
//
void CMusUiStatusPaneHandler::SetStatusPaneIconsL( CFbsBitmap* aBitmap )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::SetStatusPaneIconsL" );
    CEikImage* image = new (ELeave) CEikImage();
    image->SetBitmap( aBitmap );
    CEikImage* oldImage = iContextPane->SwapPicture( image );
    delete oldImage;
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::SetStatusPaneIconsL" );
    }


// -----------------------------------------------------------------------------
// Hides navi-pane decorators and clears text
// -----------------------------------------------------------------------------
//
void CMusUiStatusPaneHandler::ClearNaviPaneL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::ClearNaviPaneL" );
    delete iNaviPaneText;
    iNaviPaneText = NULL;

    // Pushes default decorator object (empty one) to top of the stack
    // and draws it
    iNaviPane->PushDefaultL();

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::ClearNaviPaneL" );
    }


// -----------------------------------------------------------------------------
// Sets title-pane text
// -----------------------------------------------------------------------------
//
void CMusUiStatusPaneHandler::SetTitleL( const TDesC& aTitle )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::SetTitleL" );
    iTitlePane->SetTextL( aTitle );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::SetTitleL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiStatusPaneHandler::SetTitleFromResourceL( TInt aResourceId )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::SetTitleFromResourceL" );
    HBufC* titlePaneTitle =  StringLoader::LoadLC( aResourceId );
    iTitlePane->SetTextL( *titlePaneTitle );
    CleanupStack::PopAndDestroy( titlePaneTitle );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::SetTitleFromResourceL" );
    }
    

// -----------------------------------------------------------------------------
// Get the current volume setting.
// -----------------------------------------------------------------------------
//
TInt CMusUiStatusPaneHandler::GetVolumeControlValue() const
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::GetVolumeControlValue" );
             
    TInt volume = iVolumePopup->Value();
    
    MUS_LOG1( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::GetVolumeControlValue: [%d]",
                            volume );                        
    return volume;
    }


// -----------------------------------------------------------------------------
// Activates volume control.
// -----------------------------------------------------------------------------
//
void CMusUiStatusPaneHandler::ActivateVolumeControlL( TInt aVolume )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::ActivateVolumeControlL" );

    if ( aVolume < KMusVolumeMuted || aVolume > KMusVolumeMaximum )
        {
        MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::SetVolumeControlValue: returning" );
        return;
        }
        
    iVolumePopup->SetValue( aVolume );
    iVolumePopup->ShowVolumePopupL();
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::ActivateVolumeControlL" );
    }
    

// -----------------------------------------------------------------------------
// Activate media navi pane.
// -----------------------------------------------------------------------------
//
void CMusUiStatusPaneHandler::ActivateNaviPaneMediaL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::ActivateNaviPaneMediaL" );
    iNaviPane->PushL( *iMediaControl );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::ActivateNaviPaneMediaL" );
    }
  

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiStatusPaneHandler::ShowNaviPaneIconL(
                        TMusUiNaviMediaDecorator aIcon )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::ShowNaviPaneIconL" );
    CMusUiNaviMediaDecorator* mediaNavi =
                            static_cast<CMusUiNaviMediaDecorator*>(
                                            iMediaControl->DecoratedControl() );
    mediaNavi->ShowIcon( aIcon );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::ShowNaviPaneIconL" );
    }


// -----------------------------------------------------------------------------
// Sets navi-pane label for left side
// -----------------------------------------------------------------------------
//
void CMusUiStatusPaneHandler::SetLeftLabelL( const TDesC& aLabel )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::SetLeftLabelL" );

    static_cast<CMusUiNaviMediaDecorator*>
                    ( iMediaControl->DecoratedControl() )->SetLeftLabelL( aLabel );
    iNaviPane->DrawNow();
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::SetLeftLabelL" );
    }


// -----------------------------------------------------------------------------
// Sets navi-pane label for right side
// -----------------------------------------------------------------------------
//
void CMusUiStatusPaneHandler::SetRightLabelL( const TDesC& aLabel )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::SetRightLabelL" );

    static_cast<CMusUiNaviMediaDecorator*>
                    ( iMediaControl->DecoratedControl() )->SetRightLabelL( aLabel );
    iNaviPane->DrawNow();
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::SetRightLabelL" );
    }


// -----------------------------------------------------------------------------
// Create media navi pane.
// -----------------------------------------------------------------------------
//
CAknNavigationDecorator* CMusUiStatusPaneHandler::CreateMediaNaviL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiStatusPaneHandler::CreateMediaNaviL" );
    CMusUiNaviMediaDecorator* naviDecorator = CMusUiNaviMediaDecorator::NewL();
    CleanupStack::PushL( naviDecorator );

    CAknNavigationDecorator* decoratedFolder =
                            CAknNavigationDecorator::NewL(
                                    iNaviPane,
                                    naviDecorator,
                                    CAknNavigationDecorator::ENotSpecified );
    CleanupStack::Pop( naviDecorator );// decoratedFolder owns naviDecorator now

    CleanupStack::PushL( decoratedFolder );
    decoratedFolder->SetContainerWindowL( *iNaviPane );
    naviDecorator->SetContainerWindowL( *decoratedFolder );
    CleanupStack::Pop( decoratedFolder );

    decoratedFolder->MakeScrollButtonVisible( EFalse );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiStatusPaneHandler::CreateMediaNaviL" );
    return decoratedFolder;
    }


// -----------------------------------------------------------------------------
// Get Volump Popup control pointer
// -----------------------------------------------------------------------------
//
CAknVolumePopup* CMusUiStatusPaneHandler::GetVolumePopup() const
    {
    return iVolumePopup;
    }


// end of file
