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
#include <ecam.h>

// FORWARD DECLARATIONS
class CMceCameraSource;
class CMceSession;

// CLASS

class TMusEngCameraHandler
    {
    MUS_UNITTEST( UT_CMusEngOutSession )
    MUS_UNITTEST( UT_CMusEngLiveSession )
    MUS_UNITTEST( UT_CMusEngTwoWaySession )
    MUS_UNITTEST( UT_CMusEngTwoWayRecvSession )
    MUS_UNITTEST( UT_TMusEngCameraHandler )
        
    public:
    /*
    * Defines current camera to be used  
    */
    enum TCurrentCamera
        {
        ENextCamera = -1,
        EBackCamera = 0, 
        EFrontCamera = 1
        };
    
    public:
    
        /** 
        * Determines the number of cameras on the device.
        *
        * @param aCurrentCamera returns current camera..
        *
        * @return Count of cameras present on the device.
        */
        IMPORT_C TInt CamerasAvailableL( TCurrentCamera& aCurrentCamera );
       
        /**
        * Change current camera to front to back
        * 
        * @param aCurrentCamera Change camera to be used.
        */
        IMPORT_C void ChangeCameraL( TCurrentCamera aCurrentCamera );

        /**
        * Gets current zoom factor
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt CurrentZoomL() const;
        
        /**
        * Sets new zoom factor
        * 
        * @pre Session is established
        * @pre MinZoomL <= aNewZoomFactor <= MaxZoomL
        * @leave KErrNotReady if session is not established
        */
        IMPORT_C void SetZoomL( TInt aNewZoomFactor );
        
        /**
        * Gets minimum zoom factor
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt MinZoomL() const;

        /**
        * Gets maximum zoom factor
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt MaxZoomL() const;
        
        /**
        * Increases zoom factor by one.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void ZoomInL();

        /**
        * Decreases zoom factor by one.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void ZoomOutL();

        /**
        * Sets zoom factor to default.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void ZoomDefaultL();

        /**
        * Sets current brightness
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void SetBrightnessL( TInt aBrightness ) const;


        /**
        * Gets current brightness
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt CurrentBrightnessL() const;

        /**
        * Gets maximum brightness
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt MaxBrightnessL() const;

        /**
        * Gets minimum brightness
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C TInt MinBrightnessL() const;

        /**
        * Increases brightness by one.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void IncreaseBrightnessL();

        /**
        * Decreases brightness by one.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void DecreaseBrightnessL();

        /**
        * Sets brightness factor to default.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void BrightnessDefaultL();

        /**
        * Sets brightness to automatic.
        * 
        * @pre Session is established
        * @leave KErrNotReady if precondition not fullfilled
        */
        IMPORT_C void BrightnessAutoL();
        
        /**
        * Resumes previously paused session. 
        * Continues using viewfinder and enables streaming video.
        *
        */
        IMPORT_C void PlayL();
        
        /**
        * Pauses session.
        * Holds display and disables streaming video to network.
        *
        */
        IMPORT_C void PauseL();
        
        /**
        * Tells whether session is paused or not
        */
        IMPORT_C TBool IsPlayingL();
                       

    public:
        
        TMusEngCameraHandler();
        
        void SetSession( CMceSession* aSession );
        
        void InitializeL( CMceCameraSource& aCamera );
        
        void InitializeZoomStepSize();
        
        void ReadCameraUsageKeyL();
        
        TInt ZoomStepSize( TInt64& aTime );
        
        void ChangeActiveCameraL( TCurrentCamera aNewCamera );

    private:
        
        CMceSession* iSession; // Not owned
        
        TCameraInfo iCameraInfo;

        TInt iDefaultZoomFactor;
        
        TInt iDefaultBrightness;
        
        TInt64 iZoomInTime;
        
        TInt64 iZoomOutTime;        
                      
        TInt iSmallZoomStep;
        
        TInt iBigZoomStep;
        
        TCurrentCamera iCurrentCamera;
        
        MusSettingsKeys::TUseCamera iCameraUsage;
        
    };

#endif

