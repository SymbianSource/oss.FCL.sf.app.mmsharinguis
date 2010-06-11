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

#ifndef MLCWINDOW_H
#define MLCWINDOW_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION
/**
* Class representing a window for a video player.
*/
class MLcWindow
    {
    public:

        /**
        * The possible window orientations.
        */
        enum TLcWindowOrientation
            {
            EPortrait, // Normal
            ELandscape // 90 degree's clockwise rotation
            };

        /**
        * Enables / disables the window into use 
        * and start / stops to show the video in it.
        * @param aEnabled ETrue enables the window, EFalse disables it.
        */
        virtual void EnableLcWindowL( TBool aEnable ) = 0;
        
        /**
        * Checks whether the window has been enabled.
        * @return ETrue if the window has been enabled, otherwise EFalse.
        */
        virtual TBool IsLcWindowEnabled() = 0;
        
        /**
        * Sets the rectangle for the window.
        * @param aRect the rectangle for the window
        */
        virtual void SetLcWindowRectL( TRect aRect ) = 0;
        
        /**
        * Returns the current rectangle for the window.
        * @return the current rectangle for the window
        */
        virtual TRect LcWindowRect() = 0;
        
        /**
        * Sets the window orientation.
        * @param aOrientation the current window orientation
        */
        virtual void SetLcWindowOrientationL( 
            TLcWindowOrientation aOrientation ) = 0;
        
        /**
        * Returns the current window orientation.
        * @return the current window orientation
        */
        virtual TLcWindowOrientation LcWindowOrientationL() = 0;
    };

#endif // MLCWINDOW_H

// end of file
