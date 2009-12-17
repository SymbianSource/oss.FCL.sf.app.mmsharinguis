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



#include "musavaconnectionmonitor.h"
#include "musavasettingsimp.h"
#include "musavanetworkavailability.h"
#include "musavaobserver.h"
#include "musavaavailability.h"
#include "musavasharedobject.h"
#include "musavaconnectionmonitorobserver.h"
#include "musavasip.h"
#include "muslogger.h"

#include <e32base.h>
#include <sipprofile.h>
#include <mmtsy_names.h>

// -------------------------------------------------------------------------
//  Two-phased constructor.
// -------------------------------------------------------------------------
//
CMusAvaConnectionMonitor* CMusAvaConnectionMonitor::NewL( 
                                        CMusAvaSharedObject& aSharedObject )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionMonitor::NewL()" )
    CMusAvaConnectionMonitor* self = 
        new( ELeave ) CMusAvaConnectionMonitor( aSharedObject );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionMonitor::NewL()" )
    return self;
    }


// -------------------------------------------------------------------------
//  Destructor.
// -------------------------------------------------------------------------
//
CMusAvaConnectionMonitor::~CMusAvaConnectionMonitor()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionMonitor::\
        ~CMusAvaConnectionMonitor()" )
    iConnectionMonitorObserver.Reset();
    iConnectionMonitorObserver.Close();
    // Close the monitor when event notifications are no longer required
    iConnectionMonitor.Close();

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionMonitor::\
        ~CMusAvaConnectionMonitor()" )
    }

// -------------------------------------------------------------------------
//  Constructor function.
// -------------------------------------------------------------------------
//
void CMusAvaConnectionMonitor::EventL(const CConnMonEventBase& aConnMonEvent)
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionMonitor::EventL()" )
    TUint connectionId = aConnMonEvent.ConnectionId();
    TInt eventType = aConnMonEvent.EventType();
    MUS_LOG1( "Connection ID = %d", connectionId )
    MUS_LOG1( "Event type = %d", eventType )

    for( TInt i=0; i<iConnectionMonitorObserver.Count(); i++ )
        {
        iConnectionMonitorObserver[i]->EventL( aConnMonEvent );
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionMonitor::EventL()" )
    }


// -------------------------------------------------------------------------
//  Constructor function.
// -------------------------------------------------------------------------
//
CMusAvaConnectionMonitor::CMusAvaConnectionMonitor( 
                                        CMusAvaSharedObject& aSharedObject ):
iSharedObj( &aSharedObject )
    {
    }

// -------------------------------------------------------------------------
//  Second phase constructor.
// -------------------------------------------------------------------------
//
void CMusAvaConnectionMonitor::ConstructL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionMonitor::ConstructL()" )
    User::LeaveIfError( iConnectionMonitor.ConnectL() );

    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionMonitor::ConstructL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaConnectionMonitor::AddAdapterL( 
                                    MMusAvaConnectionMonitorObserver& aAdapter )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionMonitor::\
             AddAdapterL( MMusAvaConnectionMonitorObserver& aAdapter )" )
    iConnectionMonitorObserver.AppendL( &aAdapter );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::\
             AddAdapterL( MMusAvaConnectionMonitorObserver& aAdapter )" )
    }

// -------------------------------------------------------------------------
// Created connection ID for multimedia sharing is returned
// -------------------------------------------------------------------------
//
TUint CMusAvaConnectionMonitor::CreatedConnectionID()
    {
    return iConnectionID;
    }

// -------------------------------------------------------------------------
// Connection ID is stored
// -------------------------------------------------------------------------
//
void CMusAvaConnectionMonitor::SetConnectionID( TUint aConnectionID )
    {
    iConnectionID = aConnectionID;
    }
// -------------------------------------------------------------------------
// Active connection ID for multimedia sharing is returned
// -------------------------------------------------------------------------
//
TUint CMusAvaConnectionMonitor::ConnectionIDL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionMonitor::ConnectionIDL()" )
    TUint iap =  0; //Internet acces point
    TUint sipIap =  0;  //SIP acces point
    TUint count = 0;    //Number of connections
    TUint id = 0;   //Connection ID
    TUint activeIapID = 0;   //Connection ID for active SIP IAP
    TUint subId = 0;    //SubconnectionID
    TUint subCount; //Subconnectioncount
    TRequestStatus status;

    //Fetch number of connections to count
    count = ConnectionCount();
    if ( count == 0 )
        {
        MUS_LOG( "mus: [MUSAVA] Non connections" )
        }
    else
        {
        MUS_LOG1( "mus: [MUSAVA] Fetch number of connections: %d", count )

        //Fetch internet access point to iap
        sipIap =  MultimediaSharingAccessPointID();
        if ( sipIap == 0 )
            {
            MUS_LOG( "mus: [MUSAVA] Non Access Point ID's for app" )
            }
        else
            {
            MUS_LOG1( "mus: [MUSAVA] Internet access point for sip: %d", 
                                                                sipIap )
    
            TBool goOn = ETrue;
            for ( TUint i = 1; i <= count && goOn; i++ )
                {
                //Fetch connectionId to id
                User::LeaveIfError(iConnectionMonitor.GetConnectionInfo( 
                                                            i, id, subCount ));
                MUS_LOG1( "mus: [MUSAVA] Connection ID = %d", id )
    
                //Fetch internet access point to iap
                iConnectionMonitor.GetUintAttribute( id, subId, KIAPId, 
                                                iap, status );
                #ifndef UNIT_TESTING
                User::WaitForRequest( status );
                #endif
                MUS_LOG1( "mus: [MUSAVA] Internet access point %d ", iap )
    
                // when correct Iap found ...
                if( iap == sipIap )
                    {
                    MUS_LOG( "mus: [MUSAVA]  Connection ID found" )
                    activeIapID = id;
                    goOn = EFalse;
                    }
                }
            
            }
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionMonitor::ConnectionIDL()" )
    return activeIapID;
    }
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
TBool CMusAvaConnectionMonitor::KillPdpContext()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionMonitor::KillPdpContext" )

    TUint connId = 0;
    TUint subConnectionCount = 0;
    TRequestStatus requestStatus;
    TUint count = ConnectionCount();
    TInt ret = 0;
    TBool retval = EFalse;
    
    for ( TUint i = 1; i <= count && !retval; i++ )
        {
        TInt bearerType = 0;
        iConnectionMonitor.GetIntAttribute( connId,
                                     subConnectionCount,
                                     KBearer,
                                     bearerType,
                                     requestStatus );
        User::WaitForRequest( requestStatus );
        if ( bearerType == EBearerCSD      ||
             bearerType == EBearerHSCSD    ||
             bearerType == EBearerGPRS     ||
             bearerType == EBearerEdgeGPRS ||
             bearerType == EBearerWCDMA    ||
             bearerType == EBearerWcdmaCSD )
            {
            MUS_LOG1( "mus: [MUSAVA] now killing connection whose connId \
                == <%d>", connId )
            ret = iConnectionMonitor.GetConnectionInfo( i, connId, 
                                                    subConnectionCount );
            if ( ret == KErrNone )
                {
                ret = iConnectionMonitor.SetBoolAttribute( connId, 0, 
                                                    KConnectionStop, ETrue );
                if ( ret == KErrNone )
                    {
                    retval = ETrue;
                    }
                }
            }
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionMonitor::KillPdpContext" )
    return retval;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaConnectionMonitor::RemoveAdapter( 
                                    MMusAvaConnectionMonitorObserver& aAdapter )
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionMonitor::\
             RemoveAdapter( MMusAvaConnectionMonitorObserver& aAdapter )" )
    TInt index = iConnectionMonitorObserver.Find( &aAdapter );

    if( index != KErrNotFound )
        {
        iConnectionMonitorObserver.Remove( index );
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaSip::\
             RemoveAdapter( MMusAvaConnectionMonitorObserver& aAdapter )" )
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void CMusAvaConnectionMonitor::NotifyEventL()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionMonitor::NotifyEventL()" )
    iConnectionMonitor.NotifyEventL( *this );
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionMonitor::NotifyEventL()" )
    }

// -------------------------------------------------------------------------
//  Active connection count for multimedia sharing is returned
// -------------------------------------------------------------------------
//
TUint CMusAvaConnectionMonitor::ConnectionCount()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionMonitor::ConnectionCount()" )
    TUint          count = (TUint) KErrNotFound;
    TRequestStatus status;

    iConnectionMonitor.GetConnectionCount( count, status );
    #ifndef UNIT_TESTING
    User::WaitForRequest( status );
    #endif
    if ( status.Int() != KErrNone )
        {
        MUS_LOG( "mus: [MUSAVA]  Phone doesn't have any active connections" )
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionMonitor::ConnectionCount()" )
    return count;
    }

// -------------------------------------------------------------------------
// Active Internet Access Point ID for multimedia sharing is returned
// -------------------------------------------------------------------------
//
TUint CMusAvaConnectionMonitor::MultimediaSharingAccessPointID()
    {
    MUS_LOG( "mus: [MUSAVA]  -> CMusAvaConnectionMonitor::\
        MultimediaSharingAccessPointID()" )
    TUint retval = KErrNone;
    TBool profileCreated = EFalse;
    TRAPD( err, profileCreated = iSharedObj->MusAvaSip().CreateProfileL() );
    if ( err || !profileCreated )
        {
        retval = (TUint)KErrNotFound;
        }
	else
		{
        CSIPProfile* profile = iSharedObj->MusAvaSip().Profile();
        // Get IAP ID
        MUS_LOG( "mus: [MUSAVA]  Get IAP ID" )
    
        TInt ret = profile->GetParameter( KSIPAccessPointId, iProfileId );
        if( ret )
            {
            MUS_LOG( "mus: [MUSAVA]  IAP ID is not found" )
            MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionMonitor::\
            MultimediaSharingAccessPointID()" )
            retval = (TUint)ret;
            }
		else
			{
	        MUS_LOG1( "mus: [MUSAVA]     IAP ID = %d", iProfileId )
			retval = iProfileId;
			}        
        }
    MUS_LOG( "mus: [MUSAVA]  <- CMusAvaConnectionMonitor::\
        MultimediaSharingAccessPointID()" )
    return retval;
    }
