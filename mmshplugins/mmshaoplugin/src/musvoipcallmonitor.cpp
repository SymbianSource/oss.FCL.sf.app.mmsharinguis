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
* Description:  Monitors the VOIP calls.
*
*/


#include <e32property.h>
#include <telephonydomainpskeys.h>

// Call Information API
#include <ccallinformation.h>

// SP Settings
#include <spdefinitions.h>
#include <spentry.h>
#include <spproperty.h>
#include <spsettings.h>

#include "musvoipcallmonitor.h"
#include "mussesseioninformationapi.h"
#include "muslogger.h"

const TUint KMusAoRemoteInfoMonitoringTimeout = 2000000; // 2 seconds
_LIT( KSipAt, "@" );

// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusVoipCallMonitor* CMusVoipCallMonitor::NewL( TName& aCallName, 
                                        MMusCallStateObserver& aCallStateObserver )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::NewL" )
    CMusVoipCallMonitor* self = 
                new (ELeave) CMusVoipCallMonitor( aCallName, aCallStateObserver );
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusVoipCallMonitor::~CMusVoipCallMonitor()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::~CMusVoipCallMonitor" )    
    
    ResetCallProviderName();
    
    StopRemoteInfoMonitoring();
    
    iPropertyEvent.Close();
    
    delete iGuardTimer;
    
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::~CMusVoipCallMonitor" )
    }


// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusVoipCallMonitor::CMusVoipCallMonitor( TName& aCallName, 
                                        MMusCallStateObserver& aCallStateObserver ):
    CActive( CActive::EPriorityStandard ),
    iCallName ( aCallName ),
    iCallStateObserver ( aCallStateObserver ),
    iGuardTimerCallBack( GuardTimerExpired, this ) 
    {    
    CActiveScheduler::Add( this );
    
    iGuardTimerEntry.Set( iGuardTimerCallBack );
    }


// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusVoipCallMonitor::ConstructL( )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::ConstructL" ) 
    
    User::LeaveIfError( iPropertyEvent.Attach(
        KPSUidTelRemotePartyInformation,
        KTelCLINumber ) );
    
    iGuardTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
    
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::ConstructL" )
    }

// -----------------------------------------------------------------------------
// CMusVoipCallMonitor::SetStateL( )
// Sets the call state property key.See mussesseioninformationapi.h
// Also sets the call information if call state is Connected.
// If call is connected but remote info cannot be found, information
// publishing is delayed until remote info comes available or monitoring
// for the info timeouts. In voip case, remote info should be basically
// always available, but in some cases it is not immediately available
// when call status changes to connected.
// -----------------------------------------------------------------------------
//
void CMusVoipCallMonitor::SetStateL(NMusSessionInformationApi::TMusCallEvent aVal)
    {    
    MUS_LOG1( "mus: [MUSAO]  -> CMusVoipCallMonitor::SetStateL, aVal %d", aVal )
    // set the call state only if the call count is one else it should be decided 
    // by conference monitor or line/tsy monitor.

    TBool delayStateChange( EFalse );
    if ( aVal == NMusSessionInformationApi::ECallConnected )
        {
        if ( !RemoteInfoExistsL() )
            {
            MonitorForRemoteInfoL();
            delayStateChange = ETrue;
            }
        }
    else
        {
        StopRemoteInfoMonitoring();
        }
    
    if ( delayStateChange )
        {
        MUS_LOG( "mus: [MUSAO]      state change delayed!" )   
        }
    else
        {
        ReportStateChangeL( aVal );
        }
    
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::SetStateL" )    
    }

///-----------------------------------------------------------------------------
// CMusVoipCallMonitor::SetCallInfoL( )
// -----------------------------------------------------------------------------
//
void CMusVoipCallMonitor::SetCallInfoL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::SetCallInfoL")     
    
    // Set Call Provider Information
    TUint32 serviceId = CallServiceIdL();
    SetCallProviderInfoL( serviceId );

    HBufC* remoteUri = HBufC::NewLC( RProperty::KMaxPropertySize );
    TPtr rUri = remoteUri->Des();
    TInt err = RProperty::Get( KPSUidTelRemotePartyInformation,
                               KTelCLINumber,
                               rUri );
    
    if ( err == KErrNone && remoteUri->Des().Length() > 0 )
        {
        MUS_LOG_TDESC( "mus: [MUSAO] voip remote name: ", remoteUri->Des());
        
        // Add "sip:" prefix to remoteUri unless it already exists
        _LIT( KSipPrefix, "sip:" );
        TPtr prefix = remoteUri->Des().LeftTPtr( KSipPrefix().Length() );
        if ( prefix.Compare( KSipPrefix ) != 0 )
            {
            if ( remoteUri->Des().Length() + KSipPrefix().Length() >
                 remoteUri->Des().MaxLength() )
                {
                User::Leave( KErrOverflow );
                }
            remoteUri->Des().Insert( 0, KSipPrefix );
            }
        //if domain not present, add domain from voip profile (username)
        if ( remoteUri->Find( KSipAt ) == KErrNotFound )
            {
            rUri.Set( remoteUri->Des() );
            AddDomainFromOwnUsernameL( serviceId, rUri );
            MUS_LOG_TDESC ( "mus: [MUSAO] full voip remote name:", (*remoteUri) )
            }
        }
    else
        {
        remoteUri->Des().Zero();
        }

    User::LeaveIfError( RProperty::Set(
            NMusSessionInformationApi::KCategoryUid,
            NMusSessionInformationApi::KMusTelNumber, remoteUri->Des() ) );
    
    CleanupStack::PopAndDestroy(remoteUri);
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::SetCallInfoL" )  
    }

///-----------------------------------------------------------------------------
// CMusVoipCallMonitor::AddDomainFromOwnUsernameL( )
// -----------------------------------------------------------------------------
//
void CMusVoipCallMonitor::AddDomainFromOwnUsernameL( 
    TUint32 /*aServiceId*/, 
    TDes16& /*aUri*/ )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::AddDomainFromOwnUsernameL")
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::AddDomainFromOwnUsernameL, NOT SUPPORTED")
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::AddDomainFromOwnUsernameL")
    }

///-----------------------------------------------------------------------------
// CMusVoipCallMonitor::CallServiceIdL( )
// -----------------------------------------------------------------------------
//
TUint32 CMusVoipCallMonitor::CallServiceIdL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::CallServiceIdL")
    TUint32 serviceId( 0 );
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::CallServiceIdL, NOT SUPPORTED")
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::CallServiceIdL")
    return serviceId;
    }

///-----------------------------------------------------------------------------
// CMusVoipCallMonitor::SetCallProviderInfoL( )
// -----------------------------------------------------------------------------
//
void CMusVoipCallMonitor::SetCallProviderInfoL( TUint32 aServiceId )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::SetCallProviderInfoL")
    CSPSettings* settings = CSPSettings::NewLC();    
    CSPEntry* entry = CSPEntry::NewLC();
    
    TInt errorCode = settings->FindEntryL( aServiceId, *entry );
    
    if ( errorCode == KErrNone && entry != NULL ) 
        {
        const TDesC& name = entry->GetServiceName();    
    
        MUS_LOG_TDESC ( "mus: [MUSAO]]  CallProviderName IS -->", name )
        User::LeaveIfError(RProperty::Set( 
                                        NMusSessionInformationApi::KCategoryUid,
                                        NMusSessionInformationApi::KMUSCallProvider,
                                        name));
        }
    
    CleanupStack::PopAndDestroy( entry );
    CleanupStack::PopAndDestroy( settings );
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::SetCallProviderInfoL" )  
    }


// -----------------------------------------------------------------------------
// Checks the aName is equal to this monitors name.
// @return ETrue if matches else EFalse
// -----------------------------------------------------------------------------
//
TBool CMusVoipCallMonitor::IsEqual(TName& aName)
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::IsEqual" )
    MUS_LOG_TDESC( "mus: [MUSAO]  -> aName : ", aName );
    MUS_LOG_TDESC( "mus: [MUSAO]  -> iCallName : ", iCallName );
    TBool val = EFalse ;    
    val = ( aName == iCallName );    
    MUS_LOG1( "mus: [MUSAO]  <- CMusCallMonitor::IsEqual = %d",val )
    return val;
    }

// -----------------------------------------------------------------------------
// CMusVoipCallMonitor::RunL
// -----------------------------------------------------------------------------
//
void CMusVoipCallMonitor::RunL()
    {
    MUS_LOG( "mus: [MUSAO] -> CMusVoipCallMonitor::RunL" )
    
    if ( RemoteInfoExistsL() )
        {
        // Connected state reporting was delayed because not having
        // remote info, now it can be reported and monitoring is not needed anymore
        StopRemoteInfoMonitoring();
        ReportStateChangeL( NMusSessionInformationApi::ECallConnected  );
        }
    else
        {
        // resubscribe
        iPropertyEvent.Subscribe( iStatus );    
        SetActive();
        }

    MUS_LOG( "mus: [MUSAO] <- CMusVoipCallMonitor::RunL" )
    }


// -----------------------------------------------------------------------------
// CMusVoipCallMonitor::DoCancel
// -----------------------------------------------------------------------------
//
void CMusVoipCallMonitor::DoCancel()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::DoCancel" )
    
    iPropertyEvent.Cancel();
    
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::DoCancel" )
    }

// -----------------------------------------------------------------------------
// CMusVoipCallMonitor::RunError
// -----------------------------------------------------------------------------
//
TInt CMusVoipCallMonitor::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusVoipCallMonitor::RunError = %d", aError )    
    if ( aError != KErrNoMemory )
        {
        aError = KErrNone;
        }
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::RunError" )
    return aError;    
    }

// -----------------------------------------------------------------------------
// CMusVoipCallMonitor::RemoteInfoExistsL
// -----------------------------------------------------------------------------
//
TBool CMusVoipCallMonitor::RemoteInfoExistsL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::RemoteInfoExistsL" )

    HBufC* remoteUri = HBufC::NewLC(RProperty::KMaxPropertySize);
    TPtr rUri = remoteUri->Des();
    
    TInt err = RProperty::Get( KPSUidTelRemotePartyInformation,
                                 KTelCLINumber,
                                 rUri );

    TBool remoteInfoExists( err == KErrNone && remoteUri->Des().Length() > 0 );
    CleanupStack::PopAndDestroy(remoteUri);
    
    MUS_LOG1( "mus: [MUSAO]  <- CMusVoipCallMonitor::RemoteInfoExistsL = %d", 
              remoteInfoExists )    

    return remoteInfoExists;
    }

// -----------------------------------------------------------------------------
// CMusVoipCallMonitor::MonitorForRemoteInfoL
// Start monitoring remote info P&S key. Guard timer is started
// to prevent waiting for the infomation forever.
// -----------------------------------------------------------------------------
//
void CMusVoipCallMonitor::MonitorForRemoteInfoL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::MonitorForRemoteInfoL" )
    
    if ( !IsActive() )
        {
        MUS_LOG( "mus: [MUSAO]      activate" )
        
        iGuardTimer->Remove( iGuardTimerEntry );
        TTimeIntervalMicroSeconds32 interval( KMusAoRemoteInfoMonitoringTimeout );
        iGuardTimer->Queue( interval, iGuardTimerEntry );
        
        iPropertyEvent.Subscribe( iStatus );    
        SetActive();
        }
    
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::MonitorForRemoteInfoL" )
    }

// -----------------------------------------------------------------------------
// CMusVoipCallMonitor::MonitorForRemoteInfoL
// -----------------------------------------------------------------------------
//
void CMusVoipCallMonitor::StopRemoteInfoMonitoring()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::StopRemoteInfoMonitoring" )
    
    if ( iGuardTimer )
        {
        iGuardTimer->Remove( iGuardTimerEntry );
        }
    
    Cancel();
    
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::StopRemoteInfoMonitoring" )
    }

// -----------------------------------------------------------------------------
// CMusVoipCallMonitor::ReportStateChangeL
// -----------------------------------------------------------------------------
//
void CMusVoipCallMonitor::ReportStateChangeL( 
    NMusSessionInformationApi::TMusCallEvent aVal )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::ReportStateChangeL" )
    
    TInt currentVal = 0;
    User::LeaveIfError(RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                                       NMusSessionInformationApi::KMusCallEvent,
                                       currentVal));
    
    
    // Report only if the value changed else do not publish.
    if(currentVal != (TInt)aVal)
        {
        User::LeaveIfError(RProperty::Set( 
                                   NMusSessionInformationApi::KCategoryUid,
                                   NMusSessionInformationApi::KMusCallEvent,
                                   aVal ));    
        }        
    // When call is connected and the callcount is one, 
    // set the call informations.
    if( aVal==NMusSessionInformationApi::ECallConnected ) 
        {
        SetCallInfoL();
        }  
    
    iCallStateObserver.MusCallStateChanged();
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::ReportStateChangeL" )
    }

// -----------------------------------------------------------------------------
// CMusVoipCallMonitor::GuardTimerExpired
// If timer expired, it took too long to get remote party information via
// P&S monitoring. Because of monitoring, call connected was not yet informed,
// inform it now.
// -----------------------------------------------------------------------------
//
TInt CMusVoipCallMonitor::GuardTimerExpired( TAny* aPtr )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::GuardTimerExpired" )
    if ( aPtr )
        {
        CMusVoipCallMonitor* self = reinterpret_cast<CMusVoipCallMonitor*>( aPtr );
        self->StopRemoteInfoMonitoring();
        TRAP_IGNORE( 
                self->ReportStateChangeL( NMusSessionInformationApi::ECallConnected ) )
        }
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::GuardTimerExpired" )
    
    return KErrNone;
    }

// --------------------------------------------------------------------------------
// CMusCallConferenceMonitor::IsDataReadyL()
// Checks if Data is ready VoIP Call.
// --------------------------------------------------------------------------------

TBool CMusVoipCallMonitor::IsDataReadyL()
    {
    //This function need to updated if some other P/S Key is published
    //to ensure correct behaviour.

    MUS_LOG( "mus: [MUSAO]  -> CMusVoipCallMonitor::IsDataReadyL" )

    // Ensure Provider Information is Availble.
    HBufC* providerInfo = HBufC::NewLC( RProperty::KMaxPropertySize );
    TPtr pInfo = providerInfo->Des();
    TInt err  = KErrNone;
    
    err = RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                                NMusSessionInformationApi::KMUSCallProvider,
                                 pInfo );

    TBool providerInfoExisit ( err == KErrNone && providerInfo->Des().Length() > 0 );
    CleanupStack::PopAndDestroy( providerInfo );

    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::IsDataReadyL" )
    return !IsActive() && providerInfoExisit;
    }


// --------------------------------------------------------------------------------
// Reset the Call Provider Information. 
// --------------------------------------------------------------------------------

void CMusVoipCallMonitor::ResetCallProviderName( )
    {
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::ResetCallProviderName" )
    TInt errorCode = RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                 NMusSessionInformationApi::KMUSCallProvider,
                                 KNullDesC );

    // Error Code is just for debug only 
    MUS_LOG1( "mus: [MUSAO]  ErrorCode = %d", errorCode )
    MUS_LOG( "mus: [MUSAO]  <- CMusVoipCallMonitor::ResetCallProviderName" )
    }


// End of file
