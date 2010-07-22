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

#ifndef MLCCAMERACONTROL_H
#define MLCCAMERACONTROL_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION
/**
* Class for querying the device's camera capabilities 
* and switching between the device's cameras. 
*/
class MLcCameraControl
    {
    public:

        /**
        * Returns the count of cameras available in the device.
        * @return the count of cameras in the device
        */
        virtual TInt LcCameraCountL() = 0;

        /**
        * Return the index of the currently used camera.
        * @return the camera index
        */
        virtual TInt CurrentLcCameraIndex() = 0;
        
        /**
        * Switches to the next camera available in the device.
        * For example, if there are three cameras in the device,
        * the first call switches from camera 1 to camera 2, 
        * the second call switches from camera 2 to camera 3 and
        * the third call switches from camera 3 to camera 1. 
        */
        virtual void ToggleLcCameraL() = 0;
    };

#endif // MLCCAMERACONTROL_H

// end of file
