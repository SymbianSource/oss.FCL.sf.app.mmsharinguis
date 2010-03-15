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
* Description: 
*
*/


#ifndef MUSENGVOLUMECHANGEOBSERVER_H
#define MUSENGVOLUMECHANGEOBSERVER_H

// INCLUDES
#include <e32def.h>

// CLASS DECLARATION

/**
* An interface to be implemented by users of Multimedia Sharing Engine if 
* they wish to be notified about volume level CenRep keys change
*
*/
class MMusEngVolumeChangeObserver
    {
    public: 
    
        /**
	    * Indicates that volume level was changed either by some other 
	    * application or due to audio route change
	    * @param aVolume new volume level
	    * @param aAudioRouteChanged indicates whether volume level changed due
	    *        to audio route change
	    */
        virtual void VolumeChanged( TInt aVolume, TBool aAudioRouteChanged ) = 0;        
    };

#endif
