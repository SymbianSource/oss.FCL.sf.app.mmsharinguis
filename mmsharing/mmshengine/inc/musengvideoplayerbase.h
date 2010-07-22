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


#ifndef MUSHENGVIDEOPLAYERBASE_H
#define MUSHENGVIDEOPLAYERBASE_H

// INCLUDES
#include <e32base.h>
#include <lcvideoplayer.h>
#include <lcwindow.h>

// FORWARD DECLARATIONS
class CMceSession;
class TMusEngCameraHandler;
class MMusEngDisplayHandler;

// CLASS DECLARATION
class CMusEngVideoPlayerBase : 
    public CBase, public MLcVideoPlayer, public MLcWindow
    {
    public: // Constructors and destructor
        
        ~CMusEngVideoPlayerBase();

    public: // New functions
        
        void SetMceSession( CMceSession* aMceSession );
        
    public: // From MLcVideoPlayer
    
        virtual void LcPlayL( TLcVideoPlayerSource aSource=ECameraSource );
        
        virtual void LcPauseL();        
        
        MLcWindow* LcWindow();

        virtual MLcCameraControl* LcCameraControl();
        
        virtual MLcSourceFileControl* LcSourceFileControl();
        
        virtual MLcDestinationFileControl* LcDestinationFileControl();
        
        MLcAudioControl* LcAudioControl();
    
        virtual MLcZoomControl* LcZoomControl();
    
        virtual MLcBrightnessControl* LcBrightnessControl();
    
    public: // from MLcWindow
        
        void EnableLcWindowL( TBool aEnable );
        
        TBool IsLcWindowEnabled();        
        
        void SetLcWindowRectL( TRect aRect );
        
        TRect LcWindowRect();
        
        void SetLcWindowOrientationL( 
            TLcWindowOrientation aOrientation );
        
        TLcWindowOrientation LcWindowOrientationL();    
        
    protected: // Constructors
        
        CMusEngVideoPlayerBase(
            MMusEngDisplayHandler& aDisplayHandler,
            MLcAudioControl& aLcAudioControl );
        
    protected: // Data
        
        MMusEngDisplayHandler& iDisplayHandler;
        MLcAudioControl& iLcAudioControl;
        
        CMceSession* iMceSession; // Not owned        
    };

#endif // MUSHENGVIDEOPLAYERBASE_H
