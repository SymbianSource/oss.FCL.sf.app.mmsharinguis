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
#include "musuiclipsharingviewcontainer.h"
#include "musuieventcontroller.h"
#include "musuilevelindicator.h"
#include "musuiclipsharingview.h"
#include "musuiappui.h"
#include "musuidialogutil.h"
#include "musui.hrh"
#include "muslogger.h" // debug logging
#include "mussettings.h"
#include <musui.rsg>


#include <AknsDrawUtils.h>
#include <AknProgressDialog.h>
#include <eikprogi.h>                   // CEikProgressInfo
#include <AknDef.h>
#include <MGFetch.h>
#include <MediaFileTypes.hrh>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingViewContainer::ConstructL( CMusUiGeneralView* aView,
                                                 const TRect& aRect )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingViewContainer::ConstructL" );
    
    CMusUiSendViewContainer::ConstructL( aView, 
                                         aRect,
                                         EAknOrientationHorizontal,
                                         ETrue,
                                         EFalse );
    // check if operator specific functionality is needed  
    iOperatorSpecificFunctionality = 
        ( MultimediaSharingSettings::OperatorVariantSettingL() == 
                                        MusSettingsKeys::EOperatorSpecific );
    
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingViewContainer::ConstructL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiClipSharingViewContainer::~CMusUiClipSharingViewContainer()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingViewContainer::~CMusUiClipSharingViewContainer" );
    
    delete iFileName;

    TRAP_IGNORE( CancelTranscodingL() );

    if ( iTranscodingDialog )
        {
        TRAP_IGNORE( iTranscodingDialog->ProcessFinishedL() );
        }

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingViewContainer::~CMusUiClipSharingViewContainer" );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingViewContainer::ShowTranscodingProgressDialogL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingViewContainer::ShowTranscodingProgressDialogL" );
    iTranscodingOngoing = ETrue;
    HBufC* transcodText;
    iTranscodingDialog =
        new ( ELeave ) CAknProgressDialog(
            ( reinterpret_cast<CEikDialog**>( &iTranscodingDialog ) ) );

    iTranscodingDialog->SetCallback( this );
    transcodText = iEikonEnv->AllocReadResourceLC(
                            R_MUS_PROGRESS_NOTE_CONVERTING_CLIP );
    iTranscodingDialog->SetCurrentLabelL( EAknCtNote, *transcodText );
    CleanupStack::PopAndDestroy( transcodText );
    iTranscodingDialog->ExecuteLD( R_MUS_VIEW_TRANSCODING_WAIT_NOTE );
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingViewContainer::ShowTranscodingProgressDialogL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingViewContainer::UpdateClipTranscodingPercentageL(
                        TInt aPercentage )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingViewContainer::UpdateClipTranscodingPercentageL" );
    if ( iTranscodingDialog )
        {
        if ( aPercentage < KMusOneHundredPercent )
            {
            iTranscodingProgress = iTranscodingDialog->GetProgressInfoL();
            iTranscodingProgress->SetFinalValue( KMusOneHundredPercent );
            if ( iTranscodingProgress )
                {
                iTranscodingProgress->IncrementAndDraw(
                        aPercentage - iTranscodingProgress->CurrentValue() );
                }
            }
        else
            {
            iTranscodingOngoing = EFalse;
            iTranscodingDialog->ProcessFinishedL();
            iTranscodingDialog = NULL;
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingViewContainer::UpdateClipTranscodingPercentageL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingViewContainer::CancelTranscodingL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingViewContainer::CancelTranscodingL" );
    if ( iTranscodingOngoing )
        {
        iTranscodingOngoing = EFalse;
        MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingViewContainer::CancelTranscodingL: iTranscodingOngoing" );
        if ( iTranscodingDialog )
            {
            MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingViewContainer::CancelTranscodingL: iTranscodingDialog" );
            iTranscodingDialog->ProcessFinishedL();
            iTranscodingDialog = NULL;
            MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingViewContainer::CancelTranscodingL: ProcessFinishedL()" );
            }
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingViewContainer::CancelTranscodingL" );
    }


// -----------------------------------------------------------------------------
// Runs the Media Gallery Fetch dialog.
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingViewContainer::RunFetchDialogL()
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingViewContainer::RunFetchDialogL" );

    // Create array of descriptors for the selected files
    CDesCArrayFlat* fileArray = new ( ELeave ) CDesCArrayFlat( KMusDefaultArrayGranularity );
    CleanupStack::PushL( fileArray );

    // Open the dialog.
    TBool ret = MGFetch::RunL(
        *fileArray, // When dialog is closed, fileArray contains selected files
        EVideoFile, // Displays only media files of type aMediaType
        EFalse,     // Not used in S60 2.0 ( single or multiple file selection )
        this        // Pointer to class implementing MMGFetchVerifier;
                    // when user has selected file( s ),
                    // MMGFetchVerifier::VerifySelectionL is called.
        );

    if ( ret )
        {
        iFileSelected = ETrue;
        iFileName = fileArray->MdcaPoint( 0 ).AllocL();
        }
    else
        {
        // The user has not selected any file
        iFileSelected = EFalse;
        }

    CleanupStack::PopAndDestroy( fileArray );

    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingViewContainer::RunFetchDialogL" );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusUiClipSharingViewContainer::FetchedFileName()
    {
    MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingViewContainer::FetchedFileName" );
    return *iFileName;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiClipSharingViewContainer::FileSelected()
    {
    MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingViewContainer::FileSelected" );
    return iFileSelected;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingViewContainer::SetDurationValue( TInt aDurationValue )
    {
    const TInt KMinLevel = 0;
    iIndicator->SetRange( KMinLevel, aDurationValue, EFalse );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingViewContainer::SetPositionValueL( TInt aPositionValue )
    {
    iIndicator->SetLevelL( EMusUiIndicatorTypeDuration, 
                           aPositionValue, 
                           EFalse /*never use timer*/ );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingViewContainer::SetDurationIndicatorVisible( TBool aVisible )
    {
    iIndicator->SetIndicatorType(EMusUiIndicatorTypeDuration);
    iIndicator->MakeVisible( aVisible );
    }
    

// -----------------------------------------------------------------------------
// Verifies the current user selection.
// -----------------------------------------------------------------------------
//
TBool CMusUiClipSharingViewContainer::VerifySelectionL( const MDesCArray* 
                                                        aSelectedFiles )
    {
    MUS_LOG( "mus: [MUSUI ]     CMusUiClipSharingViewContainer::\
                                VerifySelectionL" );
    return ( aSelectedFiles->MdcaCount() > 0 ) ? ETrue : EFalse;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingViewContainer::DialogDismissedL( TInt /*aButtonId*/ )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiClipSharingViewContainer::DialogDismissedL" );
    if ( iTranscodingOngoing )
        {
        (static_cast <CMusUiClipSharingView*>(MusUiView()))->CancelTranscodingL();
        iTranscodingOngoing = EFalse;
        }
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiClipSharingViewContainer::DialogDismissedL" );
    }


// -----------------------------------------------------------------------------
// Called by framework when to retrieve the no. of controls.
// -----------------------------------------------------------------------------
//
TInt CMusUiClipSharingViewContainer::CountComponentControls() const
    {
    return 1; // return nbr of controls inside this container
    }


// -----------------------------------------------------------------------------
// Called by framework to retrieve the control specified by index.
// -----------------------------------------------------------------------------
//
CCoeControl* CMusUiClipSharingViewContainer::ComponentControl( TInt aIndex ) const
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
// Called by framework to redraw the screen area.
// -----------------------------------------------------------------------------
//
void CMusUiClipSharingViewContainer::Draw( const TRect& aRect ) const
    {
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
    }

// -----------------------------------------------------------------------------
// Called by framework to act on key events if required.
// -----------------------------------------------------------------------------
//
TKeyResponse CMusUiClipSharingViewContainer::OfferKeyEventL(
                        const TKeyEvent& aKeyEvent,
                        TEventCode aType )
    {
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiClipSharingViewContainer::OfferKeyEventL: %d",
                            aKeyEvent.iCode );
    MUS_LOG1( "mus: [MUSUI ]  -> CMusUiClipSharingViewContainer::OfferKeyEventL: %d",
                            aKeyEvent.iScanCode );

    TKeyResponse ret( EKeyWasNotConsumed );

    /**
    * special keys have scancode
    */
    switch( aKeyEvent.iScanCode )
        {
        // Fast Forward:
        case EStdKeyUpArrow: // EKeyUpArrow:
            {                
            if ( !MUS_NO_TOOLBAR )
                {
            if( aType == EEventKeyDown )
                {
                (static_cast <CMusUiClipSharingView*>
                        (MusUiView()))->HandleCommandL( EMusuiCmdViewFastForward );
                }

            if( aType == EEventKeyUp )
                {
                (static_cast <CMusUiClipSharingView*>
                        (MusUiView()))->HandleCommandL( EMusuiCmdViewFastForwardEnd );
                    }
                }
            return EKeyWasConsumed;
            }

        // Fast Reverse:
        case EStdKeyDownArrow: // EKeyDownArrow:
            {
            if ( !MUS_NO_TOOLBAR )
            {
            if( aType == EEventKeyDown )
                {
                (static_cast <CMusUiClipSharingView*>
                            (MusUiView()))->HandleCommandL( EMusuiCmdViewFastReverse );
                }
            if( aType == EEventKeyUp )
                {
                (static_cast <CMusUiClipSharingView*>
                            (MusUiView()))->HandleCommandL( EMusuiCmdViewFastReverseEnd );
                    }
                }
            return EKeyWasConsumed;
            }
        default:
            {
            ret = EKeyWasNotConsumed;
            break;
            }
        }


    switch ( aKeyEvent.iCode )
        {
        // Volume Control
        case EKeyLeftArrow:
            {
            (static_cast <CMusUiClipSharingView*>
                    (MusUiView()))->HandleCommandL( EMusuiCmdViewVolumeDown );
            ret = EKeyWasConsumed;
            break;
            }
        case EKeyRightArrow:
            {
            (static_cast <CMusUiClipSharingView*>
                    (MusUiView()))->HandleCommandL( EMusuiCmdViewVolumeUp );
            ret = EKeyWasConsumed;
            break;
            }

        default:
            {
            ret = EKeyWasNotConsumed;
            break;
            }
        }
    
    return ret;
    }


// end of file
