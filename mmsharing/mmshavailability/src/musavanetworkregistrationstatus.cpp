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



#include "musavanetworkregistrationstatus.h"
#include "musavatelephonystatusobserver.h"
#include "musavaavailability.h"
#include "musunittesting.h"
#include "muslogger.h"

#include <e32base.h>
#include <mmtsy_names.h>

// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaNetworkRegistrationStatus* CMusAvaNetworkRegistrationStatus::NewL()
    {
    CMusAvaNetworkRegistrationStatus* self = 
    new( ELeave ) CMusAvaNetworkRegistrationStatus();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -------------------------------------------------------------------------
//  Destructor.
// -------------------------------------------------------------------------
//
CMusAvaNetworkRegistrationStatus::~CMusAvaNetworkRegistrationStatus()
    {
    iObserver.Reset();
    iObserver.Close();
    iPhone.Close();
    if( iTelServer.Handle() )
    	{
        iTelServer.UnloadPhoneModule( KMmTsyModuleName );
        iTelServer.Close();
    	}
    }

    
// -------------------------------------------------------------------------
// Test Current network registration status
// -------------------------------------------------------------------------
//
void CMusAvaNetworkRegistrationStatus::NetworkRegistration( 
                    RMobilePhone::TMobilePhoneRegistrationStatus& aStatus )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkRegistrationStatus::\
    NetworkRegistrationStatus()" )
    
    TRequestStatus status; 
    iPhone.GetNetworkRegistrationStatus( status, aStatus );    
    #ifndef UNIT_TESTING
    User::WaitForRequest( status );
    #endif
    MUS_LOG1( "mus: [MUSAVA]     RMobilePhone::\
    GetNetworkRegistrationStatus %d", aStatus )
    
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaNetworkRegistrationStatus::\
    NetworkRegistrationStatus()" )
    }

// -------------------------------------------------------------------------
//  Reads the telephony status.
// -------------------------------------------------------------------------
//
void CMusAvaNetworkRegistrationStatus::TelephonyStatus()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkRegistrationStatus::TelephonyStatus()" )
    MUS_LOG( "mus: [MUSAVA] NotifyNetworkRegistrationStatusChange" )
    iPhone.NotifyNetworkRegistrationStatusChange( iStatus, iRegistrationStatus );
    SetActive();
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkRegistrationStatus::TelephonyStatus()" )
    }

// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
//    
void CMusAvaNetworkRegistrationStatus::NetworkRegistrationStatus( 
                    RMobilePhone::TMobilePhoneRegistrationStatus aRegStatus )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkRegistrationStatus::RegistrationStatus()" )
    MUS_LOG1( "TRegistrationStatus = ", aRegStatus )

    for( TInt i=0; i<iObserver.Count(); i++ )
        {
        iObserver[i]->NetworkRegistrationStatus( aRegStatus );
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaNetworkRegistrationStatus::NetworkRegistrationStatus()" )
    }

// -------------------------------------------------------------------------
//  Reads the call status property value and subscribes again.
// -------------------------------------------------------------------------
//
void CMusAvaNetworkRegistrationStatus::RunL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkRegistrationStatus::RunL()" )
    if( iStatus==KErrNone )
        {
        NetworkRegistrationStatus( iRegistrationStatus );
        }
    //Subscribes registration status again.  
    MUS_LOG( "mus: [MUSAVA]  Subscribes registration status again" )
    TelephonyStatus();
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkRegistrationStatus::RunL()" )
    }

// -------------------------------------------------------------------------
//  Constructor function.
// -------------------------------------------------------------------------
//
CMusAvaNetworkRegistrationStatus::CMusAvaNetworkRegistrationStatus( )
    {
   
    }

// -------------------------------------------------------------------------
//  Second phase constructor.
// -------------------------------------------------------------------------
//
void CMusAvaNetworkRegistrationStatus::ConstructL()
    {
    RTelServer::TPhoneInfo phoneInfo;
    User::LeaveIfError( iTelServer.Connect() ); 
    User::LeaveIfError( iTelServer.LoadPhoneModule( KMmTsyModuleName ) );
	User::LeaveIfError( iTelServer.GetPhoneInfo( 0, phoneInfo ) );
	User::LeaveIfError( iPhone.Open( iTelServer, phoneInfo.iName ) );
    User::LeaveIfError( iPhone.Initialise() );
    }
