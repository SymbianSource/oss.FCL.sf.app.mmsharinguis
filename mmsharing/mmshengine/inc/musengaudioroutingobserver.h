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


#ifndef MUSENGAUDIOROUTINGOBSERVER_H
#define MUSENGAUDIOROUTINGOBSERVER_H


// CLASS DECLARATION

/**
* An interface to be implemented by users of Multimedia Sharing Engine if 
* they wish to be notified about changes in audio routing.
*
*/
class MMusEngAudioRoutingObserver
    {

    public:

        /**
        * Indicates that some other application has changed audio routing.
        * @param aShowNote ETrue if notification of current change should be
        *        shown.
        *
        */
        virtual void AudioRoutingChanged( TBool aShowNote ) = 0;

        /**
        * Observer is required to give information whether audio route change
        * is allowed currently.
        * @param ETrue if change is allowed, otherwise EFalse
        *
        */
        virtual TBool AudioRouteChangeAllowed() const = 0;
        
    };

#endif // INCLUDE GUARD