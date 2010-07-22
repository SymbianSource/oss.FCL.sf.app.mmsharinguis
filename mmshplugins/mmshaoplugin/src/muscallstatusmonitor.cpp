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
* Description:  Monitor the status of CS call
*
*/

 
#include "muscallstatusmonitor.h"
#include "mussesseioninformationapi.h"
#include "muslogger.h"

#include <mmtsy_names.h>
#include <e32property.h>


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusCallStatusMonitor* CMusCallStatusMonitor::NewL( 
    const RMobileCall& aCall, 
    MMusTsyPropertyObserver& aObserver,
    MMusCallStateObserver& aCallStateObserver )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallStatusMonitor::NewL" )
    CMusCallStatusMonitor* self = 
        new( ELeave )CMusCallStatusMonitor( aCall, aObserver, aCallStateObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAO]  <- CMusCallStatusMonitor::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusCallStatusMonitor::~CMusCallStatusMonitor()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallStatusMonitor::~CMusCallStatusMonitor" )        
    Cancel();        
    MUS_LOG( "mus: [MUSAO]  <- CMusCallStatusMonitor::~CMusCallStatusMonitor" )
    }

// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusCallStatusMonitor::ConstructL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallStatusMonitor::ConstructL" )   
    CActiveScheduler::Add( this );           
    User::LeaveIfError( iCall.GetMobileCallStatus(iCallStatus) );
    CheckStateL();
    iCall.NotifyMobileCallStatusChange( iStatus, iCallStatus );
    SetActive();        
    MUS_LOG( "mus: [MUSAO]  <- CMusCallStatusMonitor::ConstructL" )
    }

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusCallStatusMonitor::CMusCallStatusMonitor(
    const RMobileCall& aCall, 
    MMusTsyPropertyObserver& aObserver,
    MMusCallStateObserver& aCallStateObserver ) 
    : CMusCallMonitorBase( aCall, aObserver, aCallStateObserver )  
    {    
    }


// -----------------------------------------------------------------------------
// CMusCallStatusMonitor::RunL()
// Implemented for CActive.It will be called any NotifyMobileLineStatusChange
// Event happens.
// -----------------------------------------------------------------------------
//
void CMusCallStatusMonitor::RunL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallStatusMonitor::RunL" )    
    iCall.NotifyMobileCallStatusChange( iStatus, iCallStatus );
    SetActive();       
    CheckStateL();
    MUS_LOG( "mus: [MUSAO]  <- CMusCallStatusMonitor::RunL" )
    }


// -----------------------------------------------------------------------------
// CMusCallStatusMonitor::DoCancel()
// Implemented for CActive.
// Will be called when Cancel() method is called.
// -----------------------------------------------------------------------------
//
void CMusCallStatusMonitor::DoCancel()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallStatusMonitor::DoCancel" )
    iCall.CancelAsyncRequest( EMobileCallNotifyMobileCallStatusChange );
    MUS_LOG( "mus: [MUSAO]  <- CMusCallStatusMonitor::DoCancel" )
    }


// -----------------------------------------------------------------------------
// CMusCallStatusMonitor::SetIntialStateL()
// Sets the Call State to Property
// -----------------------------------------------------------------------------
//

void CMusCallStatusMonitor::CheckStateL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallStatusMonitor::CheckStateL" ) 
    MUS_LOG1( "mus: [MUSAO]  Call->iCallStatus =  %d", iCallStatus )    
    switch ( iCallStatus )    
        {
        case RMobileCall::EStatusConnected:	
            SetStateL( NMusSessionInformationApi::ECallConnected );
            break;
        case RMobileCall::EStatusDisconnecting:                 
        case RMobileCall::EStatusDisconnectingWithInband:
            SetStateL( NMusSessionInformationApi::ENoCall );
            // The following callback causes self deletion:
            NotifyCallStateChanged( NMusSessionInformationApi::ENoCall );
            // Do not use the member variables after this point. 
            break;
        case RMobileCall::EStatusHold:	
            SetStateL( NMusSessionInformationApi::ECallHold );
            break;
        default:           
            break; // No actions
        } 
    MUS_LOG( "mus: [MUSAO]  <- CMusCallStatusMonitor::CheckStateL" )    
    }

// End of file


