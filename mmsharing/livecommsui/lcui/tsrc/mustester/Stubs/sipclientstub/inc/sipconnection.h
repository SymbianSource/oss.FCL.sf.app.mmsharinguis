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
* Description:  
*
*/


#ifndef CSIPCONNECTION_H
#define CSIPCONNECTION_H

//  INCLUDES
#include <e32base.h>
#include <in_sock.h>

// FORWARD DECLARATIONS
class CSIP;
class MSIPConnectionObserver;
class CSIPTransactionBase;
class CSIPClientTransaction;
class CSIPServerTransaction;
class MSIPRegistrationContext;
class CSIPRegistrationBinding;
class CRegistrationState;
class CSIPDialog;
class CDialogState;
class CSIPRefresh;
class CSIPMessageElements;
class CSIPRequestElements;
class CSIPResponseElements;
class CSIPClientConnectionObserver;
class CConnectionCallback;
class CSIPClientConnection;


//CONSTANTS
/** SIP minimum value for the expiration in seconds*/
//const TUint KSIPMinExpirationValue=60;


// CLASS DECLARATION

/**
*  Class for monitoring connection's state and sending SIP requests outside
*  the dialog associations.Class must not be used
*  -for sending requests to which received non-failure response creates a dialog
*  -for sending requests that must be sent within SIP dialog
* Note that the user can have only one instance of the class per same IAP-id
*/
class CSIPConnection :
    public CBase
	{
    public:
    
        /** Connection states */
        enum TState 
            {
            /** Connection initializing */
			EInit=1,
			/** Connection active */
			EActive,
			/** Connection suspended */
			ESuspended,
			/** Connection is inactive */
			EInactive,
			/** Connection is permanently unavailable */
			EUnavailable
            };

    public:  // Constructors and destructor

	    /**
        * Two-phased constructor.
		* @param aSip a handle to the SIP server
		* @param aIapId IAP-id
		* @param aObserver a observer for receiving asynchonous events on this
        *   connection
        * @return New object, ownership is transferred
        */

		IMPORT_C static CSIPConnection*
            NewL(CSIP& aSIP,
				 TUint32 aIapId,
				 MSIPConnectionObserver& aObserver);


	    /**
        * Two-phased constructor.
		* Constructs an object and adds the pointer to the cleanup stack;
		* @param aSip a handle to the SIP server
		* @param aIapId IAP-id
		* @param aObserver a observer for receiving asynchonous events on this
        *   connection
        * @return New object, ownership is transferred
        */

		IMPORT_C static CSIPConnection*
            NewLC(CSIP& aSIP,
				  TUint32 aIapId,
				  MSIPConnectionObserver& aObserver);


		/**
		* Destructor
		*/

		IMPORT_C ~CSIPConnection();

    public: // New functions

        /**
		* Gets current state of the connection        
        * @return current state of the connection
        */

		IMPORT_C TState State() const;

        /**
		* Creates SIP request and sends it to the given destination.
		* The function must not be used with SIP requests that creates
		* SIP dialog associations or must be sent within SIP dialog
		* association.
		* This provokes the Non-invite state machine.
		* This function must be used if the user has created the
		* registration binding.
		* @pre State() == EActive.
		* @pre aContext.IsContextActive()==ETrue
		* @param aElements contains local and remote addresses,
		*		 optional SIP message headers and body.
		*        The ownership is transferred.
		* @param aContext defines outbound proxy to be used. If the user
		*        does not provide from-address it will be constucted using
		*        user's AOR used during the registration.
        * @return SIP client transaction, ownership is transferred
        */

		IMPORT_C CSIPClientTransaction*
            SendRequestL(CSIPRequestElements* aElements,
                         const MSIPRegistrationContext& aContext);


        /**
		* Creates SIP request and sends it to the given destination.
		* The function must not be used with SIP request that creates
		* SIP dialog associations or must be sent within SIP diallog
		* association.
		* This provokes the Non-invite state machine.
		* This function must be used if the user has created the
		* registration binding.
		* @pre State() == EActive.
		* @pre aContext.IsContextActive()==ETrue
		* @param aElements contains local and remote addresses,
		*		 optional SIP message headers and body.
		*        The ownership is transferred.
		* @param aContext defines outbound proxy to be used. If the user
		*        does not provide from-address it will 
		*        be constucted using user's AOR used during the registration.
		* @param aRefresh defines the refresh interval for this transaction.
        * @return SIP client transaction, ownership is transferred
        */

		IMPORT_C CSIPClientTransaction*
            SendRequestL(CSIPRequestElements* aElements,                                        
                         const MSIPRegistrationContext& aContext,
						 CSIPRefresh& aRefresh); 


        /**
		* Creates SIP request and sends it to the given destination.
		* The function must not be used with SIP request that creates SIP
		* diallog associations or must be sent within SIP dialog association.
		* This invokes the Non-invite state machine.
		* @pre State() == EActive.
		* @pre aElements.FromHeader() != 0
		* @param aElements contains local and remote addresses,
		*		 optional SIP message headers and body.
		*        The ownership is transferred.
        * @return SIP client transaction, ownership is transferred
        */

		IMPORT_C CSIPClientTransaction*
            SendRequestL(CSIPRequestElements* aElements);
        

        /**
		* Creates SIP request and sends it to the given destination.
		* The function must not be used with SIP request that creates SIP
		* dialog associations or must be sent within SIP dialog association.
		* This invokes the Non-invite state machine.
		* @pre State() == EActive.
		* @pre aElements.FromHeader() != 0
		* @param aElements contains local and remote addresses,
		*		 optional SIP message headers and body.
		*        The ownership is transferred.
		* @param aRefresh indicates that transaction will be refreshed
        * @return SIP client transaction, ownership is transferred
        */

		IMPORT_C CSIPClientTransaction*
            SendRequestL(CSIPRequestElements* aElements,
						 CSIPRefresh& aRefresh);


        /**
		* Creates SIP REGISTER request and sends it to the given destination.
		* User must no define any Contact or Expires-headers
		* in the provided elements.
		* @pre State() == EActive
        * @pre aElements != 0
		* @pre aElements->Method().DesC().Length()==0
		* @param aElements contains local and remote addresses, optional SIP
        *        message headers and body. User must not set any expires values
        *        to aElements. The ownership is transferred.
        * @return SIP REGISTER client transaction, ownership is transferred
        */

		IMPORT_C CSIPClientTransaction*
            FetchRegistrationsL(CSIPRequestElements* aElements);
            
        // Not in real SIP API:
        IMPORT_C void SetState(TState aState);    

		//TODO:SHOULD THIS NEW FUNCTION BE INCLUDED INTO STUB???
		/**
		* Gets Iap-id used for this connection
		* @return Iap-id
		*/
		
		IMPORT_C TUint32 IapId() const;
		
		
		/**
        * Gets SIP server handle
        * @return handle to SIP server, or 0-pointer if SIP server does not
        *	exist any more
        */
        IMPORT_C CSIP* SIP();
  
		/**
        * Gets SIP server handle
        * @return handle to SIP server, or 0-pointer if SIP server does not
        *	exist any more
        */
        
        IMPORT_C const CSIP* SIP() const;
		//TODO:ENDS
		
        /**
		* Incoming request outside dialog has been received.
		*
		* @param aTransaction Server transaction. Ownership is transferred.        
        */

        void IncomingRequestL(CSIPServerTransaction* aTransaction) const;

        /**
        * Gets the local IP address of this CSIPConnection 
        * that is used by the SIP implementation.
        * The user should re-new the call whenever the connection state changes
        * to the CSIPConnection::EActive in order to verify the correctness of
        * the previously retrieved local IP address. The local IP address might
        * be different as a result of the connection state change.
        * @pre State() == CSIPConnection::EActive
        * @param aAddr on return filled with local IP address
        * @leave a system-wide error code
        */ 	                         
	    IMPORT_C void GetLocalAddrL(TInetAddr& aAddr) const;

        
        /**
		* Incoming request within dialog has been received.
		*
		* @param aTransaction Server transaction. Ownership is transferred.
        * @param aDialogId Identifies the dialog to which the transaction
        *   belongs to.
        */

        void IncomingDialogRequestL(CSIPServerTransaction* aTransaction,
                              		TUint32 aDialogId);

        void IncomingResponseL(CSIPResponseElements* aElements,
                               TUint32 aRequestId) const;
    
        void IncomingRegistrationResponseL(CSIPResponseElements* aElements,
		                       			   TUint32 aRequestId,
					           			   TUint32 aRegistrationId) const;

        void IncomingRefreshResponseL(CSIPResponseElements* aElements,
		                       		  TUint32 aRequestId,
							   		  TUint32 aRefreshId);


        void ErrorOccured(TInt aError, TUint32 aRequestId) const;

        void RefreshErrorOccured(TInt aError, TUint32 aRefreshId) const;

	    void RegistrationErrorOccured(TInt aError,
                          			  TUint32 aRegistrationId,
                          			  TUint32 aRequestId) const;

		void DialogErrorOccured(TInt aError,
                          		TUint32 aDialogId,
                          		TUint32 aRequestId) const;

        void DialogErrorOccured(TInt aError,
                          		TUint32 aDialogId,
                          		TUint32 aRefreshId,
                          		TUint32 aRequestId) const;

        void ConnectionStateChanged(CSIPConnection::TState aState);

        void InviteCompleted(TUint32 aRequestId) const;


		/**
        * Searches for a transaction with the matching aRequestId.
		* @param aRequestId RequestId associated with the transaction
		* @return Found transaction or NULL if not found. Ownership is not
        *   transferred.
        */
        
        CSIPTransactionBase* FindTransaction(TUint32 aRequestId) const;
   
   
        TInt AddRegistration(CSIPRegistrationBinding* aRegistration);
        void RemoveRegistration(const CSIPRegistrationBinding* aRegistration);
        

      /**
        * Searches for a registration binding, using aRegistrationId and
        * aRequestId as keys. Registration might not yet have received
        * RegistrationId from SIP client, so the aRequestId may be needed to
        * locate the registration binding.
        * @param aRegistrationId RegistrationId of the registration binding
		* @param aRequestId RequestId of a transaction associated with the
        *   registration
		* @return Found registration binding or NULL if not found.
		*	Ownership is not transferred.
        */
        
        CSIPRegistrationBinding* FindRegistration(TUint32 aRegistrationId,
                             			   		  TUint32 aRequestId) const;


        TInt AddDialog(CSIPDialog* aDialog);
        void RemoveDialog(const CSIPDialog* aDialog);


        /**
        * Searches for a dialog, using aRequestId as a key.
        * @param aRequestId RequestId of a transaction associated with the
        *   dialog
		* @return Found dialog or NULL if not found. Ownership is not
        *   transferred.
        */

        CSIPDialog* FindDialogByRequestId(TUint32 aRequestId) const;


        /**
		* Informs CSIPConnection that the associated CSIP instance has been
        * deleted. After this the CSIPConnection object can't be used anymore.
		*/

        void CSIPDeleted();


        /**
		* Obtains the initial state for registration state machine.
        *
        * @return Initial registration state, ownership is not transferred
		*/

        CRegistrationState* InitialRegisterStateL() const;


        /**
		* Obtains the initial state for dialog state machine.
        *
        * @return Initial dialog state, ownership is not transferred
		*/

        CDialogState* InitialDialogStateL() const;


        /**
        * Response related to a dialog has been received.
		*
        * @param aElements SIP response elements. Ownership is transferred.
        * @param aRequestId Identifies the transaction
        * @param aDialogId DialogId
        * @param aRefreshId NULL if the response is not related to a refresh,
        *   otherwise the RefreshId. Ownership is not transferred.
		*/

        void IncomingResponseToDialogL(CSIPResponseElements* aElements,
		                               TUint32 aRequestId,
                                       TUint32 aDialogId,
                                       TUint32* aRefreshId);

    private:
        CSIPConnection(CSIP& aSIP,
                       TUint32 aIapId,
                       MSIPConnectionObserver& aObserver);

        void ConstructL();


        /**
        * Send a SIP request.
        *
		* @param aElements SIP headers. Ownership is transferred.
        * @param aRegistrationId If non-NULL, request will use the outbound
        *   proxy and some SIP headers from the registration identified by
        *   aRegistrationId. Ownership is not transferred.
        * @param aRefresh If non-NULL, the request will be refreshed. Otherwise
        *   NULL. Ownership is not transferred.
		* @return New transaction. Ownership is transferred.
        */

        CSIPClientTransaction*
            SendRequestL(CSIPRequestElements* aElements,
                         TUint32* aRegistrationId,
						 CSIPRefresh* aRefresh);


        /**
        * Searches for a refresh and transaction, using aRequestId and
        * aRefreshId as keys.        
		* @param aRequestId RequestId associated with the refreshed transaction
        * @param aRefreshId RefreshId associated with the refresh instance
        * @param aRefresh OUT: if refresh was found, this pointer is set to the
        *   refresh instance. Ownership is not transferred.
        * @param aTransaction OUT: if transaction was found, this pointer is
        *   set to the refreshed client transaction. Ownership is not
        *   transferred.
		* @return ETrue if found, EFalse otherwise
        */

        TBool FindRefresh(TUint32 aRequestId,
                          TUint32 aRefreshId,
                          CSIPRefresh** aRefresh,
                          CSIPClientTransaction** aTransaction) const;

        /**
        * Searches for a registration binding, using aRequestId as a key.
		* @param aRequestId RequestId of a transaction associated with the
        *   registration binding
		* @return Found registration binding or NULL if not found.
		*	Ownership is not transferred.
        */

        CSIPRegistrationBinding* FindRegistration(TUint32 aRequestId) const;

        
        /**
        * Searches for a dialog, using aDialogId as a key.        
        * @param aDialogId DialogId of the dialog.
		* @return Found dialog or NULL if not found. Ownership is not
        *   transferred.
        */

        CSIPDialog* FindDialog(TUint32 aDialogId) const;


        /**
        * A request has been received with a dialogId, but no matching dialog
        * exists.
		* If the request is NOTIFY, it is stored until a response with the same
        * dialogId is received. Then the response is passed to dialog and after
        * that all the stored NOTIFY are passed to the same dialog.
        * This is needed since NOTIFY can be received before a sent SUBSCRIBE
        * receives a response with dialogId.
		*
        * @param aTransaction Server transaction, ownership is transferred
        * @param aDialogId DialogId
		*/

        void IncomingRequestBeforeDialogExistsL(
                                    CSIPServerTransaction* aTransaction,
                                    TUint32 aDialogId);


        /**
		* Extracts the oldest stored transaction for the dialog identified by
        * aDialogId.
        * @param aDialogId DialogId
        * @return Server transaction or NULL if no transaction is stored for
        *   the dialog. Ownership is transferred.
		*/

        CSIPServerTransaction* GetStoredTransaction(TUint32 aDialogId);


        /**
		* Checks that CSIP is available for use (not NULL). If iSIP is NULL,
        * it means user has deleted a resource needed by CSIPConnection, and
        * this function leaves.
		*/

        void CheckCSipL() const;       


        /**
		* Connection is no longer available. All registrations, dialogs,
        * transactions and refreshes using this connection are terminated, but
        * not deleted.
		*/

        void ConnectionLost();


        //The used CSIP instance. CSIPConnection doesn't own the CSIP.
        //If this pointer is NULL; it means application has deleted the
        //CSIP instance before deleting the CSIPConnection, and this
        //CSIPConnection is now useless.
        CSIP* iSIP;

        //IAP-id of this connection
        TUint32 iIapId;

        //Callback to upper layer
        MSIPConnectionObserver& iObserver;

        //Callback receiving events from client
        CSIPClientConnectionObserver* iClientConnectionObserver;        

        //Connection used by this CSIPConnection instance
        CSIPClientConnection* iClientConnection;

        //CSIPConnection doesn't own CSIPRegistrationBindings
        RPointerArray<CSIPRegistrationBinding> iRegistrations;

        //CSIPConnection doesn't delete the CSIPDialog objects. CSIPDialog
        //deletes itself when its last dialog association is deleted.
        //NOTE: When CSIPDialog has just been created and has no dialog
        //associations yet, it must be stored in CleanupStack, otherwise it
        //won't be freed if leave occurs!
        RPointerArray<CSIPDialog> iDialogs;

        //CSIPConnection has to know which transactions are attached to it, but
        //it doesn't own the transactions. Transaction's ownership is passed
        //to application.
        RPointerArray<CSIPTransactionBase> iTransactions;

        //Standalone refreshes, application owns these
        RPointerArray<CSIPRefresh> iRefreshes;

        //To avoid using callbacks to MSIPConnectionObserver from within
        //CSIPRegistrationBinding and CSIPDialog, CConnectionCallback is passed to
        //the methods which would need to use MSIPConnectionObserver calls.
        //CSIPRegistrationBinding and CSIPDialog will fill CConnectionCallback with
        //the selected callback method and its parameters, and by returning
        //ETrue they tell to CSIPConnection that a callback has to be done.
        CConnectionCallback* iCallbackInfo;
        
        
        // Not in real SIP API
        TState iState;
	};

#endif

