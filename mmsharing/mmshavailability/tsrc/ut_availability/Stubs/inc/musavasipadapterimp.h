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
* Description:  This observer is used by
*                ECOM clients to get function call from the availability Plug-in
*
*/



#ifndef __MUSAVASIPOBSERVERIMP_H__
#define __MUSAVASIPOBSERVERIMP_H__

#include "musavasipadapter.h"
#include "musavasipconnectionadapter.h"
#include "musunittesting.h"
#include <sipconnection.h>
#include <e32base.h>
#include <e32def.h>

#ifdef MUS_UNITTEST
#undef IMPORT_C
#define IMPORT_C
#endif

/**
 *  MusAvailability Observer base class 
 *
 *  Observer interface for ECOM clients.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class CMusAvaSipAdapterImp : public CBase, 
                             public MMusAvaSipAdapter,
                             public MMusAvaSipConnectionAdapter
                                
	{
public:

    enum
        {
        };
   
     /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param 
     * @return Returns pointer to CMusAvaController object
     */
     
	static CMusAvaSipAdapterImp* NewL(  );
	
	/**
     * Destructor
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	~CMusAvaSipAdapterImp();
	
	protected:
	
	/**
     * Constructor
     *
     * @since S60 v3.2
     * @return 
     */

	CMusAvaSipAdapterImp( );
	
    /**
     * Perform the second phase construction of a
     *             CMusAvaController object
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	void ConstructL();
	
public: // from base class MMusAvaSipAdapter
    
    TInt IncomingRequest( TUint32 aIapId,
                          CSIPServerTransaction* aTransaction );

    TInt TimedOut( CSIPServerTransaction& aTransaction );

public: // from base class MMusAvaSipConnectionAdapter

	TInt IncomingRequest( CSIPServerTransaction* aTransaction );

	TInt IncomingRequest( CSIPServerTransaction* aTransaction,
					      CSIPDialog& aDialog );

	TInt IncomingResponse( CSIPClientTransaction& aTransaction );

	TInt IncomingResponse (
					CSIPClientTransaction& aTransaction,
					CSIPDialogAssocBase& aDialogAssoc );

    TInt IncomingResponse(
					CSIPClientTransaction& aTransaction,
					CSIPInviteDialogAssoc* aDialogAssoc );

	TInt IncomingResponse( CSIPClientTransaction& aTransaction,
                           CSIPRegistrationBinding& aRegistration );

 	TInt ErrorOccured( TInt aError,
                       CSIPTransactionBase& aTransaction );

	TInt ErrorOccured( TInt aError,
				       CSIPClientTransaction& aTransaction,
					   CSIPRegistrationBinding& aRegistration ); 
    TInt ErrorOccured( TInt aError,
				       CSIPTransactionBase& aTransaction,
					   CSIPDialogAssocBase& aDialogAssoc );

	TInt ErrorOccured( TInt aError, CSIPRefresh& aSIPRefresh );

	TInt ErrorOccured( TInt aError,	CSIPRegistrationBinding& aRegistration );

	TInt ErrorOccured( TInt aError,			
					   CSIPDialogAssocBase& aDialogAssoc ) ;

    TInt InviteCompleted( CSIPClientTransaction& aTransaction );

	TInt InviteCanceled( CSIPServerTransaction& aTransaction );
                
	TInt ConnectionStateChanged( CSIPConnection::TState aState );

public:

    void Reset();
    void CleanServerTransaction();
    void SetServerTransaction( CSIPServerTransaction& aTransaction );
    void SetServerTransaction( CSIPServerTransaction* aTransaction );
                
public:

    CSIPClientTransaction* iClientTransaction;
    CSIPServerTransaction* iServerTransaction;
    CSIPConnection::TState iState;
    TInt iError;
    CSIPRegistrationBinding* iRegBinding;
    CSIPRefresh* iRefresh;
    CSIPDialogAssocBase* iDialogAssoc;
    CSIPDialog* iDialog;
    TUint32 iIapId;
    CSIPInviteDialogAssoc* iInviteAssoc;
    CSIPTransactionBase* iTransaction;
    TBool iServerTransctionOwned;
    TInt iReturnCode;
    };


#endif // __MUSAVASIPOBSERVERIMP_H__

