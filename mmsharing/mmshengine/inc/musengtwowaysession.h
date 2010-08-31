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
#include "musenglivesession.h"
#include "musunittesting.h"
#include "mussettings.h"

// SYSTEM
#include <ecam.h>

// FORWARD DECLARATIONS
class MMusEngLiveSessionObserver;
class CMceDisplaySink;
class MMusEngTwoWaySessionObserver;
class CMusEngRemoteVideoPlayer;


class CMusEngTwoWaySession : public CMusEngLiveSession
    {
    MUS_UNITTEST( UT_CMusEngOutSession )
    MUS_UNITTEST( UT_CMusEngTwoWaySession )

    
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

        void SetOrientationL( TDisplayOrientation aOrientation );
        
    public:
        
        /**
        * Sets both remote (receiving video) and local (viewfinder) rects.
        * 
        */
        void SetRectsL( const TRect& aRemoteRect,
                        const TRect& aLocalRect );

    public: // From MLcSession
        
        MLcVideoPlayer* RemoteVideoPlayer();
    
    public: // from MLcFileControl

        void EnableLcFileL( TBool aEnable );        
        
    protected: // inherited from CMusEngMceOutSession
        
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
        
        /**
        * 
        */
        TBool IsDisplayEnabled();
        
        /**
        * Checks if displaying remote video.
        *  
        */
        TBool IsDisplayActive();

        void RectChangedL();
        
        /**
        * @param aLocalBundle All local streams are supposed to be added to
        *        this bundle
        */
		void CompleteSessionStructureL( CMceStreamBundle& aLocalBundle );
		
		void AddDisplayL( CMceMediaStream& aStream );

        void CreateMceSessionStructureL( TBool aForceSdpBandwidth =  
                                                 EFalse );


    protected: // overrides ancestor class versions
        
        /**
        * The state of the session has changed
        * @param aSession, the session that has changed.
        */
        virtual void HandleSessionStateChanged( CMceSession& aSession,
                                                TInt aStatusCode,
                                                const TDesC8& aReasonPhrase );
                                                  
        /**
        * Sets Multimediasharing specific video codec settings like video 
        * bitrate maxbitrate. This functions gets called for every video codec 
        * in session. This function overrides function in ancestor classes. 
        * Function calls also overridden version.
        */ 
        virtual void AdjustVideoCodecL( CMceVideoCodec& aVideoCodec,
                                        TMceSourceType aSourceType );
          
    private: // from MMceStreamObserver, 
             // overrides the function in CMusEngMceSession
        
        void StreamStateChanged( CMceMediaStream& aStream );

        void  InactivityTimeout( CMceMediaStream& aStream,
                                 CMceRtpSource& aSource );
    
    private:

        /**
        * Constructor
        */
        CMusEngTwoWaySession();

        /**
        * Second-phase constructor
        */
        void ConstructL();

    private:
        
        void ReceivingStarted();
        
    private: // Data
        
        TRect iLocalRect;
        
        TBool iReceiving;
        
        TBool iBuffered;
        
        /// The remote video player implementing MLcVideoPlayer
        CMusEngRemoteVideoPlayer* iRemoteVideoPlayer;
        
        TRect iSetRemoteRect;
        TRect iSetLocalRect;
    };

#endif

