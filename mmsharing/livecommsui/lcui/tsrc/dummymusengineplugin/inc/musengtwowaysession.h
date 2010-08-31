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


#ifndef MUSHENGTWOWAYSESSION_H
#define MUSHENGTWOWAYSESSION_H

// USER
#include "musengmcesession.h"

// SYSTEM
#include <ecam.h>

// FORWARD DECLARATIONS
class MMusEngLiveSessionObserver;
class CMceDisplaySink;
class MMusEngTwoWaySessionObserver;
class CMusEngRemoteVideoPlayer;


class CMusEngTwoWaySession : public CMusEngMceSession
    {    
    public:
        /**
        * Creates new MultimediaSharing Live session.
        * @return CMusEngTwoWaySession* New instanse of specified class
        */
        static CMusEngTwoWaySession* NewL();


    public:

        /**
        * Destructor
        */
        ~CMusEngTwoWaySession();

    public:
        
        void EnableDisplayL( TBool aEnable );

        void SetOrientationL( MMusEngDisplayHandler::TDisplayOrientation aOrientation );

        void EstablishLcSessionL();
        
        void TerminateLcSessionL();        
        
        MLcVideoPlayer* RemoteVideoPlayer();
    public:
        
        /**
        * Sets both remote (receiving video) and local (viewfinder) rects.
        * 
        */
        void SetRectsL( const TRect& aRemoteRect,
                        const TRect& aLocalRect );
        
    private:

        /**
        * Constructor
        */
        CMusEngTwoWaySession();

        /**
        * Second-phase constructor
        */
        void ConstructL();

    };

#endif

