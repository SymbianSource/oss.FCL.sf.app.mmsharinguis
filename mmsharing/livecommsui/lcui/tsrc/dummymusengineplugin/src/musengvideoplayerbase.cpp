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
#include "musengcamerahandler.h"
#include "musengdisplayhandler.h"

// SYSTEM
#include <e32base.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngVideoPlayerBase::CMusEngVideoPlayerBase( 
    MMusEngDisplayHandler& aDisplayHandler,
    MLcAudioControl& aLcAudioControl ) :
    iDisplayHandler( aDisplayHandler ),
    iLcAudioControl( aLcAudioControl )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngVideoPlayerBase::~CMusEngVideoPlayerBase()
    {
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CMusEngVideoPlayerBase::LcPlayL(MLcVideoPlayer::TLcVideoPlayerSource /*aSource*/)
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CMusEngVideoPlayerBase::LcPauseL()
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcWindow* CMusEngVideoPlayerBase::LcWindow()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcCameraControl* CMusEngVideoPlayerBase::LcCameraControl()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcSourceFileControl* CMusEngVideoPlayerBase::LcSourceFileControl()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcDestinationFileControl* CMusEngVideoPlayerBase::LcDestinationFileControl()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcAudioControl* CMusEngVideoPlayerBase::LcAudioControl()
    {
    return &iLcAudioControl;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcZoomControl* CMusEngVideoPlayerBase::LcZoomControl()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcBrightnessControl* CMusEngVideoPlayerBase::LcBrightnessControl()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
void CMusEngVideoPlayerBase::EnableLcWindowL( TBool aEnable )
    {
    iDisplayHandler.EnableDisplayL( aEnable );
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
TBool CMusEngVideoPlayerBase::IsLcWindowEnabled()
    {
    return iDisplayHandler.IsDisplayEnabled();
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
void CMusEngVideoPlayerBase::SetLcWindowRectL( TRect aRect )
    {
    iDisplayHandler.SetRectL( aRect );
    }


// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
TRect CMusEngVideoPlayerBase::LcWindowRect()
    {
    return iDisplayHandler.Rect();
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
void CMusEngVideoPlayerBase::SetLcWindowOrientationL( 
    TLcWindowOrientation aOrientation )
    {
    if ( aOrientation == MLcWindow::ELandscape )
        {
        iDisplayHandler.SetOrientationL( MMusEngDisplayHandler::ELandscape );
        }
    else 
        {
        iDisplayHandler.SetOrientationL( MMusEngDisplayHandler::EPortrait );
        }
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
MLcWindow::TLcWindowOrientation CMusEngVideoPlayerBase::LcWindowOrientationL()
    {
    MLcWindow::TLcWindowOrientation orientation;
    if ( iDisplayHandler.OrientationL() == MMusEngDisplayHandler::ELandscape )
        {
        orientation = MLcWindow::ELandscape;
        }
    else
        {
        orientation = MLcWindow::EPortrait;
        }
    return orientation;
    }
