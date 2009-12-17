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
* Description:   Provide interface for the client requestin availability class.
*
*/



#include "musavanetworkavailability.h"
#include "musavaavailabilityobserver.h"
#include "musavasettingsimp.h"
#include "musavanetworkavailability.h"
#include "mussesseioninformationapi.h"
#include "musavaobserver.h"
#include "musavasharedobject.h"
#include "muslogger.h"
#include "musavacalleventmonitor.h"

#include <e32base.h>


CMusAvaNetworkAvailability* CMusAvaNetworkAvailability::NewL(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    {
    CMusAvaNetworkAvailability* self =
        CMusAvaNetworkAvailability::NewLC(
            aObserver,
            aSettings );

    CleanupStack::Pop( self );
    return self;
    }


CMusAvaNetworkAvailability* CMusAvaNetworkAvailability::NewLC(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    {
    CMusAvaNetworkAvailability* self =
        new( ELeave ) CMusAvaNetworkAvailability(
            aObserver,
            aSettings );

    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


CMusAvaNetworkAvailability::~CMusAvaNetworkAvailability()
    {
    if ( iPhoneStatus )
        {
        iPhoneStatus->Cancel();
        }
    delete iPhoneStatus;
    iPhoneStatus = NULL;
    }


CMusAvaNetworkAvailability::CMusAvaNetworkAvailability(
    MMusAvaAvailabilityObserver& aObserver,
    CMusAvaSettingsImp& aSettings )
    :CMusAvaAvailability( aObserver ),
    iSettings( aSettings )
    {
    }


void CMusAvaNetworkAvailability::ConstructL()
    {
    iPhoneStatus = CMusAvaCallEventMonitor::NewL( *this );
    }


// -------------------------------------------------------------------------
//  Starts the loopy execution.
// -------------------------------------------------------------------------
//
void CMusAvaNetworkAvailability::DoExecuteL()
    {
     MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkAvailability::\
     Execute()" )

    // read the current value and subscribe for changes
    // that's done in RunL    
    switch (iPhoneStatus->CallStatusL())
        {
            case NMusSessionInformationApi::ECallConnected:                 
                {
                 TBuf<CMusAvaCallEventMonitor::KMusTelNumberMaxLength> telNumber;
                 iPhoneStatus->GetTelNumberL( telNumber );
                 iSettings.SetTelNumberL( telNumber );
                 iSettings.SetCallDirection( iPhoneStatus->CallDirectionL() );
                 iSettings.SetCallPrivacy( iPhoneStatus->CallPrivacyL() );
        
                 SetState( MMusAvaObserver::EMusAvaStatusAvailable );
                 break;
                }      
            case NMusSessionInformationApi::ECallHold:  
                 SetState( MMusAvaObserver::EMusAvaStatusCallOnHold );
                 break;
            case NMusSessionInformationApi::EConferenceCall:
                 SetState( MMusAvaObserver::EMusAvaStatusConferenceCall );
                 break;
            case NMusSessionInformationApi::ENoCall:
                 SetState( MMusAvaObserver::EMusAvaStatusNoCall );
                 break;
            default:
                 SetState( MMusAvaObserver::EMusAvaStatusNoCall );
                 break;
        }

    // subscribe for the future changes.
    iPhoneStatus->StartMonitoringL();

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaNetworkAvailability::\
                ExecutePatternL()" )
    }


void CMusAvaNetworkAvailability::Stop()
    {
    iPhoneStatus->Cancel();
    SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
    }


MMusAvaObserver::TAvailabilityName CMusAvaNetworkAvailability::Name()
    {
    return MMusAvaObserver::EMusAvaNameNetworkStatus;
    }

// -------------------------------------------------------------------------
//  This function is called when a cs call is connected.
// -------------------------------------------------------------------------
//
void CMusAvaNetworkAvailability::CallConnectedL( const TDesC& aTelNumber )
    {
    MUS_LOG( "mus: [MUSAVA]  \
                -> CMusAvaNetworkAvailability::CallConnectedL()" )
    // try to resolve to contact information of the remote host

    iSettings.SetTelNumberL( aTelNumber );
    iSettings.SetCallDirection( iPhoneStatus->CallDirectionL() );
    iSettings.SetCallPrivacy( iPhoneStatus->CallPrivacyL() );
    SetState( MMusAvaObserver::EMusAvaStatusAvailable );

    MUS_LOG( "mus: [MUSAVA]  \
                <- CMusAvaNetworkAvailability::CallConnectedL()" )
    }


// -------------------------------------------------------------------------
//  This function is called when a cs call goes into a state
//  that is not connected.
// -------------------------------------------------------------------------
//
void CMusAvaNetworkAvailability::ConferenceCallL()
    {
    MUS_LOG( "mus: [MUSAVA]  \
            -> CMusAvaNetworkAvailability::ConferenceCallL()" )
    //Report line status
    SetState( MMusAvaObserver::EMusAvaStatusConferenceCall );
    MUS_LOG( "mus: [MUSAVA]  \
                <- CMusAvaNetworkAvailability::ConferenceCallL()" )
    }


// -------------------------------------------------------------------------
//  This function is called when a cs call goes hold
// -------------------------------------------------------------------------
//
void CMusAvaNetworkAvailability::CallHoldL( const TDesC& aTelNumber )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkAvailability::CallHoldL()" )
    //Report line status
    iSettings.SetTelNumberL( aTelNumber );
    iSettings.SetCallDirection( iPhoneStatus->CallDirectionL() );
    iSettings.SetCallPrivacy( iPhoneStatus->CallPrivacyL() );
    SetState( MMusAvaObserver::EMusAvaStatusCallOnHold );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaNetworkAvailability::CallHoldL()" )
    }


// -------------------------------------------------------------------------
//  This function is called when a cs call goes into a state
//  that is not connected.
// -------------------------------------------------------------------------
//
void CMusAvaNetworkAvailability::NoActiveCallL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkAvailability::NoActiveCallL()" )

    iSettings.SetTelNumberL( KNullDesC() );

    //Report line status
    //Availability status
    SetState( MMusAvaObserver::EMusAvaStatusNoCall );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaNetworkAvailability::NoActiveCallL()" )
    }
