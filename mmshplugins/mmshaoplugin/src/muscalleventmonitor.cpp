/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Monitor the events of CS call
*
*/


#include "mussesseioninformationapi.h"
#include "muscalleventmonitor.h"
#include "muslogger.h"

#include <etelmm.h>
#include <e32svr.h>
#include <mmtsy_names.h>
#include <e32property.h>


// ------------------------------------------------------------------------------
// Symbian two-phase constructor.
// ------------------------------------------------------------------------------
//
CMusCallEventMonitor* CMusCallEventMonitor::NewL( const RMobileCall& aCall, 
                                                    MMusTsyPropertyObserver& aObserver,
                                                    MMusCallStateObserver& aCallStateObserver )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallEventMonitor::NewL" )
    CMusCallEventMonitor* self = new (ELeave) CMusCallEventMonitor(aCall, aObserver, aCallStateObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAO]  <- CMusCallEventMonitor::NewL" )
    return self;
    }


// ------------------------------------------------------------------------------
// C++ destructor.
// ------------------------------------------------------------------------------
//
CMusCallEventMonitor::~CMusCallEventMonitor()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallEventMonitor::~CMusCallEventMonitor" )        
    Cancel();        
    MUS_LOG( "mus: [MUSAO]  <- CMusCallEventMonitor::~CMusCallEventMonitor" )
    }

// ------------------------------------------------------------------------------
// Symbian second-phase constructor.
// ------------------------------------------------------------------------------
//
void CMusCallEventMonitor::ConstructL( )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallEventMonitor::ConstructL" )   
    CActiveScheduler::Add( this );         
    iCall.NotifyCallEvent( iStatus, iCallEvent );
    SetActive();        
    MUS_LOG( "mus: [MUSAO]  <- CMusCallEventMonitor::ConstructL" )
    }


// ------------------------------------------------------------------------------
// C++ constructor.
// ------------------------------------------------------------------------------
//
CMusCallEventMonitor::CMusCallEventMonitor( const RMobileCall& aCall, 
                                            MMusTsyPropertyObserver& aObserver,
                                            MMusCallStateObserver& aCallStateObserver ) 
        : CMusCallMonitorBase(aCall, aObserver, aCallStateObserver)     
        
    { 
    
    }


// -----------------------------------------------------------------------------
// CMusCallEventMonitor::RunL()
// From CActive.It will be called any NotifyMobileLineStatusChange
// Event happens.
// -----------------------------------------------------------------------------
//
void CMusCallEventMonitor::RunL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallEventMonitor::RunL" )    
    iCall.NotifyCallEvent( iStatus, iCallEvent );
    SetActive();
    MUS_LOG1( "mus: [MUSAO]  iCallEvent =  %d", iCallEvent )    
    switch(iCallEvent)    
        {              	
            case RMobileCall::ELocalHold:
                 iLocalCallEvent = RMobileCall::ELocalHold;
                 SetStateL( NMusSessionInformationApi::ECallHold );
                 break;
		    case RMobileCall::ELocalResume:	
                 iLocalCallEvent = RMobileCall::ELocalResume;
                 // when local resume happens , check the remote call state. 
                 // without knowing remote call state we can not set the state
                 // to call connected.        
                 if( iRemoteCallEvent == RMobileCall::ERemoteResume) 
                    {
                    SetStateL( NMusSessionInformationApi::ECallConnected );
                    }
                 else
                    {
                    SetStateL( NMusSessionInformationApi::ECallHold );
                    }                 
                 break;
		    case RMobileCall::ELocalDeflectCall:	
                 //SetStateL( NMusSessionInformationApi::ECallHold );
                 break;
		    case RMobileCall::ELocalTransfer:
                 SetStateL( NMusSessionInformationApi::ENoCall );
                 break;	
		    case RMobileCall::ERemoteHold:	
                 iRemoteCallEvent = RMobileCall::ERemoteHold;	
                 SetStateL( NMusSessionInformationApi::ECallHold );
                 break;
		    case RMobileCall::ERemoteResume:	
                 iRemoteCallEvent = RMobileCall::ERemoteResume;
                 // when remote resume happens , check the local call state.         
                 // without knowing local call state we can not set the state
                 // to call connected. 
                 if( iLocalCallEvent == RMobileCall::ELocalResume) 
                    {
                    SetStateL( NMusSessionInformationApi::ECallConnected );   
                    }
                 else
                    {
                    SetStateL( NMusSessionInformationApi::ECallHold );
                    }       	                 
                 break;
		    case RMobileCall::ERemoteTerminated:	
                 SetStateL( NMusSessionInformationApi::ENoCall );
                 NotifyCallStateChanged( NMusSessionInformationApi::ENoCall );
                 break;
		    case RMobileCall::ERemoteConferenceCreate:
                 SetStateL( NMusSessionInformationApi::EConferenceCall );
                 break;
		    case RMobileCall::ERemoteTransferring:
                 //SetStateL( NMusSessionInformationApi::ECallHold );
                 break;
		    case RMobileCall::ERemoteTransferAlerting:	
                 //SetStateL( NMusSessionInformationApi::ECallHold );
                 break;
		    case RMobileCall::ERemoteBarred:	
                 //SetStateL( NMusSessionInformationApi::ECallHold );
                 break;
		    case RMobileCall::ERemoteForwarding:	
                 //SetStateL( NMusSessionInformationApi::ECallHold );
                 break;
		    case RMobileCall::ERemoteWaiting:	
                 //SetStateL( NMusSessionInformationApi::ECallHold );
                 break;
		    case RMobileCall::ELocalBarred:
                 //SetStateL( NMusSessionInformationApi::ECallHold );
                 break;
            default:
                 break;
        }    
    MUS_LOG( "mus: [MUSAO]  <- CMusCallEventMonitor::RunL" )
    }


// -----------------------------------------------------------------------------
// CMusCallEventMonitor::DoCancel()
// Implemented for CActive.
// Will be called when Cancel() method is called.
// -----------------------------------------------------------------------------
//
void CMusCallEventMonitor::DoCancel()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallEventMonitor::DoCancel" )
    iCall.CancelAsyncRequest( EMobileCallNotifyCallEvent );
    MUS_LOG( "mus: [MUSAO]  <- CMusCallEventMonitor::DoCancel" )
    }

// End of file
