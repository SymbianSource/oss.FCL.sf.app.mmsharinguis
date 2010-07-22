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
#include "musengremotevideoplayer.h"
#include "musengmceutils.h"
#include "musengdisplayhandler.h"

// SYSTEM
#include <mcesession.h>
#include <mcevideostream.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngRemoteVideoPlayer* CMusEngRemoteVideoPlayer::NewL(
    MMusEngDisplayHandler& aDisplayHandler,
    MLcAudioControl& aLcAudioControl )
    {
    return new( ELeave )CMusEngRemoteVideoPlayer( 
        aDisplayHandler, aLcAudioControl );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngRemoteVideoPlayer::CMusEngRemoteVideoPlayer( 
    MMusEngDisplayHandler& aDisplayHandler,
    MLcAudioControl& aLcAudioControl ) :
    CMusEngVideoPlayerBase( aDisplayHandler, aLcAudioControl )
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngRemoteVideoPlayer::~CMusEngRemoteVideoPlayer()
    {
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcVideoPlayer::TLcVideoPlayerState 
CMusEngRemoteVideoPlayer::LcVideoPlayerState() const
    {
    if ( !iMceSession )
        {
        return MLcVideoPlayer::EUnavailable;
        }
    CMceVideoStream* stream = NULL;
    TRAPD( err, stream = MusEngMceUtils::GetVideoInStreamL( *iMceSession ) )
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
                  // MCE stream state does not contain RTP activity information
                  playerState = iDisplayHandler.IsDisplayActive() ?
                      MLcVideoPlayer::EPlaying : MLcVideoPlayer::EPaused;
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
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
TBool CMusEngRemoteVideoPlayer::LcIsPlayingL()
    {
    if ( !iMceSession )
        {
        return EFalse;
        }    
    CMceMediaStream* stream = MusEngMceUtils::GetVideoInStreamL( *iMceSession );
    TBool isPlaying( EFalse );
    
    if ( iMceSession->State() == CMceSession::EEstablished &&
         stream && 
         stream->State() == CMceMediaStream::EStreaming )
        {
        isPlaying = ETrue;
        }

    return isPlaying;
    }
