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

#ifndef MLCDESTINATIONFILECONTROL_H
#define MLCDESTINATIONFILECONTROL_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION
/**
* Class for controlling the usage of a destination file for a player.
*/
class MLcDestinationFileControl
    {
    public: 
        
        /**
        * Sets the name of the destination file.
        * @param aFileName the file name
        */
        virtual void SetLcFileNameL( const TFileName& aFileName ) = 0;
        
        /**
        * Returns the name of the destination file.
        * @return the file name
        */
        virtual TFileName& LcFileName() = 0;

        /**
        * Starts or pauses recording
        */
        virtual void LcRecordL( TBool aRecord ) = 0;

        /**
        * Checks whether recording is paused or not
        */
        virtual TBool LcIsRecording() = 0;    
    };

#endif // MLCDESTINATIONFILECONTROL_H

// end of file
