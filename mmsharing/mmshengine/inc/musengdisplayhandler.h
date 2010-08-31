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


#ifndef MUSHENGDISPLAYHANDLER_H
#define MUSHENGDISPLAYHANDLER_H


#include <e32std.h>

// CLASS DECLARATION

/**
* An Interface for display services
*
* @lib museng.lib
* @since S60 v3.2
*/
class MMusEngDisplayHandler
    {

    public:
        
        /*
        * Defines possible rotations 
        */
        enum TDisplayOrientation
            {
            EPortrait, // Normal
            ELandscape // 90 degree's clockwise rotation
            };

        /**
        * Returns currently assigned drawing area
        *
        * @return TRect This session drawing area rectangle
        */
        virtual TRect Rect() const = 0;

        /**
        * Sets new drawing area
        *
        * @param TRect This session new drawing area rectangle
        */
        virtual void SetRectL( const TRect& aRect ) = 0;
        
        /**
        * Sets secondary rect (e.g. viewfinder in twoway session)
        * @param TRect This session new secondary drawing area rectangle
        */
        virtual void SetSecondaryRectL( const TRect& aSecondaryRect ) = 0;
        
        /**
        * Gets secondary rect.
        * @return TRect This session secondary drawing area rectangle
        */
        virtual TRect SecondaryRect() const = 0;
        
        /**
        * Enables or disables display. Call to this function is considered
        * as a permission or denial to draw to the display.
        */
        virtual void EnableDisplayL( TBool aEnable ) = 0;
        
        /**
        * Check whether display is enabled 
        */
        virtual TBool IsDisplayEnabled() = 0;
        
        /**
        * Returns current display orientation.
        *
        * @pre Session is ongoing
        * @return Current display orientation 
        * @leave KErrNotReady if precondition is not fullfilled
        */
        virtual TDisplayOrientation OrientationL() = 0;
        
        /**
        * Sets display orientation.
        *
        * @pre Session is ongoing
        * @return Sets display orientation 
        * @leave KErrNotReady if precondition is not fullfilled
        */
        virtual void SetOrientationL( TDisplayOrientation aOrientation ) = 0;
        
        /**
        * Check if display is actively displaying content
        * @return ETrue if is displaying content, otherwise EFalse
        */
        virtual TBool IsDisplayActive() = 0;
    };

#endif
