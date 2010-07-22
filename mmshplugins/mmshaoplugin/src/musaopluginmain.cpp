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
* Description:  DLL entry point
*
*/



#include <e32std.h>
#include <ecom/implementationproxy.h>

#include "musaoplugin.hrh"
#include "musaoplugin.h"

// --------------------------------------------------------------------------
// Exported proxy for instantiation method resolution.  Use the interface
// UID (same as the DLL UID) and the implementation factory function as a
// pair for ECom instantiation.
// --------------------------------------------------------------------------
// 
const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x1028238B, CMusAoPlugin::NewL )
    };


// --------------------------------------------------------------------------
// Give the table to the caller.
// --------------------------------------------------------------------------
// 
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / sizeof( TImplementationProxy );
    return ImplementationTable;
    }

// End of file

