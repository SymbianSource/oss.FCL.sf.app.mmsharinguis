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


#ifndef MUSHENGCAMERAHANDLER_H
#define MUSHENGCAMERAHANDLER_H

// USER
#include "musunittesting.h"
#include "mussettings.h"

// SYSTEM
#include <lccameracontrol.h>
#include <lcbrightnesscontrol.h>
#include <lczoomcontrol.h>
#include <ecam.h>

// FORWARD DECLARATIONS
class CMceCameraSource;
class CMceSession;

// CLASS

class TMusEngCameraHandler :    
    public MLcCameraControl,
    public MLcBrightnessControl,
    public MLcZoomControl
    {
    MUS_UNITTEST( UT_CMusEngOutSession )
    MUS_UNITTEST( UT_CMusEngLiveSession )
    MUS_UNITTEST( UT_CMusEngTwoWaySession )
    MUS_UNITTEST( UT_CMusEngTwoWayRecvSession )
    MUS_UNITTEST( UT_TMusEngCameraHandler )       
        
    public: // From MLcCameraControl
        
        TInt LcCameraCountL();

        TInt CurrentLcCameraIndex();
        
        void ToggleLcCameraL();       

    public: // From MLcBrightnessControl
        
        TInt MinLcBrightnessL();

        TInt MaxLcBrightnessL();

        TInt LcBrightnessL();

        void SetLcBrightnessL( TInt aValue );
        
        void IncreaseLcBrightnessL();
        
        void DecreaseLcBrightnessL();
        
    public: // From MLcZoomControl
        
        TInt MinLcZoomL();

        TInt MaxLcZoomL();

        TInt LcZoomValueL();

        void SetLcZoomValueL( TInt aValue );
        
        void LcZoomInL();
        
        void LcZoomOutL();     
    
    public:
        
        /**
        * Resumes previously paused session. 
        * Continues using viewfinder and enables streaming video.
        *
        */
        void PlayL();
        
        /**
        * Pauses session.
        * Holds display and disables streaming video to network.
        *
        */
        void PauseL();
        
        /**
        * Tells whether session is paused or not
        */
        TBool IsPlayingL();
                       
    public:
        
        TMusEngCameraHandler();
        
        void SetSession( CMceSession* aSession );
        
        void InitializeL( CMceCameraSource& aCamera );
        
        void InitializeZoomStepSize();
        
        void ReadCameraUsageKeyL();
        
        TInt ZoomStepSize( TInt64& aTime );

    private:
    
        enum TCurrentCamera
            {
            ECameraNotAvailable = KErrNotFound,
            EBackCamera = 0, 
            EFrontCamera = 1
            };         
   
    private: // New functions
        
        TInt CamerasAvailableL( TCurrentCamera& aCurrentCamera );

        void ChangeCameraL( TCurrentCamera aCurrentCamera );
        
        void ChangeActiveCameraL( TCurrentCamera aNewCamera );
        
    private: // Data
        
        CMceSession* iSession; // Not owned
        
        TCameraInfo iCameraInfo;

        TInt iDefaultBrightness;
        
        TInt64 iZoomInTime;
        
        TInt64 iZoomOutTime;        
                      
        TInt iSmallZoomStep;
        
        TInt iBigZoomStep;
        
        TCurrentCamera iCurrentCamera;
        
        MusSettingsKeys::TUseCamera iCameraUsage;    
    };

#endif

