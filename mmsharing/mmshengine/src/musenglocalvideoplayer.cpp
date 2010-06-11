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
#include "musenglocalvideoplayer.h"
#include "musengmceutils.h"
#include "musengdisplayhandler.h"

// SYSTEM
#include <mcesession.h>
#include <mcevideostream.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLocalVideoPlayer::CMusEngLocalVideoPlayer( 
    MMusEngDisplayHandler& aDisplayHandler,
    MLcAudioControl& aLcAudioControl ) :
    CMusEngVideoPlayerBase( aDisplayHandler, aLcAudioControl )
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLocalVideoPlayer::~CMusEngLocalVideoPlayer()
    {
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcVideoPlayer::TLcVideoPlayerState 
CMusEngLocalVideoPlayer::LcVideoPlayerState() const
    {
    if ( !iMceSession )
        {
        return MLcVideoPlayer::EUnavailable;
        }
    CMceVideoStream* stream = NULL;
    TRAPD( err, stream = MusEngMceUtils::GetVideoOutStreamL( *iMceSession ) )
    TLcVideoPlayerState playerState = MLcVideoPlayer::EUnavailable;
    if ( err == KErrNone && stream )
          {
          switch ( stream->State() )
              {
              case CMceMediaStream::EInitialized:
                  {
                  playerState = MLcVideoPlayer::EInit; 
                  break;
                  }
              case CMceMediaStream::EBuffering:
                  {
                  playerState = MLcVideoPlayer::EBuffering; 
                  break;
                  }
              case CMceMediaStream::EIdle:
              case CMceMediaStream::EDisabled:
                  {
                  playerState = MLcVideoPlayer::EPaused; 
                  break;
                  }
              case CMceMediaStream::EStreaming:
                  {
                  playerState = MLcVideoPlayer::EPlaying; 
                  break;
                  }
              default: // Other MCE stream states: MLcVideoPlayer::EUnavailable
                 {
                 break;
                 }
              }
          }
    return playerState;
    }


// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
void CMusEngLocalVideoPlayer::SetLcWindowRectL( TRect aRect )
    {
    iDisplayHandler.SetSecondaryRectL( aRect );
    }


// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
TRect CMusEngLocalVideoPlayer::LcWindowRect()
    {
    return iDisplayHandler.SecondaryRect();
    }
