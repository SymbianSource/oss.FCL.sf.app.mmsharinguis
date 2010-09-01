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



#include "musengmcesession.h"
#include "musenguriparser.h"
#include "musunittesting.h"

#include <mcemediasink.h>

class MMusEngReceiveSessionObserver;
class CMceInSession;

class CMusEngReceiveSession : public CMusEngMceSession
    {
    MUS_UNITTEST( UT_CMusEngMceSession )
    MUS_UNITTEST( UT_CMusEngReceiveSession )
    
    public: // Contructors and destructor

       /**
        * Creates new MultimediaSharing Receive session.
        *
        * @param aRect UI drawing area. It is allowed handle from engine
        * @param aEngObserver Engine specific callbacks
        * @param aSessionObserver Session specific callbacks
        * @return CMusEngReceiveSession* New instanse of specified class
        */
        IMPORT_C static CMusEngReceiveSession* NewL( 
                    const TRect& aRect,
                    MMusEngSessionObserver& aSessionObserver,
                    MMusEngReceiveSessionObserver& aReceiveSessionObserver );
       /**
        * Destructor
        *
        * @since S60 v3.2
        */
        ~CMusEngReceiveSession();
      
        
    public: // API

       /**
        * Accept or deny processed invitation
        *
        * @param aAccept ETrue if session is to be accepted and EFalse if 
        *        to be rejected.
        */
        IMPORT_C void AcceptInvitationL(const TBool& aAccept);
   

    private: // Overrides function in CMusEngMceSession
    
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
        virtual void AdjustVideoCodecL( CMceVideoCodec& aVideoCodec );
        
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
                         
                         
    private: // from MMceInSessionObserver
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


    private: // from MMceStreamObserver, 
             // overrides the function in CMusEngMceSession

        /**
        * The state of the stream has changed.
        *
        * @since S60 v3.2
        * @param aStream, the stream that uses the source.
        */
        void StreamStateChanged( CMceMediaStream& aStream );


    private: // CONSTRUCTORS
    
        CMusEngReceiveSession(  
                        MMusEngSessionObserver& aSessionObserver,
                        MMusEngReceiveSessionObserver& aReceiveSessionObserver,
                        const TRect& aRect );

		void ConstructL();
		
    private: // HELPERS
    
        CMceInSession* InSession();
        
        void PrepareToRequire100RelL( 
                        TMceTransactionDataContainer* aContainer);
        
        void CompleteSessionStructureL();
        
         /**
         * Parse P-Asserted-Identity header. 
         * Tries to fetch the telephone number from tel uri, 
         * if tel uri not found it suppose that telephone number
         * is in sip url. 
         */
        void ParseAssertedIdentity(
                        TMceTransactionDataContainer* aContainer );
        
    private: // DATA
    
        MMusEngReceiveSessionObserver& iReceiveSessionObserver;
    
        // Set to ETrue if CMceInSession::RingL has already been called
        TBool iRingLCalled;
        
        // identity of originator parsed form P-Asserted-Identity field
        TBuf8<KMaxUriLength> iIdentity;
    };

#endif

