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

#ifndef MLCVIDEOPLAYER_H
#define MLCVIDEOPLAYER_H

// INCLUDES
#include <e32std.h>

// FORWARD DECLARATIONS
class MLcWindow;
class MLcCameraControl;
class MLcCamera;
class MLcSourceFileControl;
class MLcDestinationFileControl;
class MLcAudioControl;
class MLcZoomControl;
class MLcBrightnessControl;

// CLASS DECLARATION
/**
* A class representing a video player and its controls.
*/
class MLcVideoPlayer
    {
    public:


        /**
        * The list of possible video player sources.
        */
        enum TLcVideoPlayerSource
            {
            ECameraSource,
            EFileSource
            };

        /**
        * The list of possible video player states.
        */
        enum TLcVideoPlayerState
            {
            EInit,
            EPreparing,
            EBuffering,
            EPlaying,
            EPaused,
            EUnavailable
            };

        /**
        * Returns the current video player state.
        * @return the video player state
        */
        virtual TLcVideoPlayerState LcVideoPlayerState() const = 0;
        
        /**
        * Checks whether the video is currently being played.
        * @return ETrue if the video is being played, otherwise EFalse.
        */
        virtual TBool LcIsPlayingL() = 0;
        
        /**
        * Returns the current video player source.
        * @return the video player source.
        */
        virtual TLcVideoPlayerSource LcVideoPlayerSourceL() { return ECameraSource; };

        /**
        * Starts to play the source mentioned in the argument
        * @aSource : One of the sources in TLcVideoPlayerSource enum.
        * default  : Play from camera.
        */
        virtual void LcPlayL( TLcVideoPlayerSource aSource=ECameraSource ) = 0;
        
        /**
        * Pauses the playing of the video. 
        */
        virtual void LcPauseL() = 0;
    
        /**
        * Returns the window for the player, if present.
        * @return the window for the player or NULL if not present
        */
        virtual MLcWindow* LcWindow() = 0;

        /**
        * Returns the camera control, 
        * if the video player uses the device's camera 
        * as the source of the played video.
        * @return the camera control or NULL if not present
        */
        virtual MLcCameraControl* LcCameraControl() = 0;
        
        /**
        * Returns the source file control for the video player,
        * if the video is played from a local video file.
        * @return the source file control or NULL if not present
        */
        virtual MLcSourceFileControl* LcSourceFileControl() = 0;
        
        /**
        * Returns the destination file control for the video player,
        * if the video is being recorded to a local video file.
        * @return the destination file control or NULL if not present
        */
        virtual MLcDestinationFileControl* LcDestinationFileControl() = 0;
        
        /**
        * Returns the audio control for the player, if present.
        * @return the audio control for the player or NULL if not present
        */
        virtual MLcAudioControl* LcAudioControl() = 0;
    
        /**
        * Returns the zoom control for the player, if present.
        * @return the zoom control for the player or NULL if not present
        */
        virtual MLcZoomControl* LcZoomControl() = 0;
    
        /**
        * Returns the brightness control for the player, if present.
        * @return the brightness control for the player or NULL if not present
        */
        virtual MLcBrightnessControl* LcBrightnessControl() = 0;
    };

#endif // MLCVIDEOPLAYER_H

// end of file
