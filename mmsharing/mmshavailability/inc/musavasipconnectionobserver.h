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
* Description:  CMusAvaSipConObserver is SIP API observer class.
*
*/



#ifndef MUSAVASIPCONNECTIONOBSERVER_H
#define MUSAVASIPCONNECTIONOBSERVER_H

#include <e32base.h>
#include <sipconnectionobserver.h>
#include "musunittesting.h"

class CMusAvaSip;
class CSIPDialog;
class CSIPServerTransaction;
class CSIPClientTransaction;
class CSIPDialogAssocBase;
class CSIPRegistrationBinding;
class MMusAvaSipConnectionAdapter;
class CMusAvaSipDefaultHandler;

/**
 *  CMusAvaSipConnectionObserver implements observer interface of SIP API
 *
 *
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */


class CMusAvaSipConnectionObserver : public CBase,
                                     public MSIPConnectionObserver
    {

public: // constructors and destructor

    /**
    * Two-phased constructor.
    *
    * @since S60 v3.2
    * @param aMusAvaSip pointer to CMusAvaSip
    */
    static CMusAvaSipConnectionObserver* NewL();

    /**
    * Destructor.
    */
    ~CMusAvaSipConnectionObserver();

public:
    /**
     * .
     */
    void AddObserverL( MMusAvaSipConnectionAdapter& aAdapter, TInt aIndex );

    /**
     * .
     */
    void RemoveObserver( MMusAvaSipConnectionAdapter& aAdapter );


public: // functions from base classes

    /**
    * A SIP request outside a dialog has been received from the network.
    *
    * @since S60 v3.2
    * @param aTransaction SIP server transaction. The ownership is
    *   transferred.
    */
    void IncomingRequest ( CSIPServerTransaction* aTransaction );

    /**
    * A SIP request within a dialog has been received from the network.
    * The client must resolve the actual dialog association to which
    * this request belongs.
    *
    * @since S60 v3.2
    * @param aTransaction SIP server transaction. The ownership is
    *        transferred.
    * @param aDialog the dialog that this transaction belongs to.
    */
    void IncomingRequest( CSIPServerTransaction* aTransaction,
                          CSIPDialog& aDialog );

    /**
    * A SIP response received from the network.
    *
    * @since S60 v3.2
    * @param aTransaction contains response elements.
    */
    void IncomingResponse( CSIPClientTransaction& aTransaction );

    /**
    * A SIP response that is within a dialog association or creates
    * a dialog association.
    *
    * @since S60 v3.2
    * @param aTransaction contains response elements.
    * @param aDialogAssoc a dialog association.
    */
    void IncomingResponse( CSIPClientTransaction& aTransaction,
                           CSIPDialogAssocBase&   aDialogAssoc );

    /**
    * Multiple SIP responses have been received to the single INVITE due
    * to the forking proxy. Note that each response creates a separate
    * INVITE dialog association.
    * Multiple responses can arrive until SIP stack completes UAC core
    * INVITE transaction.
    * If clients deletes INVITE transaction after first SIP response
    * other possible responses will be consumed by the implementation.
    *
    * @since S60 v3.2
    * @param aTransaction contains response elements
    * @param aDialogAssoc INVITE dialog association; the ownership is
    *        transferred
    */
    void IncomingResponse( CSIPClientTransaction& aTransaction,
                           CSIPInviteDialogAssoc* aDialogAssoc );
    /**
    * A SIP response related a registration binding or an error response
    * that is related to a refreshed registration binding has been received
    * from the network.
    *
    * @since S60 v3.2
    * @param aTransaction contains response elements.
    * @param aRegistration associated registration binding.
    */
    void IncomingResponse (CSIPClientTransaction& aTransaction,
                              CSIPRegistrationBinding& aRegistration);

    /**
    * An asynchronous error has occurred in the stack related to the
    * request indicated by the given transaction.
    *
    * @since S60 v3.2
    * @param aError error code
    * @param aTransaction failed transaction.
    * @param aSIPConnection a SIP connection
    */
    void ErrorOccured( TInt aError,
                       CSIPTransactionBase& aTransaction );

   /**
   * An asynchronous error has occurred in the stack related
   * to the request indicated by the given transaction.
   *
   * @since S60 v3.2
   * @param aError system wide or SIP error code
   * @param aTransaction the failed transaction
   * @param aRegistration the failed registration binding
   */
   void ErrorOccured (TInt aError,
      CSIPClientTransaction& aTransaction,
      CSIPRegistrationBinding& aRegistration);

    /**
    * An asynchronous error has occured related to a request within
    * an existing dialog.
    *
    * @since S60 v3.2
    * @param aError error code
    * @param aTransaction the failed transaction.
    * @param aDialogAssoc the failed dialog associoation.
    */
    void ErrorOccured( TInt aError,
                       CSIPTransactionBase& aTransaction,
                       CSIPDialogAssocBase& aDialogAssoc );

    /**
    * An asynchronous error has occured related to a refresh
    *
    * @since S60 v3.2
    * @param aError error code
    * @param aSIPRefresh original refresh object.
    */
    void ErrorOccured( TInt aError, CSIPRefresh& aSIPRefresh );

    /**
    * An asynchronous error has occured related to a periodical refresh
    * that relates to a registration.
    *
    * @since S60 v3.2
    * @param aError system wide or SIP error code;
    * KErrCouldNotConnect if the refresh has failed
    * due to the suspended connection.
    * @param aRegistration associated registration binding
    */
   void ErrorOccured (TInt aError,
                CSIPRegistrationBinding& aRegistration);

    /**
    * An asynchronous error has occured related to a periodical refresh
    * that belongs to SIP dialog association.
    *
    * @since S60 v3.2
    * @param aError error code
    * @param aDialogAssoc SIP dialog association.
    */
    void ErrorOccured( TInt aError,
                       CSIPDialogAssocBase& aDialogAssoc );


     /**
    * SIP stack has completed UAC core INVITE transaction 64*T1 seconds
    * after the reception of the first 2xx response. No more 2xx responses
    * can be received to the issued single INVITE.
    *
    * If the INVITE transaction does not create a dialog, or the INVITE
    * transaction encounters an error, this event will not be sent.
    *
    * @since S60 v3.2
    * @param aTransaction a complete UAC core INVITE transaction
    */
    void InviteCompleted (CSIPClientTransaction& aTransaction);

    /**
    * Invite was canceled with the CANCEL
    *
    * @since S60 v3.2
    * @param aTransaction a canceled INVITE UAS transaction
    */
    void InviteCanceled (CSIPServerTransaction& aTransaction);

    /**
    * Connection state has changed.
    * If connection state has changed to EInactive or EUnavailable,
    * SIP stack has removed all stand-alone SIP refreshes, registrations
    * and dialog associations that client requested to refresh. Client may
    * re-issue refresh requests (stand-alone, registration or dialog
    * association related) when connection becomes EActive again.
    * SIP stack also terminates all pending SIP client transactions and no
    * errors are reported back to the client about the terminated
    * transactions nor about removed refreshes in order to avoid event
    * flood.
    *
    * @since S60 v3.2
    * @param aState indicates the current connection state
    */
    void ConnectionStateChanged (CSIPConnection::TState aState);


private:

    /**
    * C++ default constructor.
    * @param aMusAvaSip pointer to CMusAvaSip
    */
    CMusAvaSipConnectionObserver();

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private: // data

    RPointerArray<MMusAvaSipConnectionAdapter> iObservers;
    
    CMusAvaSipDefaultHandler* iDefaultHandler;

private: // For testing purposes

    //MUS_UNITTEST( UT_CMusAvaSip )
    MUS_UNITTEST( UT_CMusAvaSipConnectionObserver )

    };

#endif // MUSAVASIPCONNECTIONOBSERVER_H
