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
* Description:  Timer service for SVP     
*
*/


#include "svptimer.h"
#include "svptimerobserver.h"


const TInt KMicroSecondsCoefficient = 1000;


// ---------------------------------------------------------------------------
// CSVPTimer::CSVPTimer
// ---------------------------------------------------------------------------
//
CSVPTimer::CSVPTimer( MSVPTimerObserver& aObserver, TInt aTimerId ): 
    CTimer( EPriorityHigh ),
    iObserver( aObserver ),
    iId( aTimerId )
    {
    CActiveScheduler::Add( this );
    }

// ---------------------------------------------------------------------------
// CSVPTimer::CSVPTimer
// ---------------------------------------------------------------------------
//
void CSVPTimer::ConstructL()
    {
    CTimer::ConstructL();
    }

// ---------------------------------------------------------------------------
// CSVPTimer::CSVPTimer
// ---------------------------------------------------------------------------
//
CSVPTimer* CSVPTimer::NewL( MSVPTimerObserver& aObserver,
                            TInt aTimerId )
    {
    CSVPTimer* self = new (ELeave) CSVPTimer( aObserver, aTimerId );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// ---------------------------------------------------------------------------
// CSVPTimer::~CSVPTimer
// ---------------------------------------------------------------------------
//  
CSVPTimer::~CSVPTimer()
    {
    Cancel();
    }

// ---------------------------------------------------------------------------
// CSVPTimer::RunL
// ---------------------------------------------------------------------------
//  
void CSVPTimer::RunL()
    {
    iObserver.TimedOut( iId );
    }

// ---------------------------------------------------------------------------
// CSVPTimer::SetTime
// ---------------------------------------------------------------------------
//  
void CSVPTimer::SetTime( TInt aMilliSeconds, TInt aTimerId )
    {
    if ( IsActive() )
        {
        //iObserver.TimedOut( iId ); // Discard previous request
        Cancel();
        }
        
    iId = aTimerId;
        
    // Milliseconds to microseconds conversion
    const TTimeIntervalMicroSeconds32 time(
        KMicroSecondsCoefficient * aMilliSeconds );
    
    CTimer::After( time ); // NB, CTimer::After calls SetActive
    }

// ---------------------------------------------------------------------------
// CSVPTimer::Stop
// ---------------------------------------------------------------------------
//  
void CSVPTimer::Stop()
    {
    Cancel(); 
    }

// ---------------------------------------------------------------------------
// CSVPTimer::Id
// ---------------------------------------------------------------------------
//  
TInt CSVPTimer::Id() const
    {
    return iId;
    }

//  End of File
