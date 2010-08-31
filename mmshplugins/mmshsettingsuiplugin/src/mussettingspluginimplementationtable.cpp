/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ECOM proxy table for MUSSettingsPlugin.
*
*/



#include "mussettingsplugin.h"
#include <ecom/implementationproxy.h>
#include <e32std.h>


// ======== LOCAL FUNCTIONS ========


const TImplementationProxy KGSVSSettingsPluginImplementationTable[] =
	{
	IMPLEMENTATION_PROXY_ENTRY( 0x1028238F,	CMusSettingsPlugin::NewL )
	};


// ======== GLOBAL FUNCTIONS ========


// ---------------------------------------------------------------------------
// Returns implementation table of this plugin to ECom framework.
// ---------------------------------------------------------------------------
//
EXPORT_C const TImplementationProxy* ImplementationGroupProxy(
    TInt& aTableCount )
	{
    RDebug::Print(_L("[GSLog] ImplementationGroupProxy()" ));
	aTableCount = sizeof(KGSVSSettingsPluginImplementationTable)
        / sizeof(TImplementationProxy);
	return KGSVSSettingsPluginImplementationTable;
	}
