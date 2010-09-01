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
* Description:  class to watch call monitor event properties
*
*/


#include "muspropertymonitor.h"
#include "musmanager.h"
#include "mussesseioninformationapi.h"
#include "muslogger.h"


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusPropertyMonitor* CMusPropertyMonitor::NewL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPropertyMonitor::NewL" )
    CMusPropertyMonitor* self = new (ELeave) CMusPropertyMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAO]  <- CMusPropertyMonitor::NewL" )
    return self;
    }

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusPropertyMonitor::CMusPropertyMonitor()
    :CActive( EPriorityNormal )
    {
    }

// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusPropertyMonitor::ConstructL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPropertyMonitor::ConstructL" )
    CActiveScheduler::Add(this);
    User::LeaveIfError( 
            iPropertyEvent.Attach(
                                NMusSessionInformationApi::KCategoryUid,
                                NMusSessionInformationApi::KMusCallEvent ) );
    MUS_LOG( "mus: [MUSAO] Property attached to KMusCallEvent" )    
    iPropertyEvent.Subscribe( iStatus );
    SetActive();
    MUS_LOG( "mus: [MUSAO]  <- CMusPropertyMonitor::ConstructL" )
    }

// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusPropertyMonitor::~CMusPropertyMonitor()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPropertyMonitor::~CMusPropertyMonitor" )
    Cancel();
    iPropertyEvent.Close(); 
    delete iManager;        
    MUS_LOG( "mus: [MUSAO]  -> <- CMusPropertyMonitor::~CMusPropertyMonitor" )
    }

// -----------------------------------------------------------------------------
// CMusPropertyMonitor::RunL()
// Implemented for CActive.It will be called automatically
// when new property event occurs
// -----------------------------------------------------------------------------
//
void CMusPropertyMonitor::RunL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPropertyMonitor::RunL" )
    // resubscribe before processing new value to prevent missing updates
    iPropertyEvent.Subscribe( iStatus );
    SetActive();    
    TInt value = NMusSessionInformationApi::ENoCall;
    User::LeaveIfError(iPropertyEvent.Get( value ));     
    switch(value)
        {      
        /* CallHold and ConferenceCall are Error Cases.Will be handled by
           Availability Plugin and Inform to AIW.
         */           
        case NMusSessionInformationApi::ECallHold:
             MUS_LOG( "mus: [MUSAO]  CallEvent  = ECallHold" )                 
             break;
        case NMusSessionInformationApi::EConferenceCall:
             MUS_LOG( "mus: [MUSAO]  CallEvent  = EConferenceCall" )                 
             break;
        /* When Call is connected , start the MusClient */
        case NMusSessionInformationApi::ECallConnected:
             MUS_LOG( "mus: [MUSAO]  CallEvent  = ECallConnected" )
             StartMusClientL();
             break;
        /* When Call is disconnected , stop the MusClient */
        case NMusSessionInformationApi::ENoCall:
             MUS_LOG( "mus: [MUSAO]  CallEvent  = ENoCall" )
             StopMusClient();
             break;     
        case NMusSessionInformationApi::EConferenceTerminated:
             MUS_LOG( "mus: [MUSAO]  CallEvent  = EConferenceTerminated" ) 
             break;
        
        default:
             MUS_LOG( "mus: [MUSAO]  CallEvent  = default. Treated ENoCall" )
             StopMusClient();
        }
    MUS_LOG( "mus: [MUSAO]  <- CMusPropertyMonitor::RunL" )
    }


// -----------------------------------------------------------------------------
// CMusPropertyMonitor::DoCancel()
// Implemented for CActive.Will be called when Cancel() method called.
// -----------------------------------------------------------------------------
//
void CMusPropertyMonitor::DoCancel()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPropertyMonitor::DoCancel" )
    iPropertyEvent.Cancel();
    MUS_LOG( "mus: [MUSAO]  <- CMusPropertyMonitor::DoCancel" )
    }

// -----------------------------------------------------------------------------
// CMusPropertyMonitor::StartMusClient()
// This will start the MusManager Client which inturn should start
// MusManager Server and Availability Plugin.
// -----------------------------------------------------------------------------
//
void CMusPropertyMonitor::StartMusClientL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPropertyMonitor::StartMusClient" )
    if( !iManager )
        {
        iManager = CMusManager::NewL();
        }
    iManager->ExamineAvailabilityL();   
    MUS_LOG( "mus: [MUSAO]  <- CMusPropertyMonitor::StartMusClient" ) 
    }

// -----------------------------------------------------------------------------
// CMusPropertyMonitor::StopMusClient()
// This will stop the MusManager Client which inturn should stop
// MusManager Server and Availability Plugin.
// -----------------------------------------------------------------------------
//
void CMusPropertyMonitor::StopMusClient()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPropertyMonitor::StopMusClient" )
    if( iManager )
        {
        delete iManager;
        iManager = NULL;
        }
    MUS_LOG( "mus: [MUSAO]  <- CMusPropertyMonitor::StopMusClient" )
    
    }

// -----------------------------------------------------------------------------
// CMusPropertyMonitor::RunError()
// Implemented for CActive.It will be called automatically
// when a leave occurs in RunL()
// -----------------------------------------------------------------------------
//
TInt CMusPropertyMonitor::RunError(TInt aError)
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusPropertyMonitor::RunError = %d",aError )
    if( iManager )
        {
        delete iManager;
        iManager = NULL;
        }    
    aError = KErrNone; // We handled this error already. So return KErrNone.
    MUS_LOG( "mus: [MUSAO]  <- CMusPropertyMonitor::RunError " )    
    return aError;
    }

// End of file
