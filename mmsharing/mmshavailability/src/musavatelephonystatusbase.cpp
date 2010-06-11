/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:   Provide interface for the client requestin availability class.
*
*/


#include "musavatelephonystatusbase.h"
#include "musavatelephonystatusobserver.h"
#include "musavaavailability.h"
#include "musunittesting.h"
#include "muslogger.h"

#include <e32base.h>


// -------------------------------------------------------------------------
//  Destructor.
// -------------------------------------------------------------------------
//
CMusAvaTelephonyStatusBase::~CMusAvaTelephonyStatusBase()
    {
    CActive::Cancel();
    }

// -----------------------------------------------------------------------------
// Adds new observer
// -----------------------------------------------------------------------------
void CMusAvaTelephonyStatusBase::AddAdapterL( MMusAvaTelephonyStatusObserver& aAdapter )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaTelephonyStatusBase::\
             AddAdapterL( MMusAvaTelephonyStatusObserver& aAdapter )" )
    iObserver.AppendL( &aAdapter );
   
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::\
             AddAdapterL( MMusAvaTelephonyStatusObserver& aAdapter )" )
    }


// -------------------------------------------------------------------------
//  Reads the telephony status.
// -------------------------------------------------------------------------
//
void CMusAvaTelephonyStatusBase::TelephonyStatus()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaTelephonyStatusBase::TelephonyStatus()" )
   
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaTelephonyStatusBase::TelephonyStatus()" )
    }

// -----------------------------------------------------------------------------
// Removes the adapter
// -----------------------------------------------------------------------------
void CMusAvaTelephonyStatusBase::RemoveAdapter( 
                                    MMusAvaTelephonyStatusObserver& aAdapter )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaTelephonyStatusBase::\
             RemoveAdapter( MMusAvaTelephonyStatusObserver& aAdapter )" )
    TInt index = iObserver.Find( &aAdapter );
    
    if( index != KErrNotFound )
        {
        iObserver.Remove( index );
        }
        
    if( iObserver.Count() == 0 )  
        {
        CActive::Cancel();
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::\
             RemoveAdapter( MMusAvaTelephonyStatusObserver& aAdapter )" )
    }
// -------------------------------------------------------------------------
//  Derived from CActive. 
// -------------------------------------------------------------------------
//
void CMusAvaTelephonyStatusBase::RunL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaTelephonyStatusBase::RunL()" )
   
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaTelephonyStatusBase::RunL()" )
    }    

// -------------------------------------------------------------------------
//  Derived from CActive. Called after somebody calls Cancel;
// -------------------------------------------------------------------------
//
void CMusAvaTelephonyStatusBase::DoCancel()
    {
    
    }

// -------------------------------------------------------------------------
//  If RunL() leaves,It should be handled here.
// -------------------------------------------------------------------------
//
TInt CMusAvaTelephonyStatusBase::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSAVA]     -> CMusAvaTelephonyStatusBase::\
              RunError() return #%d", aError )
    
    // Nothing can be done here.
    aError = KErrNone;

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaTelephonyStatusBase::RunError()" )
    return aError;
    }

// -------------------------------------------------------------------------
//  Constructor function.
// -------------------------------------------------------------------------
//
CMusAvaTelephonyStatusBase::CMusAvaTelephonyStatusBase( )
    : CActive( EPriorityStandard )
    {
    CActiveScheduler::Add( this );
    }

