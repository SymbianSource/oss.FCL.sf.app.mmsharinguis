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

#ifndef MLCSESSION_H
#define MLCSESSION_H

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class MLcSessionObserver;
class MLcUiProvider;
class MLcVideoPlayer;

// CLASS DECLARATION
/**
* A class representing a Live Comms session. 
*/
class MLcSession
    {
    public:

        /**
        * The list of possible session states.
        */
        enum TLcSessionState
            {
            EUninitialized,
            EInitialized,
            EReceived,
            EOpening,
            EOpen,
            EClosing,
            EClosed
            };

        /**
        * The possible failure reasons for a session.
        */
        enum TLcSessionFailure
            {
            ENoAnswer = 1,
            ERecipientNotFound,
            ESessionCancelled,
            ESessionRejected,
            ERecipientBusy,
            ERecipientTemporarilyNotAvailable,
            EDiskFull,
            EConnectionLost,
            EMediaInactivityTimeout,
            };
        
        /**
        * Returns the current session state.
        * @return the session state
        */
        virtual TLcSessionState LcSessionState() const = 0;

        /**
        * Sets the observer for the session related events.
        * @param aObserver the observer
        */
        virtual void SetLcSessionObserver( MLcSessionObserver* aObserver ) = 0;

        /**
        * Sets the callback for the UI prompts.
        * @param aUiProvider callback for UI prompts
        */
        virtual void SetLcUiProvider( MLcUiProvider* aUiProvider ) = 0;        
        
        /**
        * Establishes the session.
        * In case of a MO session initiates the session negotiation.
        * In case of a MT session accepts the incoming session. 
        */
        virtual void EstablishLcSessionL() = 0;
    
        /**
        * Terminates the session.
        * Can be used for terminating an existing session as well as 
        * cancelling a MO session that has not yet been established.
        * Can also be used for rejecting an incoming session.  
        */
        virtual void TerminateLcSessionL() = 0;
       
        /**
        * Returns the video player for the received media.
        * Is present if the session contains video 
        * received from the remote participant.
        * @return the remote video player or NULL if not present
        */
        virtual MLcVideoPlayer* RemoteVideoPlayer() = 0;
    
        /**
        * Returns the video player for the media that is being sent.
        * Is present if the session contains outgoing video.
        * @return the local video player or NULL if not present
        */
        virtual MLcVideoPlayer* LocalVideoPlayer() = 0;
       
        /**
        * Returns the own display name for the session.
        * @return the local display name or KNullDesC if local
        * display name is not known
        */
        virtual const TDesC& LocalDisplayName() = 0;
    
        /**
        * Returns the remote party's display name for the session.
        * @return the remote display name or KNullDesC if remote 
        * display name is not known
        */
        virtual const TDesC& RemoteDisplayName() = 0;

        /**
        * Sets a parameter for the session.
        * The parameters can be plug-in specific.
        * @param aId the identifier for the parameter
        * @param aValue the value for the parameter
        */
        virtual TInt SetParameter( TInt aId, TInt aValue ) = 0;

        /**
        * Returns a value for a session parameter.
        * @param aId the identifier for the parameter
        * @return the value for the parameter or an error if not present
        */
        virtual TInt ParameterValue( TInt aId ) = 0;        
        
        /**
        * Checks whether application should be started in background mode. 
        * In background mode session will be pre-set up without user   
        * interaction and will be in paused state.
        * @return ETrue if background mode should be used
        */
        virtual TBool IsBackgroundStartup() = 0;
        
        /**
        * Engine can be informed about application foreground status via
        * this method.
        * @param aIsForeground ETrue if application is at foreground, EFalse
        *   if at background
        * @return KErrNone if succesfully handled
        */
        virtual TInt SetForegroundStatus( TBool aIsForeground ) = 0;
        
        /**
        * Returns remote party details for the session
        * @return remote party details or KNullDesC if those are not known
        */
        virtual const TDesC& RemoteDetails() = 0;
        
        /**
        * Updates established session. 
        * All the changes, if they are not yet delivered, to players, windows 
        * and session parameters has been commited after the call.
        */
        virtual void UpdateLcSessionL() = 0;
        
        /**
        * Send DTMF signals. 
        * @param aKey Key pressed from dialpad. Key can be  [0-9],*,#,A,B,C,D.
        * which is 16 key combination of DTMF.
        * Send DTMF signals to remote party.
        * @return ETrue upon success else EFalse.
        */
        virtual TBool SendDialTone( TChar aKey) = 0;
    };
    
#endif // MLCSESSION_H

// end of file
