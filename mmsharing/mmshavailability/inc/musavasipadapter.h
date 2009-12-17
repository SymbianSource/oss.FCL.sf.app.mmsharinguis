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


#ifndef MMUSAVASIPADAPTER_H
#define MMUSAVASIPADAPTER_H

//  INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CSIPServerTransaction;

// CLASS DECLARATION

/**
*  @publishedAll
*  @released
*
*  An interface to be implemented by users of CSIPConnection.
*  Interface allows to be able to receive requests, responses, connection state
*  notifications and error notifications from sip stack.
*
*  Note: The client must not delete related CSIPConnection object 
*  during the execution of the interface function.
*
*  @lib n/a
*/

class MMusAvaSipAdapter
	{    	
    public:
        /**
        * A SIP request has been received from the network.
        * This function is called when the SIP request was received using such
        * an IAP, for which the application has not created a CSIPConnection
        * object.
        * @pre aTransaction != 0
        * @param aIapId The IapId from which the SIP request was received.
        * @param aTransaction contains local address, remote address of a SIP
        *        message, as well as optional SIP message method, headers and
        *        body. The ownership is transferred.
        */
        virtual TInt IncomingRequest(TUint32 aIapId,
                                     CSIPServerTransaction* aTransaction) = 0;

        /**
        * The received SIP request time-outed and it is invalid i.e. cannot be used
        * anymore.
        * This will be called if the user fails to create a SIP connection and
        * does not send an appropriate SIP response.
        * @param aTransaction The time-outed transaction.
        */
        virtual TInt TimedOut(CSIPServerTransaction& aTransaction) = 0;
	};

#endif // MMUSAVASIPCONNECTIONADAPTER_H
