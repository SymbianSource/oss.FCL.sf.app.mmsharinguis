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


#ifndef CMCESESSION_H
#define CMCESESSION_H

//  INCLUDES
#include <mcedefs.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class CDesC8Array;
class TMceIds;
class CMceMediaStream;
class CMceManager;
class TMceFactory;
class RReadStream;
class RWriteStream;
class CMceItcSender;
class CMceSessionReceiver;
class MMceFcMsgObserver;
class CMceComSession;
class CMceMsgBase;
class TMceEvent;
class CMceInSession;
class MDesC8Array;
class CMceStreamBundle;


// CLASS DECLARATION

/**
* Base class for MCE sessions, inherited by both CMceInSession and CMceOutSession.
*
* CMceSession represents peer-to-peer connection made with remote terminal.
* This connection is also known as dialog and it is set up using SIP INVITE
* method.
*
* The characteristics of the session are defined by user of this class by attaching
* media streams into the session.
*
* After the session has been succesfylly established, it can be later updated and
* must be finally terminated.
*
* If for any reason instance of this class is deleted while in established state,
* session is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class CMceSession : public CBase
	{	
	public: // Session states

	    enum TState
	        {
	        EIdle, 			// Idle
	        EOffering,      // Outgoing session, or session update establishment in progress, 
	        EIncoming,		// Session received incoming call
	        EReserving,     // UpdateL called to incoming session during session establishment
	        EAnswering, 	// Answering an incoming call
	        EProceeding,	// Session media negotiations proceeding
	        EEstablished,   // Session established
	        ECancelling,    // Cancelling outgoing session establishment
	        ETerminating,   // Terminating session
	        ETerminated,    // Session terminated
	        };

	public: // Session termination reasons

	    enum TReason
	        {
	        EDeclined,
	        EBusy,
	        ECancelled,
	        EDisconnected
	        };

	public: 

	    /**
	    * Destructor.
	    */
		virtual ~CMceSession();

	public: // Functions

	    /**
	    * Returns the state of the session.
	    * @return The current state of the session
	    */
	    IMPORT_C TState State() const;

	    /**
	    * Returns the state of network connection of the session.
	    * @return ETrue if connection active, EFalse if connection inactive.
	    */
	    IMPORT_C TBool ConnectionActive() const;


        /**
        * Removes media stream from session.
        * @pre State() == CMceSession::EIdle ||
        *      State() == CMceSession::EIncoming ||
        *      State() == CMceSession::EEstablished
        * @post aMediaStream cannot be used anymore and contents of 
        *       previously returned stream array (CMceSession::Streams())
        *       are not anymore valid
        * @param aMediaStream media stream to be removed
        */
        IMPORT_C void RemoveStreamL( CMceMediaStream& aMediaStream );
	    
	    /**
	    * Adds media stream into session.
		* @pre State() == CMceSession::EIdle ||
		*      State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
		* @pre aMediaStream != 0
	    * @param aMediaStream, media stream to be added.
	    *		 ownership is transferred.
	    */
	    virtual void AddStreamL( CMceMediaStream* aMediaStream );

	    /**
		* Updates offered session parameters during session
		* establishement phase for first answer.
	    * Updates established session using re-invite.
	    * In state EIncoming the timeout value must be smaller or equal to
	    * offered value obtained by SessionTimer().
		* @pre State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
		* @post State() == CMceSession::EReserving for incoming session ||
		*       State() == CMceSession::EOffering for established session
	    * @param aTimeout, the session timer value in seconds
	    * @param aHeaders, SIP headers to be added. Ownership is
		*		 transferred.
	    * @param aContentType, content type of the body.
		*		 Ownership is transferred.
	    * @param aContent, content to be added in body. Ownership is 
		*		 transferred.
	    * @param aContentHeaders, content headers to be added. Ownership is
		*		 transferred.
	    */
		IMPORT_C void UpdateL(
					 TUint32 aTimeout = 0,
					 CDesC8Array* aHeaders = 0, 
					 HBufC8* aContentType = 0,
					 HBufC8* aContent = 0,
					 CDesC8Array* aContentHeaders = 0 ); 

        /**
	    * Terminates established session.
		* @pre State() == CMceSession::EEstablished
		* @post State() == CMceSession::ETerminating
	    * @param aHeaders, SIP headers to be added. Ownership is
		*		 transferred.
	    * @param aContentType, content type of the body.
		*		 Ownership is transferred.
	    * @param aContent, content to be added in body. Ownership is 
		*		 transferred.
	    */
		IMPORT_C void TerminateL( CDesC8Array* aHeaders = 0,
					              HBufC8* aContentType = 0,
					              HBufC8* aContent = 0 ); 

	    /**
	    * Returns the streams belonging to the session.
	    * @return streams of the session.
	    */
		IMPORT_C const RPointerArray<CMceMediaStream>& Streams() const;

	    /**
	    * Returns the recipient of the session.
		* @pre State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
	    * @return The recipients address.
	    */
	    IMPORT_C const TDesC8& Recipient() const;

	    /**
	    * Returns the originator of the session.
		* @pre State() == CMceSession::EIdle ||
		*      State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
	    * @return the originators address.
	    */
	    IMPORT_C const TDesC8& Originator() const;

	    /**
		* Configures session by updating SDP session lines. 
		* In order to get complete update, whole session must be 
		* updated causing sending of re-invite.
		* @pre State() == CMceSession::EIdle ||
		*      State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
		* @pre aSessionSDPLines != 0
	    * @param aSessionSDPLines, application specific SDP lines for session.
		*		 Ownership is transferred.
	    */
		IMPORT_C void SetSessionSDPLinesL( CDesC8Array* aSessionSDPLines );
		
        /**
        * Gets SDP lines of the session.
        * @return array of session SDP lines, owneship is transferred.
        */
        IMPORT_C MDesC8Array* SessionSDPLinesL();
		
		/**
	    * Returns the session timer value of the session.
		* @return the session timer value.
	    */
		IMPORT_C TUint32 SessionTimer() const;	    

        /**
        * Adds grouped media streams to the session. Changes SDP
        * of the session. In order to get complete update, whole
        * session must be updated causing sending of re-invite.
        * @pre State() == CMceSession::EIdle ||
        *      State() == CMceSession::EIncoming ||
        *      State() == CMceSession::EEstablished
        * @pre aBundle != 0
        * @param aBundle group of media streams;
        *        ownership of the bundle is transferred
        */
        IMPORT_C void AddBundleL( CMceStreamBundle* aBundle );

        /**
        * Removes grouped media stream from session. Changes SDP
        * of the session. In order to get complete update, whole
        * session must be updated causing sending of re-invite.
        * @pre State() == CMceSession::EIdle ||
        *      State() == CMceSession::EIncoming ||
        *      State() == CMceSession::EEstablished
        * @post aBundle cannot be used anymore and contents of 
        *       previously returned bundle array (CMceSession::Bundles())
        *       are not anymore valid
        * @param aBundle bundle to be removed
        */
        IMPORT_C void RemoveBundleL( CMceStreamBundle& aBundle );

        /**
        * Gets the grouped media streams of the session.
        * @return bundles of the session
        */
        IMPORT_C const RPointerArray< CMceStreamBundle >& Bundles() const;

        /**
        * Gets session modifier of type TUint.
        * @param aModifier is a modifier to be retrieved
        * @param aValue on return will contain modifier value
        * @leave KErrNotFound if modifier is not found
        */
        IMPORT_C void GetModifierL( TMceSessionModifier aModifier,
                                    TUint& aValue ) const;

        /**
        * Sets session modifier of type TUint.
        * @param aModifier is a modifier to be set 
        * @param aValue is a new value of a modifier
        * @leave KErrNotFound if modifier is not found
        */
        IMPORT_C void SetModifierL( TMceSessionModifier aModifier, 
                                    TUint aValue );  
                                         
        /**
         * Returns the identifier of profile used by the sesison.
         * @return TUint32 Identifier of the used profile. 0, 
         *         if session is setup without profile.
         */
        IMPORT_C TUint32 Profile() const;
        
        
    public:
        
        CMceManager& Manager() const;
        
        // Restarts the simulator
        void RestartSimulator();

	protected:

	    /**
	     * C++ default constructor.
	     * @param aManager, reference to session manager.
	     * @param aProfileId, id of the sip profile used for the session.
	     */
		CMceSession( CMceManager* aManager, TUint32 aProfileId );

	    /**
	     * second-phase constructor
	     */
		void ConstructL();
	    
        /**
        * Initializes 
        */
        void InitializeL();


	protected: // Not owned data

	    /**
	     * profile id
	     */
	     TUint32 iProfileId;
		 
	private: // Owned data

	    /**
	     * media streams
	     */
		RPointerArray<CMceMediaStream> iMediaStreams;

        /**
	     * bundles
	     */
		RPointerArray<CMceStreamBundle> iBundles;
		
		//for testing

		//friend class CMceManager;
	
	public: // stub data

	    CMceManager* iManager;
	
	    CMceSession::TState iState;
	        
	    TBool iIsConnectionActive;
	    
	    HBufC8* iOriginator;
	    
	    HBufC8* iRecipient;
	    
	    TUint32 iTimeout;
	    
	    CDesC8Array* iSessionSDPLines;
	    
	    CDesC8Array* iHeaders;
	    
	    TBool iForceInternetSignaling;
	    
	    TUint i100RelUsage;
	    
	    // If this code is != KErrNone, next operation that can fail will fail
	    // (leave or return a value) with this code. Variable is reseted before 
	    // failing. 
	    // Obs! Check that wanted failing is really implemented in this stub
	    TInt iFailWithCode;
	    
	    TBuf8<200> iReasonPhrase;
	    
	    TInt iReasonCode;
	};

#endif
