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
#include "musengsessiondurationtimerobserver.h"
#include "musunittesting.h"
#include "mussipprofileuser.h"
#include "musengdisplayhandler.h"
#include "musengaudioroutingobserver.h"

// SYSTEM
#include <lcsession.h>
#include <lcaudiocontrol.h>
#include <mcesessionobserver.h>
#include <mceinsessionobserver.h>
#include <mcestreamobserver.h>
#include <mcertpobserver.h>
#include <mcetransactiondatacontainer.h>
#include <mcemediasource.h>

// FORWARD DECLARATIONS
class CMceManager;
class CMceSession;
class CMceVideoStream;
class CMceAudioStream;
class CMceVideoCodec;
class CMceAudioCodec;
class CMusEngSessionDurationTimer;
class CMusSipProfileHandler;
class CMusEngTelephoneUtils;
class MMusEngAudioRoutingObserver;
class MLcSessionObserver;

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

// Value for uninitialized rect
const TInt KMusEngRectNotInit = 1000;

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
                          public MMceSessionObserver,
                          public MMceInSessionObserver,
                          public MMceStreamObserver,
                          public MMceRtpObserver,
                          public MMusEngSessionDurationTimerObserver,
                          public MMusSipProfileUser,
                          public MMusEngDisplayHandler,
                          public MMusEngAudioRoutingObserver
    {
    public:

        /**
        * Destructor
        */
        ~CMusEngMceSession();

        
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
        
        void UpdateLcSessionL();
        
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

    public:
        
        MLcSessionObserver* LcSessionObserver();
        
        MLcUiProvider* LcUiProvider();
        
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
        

    protected: // VIRTUAL HELPER FUNCTIONS

        /**
        * Function handling MCE session state changes.
        * To be overridden in sibling classes if needed.
        */
        virtual void HandleSessionStateChanged( CMceSession& aSession,
                                                TInt aStatusCode,
                                                const TDesC8& aReasonPhrase );
                        
        /**
        * Function handling MCE session termination.
        */
        virtual void HandleTermination( TInt aStatusCode,
                                        const TDesC8& aReasonPhrase );

        /**
        * Sets Multimediasharing specific video codec settings. This functions 
        * gets called for every video codec in session. To be overridden in 
        * sibling classes if needed.
        */ 
        virtual void AdjustVideoCodecL( CMceVideoCodec& aVideoCodec,
                                        TMceSourceType aSourceType );
        
        /**
        * Sets Multimediasharing specific audio codec settings like audio MMF 
        * priority and preference. This functions gets called for every audio 
        * codec in session. To be overridden in sibling classes if needed.
        */ 
        virtual void AdjustAudioCodecL( CMceAudioCodec& aAudioCodec );

       /**
        * Removes multiples of H.263 codec, prefers H263-2000 over H263-1998.
        * Additionally selects just the one with best quality from selected mode.
        * @param aVideoStream
        */
        virtual void DoCodecSelectionL( CMceVideoStream& aVideoStream );
        
        virtual void RectChangedL();
        
    protected: // HELPER FUNCTIONS

        void InformObserverAboutSessionStateChange();
        
        void InformObserverAboutSessionUpdate();
        
        void InformObserverAboutSessionFailure( TInt aReason );
        
        void InformObserverAboutPlayerStateChange( MLcVideoPlayer* aPlayer );
        
        void InformObserverAboutPlayerUpdate( MLcVideoPlayer* aPlayer );
        
        void InformObserverAboutPlayerFailure( 
            MLcVideoPlayer* aPlayer, 
            TInt aReason );
        
        void InformUiProviderAboutReceivingStart();
        
        MLcUiProvider& LcUiProviderL();
  
        /**
        * Sets session specific SDP lines to a session.
        */
        void SetSessionSdpLinesL( CMceSession& aSession, 
                                  TBool aForceBandwidthLine = EFalse );
        /**
        * Sets media specific SDP lines to media.
        */
        void SetMediaSdpLinesL( CMceMediaStream& aStream, 
                                TBool aForceBandwidthLine = EFalse );
        
        /**
        * Sets the volume level of all speakers in session structure.
        * @param aNewVolume New volume level for all speakers
        */
        void SetSpeakerVolumeL( TInt aNewVolume );

        /**
        * Adjusts settings of all streams. Calls AdjustVideoStreamL for every
        * video stream and AdjustAudioStreamL for every audio stream in session.
        */ 
        void AdjustStreamsAndCodecsL();

        /**
        * Adjusts stream settings. Calls AdjustVideoCodecL for every video
        * codec in stream.
        */ 
        void AdjustVideoStreamL( CMceVideoStream& aVideoStream );

        /**
        * Adjusts stream settings. Calls AdjustAudioCodecL for every audio 
        * codec in stream.
        */ 
        void AdjustAudioStreamL( CMceAudioStream& aAudioStream );

        /*
        * Calls CMceInSession::RejectL() inside TRAP_IGNORE
        */
        void Reject( CMceInSession& aSession,
                     const TDesC8& aReason = KNullDesC8(),
                     TUint32 aCode = 0 );
        
        void SaveContactL( const TDesC8& aAddress );
            
    protected: // from MMceInSessionObserver
    
        /**
        * Incoming session invitation. The CMceInSession is given to the
        * application as a callback. This default implementation rejects
        * session immediately.
        * @param aSession, pointer to the incoming session. Ownership is
        *	     transferred.
        * @param aContainer, if present, holds details of
        *        transaction causing state change.
        */
        void IncomingSession( CMceInSession* aSession,
                              TMceTransactionDataContainer* aContainer );


        /**
        * Incoming session update. The new updated CMceInSession is given to the
        * application as a callback. This default implementation rejects
        * session immediately.
        * @param aOrigSession, the original session to be updated. This instance
        *        cannot be used anymore, all actions done using aUpdatedSession 
        *        instance.
        * @param aUpdatedSession, pointer to the new updated session. Ownership
        *        is transferred.
        * @param aContainer, if present, holds details of
        *        update transaction.
        */
        void IncomingUpdate( CMceSession& aOrigSession, 
                             CMceInSession* aUpdatedSession,
                             TMceTransactionDataContainer* aContainer );
    
    
    protected: // from MMceStreamObserver  

        /**
        * The state of the stream has changed.
        *
        * @param aStream, the stream that has changed.
        */
        void StreamStateChanged( CMceMediaStream& aStream );
        
        /**
        * The state of the source has changed.
        *
        * @param aStream, the stream that uses the source.
        * @param aSource, the source that has changed.
        */
        void StreamStateChanged( CMceMediaStream& aStream,
                                 CMceMediaSource& aSource );

        /**
        * The state of the sink has changed.
        *
        * @param aStream, the stream that uses the sink.
        * @param aSink, the sink that has changed.
        */
        void StreamStateChanged( CMceMediaStream& aStream,
                                 CMceMediaSink& aSink );    
    
    
    private: // from MMceSessionObserver
    
        /**
        * The state of the session has changed.
        *
        * @param aSession, the session that has changed.
        * @param aContainer, if present, holds details of
        *        transaction causing state change.
        */
        void SessionStateChanged( CMceSession& aSession,
                                  TMceTransactionDataContainer* aContainer );
                                  
        /**
        * The state of the connection used by the session has changed.
        *
        * @param aSession, the session that has changed.
        * @param aActive, ETrue if connection active, EFalse if
        *                 connection inactive.
        */
        void SessionConnectionStateChanged( CMceSession& aSession,
                                            TBool aActive );

        /**
        * An error has occurred concerning a specific session.
        * Note, that each error causes the session state to be ETerminated.
        *
        * @param aSession, The session raising the error.
        * @param aError, Error code
        */
        void Failed( CMceSession& aSession, TInt aError );

        /**
        * An session update has been failed concerning a specific session.
        * Note, that each error causes the session state to be ETerminated.
        *
        * @param aSession, The session raising the error.
        * @param aContainer, if present, holds details of
        *        transaction causing update.
        */
        void UpdateFailed( CMceSession& aSession,
                           TMceTransactionDataContainer* aContainer );


    private: // from MMCERtpObserver

        /**
        * Callback function to indicate that an RTCP sender report
        * has been received.
        * @param aSession the session that has changed
        * @param aStream the stream that has changed
        */
        void SRReceived( CMceSession& aSession, CMceMediaStream& aStream);

        /**
        * Callback function to indicate that an RTCP receiver report
        * has been received.
        * @param aSession the session that has changed
        * @param aStream the stream that has changed
        */
        void RRReceived( CMceSession& aSession, CMceMediaStream& aStream);
					
        /**
        * Callback function to indicate inactivity timer timeout
        * has occurred. Timer is disabled after the timeout.
        *
        * @param aStream, the session that has changed.
        * @param aSource, the source that the timeout occurred.
        */
        void  InactivityTimeout( CMceMediaStream& aStream,
                                 CMceRtpSource& aSource );

        /**
        * Callback function to indicate that spesified CMceRtpSource has
        * received RTP from a new synchronization source (SSRC).
        * @param aStream the stream that new SSRC was added to
        * @param aSource the source that new SSRC was added to
        * @param aSsrc added SSRC identifier
        */			
        void SsrcAdded( CMceMediaStream& aStream,
                        CMceRtpSource& aSource,
                        TUint aSsrc );       
	    
        /**
        * Callback function to indicate that spesified CMceRtpSource has
        * received RTP BYE message from a spesified synchronization 
        * source (SSRC).
        * @param aStream the stream that SSRC was removed from
        * @param aSource the source that SSRC was removed from
        * @param aSsrc removed SSRC identifier
        */	
        void SsrcRemoved( CMceMediaStream& aStream,
                          CMceRtpSource& aSource,
                          TUint aSsrc );       
    
        
    private: // from MMusSipProfileUser
        
        virtual TBool IsRoamingBetweenAPsAllowed();
        
        virtual void ProfileRegistered();
        
        
    private: // HELPER FUNCTIONS

        /**
        * Adds b=AS and b=TIAS lines to the array
        */
        void AddSdpBandwidthAttributesL( CDesC8Array& aSdpLinesArray,
                                         TInt aBandwidthAs );

        /**
        * Checks whether AS or TIAS bandwidth attributes present in the array 
        */
        TBool IsBandwidthAttributeSet( MDesC8Array* aSdpLinesArray );


        /**
        * Callback
        */
        void UpdateTimerEvent();

        /**
        * Does actual muting or unmuting of local audio playback but does not
        * mark speaker as explicitly muted so this function can be called also
        * when muting speaker as a consequence of disabling or enabling 
        * bundled display sink.
        *
        */
        void DoMuteSpeakerL( TBool aMute );
        
        /**
        * Checks if some of array items contains specific text.
        * @param aArray
        * @param aItem
        * @return ETrue if array contains aItem
        */
        TBool ContainsText( MDesC8Array* aArray, const TDesC8& aItem );
        
        void DoBitrateBasedRemovalL( CMceVideoStream& aVideoStream, 
                                     const CMceVideoCodec& aBestBitrateVideoCodec );
                                     
        void DoCodecModeBasedRemovalL( CMceVideoStream& aVideoStream );
  
        /**
        * Get session time return estabilished session time
        * @return TTime returns time if connection established else < 0
        */
        TTimeIntervalSeconds GetSessionTime() const;        
        
    protected:
        
        /**
        * Reads from CenRep and sets encoding device for a codec.
        */
        void SetEncodingDeviceL( CMceVideoCodec& aVideoCodec );

        /**
        * Sets configuration key for a codec
        */
        void SetCodecConfigKeyL( CMceVideoCodec& aVideoCodec );
    
        /**
        * Reads proper configuration key for a codec. NULL if not available.
        * Ownership is transferred.
        */
        HBufC8* ReadCodecConfigKeyL( const CMceVideoCodec& aVideoCodec ) const;
        
        /**
        * Constructs and stores to cenrep a concatenation of all AVC config keys.
        */
        void StoreEncoderConfigInfoL() const;
        
        /**
        * Returns config key id for the provided codec. Ownership is transferred
        */
        const TPtrC8 ConfigKeyIdL( const CMceVideoCodec& aVideoCodec ) const;

            
    protected: // Data

        /**
        * Drawing area rect.
        */
        TRect iRect;

        /**
        * Telephone utilities.
        */
        CMusEngTelephoneUtils* iTelephoneUtils;        
        
        /**
         * UID used to identify application, needed to create MCE manager
         */
        TUid iMceManagerUid;
        
        /**
        * Class for connecting to MCE server.
        * This class provides the connection to the MCE server and
        * provides functions
        * for setting observers for asynchronous events.
        * The user should create only one instance of this class.
        */
        CMceManager* iManager;

        /**
        * Base class for MCE sessions, inherited by both CMCEInSession and
        * CMCEOutSession. * CMCESession represents peer-to-peer connection
        * made with remote terminal. This connection is also known as dialog
        * and it is set up using SIP INVITE method. The characteristics of the
        * session are defined by user of this class by attaching media streams
        * into the session. After the session has been succesfully established,
        * it can be later updated and must be finally terminated. If for any
        * reason instance of this class is deleted while in established state,
        * session is automatically terminated by MCE server.
        */
        CMceSession* iSession;

        /**
        * ETrue if operator specific behavior is expected
        */
        TBool iOperatorVariant;

        /**
        * The handler for the SIP profile related to this session
        */
        CMusSipProfileHandler* iSipProfileHandler;

        /**
        * Session state for MLcSession 
        */
        MLcSession::TLcSessionState iLcSessionState;       
        
        /**
        * If true, configuration key of current AVC codec must be written to
        * CenRep after session establishment.
        */
        TBool iStoreEncoderConfigInfo;
        
        /**
        * Remote details 
        */
        HBufC* iRemoteDetails;
        
    private: // New functions
        
        void StreamStreaming( CMceMediaStream& aStream );
                
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
        * It is assumed, that user has only one instance of this class.
        * The same instance is passed between MCE and user, but the data
        * inside of this container is just updated for each event.
        */
        TMceTransactionDataContainer iTransactionDataContainer;

        /**
        * Session time. Acts simultaneously as RTCP inactivity timer.
        */
        TTime iStartTime;

        CMusEngSessionDurationTimer* iUpdateTimer;
        
        /**
        * Value indicating seconds gone since last received RTCP sender or 
        * receiver report. Value 20 indicates RTCP inactivity timeout.  
        */
        TInt iSecondsFromLastRtcpReport;

        /**
        * Value indicating if speaker is explicitly muted instead of
        * muted as part of disabling bundled display sink 
        */
        TBool iExplicitlyMuted;
        
        /**
        * Value indicates whether session was started in background mode.
        * (see IsBackgroundStartup()). Once application comes to foreground, 
        * iBackground is set to EFalse
        */
        TBool iBackground;
        
    MUS_UNITTEST( UT_CMusEngSession )
    MUS_UNITTEST( UT_CMusEngMceSession )
    MUS_UNITTEST( UT_CMusEngReceiveSession )
    MUS_UNITTEST( UT_CMusEngSessionManager )
    };
    
#endif // MUSENGMCESESSION_H
