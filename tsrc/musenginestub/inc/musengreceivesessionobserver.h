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


#ifndef MUSHENGRECEIVESESSIONOBSERVER_H
#define MUSHENGRECEIVESESSIONOBSERVER_H

// INCLUDES

#include <e32cmn.h>

// CLASS DECLARATION

/**
* Observer interface to be implemented by clients interested about notifications
* of receiving (MT) side session specific events.
*/
class MMusEngReceiveSessionObserver
    {

    public:
    
        /**
        * There is new incoming session. 
        * Session is not yet handled in any way and cannot yet be accepted
        * or rejected. User should not be notified at this stage.
        */
        virtual void IncomingSessionPreNotification() = 0;
        
        /**
        * There is new incoming session ready to be accepted or rejected.
        * User can be notified.
        */
        virtual void IncomingSession( const TDesC& aOriginator, 
                                      const TDesC& aOriginatorIdentity ) = 0;

        /**
        * Current receiving session is buffering received RTP.
        */
        virtual void StreamBuffering() = 0;
        
    };

#endif