/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSFILEUTIL_H
#define MUSFILEUTIL_H

#include <e32cmn.h>

class MusUiFileUtil
    {

    public:
        
        /**
         * return Existence of specified file
         */
        static TBool FileExists( const TDesC& aFullFileName );
        
        /**
         */
        static void VideoFilePathExistsL( const TDesC& aVideoFilePath );
        
        /**
         */
        static void CreateVideoFileNameL( const TDesC& aVideoFilePath, 
                                          TFileName& aFileName );
        
        /**
         */
        static void DeleteFileL( const TDesC& aFileName );

    };



#endif // MUSFILEUTIL_H
