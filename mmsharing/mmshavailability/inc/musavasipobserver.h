/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This observer is used to get function call from SIP Server
*
*/


#ifndef MUSAVASIPOBSERVER
#define MUSAVASIPOBSERVER

//  INCLUDES
#include "musunittesting.h"
#include <sipobserver.h>

#include <e32base.h>
#include <e32std.h>

class CSIPServerTransaction;
class MMusAvaSipAdapter;
/**
 *  SIP Observer class 
 *
 *  SIP Server Observer interface for MusAvailability Plug-in.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */


class CMusAvaSipObserver : public CBase, 
                           public MSIPObserver
	{
public:
	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @return Returns pointer to CMusAvaInterface object
     */
     
	static CMusAvaSipObserver* NewL();
	
	/**
     * Destructor
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	~CMusAvaSipObserver();
	
	protected:
	/**
     * Constructor
     *
     * @since S60 v3.2 
     * @return 
     */

	CMusAvaSipObserver();
	
	/**
     * Perform the second phase construction of a
     *             CMusAvaSipObserver object
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	void ConstructL();
	
public: // new functions

    void AddObserverL( MMusAvaSipAdapter& aAdapter, TInt aIndex );
    
    void RemoveObserver( MMusAvaSipAdapter& aAdapter );

public: // from base class MSIPObserver
	
   /**
    * A SIP request has been received from the network.
    * This function is called when the SIP request was received using such
    * an IAP, for which the application has not created a CSIPConnection
    * object.
    *
    * @since  S60 v3.2
    * @pre aTransaction != 0
    * @param aIapId The IapId from which the SIP request was received.
    * @param aTransaction contains local address, remote address of a SIP
    *        message, as well as optional SIP message method, headers and
    *        body. The ownership is transferred.
    * @return 
    */
    void IncomingRequest(TUint32 aIapId,
                                 CSIPServerTransaction* aTransaction);

    /**
    * The received SIP request time-outed and it is invalid i.e. cannot be used
    * anymore.
    * This will be called if the user fails to create a SIP connection and
    * does not send an appropriate SIP response.
    *
    * @since  S60 v3.2
    * @param aTransaction The time-outed transaction.
    * @return 
    */
    void TimedOut(CSIPServerTransaction& aTransaction);
    
private:
    
    RPointerArray<MMusAvaSipAdapter> iObservers;
    
    MUS_UNITTEST( UT_CMusAvaSipObserver )
    
    };


#endif //MMUSAVASIPOBSERVER
