/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MLCAUDIOCONTROL_H
#define MLCAUDIOCONTROL_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION
/**
* Class for adjusting the audio related settings 
* such as muting and unmuting the audio and changing its volume level.
*/
class MLcAudioControl
    {
    public:

        /**
        * Checks whether the local audio has been muted.
        * @return ETrue if the local audio has been muted, otherwise EFalse.
        */
        virtual TBool IsLcAudioMutedL() = 0;

        /**
        * Mutes / unmutes the local audio.
        * @param aMute ETrue mutes the audio, EFalse unmutes it.
        */
    	virtual void MuteLcAudioL( TBool aMute ) = 0;    	

    	/**
    	* Checks whether the microphone has been muted,
    	* @return ETrue if the microphone has been muted, otherwise EFalse.
    	*/
        virtual TBool IsLcMicMutedL() = 0;    

        /**
        * Mutes / unmutes the microphone.
        * @param aMute ETrue mutes the microphone, EFalse unmutes it.
        */
        virtual void MuteLcMicL( TBool aMute ) = 0;

        /**
        * Checks whether it is allowed 
        * to enable the loudspeaker by calling EnableLcLoudspeakerL.
        * @return ETrue if enabling the loudspeaker is allowed, otherwise EFalse. 
        */
        virtual TBool IsEnablingLcLoudspeakerAllowed() = 0;
        
        /**
        * Enables / disables the loudspeaker.
        * @pre IsEnablingLcLoudspeakerAllowed() == ETrue
        * @param aEnabled ETrue enables the loudspeaker, EFalse disables it.
        */
        virtual void EnableLcLoudspeakerL( TBool aEnabled ) = 0;
        
        /**
        * Checks whether the loudspeaker has been enabled.
        * @return ETrue if the loudspeaker has been enabled, otherwise EFalse.
        */
        virtual TBool IsLcLoudspeakerEnabled() = 0;

        /**
        * Returns the current volume level.
        * @return the current volume level
        */
        virtual TInt LcVolumeL() = 0;
     
        /**
        * Sets the volume level.
        * @param aValue the volume level
        */
        virtual void SetLcVolumeL( TInt aValue ) = 0;    
    
        /**
        * Increases the volume level by one step.
        */
        virtual void IncreaseLcVolumeL() = 0;
        
        /**
        * Decreases the volume level by one step.
        */
        virtual void DecreaseLcVolumeL() = 0;        
    };

#endif // MLCAUDIOCONTROL_H

// end of file
