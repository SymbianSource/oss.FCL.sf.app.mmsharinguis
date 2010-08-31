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

#ifndef LCVIDEOPLAYERSTUB_H
#define LCVIDEOPLAYERSTUB_H

#include <e32base.h>    // CBase

// USER
#include "lcvideoplayer.h"
#include "lcaudiocontrol.h"
#include "lcwindow.h"
#include "lccameracontrol.h"
#include "lczoomcontrol.h"
#include "lcbrightnesscontrol.h"

class   MLcSourceFileControl; 
class   MLcDestinationFileControl; 


#include <QString>

extern bool lcutStub_LcPlayer_expectCall( const QString& call, int callNdx = 0);
extern void lcutStub_LcPlayer_reset();
extern void lcutStub_LcPlayer_asyncCompletion( bool async );

const char lcutStub_LcPlayer_NoCall[] = "NOCALL";
const char lcutStub_LcPlayer_LcPlayL[] = "LcPlayer::LcPlayL";
const char lcutStub_LcPlayer_LcPauseL[] = "LcPlayer::LcPauseL";
const char lcutStub_LcPlayer_SetLcWindowRectL[] = "LcWindow::SetLcWindowRectL";
const char lcutStub_LcPlayer_EnableLcWindowL[] = "LcPlayer::EnableLcWindowL";
const char lcutStub_LcPlayer_DecreaseLcBrightnessL[] = "LcPlayer::DecreaseLcBrightnessL";
const char lcutStub_LcPlayer_EnableLcLoudspeakerL[] = "LcPlayer::EnableLcLoudspeakerL";
const char lcutStub_LcPlayer_LcZoomInL[] = "LcPlayer::LcZoomInL";
const char lcutStub_LcPlayer_LcZoomOutL[] = "LcPlayer::LcZoomOutL";
const char lcutStub_LcPlayer_MuteLcAudioL[] = "LcPlayer::MuteLcAudioL";
const char lcutStub_LcPlayer_MuteLcMicL[] = "LcPlayer::MuteLcMicL";
const char lcutStub_LcPlayer_SetLcBrightnessL[] = "LcPlayer::SetLcBrightnessL";
const char lcutStub_LcPlayer_IncreaseLcBrightnessL[] = "LcPlayer::IncreaseLcBrightnessL";
const char lcutStub_LcPlayer_SetLcZoomValueL[] = "LcPlayer::SetLcZoomValueL";
const char lcutStub_LcPlayer_ToggleLcCameraL[] = "LcPlayer::ToggleLcCameraL";
const char lcutStub_LcPlayer_SetLcWindowOrientationL[] = "LcWindow::SetLcWindowOrientationL";


// CLASS DECLARATION
/**
*  
*/
class CLcVideoPlayer :  public CBase,
                        public MLcVideoPlayer,
                        public MLcAudioControl,
                        public MLcWindow,
                        public MLcCameraControl,
                        public MLcZoomControl,
                        public MLcBrightnessControl
    {
    public:
        /**
        * Creates new CLcSession.
        *
        * @return CLcSession* New instanse of specified class
        */
        static CLcVideoPlayer* NewL();  
    
    public: 
    
        /**
        * Destructor
        */
        ~CLcVideoPlayer();


    public: // from MLcVideoPlayer
            
        TLcVideoPlayerState LcVideoPlayerState() const;
        
        TBool LcIsPlayingL();
        
        void LcPlayL(TLcVideoPlayerSource aSource=ECameraSource );
        
        void LcPauseL();
    
        MLcWindow* LcWindow();

        MLcCameraControl* LcCameraControl();
        
        MLcSourceFileControl* LcSourceFileControl();
            
        MLcDestinationFileControl* LcDestinationFileControl();
        
        MLcAudioControl* LcAudioControl();
    
        MLcZoomControl* LcZoomControl();
    
        MLcBrightnessControl* LcBrightnessControl();
        
public: // from MLcWindow
        
        void EnableLcWindowL( TBool aEnable );
        
        TBool IsLcWindowEnabled();
        
        void SetLcWindowRectL( TRect aRect );
        
        TRect LcWindowRect();
        
        void SetLcWindowOrientationL( 
            TLcWindowOrientation aOrientation );
        
        TLcWindowOrientation LcWindowOrientationL(); 
        
public: // from MLcAudioControl
        
        TBool IsLcAudioMutedL();

        void MuteLcAudioL( TBool aMute );       

        TBool IsLcMicMutedL();    

        void MuteLcMicL( TBool aMute );
        
        TBool IsLcLoudspeakerEnabled();

        void EnableLcLoudspeakerL( TBool aEnabled );
        
        TBool IsEnablingLcLoudspeakerAllowed();
        
        TInt LcVolumeL();
     
        void SetLcVolumeL( TInt aValue );    
    
        void IncreaseLcVolumeL();
        
        void DecreaseLcVolumeL();  
            
        
public: //from MLcCameraControl
    
    TInt LcCameraCountL();
    TInt CurrentLcCameraIndex();
    void ToggleLcCameraL();
    
public: //from MLcZoomControl
    
    TInt MinLcZoomL();
    TInt MaxLcZoomL();
    TInt LcZoomValueL();
    void SetLcZoomValueL( TInt aValue );
    void LcZoomInL();
    void LcZoomOutL();
    
public: //from MLcBrightnessControl
        
    TInt MinLcBrightnessL();
    TInt MaxLcBrightnessL();
    TInt LcBrightnessL();
    void SetLcBrightnessL( TInt aValue );
    void IncreaseLcBrightnessL();
    void DecreaseLcBrightnessL();
    
    protected: // CONSTRUCTORS

        /**
        * Constructor
        *
        */
        CLcVideoPlayer( );

        /**
        * Second-phase constructor
        *
        */
        void ConstructL();
        

    public:

        /**
        * Drawing area rect.
        */
        TRect iRect;
        
        MLcWindow::TLcWindowOrientation iOrientation;
        
        TBool iWindowEnabled;
        
        TBool iMuted;
        
        TBool iMicMuted;
        
        TBool iLoudspeakerEnabled;
        
        TBool iEnablingLoudspeakerAllowed;
        
        TInt iVolume;
        
        MLcVideoPlayer::TLcVideoPlayerState iState;
        TBool iPlaying;

        
    };
    
#endif //LCVIDEOPLAYERSTUB_H
