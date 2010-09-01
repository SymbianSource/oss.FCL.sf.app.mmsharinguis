/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Monitor that observes the calling line identity restriction
*                setting
*
*/


#include "musclirmonitor.h"
#include "muslogger.h"
#include "mussesseioninformationapi.h"
#include <mmtsy_names.h>
#include <e32property.h>

const TInt KMaxErrorsBeforeStop = 5;

using namespace NMusSessionInformationApi;

// -----------------------------------------------------------------------------
// CMusClirMonitor::NewL
// -----------------------------------------------------------------------------
//
CMusClirMonitor* CMusClirMonitor::NewL()
    {
    MUS_LOG( "mus: [CALLMN]  -> CMusClirMonitor::NewL" )
    
    CMusClirMonitor* self = new (ELeave) CMusClirMonitor;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [CALLMN]  <- CMusClirMonitor::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// CMusClirMonitor::~CMusClirMonitor
// -----------------------------------------------------------------------------
//
CMusClirMonitor::~CMusClirMonitor()
    {
    MUS_LOG( "mus: [CALLMN]  -> CMusClirMonitor::~CMusClirMonitor" )

    Cancel();
    delete iTelephony;
    
    MUS_LOG( "mus: [CALLMN]  <- CMusClirMonitor::~CMusClirMonitor" )
    }


// -----------------------------------------------------------------------------
// CMusClirMonitor::CMusClirMonitor
// Read the parameter's current value
// -----------------------------------------------------------------------------
//
CMusClirMonitor::CMusClirMonitor() :
    CActive( EPriorityNormal ),
    iTIdentityServiceV1Pckg( iTIdentityServiceV1 ),
    iAllowed( ETrue ),
    iErrorCount( 0 )
    {
    MUS_LOG( "mus: [CALLMN]  -> CMusClirMonitor::CMusClirMonitor" )

    CActiveScheduler::Add( this );

    MUS_LOG( "mus: [CALLMN]  <- CMusClirMonitor::CMusClirMonitor" )
    }


// -----------------------------------------------------------------------------
// CMusClirMonitor::ConstructL
// -----------------------------------------------------------------------------
//
void CMusClirMonitor::ConstructL()
    {
    iTelephony = CTelephony::NewL();
    ReadClir(); // Read the initial value
    }


// -----------------------------------------------------------------------------
// CMusClirMonitor::RunL
// -----------------------------------------------------------------------------
//
void CMusClirMonitor::RunL()
    {
    MUS_LOG1( "mus: [CALLMN] -> CMusClirMonitor::RunL iStatus=%d",
              iStatus.Int() )

    if ( iStatus == KErrNone )
        {
        TranslateResult( iTIdentityServiceV1.iIdentityStatus );
        iErrorCount = 0;
        }
    else
        {
        ++iErrorCount;
        MUS_LOG1( "mus: [CALLMN]  Failed, error count(%d)", iErrorCount )

        // If reading fails, retry few times
        if ( iErrorCount <= KMaxErrorsBeforeStop )
            {
            ReadClir();
            }
        }

    MUS_LOG( "mus: [CALLMN] <- CMusClirMonitor::RunL" )    
    }


// -----------------------------------------------------------------------------
// CMusClirMonitor::DoCancel
// -----------------------------------------------------------------------------
//
void CMusClirMonitor::DoCancel()
    {
    MUS_LOG( "mus: [CALLMN]  -> CMusClirMonitor::DoCancel" )

    iTelephony->CancelAsync( CTelephony::EGetIdentityServiceStatusCancel );

    MUS_LOG( "mus: [CALLMN]  <- CMusClirMonitor::DoCancel" )
    }


// -----------------------------------------------------------------------------
// CMusClirMonitor::RunError
// Ignore errors and continue
// -----------------------------------------------------------------------------
//
TInt CMusClirMonitor::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [CALLMN]  <-> CMusClirMonitor::RunError aError %d", aError )
    aError = KErrNone; // Get rid of "aError is not used" warning in UREL
    return aError;
    }


// -----------------------------------------------------------------------------
// CMusClirMonitor::ReadClir
// -----------------------------------------------------------------------------
//
void CMusClirMonitor::ReadClir()
    {
    MUS_LOG( "mus: [CALLMN]  -> CMusClirMonitor::ReadClir" )

    if ( !IsActive( ))
        {
        CTelephony::TIdentityService condition =
            CTelephony::EIdServiceCallerRestriction;
        iTelephony->GetIdentityServiceStatus( iStatus,
                                              condition,
                                              iTIdentityServiceV1Pckg );
        SetActive();
        }
    
    MUS_LOG( "mus: [CALLMN]  <- CMusClirMonitor::ReadClir" )
    }


// -----------------------------------------------------------------------------
// CMusClirMonitor::TranslateResult
// -----------------------------------------------------------------------------
//
void
CMusClirMonitor::TranslateResult( CTelephony::TIdentityServiceStatus aResult )
    {
    MUS_LOG1( "mus: [CALLMN]  -> CMusClirMonitor::TranslateResult aResult=%d",
              aResult )

    switch ( aResult )
        {
        case CTelephony::EIdServiceActivePermanent:
        case CTelephony::EIdServiceActiveDefaultRestricted:
            iAllowed = EFalse;
            break;

        case CTelephony::EIdServiceActiveDefaultAllowed:
        case CTelephony::EIdServiceNotProvisioned:
            iAllowed = ETrue;
            break;

        case CTelephony::EIdServiceUnknown:
        default:
            ; // retain current value
        }

    TMusClirSetting sendNumber( EDoNotSendOwnNumber );
    if ( iAllowed )
        {
        sendNumber = ESendOwnNumber;
        }
    RProperty::Set( KCategoryUid, KMusClirSetting, sendNumber );
            
    MUS_LOG1( "mus: [CALLMN]  <- CMusClirMonitor::TranslateResult allowed=%d",
              iAllowed )
    }

// end of file
