/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSHENGREVEIVESESSION_H
#define MUSHENGREVEIVESESSION_H

// INCLUDES
#include "musengmcesession.h"
#include "musenguriparser.h"
#include "musunittesting.h"
#include <mcemediasink.h>

// FORWARD DECLARATIONS
class MMusEngReceiveSessionObserver;
class CMceInSession;
class CMusEngRemoteVideoPlayer;


class CMusEngReceiveSession : 
    public CMusEngMceSession
    {
    MUS_UNITTEST( UT_CMusEngMceSession )
    MUS_UNITTEST( UT_CMusEngReceiveSession )
    
    public: // Contructors and destructor

       /**
        * Creates new MultimediaSharing Receive session.
        * @return CMusEngReceiveSession* New instanse of specified class
        */
        static CMusEngReceiveSession* NewL();
        
       /**
        * Destructor
        *
        * @since S60 v3.2
        */
        ~CMusEngReceiveSession();
   
    public: // From MLcSession
        
        TLcSessionState LcSessionState() const;
        
        void EstablishLcSessionL();
        
        void TerminateLcSessionL(); 
        
        MLcVideoPlayer* RemoteVideoPlayer();    
        
        const TDesC& RemoteDisplayName();         
        
    protected: // Overrides function in CMusEngMceSession
    
        /**
        * The state of the session has changed.
        *
        * @since S60 v3.2
        * @param aSession, the session that has changed.
        */
        virtual void HandleSessionStateChanged( CMceSession& aSession,
                                                TInt aStatusCode,
                                                const TDesC8& aReasonPhrase );
        
        /**
        * Sets Multimediasharing specific video codec settings like keepalive
        * timer. This functions gets called for every video codec in 
        * session. This function overrides function in base class. 
        * Function calls also overridden version.
        */ 
        virtual void AdjustVideoCodecL( CMceVideoCodec& aVideoCodec,
                                        TMceSourceType aSourceType );
        
        /**
        * Sets Multimediasharing specific audio codec settings like keepalive
        * timer. This functions gets called for every audio 
        * codec in session.  This function overrides function in base class. 
        * Function calls also overridden version.
        */ 
        virtual void AdjustAudioCodecL( CMceAudioCodec& aAudioCodec );
        
        /**
        * Removes multiples of H.263 codec, prefers H263-2000 over H263-1998.
        * Additionally selects just the one with best quality from selected mode.
        * Removes all the H.263 codecs if any AVC codecs found. Prefers
        * non-interleaved AVC over single NAL AVC
        * @param aVideoStream
        */
        virtual void DoCodecSelectionL( CMceVideoStream& aVideoStream );
        
    protected: // from MMceInSessionObserver
               // overrides the function in CMusEngMceSession

        /**
	    * Incoming session invitation. The CMCEInSession is given to the
		* application as a callback.
	    * @param aSession, pointer to the incoming session. Ownership is
		*	     transferred.
	    * @param aContainer, if present, holds details of
	    *        transaction causing state change.
	    */
        void IncomingSession(
	                CMceInSession* aSession,
    				TMceTransactionDataContainer* aContainer );
    				
    				
	    /**
	    * Incoming session update. The new updated CMCEInSession is given to the
		* application as a callback.
	    * @param aOrigSession, the original session to be updated. This instance
	    *        cannot be used anymore, all actions done using aUpdatedSession 
	    *        instance.
	    * @param aUpdatedSession, pointer to the new updated session. Ownership 
	    *        is transferred.
	    * @param aContainer, if present, holds details of update transaction.
	    */
		void IncomingUpdate(
					CMceSession& aOrigSession, 
					CMceInSession* aUpdatedSession,
    				TMceTransactionDataContainer* aContainer );


    protected: // from MMceStreamObserver, 
             // overrides the function in CMusEngMceSession

        /**
        * The state of the stream has changed.
        *
        * @since S60 v3.2
        * @param aStream, the stream that uses the source.
        */
        void StreamStateChanged( CMceMediaStream& aStream );
    
    protected: // from MMceRtpObserver

        void InactivityTimeout( CMceMediaStream& aStream,
                                CMceRtpSource& aSource );



    protected: // CONSTRUCTORS
    
        CMusEngReceiveSession();

		void ConstructL();
		
    protected: // HELPERS
    
        CMceInSession* InSession();
        
        void PrepareToRequire100RelL( 
            TMceTransactionDataContainer* aContainer );
        
        virtual void CompleteSessionStructureL();
        
         /**
         * Parse P-Asserted-Identity header. 
         * Tries to fetch the telephone number from tel uri, 
         * if tel uri not found it suppose that telephone number
         * is in sip url. 
         */
        void ParseAssertedIdentity( TMceTransactionDataContainer* aContainer );
        /*
         * Checks for buffering event happened & receiving already not started
         *      If yes, start the RTP inactivity timer and indicate to observer
         *              about videoplayer state change.
         *      else will not do anything. Bascially ignores the event.
         */
        void ReceivingStarted();
        
        /**
        * Checks for receiving already started and display sink is enabled
        *        If yes ETrue else otherwise.
        */
        TBool IsDisplayActive();
        
    protected: // DATA
    
        // Set to ETrue if CMceInSession::RingL has already been called
        TBool iRingLCalled;
        
        // identity of originator parsed form P-Asserted-Identity field
        TBuf8<KMaxUriLength> iIdentity;
        
        HBufC8* iOriginator;
        
        HBufC* iRemoteDisplayName;
       
        /// The remote video player implementing MLcVideoPlayer
        CMusEngRemoteVideoPlayer* iRemoteVideoPlayer;
        
        TUint32 iReceivingInactivityTimeout;
        
        TUint8 iKeepaliveTimer;
        
    private:
        /* Flag for whether receiving started. It will go EFalse
         * when RTP inactivity timeout happen.
         */
        TBool iReceiving;               
        /* Flag to indicate buffering event happened. This flag is for
         * optimization purpose since buffering event is the only place 
         * we can be sure about some packets were received.
         */
        TBool iBuffered;
    };

#endif

