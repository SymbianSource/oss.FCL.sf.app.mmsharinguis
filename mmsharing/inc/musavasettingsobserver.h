/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This interface is used by
*                clients to get and availability setting for Video Charging
*
*/


#ifndef __MUSAVASETTINGSOBSERVER_H__
#define __MUSAVASETTINGSOBSERVER_H__

//  INCLUDES
#include <badesca.h>
#include <e32base.h>

/**
 *  Defines setting interface for the MusAvailability Plug-in
 *
 *  ECOM clients set and get setting are used by MusAvailability Plug-in.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class MMusAvaSettingsObserver
	{
public:
    enum TApplicationState
        {
        EApplicationStateNotDefined,
        EApplicationNotRunning,
        EApplicationIsRunning
        };
    
public:

    virtual TApplicationState ApplicationState() = 0;
    
    virtual TBool OptionAllowed() = 0;

    };


#endif // __MUSAVASETTINGSOBSERVER_H__

