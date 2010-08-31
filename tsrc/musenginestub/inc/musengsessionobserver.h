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


#ifndef MUSHENGSESSIONOBSERVER_H
#define MUSHENGSESSIONOBSERVER_H


#include <e32std.h>

// CLASS DECLARATION

/**
* An interface to be implemented by users of Multimedia Sharing Engine if 
* they wish to receive events from session and streaming state changes.
*
* @lib museng.lib
* @since S60 v3.2
*/
class MMusEngSessionObserver
    {

    public:

        /**
        * Indicates that incoming or outgoing session has been established.
        *
        * @since S60 v3.2
        */
        virtual void SessionEstablished() = 0;

        /**
        * Indicates that session has been terminated.
        * After this function call session object must deleted.
        * Function is synchronous, use active object which actives
        * delete.
        *
        * @since S60 v3.2
        */
        virtual void SessionTerminated() = 0;

        /**
        * Current session has lost network connection.
        *
        * @since S60 v3.2
        */
        virtual void SessionConnectionLost() = 0; 

        /**
        * Undefined exception has occurred. Session is useless and must be
        * deleted. Function is synchronous, use active object which actives
        * delete.
        *
        * @since S60 v3.2
        */
        virtual void SessionFailed() = 0;

        /**
        * Current stream state is changed to idle meaning that
        * we are not sending or receiving RTP.
        *
        * @since S60 v3.2
        */
        virtual void StreamIdle() = 0;

        /**
        * Current session state is changed to idle.
        *
        * @since S60 v3.2
        */
        virtual void StreamStreaming() = 0;        

        /**
        * Update time information of this session
        *
        * @since S60 v3.2
        */
        virtual void SessionTimeChanged( const TTimeIntervalSeconds& aSeconds ) = 0;
        
        /**
        * RTCP inactivity timeout has occurred.
        *
        * @since S60 v3.2
        */
        virtual void InactivityTimeout() = 0;
    };

#endif