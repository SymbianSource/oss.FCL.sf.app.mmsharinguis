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
#include "musengmceutils.h"
#include "musengdisplayhandler.h"

// SYSTEM
#include <mcesession.h>
#include <mcefilesink.h>

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
void CMusEngLiveVideoPlayer::LcPlayL( TLcVideoPlayerSource /*aSource*/ )
    {
    iCameraHandler.PlayL();
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CMusEngLiveVideoPlayer::LcPauseL()
    {
    iCameraHandler.PauseL();
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
    
    __ASSERT_ALWAYS( iMceSession, User::Leave( KErrNotReady ) );

    CMceMediaSink* fileSink = 
        MusEngMceUtils::GetMediaSink( *iMceSession, KMceFileSink );
    
    __ASSERT_ALWAYS( fileSink, User::Leave( KErrNotReady ) );
    
    if ( aRecord )
        {
        if ( !fileSink->IsEnabled() )
            {
            fileSink->EnableL();
            }
        }
    else
        {
        if ( fileSink->IsEnabled() )
            {
            fileSink->DisableL();
            }
        }

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
  
    CMceMediaSink* fileSink = 
        MusEngMceUtils::GetMediaSink( *iMceSession, KMceFileSink );
    if ( fileSink )
        {
        isEnabled = fileSink->IsEnabled();
        }
        
    MUS_LOG1( "mus: [ENGINE]  <- CMusEngLiveVideoPlayer::LcIsRecording( %d )",
              isEnabled )
                 
    return isEnabled;
    }
