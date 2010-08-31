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


#ifndef CMCEOUTSESSION_H
#define CMCEOUTSESSION_H

//  INCLUDES
#include <e32std.h> 
#include <mcesession.h>

// FORWARD DECLARATIONS
class CSIPProfile;
class CMceMediaStream;
class CMceManager;
class CMceEvent;
class CMceRefer;

// CLASS DECLARATION

/**
* Class for mobile originated MCE sessions.
*
* CMceOutSession represents outgoing invitation to remote terminal to join in a session.
*
* The characteristics of the session are defined by user of this class by attaching
* media streams into the session. Theses charactereistics are turned by MCE server into SDP
* and attached into sent invitation request.
*
* After the session has been succesfylly established, it can be later updated and
* must be finally terminated.
*
* If for any reason instance of this class is deleted while in established state,
* session is automatically terminated by MCE server.
*
*  @lib mceclient.lib
*/
class CMceOutSession : public CMceSession
	{

	public: // Constructors and destructor

	    /**
	    * Two-phased constructor. Should be used when session
	    * is established into new logical connection to remote terminal.
	    * @param aManager, reference to session manager.
   	    * @param aProfile, sip profile used for the session.
	    * @param aRecipient, recipient of the session invitation.
	    * @param aOriginator, originator of the session invitation.
	    */
		IMPORT_C static CMceOutSession* NewL(
									   CMceManager& aManager,
									   CSIPProfile& aProfile,
									   const TDesC8& aRecipient,
									   HBufC8* aOriginator = 0);

	    /**
	    * Two-phased constructor. Should be used when session is associated
	    * to existing event. This guarantees that session is established with
	    * same remote terminal with the event.
	    * @param aEvent, reference to existing event that is associated
	    *        with this session.
	    */
		IMPORT_C static CMceOutSession* NewL(CMceEvent& aEvent);
									  
	    /**
	    * Two-phased constructor. Should be used when session is associated
	    * to existing refer. This guarantees that session is established with
	    * same remote terminal with the refer.
	    * @param aRefer, reference to existing refer that is associated
	    *        with this session.
	    */
		IMPORT_C static CMceOutSession* NewL(CMceRefer& aRefer);
	    
		/// Note: This function exists only in the stub
	    IMPORT_C static	CMceOutSession* NewL();
		
	    /**
	    * Destructor.
	    */
		IMPORT_C ~CMceOutSession();

	public: // from CMceSession

	    /**
	    * Adds media stream into session.
		* @pre State() == CMceSession::EIdle ||
		*      State() == CMceSession::EIncoming ||
		*      State() == CMceSession::EEstablished
		* @pre aMediaStream != 0
	    * @param aMediaStream, media stream to be added.
	    *		 ownership is transferred.
	    */
	    IMPORT_C void AddStreamL( CMceMediaStream* aMediaStream );
	    
	public: // Functions

	    /**
	    * Establishes a SIP session with the recipient.
	    * The streams in the stream array are offered in the
	    * SDP codec negotiation.
	    * @pre State() == CMceSession::EIdle
	    * @post State() == CMceSession::EOffering
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
		IMPORT_C void EstablishL(
					 TUint32 aTimeout = 0,
					 CDesC8Array* aHeaders = 0, 
					 HBufC8* aContentType = 0,
					 HBufC8* aContent = 0,
					 CDesC8Array* aContentHeaders = 0 ); 

		/**
	    * Cancels a previously initialized MO session.
	    * @pre State() == CMceSession::EOffering
	    */
		IMPORT_C void CancelL();
									  
	private://methods

	    /**
	     * C++ default constructor.
	     * @param aManager, reference to session manager.
	     * @param aProfileId, id of the sip profile used for the session.
	     */
		CMceOutSession( CMceManager* aManager, TUint32 aProfileId );

	    /**
	     * second-phase constructor
	     * @param aRecipient, the recipient
	     * @param aOriginator, the originator
	     */
	    void ConstructL( const TDesC8& aRecipient, HBufC8* aOriginator  );


	private: // Data

	};

#endif
