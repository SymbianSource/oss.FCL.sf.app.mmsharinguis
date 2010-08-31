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
#include "musmanagerserverclosetimer.h"

const TUint KMicroSecondsInMilliSecond = 1000; 

// ----------------------------------------------------------------------------
// CMusManagerServerCloseTimer::NewL
// ----------------------------------------------------------------------------
//
CMusManagerServerCloseTimer* CMusManagerServerCloseTimer::NewL()
    {
    CMusManagerServerCloseTimer* self = CMusManagerServerCloseTimer::NewLC ();
    CleanupStack::Pop (self);
    return self;
    }

// ----------------------------------------------------------------------------
// CMusManagerServerCloseTimer::NewLC
// ----------------------------------------------------------------------------
//
CMusManagerServerCloseTimer* CMusManagerServerCloseTimer::NewLC ()
    {
    CMusManagerServerCloseTimer* self = new(ELeave)CMusManagerServerCloseTimer;
    CleanupStack::PushL (self);
    self->ConstructL ();
    return self;
    }
    
// ----------------------------------------------------------------------------
// CMusManagerServerCloseTimer::ConstructL
// ----------------------------------------------------------------------------
//
void CMusManagerServerCloseTimer::ConstructL ()
    {
	User::LeaveIfError (iTimer.CreateLocal());
    }
    
    
// ----------------------------------------------------------------------------
// CMusManagerServerCloseTimer::ConstructL
// ----------------------------------------------------------------------------
//
CMusManagerServerCloseTimer::CMusManagerServerCloseTimer ()
: CActive(EPriorityStandard)
    {
	CActiveScheduler::Add(this);
    }


// ----------------------------------------------------------------------------
// CMusManagerServerCloseTimer::~CMusManagerServerCloseTimer
// ----------------------------------------------------------------------------
//
CMusManagerServerCloseTimer::~CMusManagerServerCloseTimer ()
    {
	CActive::Cancel();
	iTimer.Close();
    }
    
// ----------------------------------------------------------------------------
// CMusManagerServerCloseTimer::StopActiveSchedulerAfter
// ----------------------------------------------------------------------------
//
void CMusManagerServerCloseTimer::StopActiveSchedulerAfter (TUint aMilliSeconds)
    {
    TTimeIntervalMicroSeconds32 time( 
        KMicroSecondsInMilliSecond 
        * aMilliSeconds );
        
    if (!IsActive())
        {
        iTimer.After(iStatus,time);
        SetActive ();
        }
    }

// ----------------------------------------------------------------------------
// CMusManagerServerCloseTimer::RunL
// ----------------------------------------------------------------------------
//
void CMusManagerServerCloseTimer::RunL ()
    {
	CActiveScheduler::Stop();
    }

// ----------------------------------------------------------------------------
// CMusManagerServerCloseTimer::DoCancel
// ----------------------------------------------------------------------------
//	
void CMusManagerServerCloseTimer::DoCancel ()
    {
	iTimer.Cancel();
    }

//  End of File  

