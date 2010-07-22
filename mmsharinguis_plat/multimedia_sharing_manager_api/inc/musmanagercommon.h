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
* Description:  ?Description
*
*/



#ifndef MUSMANAGERCOMMON_H
#define MUSMANAGERCOMMON_H

#include <e32std.h>

/**
 *  A namespace container for MultimediaSharing enumerations.
 */
namespace MultimediaSharing
    {
    /**  Contains the possible values for availability status. */
    enum TMusAvailabilityStatus
        {
        // OK STATES
        ESipRegistrationPending = 1,    // SIP registration pending
        ESipOptionsSent,
        ESipOptionsNotSent,             //
        EMultimediaSharingAvailable,   
	 	EManualActivation,
	 	ESipOptionsNotCapable,

        // ERROR STATES
        EErrServerShutDown = 100,       // MUS Manager is Shut Down
        EMultimediaSharingNotAvailable,
        EErrSipRegistration,            // SIP registration failed / not done
        EErrNoActiveCall,               // no active call ongoing
        EErrCallOnHold,                 // CS call in hold
        EErrConferenceCall,             // conf call ongoing
        EErrRoaming,                    // we're roaming, but this app not allowed when so
        EErrNetwork,                    // network is of wrong type
        EErrConnection,                 // PDP connection error
        EErrActivation,                 // Activation is not allowed
        EMultimediaFailureCode,			// Start up forbiden
        // FATAL ERROR STATES
        EErrNoSipProfile = 1000         // no SIP profile; do not show MS menu options!
        };

    /**  Contains the possible use cases of MultimediaSharing. */
    enum TMusUseCase
        {
        EMusLiveVideo,           // live sharing from camera
        EMusClipVideo,           // video clip sharing
        EMusStillImage,          // still image sharing
        EMusReceive,             // receive video
        EMusContinue,            // continue running the existing application
        EMusTwoWayVideo,         // two-way sharing
        EMusReceiveTwoWayVideo   // two-way receive video
        };
   /**  Contains the possible commands of MultimediaSharing. */    
   enum TCommandType
        {
        ECommandNotDefined,
       	ECommandManualActivation
        };
    }

#endif // MUSMANAGERCOMMON_H

// End of File
