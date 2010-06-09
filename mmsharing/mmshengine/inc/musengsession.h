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


#ifndef MUSENGSESSION_H
#define MUSENGSESSION_H

// USER INCLUDES
#include "musunittesting.h"
#include "musengvolumechangeobserver.h"

//SYSTEM INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class CMusEngTelephoneUtils;
class CMusSipProfileHandler;
class CRepository;
class MMusEngAudioRoutingObserver;
class MMusEngVolumeChangeObserver;

// CLASS DECLARATION

/**
* Base class for Multimedia Sharing Engine sessions, inherited by
* CMusEngMceSession.
*
* CMusEngSession encapsulates all common activities example using
* resources.
*
* @lib musengine.lib
*/
class CMusEngSession : public CBase,
                       public MMusEngVolumeChangeObserver
    {
    MUS_UNITTEST( UT_CMusEngSession )
    
    public:

        /**
        * Desctructor, finalize session
        *
        */
        ~CMusEngSession();


    public:  // API FUNCTIONS

        /**
        * Returns currently assigned drawing area
        *
        * @return TRect This session drawing area recttangle
        */
        IMPORT_C TRect Rect() const;

        /**
        * Sets new drawing area
        *
        * @param TRect This session new drawing area rectangle
        */
        IMPORT_C void SetRectL( const TRect& aRect );

        /**
        * Checks if audio routing can be changed using EnableLoudspeakerL
        * 
        * @return ETrue if audio routing can be changed using EnableLoudspeakerL
        */
        IMPORT_C TBool AudioRoutingCanBeChanged();
        
        /**
        * Activates/deactivates loadspeaker
        *
        * @param TBool Enable or disable loudspeaker
        * @param TBool Show activation dialog
        */
        IMPORT_C void EnableLoudspeakerL( TBool aEnable, TBool aShowDialog );

        /**
        * Checks is loudspeaker enabled or disabled.
        *
        * @return TBool true if enabled
        */
        IMPORT_C TBool IsLoudSpeakerEnabled() const;

        /**
        * Returns current volume level
        */
        IMPORT_C TInt VolumeL() const;

        /**
        * Mutes CS call microphone.
        */
        IMPORT_C void MuteMicL( TBool aMute );

        /**
        * Returns current CS call mic mute state.
        */
        IMPORT_C TBool IsMicMutedL();
        
        /**
        * Sets audio routing observer. Can be set to NULL in order to indicate
        * ending of observing changes in audio routing.
        */
        IMPORT_C void SetAudioRoutingObserver( 
                                    MMusEngAudioRoutingObserver* aObserver );
        
        /**
        * Sets volume level change observer. Can be set to NULL in order to indicate
        * ending of observing changes in volume level.
        */
        IMPORT_C void SetVolumeChangeObserver( 
                                    MMusEngVolumeChangeObserver* aObserver );

    public:  // VIRTUAL API FUNCTIONS
        
        /**
        * Increases volume level by one.
        */
        IMPORT_C virtual void VolumeUpL();

        /**
        * Decreases volume level by one.
        */
        IMPORT_C virtual void VolumeDownL();
        
        /**
        * Set volume value
        * @param aVal the value of volume 
        */
        IMPORT_C virtual void SetVolumeL( TInt aVal );
        
        /**
        * Enables or disables display. Call to this function is considered
        * as a permission or denial to draw to the display.
        */
        virtual void EnableDisplayL( TBool aEnable ) = 0;

        /**
        * Mutes current audio stream meaning that any audio is not played or
        * sent to network.
        */
        virtual void MuteL() = 0;

        /**
        * Unmute current audio stream meaning that all the audio is played
        * and if needed also sent to network.
        */
        virtual void UnmuteL() = 0;
        
        /**
         *
         */
        virtual void RefreshOrientationL() = 0;


	protected: // INTERNAL 

        virtual void RectChangedL() = 0;


        //from MMusEngVolumeChangeObserver
        virtual void VolumeChanged( TInt aVolume, TBool aAudioRouteChanged );
        
    protected:

        /**
        * Constructor, private cannot be instantiate
        *
        * @param aRect reserved drawing area for this session
        */
        CMusEngSession( const TRect& aRect );


    protected:

        /**
        * second-phase constructor, called by those session specific 
        * constructors
        */
        void ConstructL();


    private: // MEMBERS

        /**
        * Drawing area rect.
        */
        TRect iRect;

        /**
        * Telephone utilities.
        */
        CMusEngTelephoneUtils* iTelephoneUtils;
        
        /**
        * Volume change observer
        */
        MMusEngVolumeChangeObserver* iVolumeObserver;

    };

#endif //MUSENGSESSION_H
