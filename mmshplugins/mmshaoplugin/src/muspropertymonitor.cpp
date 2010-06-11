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
#include "mussesseioninformationapi.h"
#include "muslogger.h"


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusPropertyMonitor* CMusPropertyMonitor::NewL( MMusCallStateObserver& aCallStateObserver )  
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPropertyMonitor::NewL" )
    CMusPropertyMonitor* self = new (ELeave) CMusPropertyMonitor( aCallStateObserver );
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
CMusPropertyMonitor::CMusPropertyMonitor( MMusCallStateObserver& aCallStateObserver )
    :CActive( EPriorityNormal ), iCallStateObserver ( aCallStateObserver )
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

    //Check if preconditions are met to start or stop the MushClient.
    iCallStateObserver.MusCallStateChanged();

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
// CMusPropertyMonitor::RunError()
// Implemented for CActive.It will be called automatically
// when a leave occurs in RunL()
// -----------------------------------------------------------------------------
//
TInt CMusPropertyMonitor::RunError(TInt aError)
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusPropertyMonitor::RunError = %d",aError )
    
    // Monitoring Error Occurred, Terminate the MushSession.
    TInt err = RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusCallEvent,
                          NMusSessionInformationApi::ENoCall);

    iCallStateObserver.MusCallStateChanged();

    aError = KErrNone; // We handled this error already. So return KErrNone.
    MUS_LOG( "mus: [MUSAO]  <- CMusPropertyMonitor::RunError " )    
    return aError;
    }

// -----------------------------------------------------------------------------
// CMusPropertyMonitor::IsCallConnected()
// Checks if the call is connected: 
// -----------------------------------------------------------------------------
//
TBool CMusPropertyMonitor::IsCallConnected()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusPropertyMonitor::IsCallConnected" )
    TInt callState = ( TInt ) NMusSessionInformationApi::ENoCall;
    TInt err = KErrNone;
    
    err = RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusCallEvent,
                          callState );
    
    MUS_LOG( "mus: [MUSAO]  <- CMusPropertyMonitor::IsCallConnected" )
    
    return ( err == KErrNone && callState != NMusSessionInformationApi::ENoCall );
    }

// End of file
