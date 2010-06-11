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


#ifndef MUSENGLIVESESSION_H
#define MUSENGLIVESESSION_H

// USER
#include "musengmceoutsession.h"
#include "musunittesting.h"
#include "mussettings.h"
#include "musengcamerahandler.h"

// SYSTEM
#include <lcvideoplayer.h>

// FORWARD DECLARATIONS
class MMusEngLiveSessionObserver;
class CMusEngLiveVideoPlayer;


class CMusEngLiveSession : 
    public CMusEngMceOutSession
    {
    MUS_UNITTEST( UT_CMusEngMceSession )
    MUS_UNITTEST( UT_CMusEngOutSession )
    MUS_UNITTEST( UT_CMusEngLiveSession )
    
    public:
                            
        /**
        * Creates new MultimediaSharing Live session.
        * @return CMusEngLiveSession* New instanse of specified class
        */
        static CMusEngLiveSession* NewL();

    public:

        /**
        * Destructor
        */
        ~CMusEngLiveSession();

    public: // from MLcSession
    
        MLcVideoPlayer* LocalVideoPlayer();  
        
    protected: // inherited from CMusEngMceOutSession

        /**
        * @param aLocalBundle All local streams are supposed to be added to
        *        this bundle
        */
		void CompleteSessionStructureL( CMceStreamBundle& aLocalBundle );


    protected: // overrides ancestor class versions
                                                
        /**
        * Sets Multimediasharing specific video codec settings like video 
        * bitrate maxbitrate. This functions gets called for every video codec 
        * in session. This function overrides function in ancestor classes. 
        * Function calls also overridden version.
        */ 
        virtual void AdjustVideoCodecL( CMceVideoCodec& aVideoCodec,
                                        TMceSourceType aSourceType );
        
        /**
        * Sets Multimediasharing specific audio codec settings. This functions 
        * gets called for every audio codec in session. This function overrides 
        * function in base class. Function calls also overridden version.
        */ 
        virtual void AdjustAudioCodecL( CMceAudioCodec& aAudioCodec );     
        
        /**
        * Removes AVC if known that it is not supported
        */
        virtual void DoCodecSelectionL( CMceVideoStream& aVideoStream );
   

    protected: // from MMceStreamObserver, 
             // overrides the function in CMusEngMceSession

        /**
        * The state of the sink has changed.
        *
        * @param aStream, the stream that uses the sink.
        * @param aSink, the sink that has changed.
        */
        void StreamStateChanged( CMceMediaStream& aStream,
                                 CMceMediaSink& aSink );    
                                 

    protected:

        /**
        * Constructor
        */
        CMusEngLiveSession();

        /**
        * Second-phase constructor
        */
        void ConstructL();


    protected:
        
        TMusEngCameraHandler iCameraHandler;
        CMusEngLiveVideoPlayer* iLiveVideoPlayer;
    };

#endif // MUSENGLIVESESSION_H

