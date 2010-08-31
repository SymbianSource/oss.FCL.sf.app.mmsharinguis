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


#ifndef MUSHENGTWOWAYRECVSESSION_H
#define MUSHENGTWOWAYRECVSESSION_H

// USER
#include "musengreceivesession.h"
#include "musunittesting.h"
#include "mussettings.h"
#include "musengcamerahandler.h"

// SYSTEM
#include <ecam.h>

// FORWARD DECLARATIONS
class CMceDisplaySink;
class CMceStreamBundle;
class CMusEngLiveVideoPlayer;


class CMusEngTwoWayRecvSession : public CMusEngReceiveSession
    {
    MUS_UNITTEST( UT_CMusEngTwoWayRecvSession )

    
    public:
        /**
        * Creates new MultimediaSharing two-way receive session.
        * @return CMusEngTwoWayRecvSession* New instanse of specified class
        */
        static CMusEngTwoWayRecvSession* NewL();

    public:

        /**
        * Destructor
        */
        ~CMusEngTwoWayRecvSession();

    public:
        
        void EnableDisplayL( TBool aEnable );
        
        TBool IsDisplayEnabled();

        CMusEngMceSession::TDisplayOrientation OrientationL();
        
        void SetOrientationL( TDisplayOrientation aOrientation );
        
    public:
        
        /**
        * Sets both remote (receiving video) and local (viewfinder) rects.
        * 
        */
        void SetRectsL( const TRect& aRemoteRect,
                                 const TRect& aLocalRect );
   
        /**
        * Gets camera handler (can be used for zooming etc.) 
        */
        TMusEngCameraHandler& Camera();
 
    public: // from MLcSession
    
        MLcVideoPlayer* LocalVideoPlayer();        
        
    protected: // From CMusEngReceiveSession
        
        /**
        * Sets local rect (viewfinder).
        * 
        */
        void SetSecondaryRectL( const TRect& aSecondaryRect );
        
        /**
        * Gets local rect (viewfinder).
        * 
        */
        TRect SecondaryRect() const;
        
        void CompleteSessionStructureL();
        
        void RectChangedL();
   
    private:

        /**
        * Constructor
        */
        CMusEngTwoWayRecvSession();

        /**
        * Second-phase constructor
        */
        void ConstructL();
        
    private:
        
        void CompleteSessionStructureAudioPartL( 
                   const RPointerArray<CMceMediaStream>& aStreams, 
                   CMceStreamBundle& aLocalBundle, 
                   CMceMediaStream* aVideoInStream,
                   CMceMediaStream* aVideoOutStream );
          
        void CompleteSessionStructureInStreamL( CMceMediaStream& aVideoInStream, 
                                                CMceStreamBundle& aLocalBundle );
          
        void CompleteSessionStructureOutStreamL( CMceMediaStream& aVideoOutStream );
        

    private:
        
        TRect iLocalRect;
        
        TMusEngCameraHandler iCameraHandler;
        
        /// The live video player implementing MLcVideoPlayer
        CMusEngLiveVideoPlayer* iLiveVideoPlayer;  
        
        TRect iSetRemoteRect;
        TRect iSetLocalRect;
       
    };

#endif

