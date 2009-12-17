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
* Description:   Plug-in entry point
*
*/



// INCLUDE FILES
#include "musunittesting.h"

#include "musavadefaultimp.h"
#include "musavailabilityplugin.hrh"


// SYSTEM
#include <e32base.h>
#include <e32std.h>
#include <ecom/implementationproxy.h>

// Disabled PC-Lint warning for "suspicious typecast" caused by Symbian's
// ECom declarations
/*lint -e611 */

// ------------------------------------------------------------------------------
// Exported proxy for instantiation method resolution.  Use the interface
// UID (same as the DLL UID) and the implementation factory function as a
// pair for ECom instantiation.
// ------------------------------------------------------------------------------
// 
const TImplementationProxy ImplementationTable[] = 
    {
    IMPLEMENTATION_PROXY_ENTRY(
    KAvailabilityPluginImplementationUid, CMusAvaDefaultImp::NewL )
    };


// ------------------------------------------------------------------------------
// Give the table to the caller.
// ------------------------------------------------------------------------------
// .
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}

