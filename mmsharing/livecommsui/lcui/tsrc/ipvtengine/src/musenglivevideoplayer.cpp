/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "musenglivevideoplayer.h"
#include "musengcamerahandler.h"
#include "musengdisplayhandler.h"
#include "musenglogger.h"

// SYSTEM
#include <e32base.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveVideoPlayer* CMusEngLiveVideoPlayer::NewL( 
    MMusEngDisplayHandler& aDisplayHandler,
    TMusEngCameraHandler& aCameraHandler,
    MLcAudioControl& aLcAudioControl )
    {
    return new( ELeave )CMusEngLiveVideoPlayer( 
        aDisplayHandler, aCameraHandler, aLcAudioControl );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveVideoPlayer::CMusEngLiveVideoPlayer( 
    MMusEngDisplayHandler& aDisplayHandler,
    TMusEngCameraHandler& aCameraHandler,
    MLcAudioControl& aLcAudioControl ) :
    CMusEngLocalVideoPlayer( aDisplayHandler, aLcAudioControl ),
    iCameraHandler( aCameraHandler )
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveVideoPlayer::~CMusEngLiveVideoPlayer()
    {
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
TBool CMusEngLiveVideoPlayer::LcIsPlayingL()
    {
    return iCameraHandler.IsPlayingL();
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CMusEngLiveVideoPlayer::LcPlayL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveVideoPlayer::LcPlayL" ) 
    iCameraHandler.PlayL();
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveVideoPlayer::LcPlayL" ) 
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CMusEngLiveVideoPlayer::LcPauseL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveVideoPlayer::LcPauseL" ) 
    iCameraHandler.PauseL();
    MUS_LOG( "mus: [ENGINE]  <> CMusEngLiveVideoPlayer::LcPauseL" ) 
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcCameraControl* CMusEngLiveVideoPlayer::LcCameraControl()
    {
    return &iCameraHandler;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcDestinationFileControl* CMusEngLiveVideoPlayer::LcDestinationFileControl()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcZoomControl* CMusEngLiveVideoPlayer::LcZoomControl()
    {
    return &iCameraHandler;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcBrightnessControl* CMusEngLiveVideoPlayer::LcBrightnessControl()
    {
    return &iCameraHandler;
    }

// -----------------------------------------------------------------------------
// From MLcFileControl
// -----------------------------------------------------------------------------
//
void CMusEngLiveVideoPlayer::SetLcFileNameL( const TFileName& aFileName )
    {
    iFileName = aFileName;
    }

// -----------------------------------------------------------------------------
// From MLcFileControl
// -----------------------------------------------------------------------------
//
TFileName& CMusEngLiveVideoPlayer::LcFileName()
    {
    return iFileName;
    }

// -----------------------------------------------------------------------------
// From MLcDestinationFileControl
// -----------------------------------------------------------------------------
//
void CMusEngLiveVideoPlayer::LcRecordL( TBool aRecord )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngLiveVideoPlayer::LcRecordL(( %d )", 
              aRecord )    

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveVideoPlayer::LcRecordL" ) 
    }    

// -----------------------------------------------------------------------------
// From MLcDestinationFileControl
// -----------------------------------------------------------------------------
//
TBool CMusEngLiveVideoPlayer::LcIsRecording()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveVideoPlayer::LcIsRecording()" )    
    
    TBool isEnabled( EFalse );
                 
    return isEnabled;
    }
