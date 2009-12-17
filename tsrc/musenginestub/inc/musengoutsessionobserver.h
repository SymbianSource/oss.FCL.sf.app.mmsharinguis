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


#ifndef MUSHENGOUTSESSIONOBSERVER_H
#define MUSHENGOUTSESSIONOBSERVER_H

// INCLUDES
#include "musengsessionobserver.h"

// CLASS DECLARATION

/**
* An interface to be implemented by users of Multimedia Sharing Engine if they 
* wish to receive feedback from outsent INVITE. All the callback functions 
* represent an answer from network to sent INVITE.
*
* This observer is set using CMusEngSession::SetSessionObserver function.
*
* @lib musengine.lib
* @since S60 v3.2
*/
class MMusEngOutSessionObserver : public MMusEngSessionObserver
    {
    public:

		/**    
   		* Indicates that MT has rejected session. ( e.g. 603 DECLINE )
        * @since S60 v3.2
		*/
     	virtual void SessionRejected() = 0;
    	
    	/**
    	* The following functions represent a specific reason in answer
    	* to sent INVITE.
    	* @since S60 v3.2
    	*/
    	
		virtual void SessionBadRequest() = 0; // 400 
		virtual void SessionUnauthorized() = 0; // 401 
		virtual void SessionPaymentRequired() = 0; // 402
    	virtual void SessionRecipientNotFound() = 0; // 404 
		virtual void SessionProxyAuthenticationRequired() = 0; // 407
		virtual void SessionRequestTimeOut() = 0; // 408
		virtual void SessionUnsupportedMediaType() = 0; // 415 
		virtual void SessionRequestCancelled() = 0; // 487    	

  	
    };

#endif