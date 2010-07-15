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


#ifndef MUSENGTELEPHONEUTILS_H
#define MUSENGTELEPHONEUTILS_H


#include "musunittesting.h"

// SYSTEM
#include <e32base.h>
#include <MTelephonyAudioRoutingObserver.h>
#include <TelephonyAudioRouting.h>
#include <RPhCltServer.h>
#include <cenrepnotifyhandler.h>

// FORWARD DECLARATIONS
class CRepository;
class CTelephonyAudioRouting;
class CPhCltCommandHandler;
class MMusEngAudioRoutingObserver;
class MMusEngVolumeChangeObserver;

/**
* Utility class to handle all phone related requests:
* Audio routing, volume control and microphone muting.
*/
class CMusEngTelephoneUtils : public CActive,
                              public MTelephonyAudioRoutingObserver,
                              public MCenRepNotifyHandlerCallback
    {
    MUS_UNITTEST( UT_CMusEngTelephoneUtils )
    MUS_UNITTEST( UT_CMusEngSession )
	
	public: 
		
		/**
   		*
        */
		static CMusEngTelephoneUtils* NewL();

		/**
   		*
        */
		~CMusEngTelephoneUtils();


	public: // API
	
	    /**
        * Checks if audio routing can be changed using LoudspeakerL
        * 
        * @return ETrue if audio routing can be changed using LoudspeakerL
        */
        TBool AudioRoutingCanBeChanged() const;
        
        /**
		* Checks if audio output is BT
		* 
		* @return ETrue if audio routing is BT
		*/
        TBool AudioOutputIsBT() const;
        
		/**
   		* Changes the audio routing between loudspeaker and handset.
   		*  
   		* @param aShowDialog if ETrue, user is notified about new audio routing
        */
        void LoudspeakerL( TBool aEnable, TBool aShowDialog );

		/**
   		*
        */
		TBool IsLoudSpeakerEnabled() const;
		
		/**
   		* Returns current CS call volume level from central repository
        */
		TInt GetVolumeL() const;

        /**
        * Returns locally cached CS call volume level
        */
		TInt GetVolume() const;
		
		/**
   		*
        */
		void SetVolumeL( TInt aVolume );
		
		/**
        * Mutes CS call microphone.
        */
        void MuteMicL( TBool aMute );
        
        /**
        * Returns current CS call mic mute state.
        */
        TBool IsMicMutedL();
    
        /**
        * Sets audio routing observer. Can be set to NULL in order to indicate
        * ending of observing changes in audio routing.
        */
        void SetAudioRoutingObserver( MMusEngAudioRoutingObserver* aObserver );
                                        
        /**
        * Sets volume level observer. Can be set to NULL in order to indicate
        * ending of observing changes in volume level.
        */
        void SetVolumeChangeObserver( MMusEngVolumeChangeObserver* aObserver );
    
    private: // inherited from CActive

		/**
   		*
        */
        void RunL();
        TInt RunError( TInt aError );
        
		/**
   		* Cancels outstanding request to phone client
        */
        void DoCancel();
            
		
    private: // MTelephonyAudioRoutingObserver functions

        /**
         * Available outputs have changed
         */
        void AvailableOutputsChanged( 
                        CTelephonyAudioRouting& aTelephonyAudioRouting );
    
        /**
         * Some other application has changed audio output routing
         */
        void OutputChanged( 
                        CTelephonyAudioRouting& aTelephonyAudioRouting );
    
        /**
         * Our request to change audio output routing has completed
         */
        void SetOutputComplete( 
                        CTelephonyAudioRouting& aTelephonyAudioRouting,
                        TInt aError );
    
        /**
        * Set output if setting is currently allowed. Leaves with KErrAccessDenied
        * if setting is not allowed.
        */
        void DoSetOutputL( CTelephonyAudioRouting::TAudioOutput aAudioOutput );
        
    private:// From MCenRepNotifyHandlerCallback
        
        void HandleNotifyGeneric( TUint32 aId );
        
    private:

		/**
   		* Default C++ constructor
        */
		CMusEngTelephoneUtils();

		/**
   		* 2nd phase constructor
        */
		void ConstructL();


	private: // HELPER
	
		/**
   		* Checks that volume value is between 1 and 10.
   		*
   		* @return validated volume value
        */
		TInt ValidateVolume( const TInt aVolume ) const;
		
		/**
		 * Checks current volume level and notifies observer, if volume changed
		 * @param aAudioRouteChanged, ETrue if volume check should be done
		 *        because of audio route change
		 */
		void UpdateCurrentVolume( TBool aAudioRouteChanged );
		
    private: // DATA

        /**
        *  Stores the audio routing state at startup.
        */
        CTelephonyAudioRouting::TAudioOutput iAudioOutputAtStartup;
        
        /**
        * Telephony audio routing instance. Owned.
        */
        CTelephonyAudioRouting* iTelephonyAudioRouting;

        /**
        * Central repository. Owned.
        */
        CRepository* iRepository;

        /**
        * Command handler for muting the microphone
        */        
        CPhCltCommandHandler* iPhoneCommandHandler;
        
        /**
        * Not owned.
        */
        MMusEngAudioRoutingObserver* iAudioRoutingObserver;

        TBool iShowDialog;
        
        /**
         * Central repository notifier instance. Owned.
         */
        CCenRepNotifyHandler* iNotifier;
       
        /**
         * Volume change observer
         * Used to inform session about volume updates
         */
        MMusEngVolumeChangeObserver* iVolumeObserver;
        
        TInt iCurrentVolume;
    };


#endif // MUSENGTELEPHONEUTILS_H