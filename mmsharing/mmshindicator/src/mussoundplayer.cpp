/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provides means to play custom indication sound.
*
*/



#include "mussoundplayer.h"
#include "muslogger.h"
#include <NokiaAudioPreference.h>


// Name of availability sound. Path used is private path of process.
_LIT( KMusAvailabilitySound, "tone.amr" );


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusSoundPlayer* CMusSoundPlayer::NewL( MMusSoundPlayerObserver& aObserver )
    {
    MUS_LOG( "mus: [MUSIND]  -> CMusSoundPlayer::NewL" )
    CMusSoundPlayer* self = NewLC( aObserver );
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSIND]  <- CMusSoundPlayer::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusSoundPlayer* CMusSoundPlayer::NewLC( MMusSoundPlayerObserver& aObserver )
    {
    MUS_LOG( "mus: [MUSIND]  -> CMusSoundPlayer::NewLC" )
    CMusSoundPlayer* self = new (ELeave) CMusSoundPlayer( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    MUS_LOG( "mus: [MUSIND]  <- CMusSoundPlayer::NewLC" )
    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusSoundPlayer::~CMusSoundPlayer()
    {
    MUS_LOG( "mus: [MUSIND]  -> CMusSoundPlayer::CMusSoundPlayer" )
    if ( iMdaPlayer )
        {
        if ( ESoundPlayerPlaying == iState )
            {
            iMdaPlayer->Stop();
            }
        delete iMdaPlayer;
        iMdaPlayer = NULL;
        }
    MUS_LOG( "mus: [MUSIND]  <- CMusSoundPlayer::CMusSoundPlayer" )
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusSoundPlayer::CMusSoundPlayer( MMusSoundPlayerObserver& aObserver ):
    iObserver( aObserver ),
    iState( ESoundPlayerReady )
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusSoundPlayer::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
// Plays specified indication sound.
// -----------------------------------------------------------------------------
//
void CMusSoundPlayer::PlayL( TVsSoundIndication aIndication )
    {
    MUS_LOG( "mus: [MUSIND]  -> CMusSoundPlayer::PlayL" )
    if ( ESoundPlayerPlaying == iState )
        {
        User::Leave( KErrNotReady );
        }

    HBufC* soundFileName = SoundFileNameLC( aIndication );

    // Used KAudioPrefVoiceAid from audiopreference.h as preference since it
    // should always be played.
    iMdaPlayer = CMdaAudioPlayerUtility::NewFilePlayerL(
        *soundFileName,
        *this,
        EMdaPriorityMax,
        (TMdaPriorityPreference) KAudioPrefNotification );

    iMdaPlayer->SetPriority( EMdaPriorityMax,
                             (TMdaPriorityPreference) KAudioPrefNotification );

    CleanupStack::PopAndDestroy( soundFileName );

    iState = ESoundPlayerPlaying;
    MUS_LOG( "mus: [MUSIND]  <- CMusSoundPlayer::PlayL" )
    }


// -----------------------------------------------------------------------------
// Stops playing current sound.
// -----------------------------------------------------------------------------
//
void CMusSoundPlayer::Stop()
    {
    MUS_LOG( "mus: [MUSIND]  -> CMusSoundPlayer::Stop" )
    if ( ESoundPlayerPlaying == iState )
        {
        iMdaPlayer->Stop();
        }

    delete iMdaPlayer;
    iMdaPlayer = NULL;

    iState = ESoundPlayerReady;
    MUS_LOG( "mus: [MUSIND]  <- CMusSoundPlayer::Stop" )
    }


// -----------------------------------------------------------------------------
// From MMdaAudioPlayerCallback.
// Defines required client behaviour when an attempt to open and initialise an
// audio sample has completed, successfully or otherwise.
// -----------------------------------------------------------------------------
//
void CMusSoundPlayer::MapcInitComplete(
    TInt aError,
    const TTimeIntervalMicroSeconds& /*aDuration*/ )
    {
    MUS_LOG1( "mus: [MUSIND]  -> CMusSoundPlayer::MapcInitComplete(%d)", aError )
    if ( aError )
        {
        delete iMdaPlayer;
        iMdaPlayer = NULL;
        iState = ESoundPlayerReady;
        iObserver.PlaySoundComplete( aError );
        }
    else
        {
        iMdaPlayer->Play();
        }
    MUS_LOG( "mus: [MUSIND]  <- CMusSoundPlayer::MapcInitComplete" )
    }


// -----------------------------------------------------------------------------
// From MMdaAudioPlayerCallback.
// Informs of playback completion.
// -----------------------------------------------------------------------------
//
void CMusSoundPlayer::MapcPlayComplete( TInt aError )
    {
    MUS_LOG1( "mus: [MUSIND]  -> CMusSoundPlayer::MapcPlayComplete(%d)", aError )
    delete iMdaPlayer;
    iMdaPlayer = NULL;
    iState = ESoundPlayerReady;
    iObserver.PlaySoundComplete( aError );
    MUS_LOG( "mus: [MUSIND]  <- CMusSoundPlayer::MapcPlayComplete" )
    }


// -----------------------------------------------------------------------------
// Returns name of sound file associated with event.
// -----------------------------------------------------------------------------
//
HBufC* CMusSoundPlayer::SoundFileNameLC( TVsSoundIndication aIndication )
    {
    MUS_LOG1( "mus: [MUSIND]  -> CMusSoundPlayer::SoundFileNameLC(%d)",
              aIndication )
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    TFileName filename;    
    fs.PrivatePath( filename );
    fs.Close();    
    switch ( aIndication )
        {
        case EVsAvailableSound:
            {
            filename.Append( KMusAvailabilitySound() );
            }
            break;

        default:
            {
            User::Leave( KErrNotFound );
            }
        }

    /* In product all ways tone file will present in ROM , 
       Note: this prevents auditory tone hearing in sis installation.             
    */
    filename.Insert(0,_L("z:"));

    MUS_LOG( "mus: [MUSIND]  <- CMusSoundPlayer::SoundFileNameLC" )
    return filename.AllocLC();
    }
