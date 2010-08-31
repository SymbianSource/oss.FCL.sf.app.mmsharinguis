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


// USER
#include "musengcamerahandler.h"
#include "musunittesting.h"
#include "musengmceutils.h"
#include "musenglogger.h"

// SYSTEM
#include <mcemanager.h>
#include <mcecamerasource.h>
#include <mcevideostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcefilesink.h>
#include <mcesession.h>

const TInt KMaxBrightness = 100;
const TInt KMinBrightness = -100;
const TInt KBrightnessStepSize = 10;


//Number of big and small zoom steps on Zoom scale
const TInt KZoomBigStepCount = 15;
const TInt KZoomSmallStepCount = KZoomBigStepCount*2;
const TInt KZoomStepMinSize = 1;

const TInt64 KZoomFasterTime = 333333; // 1/3 second
const TInt KOneCameraOnly = 1;


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TMusEngCameraHandler::TMusEngCameraHandler() :
    iSession( NULL ),
    iDefaultZoomFactor( -1 ),
    iDefaultBrightness( 0 ),
    iZoomInTime(0),
    iZoomOutTime(0),
    iSmallZoomStep( KZoomStepMinSize ),
    iBigZoomStep( KZoomStepMinSize ),
    iCameraUsage( MusSettingsKeys::EUseCameraSwapping )
    {
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void TMusEngCameraHandler::SetSession( CMceSession* aSession )
    {
    iSession = aSession;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void TMusEngCameraHandler::InitializeL( CMceCameraSource& aCamera )
    {
    aCamera.GetCameraInfo( iCameraInfo );
    iDefaultZoomFactor = aCamera.ZoomFactorL();
    
    InitializeZoomStepSize();
    
    iCurrentCamera = EBackCamera;
    if ( iCameraUsage == MusSettingsKeys::EUseOnlySecondaryCamera )
       {
       iCurrentCamera = EFrontCamera;
       }
    aCamera.SetCameraIndexL( (TInt)iCurrentCamera );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::PlayL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::PlayL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    if ( !camera->IsEnabled() )
        {
        camera->EnableL();
        }
    else
        {
        MUS_LOG( "mus: [ENGINE]    Camera already enabled, ignore request" )
        }
        
    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::PlayL()" )
    }


// -----------------------------------------------------------------------------
// Disable camera if not already disabled
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::PauseL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::PauseL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    if ( camera->IsEnabled() )
        {
        camera->DisableL();
        }
    else
        {
        MUS_LOG( "mus: [ENGINE]    Camera already disabled, ignore request" )
        }

    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::PauseL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool TMusEngCameraHandler::IsPlayingL()
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    return ( MusEngMceUtils::GetCameraL( *iSession )->IsEnabled() );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMusEngCameraHandler::CamerasAvailableL( 
    TCurrentCamera& aCurrentCamera )
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::CamerasAvailableL()" )
    __ASSERT_ALWAYS( iSession != NULL, User::Leave( KErrNotReady ) );
    
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );
    __ASSERT_ALWAYS( camera != NULL, User::Leave( KErrNotReady ) );
    aCurrentCamera = iCurrentCamera;
    TInt camerasAvailable = camera->CamerasAvailable();
    
    if ( camerasAvailable > KOneCameraOnly && 
         iCameraUsage != MusSettingsKeys::EUseCameraSwapping )
        {
        camerasAvailable = KOneCameraOnly;
        }
         
    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::CamerasAvailable()" )
    return camerasAvailable;  
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::ChangeCameraL( TCurrentCamera aCurrentCamera ) 
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::ChangeCameraL()" )
    __ASSERT_ALWAYS( iSession != NULL, User::Leave( KErrNotReady ) );
    
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );
    CMceDisplaySink* display = MusEngMceUtils::GetDisplayL( *iSession );
    
    if ( camera->CamerasAvailable() )
        {
        if ( TMusEngCameraHandler::EFrontCamera == aCurrentCamera &&
             TMusEngCameraHandler::EFrontCamera != iCurrentCamera )
            {
            MUS_LOG( "mus: [ENGINE]  Set Front camera" )
            iCurrentCamera = TMusEngCameraHandler::EFrontCamera;
            ChangeActiveCameraL( iCurrentCamera );
            }
        else if ( TMusEngCameraHandler::EBackCamera == aCurrentCamera && 
                  TMusEngCameraHandler::EBackCamera != iCurrentCamera )
            {
            MUS_LOG( "mus: [ENGINE]  Set Back camera " )
            iCurrentCamera = TMusEngCameraHandler::EBackCamera;
            ChangeActiveCameraL( iCurrentCamera );;
            }
        else if ( TMusEngCameraHandler::ENextCamera == aCurrentCamera )
            {
            if ( iCurrentCamera  == TMusEngCameraHandler::EBackCamera )
                {
                iCurrentCamera  = TMusEngCameraHandler::EFrontCamera;
                }
            else
                {
                iCurrentCamera  = TMusEngCameraHandler::EBackCamera;
                }
            ChangeActiveCameraL( iCurrentCamera );
            }
        }
    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::ChangeCameraL()" )
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMusEngCameraHandler::CurrentZoomL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::CurrentZoomL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    TInt currentZoom = camera->ZoomFactorL() + camera->DigitalZoomFactorL();

    MUS_LOG1( "mus: [ENGINE]  <- TMusEngCameraHandler::CurrentZoomL( %d )",
              currentZoom )

    return currentZoom;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::SetZoomL( TInt aNewZoomFactor )
    {
    MUS_LOG1( "mus: [ENGINE]  -> TMusEngCameraHandler::SetZoomL( %d )", 
              aNewZoomFactor )
              
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( aNewZoomFactor <= MaxZoomL() &&
                     aNewZoomFactor >= MinZoomL(), 
                     User::Leave( KErrArgument ) );
    
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    if ( aNewZoomFactor <= iCameraInfo.iMaxZoom )
        {
        MUS_LOG( "mus: [ENGINE]     Optical zoom factor increased" )
        camera->SetZoomFactorL( aNewZoomFactor ); 
        }
    
    if ( aNewZoomFactor - iCameraInfo.iMaxZoom > 0 )
        {
        camera->SetDigitalZoomFactorL( aNewZoomFactor - iCameraInfo.iMaxZoom );
        MUS_LOG1( "mus: [ENGINE]     Digital zoom factor increased to %d",
                  aNewZoomFactor - iCameraInfo.iMaxZoom )
        }
    
    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::SetZoomL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMusEngCameraHandler::MinZoomL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::MinZoomL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    // Although we do not need camera for return value, we have to have it in
    // order to have proper iCameraInfo
    MusEngMceUtils::GetCameraL( *iSession );

    MUS_LOG1( "mus: [ENGINE]  <- TMusEngCameraHandler::MinZoomL( %d )",
              iCameraInfo.iMinZoom )

    return iCameraInfo.iMinZoom;
    }
    
        
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMusEngCameraHandler::MaxZoomL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::MaxZoomL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    // Although we do not need camera for return value, we have to have it in
    // order to have proper iCameraInfo
    MusEngMceUtils::GetCameraL( *iSession );

    TInt maxZoom = iCameraInfo.iMaxZoom + iCameraInfo.iMaxDigitalZoom;

    MUS_LOG1( "mus: [ENGINE]     Max optical zoom( %d )",
              iCameraInfo.iMaxZoom )
    MUS_LOG1( "mus: [ENGINE]     Max digital zoom( %d )",
              iCameraInfo.iMaxDigitalZoom )
    MUS_LOG1( "mus: [ENGINE]  <- TMusEngCameraHandler::MaxZoomL( %d )",
              maxZoom )

    return maxZoom;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::ZoomInL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::ZoomInL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    // First try to increase optical zoom factor. If in maximum value, try to
    // increase digital zoom factor.
    TInt stepSize = ZoomStepSize( iZoomInTime );
    TInt zoomFactor = camera->ZoomFactorL();
    TInt zoomDigitalFactor = camera->DigitalZoomFactorL();
 
    //increate optical zoom factor
    if ( zoomFactor + stepSize <= iCameraInfo.iMaxZoom )
        {
        //optical zoom factor is enough
        camera->SetZoomFactorL( zoomFactor + stepSize );
        stepSize = 0;
        }
    else if (zoomFactor <  iCameraInfo.iMaxZoom)
       {
            stepSize -=  iCameraInfo.iMaxZoom - zoomFactor;
            camera->SetZoomFactorL( iCameraInfo.iMaxZoom );
       }

    //increate digital zoom factor
    if (stepSize > 0)
        {
        if ( zoomDigitalFactor + stepSize <= iCameraInfo.iMaxDigitalZoom )
            {
            camera->SetDigitalZoomFactorL( zoomDigitalFactor + stepSize );
            }
        else 
            {
            camera->SetDigitalZoomFactorL( iCameraInfo.iMaxDigitalZoom );
            MUS_LOG( "mus: [ENGINE]     TMusEngCameraHandler::ZoomInL(): Optical \
                             and digital zoom factors are in maximum value" )
            }
        }
    
    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::ZoomInL()" )

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::ZoomOutL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::ZoomOutL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    // First try to decrease digital zoom factor. If already zero, try to
    // decrease optical zoom factor.
    TInt stepSize = ZoomStepSize( iZoomOutTime );
    TInt zoomFactor = camera->ZoomFactorL();
    TInt zoomDigitalFactor = camera->DigitalZoomFactorL();
    
    //decreate digital zoom factor firstly
    if ( zoomDigitalFactor - stepSize >= 0  )
        {
        //digital zoom factor is enough
        camera->SetDigitalZoomFactorL( zoomDigitalFactor - stepSize );
        stepSize = 0;
        }
    else if ( zoomDigitalFactor > 0 )
        {
        stepSize -= zoomDigitalFactor;
        camera->SetDigitalZoomFactorL( 0 );
        MUS_LOG("mus: [ENGINE] Digigal Factor to zero")
        }
   
    //decreate optical zoom factor firstly
    if ( stepSize > 0 )
        {
        if ( zoomFactor - stepSize > iCameraInfo.iMinZoom )
            {
            camera->SetZoomFactorL( zoomFactor - stepSize );
            }
        else 
            {
            MUS_LOG( "mus: [ENGINE]    TMusEngCameraHandler::ZoomOutL(): Optical and \
                        digital zoom factors are in minimum value" )
            camera->SetZoomFactorL( iCameraInfo.iMinZoom );
            }
        }

    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::ZoomOutL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::ZoomDefaultL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::ZoomDefaultL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    camera->SetZoomFactorL( iDefaultZoomFactor );
    camera->SetDigitalZoomFactorL( 0 );

    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::ZoomDefaultL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::SetBrightnessL( TInt aBrightness ) const
    {
    MUS_LOG1( "mus: [ENGINE]  -> TMusEngCameraHandler::SetBrightnessL( %d )",
              aBrightness )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );
    camera->SetBrightnessL( aBrightness );

    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::SetBrightnessL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMusEngCameraHandler::CurrentBrightnessL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::CurrentBrightnessL()" )
    
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );
    TInt currentBrightness = camera->BrightnessL();

    MUS_LOG1( "mus: [ENGINE]  <- TMusEngCameraHandler::CurrentBrightnessL(): %d",
              currentBrightness )

    return currentBrightness;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMusEngCameraHandler::MaxBrightnessL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::MaxBrightnessL()" )
    
    // maximum brightness is not in camera info, but a constant 100

    MUS_LOG1( "mus: [ENGINE]  <- TMusEngCameraHandler::MaxBrightnessL(): %d",
              KMaxBrightness )

    return KMaxBrightness;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMusEngCameraHandler::MinBrightnessL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::MinBrightnessL()" )
 
    // maximum brightness is not in camera info, but a constant -100

    MUS_LOG1( "mus: [ENGINE]  <- TMusEngCameraHandler::MinBrightnessL(): %d",
              KMinBrightness )

    return KMinBrightness;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::IncreaseBrightnessL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::IncreaseBrightnessL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    TInt newBrightness = camera->BrightnessL() + KBrightnessStepSize;
    
    if ( newBrightness < KMaxBrightness )
        {
        camera->SetBrightnessL( newBrightness );
        }
    else
        {
        camera->SetBrightnessL( KMaxBrightness );
        }

    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::IncreaseBrightnessL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::DecreaseBrightnessL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::DecreaseBrightnessL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    TInt newBrightness = camera->BrightnessL() - KBrightnessStepSize;

    if ( newBrightness > KMinBrightness )
        {
        camera->SetBrightnessL( newBrightness );
        }
    else
        {
        camera->SetBrightnessL( KMinBrightness );
        }

    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::DecreaseBrightnessL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::BrightnessDefaultL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::BrightnessDefaultL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    camera->SetBrightnessL( iDefaultBrightness );

    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::BrightnessDefaultL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void TMusEngCameraHandler::BrightnessAutoL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::BrightnessAutoL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    camera->SetBrightnessL( CCamera::EBrightnessAuto );

    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::BrightnessAutoL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void TMusEngCameraHandler::InitializeZoomStepSize()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::InitializeZoomStepSize()" )
    MUS_LOG2( "iMaxZoom = %d, iMinZoom = %d", iCameraInfo.iMaxZoom, iCameraInfo.iMinZoom);
    MUS_LOG2( "iMaxDigitalZoom = %d, iMaxDigitalZoomFactor = %d", 
            iCameraInfo.iMaxDigitalZoom, iCameraInfo.iMaxDigitalZoomFactor);

    iBigZoomStep = ( iCameraInfo.iMaxZoom + iCameraInfo.iMaxDigitalZoom - 
            iCameraInfo.iMinZoom ) / KZoomBigStepCount;
            
    if ( iBigZoomStep < KZoomStepMinSize )
        iBigZoomStep = KZoomStepMinSize;

    iSmallZoomStep = ( iCameraInfo.iMaxZoom + iCameraInfo.iMaxDigitalZoom - 
            iCameraInfo.iMinZoom ) / KZoomSmallStepCount;
    
    if ( iSmallZoomStep < KZoomStepMinSize )
        iSmallZoomStep = KZoomStepMinSize;

    MUS_LOG2( "mus: [ENGINE]  iSmallZoomStep = %d, iBigZoomStep = %d", 
            iSmallZoomStep, iBigZoomStep )
    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::InitializeZoomStepSize()" )   
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void TMusEngCameraHandler::ReadCameraUsageKeyL()
    {
    MUS_LOG( "mus: [ENGINE] -> TMusEngCameraHandler::ReadCameraUsageKeyL()" )

    // We try to read camera usage. If we do not succeed, we use default
    iCameraUsage = MusSettingsKeys::EUseCameraSwapping;
    TRAPD( error, 
            iCameraUsage = MultimediaSharingSettings::CameraUsageL() )
    __ASSERT_ALWAYS( error == KErrNone || error == KErrNotFound, 
                     User::Leave( error ) );
    
    MUS_LOG( "mus: [ENGINE] <- TMusEngCameraHandler::ReadCameraUsageKeyL()" )
    
    }

// -----------------------------------------------------------------------------
//  Calculate the zoom step based time escaped
// -----------------------------------------------------------------------------
TInt TMusEngCameraHandler::ZoomStepSize( TInt64& aTime )
    {
    TTime now;
    now.HomeTime();
    TInt64 timeNow = now.Int64();   
    TInt stepSize = ( ( timeNow - aTime ) <= KZoomFasterTime ) ? iBigZoomStep : iSmallZoomStep;
    
    aTime = now.Int64();
    
    MUS_LOG1( "mus: [ENGINE]  stepSize = %d", stepSize );    

    return stepSize;
    }

// -----------------------------------------------------------------------------
//  Disable camera before it changes it. It enables camera after change. 
// -----------------------------------------------------------------------------
void TMusEngCameraHandler::ChangeActiveCameraL( TCurrentCamera aNewCamera )
    {
    MUS_LOG( "mus: [ENGINE] ->  ChangeActiveCamera()" )
    
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );
    CMceDisplaySink* display = MusEngMceUtils::GetDisplayL( *iSession );
    MUS_LOG( "mus: [ENGINE]  Camera enabled ?" )
    if ( camera->IsEnabled() )
        {
        MUS_LOG( "mus: [ENGINE]  Is view finder enabled? " )
            if ( display->IsEnabled() )
                {
                MUS_LOG( "mus: [ENGINE]  Disable view finder" )
                display->DisableL();
                }
        MUS_LOG( "mus: [ENGINE]  Disable camera" )
        camera->DisableL();
        camera->SetCameraIndexL( aNewCamera );
        camera->EnableL();
        display->EnableL();
        }
    else
        {
        if ( display->IsEnabled() )
            {
            MUS_LOG( "mus: [ENGINE]  Disable view finder" )
            display->DisableL();
            }
        MUS_LOG( "mus: [ENGINE]  set camera index" )
        camera->SetCameraIndexL( aNewCamera );
        MUS_LOG( "mus: [ENGINE]  Enable camera" ) 
        display->EnableL();
        }
    
    MUS_LOG( "mus: [ENGINE]  -> fetching new iCameraInfo..." )
    camera->GetCameraInfo( iCameraInfo );
    iDefaultZoomFactor = camera->ZoomFactorL();        
    InitializeZoomStepSize();
    }

