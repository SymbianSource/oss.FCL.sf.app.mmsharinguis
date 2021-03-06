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

#include "musengsessionmanager.h"
#include <ECom.h>
#include <ImplementationProxy.h>

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TImplementationProxy KMusEnginePluginImplementationTable[] =
    {
    IMPLEMENTATION_PROXY_ENTRY( 0x20029876, CMusEngSessionManager::NewL )
    };


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy( TInt& aTableCount )
    {
    aTableCount = sizeof( KMusEnginePluginImplementationTable ) /
                  sizeof( TImplementationProxy );

    return KMusEnginePluginImplementationTable;
    }
