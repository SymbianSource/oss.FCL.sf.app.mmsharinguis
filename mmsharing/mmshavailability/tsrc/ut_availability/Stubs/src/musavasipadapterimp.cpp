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
* Description:  
*
*/


#include "musavasipadapterimp.h"
#include <sipservertransaction.h>


// -------------------------------------------------------------------------
// Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaSipAdapterImp* CMusAvaSipAdapterImp::NewL()
	{
	CMusAvaSipAdapterImp* self = new (ELeave) CMusAvaSipAdapterImp();
	CleanupStack::PushL (self);
	self->ConstructL();
	CleanupStack::Pop();
	return self;
	}
	
// -------------------------------------------------------------------------
// C++ destructor.
// -------------------------------------------------------------------------
//
CMusAvaSipAdapterImp::~CMusAvaSipAdapterImp()
    {
    CleanServerTransaction();
    delete iInviteAssoc;
    }
    
// -------------------------------------------------------------------------
// Symbian second-phase constructor.
// -------------------------------------------------------------------------
//
void CMusAvaSipAdapterImp::ConstructL()
	{
	}

// -------------------------------------------------------------------------
// C++ constructor.
// -------------------------------------------------------------------------
//
CMusAvaSipAdapterImp::CMusAvaSipAdapterImp() 
	{

	}

// -------------------------------------------------------------------------
// CMusAvaSipAdapterImp::Reset()
// -------------------------------------------------------------------------
//	
void CMusAvaSipAdapterImp::Reset()
    {
    iClientTransaction = NULL;
    CleanServerTransaction();
    iState = CSIPConnection::EInit;
    iError = KErrNone;
    iRegBinding = NULL;
    iRefresh = NULL;
    iDialogAssoc = NULL;
    iDialog = NULL;
    iIapId = 0;
    delete iInviteAssoc;
    iInviteAssoc = NULL;
    iTransaction = NULL;
    iReturnCode = KErrNone;
    }

void CMusAvaSipAdapterImp::CleanServerTransaction()
    {
    if ( iServerTransctionOwned )
        {
        delete iServerTransaction;
        }
    iServerTransaction = NULL;
    }
    
void CMusAvaSipAdapterImp::SetServerTransaction( CSIPServerTransaction& aTransaction )
    {
    iServerTransctionOwned = EFalse;
    iServerTransaction = &aTransaction;
    }
 
void CMusAvaSipAdapterImp::SetServerTransaction( CSIPServerTransaction* aTransaction )
    {
    iServerTransctionOwned = ETrue;
    iServerTransaction = aTransaction;
    }
       
TInt CMusAvaSipAdapterImp::IncomingRequest( TUint32 aIapId,
                          CSIPServerTransaction* aTransaction )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iIapId = aIapId;
    CleanServerTransaction();
    SetServerTransaction( aTransaction );
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::TimedOut( CSIPServerTransaction& aTransaction )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    CleanServerTransaction();
    SetServerTransaction( aTransaction );
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::IncomingRequest( CSIPServerTransaction* aTransaction )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    CleanServerTransaction();
    SetServerTransaction( aTransaction );
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::IncomingRequest( CSIPServerTransaction* aTransaction,
					      CSIPDialog& aDialog )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
        
    CleanServerTransaction();
    SetServerTransaction( aTransaction );
    iDialog = &aDialog;
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::IncomingResponse( CSIPClientTransaction& aTransaction )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iClientTransaction = &aTransaction;
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::IncomingResponse (
					CSIPClientTransaction& aTransaction,
					CSIPDialogAssocBase& aDialogAssoc )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iClientTransaction = &aTransaction;
    iDialogAssoc = &aDialogAssoc;
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::IncomingResponse(
					CSIPClientTransaction& aTransaction,
					CSIPInviteDialogAssoc* aDialogAssoc )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iClientTransaction = &aTransaction;
    delete iInviteAssoc;
    iInviteAssoc = aDialogAssoc;
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::IncomingResponse( CSIPClientTransaction& aTransaction,
                           CSIPRegistrationBinding& aRegistration )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iClientTransaction = &aTransaction;
    iRegBinding = &aRegistration;
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::ErrorOccured( TInt aError,
                       CSIPTransactionBase& aTransaction )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iError = aError;
    iTransaction = &aTransaction;
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::ErrorOccured( TInt aError,
				       CSIPClientTransaction& aTransaction,
					   CSIPRegistrationBinding& aRegistration )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iError = aError;
    iClientTransaction = &aTransaction;
    iRegBinding = &aRegistration;
    return iReturnCode;
    }
     
TInt CMusAvaSipAdapterImp::ErrorOccured( TInt aError,
				       CSIPTransactionBase& aTransaction,
					   CSIPDialogAssocBase& aDialogAssoc )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iError = aError;
    iTransaction = &aTransaction;
    iDialogAssoc = &aDialogAssoc;
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::ErrorOccured( TInt aError, CSIPRefresh& aSIPRefresh )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iError = aError;
    iRefresh = &aSIPRefresh;
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::ErrorOccured( TInt aError,
    CSIPRegistrationBinding& aRegistration )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iError = aError;
    iRegBinding = &aRegistration;
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::ErrorOccured( TInt aError,			
					   CSIPDialogAssocBase& aDialogAssoc )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iError = aError;
    iDialogAssoc = &aDialogAssoc;
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::InviteCompleted( CSIPClientTransaction& aTransaction )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iClientTransaction = &aTransaction;
    return iReturnCode;
    }

TInt CMusAvaSipAdapterImp::InviteCanceled( CSIPServerTransaction& aTransaction )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    CleanServerTransaction();
    SetServerTransaction( aTransaction );
    return iReturnCode;
    }
                
TInt CMusAvaSipAdapterImp::ConnectionStateChanged( CSIPConnection::TState aState )
    {
    if ( iReturnCode != KErrNone )
        {
        return iReturnCode;
        }
    iState = aState;
    return iReturnCode;
    }

