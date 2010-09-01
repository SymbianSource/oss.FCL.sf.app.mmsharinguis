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
* Description:  Monitor that observes the status of the current CS call
*
*/


#include "mussesseioninformationapi.h"
#include "muspttcallmonitor.h"
#include "muscallmonitor.h"
#include "muscallstatusmonitor.h"
#include "muscalleventmonitor.h"
#include "musresourceproperties.h"
#include "muslogger.h"

#include <etelmm.h>
#include <e32property.h>



// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusCallMonitor* CMusCallMonitor::NewL( TName& aCallName,TBool aPttCallExist )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitor::NewL" )
    CMusCallMonitor* self = new (ELeave) CMusCallMonitor( aPttCallExist );
    CleanupStack::PushL( self );
    self->ConstructL( aCallName );
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitor::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusCallMonitor::~CMusCallMonitor()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitor::~CMusCallMonitor" )
    delete iCallStatusMonitor;        
    delete iCallEventMonitor;    
    iCall.Close();  
    // callcount decrease by one and if leave happens here mostly 
    // property Key not exist,so do not care.
    TRAP_IGNORE(SetCallCountL (EFalse)); 
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitor::~CMusCallMonitor" )
    }


// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusCallMonitor::CMusCallMonitor( TBool aPttCallExist ) 
    :iPttCallExist(aPttCallExist)  
    {    
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusCallMonitor::ConstructL(TName& aCallName)
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitor::ConstructL" )      
    iCallName = aCallName;
    SetCallCountL (ETrue); // increase by one.
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitor::ConstructL" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusCallMonitor::StartMonitorL(RMobileLine& aLine, MMusTsyPropertyObserver& aObserver)
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitor::StartMonitorL" )
    if(IsMonitored())User::Leave(KErrAlreadyExists);    
    User::LeaveIfError( iCall.OpenExistingCall( aLine, iCallName ) );        
    iCallStatusMonitor = CMusCallStatusMonitor::NewL( iCall, aObserver );
    iCallEventMonitor = CMusCallEventMonitor::NewL( iCall, aObserver );
    
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitor::StartMonitorL" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusCallMonitor::StopMonitor()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitor::StopMonitorL" )    
    delete iCallStatusMonitor;
    iCallStatusMonitor=NULL;
    delete iCallEventMonitor;
    iCallEventMonitor=NULL;   
    iCall.Close();  
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitor::StopMonitorL" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusCallMonitor::IsEqual(TName& aName)
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitor::IsEqual" )
    MUS_LOG_TDESC( "mus: [MUSAO]  -> aName : ", aName );
    MUS_LOG_TDESC( "mus: [MUSAO]  -> iCallName : ", iCallName );
    TBool val = EFalse ;    
    val = ( aName == iCallName );    
    MUS_LOG1( "mus: [MUSAO]  <- CMusCallMonitor::IsEqual = %d",val )
    return val;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TName CMusCallMonitor::Name()
    {
    MUS_LOG( "mus: [MUSAO]  <-> CMusCallMonitor::Name" )    
    MUS_LOG_TDESC( "mus: iCallName : ", iCallName );
    return iCallName;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusCallMonitor::IsMonitored()
    {
    MUS_LOG( "mus: [MUSAO]  <-> CMusCallMonitor::IsMonitored" )        
    return (iCallStatusMonitor || iCallEventMonitor)
                            ?ETrue :EFalse;    
    }

// -----------------------------------------------------------------------------
// Sets the call count based on callmonitors.
// ETrue to increase. EFalse to decrease. 
// -----------------------------------------------------------------------------
//
void CMusCallMonitor::SetCallCountL(TBool aIncrease)
    {
    // Get Current Call count
    TInt callCount = 0;
    User::LeaveIfError(RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMusCallCount,callCount));
    // Increment by one
    ( aIncrease ) ? callCount++ : callCount--;
    /* Publish this value through internal mush PS key.So that when
     * other call events happen , we aware about number of active calls.
     * Set this first.Once object is created.
     */
    User::LeaveIfError(RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                           NMusSessionInformationApi::KMusCallCount,
                                           callCount ));  
    }

// -----------------------------------------------------------------------------
// CMusCallMonitor::SetStateL()
// Set state of call status monitor and call event monitor
// -----------------------------------------------------------------------------
//
void CMusCallMonitor::SetStateL(NMusSessionInformationApi::TMusCallEvent aVal)
    {
    iCallStatusMonitor->SetStateL( aVal );
    iCallEventMonitor->SetStateL( aVal );
    }


// End of file
