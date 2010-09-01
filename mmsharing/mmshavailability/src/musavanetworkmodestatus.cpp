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



#include "musavanetworkmodestatus.h"
#include "musavatelephonystatusobserver.h"
#include "musavaavailability.h"
#include "musunittesting.h"
#include "mussettingskeys.h"
#include "mussettings.h"
#include "muslogger.h"

#include <e32base.h>
#include <mmtsy_names.h>

// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaNetworkModeStatus* CMusAvaNetworkModeStatus::NewL()
    {
    CMusAvaNetworkModeStatus* self = 
    new( ELeave ) CMusAvaNetworkModeStatus();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }


// -------------------------------------------------------------------------
//  Destructor.
// -------------------------------------------------------------------------
//
CMusAvaNetworkModeStatus::~CMusAvaNetworkModeStatus()
    {
    Cancel();
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
// Test Current Phone Network Mode
// -------------------------------------------------------------------------
//
RMobilePhone::TMobilePhoneNetworkMode CMusAvaNetworkModeStatus::PhoneNetworkMode()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkModeStatus::PhoneNetworkMode()" )
    RMobilePhone::TMobilePhoneNetworkMode networkMode;    
    TInt retval = iPhone.GetCurrentMode( networkMode );    
    MUS_LOG1( "mus: [MUSAVA]  <- CMusAvaNetworkModeStatus::PhoneNetworkMode()=%d",networkMode )
    return networkMode;
    }
    

// -------------------------------------------------------------------------
//  Reads the telephony status.
// -------------------------------------------------------------------------
//
void CMusAvaNetworkModeStatus::TelephonyStatus()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkModeStatus::TelephonyStatus()" )
    MUS_LOG( "mus: [MUSAVA] NotifyModeChange" )
    iPhone.NotifyModeChange( iStatus, iNetworkMode );
    SetActive();
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkModeStatus::TelephonyStatus()" )
    }

// -------------------------------------------------------------------------
// 
// -------------------------------------------------------------------------
//    
void CMusAvaNetworkModeStatus::PhoneNetworkModeStatus( 
                    RMobilePhone::TMobilePhoneNetworkMode aStatus )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkModeStatus::\
    PhoneNetworkModeStatus()" )
    MUS_LOG1( "TMobilePhoneNetworkMode = ", aStatus )

    for( TInt i=0; i<iObserver.Count(); i++ )
        {
        iObserver[i]->PhoneNetworkModeStatus( aStatus );
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaNetworkModeStatus::\
    PhoneNetworkModeStatus()" )
    }

// -------------------------------------------------------------------------
//  Reads the call status property value and subscribes again.
// -------------------------------------------------------------------------
//
void CMusAvaNetworkModeStatus::RunL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkModeStatus::RunL()" )
    if( iStatus == KErrNone )
        {
        PhoneNetworkModeStatus( iNetworkMode );
        }
    //Subscribes registration status again.  
    MUS_LOG( "mus: [MUSAVA]  Subscribes registration status again" )
    TelephonyStatus();
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaNetworkModeStatus::RunL()" )
    }
    
    
// -------------------------------------------------------------------------
//  Cancel pendig request
// -------------------------------------------------------------------------
//
void CMusAvaNetworkModeStatus::DoCancel()    
    {
    iPhone.CancelAsyncRequest( EMobilePhoneNotifyModeChange );
    }
    

// -------------------------------------------------------------------------
//  Constructor function.
// -------------------------------------------------------------------------
//
CMusAvaNetworkModeStatus::CMusAvaNetworkModeStatus( )
    {
   
    }

// -------------------------------------------------------------------------
//  Second phase constructor.
// -------------------------------------------------------------------------
//
void CMusAvaNetworkModeStatus::ConstructL()
    {
    RTelServer::TPhoneInfo phoneInfo;
   	User::LeaveIfError( iTelServer.Connect() ); 
    User::LeaveIfError( iTelServer.LoadPhoneModule( KMmTsyModuleName ) );
	User::LeaveIfError( iTelServer.GetPhoneInfo( 0, phoneInfo ) );
	User::LeaveIfError( iPhone.Open( iTelServer, phoneInfo.iName ) );
    User::LeaveIfError( iPhone.Initialise() );
    TelephonyStatus();
    }
