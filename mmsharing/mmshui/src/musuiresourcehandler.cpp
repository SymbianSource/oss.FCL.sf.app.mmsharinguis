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
* Description:  Handler for camera, keypad and video player resources
*
*/



#include "musuiresourcehandler.h"
#include "musuipropertywatch.h"
#include "musuieventobserver.h"
#include "musuidialogutil.h"
#include "muslogger.h"
#include "mussettings.inl"

#include <musui.rsg>
#include <AknWaitDialog.h>
#include <StringLoader.h>

using namespace NMusResourceApi;


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiResourceHandler::~CMusUiResourceHandler()
    {    
    delete iCameraResourceWatch;
    delete iKeypadResourceWatch;
    
    delete iChangeProductModeDialog;
    delete iDialogPrompt;
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusUiResourceHandler* CMusUiResourceHandler::NewL( 
                                            MMusUiEventObserver& aObserver )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiResourceHandler::NewL()" )
    
    CMusUiResourceHandler* self = 
                            new ( ELeave ) CMusUiResourceHandler( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    MUS_LOG( "mus: [MUSUI ] <- CMusUiResourceHandler::NewL()" )
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiResourceHandler::RequestKeypadL( TBool aMandatory )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiResourceHandler::RequestKeypadL" )
    
    TBool keypadAvailable( EFalse ); 
    
    switch ( iKeypadAvailability )
        {        
        case NMusResourceApi::EAvailable:
            {
            MUS_LOG( "mus: [MUSUI ]     Keypad is available" )            
		    break;		            
            }			         
        case NMusResourceApi::ENotAvailable:
            {
            MUS_LOG( "mus: [MUSUI ]     Keypad is not available, changing mode" )
            iKeypadAvailibilityInProgress = ETrue;
            delete iDialogPrompt;
            iDialogPrompt = NULL;
            iDialogPrompt = StringLoader::LoadL( R_MUS_CHANGE_KEYPAD_MODE_TXT );
            ShowChangeProductModeDialogL( *iDialogPrompt );            
            MUS_LOG( "                 Product mode changed" )
            MUS_LOG1( "                 Keypad availability is %d", 
                      iKeypadAvailability )            
            break;
            }
        default:
            {
            MUS_LOG( "mus: [MUSUI ]     availability default." )
            break;
            }
            
        }
        
    switch(iKeypadUsability)
    	{
    	case MusSettingsKeys::EUsabilityLandscape:
            {
            MUS_LOG( "mus: [MUSUI ]     Keypad is usable in landscape" )
            iObserver.SwitchOrientationL( 
                                    CAknAppUiBase::EAppUiOrientationLandscape );            
            break;
            }
        case MusSettingsKeys::EUsabilityPortrait:
            {
            MUS_LOG( "mus: [MUSUI ]     Keypad is usable in portrait" )
            iObserver.SwitchOrientationL( 
                                    CAknAppUiBase::EAppUiOrientationPortrait );            
            break;
            }	
    	case MusSettingsKeys::EUsabilityAlways:
    		{
    		MUS_LOG( "mus: [MUSUI ]     Keypad is usable in all mode" )
    		keypadAvailable = ETrue;	
    		}
    		break;
    	default:
    	  	MUS_LOG( "mus: [MUSUI ]     usuability default ." )
    	  	break;
    	}
    	
    keypadAvailable = IsCurrentlyAvailable( iKeypadAvailability,
		                                    iKeypadUsability );
            
    if ( aMandatory && !keypadAvailable )
        {
        MUS_LOG( "mus: [MUSUI ]     Mandatory keypad not available, exiting" )
        MusUiDialogUtil::ShowGlobalInformationDialogL( 
                                R_MUS_LIVE_SHARING_VIEW_NOTE_SHARING_ENDED );
        iObserver.HandleExit();
        }
    
    MUS_LOG1( "mus: [MUSUI ]  <- CMusUiResourceHandler::RequestKeypadL( %d )",
              keypadAvailable )
    
    return keypadAvailable;
    
    
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiResourceHandler::RequestCameraL( TBool aMandatory )
    {
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiResourceHandler::RequestCameraL" )
    
    TBool cameraAvailable( EFalse );
    
    switch ( iCameraAvailability )
        {        
        case NMusResourceApi::EAvailable:
            {
            MUS_LOG( "mus: [MUSUI ]     Camera is available" )            
            cameraAvailable = ETrue;
            break;
            }
        
        case NMusResourceApi::ENotAvailable:
            {
            MUS_LOG( "mus: [MUSUI ]     Camera is not available, changing mode" )

            iCameraAvailibilityInProgress = ETrue;
            delete iDialogPrompt;
            iDialogPrompt = NULL;
            iDialogPrompt = StringLoader::LoadL(
                R_MUS_LIVE_SHARING_VIEW_OPEN_CAMERA_LENS_COVER_WAIT_NOTE_TXT );
            ShowChangeProductModeDialogL( *iDialogPrompt );
            
            MUS_LOG( "                 Product mode changed" )
            MUS_LOG1( "                 Camera availability is %d", 
                      iCameraAvailability )                                  
            break;
            }
        default:
        	MUS_LOG( "mus: [MUSUI ]     availability default." )
            break;
        }
        
    switch(iCameraUsability)
    	{
    	case MusSettingsKeys::EUsabilityLandscape:
            {
            MUS_LOG( "mus: [MUSUI ]     Camera is usable in landscape" )
            iObserver.SwitchOrientationL( 
                                CAknAppUiBase::EAppUiOrientationLandscape );            
            break;
            }
        case MusSettingsKeys::EUsabilityPortrait:
            {
            MUS_LOG( "mus: [MUSUI ]     Camera is usable in portrait" )
            iObserver.SwitchOrientationL( 
                                CAknAppUiBase::EAppUiOrientationPortrait );           
            break;
            }	
        case MusSettingsKeys::EUsabilityAlways:
    		{
    		MUS_LOG( "mus: [MUSUI ]     camera is usable in all mode" )
    		cameraAvailable = ETrue;	
    		}
    	default:
            {
            MUS_LOG( "mus: [MUSUI ]     usuability default." )
            break;
            }
    	}
    
    cameraAvailable = IsCurrentlyAvailable( iCameraAvailability,
		                                    iCameraUsability );
    if ( aMandatory && !cameraAvailable )
        {
        MUS_LOG( "mus: [MUSUI ]     Mandatory camera not available, exiting" )
        MusUiDialogUtil::ShowGlobalInformationDialogL( 
                                R_MUS_LIVE_SHARING_VIEW_NOTE_SHARING_ENDED ); 
        iObserver.HandleExit();        
        }
    
    MUS_LOG1( "mus: [MUSUI ]  <- CMusUiResourceHandler::RequestCameraL( %d )",
              cameraAvailable )
        
    return cameraAvailable;
    }
 

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiResourceHandler::RequestVideoPlayerL( TBool aMandatory )
    { 
    MUS_LOG( "mus: [MUSUI ]  -> CMusUiResourceHandler::RequestVideoPlayerL" )
    
    TBool videoPlayerAvailable( EFalse );
    
    /* Video Player will be always available. There is no dynamic variation */
    NMusResourceApi::TAvailability vpAvailability = NMusResourceApi::EAvailable;
    
    switch ( iVideoplayerUsability )
        {       
        case MusSettingsKeys::EUsabilityAlways:
            {
            MUS_LOG( "mus: [MUSUI ]     Video player is usuable in all mode" )            
            videoPlayerAvailable = ETrue;
            break;
            }
        case MusSettingsKeys::EUsabilityLandscape:
            {
            MUS_LOG( "mus: [MUSUI ]     Video player is usable in landscape" )

            iObserver.SwitchOrientationL( 
                                    CAknAppUiBase::EAppUiOrientationLandscape );
            videoPlayerAvailable = IsCurrentlyAvailable( vpAvailability,
                                                         iVideoplayerUsability );
            break;
            }
        case MusSettingsKeys::EUsabilityPortrait:
            {
            MUS_LOG( "mus: [MUSUI ]     Video player is usable in portrait" )

            iObserver.SwitchOrientationL( 
                                    CAknAppUiBase::EAppUiOrientationPortrait );
            videoPlayerAvailable = IsCurrentlyAvailable( vpAvailability,
                                                         iVideoplayerUsability );
            break;
            }        
        default:
            {
            MUS_LOG( "mus: [MUSUI ]     usuability default." )
            break;
            }
        }
    
    if ( aMandatory && !videoPlayerAvailable )
        {
        MUS_LOG( "mus: [MUSUI ]     Mandatory player not available, exiting" )
        MusUiDialogUtil::ShowGlobalInformationDialogL( 
                                R_MUS_LIVE_SHARING_VIEW_NOTE_SHARING_ENDED );
        iObserver.HandleExit();
        }
        
    MUS_LOG1( "mus: [MUSUI ]  <- CMusUiResourceHandler::RequestVideoPlayerL(%d)",
              videoPlayerAvailable )
    
    return videoPlayerAvailable;
    }


// -----------------------------------------------------------------------------
// Called by CMusUiPropertyWatch, when resource key values change in P&S
// -----------------------------------------------------------------------------
//
void CMusUiResourceHandler::PropertyChanged( const TUint aKey,
                                             const TInt aValue )
    {
    
    MUS_LOG2( "mus: [MUSUI ] -> CMusUiResourceHandler::ResourcePropertyChanged:\
              aKey: [%u] aValue: [%d]", aKey, aValue )
              
    switch( aKey )
        {
        case NMusResourceApi::KCameraAvailability:
            {
            MUS_LOG( "mus: [MUSUI ]  KCameraAvailability   " )            
            iCameraAvailability = static_cast< TAvailability >( aValue );
            if ( iCameraAvailibilityInProgress && iChangeProductModeDialog )
                {
                TRAP_IGNORE( iChangeProductModeDialog->ProcessFinishedL() );
                }
            iCameraAvailibilityInProgress = EFalse;
            break;
            }
        case NMusResourceApi::KKeypadAvailability:
            {
            MUS_LOG( "mus: [MUSUI ]  KKeypadAvailability   " )            
            iKeypadAvailability = static_cast< TAvailability >( aValue );
            if ( iKeypadAvailibilityInProgress && iChangeProductModeDialog )
                {
                TRAP_IGNORE( iChangeProductModeDialog->ProcessFinishedL() );
                }
            iKeypadAvailibilityInProgress = EFalse;
            break;
            }       
        default:
            {
            MUS_LOG( "mus: [MUSUI ]     avilability property watch default." )
            break;
            }
            
        }
        
    MUS_LOG( "mus: [MUSUI ]  <- CMusUiResourceHandler::ResourcePropertyChanged" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusUiResourceHandler::HandlePropertyError( const TInt aReason )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiResourceHandler::HandlePropertyError()" )

    iObserver.HandleError( aReason );
    }


// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
CMusUiResourceHandler::CMusUiResourceHandler( MMusUiEventObserver& aObserver )
    :iObserver( aObserver ),
     iCameraAvailability(NMusResourceApi::EAvailable),
     iKeypadAvailability(NMusResourceApi::EAvailable),
     iCameraUsability(MusSettingsKeys::EUsabilityAlways),
     iKeypadUsability(MusSettingsKeys::EUsabilityAlways),
     iVideoplayerUsability(MusSettingsKeys::EUsabilityAlways)
    {
    }


// -----------------------------------------------------------------------------
// Symbian second phase constructor
// -----------------------------------------------------------------------------
//
void CMusUiResourceHandler::ConstructL()
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiResourceHandler::ConstructL()" )
    
	iCameraAvailibilityInProgress = EFalse;
    iKeypadAvailibilityInProgress = EFalse;     
             
    // Read the value from cenrep and check whether camera,keypad
    // and videoplayer usability.
    MultimediaSharingSettings::ResourceUsability( iCameraUsability ,
    											  iKeypadUsability,
    											  iVideoplayerUsability);
    
    // Read the value from cenrep and check whether camera and keypad
    // available dynamically.
    MusSettingsKeys::TAvailability camera,keypad;    
    MultimediaSharingSettings::ResourceAvailability( camera ,
    												 keypad);
    
    TInt err = KErrNone;
    
    // Set Camera availability     
    if(camera == MusSettingsKeys::EAvailabilityDynamic)    												 
    	{
    	// read the PS key intial value
    	err = RProperty::Get(
                            NMusResourceApi::KCategoryUid,
                            NMusResourceApi::KCameraAvailability,
                            (TInt&)iCameraAvailability );
        if(err) 
        	{
        	MUS_LOG1( "mus: [MUSUI ] Error = %d",err )
        	iCameraAvailability = NMusResourceApi::ENotAvailable;
        	}
        // watch the camera ps key for further change
    	iCameraResourceWatch = CMusUiPropertyWatch::NewL(
                            *this,
                            NMusResourceApi::KCategoryUid,
                            KCameraAvailability ); 
    	}
    else
    	{
    	iCameraAvailability = NMusResourceApi::EAvailable;	
    	}
    	
    // Set Keypad availability    
     if(keypad == MusSettingsKeys::EAvailabilityDynamic)    												 
    	{
    	// read the PS key intial value
    	err = RProperty::Get(
                            NMusResourceApi::KCategoryUid,
                            NMusResourceApi::KKeypadAvailability,
                            (TInt&)iKeypadAvailability ); 
        if(err) 
        	{
        	MUS_LOG1( "mus: [MUSUI ] Error = %d",err )
        	iKeypadAvailability = NMusResourceApi::ENotAvailable;
        	}
    	// watch the keypad ps key
    	iKeypadResourceWatch = CMusUiPropertyWatch::NewL(
                            		*this,
                            		NMusResourceApi::KCategoryUid,
                            		KKeypadAvailability );        
    	}
    else
    	{
    	iKeypadAvailability = NMusResourceApi::EAvailable;	
    	}
    
    // Videoplayer availability always static
     
    MUS_LOG( "mus: [MUSUI ] <- CMusUiResourceHandler::ConstructL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiResourceHandler::ShowChangeProductModeDialogL( 
                                                        const TDesC& aPrompt )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiResourceHandler::ShowChangeProductModeDialogL()" )
    
    TBool returnValue = EFalse;

    if ( !iChangeProductModeDialog )
        {    
        iChangeProductModeDialog = new ( ELeave ) CAknWaitDialog( (
                reinterpret_cast<CEikDialog**>( &iChangeProductModeDialog )  ), 
                ETrue );
                    
        iChangeProductModeDialog->SetTextL( aPrompt );
        
        iChangeProductModeDialog->PrepareLC( 
                        R_MUS_LIVE_SHARING_VIEW_ACTIVATE_CAMERA_WAIT_NOTE );
        returnValue = iChangeProductModeDialog->RunLD();
        
        iChangeProductModeDialog = NULL;
        }
    
    MUS_LOG( "mus: [MUSUI ] <- CMusUiResourceHandler::ShowChangeProductModeDialogL()" )
    
    return returnValue;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusUiResourceHandler::IsCurrentlyAvailable( 
                        NMusResourceApi::TAvailability aResourceAvailability,
                        MusSettingsKeys::TUsability aResourceUsability
                         )
    {
    MUS_LOG( "mus: [MUSUI ] -> CMusUiResourceHandler::IsCurrentlyAvailable()" )

    CAknAppUiBase::TAppUiOrientation orientation = iObserver.AppOrientation();
    
    MUS_LOG( "mus: [MUSUI ] <- CMusUiResourceHandler::IsCurrentlyAvailable()" )
    
    return ( aResourceAvailability == NMusResourceApi::EAvailable && 
             (
              aResourceUsability == MusSettingsKeys::EUsabilityAlways ||
              (aResourceUsability == MusSettingsKeys::EUsabilityLandscape && 
               orientation == CAknAppUiBase::EAppUiOrientationLandscape) ||
              (aResourceUsability == MusSettingsKeys::EUsabilityPortrait &&
               orientation == CAknAppUiBase::EAppUiOrientationPortrait)
             )
            );
    }






















