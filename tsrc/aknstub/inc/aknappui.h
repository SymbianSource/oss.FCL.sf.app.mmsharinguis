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

#ifndef __AKNAPPUI_H__
#define __AKNAPPUI_H__


//#include <e32base.h>


class CAknAppUiBase
    {
    public:

    enum TAppUiOrientation
        {
        /**
        * Use the default device screen rotation of the product for this
        * application. This is the default rotation setting for an
        * application, and it should be used by nearly all applications.
        */
        EAppUiOrientationUnspecified,
        /**
        * Use a portrait screen rotation for this application.
        * This should only be used when an application specifically wants
        * portrait rotation. The device will select an appropriate portrait
        * rotation, if one is available.
        */
        EAppUiOrientationPortrait,
        /**
        * Use a landscape screen rotation for this application.
        * This should only be used when an application specifically wants
        * landscape rotation. The device will select an appropriate landscape
        * rotation, if one is available.
        */
        EAppUiOrientationLandscape,
        /**
        * Use the normal device screen rotation for this application.
        * Both portrait and landscape screen rotations are possible. The
        * application rotation follows device screen rotation.
        */
        EAppUiOrientationAutomatic
        };

    };

#endif