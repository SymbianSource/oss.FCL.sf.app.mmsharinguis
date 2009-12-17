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


#ifndef MUSHENGMCESESSION_H
#define MUSHENGMCESESSION_H

// USER
#include "musengsession.h"
#include "musengsessiondurationtimerobserver.h"
#include "musunittesting.h"
#include "mussipprofileuser.h"

// SYSTEM
#include <mcesessionobserver.h>
#include <mceinsessionobserver.h>
#include <mcestreamobserver.h>
#include <mcertpobserver.h>
#include <mcetransactiondatacontainer.h>

class CMceManager;
class CMceSession;
class CMceVideoStream;
class CMceAudioStream;
class CMceVideoCodec;
class CMceAudioCodec;
class CMusEngSessionDurationTimer;
class MMusEngSessionObserver;
class CMusSipProfileHandler;

// CONSTANTS

_LIT8( KMusEngAcceptContactHeader, 
       "Accept-Contact: *;+g.3gpp.cs-voice; explicit" );
_LIT8( KMusAcceptHeader, 
       "Accept: application/sdp" );
_LIT8( KMusEngSessionSdpLineXApplication, "a=X-application:com.nokia.rtvs\r\n" );
_LIT8( KMusEngSessionSdpLineApplication , "a=application:com.gsma.rts\r\n" );
_LIT8( KMusEngSessionSdpLineType, "a=type:videolive\r\n" );
_LIT8( KMusEngSessionSdpLineBandwidthLine  , "b=AS" );
_LIT8( KMusEngSessionSdpLineBandwidthField  , "b=AS:85\r\n" );
_LIT8( KMusPPreferredIdentity, "P-Preferred-Identity" );
_LIT8( KMusPrivacyHeader, "Privacy" );
_LIT8( KMusAnonymousHeader, "\"Anonymous\" <sip:anonymous@anonymous.invalid>" );

const TInt KMusEngMaxVolume = 10;
const TInt KMusEngMinVolume = 0;

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
class CMusEngMceSession : public CMusEngSession,
                          public MMceSessionObserver,
                          public MMceInSessionObserver,
                          public MMceStreamObserver,
                          public MMceRtpObserver,
                          public MMusEngSessionDurationTimerObserver,
                          public MMusSipProfileUser
    {
    
    public: 
    
        /*
        * Defines possible rotations 
        */
        enum TDisplayOrientation
            {
            EPortrait, // Normal
            ELandscape // 90 degree's clockwise rotation
            };
        
    public:

        /**
        * Destructor
        */
        ~CMusEngMceSession();


    public: // API FUNCTIONS

        /**
        * Terminates session. 
        *
        * @pre Session is established
        * @post Session is ready to be deleted
        * @leave KErrNotReady if precondition is not fullfilled
        */
        IMPORT_C void TerminateL();

        /**
        * Get session time return estabilished session time
        *
        * @return TTime returns time if connection established else < 0
        */
        IMPORT_C TTimeIntervalSeconds GetSessionTime() const;

        /**
        * Returns current connection state
        *
        * @return TBool returns current connection state
        */
        IMPORT_C TBool ConnectionActive() const;

        /**
        * Returns presence or absence of audio in session.
        *
        * @pre Session is ongoing
        * @return ETrue if session contains audio.
        * @leave KErrNotReady if precondition is not fullfilled
        */
        IMPORT_C TBool ContainsAudioL();
        
        /**
        * Returns state of local playback meaning if playback is muted or not.
        *
        * @pre Session is ongoing
        * @return ETrue if session does not contain audio or contained 
        *         audio is muted.
        * @leave KErrNotReady if precondition is not fullfilled
        */
        IMPORT_C TBool IsMutedL();

        /**
        * Returns current display orientation.
        *
        * @pre Session is ongoing
        * @return Current display orientation 
        * @leave KErrNotReady if precondition is not fullfilled
        */
        IMPORT_C TDisplayOrientation OrientationL();
        
        /**
        * Sets display orientation.
        *
        * @pre Session is ongoing
        * @return Sets display orientation 
        * @leave KErrNotReady if precondition is not fullfilled
        */
        IMPORT_C void SetOrientationL( TDisplayOrientation aOrientation );
        

    public: // VIRTUAL API

        // No virtual api


    public: //  from CMusEngSession  

        /**
        * Increases volume level by one
        *
        * @pre Session is ongoing
        */
        IMPORT_C void VolumeUpL();

        /**
        * Decreases volume level by one
        *
        * @pre Session is ongoing
        */
        IMPORT_C void VolumeDownL();
        
        /**
        * Set volume value
        * @pre Session is ongoing
        * @param aVal the value of volume 
        */
        IMPORT_C void SetVolumeL( TInt aVal );

        /**
        * Permission to draw on screen device.
        * 
        * @pre Session is ongoing
        */
        IMPORT_C void EnableDisplayL( TBool aEnable );

        /**
        * Implements virtual from CMusEngSession
        *
        * @pre Session is ongoing
        */
        IMPORT_C void MuteL();

        /**
        * Implements virtual from CMusEngSession
        *
        * @pre Session is ongoing
        */
        IMPORT_C void UnmuteL();
        

    protected: // CONSTRUCTORS

        /**
        * Constructor
        *
        */
        CMusEngMceSession( const TRect& aRect,
                           MMusEngSessionObserver& aSessionObserver );

        /**
        * Second-phase constructor
        *
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
        virtual void AdjustVideoCodecL( CMceVideoCodec& aVideoCodec );
        
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
        
        
    protected: // HELPER FUNCTIONS

        /**
        *
        */
        void RectChangedL();
  
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
        
        TBool IsRoamingBetweenAPsAllowed();
      			
    private: // HELPER FUNCTIONS

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
         * This function currently used to remove the AVC codec support based on
         * cenrep encoder device uid value.
         */
        void DoCodecConfigurationBasedRemovalL( CMceVideoStream& aVideoStream );


    protected: // MEMBERS

        /**
        * Callback reference to session observer interface.
        */
        MMusEngSessionObserver& iSessionObserver;
        
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

    private:

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

        CMusEngSessionDurationTimer*   iUpdateTimer;
        
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


    MUS_UNITTEST( UT_CMusEngSession )
    MUS_UNITTEST( UT_CMusEngMceSession )
    MUS_UNITTEST( UT_CMusEngReceiveSession )
    };
    
#endif //MUSHENGMCESESSION_H
