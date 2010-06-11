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
#include "musenglogger.h"


// SYSTEM
#include <e32base.h>

const TInt KMaxBrightness = 100;
const TInt KMinBrightness = -100;
const TInt KBrightnessStepSize = 10;


//Number of big and small zoom steps on Zoom scale
const TInt KZoomBigStepCount = 15;
const TInt KZoomSmallStepCount = KZoomBigStepCount*2;
const TInt KZoomStepMinSize = 1;

const TInt64 KZoomFasterTime = 333333; // 1/3 second


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TMusEngCameraHandler::TMusEngCameraHandler() :
    iDefaultBrightness( 0 ),
    iZoomInTime(0),
    iZoomOutTime(0),
    iSmallZoomStep( KZoomStepMinSize ),
    iBigZoomStep( KZoomStepMinSize ),
    iCurrentCamera( TMusEngCameraHandler::ECameraNotAvailable )
    {
    }

// -----------------------------------------------------------------------------
// From MLcCameraControl
// -----------------------------------------------------------------------------
//
TInt TMusEngCameraHandler::LcCameraCountL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::LcCameraCountL()" )
    
    return 2;
    }

// -----------------------------------------------------------------------------
// From MLcCameraControl
// -----------------------------------------------------------------------------
//
TInt TMusEngCameraHandler::CurrentLcCameraIndex()
    {
    return iCurrentCamera;
    }

// -----------------------------------------------------------------------------
// From MLcCameraControl
// -----------------------------------------------------------------------------
//
void TMusEngCameraHandler::ToggleLcCameraL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::ToggleLcCamera()" )
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
TInt TMusEngCameraHandler::MinLcBrightnessL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::MinLcBrightnessL()" )
 
    // minimum brightness is not in camera info, but a constant

    MUS_LOG1( "mus: [ENGINE]  <- TMusEngCameraHandler::MinLcBrightnessL(): %d",
              KMinBrightness )

    return KMinBrightness;
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
TInt TMusEngCameraHandler::MaxLcBrightnessL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::MaxLcBrightnessL()" )
    
    // maximum brightness is not in camera info, but a constant

    MUS_LOG1( "mus: [ENGINE]  <- TMusEngCameraHandler::MaxLcBrightnessL(): %d",
              KMaxBrightness )

    return KMaxBrightness;
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
TInt TMusEngCameraHandler::LcBrightnessL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::LcBrightnessL()" )
    
    return 5;
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
void TMusEngCameraHandler::SetLcBrightnessL( TInt aValue )
    {
    MUS_LOG1( "mus: [ENGINE]  -> TMusEngCameraHandler::SetLcBrightnessL( %d )",
              aValue )


    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::SetLcBrightnessL()" )
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
void TMusEngCameraHandler::IncreaseLcBrightnessL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::IncreaseLcBrightnessL()" )

    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::IncreaseLcBrightnessL()" )
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
void TMusEngCameraHandler::DecreaseLcBrightnessL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::DecreaseLcBrightnessL()" )

    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::DecreaseLcBrightnessL()" )
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
TInt TMusEngCameraHandler::MinLcZoomL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::MinLcZoomL()" )

    return 1;
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
TInt TMusEngCameraHandler::MaxLcZoomL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::MaxLcZoomL()" )

    return 10;
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
TInt TMusEngCameraHandler::LcZoomValueL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::LcZoomValueL" )


    return 5;
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
void TMusEngCameraHandler::SetLcZoomValueL( TInt aValue )
    {
    MUS_LOG1( "mus: [ENGINE]  -> TMusEngCameraHandler::SetLcZoomValueL( %d )", 
              aValue )
                  
    MUS_LOG( "mus: [ENGINE]  <- MusEngCameraHandler::SetLcZoomValueL()" )
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
void TMusEngCameraHandler::LcZoomInL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::LcZoomInL()" )
    
    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::LcZoomInL()" )
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
void TMusEngCameraHandler::LcZoomOutL()
    {
    MUS_LOG( "mus: [ENGINE]  -> TMusEngCameraHandler::LcZoomOutL()" )

    MUS_LOG( "mus: [ENGINE]  <- TMusEngCameraHandler::LcZoomOutL()" )
    }
