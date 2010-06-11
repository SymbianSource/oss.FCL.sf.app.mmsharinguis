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


#ifndef MUSAIWPROVIDERSTUBS_H
#define MUSAIWPROVIDERSTUBS_H


#include <e32base.h>

class MusAiwResourceFinderStub
    {
public:
    /**
     * Returns the file name of resource file from ROM.
     *
     * @return The resource filename, if in ROM.
     */
    static const TDesC& ResourceFilenameRom();
    
    /**
     * Returns the file name of resource file from writable media.
     *
     * @return The resource filename, if in writable media.
     */
    static const TDesC& ResourceFilenameRam();
    };
    


#endif // MUSAIWRESOURCEFINDER_H
