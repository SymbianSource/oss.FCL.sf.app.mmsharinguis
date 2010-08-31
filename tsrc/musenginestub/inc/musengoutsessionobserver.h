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


#ifndef MUSENGOUTSESSIONOBSERVER_H
#define MUSENGOUTSESSIONOBSERVER_H

// INCLUDES
#include <badesca.h>

// FORWARD DECLARATIONS
class MMusEngAsyncRetryObserver;

// CLASS DECLARATION

/**
* An interface to be implemented by users of Multimedia Sharing Engine if they 
* wish to receive feedback from outsent INVITE. All the callback functions 
* represent an answer from network to sent INVITE.
*/
class MMusEngOutSessionObserver
    {
    public:

        /**    
        * Indicates that preconditions for sending invitation cannot be met.
        */
        virtual void UnableToSendInvitation() = 0;

	    /**    
        * Indicates that no response received to invitation
        */
        virtual void NoResponseReceivedToInvitation() = 0;
        
		/**    
   		* Indicates that MT has rejected session. ( e.g. 603 DECLINE ).
		*/
     	virtual void SessionRejected() = 0;
    	
    	/**
    	* The following functions represent a specific reason in answer
    	* to sent INVITE.
    	*/
    	
		virtual void SessionBadRequest() = 0; // 400 
		virtual void SessionUnauthorized() = 0; // 401 
		virtual void SessionPaymentRequired() = 0; // 402
    	virtual void SessionRecipientNotFound() = 0; // 404 
		virtual void SessionProxyAuthenticationRequired() = 0; // 407
		virtual void SessionRequestTimeOut() = 0; // 408
		virtual void SessionUnsupportedMediaType() = 0; // 415
		virtual void SessionBusyHere() = 0; // 486
		virtual void SessionRequestCancelled() = 0; // 487    	
  	    virtual void SessionTemporarilyNotAvailable() = 0; // 480
  	          
  	    /**
  	    * Indicates possibility to retry session. Recipient proposal is given
  	    * but implementation can query user for new recipient address.
  	    * Retry address should be filled to aRetryAddress on return.
  	    * @return ETrue if retry should be made
  	    */
  	    virtual TBool RetrySessionSync( const TDesC& aRecipientProposal,
                                        TDes& aRetryAddress ) = 0;
  	    
  	    /**
  	    * Indicates that retry with given address is proceeding.
  	    */
  	    virtual void RetryProceeding() = 0;
  	    
  	    /**
  	    * Indicates that there's multiple suitable recipient addresses.
  	    * Client can choose what to use.
  	    * @param aAddressArray, list of suitable recipient addresses
  	    * @param aRecipientAddress, on return should contain recipient
  	    *    address to be used
  	    * @return ETrue if recipient was selected, otherwise EFalse
  	    */
  	    virtual TBool SelectRecipientL( CDesCArray& aAddressArray, 
                                        TDes& aRecipientAddress ) = 0;
    };

#endif
