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
#include "musengmcesession.h"


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceSession::CMusEngMceSession( const TRect& aRect )
    : iRect ( aRect )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::ConstructL()
    {
    CMusEngSession::ConstructL(); // Base class ConstructL -first
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceSession::~CMusEngMceSession()
    {
    }

// -----------------------------------------------------------------------------
// From MLcSession 
// -----------------------------------------------------------------------------
//

MLcSession::TLcSessionState CMusEngMceSession::LcSessionState() const
    {
    TLcSessionState lcSessionState = MLcSession::EUninitialized;
    return lcSessionState;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::EstablishLcSessionL()
    {
    User::Leave( KErrNotSupported );
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::TerminateLcSessionL()
    {
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngMceSession::RemoteVideoPlayer()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngMceSession::LocalVideoPlayer()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
const TDesC& CMusEngMceSession::LocalDisplayName()
    {
    return KNullDesC;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
const TDesC& CMusEngMceSession::RemoteDisplayName()
    {
    return KNullDesC;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TInt CMusEngMceSession::SetParameter( TInt /*aId*/, TInt /*aValue*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TInt CMusEngMceSession::ParameterValue( TInt /*aId*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::EnableLcWindowL( TBool aEnable )
    {
    iWindowEnabled = aEnable;
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsLcWindowEnabled()
    {
    return iWindowEnabled;
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetLcWindowRectL( TRect aRect )
    {
    iRect = aRect;
    }


// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
TRect CMusEngMceSession::LcWindowRect()
    {
    return iRect;
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetLcWindowOrientationL( 
    TLcWindowOrientation aOrientation )
    {
    iOrientation = aOrientation;
    }

// -----------------------------------------------------------------------------
// From MLcWindow
// -----------------------------------------------------------------------------
//
MLcWindow::TLcWindowOrientation CMusEngMceSession::LcWindowOrientationL()
    {
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
TBool CMusEngMceSession::IsLcAudioMutedL()
    {
    return iMuted;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::MuteLcAudioL( TBool aMute )
    {
    iMuted = aMute;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsEnablingLcLoudspeakerAllowed()
    {
    return iEnablingLoudspeakerAllowed;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsLcMicMutedL()
    {
    return iMicMuted;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::MuteLcMicL( TBool aMute )
    {
    iMicMuted = aMute;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsLcLoudspeakerEnabled()
    {
    return iLoudspeakerEnabled;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::EnableLcLoudspeakerL( TBool aEnabled )
    {
    iLoudspeakerEnabled = aEnabled;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TInt CMusEngMceSession::LcVolumeL()
    {
    return iVolume;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetLcVolumeL( TInt aValue )
    {
    iVolume = aValue;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::IncreaseLcVolumeL()
    {
    iVolume++;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::DecreaseLcVolumeL()
    {
    iVolume--;
    }

void CMusEngMceSession::UpdateLcSessionL()
    {
    }



// End of file
