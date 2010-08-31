/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSHENGLIVEVIDEOPLAYER_H
#define MUSHENGLIVEVIDEOPLAYER_H

// INCLUDES
#include "musenglocalvideoplayer.h"
#include "lcdestinationfilecontrol.h"

// FORWARD DECLARATIONS
class CMceSession;
class TMusEngCameraHandler;

// CLASS DECLARATION
class CMusEngLiveVideoPlayer : 
    public CMusEngLocalVideoPlayer,
    public MLcDestinationFileControl
    {
    public: // Constructors and destructor
        
        static CMusEngLiveVideoPlayer* NewL( 
            MMusEngDisplayHandler& aDisplayHandler,
            TMusEngCameraHandler& aCameraHandler,
            MLcAudioControl& aLcAudioControl );
        
        ~CMusEngLiveVideoPlayer();
        
    public: // From MLcVideoPlayer
        
        TBool LcIsPlayingL();
        
        void LcPlayL();
        
        void LcPauseL();

        MLcCameraControl* LcCameraControl();
        
        MLcDestinationFileControl* LcDestinationFileControl();
            
        MLcZoomControl* LcZoomControl();
    
        MLcBrightnessControl* LcBrightnessControl(); 
        
    public: // From MLcDestinationFileControl

        void SetLcFileNameL( const TFileName& aFileName );
        
        TFileName& LcFileName();

        void LcRecordL( TBool aRecord );

        TBool LcIsRecording();
        
    private: // Constructors
        
        CMusEngLiveVideoPlayer(
            MMusEngDisplayHandler& aDisplayHandler,
            TMusEngCameraHandler& aCameraHandler,
            MLcAudioControl& aLcAudioControl );
        
    private: // Data
        
        TFileName iFileName;
        TMusEngCameraHandler& iCameraHandler;
    };

#endif // MUSHENGLIVEVIDEOPLAYER_H
