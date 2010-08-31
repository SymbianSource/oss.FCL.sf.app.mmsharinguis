/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSHENGREVEIVESESSION_H
#define MUSHENGREVEIVESESSION_H

// INCLUDES
#include "musengmcesession.h"
#include "musenguriparser.h"
#include "musunittesting.h"
#include "lcvideoplayer.h"
#include <mcemediasink.h>

// FORWARD DECLARATIONS
class MMusEngReceiveSessionObserver;
class CMceInSession;


class CMusEngReceiveSession : 
    public CMusEngMceSession,
    public MLcVideoPlayer
    {
    
    public: // Contructors and destructor

       /**
        * Creates new MultimediaSharing Receive session.
        *
        * @param aRect UI drawing area. It is allowed handle from engine
        * @param aEngObserver Engine specific callbacks
        * @param aSessionObserver Session specific callbacks
        * @return CMusEngReceiveSession* New instanse of specified class
        */
        static CMusEngReceiveSession* NewL( const TRect& aRect );
       /**
        * Destructor
        *
        * @since S60 v3.2
        */
        ~CMusEngReceiveSession();
      

    public: // from MLcSession
        
        void EstablishLcSessionL();
    
        MLcVideoPlayer* RemoteVideoPlayer();    
        
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
        

    protected: // CONSTRUCTORS
    
        CMusEngReceiveSession( const TRect& aRect );

		void ConstructL();
		
       
    protected: // DATA
    
        // identity of originator parsed form P-Asserted-Identity field
        TBuf8<KMaxUriLength> iIdentity;
        
        HBufC8* iOriginator;
        
        HBufC* iRemoteDisplayName;
       
        /** 
        * Dummy member variable to implement MLcVideoPlayer::LcExtensionControls
        */
        RPointerArray< MLcValueControl > iValueControls; 
    };

#endif

