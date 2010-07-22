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

#ifndef MLCSOURCEFILECONTROL_H
#define MLCSOURCEFILECONTROL_H

// INCLUDES
#include <e32std.h>

// CLASS DECLARATION
/**
* Class for controlling the usage of a source file for a player.
*/
class MLcSourceFileControl
    {
    public: 
        
        /**
        * Sets the name of the source file.
        * @param aFileName the file name
        */
        virtual void SetLcFileNameL( const TFileName& aFileName ) = 0;
        
        /**
        * Returns the name of the source file.
        * @return the file name
        */
        virtual TFileName& LcFileName() = 0;
 
        /**
        * Starts or stops fast forwarding the file. 
        * The file is left paused after stopping the fast forwarding.
        * @param aUseFFWD ETrue to start FFWD, EFalse to stop it.
        */       
        virtual void LcFastForwardL( TBool aUseFFWD ) = 0;

        /**
        * Starts or stops fast rewinding the file. 
        * The file is left paused after stopping the fast rewinding.
        * @param aUseFRWD ETrue to start FRWD, EFalse to stop it.
        */        
        virtual void LcFastRewindL( TBool aUseFRWD ) = 0;

        /**
        * Returns duration of the file as time interval.
        * @return the duration of the file
        */        
        virtual TTimeIntervalSeconds LcFileDurationL() = 0;
        
        /**
        * Returns current file position as time interval.
        * @return the current position
        */        
        virtual TTimeIntervalSeconds LcFilePositionL() = 0;

        /**
        * Sets the file position as time interval.
        * @param aPosition the new position
        */ 
        virtual void SetLcFilePositionL( 
            const TTimeIntervalSeconds& aPosition ) = 0;       
    };

#endif // MLCSOURCEFILECONTROL_H

// end of file
