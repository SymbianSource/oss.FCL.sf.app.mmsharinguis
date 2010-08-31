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


#ifndef MUSENGMCESESSION_H
#define MUSENGMCESESSION_H

// USER
#include "musengcamerahandler.h"
#include "musengdisplayhandler.h"
#include "lcsession.h"
#include "lcaudiocontrol.h"
#include "svptimerobserver.h"

#include <e32base.h>

#include    <lcvtindicatorcontroller.h>


// FORWARD DECLARATIONS
class CContactTextField;
class CPbkContactItem;
class CPbkContactEngine;
class MLcSessionObserver;
class CMusEngLiveVideoPlayer;
class CMusEngRemoteVideoPlayer;
class CSVPTimer;
class CMccScreen;


// CONSTANTS

_LIT8( KMusEngAcceptContactHeader, 
       "Accept-Contact: *;+g.3gpp.cs-voice; explicit" );
_LIT8( KMusAcceptHeader, 
       "Accept: application/sdp" );
_LIT8( KMusEngSessionSdpLineXApplication, "a=X-application:com.nokia.rtvs\r\n" );
_LIT8( KMusEngSessionSdpLineApplication , "a=application:com.gsma.rts\r\n" );
_LIT8( KMusEngSessionSdpLineType, "a=type:videolive\r\n" );
_LIT8( KMusEngSessionSdpLineBandwidthLine  , "b=AS:" );
_LIT8( KMusEngSessionSdpLineBandwidthField  , "b=AS:85\r\n" );
_LIT8( KMusEngSessionSdpLineTiasLine  , "b=TIAS:" );
_LIT8( KMusPPreferredIdentity, "P-Preferred-Identity" );
_LIT8( KMusEngNewLine  , "\r\n" );

const TInt KMusTiasMultiplier = 1000;
const TUint8 KMusEngRtpKeepAliveTimer = 5;
const TUint8 KMusEngKeepAlivePayloadTypeVideoH263 = 96;
const TUint8 KMusEngKeepAlivePayloadTypeAudio = 97;
const TUint8 KMusEngKeepAlivePayloadTypeVideoAvc = 98;

// CLASS DECLARATION

/**
* Base class for Multimedia Sharing Engine MCE sessions, inherited by
* CMusEngLiveSession CMusEngClipSession and CMusEngReceiveSession.
*
* CMusEngMceSession encapsulates CMCESession and simplifies use of
* MCE Sessions to user interface. Also splits observers messy events
* to simplified function calls.
*
* @lib musengine.lib
*/
class CMusEngMceSession : public CBase,
                          public MLcSession,
                          public MLcAudioControl,
			  public MMusEngDisplayHandler,
			  public MSVPTimerObserver
    {
    public:

        /**
        * Destructor
        */
        ~CMusEngMceSession();

        void SetRemoteL( const TDesC& aRemote );
	      void ToggleCameraL();
	      void PlayCameraL();
	      void PauseCameraL();
        void InformUiProviderAboutReceivingStart();

    public: //  from MSVPTimerObserver

        virtual void TimedOut( TInt aTimerId );       

        
    public: //  from MMusEngDisplayHandler

        /**
        * Returns currently assigned drawing area
        *
        * @return TRect This session drawing area rectangle
        */
        TRect Rect() const;
        
        /**
        * Sets new drawing area
        *
        * @param TRect This session new drawing area rectangle
        */
        void SetRectL( const TRect& aRect );
        
        /**
        * Sets secondary rect (e.g. viewfinder in twoway session)
        * @param TRect This session new secondary drawing area rectangle
        */
        void SetSecondaryRectL( const TRect& aSecondaryRect );
        
        /**
        * Gets secondary rect.
        * @return TRect This session secondary drawing area rectangle
        */
        virtual TRect SecondaryRect() const;
            
        /**
        * Returns current display orientation.
        *
        * @pre Session is ongoing
        * @return Current display orientation 
        * @leave KErrNotReady if precondition is not fullfilled
        */
        TDisplayOrientation OrientationL();
        
        /**
        * Sets display orientation.
        *
        * @pre Session is ongoing
        * @return Sets display orientation 
        * @leave KErrNotReady if precondition is not fullfilled
        */
        void SetOrientationL( TDisplayOrientation aOrientation );

        /**
        * Permission to draw on screen device.
        * 
        * @pre Session is ongoing
        */
        void EnableDisplayL( TBool aEnable );
        
        /**
        * Implements virtual from MMusEngDisplayHandler
        *
        * @pre Session is ongoing
        */
        TBool IsDisplayEnabled();
        
        /**
        * Implements virtual from MMusEngDisplayHandler
        *
        * @pre Session is ongoing
        */
        TBool IsDisplayActive();

        
    public: // From MMusEngAudioRoutingObserver
        
        void AudioRoutingChanged();        
        
        
    public: // From MLcSession
        
        virtual TLcSessionState LcSessionState() const;
        
        void SetLcSessionObserver( MLcSessionObserver* aObserver );

        void SetLcUiProvider( MLcUiProvider* aUiProvider );        
        
        virtual MLcVideoPlayer* RemoteVideoPlayer();
    
        virtual MLcVideoPlayer* LocalVideoPlayer();       
       
        virtual const TDesC& LocalDisplayName();
    
        virtual const TDesC& RemoteDisplayName();

        TInt SetParameter( TInt aId, TInt aValue );

        TInt ParameterValue( TInt aId );    
        
        TBool IsBackgroundStartup();
        
        TInt SetForegroundStatus( TBool aIsForeground );
        
        const TDesC& RemoteDetails();
        
        TBool SendDialTone( TChar aKey);
        
        
    public: // From MLcAudioControl
        
        TBool IsLcAudioMutedL();

        void MuteLcAudioL( TBool aMute );       

        TBool IsLcMicMutedL();    

        void MuteLcMicL( TBool aMute );
        
        TBool IsEnablingLcLoudspeakerAllowed();

        void EnableLcLoudspeakerL( TBool aEnabled );
        
        TBool IsLcLoudspeakerEnabled();

        TInt LcVolumeL();
     
        void SetLcVolumeL( TInt aValue );    
    
        void IncreaseLcVolumeL();
        
        void DecreaseLcVolumeL();        

        
    protected: // CONSTRUCTORS

        /**
        * Constructor
        *
        */
        CMusEngMceSession();

        /**
        * Second-phase constructor
        */
        void ConstructL();
                            
    protected: // Data

        /**
        * Drawing area rect.
        */
        TRect iRect;
        
        /**
        * ETrue if operator specific behavior is expected
        */
        TBool iOperatorVariant;

        /**
        * Session state for MLcSession 
        */
        MLcSession::TLcSessionState iLcSessionState;       
                                
        TMusEngCameraHandler iCameraHandler;

        CMusEngLiveVideoPlayer* iLiveVideoPlayer;
        CMusEngRemoteVideoPlayer* iRemoteVideoPlayer;
        TRect iSetRemoteRect;
        TRect iSetLocalRect;
	CSVPTimer* iTimer;
    	TLcSessionState lLcSessionState;
	HBufC* iRemoteAddress;
	CMccScreen* iScreen;

    private: // Data

        /**
        * The session observer, if present.
        */
        MLcSessionObserver* iLcSessionObserver;

        /**
        * The UI provider, if present.
        */        
        MLcUiProvider* iLcUiProvider;        
                
        /**
        * Value indicates whether session was started in background mode.
        * (see IsBackgroundStartup()). Once sending of video started, 
        * iBackground is set to EFalse
        */
        TBool iBackground;
        
        // indicator control
        LcVtIndicatorController* iIndicatorCtr;


    };
    
#endif // MUSENGMCESESSION_H
