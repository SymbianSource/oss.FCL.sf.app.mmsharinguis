/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Mus Applications Event Observer Interface
*
*/



#ifndef MUSUINAVIMEDIAANIMATOROBSERVER_H
#define MUSUINAVIMEDIAANIMATOROBSERVER_H

#include <e32base.h>
#include <eikenv.h>
#include <cntdef.h>

class MMusUiNaviMediaAnimatorObserver
    {

    public:

        /**
        * Sets new image for drawing. Ownership is not transferred.
        *
        */
        virtual void SetIcon( CGulIcon* aIcon ) = 0;

    };

#endif // MUSUIEVENTOBSERVER_H

// end of file
