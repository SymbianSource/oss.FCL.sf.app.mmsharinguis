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
* Description:   class to watch call monitor event properties
*
*/



#include "musavacalleventmonitor.h"
#include "muslogger.h"
#include "mussesseioninformationapi.h"
#include "musresourceproperties.h"
#include "muscallmonitorobserver.h"
#include <e32property.h>

// --------------------------------------------------------------------------
// Two phase constructor NewL()
// --------------------------------------------------------------------------
//
CMusAvaCallEventMonitor* CMusAvaCallEventMonitor::NewL(
    MMusCallMonitorObserver& aObserver )
    {
    MUS_LOG( "mus: [MUSAOP]  -> CMusAvaCallEventMonitor::NewL" )
    CMusAvaCallEventMonitor* self = 
        new (ELeave) CMusAvaCallEventMonitor( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAOP]  <- CMusAvaCallEventMonitor::NewL" )
    return self;
    }

// --------------------------------------------------------------------------
// Destructor
// --------------------------------------------------------------------------
//
CMusAvaCallEventMonitor::~CMusAvaCallEventMonitor()
    {
    MUS_LOG( "-> CMusAvaCallEventMonitor::~CMusAoPlugin" )
    
    MUS_LOG( "<- CMusAvaCallEventMonitor::~CMusAoPlugin" )
    }

// --------------------------------------------------------------------------
// Constructor 
// --------------------------------------------------------------------------
//
CMusAvaCallEventMonitor::CMusAvaCallEventMonitor( 
    MMusCallMonitorObserver& aObserver )
    :CActive( EPriorityNormal ),
    iObserver( aObserver )
    {
    }

// --------------------------------------------------------------------------
// Two phase constructor ConstructL()
// --------------------------------------------------------------------------
//
void CMusAvaCallEventMonitor::ConstructL()
    {
    MUS_LOG( "mus: [MUSAOP]  -> CMusAvaCallEventMonitor::ConstructL" )
    CActiveScheduler::Add( this );
    MUS_LOG( "mus: [MUSAOP]  <- CMusAvaCallEventMonitor::ConstructL" )
    }


// --------------------------------------------------------------------------
// Returns the status of the call
// --------------------------------------------------------------------------
//
TInt CMusAvaCallEventMonitor::CallStatusL()
    {
    MUS_LOG( "mus: [CMusAvaCallEventMonitor]  -> CMusAvaCallEventMonitor::\
        CallStatusL" )    
    TInt callStatus;
    User::LeaveIfError( RProperty::Get(
        NMusSessionInformationApi::KCategoryUid,
        NMusSessionInformationApi::KMusCallEvent,
        callStatus ) );

    MUS_LOG1( "mus: [CMusAvaCallEventMonitor]     Call Event = %d ", 
        callStatus )    
    MUS_LOG( "mus: [CMusAvaCallEventMonitor]  <- CMusAvaCallEventMonitor::\
        CallStatusL" )    
    return callStatus;
    }

// --------------------------------------------------------------------------
// Returns the direction of connected call.
// --------------------------------------------------------------------------
//
TInt CMusAvaCallEventMonitor::CallDirectionL()
    {
    MUS_LOG( "mus: [CMusAvaCallEventMonitor]  -> CMusAvaCallEventMonitor::\
        CallDirectionL" )
    // retrieve call count
    TInt direction( 0 );
    User::LeaveIfError( RProperty::Get(
        NMusSessionInformationApi::KCategoryUid,
        NMusSessionInformationApi::KMusCallDirection,
        direction ) );

    MUS_LOG1( "mus: [CMusAvaCallEventMonitor]     Call Direction = %d ", 
        direction )
    MUS_LOG( "mus: [CMusAvaCallEventMonitor]  <- CMusAvaCallEventMonitor::\
        CallDirectionL" )
    return direction;
    }
    
// --------------------------------------------------------------------------
// Returns the direction of connected call.
// --------------------------------------------------------------------------
//
TInt CMusAvaCallEventMonitor::CallPrivacyL()
    {
    MUS_LOG( "mus: [CMusAvaCallEventMonitor]  -> CMusAvaCallEventMonitor::\
            CallPrivacyL" )
    // retrieve call count
    TInt privacy( 0 );
    User::LeaveIfError( RProperty::Get(
        NMusSessionInformationApi::KCategoryUid,
        NMusSessionInformationApi::KMUSPrivacy,
        privacy ) );

    MUS_LOG1( "mus: [CMusAvaCallEventMonitor]     Call privacy = %d ", 
            privacy )
    MUS_LOG( "mus: [CMusAvaCallEventMonitor]  <- CMusAvaCallEventMonitor::\
        CallPrivacyL" )
    return privacy;
    }
	
// --------------------------------------------------------------------------
// aTelNumber will be filled with telephone number of current connected call
// --------------------------------------------------------------------------
//
void CMusAvaCallEventMonitor::GetTelNumberL( TDes& aTelNumber )
    {
    MUS_LOG( "mus: [CMusAvaCallEventMonitor]  -> CMusAvaCallEventMonitor::\
        GetTelNumberL" )
    TBuf<KMusTelNumberMaxLength> telNumber;

    MUS_LOG( "mus: [CMusAvaCallEventMonitor]  CMusAvaCallEventMonitor::\
        GetTelNumberL - a" )
    TInt error = RProperty::Get(
        NMusSessionInformationApi::KCategoryUid,
        NMusSessionInformationApi::KMusTelNumber,
        telNumber );

    MUS_LOG1( "mus: [CMusAvaCallEventMonitor]  CMusAvaCallEventMonitor::\
        GetTelNumberL (%d)", error )

    if ( error != KErrOverflow )
        {
        User::LeaveIfError( error );
        }

    TInt length = telNumber.Length();
    TInt maxLength = aTelNumber.MaxLength();
    if ( maxLength < length )
        {
        // buffer is too small for returned tel number.
        MUS_LOG2( "mus: [CMusAvaCallEventMonitor]  CMusAvaCallEventMonitor::\
            GetTelNumberL - Buffer too small (%d, %d), leave", 
            maxLength, length )
        User::Leave( KErrOverflow );
        }

    aTelNumber.Copy( telNumber );
    MUS_LOG( "mus: [CMusAvaCallEventMonitor]  <- CMusAvaCallEventMonitor::\
        GetTelNumberL" )
    }


// --------------------------------------------------------------------------
// Subscribe to the property, means start monitoring the property
// --------------------------------------------------------------------------
//
void CMusAvaCallEventMonitor::StartMonitoringL()
    {
    MUS_LOG( "mus: [CMusAvaCallEventMonitor]  -> CMusAvaCallEventMonitor::\
        StartMonitoringL" )
    if ( !IsActive() )
        {
        MUS_LOG( "mus: [CMusAvaCallEventMonitor]   CMusAvaCallEventMonitor::\
            StartMonitoringL - Not active, starting" )
        User::LeaveIfError( iPropertyEvent.Attach(
            NMusSessionInformationApi::KCategoryUid,
            NMusSessionInformationApi::KMusCallEvent ) );
        // initial subscription and process current property value
        iPropertyEvent.Subscribe( iStatus );
        SetActive();
        }
    MUS_LOG( "mus: [CMusAvaCallEventMonitor]  <- CMusAvaCallEventMonitor::\
        StartMonitoringL" )
    }


// --------------------------------------------------------------------------
// Called by Cancel()
// --------------------------------------------------------------------------
//
void CMusAvaCallEventMonitor::DoCancel()
    {
    iPropertyEvent.Cancel();
    iPropertyEvent.Close();
    }


// --------------------------------------------------------------------------
// Active Object RunL implementation. Will be called when property changes.
// --------------------------------------------------------------------------
//
void CMusAvaCallEventMonitor::RunL()
    {
    MUS_LOG( "mus: [MUSAOP]  -> CMusAvaCallEventMonitor::RunL" )

    // resubscribe before processing new value to prevent missing updates
    iPropertyEvent.Subscribe( iStatus );
    SetActive();

    // property updated, get new value
    TInt callStatus;
    if ( iPropertyEvent.Get( callStatus ) == KErrNotFound )
        {
        // property deleted
        iObserver.NoActiveCallL();
        }
    else
        {
        switch( callStatus )
            {
            case NMusSessionInformationApi::ENoCall:
                {
                iObserver.NoActiveCallL();
                break;
                }

            case NMusSessionInformationApi::ECallConnected:
                {
                // retrieve telnumber
                TInt error;
                TBuf<KMusTelNumberMaxLength> telNumber;
                error = RProperty::Get(
                    NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMusTelNumber,
                    telNumber );

                if ( error != KErrOverflow )
                    {
                    User::LeaveIfError( error );
                    }

                iObserver.CallConnectedL( telNumber );
                break;
                }

            case NMusSessionInformationApi::ECallHold:
                {
                 // retrieve telnumber
                TInt error;
                TBuf<KMusTelNumberMaxLength> telNumber;
                error = RProperty::Get(
                    NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMusTelNumber,
                    telNumber );

                if ( error != KErrOverflow )
                    {
                    User::LeaveIfError( error );
                    }
                iObserver.CallHoldL( telNumber );
                break;
                }

            case NMusSessionInformationApi::EConferenceCall:
                {
                iObserver.ConferenceCallL();
                break;
                }
            case NMusSessionInformationApi::EConferenceTerminated:
            	{
            	iObserver.ConferenceCallLTerminated();
            	break;
            	}

            default:
                {
                iObserver.NoActiveCallL();
                break;
                }
            }
        }

    MUS_LOG( "mus: [MUSAOP]  <- CMusAvaCallEventMonitor::RunL" )
    }


// --------------------------------------------------------------------------
// RunL leaves will be handled here. Make sure it returns KErrNone
// Otherwise Active Scheduler Error() will be called.
// --------------------------------------------------------------------------
//
TInt CMusAvaCallEventMonitor::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSAOP]  -> CMusAvaCallEventMonitor::RunError(%d)",
        aError )

    // Not much things can be done here as cleanup operation.
    aError = KErrNone;

    MUS_LOG( "mus: [MUSAOP]  <- CMusAvaCallEventMonitor::RunError" )
    return aError;
    }
