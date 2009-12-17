/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  The observer class for monitoring SIP profiles.
*
*/


#ifndef MMUSSIPOBSERVER_H
#define MMUSSIPOBSERVER_H

#include <e32std.h>
#include <sipobserver.h>

/**
*  @publishedAll
*  @released
*
*  The observer class for monitoring SIP profiles.
*  Class must be implemented by the user in order to
*  observe changes in the SIP profiles.
*/
class MMusSipObserver : public MSIPObserver
    {
public: // from MSIPObserver

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
    virtual void IncomingRequest( TUint32 /*aIapId*/,
                                  CSIPServerTransaction* /*aTransaction*/ ) { }

    /**
    * The received SIP request time-outed and it is invalid i.e. cannot be used
    * anymore.
    * This will be called if the user fails to create a SIP connection and
    * does not send an appropriate SIP response.
    * @param aTransaction The time-outed transaction.
    */
    virtual void TimedOut(CSIPServerTransaction& /*aTransaction*/ ) { }
    };

#endif // MMUSSIPOBSERVER_H
