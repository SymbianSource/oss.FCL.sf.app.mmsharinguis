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
* Description:  ?Description
*
*/


#ifndef MUSAVAILABILITYPLUGINLOADER_H
#define MUSAVAILABILITYPLUGINLOADER_H

#include <e32base.h>

class CMusAvailabilityPlugin;

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class MusAvailabilityPluginLoader
    {

public:

    static CMusAvailabilityPlugin* LoadPluginImplementationL();

    };


#endif // ? MUSAVAILABILITYPLUGINLOADER_H
