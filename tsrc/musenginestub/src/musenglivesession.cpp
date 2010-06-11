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
#include "musenglivesession.h"
#include "musengsessionobserver.h"
#include "musenglivesessionobserver.h"
#include "musunittesting.h"
#include "musengmceutils.h"
#include "musenglogger.h"
#include "mussessionproperties.h"

// SYSTEM
#include <mcemanager.h>
#include <mcecamerasource.h>
#include <mcevideostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcefilesink.h>
#include <mcesession.h>
#include <mcevideocodec.h>
#include <mceh263codec.h>
#include <mceavccodec.h>


// Names of AVC levels in string for config keys stored in CenRep 
_LIT8( KMusAvcBitrateLevel1, "AvcBrL1=" );
_LIT8( KMusAvcBitrateLevel1b, "AvcBrL1b=" );
_LIT8( KMusAvcBitrateLevel1_1, "AvcBrL1_1=" );
_LIT8( KMusAvcBitrateLevel1_2, "AvcBrL1_2=" );
_LIT8( KMusAvcBitrateLevel1_3, "AvcBrL1_3=" );
_LIT8( KMusAvcBitrateLevel2, "AvcBrL2=" );

_LIT8( KMusEncoderInfoTokenizer, ";" );


const TInt KMaxDispName = 512;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession* CMusEngLiveSession::NewL(
                        const TDesC& aFileName,
                        const TRect& aRect )
    {
    CMusEngLiveSession* self = new( ELeave ) CMusEngLiveSession( aRect,
                                                                 aFileName );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession* CMusEngLiveSession::NewL(
                        const TRect& aRect )
    {
    CMusEngLiveSession* self = new( ELeave ) CMusEngLiveSession( aRect );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession::~CMusEngLiveSession()
    {
    delete iRemoteDisplayName;
    }



// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::EstablishLcSessionL()
    {
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngLiveSession::LocalVideoPlayer()
    {
    return this;
    }
    
 
    
// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
 
const TDesC& CMusEngLiveSession::RemoteDisplayName()
 	{
    return *iRemoteDisplayName;
 	}




// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcVideoPlayer::TLcVideoPlayerState CMusEngLiveSession::LcVideoPlayerState() const
    {
    TLcVideoPlayerState vidPlayerState = MLcVideoPlayer::EUnavailable;
    return vidPlayerState;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
TBool CMusEngLiveSession::LcIsPlayingL()
    {
    return iPlaying;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::LcPlayL()
    {
    iPlaying = ETrue;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::LcPauseL()
    {
    iPlaying = EFalse;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcWindow* CMusEngLiveSession::LcWindow()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcCameraControl* CMusEngLiveSession::LcCameraControl()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcFileControl* CMusEngLiveSession::LcSourceFile()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcFileControl* CMusEngLiveSession::LcDestinationFile()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcAudioControl* CMusEngLiveSession::LcAudioControl()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcZoomControl* CMusEngLiveSession::LcZoomControl()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
MLcBrightnessControl* CMusEngLiveSession::LcBrightnessControl()
    {
    return this;
    }

// -----------------------------------------------------------------------------
// From MLcVideoPlayer
// -----------------------------------------------------------------------------
//
RPointerArray< MLcValueControl >& CMusEngLiveSession::LcExtensionControls()
    {
    return iValueControls;
    }

// -----------------------------------------------------------------------------
// From MLcCameraControl
// -----------------------------------------------------------------------------
//
TInt CMusEngLiveSession::LcCameraCountL()
    {
    return 1;
    }

// -----------------------------------------------------------------------------
// From MLcCameraControl
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::ToggleLcCameraL()
    {
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
TInt CMusEngLiveSession::MinLcBrightnessL()
    {
    return iCurrentBrighness;
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
TInt CMusEngLiveSession::MaxLcBrightnessL()
    {
    return iCurrentBrighness;
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
TInt CMusEngLiveSession::LcBrightnessL()
    {
    return iCurrentBrighness;
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::SetLcBrightnessL( TInt aValue )
    {
    iCurrentBrighness = aValue;
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::IncreaseLcBrightnessL()
    {
    iCurrentBrighness++;
    }

// -----------------------------------------------------------------------------
// From MLcBrightnessControl
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::DecreaseLcBrightnessL()
    {
    iCurrentBrighness--;
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
TInt CMusEngLiveSession::MinLcZoomL()
    {
    return iCurrentZoom;
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
TInt CMusEngLiveSession::MaxLcZoomL()
    {
    return iCurrentZoom;
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
TInt CMusEngLiveSession::LcZoomValueL()
    {
    return iCurrentZoom;
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::SetLcZoomValueL( TInt aValue )
    {
    iCurrentZoom = aValue;
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::LcZoomInL()
    {
    iCurrentZoom++;
    }

// -----------------------------------------------------------------------------
// From MLcZoomControl
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::LcZoomOutL()
    {
    iCurrentZoom--;
    }

// -----------------------------------------------------------------------------
// From MLcFileControl
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::EnableLcFileL( TBool aEnable )
    {
    iFileEnabled = aEnable;
    }

// -----------------------------------------------------------------------------
// From MLcFileControl
// -----------------------------------------------------------------------------
//
TBool CMusEngLiveSession::IsLcFileEnabled()
    {
    return iFileEnabled;
    }

// -----------------------------------------------------------------------------
// From MLcFileControl
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::SetLcFileNameL( const TFileName& aFileName )
    {
    iRecordedFile = aFileName;
    }

// -----------------------------------------------------------------------------
// From MLcFileControl
// -----------------------------------------------------------------------------
//
TFileName& CMusEngLiveSession::LcFileName()
    {
    return iRecordedFile;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession::CMusEngLiveSession(
                        const TRect& aRect, 
                        const TDesC& aRecordedFile )
    : CMusEngMceOutSession( aRect ),
      iRecordedFile( aRecordedFile )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::ConstructL()" )
    
    CMusEngMceOutSession::ConstructL();
        
    iRemoteDisplayName = HBufC::NewL( KMaxDispName );

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::ConstructL()" )
    }
