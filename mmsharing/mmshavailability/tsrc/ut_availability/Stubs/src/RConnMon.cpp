/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/


// INCLUDE FILES
#include "rconnmon.h"
/*#include "ConnMonDef.h"
#include "ConnMonCli.h"
#include "ConnMonServ.h"*/
#include "CSipSseTestTls.h"
#include "musavatimer.h"

#include "musunittesting.h"

#include <e32std.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// RConnectionMonitor::ConnectL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RConnectionMonitor::ConnectL()
    {
    
    iMusAvaTimer = CMusAvaTimer::NewL();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::Close
// -----------------------------------------------------------------------------
//
EXPORT_C void RConnectionMonitor::Close()
    {
    delete iEmptyHbufC;
    iEmptyHbufC = NULL;
    delete iMusAvaTimer;
    iMusAvaTimer = NULL;
    iPtrArray.Close();
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::GetConnectionCount
// -----------------------------------------------------------------------------
//
EXPORT_C void RConnectionMonitor::GetConnectionCount( TUint& aConnectionCount, 
                                                      TRequestStatus& aStatus )
    {
    aConnectionCount = iConnectionCounter;
    aStatus = KErrNone;
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::GetConnectionInfo
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RConnectionMonitor::GetConnectionInfo( const TUint aIndex, 
                                                     TUint& aConnectionId, 
                                                     TUint& iConnectionCounterer ) const
    {    
    aConnectionId = iConnectionId;
    iConnectionCounterer = iSubConnectionCount;
    return iConnectionInfoError;       
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::GetSubConnectionInfo
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RConnectionMonitor::GetSubConnectionInfo( const TUint aConnectionId, 
                                                        const TUint aIndex, 
                                                        TUint& aSubConnectionId ) const
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::GetIntAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C void RConnectionMonitor::GetIntAttribute( const TUint aConnectionId, 
                                                   const TUint aSubConnectionId, 
                                                   const TUint aAttribute, 
                                                   TInt& aValue, 
                                                   TRequestStatus& aStatus )     
    {
    CSipSseTestTls* tls = CSipSseTestTls::Storage();
    if ( tls )
        {
        tls->GetIntAttribute(  aConnectionId, 
                                aSubConnectionId, 
                                aAttribute, 
                                aValue, 
                                aStatus );
        //iMusAvaTimer->StopActiveSchedulerAfter( 10, &aStatus);
        }
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::GetUintAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C void RConnectionMonitor::GetUintAttribute( const TUint aConnectionId, 
                                                    const TUint aSubConnectionId, 
                                                    const TUint aAttribute, 
                                                    TUint& aValue, 
                                                    TRequestStatus& aStatus )     
    {
    aValue = iUintAttributeValue;
    aStatus = iStatus;
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::GetBoolAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C void RConnectionMonitor::GetBoolAttribute( const TUint aConnectionId, 
                                                    const TUint aSubConnectionId, 
                                                    const TUint aAttribute, 
                                                    TBool& aValue, 
                                                    TRequestStatus& aStatus )     
    {
   ;    
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::GetStringAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C void RConnectionMonitor::GetStringAttribute( const TUint aConnectionId, 
                                                      const TUint aSubConnectionId, 
                                                      const TUint aAttribute, 
                                                      TDes& aValue, 
                                                      TRequestStatus& aStatus ) const
    {
        
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::GetPckgAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C void RConnectionMonitor::GetPckgAttribute( const TUint aConnectionId, 
                                                    const TUint aSubConnectionId, 
                                                    const TUint aAttribute, 
                                                    TDes8& aValue, 
                                                    TRequestStatus& aStatus ) const
    {
      
    }
    
// -----------------------------------------------------------------------------
// RConnectionMonitor::GetPckgAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C void RConnectionMonitor::GetPckgAttribute( const TUint aConnectionId, 
                                                    const TUint aSubConnectionId, 
                                                    const TUint aAttribute, 
                                                    TDes16& aValue, 
                                                    TRequestStatus& aStatus ) const
    {
    
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::SetIntAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RConnectionMonitor::SetIntAttribute( const TUint aConnectionId, 
                                                   const TUint aSubConnectionId, 
                                                   const TUint aAttribute, 
                                                   const TInt aValue ) const    
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::SetUintAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RConnectionMonitor::SetUintAttribute( const TUint aConnectionId, 
                                                    const TUint aSubConnectionId, 
                                                    const TUint aAttribute, 
                                                    const TUint aValue ) const    
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::SetBoolAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RConnectionMonitor::SetBoolAttribute( const TUint aConnectionId, 
                                                    const TUint aSubConnectionId, 
                                                    const TUint aAttribute, 
                                                    const TBool aValue ) const    
    {
    return KErrNone;   
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::SetStringAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RConnectionMonitor::SetStringAttribute( const TUint aConnectionId, 
                                                      const TUint aSubConnectionId, 
                                                      const TUint aAttribute, 
                                                      const TDes& aValue ) const
    {
    return KErrNone;   
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::SetPckgAttribute
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RConnectionMonitor::SetPckgAttribute( const TUint aConnectionId, 
                                                    const TUint aSubConnectionId, 
                                                    const TUint aAttribute, 
                                                    const TDes8& aValue ) const
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::CancelAsyncRequest
// -----------------------------------------------------------------------------
//
EXPORT_C void RConnectionMonitor::CancelAsyncRequest( TInt aReqToCancel )
    {
      
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::NotifyEventL
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RConnectionMonitor::NotifyEventL( MConnectionMonitorObserver& aObserver )
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::CancelNotifications
// -----------------------------------------------------------------------------
//
EXPORT_C void RConnectionMonitor::CancelNotifications()
    {
     
    }


// private methods in conjunction with CConnMonEventHandler

// -----------------------------------------------------------------------------
// RConnectionMonitor::ReceiveEvent
// -----------------------------------------------------------------------------
//
void RConnectionMonitor::ReceiveEvent( TDes8&          aBuffer, 
                                       TDes8&          aExtraBuf,
                                       TRequestStatus& aStatus )
    {
    
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::CancelReceiveEvent
// -----------------------------------------------------------------------------
//
void RConnectionMonitor::CancelReceiveEvent()
    {
    
    }

// private internal methods

// -----------------------------------------------------------------------------
// RConnectionMonitor::IsConnected
// -----------------------------------------------------------------------------
//
TBool RConnectionMonitor::IsConnected() const
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// RConnectionMonitor::NextPtr
// -----------------------------------------------------------------------------
//
TPtr8& RConnectionMonitor::NextPtr()
    {
    iEmptyHbufC = HBufC8::NewL( 0 );
    TPtr8 ptr = iEmptyHbufC->Des();
    iPtrArray.Append( ptr );
    return iPtrArray[0];
    
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonEventBase::CConnMonEventBase
// -----------------------------------------------------------------------------
//
CConnMonEventBase::CConnMonEventBase( const TInt aEventType, const TUint aConnectionId )
    {
    iEventType    = aEventType ;
    iConnectionId = aConnectionId ;
    }

// Destructor
CConnMonEventBase::~CConnMonEventBase()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonEventBase::EventType
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CConnMonEventBase::EventType() const
    {
    return iEventType;  
    }

// -----------------------------------------------------------------------------
// CConnMonEventBase::ConnectionId
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonEventBase::ConnectionId() const
    {
    return iConnectionId;  
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonCreateConnection::CConnMonCreateConnection
// -----------------------------------------------------------------------------
//
CConnMonCreateConnection::CConnMonCreateConnection( const TUint aConnectionId )
    :CConnMonEventBase( EConnMonCreateConnection, aConnectionId ) 
    {
    }

// Destructor
CConnMonCreateConnection::~CConnMonCreateConnection()
    {
    
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonDeleteConnection::CConnMonDeleteConnection
// -----------------------------------------------------------------------------
//
CConnMonDeleteConnection::CConnMonDeleteConnection( const TUint aConnectionId, 
                                                    const TUint aDownlinkData, 
                                                    const TUint aUplinkData,
                                                    const TBool aAuthDelete )
    :CConnMonEventBase( EConnMonDeleteConnection, aConnectionId ) 
    {
    iDownlinkData = aDownlinkData ;
    iUplinkData   = aUplinkData ;
    iAuthDelete   = aAuthDelete ;
    }

// Destructor
CConnMonDeleteConnection::~CConnMonDeleteConnection()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonDeleteConnection::DownlinkData
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonDeleteConnection::DownlinkData() const
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// CConnMonDeleteConnection::UplinkData
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonDeleteConnection::UplinkData() const
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// CConnMonDeleteConnection::AuthoritativeDelete
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CConnMonDeleteConnection::AuthoritativeDelete() const
    {
    return KErrNone;  
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonCreateSubConnection::CConnMonCreateSubConnection
// -----------------------------------------------------------------------------
//
CConnMonCreateSubConnection::CConnMonCreateSubConnection( const TUint aConnectionId, 
                                                          const TUint aSubConnectionId )
    :CConnMonEventBase( EConnMonCreateSubConnection, aConnectionId ) 
    {
     
    }

// Destructor
CConnMonCreateSubConnection::~CConnMonCreateSubConnection()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonCreateSubConnection::SubConnectionId
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonCreateSubConnection::SubConnectionId() const
    {
    return KErrNone;  
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonDeleteSubConnection::CConnMonDeleteSubConnection
// -----------------------------------------------------------------------------
//
CConnMonDeleteSubConnection::CConnMonDeleteSubConnection( const TUint aConnectionId, 
                                                          const TUint aSubConnectionId,
                                                          const TUint aDownlinkData, 
                                                          const TUint aUplinkData,
                                                          const TBool aAuthDelete )
    :CConnMonEventBase( EConnMonDeleteSubConnection, aConnectionId ) 
    {
    iSubConnectionId = aSubConnectionId ;
    iDownlinkData    = aDownlinkData ;
    iUplinkData      = aUplinkData ;
    iAuthDelete      = aAuthDelete ;
    }

// Destructor
CConnMonDeleteSubConnection::~CConnMonDeleteSubConnection()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonDeleteSubConnection::SubConnectionId
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonDeleteSubConnection::SubConnectionId() const
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// CConnMonDeleteSubConnection::DownlinkData
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonDeleteSubConnection::DownlinkData() const
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// CConnMonDeleteSubConnection::UplinkData
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonDeleteSubConnection::UplinkData() const
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// CConnMonDeleteSubConnection::AuthoritativeDelete
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CConnMonDeleteSubConnection::AuthoritativeDelete() const
    {
    return ETrue;  
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonDownlinkDataThreshold::CConnMonDownlinkDataThreshold
// -----------------------------------------------------------------------------
//
CConnMonDownlinkDataThreshold::CConnMonDownlinkDataThreshold( const TUint aConnectionId, 
                                                              const TUint aSubConnectionId, 
                                                              const TUint aDownlinkData )
    :CConnMonEventBase( EConnMonDownlinkDataThreshold, aConnectionId ) 
    {
    iSubConnectionId = aSubConnectionId ;
    iDownlinkData    = aDownlinkData ;
    }

// Destructor
CConnMonDownlinkDataThreshold::~CConnMonDownlinkDataThreshold()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonDownlinkDataThreshold::SubConnectionId
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonDownlinkDataThreshold::SubConnectionId() const
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// CConnMonDownlinkDataThreshold::DownlinkData
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonDownlinkDataThreshold::DownlinkData() const
    {
    return KErrNone;  
    }
    
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonUplinkDataThreshold::CConnMonUplinkDataThreshold
// -----------------------------------------------------------------------------
//
CConnMonUplinkDataThreshold::CConnMonUplinkDataThreshold( const TUint aConnectionId, 
                                                          const TUint aSubConnectionId, 
                                                          const TUint aUplinkData )
    :CConnMonEventBase( EConnMonUplinkDataThreshold, aConnectionId ) 
    {
    iSubConnectionId = aSubConnectionId ;
    iUplinkData      = aUplinkData ;
    }

// Destructor
CConnMonUplinkDataThreshold::~CConnMonUplinkDataThreshold()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonUplinkDataThreshold::SubConnectionId
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonUplinkDataThreshold::SubConnectionId() const
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// CConnMonUplinkDataThreshold::UplinkData
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonUplinkDataThreshold::UplinkData() const
    {
    return KErrNone;  
    }
    
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonNetworkStatusChange::CConnMonNetworkStatusChange
// -----------------------------------------------------------------------------
//
CConnMonNetworkStatusChange::CConnMonNetworkStatusChange( const TUint aConnectionId, 
                                                          const TInt  aNetworkStatus )
    :CConnMonEventBase( EConnMonNetworkStatusChange, aConnectionId ) 
    {
    iNetworkStatus = aNetworkStatus;
    }

// Destructor
CConnMonNetworkStatusChange::~CConnMonNetworkStatusChange()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonNetworkStatusChange::NetworkStatus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CConnMonNetworkStatusChange::NetworkStatus() const
    {
    return iNetworkStatus;  
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonConnectionStatusChange::CConnMonConnectionStatusChange
// -----------------------------------------------------------------------------
//
CConnMonConnectionStatusChange::CConnMonConnectionStatusChange( const TUint aConnectionId, 
                                                                const TUint aSubConnectionId, 
                                                                const TInt  aConnectionStatus )
    :CConnMonEventBase( EConnMonConnectionStatusChange, aConnectionId ) 
    {
    iSubConnectionId  = aSubConnectionId;
    iConnectionStatus = aConnectionStatus;
    }

// Destructor
CConnMonConnectionStatusChange::~CConnMonConnectionStatusChange()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonConnectionStatusChange::SubConnectionId
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonConnectionStatusChange::SubConnectionId() const
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// CConnMonConnectionStatusChange::ConnectionStatus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CConnMonConnectionStatusChange::ConnectionStatus() const
    {
    return iConnectionStatus;  
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonConnectionActivityChange::CConnMonConnectionActivityChange
// -----------------------------------------------------------------------------
//
CConnMonConnectionActivityChange::CConnMonConnectionActivityChange( const TUint aConnectionId, 
                                                                    const TUint aSubConnectionId, 
                                                                    const TBool aActivity )
    :CConnMonEventBase( EConnMonConnectionActivityChange, aConnectionId ) 
    {
    iSubConnectionId = aSubConnectionId;
    iActivity        = aActivity;
    }

// Destructor
CConnMonConnectionActivityChange::~CConnMonConnectionActivityChange()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonConnectionActivityChange::SubConnectionId
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonConnectionActivityChange::SubConnectionId() const
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// CConnMonConnectionActivityChange::ConnectionActivity
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CConnMonConnectionActivityChange::ConnectionActivity() const
    {
    return ETrue ;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonNetworkRegistrationChange::CConnMonNetworkRegistrationChange
// -----------------------------------------------------------------------------
//
CConnMonNetworkRegistrationChange::CConnMonNetworkRegistrationChange
                                              ( const TUint aConnectionId, 
                                                const TInt  aRegistrationStatus )
    :CConnMonEventBase( EConnMonNetworkRegistrationChange, aConnectionId ) 
    {
    iRegistrationStatus = aRegistrationStatus;
    }

// Destructor
CConnMonNetworkRegistrationChange::~CConnMonNetworkRegistrationChange()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonNetworkRegistrationChange::RegistrationStatus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CConnMonNetworkRegistrationChange::RegistrationStatus() const
    {
    return iRegistrationStatus;  
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonBearerChange::CConnMonBearerChange
// -----------------------------------------------------------------------------
//
CConnMonBearerChange::CConnMonBearerChange( const TUint aConnectionId, const TInt aBearer )
    :CConnMonEventBase( EConnMonBearerChange, aConnectionId ) 
    {
    iBearer = aBearer;
    }

// Destructor
CConnMonBearerChange::~CConnMonBearerChange()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonBearerChange::RegistrationStatus
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CConnMonBearerChange::Bearer() const
    {
    return iBearer;  
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonSignalStrengthChange::CConnMonSignalStrengthChange
// -----------------------------------------------------------------------------
//
CConnMonSignalStrengthChange::CConnMonSignalStrengthChange( const TUint aConnectionId, 
                                                            const TInt  aSignalStrength )
    :CConnMonEventBase( EConnMonSignalStrengthChange, aConnectionId ) 
    {
    iSignalStrength = aSignalStrength;
    }

// Destructor
CConnMonSignalStrengthChange::~CConnMonSignalStrengthChange()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonSignalStrengthChange::SignalStrength
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CConnMonSignalStrengthChange::SignalStrength() const
    {
    return KErrNone;  
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonBearerAvailabilityChange::CConnMonBearerAvailabilityChange
// -----------------------------------------------------------------------------
//
CConnMonBearerAvailabilityChange::CConnMonBearerAvailabilityChange( const TUint aConnectionId, 
                                                                    const TBool aAvailability )
    :CConnMonEventBase( EConnMonBearerAvailabilityChange, aConnectionId ) 
    {
    iAvailability    = aAvailability;
    }

// Destructor
CConnMonBearerAvailabilityChange::~CConnMonBearerAvailabilityChange()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonBearerAvailabilityChange::Availability
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CConnMonBearerAvailabilityChange::Availability() const
    {
    return iAvailability;  
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonGenericEvent::CConnMonGenericEvent
// -----------------------------------------------------------------------------
//
CConnMonGenericEvent::CConnMonGenericEvent( const TUint aType,
                                            const TUint aConnectionId, 
                                            TAny* aData )
    :CConnMonEventBase( aType, aConnectionId )
    {
    iData = aData;
    }

// Destructor
CConnMonGenericEvent::~CConnMonGenericEvent()
    {
    iData = 0;
    }

// -----------------------------------------------------------------------------
// CConnMonGenericEvent::Data
// -----------------------------------------------------------------------------
//
EXPORT_C TAny* CConnMonGenericEvent::Data() const
    {
    return iData;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonIapAvailabilityChange::CConnMonIapAvailabilityChange
// -----------------------------------------------------------------------------
//
CConnMonIapAvailabilityChange::CConnMonIapAvailabilityChange( const TUint aConnectionId, 
                                                              const TConnMonIapInfo* aIapInfoPtr )
    :CConnMonEventBase( EConnMonIapAvailabilityChange, aConnectionId )
    {
    iIapInfo.iCount = 0;

    if ( aIapInfoPtr != NULL )
        { 
        iIapInfo = *aIapInfoPtr;
        }
    }

// Destructor
CConnMonIapAvailabilityChange::~CConnMonIapAvailabilityChange()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonIapAvailabilityChange::IapAvailability
// -----------------------------------------------------------------------------
//
EXPORT_C TConnMonIapInfo CConnMonIapAvailabilityChange::IapAvailability() const
    {
    return iIapInfo;
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CConnMonTransmitPowerChange::CConnMonTransmitPowerChange
// -----------------------------------------------------------------------------
//
CConnMonTransmitPowerChange::CConnMonTransmitPowerChange( const TUint aConnectionId, 
                                                          const TUint aTransmitPower )
    :CConnMonEventBase( EConnMonTransmitPowerChange, aConnectionId ) 
    {
    iTransmitPower = aTransmitPower;
    }

// Destructor
CConnMonTransmitPowerChange::~CConnMonTransmitPowerChange()
    {
    }

// -----------------------------------------------------------------------------
// CConnMonTransmitPowerChange::TransmitPower
// -----------------------------------------------------------------------------
//
EXPORT_C TUint CConnMonTransmitPowerChange::TransmitPower() const
    {
    return iTransmitPower;
    }

// End-of-file
