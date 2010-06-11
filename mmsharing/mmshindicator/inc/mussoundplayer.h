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



#ifndef C_MUSSOUNDPLAYER_H
#define C_MUSSOUNDPLAYER_H


#include "musunittesting.h"
#include "mussoundplayerobserver.h"
#include <mdaaudiosampleplayer.h>
#include <e32base.h>


/**
 *  CMusSoundPlayer provides means to play custom indication sounds.
 *
 *  @code
 *  void CMyClass::PlaySoundL()
 *      {
 *      if ( iPlayer )
 *          {
 *          User::Leave( KErrInUse );
 *          }
 *      iPlayer = CMusSoundPlayer::NewL( *this );
 *      TRAPD( error, iPlayer->PlayL( CMusSoundPlayer::EVsAvailableSound ) );
 *      if ( error )
 *          {
 *          delete iPlayer;
 *          iPlayer = NULL;
 *          User::Leave( error );
 *          }
 *      }
 *
 *  // from MMusSoundPlayerObserver
 *  void CMyClass::PlaySoundComplete( TInt aError )
 *      {
 *      delete iPlayer;
 *      iPlayer = NULL;
 *      }
 *
 *  void CMyClass::~CMyClass()
 *      {
 *      delete iPlayer;
 *      iPlayer = NULL;
 *      }
 *  @endcode
 *
 *  @lib musindicator.lib
 */
class CMusSoundPlayer : public CBase, protected MMdaAudioPlayerCallback
    {
public:

    MUS_UNITTEST( UT_CMusSoundPlayer )

    /**  VS sound indications */
    enum TVsSoundIndication
        {
        EVsAvailableSound   // Plays sound associated to VS becoming available.
        };

    /**
     * Two-phased constructor.
     *
     * @param aObserver Reference to observer interface.
     * @return Pointer to newly instantiated CMusSoundPlayer.
     */
    static CMusSoundPlayer* NewL( MMusSoundPlayerObserver& aObserver );

    /**
     * Two-phased constructor.
     *
     * @param aObserver Reference to observer interface.
     * @return Pointer to newly instantiated CMusSoundPlayer.
     */
    static CMusSoundPlayer* NewLC( MMusSoundPlayerObserver& aObserver );

    /**
     * Destructor.
     */
    ~CMusSoundPlayer();

    /**
     * Plays selected sound.
     *
     * @param aIndication Identifies the played sound.
     */
    void PlayL( TVsSoundIndication aIndication );

    /**
     * Stops playing sound.
     */
    void Stop();

protected:

// from base class MMdaAudioPlayerCallback.

    /**
     * From MMdaAudioPlayerCallback.
     * Informs that initializing an audio sample has completed.
     *
     * @param aError Systemwide errorcode.
     * @param aDuration Duration of the sample.
     */
    virtual void MapcInitComplete( TInt aError, const TTimeIntervalMicroSeconds& aDuration );

    /**
     * From MMdaAudioPlayerCallback.
     * Informs that playing an audio sample has completed.
     *
     * @param aError Systemwide errorcode.
     */
    virtual void MapcPlayComplete( TInt aError );

private:

    CMusSoundPlayer( MMusSoundPlayerObserver& aObserver );

    void ConstructL();

    /**
     * Returns sound file name associated with event.
     *
     * @param aIndication Identifies sound to return filename for.
     * @return Buffer containing filename of sound. Ownership is transferred.
     */
    HBufC* SoundFileNameLC( TVsSoundIndication aIndication );

    /**  Internal state. */
    enum TMusSoundPlayerState
        {
        ESoundPlayerReady,
        ESoundPlayerPlaying
        };

private: // data

    /**
     * Reference to observer interface.
     */
    MMusSoundPlayerObserver& iObserver;

    /**
     * Internal state.
     */
    TMusSoundPlayerState iState;

    /**
     * Instance of MDA audio player.
     * Own.
     */
    CMdaAudioPlayerUtility* iMdaPlayer;
    };

#endif // C_MUSSOUNDPLAYER_H
