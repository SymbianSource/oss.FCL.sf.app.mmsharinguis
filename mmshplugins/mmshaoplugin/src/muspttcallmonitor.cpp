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
* Description:  Monitor that observes the status of the Kodiak PTT call
*
*/


#include "muspttcallmonitor.h"
#include "muslogger.h"
#include <e32property.h>


// --------------------------------------------------------------------------
// Symbian two-phase constructor.
// --------------------------------------------------------------------------
//
CMusPttCallMonitor* CMusPttCallMonitor::NewL(MMusPttEventObserver& aObserver)
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPttCallMonitor::NewL" )
    CMusPttCallMonitor* self = new (ELeave) CMusPttCallMonitor(aObserver);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAO]  <- CMusPttCallMonitor::NewL" )
    return self;
    }


// --------------------------------------------------------------------------
// C++ destructor.
// --------------------------------------------------------------------------
//
CMusPttCallMonitor::~CMusPttCallMonitor()
    {    
    MUS_LOG( "mus: [MUSAO]  -> CMusPttCallMonitor::~CMusPttCallMonitor" )
    Cancel();
    iPropertyEvent.Close();
    MUS_LOG( "mus: [MUSAO]  <- CMusPttCallMonitor::~CMusPttCallMonitor" )    
    }


// --------------------------------------------------------------------------
// C++ constructor.
// --------------------------------------------------------------------------
//
CMusPttCallMonitor::CMusPttCallMonitor(MMusPttEventObserver& aObserver)
    :CActive( EPriorityNormal ),iObserver(aObserver)
    {
    
    }


// --------------------------------------------------------------------------
// Symbian second-phase constructor.
// --------------------------------------------------------------------------
//
void CMusPttCallMonitor::ConstructL()
    {    
    MUS_LOG( "mus: [MUSAO]  -> CMusPttCallMonitor::ConstructL" )
    CActiveScheduler::Add(this);
    
    User::LeaveIfError( iPropertyEvent.Attach(
                                       KPSUidCtsyCallInformation,
                                       KCTsyKodiakPtt) );        
    iPropertyEvent.Subscribe( iStatus );
    SetActive();
    
    MUS_LOG( "mus: [MUSAO]  <- CMusPttCallMonitor::ConstructL" )    
    }

// -----------------------------------------------------------------------------
// CMusPttCallMonitor::RunL()
// -----------------------------------------------------------------------------
//
void CMusPttCallMonitor::RunL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPttCallMonitor::RunL" )
    // resubscribe before processing new value to prevent missing updates
    
    iPropertyEvent.Subscribe( iStatus );
    SetActive();    
    TInt val = KErrNone;
    User::LeaveIfError(iPropertyEvent.Get( val ));      
    iObserver.PttCallEventOccuredL((TPSCTsyKodiakPtt&)val);
    
    MUS_LOG( "mus: [MUSAO]  <- CMusPttCallMonitor::RunL" )
    }


// -----------------------------------------------------------------------------
// CMusPttCallMonitor::DoCancel()
// -----------------------------------------------------------------------------
//
void CMusPttCallMonitor::DoCancel()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPttCallMonitor::DoCancel" )
    iPropertyEvent.Cancel();
    MUS_LOG( "mus: [MUSAO]  <- CMusPttCallMonitor::DoCancel" )
    }


// -----------------------------------------------------------------------------
// CMusPttCallMonitor::IsPTTCallExist()
// -----------------------------------------------------------------------------
//
TBool CMusPttCallMonitor::IsPTTCallExist()
    {    
    MUS_LOG( "mus: [MUSAO]  -> CMusPttCallMonitor::IsPTTCallExist" ) 

    TInt val = KErrNone;
    TInt ret = iPropertyEvent.Get( val ); 
    MUS_LOG1( "mus: [MUSAO]  iPropertyEvent.Get( val ) returns: %d",ret )           
    MUS_LOG1( "mus: [MUSAO]  <- CMusPttCallMonitor::IsPTTCallExist = %d",val )           
    return ( val==EPSCTsyKodiakPttActive ) ? ETrue : EFalse;        
    }

// -----------------------------------------------------------------------------
// CMusCallEventMonitor::RunError( TInt aError )
// Implemented for CActive.
// Will be called automatically by framework when RunL Leaves.
// -----------------------------------------------------------------------------
//

TInt CMusPttCallMonitor::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusCallEventMonitor::RunError, aError %d", aError )
    aError = KErrNone; // just to avoid warnings.
    MUS_LOG( "mus: [MUSAO]  <- CMusCallEventMonitor::RunError" )    
    return aError;
    }



//End of File
