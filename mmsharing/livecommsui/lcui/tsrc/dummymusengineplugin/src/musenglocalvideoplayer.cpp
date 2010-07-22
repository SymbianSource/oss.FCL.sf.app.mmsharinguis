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
#include "musengdisplayhandler.h"


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
    return MLcVideoPlayer::EPlaying;
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
