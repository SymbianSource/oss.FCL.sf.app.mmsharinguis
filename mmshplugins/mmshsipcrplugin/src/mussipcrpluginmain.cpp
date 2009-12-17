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


#include "musunittesting.h"
#include "mussipcrplugin.h"
#include "mussipcrplugin.hrh"
#include "muscommon.h"
#include "muslogger.h"

#include <ecom/ecom.h>
#include <ecom/implementationproxy.h>

/**
 * Holds the implementation table.
 */
const TImplementationProxy KMusSipCrPluginImplementationTable[] =
    {
// disable PC-Lint warning for "suspicious typecast", caused by
// Symbian's ECom declarations, by "lint -e611"
    IMPLEMENTATION_PROXY_ENTRY( KMusSipCrPluginImplementationUid, //lint -e611
                                CMusSipCrPlugin::NewL )
    };

/**
 * Returns the implementation table and updates the aTableCount parameter
 * to hold the number of elements in the table.
 */
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    MUS_LOG( "mus: [MUSSCR]  -> TImplementationProxy* ImplementationGroupProxy" );
    aTableCount = sizeof( KMusSipCrPluginImplementationTable ) /
                  sizeof( TImplementationProxy );
    MUS_LOG( "mus: [MUSSCR]  <- TImplementationProxy* ImplementationGroupProxy" );
    return KMusSipCrPluginImplementationTable;
    }
