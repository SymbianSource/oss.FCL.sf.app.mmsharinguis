/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Observer interface for CMusSoundPlayer.
*
*/



#ifndef M_MUSSOUNDPLAYEROBSERVER_H
#define M_MUSSOUNDPLAYEROBSERVER_H


#include <e32base.h>


/**
 *  Observer interface for CMusSoundPlayer.
 *  Provides notify of sound playing status.
 *
 *  @lib musindicator.lib
 */
class MMusSoundPlayerObserver
    {
public:

    /**
     * Notifies of sound playing complete.
     *
     * @param aError Errorcode associated with playback.
     */
    virtual void PlaySoundComplete( TInt aError ) = 0;

    };


#endif // M_MUSSOUNDPLAYEROBSERVER_H
