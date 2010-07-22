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

#ifndef LCSESSIONSTUB_H
#define LCSESSIONSTUB_H

#include <QString>
#include <QMap>
#include <e32std.h>

class MLcUiProvider;

extern bool lcutStub_LcSession_expectCall( const QString& call, int callNdx = 0);
extern void lcutStub_LcSession_reset();
extern MLcUiProvider* lcutStub_LcSession_lcUiProvider();

const char lcutStub_LcSession_NoCall[] = "NOCALL";
const char lcutStub_LcSession_establishSession[] = "LcEngine::EstablishLcSessionL";
const char lcutStub_LcSession_terminateSession[] = "LcEngine::TerminateLcSessionL";
const char lcutStub_LcSession_updateSession[] = "LcEngine::UpdateLcSessionL";

_LIT( KLcStubLocalDisplayName, "local");
_LIT( KLcStubRemoteDisplayName, "remote");
_LIT( KLcStubRemoteDetails, "112233459");

// INCLUDES
#include "lcsession.h"

#include <e32std.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MLcSessionObserver;
class MLcUiProvider;
class CLcVideoPlayer;


// CLASS DECLARATION
/**
* A class representing a Live Comms session. 
*/
class CLcSession : public CBase,
                   public MLcSession
    {
public:
        /**
        * Creates new CLcSession.
        *
        * @return CLcSession* New instanse of specified class
        */
        static CLcSession* NewL();  
        

public:

        /**
        * Destructor
        */
        ~CLcSession();

public: // from MLcSession
    
        /**
        * Returns the current session state.
        * @return the session state
        */
        TLcSessionState LcSessionState() const;

        /**
        * Sets the observer for the session related events.
        * @param aObserver the observer
        */
        void SetLcSessionObserver( MLcSessionObserver* aObserver );

        /**
        * Sets the callback for the UI prompts.
        * @param aUiProvider callback for UI prompts
        */
        void SetLcUiProvider( MLcUiProvider* aUiProvider );        
        
        /**
        * Establishes the session.
        * In case of a MO session initiates the session negotiation.
        * In case of a MT session accepts the incoming session. 
        */
        void EstablishLcSessionL();
    
        /**
        * Terminates the session.
        * Can be used for terminating an existing session as well as 
        * cancelling a MO session that has not yet been established.
        * Can also be used for rejecting an incoming session.  
        */
        void TerminateLcSessionL();
       
        /**
        * Returns the video player for the received media.
        * Is present if the session contains video 
        * received from the remote participant.
        * @return the remote video player or NULL if not present
        */
        MLcVideoPlayer* RemoteVideoPlayer();
    
        /**
        * Returns the video player for the media that is being sent.
        * Is present if the session contains outgoing video.
        * @return the local video player or NULL if not present
        */
        MLcVideoPlayer* LocalVideoPlayer();
       
        /**
        * Returns the own display name for the session.
        * @return the local display name
        */
        const TDesC& LocalDisplayName();
    
        /**
        * Returns the remote party's display name for the session.
        * @return the remote display name
        */
        const TDesC& RemoteDisplayName();

        /**
        * Sets a parameter for the session.
        * The parameters can be plug-in specific.
        * @param aId the identifier for the parameter
        * @param aValue the value for the parameter
        */
        TInt SetParameter( TInt aId, TInt aValue );

        /**
        * Returns a value for a session parameter.
        * @param aId the identifier for the parameter
        * @return the value for the parameter or an error if not present
        */
        TInt ParameterValue( TInt aId );        
        
        /**
        * Checks whether application should be started in background mode. 
        * In background mode session will be pre-set up without user   
        * interaction and will be in paused state.
        * @return ETrue if background mode should be used
        */
        TBool IsBackgroundStartup();
        
        /**
       * Engine can be informed about application foreground status via
       * this method.
       * @param aIsForeground ETrue if application is at foreground, EFalse
       *   if at background
       * @return KErrNone if succesfully handled
       */
       TInt SetForegroundStatus( TBool aIsForeground );
       
       /**
       * Returns remote party details for the session
       * @return remote party details or KNullDesC if those are not known
       */
       const TDesC& RemoteDetails();
        
       /**
       * Updates established session. 
       * All the changes, if they are not yet delivered, to players, windows 
       * and session parameters has been commited after the call.
       */
       void UpdateLcSessionL();
       
       /**
       * Send DTMF signals. 
       * @param aKey Key pressed from dialpad. Key can be  [0-9],*,#,A,B,C,D.
       * which is 16 key combination of DTMF.
       * Send DTMF signals to remote party.
       * @return ETrue upon success else EFalse.
       */
       TBool SendDialTone( TChar aKey);

       
protected:

        /**
        * Constructor
        */
        CLcSession( );
        
        /**
        * Second-phase constructor
        */
        void ConstructL();
public:
        
        MLcSessionObserver* iSessionObserver;
        MLcUiProvider* iLcUiProvider;
        HBufC* iLocalDisplayName; 
        HBufC* iRemoteDisplayName;
        HBufC* iRemoteDetails;
        CLcVideoPlayer* iLocalPlayer;
        CLcVideoPlayer* iRemotePlayer;
        TBool iIsForeground;
        TBool iForegroundChange;
        TLcSessionState iState;
        
        QMap<int, int> mParams;
        
    };
 
#endif // LCSESSIONSTUB_H

// end of file
