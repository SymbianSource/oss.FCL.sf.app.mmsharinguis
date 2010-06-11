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
#include "musengdisplayhandler.h"

// SYSTEM
#include <e32base.h>

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
    return MLcVideoPlayer::EPlaying;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
TBool CMusEngRemoteVideoPlayer::LcIsPlayingL()
    {

    return ETrue;
    }
