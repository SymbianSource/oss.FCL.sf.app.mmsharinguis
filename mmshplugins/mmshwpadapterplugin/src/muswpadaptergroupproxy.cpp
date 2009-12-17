/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  project specification
*
*/


#include "muswpadapteruids.h"
#include "muswpadapter.h"

#include <ecom/implementationproxy.h>


// --------------------------------------------------------------------------
// Exported proxy for instantiation method resolution.  Use the interface
// UID (same as the DLL UID) and the implementation factory function as a
// pair for ECom instantiation.
// --------------------------------------------------------------------------
//
const TImplementationProxy ImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( KMUSWPADAPTERDLLUID3, 
                                CMusWpAdapter::NewL )
    };


// --------------------------------------------------------------------------
// Implementation factory function.
// Returns: TImplementationProxy*: pointer to TImplementationProxy
// --------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( 
                                                    TInt& aTableCount )
    {
    aTableCount = sizeof( ImplementationTable ) / 
                  sizeof( TImplementationProxy );
    return ImplementationTable;
    }
