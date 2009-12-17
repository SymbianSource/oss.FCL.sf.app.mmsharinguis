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
#include "musengsessiondurationtimer.h"
#include "musengoutsessionobserver.h"
//#include "musengmceutils.h"
//#include "musuid.hrh"
#include "musengclipsessionobserver.h"

// SYSTEM
/*
#include <mcemanager.h>
#include <mcesession.h>
#include <mcevideostream.h>
#include <mceaudiostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <mcefilesource.h>
*/

const TInt KMusEngTimerInterval = 1000000; // 1 second
const TInt KMusEngRtcpInactivityThreshold = 20; // seconds

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceSession::~CMusEngMceSession()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::TerminateL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::ReleaseInternalObservers()
    {
    }


// -----------------------------------------------------------------------------
// Returns estabilished session time. If not established return
// value is < 0
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalSeconds CMusEngMceSession::GetSessionTime() const
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngMceSession::ConnectionActive() const
    {
    return EFalse;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::VolumeUpL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::VolumeDownL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::EnableDisplayL( TBool aEnable )
    {
    iDisplayEnabled = aEnable;
    }


// -----------------------------------------------------------------------------
// Mutes playback of sended audio streams. Audio data is still streamed.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::MuteL()
    {
    }


// -----------------------------------------------------------------------------
// Unmutes playback of sended audio streams.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::UnmuteL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceSession::CMusEngMceSession( const TRect& aRect )
    : CMusEngSession( aRect ),
      iSecondsFromLastRtcpReport ( 0 )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::RectChangedL()
    {
    }




// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsRtpcInactivityTimoutSupported()
{
    return EFalse;
}

// -----------------------------------------------------------------------------
// Initializes session timer to current time
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InitializeSessionTimer()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::UpdateTimerEvent()
    {
    }


// -----------------------------------------------------------------------------
// Enables or disables all the speaker and rtp sinks of all the audio streams
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::DoMuteL( TBool aMute )
    {
    }


// -----------------------------------------------------------------------------
// Changes volume of all speaker sinks in the session structure
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::ChangeVolumeByOneL( TBool aIncreaseVolume )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::CheckClipEndL()
    {
    }


