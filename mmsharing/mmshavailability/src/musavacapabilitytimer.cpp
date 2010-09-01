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
* Description:   Timer
*
*/


// INCLUDE FILES
#include "musavacapabilitytimer.h"

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------------------------
// Two-phased constructor.
// ---------------------------------------------------------------------------
//
CMusAvaCapabilityTimer* CMusAvaCapabilityTimer::NewL( 
                    CMusAvaCapabilityTimer::MTimeOutNotify& aNotify )
    {
    CMusAvaCapabilityTimer* self = 
        new ( ELeave ) CMusAvaCapabilityTimer( EPriorityStandard, aNotify );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
CMusAvaCapabilityTimer::~CMusAvaCapabilityTimer()
    {
    Cancel();
    }


// ---------------------------------------------------------------------------
// CMusAvaCapabilityTimer::RunL()
// 
// ---------------------------------------------------------------------------
//
void CMusAvaCapabilityTimer::RunL()
    {
    iNotify->OnExpiredL( iStatus.Int() );
    }

// -------------------------------------------------------------------------
//  If RunL() leaves,It should be handled here.
// -------------------------------------------------------------------------
//
TInt CMusAvaCapabilityTimer::RunError( TInt aError )
    {    
    // Nothing can be done here.
    aError = KErrNone;

    return aError;
    }
    

// ---------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CMusAvaCapabilityTimer::CMusAvaCapabilityTimer( const TInt aPriority, 
                        CMusAvaCapabilityTimer::MTimeOutNotify& aNotify ) 
    : CTimer( aPriority ),
    iNotify( &aNotify )
    {
    }


// ---------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// ---------------------------------------------------------------------------
//
void CMusAvaCapabilityTimer::ConstructL()
    {
    CTimer::ConstructL();
    CActiveScheduler::Add( this );
    }

//  End of File

