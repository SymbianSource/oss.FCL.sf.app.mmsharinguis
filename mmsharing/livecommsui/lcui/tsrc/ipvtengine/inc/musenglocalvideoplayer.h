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


#ifndef MUSHENGLOCALVIDEOPLAYER_H
#define MUSHENGLOCALVIDEOPLAYER_H

// INCLUDES
#include "musengvideoplayerbase.h"

// CLASS DECLARATION
class CMusEngLocalVideoPlayer : public CMusEngVideoPlayerBase
    {
    public: // Constructors and destructor
        
        ~CMusEngLocalVideoPlayer();
        
    public: // from CMusEngVideoPlayerBase
        
        TLcVideoPlayerState LcVideoPlayerState() const;
        
        void SetLcWindowRectL( TRect aRect );
        
        TRect LcWindowRect();

        TLcVideoPlayerState iState;
        
    protected: // Constructors
        
        CMusEngLocalVideoPlayer(
            MMusEngDisplayHandler& aDisplayHandler,
            MLcAudioControl& aLcAudioControl );
    };

#endif // MUSHENGLOCALVIDEOPLAYER_H
