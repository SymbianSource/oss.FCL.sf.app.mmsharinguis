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

// FORWARD DECLARATIONS
class CRepository;
class CTelephonyAudioRouting;
class CPhCltCommandHandler;
class MMusEngAudioRoutingObserver;

/**
* Utility class to handle all phone related requests:
* Audio routing, volume control and microphone muting.
*/
class CMusEngTelephoneUtils : public CActive,
                              public MTelephonyAudioRoutingObserver
    {
    MUS_UNITTEST( UT_CMusEngTelephoneUtils )
    MUS_UNITTEST( UT_CMusEngSession )
	
	public: 
		
		/**
   		*
        */
		static CMusEngTelephoneUtils* NewL( 
		    MMusEngAudioRoutingObserver& aAudioRoutingObserver );

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
        TBool AudioRoutingCanBeChanged();

		/**
   		* Changes the audio routing between loudspeaker and handset.
        */
        void LoudspeakerL( TBool aEnable );

		/**
   		*
        */
		TBool IsLoudSpeakerEnabled() const;
		
		/**
   		*
        */
		TInt GetVolumeL() const;

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
    
    private: // inherited from CActive

		/**
   		*
        */
        void RunL();
        
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
        
    private:

		/**
   		* Default C++ constructor
        */
		CMusEngTelephoneUtils( 
		    MMusEngAudioRoutingObserver& aAudioRoutingObserver );

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
		

    private: // DATA
        
        MMusEngAudioRoutingObserver& iAudioRoutingObserver;

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
    };

#endif // MUSENGTELEPHONEUTILS_H
