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

#ifndef MLCBRIGHTNESSCONTROL_H
#define MLCBRIGHTNESSCONTROL_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION
/**
* Class for adjusting the brightness of a video.
*/
class MLcBrightnessControl
    {
    public:

        /**
        * Returns the minimum brightness level.
        * @return the minimum brightness level
        */
        virtual TInt MinLcBrightnessL() = 0;

        /**
        * Returns the maximum brightness level.
        * @return the maximum brightness level
        */
        virtual TInt MaxLcBrightnessL() = 0;

        /**
        * Returns the current brightness level.
        * @return the current brightness level
        */
        virtual TInt LcBrightnessL() = 0;

        /**
        * Sets the brightness level.
        * @pre aValue >= MinLcBrightnessL && aValue <= MaxLcBrightnessL
        * @param aValue the new brightness level
        */
        virtual void SetLcBrightnessL( TInt aValue ) = 0;
        
        /**
        * Increments the brightness level by one step.
        */
        virtual void IncreaseLcBrightnessL() = 0;
        
        /**
        * Decrements the brightness level by one step.
        */
        virtual void DecreaseLcBrightnessL() = 0; 
    };

#endif // MLCBRIGHTNESSCONTROL_H

// end of file
