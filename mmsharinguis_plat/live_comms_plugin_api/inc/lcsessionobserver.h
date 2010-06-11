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

#ifndef MLCSESSIONOBSERVER_H
#define MLCSESSIONOBSERVER_H

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class MLcSession;
class MLcVideoPlayer;

// CLASS DECLARATION
/**
* A callback for a Live Comms session related events. 
*/
class MLcSessionObserver
    {
    public:

        /**
        * The state of the session has changed.
        * The new state can be obtained from the session.
        * @param aSession the session
        */
        virtual void StateChanged( MLcSession& aSession ) = 0;
    
        /**
        * The state of the player has changed.
        * The new state can be obtained from the player.
        * @param aPlayer the player
        */
        virtual void StateChanged( MLcVideoPlayer& aPlayer ) = 0;
    
        /**
        * The session has been updated.
        * For example a new player has been added.
        * @param aSession the updated session
        */
        virtual void Updated( MLcSession& aSession ) = 0;

        /**
        * The session has been updated.
        * For example a new control has been added or 
        * an existing control has been updated.
        * @param aPlayer the updated player
        */
        virtual void Updated( MLcVideoPlayer& aPlayer ) = 0;
    
        /**
        * The session has failed.
        * @param aSession the failed session
        * @param aError the reason for the failure 
        */
    	virtual void Failed( 
            MLcSession& aSession,
            TInt aError ) = 0;
    
    	/**
    	* The player has failed.
    	* @param aPlayer the failed player
    	* @param aError the reason for the failure 
    	*/
        virtual void Failed( 
            MLcVideoPlayer& aPlayer, 
            TInt aError ) = 0;
    
        /**
        * The session duration time has changed.
        * @param aSession the session
        * @param aSeconds the current session duration.
        */
        virtual void SessionTimeChanged( 
            MLcSession& aSession,
            const TTimeIntervalSeconds& aSeconds ) = 0;
    };   
    
#endif // MLCSESSIONOBSERVER_H

// end of file
