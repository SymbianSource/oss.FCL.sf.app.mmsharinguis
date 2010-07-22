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
* Description:   mussipobserver is SIP API observer class for the MUS availability plug-in.
*
*/



#include "musunittesting.h"
#include "musavasipconnectionobserver.h"
#include "musavasipconnectionadapter.h"
#include "musavasipdefaulthandler.h"
#include "musavasip.h"
#include "muslogger.h"

#include <e32std.h>
#include <sipconnectionobserver.h>
#include <sipservertransaction.h>
#include <sipinvitedialogassoc.h>




// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
CMusAvaSipConnectionObserver* CMusAvaSipConnectionObserver::NewL()
    {
    CMusAvaSipConnectionObserver* self = 
        new( ELeave ) CMusAvaSipConnectionObserver();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

    
// Destructor
CMusAvaSipConnectionObserver::~CMusAvaSipConnectionObserver()
    {                      
    delete iDefaultHandler;
    iObservers.Reset();
    iObservers.Close();
    }


// -----------------------------------------------------------------------------
// A SIP request outside a dialog has been received from the network.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::IncomingRequest( 
                                    CSIPServerTransaction* aTransaction )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->IncomingRequest( aTransaction );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// A SIP request within a dialog has been received from the network.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::IncomingRequest(
                                    CSIPServerTransaction* aTransaction,
                                    CSIPDialog& aSIPDialog )

    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->IncomingRequest( aTransaction, aSIPDialog );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// A SIP response that is within a dialog association or creates a
// dialog association.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::IncomingResponse(
                                    CSIPClientTransaction& aTransaction )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->IncomingResponse( aTransaction );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// A SIP response that is within a dialog association or creates a
// dialog association.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::IncomingResponse(
                                    CSIPClientTransaction& aTransaction,
                                    CSIPDialogAssocBase& aDialogAssoc)
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->IncomingResponse( aTransaction, aDialogAssoc );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// Multiple SIP responses have been received to the single INVITE.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::IncomingResponse(
                                    CSIPClientTransaction& aTransaction,
                                    CSIPInviteDialogAssoc* aDialogAssoc )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->IncomingResponse( aTransaction, aDialogAssoc );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// SIP stack has completed UAC core INVITE transaction.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::InviteCompleted( 
                                    CSIPClientTransaction& aTransaction )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->InviteCompleted( aTransaction );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// A SIP response creating a registration binding or an error response
// that is related to an existing refreshed registration binding has
// been received from the network.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::IncomingResponse(
                                    CSIPClientTransaction& aTransaction,
                                    CSIPRegistrationBinding& aRegistration )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->IncomingResponse( aTransaction, aRegistration );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// An asynchronous error has occurred in the stack related to the 
// request indicated by the given transaction.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::ErrorOccured( 
                                    TInt aError,
                                    CSIPTransactionBase& aTransaction )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->ErrorOccured( aError, aTransaction );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// An asynchronous error has occurred in the stack related to the 
// request indicated by the given transaction.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::ErrorOccured(
                                    TInt aError,
                                    CSIPClientTransaction& aTransaction,
                                    CSIPRegistrationBinding& aRegistration ) 
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->ErrorOccured( aError, aTransaction, aRegistration );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// An asynchronous error has occured related to a request within an
// existing dialog.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::ErrorOccured(
                                    TInt aError,
                                    CSIPTransactionBase& aSIPTransactionBase,
                                    CSIPDialogAssocBase& aSIPDialogAssociation )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->ErrorOccured( aError, 
                                              aSIPTransactionBase, 
                                              aSIPDialogAssociation );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// An asynchronous error has occured related to a refresh.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::ErrorOccured( TInt aError, 
                                                 CSIPRefresh& aSIPRefresh )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->ErrorOccured( aError, aSIPRefresh );
        if( retval == KErrNone)
            {
            return;
            }
        }
    } 
    

// -----------------------------------------------------------------------------
// An asynchronous error has occured related to a periodical refresh 
// that relates to a registration.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::ErrorOccured( 
                                    TInt aError,
                                    CSIPRegistrationBinding& aRegistration )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->ErrorOccured( aError, aRegistration );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// An asynchronous error has occured related to a periodical refresh
// that belongs to SIP dialog association.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::ErrorOccured(
                                TInt aError,            
                                CSIPDialogAssocBase& aSIPDialogAssociation )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->ErrorOccured( aError, aSIPDialogAssociation );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// Invite was canceled with the CANCEL
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::InviteCanceled( 
                                CSIPServerTransaction& aTransaction )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->InviteCanceled( aTransaction );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }


// -----------------------------------------------------------------------------
// Is called when the connection state changes
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::ConnectionStateChanged ( 
                                CSIPConnection::TState aState )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->ConnectionStateChanged( aState );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }

// -----------------------------------------------------------------------------
// C++ default constructor.
// -----------------------------------------------------------------------------
CMusAvaSipConnectionObserver::CMusAvaSipConnectionObserver()
    {
    }


// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor.
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::ConstructL()
    {
    iDefaultHandler = new( ELeave ) CMusAvaSipDefaultHandler();                      
 	iObservers.AppendL( iDefaultHandler ); 
 	}
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::AddObserverL( 
                                        MMusAvaSipConnectionAdapter& aAdapter,
                                        TInt aIndex )
    {
    TInt index = iObservers.Find( &aAdapter );
    
    if( index == KErrNotFound )
        {
        TInt count = iObservers.Count();
        if ( aIndex > KErrNotFound && aIndex < iObservers.Count() )
            {
            iObservers.InsertL( &aAdapter, aIndex );
            }
        else
            {
            iObservers.InsertL( &aAdapter, count-1 );       
            }    
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
void CMusAvaSipConnectionObserver::RemoveObserver( MMusAvaSipConnectionAdapter& aAdapter )
    {
    TInt index = iObservers.Find( &aAdapter );
    
    if( index != KErrNotFound )
        {
        iObservers.Remove( index );
        }
    }

