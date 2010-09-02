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
#include "lcenginestub.h"
#include "lcvideoplayerstub.h"
#include "lcvideoplayer.h"
#include "lcaudiocontrol.h"
#include "lccameracontrol.h"
#include "lczoomcontrol.h"
#include "lcbrightnesscontrol.h"
#include "lcsessionstub.h"
#include "lcuiprovider.h"

#include <QStringList>

static QStringList lcutStub_LcPlayer_calledMethods;
static bool lcutStub_LcPlayer_asyncCompletionEnabled = false;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcVideoPlayer* CLcVideoPlayer::NewL()
    {
    CLcVideoPlayer* self = new( ELeave )CLcVideoPlayer();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    } 
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcVideoPlayer::CLcVideoPlayer()
    : iRect( TRect(10,10,50,100))
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::ConstructL()
    {
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CLcVideoPlayer::~CLcVideoPlayer()
    {
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcVideoPlayer::TLcVideoPlayerState CLcVideoPlayer::LcVideoPlayerState() const
    {
    return iState;
    }
// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
TBool CLcVideoPlayer::LcIsPlayingL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    return iPlaying;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::LcPlayL(TLcVideoPlayerSource /*aSource*/)
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    iPlaying = ETrue;
    iState = MLcVideoPlayer::EPlaying;
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_LcPlayL );
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::LcPauseL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    iPlaying = EFalse;
    iState = MLcVideoPlayer::EPaused;
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_LcPauseL );
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcWindow* CLcVideoPlayer::LcWindow()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
// 
MLcCameraControl* CLcVideoPlayer::LcCameraControl()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcSourceFileControl* CLcVideoPlayer::LcSourceFileControl()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcDestinationFileControl* CLcVideoPlayer::LcDestinationFileControl()
    {
    return NULL;
    }
// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcAudioControl* CLcVideoPlayer::LcAudioControl()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcZoomControl* CLcVideoPlayer::LcZoomControl()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcBrightnessControl* CLcVideoPlayer::LcBrightnessControl()
    {
    return this;
    }

//-----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::EnableLcWindowL( TBool aEnable )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    iWindowEnabled = aEnable;
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_EnableLcWindowL );
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
TBool CLcVideoPlayer::IsLcWindowEnabled()
    {
    return iWindowEnabled;
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::SetLcWindowRectL( TRect aRect )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    iRect = aRect;
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_SetLcWindowRectL );
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
TRect CLcVideoPlayer::LcWindowRect()
    {
    return iRect;
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::SetLcWindowOrientationL( 
    TLcWindowOrientation aOrientation )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    iOrientation = aOrientation;
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_SetLcWindowOrientationL );
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
MLcWindow::TLcWindowOrientation CLcVideoPlayer::LcWindowOrientationL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    MLcWindow::TLcWindowOrientation orientation;
    if ( iOrientation == ELandscape )
        {
        orientation = MLcWindow::ELandscape;
        }
    else
        {
        orientation = MLcWindow::EPortrait;
        }
    return orientation;
    }
// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CLcVideoPlayer::IsLcAudioMutedL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    return iMuted;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::MuteLcAudioL( TBool aMute )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_MuteLcAudioL );
    iMuted = aMute;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CLcVideoPlayer::IsEnablingLcLoudspeakerAllowed()
    {
    return iEnablingLoudspeakerAllowed;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CLcVideoPlayer::IsLcMicMutedL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    return iMicMuted;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::MuteLcMicL( TBool aMute )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_MuteLcMicL );
    iMicMuted = aMute;
    
    if ( lcutStub_LcPlayer_asyncCompletionEnabled && 
         lcutStub_LcSession_lcUiProvider() )
        {
        lcutStub_LcSession_lcUiProvider()->BlockUi( ETrue );
        }
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CLcVideoPlayer::IsLcLoudspeakerEnabled()
    {
    return iLoudspeakerEnabled;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::EnableLcLoudspeakerL( TBool aEnabled )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_EnableLcLoudspeakerL );
    iLoudspeakerEnabled = aEnabled;
    
    if ( lcutStub_LcPlayer_asyncCompletionEnabled && 
         lcutStub_LcSession_lcUiProvider() )
        {
        lcutStub_LcSession_lcUiProvider()->BlockUi( ETrue );
        }
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TInt CLcVideoPlayer::LcVolumeL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    return iVolume;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::SetLcVolumeL( TInt aValue )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    iVolume = aValue;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::IncreaseLcVolumeL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    iVolume++;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CLcVideoPlayer::DecreaseLcVolumeL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    iVolume--;
    }

TInt CLcVideoPlayer::LcCameraCountL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    return 2;
    }

TInt CLcVideoPlayer::CurrentLcCameraIndex()
    {
    return 0;
    }

void CLcVideoPlayer::ToggleLcCameraL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_ToggleLcCameraL );
    }

TInt CLcVideoPlayer::MinLcZoomL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    return 0;
    }

TInt CLcVideoPlayer::MaxLcZoomL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    return 10;
    }

TInt CLcVideoPlayer::LcZoomValueL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    return 0;
    }

void CLcVideoPlayer::SetLcZoomValueL( TInt /*aValue*/ )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_SetLcZoomValueL );
    }

void CLcVideoPlayer::LcZoomInL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_LcZoomInL );
    }

void CLcVideoPlayer::LcZoomOutL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_LcZoomOutL );
    }

TInt CLcVideoPlayer::MinLcBrightnessL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    return 0;
    }

TInt CLcVideoPlayer::MaxLcBrightnessL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    return 20; 
    }

TInt CLcVideoPlayer::LcBrightnessL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    return 10; 
    }

void CLcVideoPlayer::SetLcBrightnessL( TInt /*aValue*/ )
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_SetLcBrightnessL );
    }

void CLcVideoPlayer::IncreaseLcBrightnessL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_IncreaseLcBrightnessL );
    }

void CLcVideoPlayer::DecreaseLcBrightnessL()
    {
    User::LeaveIfError( lcutStub_LcEngine_failure() );
    lcutStub_LcPlayer_calledMethods.insert(0,lcutStub_LcPlayer_DecreaseLcBrightnessL );
    }


//stub control

bool lcutStub_LcPlayer_expectCall( const QString& call, int callNdx )
{
    if ( callNdx >= lcutStub_LcPlayer_calledMethods.count() ) {
        return call == lcutStub_LcPlayer_NoCall;
    }
    return lcutStub_LcPlayer_calledMethods.count() > 0 ?
           lcutStub_LcPlayer_calledMethods[callNdx] == call :
           call == lcutStub_LcPlayer_NoCall;
}

    
void lcutStub_LcPlayer_reset()
{
    lcutStub_LcPlayer_calledMethods.clear();
    lcutStub_LcPlayer_asyncCompletion( false );
}

void lcutStub_LcPlayer_asyncCompletion( bool async )
{
    lcutStub_LcPlayer_asyncCompletionEnabled = async;
}

// End of File
