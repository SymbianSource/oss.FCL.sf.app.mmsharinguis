/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/


// INCLUDES
#include "musavatimer.h"
#include <e32std.h>

const TUint KMicroSecondsInMilliSecond = 1000; 

// ----------------------------------------------------------------------------
// CMusAvaTimer::NewL
// ----------------------------------------------------------------------------
//
CMusAvaTimer* CMusAvaTimer::NewL()
    {
    CMusAvaTimer* self = CMusAvaTimer::NewLC ();
    CleanupStack::Pop (self);
    return self;
    }

// ----------------------------------------------------------------------------
// CMusAvaTimer::NewLC
// ----------------------------------------------------------------------------
//
CMusAvaTimer* CMusAvaTimer::NewLC ()
    {
    CMusAvaTimer* self = new(ELeave)CMusAvaTimer;
    CleanupStack::PushL (self);
    self->ConstructL ();
    return self;
    }
    
// ----------------------------------------------------------------------------
// CMusAvaTimer::ConstructL
// ----------------------------------------------------------------------------
//
void CMusAvaTimer::ConstructL ()
    {
    CActiveScheduler::Add(this);
	User::LeaveIfError (iTimer.CreateLocal());
    }
    
    
// ----------------------------------------------------------------------------
// CMusAvaTimer::ConstructL
// ----------------------------------------------------------------------------
//
CMusAvaTimer::CMusAvaTimer ()
: CActive(EPriorityHigh)
    {
    }


// ----------------------------------------------------------------------------
// CMusAvaTimer::~CMusAvaTimer
// ----------------------------------------------------------------------------
//
CMusAvaTimer::~CMusAvaTimer ()
    {
	CActive::Cancel();
	iTimer.Close();
    }
    
// ----------------------------------------------------------------------------
// CMusAvaTimer::StopActiveSchedulerAfter
// ----------------------------------------------------------------------------
//
void CMusAvaTimer::StopActiveSchedulerAfter ( TInt aMilliSeconds, 
                                              TRequestStatus* aStatus )
    {
   
        
    if (!IsActive())
        {
        iTimer.After( iStatus, TTimeIntervalMicroSeconds32(aMilliSeconds * 1000) );

        iRequestStatus = aStatus;
        //*iRequestStatus = KRequestPending;
        *iRequestStatus = KErrNone;
        SetActive ();
        TRequestStatus* ptr = &iStatus;
        //User::RequestComplete(ptr, KErrNone);
        //TInt result = KErrNone;
        //CActiveScheduler::RunIfReady( result, EPriorityStandard );
        /*TRequestStatus* ptr = &iStatus;
        iStatus = KRequestPending;
        SetActive();
        User::RequestComplete(ptr, KErrNone);
        TInt result = KErrNone;
        CActiveScheduler::RunIfReady( result, EPriorityStandard );*/
        }
    }

// ----------------------------------------------------------------------------
// CMusAvaTimer::RunL
// ----------------------------------------------------------------------------
//
void CMusAvaTimer::RunL ()
    {
    TRequestStatus* pRS = iRequestStatus;
    User::RequestComplete(pRS, KErrNone );
    }

// ----------------------------------------------------------------------------
// CMusAvaTimer::DoCancel
// ----------------------------------------------------------------------------
//	
void CMusAvaTimer::DoCancel ()
    {
	iTimer.Cancel();
    }

//  End of File  

