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
* Description:  Multimedia Sharing Domain Publish and Subscribe keys
*
*/


#ifndef MUSADOMAINPSKEYS_H
#define MUSADOMAINPSKEYS_H

#include <e32std.h>

// CONSTANTS

/******************************************************************************
* Multimedia Sharing Session API
******************************************************************************/

namespace NMusSessionApi 
    {
    const TUid KMusSessionApi = {0x1028238D}; 

    /**
     * Indication about Multimedia Sharing availability change. 
     *
     * @type RProperty::TInt
     */
    const TUint32 KMusAvailabilityStatus = 0x00000012;   
    
    /**
     * Indication about Multimedia Sharing able to show indicator. 
     *
     * @type RProperty::TBool
     */
    const TBool KMusIndicator = 0x0000001C;
      
    }

#endif   // MUSADOMAINPSKEYS_H

// End of file