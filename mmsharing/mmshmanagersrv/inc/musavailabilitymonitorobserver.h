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
* Description:  ?Description
*
*/


#ifndef MUSCAPABILITYPLUGINMANAGEROBSERVER_H
#define MUSCAPABILITYPLUGINMANAGEROBSERVER_H

#include <e32base.h>

#include "musmanagercommon.h"

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 v3.2
 */
class MMusAvailabilityPluginManagerObserver 
    {
public:

    virtual void PluginStopped() = 0;
    
    virtual void AvailabilityChangedL( 
    						MultimediaSharing::TMusAvailabilityStatus aAvailability ) = 0;
    };


#endif // ? MUSCAPABILITYPLUGINMANAGEROBSERVER_H
