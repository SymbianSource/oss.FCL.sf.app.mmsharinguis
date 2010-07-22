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
class MMusEngLiveSessionObserver;
class CMceDisplaySink;
class CMceStreamBundle;

class CMusEngTwoWayRecvSession : public CMusEngReceiveSession
    {
    MUS_UNITTEST( UT_CMusEngTwoWayRecvSession )

    
    public:
        /**
        * Creates new MultimediaSharing two-way receive session.
        *
        * @param aRemoteRect UI drawing area for received video.
        * @param aLocalRect UI drawing area for camera viewfinder.
        * @param aSessionObserver Interface for session specific callbacks
        * @param aReceiveSessionObserver Interface for receivesession specific callbacks
        * @return CMusEngTwoWayRecvSession* New instanse of specified class
        */
        IMPORT_C static CMusEngTwoWayRecvSession* NewL( 
                            const TRect& aRemoteRect,
                            const TRect& aLocalRect,
                            MMusEngSessionObserver& aSessionObserver,
                            MMusEngReceiveSessionObserver& aReceiveSessionObserver );


    public:

        /**
        * Destructor
        */
        ~CMusEngTwoWayRecvSession();

    public:
        
        void EnableDisplayL( TBool aEnable );

        CMusEngMceSession::TDisplayOrientation OrientationL();
        
        void SetOrientationL( TDisplayOrientation aOrientation );
        
    public:
        
        /**
        * Sets both remote (receiving video) and local (viewfinder) rects.
        * 
        */
        IMPORT_C void SetRectsL( const TRect& aRemoteRect,
                                 const TRect& aLocalRect );

        /**
        * Sets local rect (viewfinder).
        * 
        */
        IMPORT_C void SetLocalRectL( const TRect& aLocalRect );
        
        /**
        * Gets local rect (viewfinder).
        * 
        */
        IMPORT_C TRect LocalRect() const;
   
        /**
        * Gets camera handler (can be used for zooming etc.) 
        */
        IMPORT_C TMusEngCameraHandler& Camera();
        
    protected: // From CMusEngReceiveSession
        
        void CompleteSessionStructureL();
        
        void RectChangedL();

   
    private: // from MMceStreamObserver, 
             // overrides the functions in ancestor classes


        void StreamStateChanged( CMceMediaStream& aStream,
                                 CMceMediaSink& aSink );   
        
        void StreamStateChanged( CMceMediaStream& aStream );
                                 
    private:

        /**
        * Constructor
        */
        CMusEngTwoWayRecvSession( MMusEngSessionObserver& aSessionObserver,
                            MMusEngReceiveSessionObserver& aReceiveSessionObserver,
                            const TRect& aRemoteRect,
                            const TRect& aLocalRect );

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
    };

#endif

