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


#ifndef MUSHENGREMOTEVIDEOPLAYER_H
#define MUSHENGREMOTEVIDEOPLAYER_H

// INCLUDES
#include <e32base.h>
#include "musengvideoplayerbase.h"

// FORWARD DECLARATIONS
class CMceSession;
class MLcWindow;
class MLcAudioControl;

// CLASS DECLARATION
class CMusEngRemoteVideoPlayer : public CMusEngVideoPlayerBase
    {
    public: // Constructors and destructor
        
        static CMusEngRemoteVideoPlayer* NewL( MMusEngDisplayHandler& aDisplayHandler,
                                               MLcAudioControl& aLcAudioControl );
        
        ~CMusEngRemoteVideoPlayer();
     
        
    public: // from CMusEngVideoPlayerBase
        
        TLcVideoPlayerState LcVideoPlayerState() const;
        
        TBool LcIsPlayingL();
        
        TLcVideoPlayerState iState;
        
    private: // Constructors
        
        CMusEngRemoteVideoPlayer( MMusEngDisplayHandler& aDisplayHandler,
                                  MLcAudioControl& aLcAudioControl );
        void ConstructL();
        
    private: // Data
  
    };

#endif // MUSHENGREMOTEVIDEOPLAYER_H
