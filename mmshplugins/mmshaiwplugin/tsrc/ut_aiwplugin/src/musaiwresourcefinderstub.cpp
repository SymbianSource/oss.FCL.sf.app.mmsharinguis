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
* Description:  The ECOM interface implementation.
*
*/


#include "musaiwproviderstubs.h"

// possible locations of compiled resource file

_LIT( KMusAiwProviderResource1, "c:\\sys\\bin\\musaiwprovider2.rsc" );
_LIT( KMusAiwProviderResource2, "c:\\sys\\bin\\musaiwprovider3.rsc" );

const TDesC& MusAiwResourceFinderStub::ResourceFilenameRom()
    {
    return KMusAiwProviderResource1();
    }

const TDesC& MusAiwResourceFinderStub::ResourceFilenameRam()
    {
    return KMusAiwProviderResource2();
    }


// end of file
