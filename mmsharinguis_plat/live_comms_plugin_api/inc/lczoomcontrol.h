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

#ifndef MLCZOOMCONTROL_H
#define MLCZOOMCONTROL_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION
/**
* Class for controlling the zoom level of a video player.
*/
class MLcZoomControl
    {
    public:

        /**
        * Returns the minimum zoom value.
        * @return the minimum zoom value
        */
        virtual TInt MinLcZoomL() = 0;

        /**
        * Returns the maximum zoom value.
        * @return the maximum zoom value
        */
        virtual TInt MaxLcZoomL() = 0;

        /**
        * Returns the current value for the zoom.
        * @return the current zoom value
        */
        virtual TInt LcZoomValueL() = 0;

        /**
        * Sets the current zoom level.
        * @pre aValue >= MinLcZoomL && aValue <= MaxLcZoomL
        * @param aValue the new zoom level
        */
        virtual void SetLcZoomValueL( TInt aValue ) = 0;
        
        /**
        * Zooms in by one step.
        */
        virtual void LcZoomInL() = 0;
        
        /**
        * Zooms out by one step.
        */
        virtual void LcZoomOutL() = 0; 
    };

#endif // MLCZOOMCONTROL_H

// end of file
