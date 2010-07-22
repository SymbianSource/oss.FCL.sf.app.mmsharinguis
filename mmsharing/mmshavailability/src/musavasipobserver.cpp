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
* Description:   Implementation class is used to get function call from SIP Server.
*
*/



#include "musavasipobserver.h"
#include "musavasipadapter.h"

#include <e32base.h>
#include <sipservertransaction.h>
// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMusAvaSipObserver* CMusAvaSipObserver::NewL()
    {
    CMusAvaSipObserver* self = new (ELeave) CMusAvaSipObserver();
    CleanupStack::PushL (self);
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusAvaSipObserver::~CMusAvaSipObserver()
    {
    iObservers.Reset();
    iObservers.Close();
    }

// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusAvaSipObserver::ConstructL()
    {
    }

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusAvaSipObserver::CMusAvaSipObserver()
    {
    }

// -----------------------------------------------------------------------------
// A SIP request has been received from the network.
// -----------------------------------------------------------------------------
//
void CMusAvaSipObserver::IncomingRequest(
                                TUint32 aIapId,
                                CSIPServerTransaction* aTransaction )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->IncomingRequest( aIapId,
                                                 aTransaction );
        if( retval == KErrNone)
            {
            return;
            }
        }
    delete aTransaction;        
    }
// -----------------------------------------------------------------------------
// The received SIP request time-outed and it is invalid i.e. cannot be used
// anymore.
// -----------------------------------------------------------------------------
//
void CMusAvaSipObserver::TimedOut(CSIPServerTransaction& aTransaction )
    {
    for( TInt i=0; i<iObservers.Count(); i++ )
        {
        TInt retval( KErrNotFound );
        retval = iObservers[i]->TimedOut( aTransaction );
        if( retval == KErrNone)
            {
            return;
            }
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAvaSipObserver::AddObserverL( MMusAvaSipAdapter& aAdapter, TInt aIndex )
    {
    TInt index = iObservers.Find( &aAdapter );
    
    if( index == KErrNotFound )
        {
        if ( aIndex > KErrNotFound && aIndex < iObservers.Count() )
              {
              iObservers.InsertL( &aAdapter, aIndex );
              }
          else
              {
              iObservers.AppendL( &aAdapter );
              }            
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusAvaSipObserver::RemoveObserver( MMusAvaSipAdapter& aAdapter )
    {
    TInt index = iObservers.Find( &aAdapter );
    
    if( index != KErrNotFound )
        {
        iObservers.Remove( index );
        }
    }

