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
* Description:  Global functions.
*
*/



#include "musaiwprovider.h"
#include "musaiwprovider.hrh"

#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>


/**
 * Holds the implementation table.
 */
const TImplementationProxy KMusAiwProviderImplementationTable[] =
    {
// disable PC-Lint warning for "suspicious typecast", caused by
// Symbian's ECom declarations, by "lint -e611"
    IMPLEMENTATION_PROXY_ENTRY( KMusAiwProviderUid, //lint -e611
                                CMusAiwProvider::NewL )
    };

/**
 * Returns the implementation table and updates the aTableCount parameter
 * to hold the number of elements in the table.
 */
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( KMusAiwProviderImplementationTable ) /
                  sizeof( TImplementationProxy );
    return KMusAiwProviderImplementationTable;
    }
