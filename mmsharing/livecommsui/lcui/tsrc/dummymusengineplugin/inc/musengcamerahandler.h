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
#include "lccameracontrol.h"
#include "lcbrightnesscontrol.h"
#include "lczoomcontrol.h"

// SYSTEM
#include <e32base.h>
#include <ecam.h>


// CLASS

class TMusEngCameraHandler :    
    public MLcCameraControl,
    public MLcBrightnessControl,
    public MLcZoomControl
    {
        
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
        
        TMusEngCameraHandler();
        
        TInt ZoomStepSize( TInt64& aTime );

    private:
    
        enum TCurrentCamera
            {
            ECameraNotAvailable = KErrNotFound,
            EBackCamera = 0, 
            EFrontCamera = 1
            };         
   
        
    private: // Data
        
        
        TCameraInfo iCameraInfo;

        TInt iDefaultBrightness;
        
        TInt64 iZoomInTime;
        
        TInt64 iZoomOutTime;        
                      
        TInt iSmallZoomStep;
        
        TInt iBigZoomStep;
        
        TCurrentCamera iCurrentCamera;
    };

#endif

