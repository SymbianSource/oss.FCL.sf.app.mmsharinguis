/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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



// USER INCLUDES

#include "musengreceivesession.h"
#include "musengsessionobserver.h"
#include "musengreceivesessionobserver.h"
#include "musengmceutils.h"
#include "musenglogger.h"
#include "mussipprofilehandler.h"
#include "mussessionproperties.h"

// SYSTEM INCLUDES
#include <mcemanager.h>
#include <mceinsession.h>
#include <mcestreambundle.h>
#include <mceaudiostream.h>
#include <mcevideostream.h>
#include <mcemediasource.h>
#include <mcertpsource.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <mceaudiocodec.h>
#include <mceavccodec.h>
#include <e32property.h>

#include <utf.h>


const TUint8 KMusEngRtpKeepAliveTimer = 5; // this should be 30 sec, this a temporary fix
const TUint8 KMusEngKeepAlivePayloadTypeVideoH263 = 96;
const TUint8 KMusEngKeepAlivePayloadTypeAudio = 97;
const TUint8 KMusEngKeepAlivePayloadTypeVideoAvc = 98;

const TInt KMusEngJitterBufferLength = 51; //Must be bigger than treshold
// Using following value increases treshold buffer to 1 second from 
// default 100 ms
const TInt KMusEngJitterBufferTreshold = 50;
const TInt KMusEngTresholdToSecondsFactor = 20;
const TInt KMusEngOneSecondInMilliSeconds = 1000; 
// Use inactivity timer value that is a little bigger than treshold in seconds
const TUint KMusEngInactivityTimer = KMusEngTresholdToSecondsFactor * 
                                     KMusEngJitterBufferTreshold + 
                                     KMusEngOneSecondInMilliSeconds;

_LIT8( KMusEngSwisIdentifier, "Require: precondition" );
_LIT8( KMusEngAssertedIdentity, "P-Asserted-Identity" );
_LIT8( KMusEngBiggerSign, ">" );

const TInt KMusEngSipReasonCodeBusyHere = 486;
_LIT8( KMusEngSipReasonPhraseBusy, "Busy" );

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngReceiveSession::~CMusEngReceiveSession()
    {
    delete iRemoteDisplayName;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngReceiveSession* CMusEngReceiveSession::NewL( const TRect& aRect )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::NewL(...)" )

    CMusEngReceiveSession* self = new( ELeave ) CMusEngReceiveSession( aRect );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::NewL(...)" )
    return self;
    }


// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::EstablishLcSessionL()
    {
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngReceiveSession::RemoteVideoPlayer()
    {
    return this;
    }
    
// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
 
const TDesC& CMusEngReceiveSession::RemoteDisplayName()
 	{
    return *iRemoteDisplayName;
 	}
 	

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcVideoPlayer::TLcVideoPlayerState CMusEngReceiveSession::LcVideoPlayerState() const
    {
    TLcVideoPlayerState vidPlayerState = MLcVideoPlayer::EUnavailable;

    return vidPlayerState;
    }
// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
TBool CMusEngReceiveSession::LcIsPlayingL()
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::LcPlayL()
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::LcPauseL()
    {
    // NOP
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcWindow* CMusEngReceiveSession::LcWindow()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcCameraControl* CMusEngReceiveSession::LcCameraControl()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcFileControl* CMusEngReceiveSession::LcSourceFile()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcFileControl* CMusEngReceiveSession::LcDestinationFile()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcAudioControl* CMusEngReceiveSession::LcAudioControl()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcZoomControl* CMusEngReceiveSession::LcZoomControl()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcBrightnessControl* CMusEngReceiveSession::LcBrightnessControl()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
RPointerArray< MLcValueControl >& CMusEngReceiveSession::LcExtensionControls()
    {
    return iValueControls;
    }

// End of File
