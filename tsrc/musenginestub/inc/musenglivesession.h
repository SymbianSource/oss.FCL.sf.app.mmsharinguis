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


#ifndef MUSHENGLIVESESSION_H
#define MUSHENGLIVESESSION_H

// USER
#include "musengmceoutsession.h"
#include "musunittesting.h"
#include "mussettings.h"
#include "musengcamerahandler.h"
#include "lcvideoplayer.h"
#include "lccameracontrol.h"
#include "lcbrightnesscontrol.h"
#include "lczoomcontrol.h"
#include "lcfilecontrol.h"

// SYSTEM
#include <ecam.h>

// FORWARD DECLARATIONS
class MMusEngLiveSessionObserver;
class MLcValueControl;


class CMusEngLiveSession : 
    public CMusEngMceOutSession, 
    public MLcVideoPlayer,
    public MLcCameraControl,
    public MLcBrightnessControl,
    public MLcZoomControl,
    public MLcFileControl
    {
    
    public:
        /**
        * Creates new MultimediaSharing Live session.
        *
        * @param aFileName Media filename
        * @param aRect UI drawing area. It is allowed handle from engine
        * @param aSessionObserver Interface for session specific callbacks
        * @param aOutSessionObserver Interface for outsession specific callbacks
        * @param aLiveSessionObserver interface for live session specific
        *        callbacks
        * @return CMusEngLiveSession* New instanse of specified class
        */
        static CMusEngLiveSession* NewL( 
							const TDesC& aFileName,
                            const TRect& aRect );
                            
        /**
        * Creates new MultimediaSharing Live session.
        *
        * @param aRect UI drawing area. It is allowed handle from engine
        * @param aSessionObserver Interface for session specific callbacks
        * @param aOutSessionObserver Interface for outsession specific callbacks
        * @param aLiveSessionObserver interface for live session specific
        *        callbacks
        * @return CMusEngLiveSession* New instanse of specified class
        */
        static CMusEngLiveSession* NewL( 
                            const TRect& aRect );


    public:

        /**
        * Destructor
        */
        ~CMusEngLiveSession();


    public: // from MLcSession
        
        void EstablishLcSessionL();
    
        MLcVideoPlayer* LocalVideoPlayer();
        
        const TDesC& RemoteDisplayName();
        
        
    public: // from MLcVideoPlayer
        
        TLcVideoPlayerState LcVideoPlayerState() const;
        
        TBool LcIsPlayingL();
        
        void LcPlayL();
        
        void LcPauseL();
    
        MLcWindow* LcWindow();

        MLcCameraControl* LcCameraControl();
        
        MLcFileControl* LcSourceFile();
        
        MLcFileControl* LcDestinationFile();
        
        MLcAudioControl* LcAudioControl();
    
        MLcZoomControl* LcZoomControl();
    
        MLcBrightnessControl* LcBrightnessControl();

        RPointerArray< MLcValueControl >& LcExtensionControls();    

    public: // from MLcCameraControl
        
        TInt LcCameraCountL();

        void ToggleLcCameraL();       

    public: // MLcBrightnessControl
        
        TInt MinLcBrightnessL();

        TInt MaxLcBrightnessL();

        TInt LcBrightnessL();

        void SetLcBrightnessL( TInt aValue );
        
        void IncreaseLcBrightnessL();
        
        void DecreaseLcBrightnessL();
        
    public: // from MLcZoomControl
        
        TInt MinLcZoomL();

        TInt MaxLcZoomL();

        TInt LcZoomValueL();

        void SetLcZoomValueL( TInt aValue );
        
        void LcZoomInL();
        
        void LcZoomOutL();        
        
    public: // from MLcFileControl
        
        void EnableLcFileL( TBool aEnable );
        
        TBool IsLcFileEnabled();

        void SetLcFileNameL( const TFileName& aFileName );
        
        TFileName& LcFileName();  
        
    protected:

        /**
        * Constructor
        */
        CMusEngLiveSession( const TRect& aRect, 
        					const TDesC& aRecordedFile = KNullDesC );

        /**
        * Second-phase constructor
        */
        void ConstructL();

    

    protected:

        /**
        * File name for recording. If KNullDesC no recording performed.
        */
        TFileName iRecordedFile;
        
        /** 
        * Dummy member variable to implement MLcVideoPlayer::LcExtensionControls
        */
        RPointerArray< MLcValueControl > iValueControls;    
        
        TMusEngCameraHandler iCameraHandler;
        
        /*
         * Remote Display Name
         */
        HBufC* iRemoteDisplayName;
        
        TInt iCurrentZoom;
        TInt iDefaultZoomFactor;
        TInt iCurrentBrighness;
        TInt iPlaying;
        TBool iFileEnabled;
        
    };

#endif

