/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provides means to read strings from resource files.
*
*/



#ifndef MUSRESOURCEUTIL_H
#define MUSRESOURCEUTIL_H


#include "musunittesting.h"
#include <e32base.h>


/** Resource filename for musindicator.dll */
_LIT( KMusIndicatorResource, "musindicator.rsc" );


/**
 *  MusResourceUtil retrieves strings from resourcefiles.
 *
 *  @code
 *  HBufC* text = ReadResourceString16L( R_SOME_STRING_ID, KResourceFileName );
 *  @endcode
 *
 *  @lib musindicator.lib
 */
class MusResourceUtil
    {

public:

    /**
     * Reads resource string.
     *
     * @param aId Identifier of the resource to be read.
     * @param aFilename Name of used resource file.
     * @return Text containing resource string. Ownership is transferred.
     */
    static HBufC16* ReadResourceString16LC( TUint aId, const TDesC& aFilename );

    /**
     * Reads resource string.
     *
     * @param aId Identifier of the resource to be read.
     * @param aFilename Name of used resource file.
     * @return Text containing resource string. Ownership is transferred.
     */
    static HBufC16* ReadResourceString16L( TUint aId, const TDesC& aFilename );

    /**
     * Reads resource string.
     *
     * @param aId Identifier of the resource to be read.
     * @param aFilename Name of used resource file.
     * @return Text containing resource string. Ownership is transferred.
     */
    static HBufC8* ReadResourceString8LC( TUint aId, const TDesC& aFilename );

    /**
     * Reads resource string.
     *
     * @param aId Identifier of the resource to be read.
     * @param aFilename Name of used resource file.
     * @return Text containing resource string. Ownership is transferred.
     */
    static HBufC8* ReadResourceString8L( TUint aId, const TDesC& aFilename );

    /**
     * Converts HBufC8 to HBufC.
     *
     * @param aText Text to be converted.
     * @return Text converted text. Ownership is transferred.
     */
    static HBufC* ConvertResourceStringLC( const TDesC8& aText );

    /**
     * Constructs name of resource file.
     *
     * @param aFilename Name of resource file (eg. file.rsc).
     * @return Text containing resource filename. Ownership is transferred.
     */
    static HBufC* ResourceFilenameLC( const TDesC& aFilename );

    };

#endif // MUSRESOURCEUTIL_H
