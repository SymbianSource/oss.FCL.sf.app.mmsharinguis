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
* Description:  Monitor that observes the status of the current CS call
*
*/

#include "mussesseioninformationapi.h"
#include "mustsypropertymonitor.h"
#include "muscallconferencemonitor.h"
#include "muscallmonitor.h"
#include "muslogger.h"
#include "mussettings.h"
#include "musclirmonitor.h"

#include <etel.h>
#include <e32property.h>
#include <mmtsy_names.h>
#include <ctsydomainpskeys.h>


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusTsyPropertyMonitor* CMusTsyPropertyMonitor::NewL( RMobilePhone& aPhone )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::NewL" )
    CMusTsyPropertyMonitor* self = new (ELeave) CMusTsyPropertyMonitor(aPhone);
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusTsyPropertyMonitor::~CMusTsyPropertyMonitor()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::~CMusTsyPropertyMonitor" )    
    Cancel();
    delete iConferenceMonitor;
    delete iClirMonitor;
    iCallMonitorArray.ResetAndDestroy();
    iLine.Close();
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::~CMusTsyPropertyMonitor" )
    }

// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::ConstructL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::ConstructL" )
    CActiveScheduler::Add(this);
    User::LeaveIfError( iLine.Open( iPhone, KMmTsyVoice1LineName() ) );
    iConferenceMonitor = CMusCallConferenceMonitor::NewL(
                                                iPhone,iLine,iCallMonitorArray);
    
    if (MultimediaSharingSettings::PrivacySetting())
        {
        iClirMonitor = CMusClirMonitor::NewL();
        }
    
    User::LeaveIfError( iPropertyEvent.Attach(
                                KPSUidCtsyCallInformation,
                                KCTsyCallState ) );
    iPropertyEvent.Subscribe( iStatus );    
    SetActive();
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::ConstructL" )
    }

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusTsyPropertyMonitor::CMusTsyPropertyMonitor(RMobilePhone& aPhone)
    : CActive( EPriorityNormal ),iPhone(aPhone)
    {
    }

// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::RunL()
// Implemented for CActive.It will be called any NotifyMobileLineStatusChange
// Event happens.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::RunL()
    {
    MUS_LOG( "mus: [MUSAO] -> CMusTsyPropertyMonitor::RunL" )
    // subscribe , so that we dont miss any events.
    iPropertyEvent.Subscribe( iStatus );    
    SetActive();
    TInt value = EPSCTsyCallStateUninitialized;
    User::LeaveIfError(iPropertyEvent.Get( value )); 
    MUS_LOG1( "mus: [MUSAO]  iTsyCallMonitor->CallState = %d",value )        
    switch(value)
        {   
            case EPSCTsyCallStateNone:                 
                 SetStateL(NMusSessionInformationApi::ENoCall);
                 // Sometimes disconnected call goes stright to this state
                 // so we have to call this function to remove call monitors.
                 RemoveAllCallMonitor();
                 break;                
            case EPSCTsyCallStateDisconnecting:                 
            case EPSCTsyCallStateAnswering:                 
            case EPSCTsyCallStateConnected:                                             
            case EPSCTsyCallStateHold:                                    
                  // in all above states if multible call exist then 
                 // we have to find all the calls state and do accordingly.
                 MonitorCallL(); 
                 break;
            default:
                 MUS_LOG( "mus: [MUSAO]  Undesired CallState " )   
                 break;              
        }    
    MUS_LOG( "mus: [MUSAO] <- CMusTsyPropertyMonitor::RunL" )
    }


// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::DoCancel()
// Implemented for CActive.
// Will be called when Cancel() method is called.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::DoCancel()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::DoCancel" )
    iPropertyEvent.Cancel();
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::DoCancel" )
    }

// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::RunError( TInt aError )
// Implemented for CActive.
// Will be called automatically by framework when RunL Leaves.
// -----------------------------------------------------------------------------
//
TInt CMusTsyPropertyMonitor::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::RunError = %d", aError )    
    aError = KErrNone; // Just to avoid compilation warnings. Dunno what we do.
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::RunError" )
    return aError;    
    }

// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::AddCallMonitorL( TName aCallName )
// Adds new call monitor if the call is not monitored already.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::AddCallMonitorL(TName& aCallName) 
    {
    MUS_LOG_TDESC( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::AddCallMonitorL"\
                                                                   ,aCallName )
    TBool isCallExist =  EFalse;
    for ( TInt i = 0; i < iCallMonitorArray.Count() && !isCallExist; i++ )
        {             
        if( iCallMonitorArray[i]->IsEqual( aCallName ) )
            {
            isCallExist = ETrue;
            }
        }
    MUS_LOG1( "mus: [MUSAO]  Call Exist = %d ",isCallExist )
    if( !isCallExist ) 
        {
        if ( iClirMonitor )
            {
            //Start async reading of CLIR from phone settings
            iClirMonitor->ReadClir();
            }
        
        CMusCallMonitor* callMonitor = CMusCallMonitor::NewL( aCallName,
                                                              IsPttCallExist() );   
        CleanupStack::PushL( callMonitor );
        iCallMonitorArray.AppendL( callMonitor );
        CleanupStack::Pop( callMonitor );
             callMonitor->StartMonitorL(iLine, *this);       
        }
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::AddCallMonitorL" )
    }
    
    
// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::RemoveCallEventMonitorL( TName aCallName ) 
// Remove the Call Monitor if it exist in array.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::RemoveCallMonitor( TName& aCallName ) 
    {
    MUS_LOG_TDESC( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::RemoveCallMonitor "
                                                                    ,aCallName )
    for ( TInt i = 0; i < iCallMonitorArray.Count(); i++ )
        {
        if( iCallMonitorArray[i]->IsEqual( aCallName ) )
            {            
            delete iCallMonitorArray[i];
            iCallMonitorArray.Remove(i);                      
            break;
            }
        }     
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::RemoveCallMonitor" )
    }

// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::RemoveCallEventMonitorL( TName aCallName ) 
// Remove the Call Monitor if it exist in array.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::RemoveAllCallMonitor() 
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::RemoveAllCallMonitor " )
    iCallMonitorArray.ResetAndDestroy();     
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::RemoveAllCallMonitor" )
    }
// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::IsPTTCallExist() 
// Remove the Call Monitor if it exist in array.
// -----------------------------------------------------------------------------
//
TBool CMusTsyPropertyMonitor::IsPttCallExist() 
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::IsPTTCallExist")        
    TBool exist = EFalse;
    /*
    TInt val = 0;
    TInt err = RProperty::Get(KPSUidCtsyCallInformation,KCTsyKodiakPtt,val );
    MUS_LOG2( "mus: [MUSAO] PTTCallValue = val  Error = %d",val,err )    
    if(!err && val== EPSCTsyKodiakPttActive )
        {        
        exist = ETrue;
        }
    else
        {
        // if there is an error dont know what to do. 
        // there could be cases like KodiakPTT is not supported
        // So considering there is no PttCall.
        exist = EFalse;
        }    
    */
    MUS_LOG1( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::IsPTTCallExist = %d",\
                                                                        exist )
    return exist;
    }


// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::IsPTTCallExist() 
// Remove the Call Monitor if it exist in array.
// -----------------------------------------------------------------------------
//

// Enable the below line if Kodiak Ptt has to be monitered
/*
void CMusTsyPropertyMonitor::PttCallEventOccuredL( TPSCTsyKodiakPtt& aVal)
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::PttCallEventOccured\
                                                                   = %d",aVal)
 
    
    if (iCallMonitorArray.Count()>0)
        {
        if( aVal == EPSCTsyKodiakPttNotActive && iCallMonitorArray.Count()==1)
            {                                
            iCallMonitorArray[0]->StartMonitorL(iLine);                    
            }
        else 
            {                      
            iCallMonitorArray[0]->StopMonitorL();                            
            }        
        }
    else
        {
        // No CS call. Dont care this event.    
        }    
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::PttCallEventOccured" )
    }
*/


// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::SetStateL( TInt aError )
// Sets the Call State to Property
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::SetStateL(
                                   NMusSessionInformationApi::TMusCallEvent aVal)
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::SetStateL,aVal %d",aVal )
    TInt currentVal;
    User::LeaveIfError(RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,currentVal));
    if(currentVal != (TInt)aVal)
        {
        User::LeaveIfError(RProperty::Set( 
                                        NMusSessionInformationApi::KCategoryUid,
                                        NMusSessionInformationApi::KMusCallEvent,
                                        aVal ));    
        }
    else
        {
         // No point of reporting it .
        }    
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::SetStateL" )    
    }


// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::MonitorCallL() 
// Monitors the call and if needed adds/removes  call monitor.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::MonitorCallL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::MonitorCallL")
    TInt callCount;
    iLine.EnumerateCall(callCount);
    MUS_LOG1( "mus: [MUSAO]  -> callCount = %d",callCount)    
    RLine::TCallInfo callInfo;
    for(TInt i=0;i<callCount;i++)
        {        
        iLine.GetCallInfo(i,callInfo);
        MUS_LOG1( "mus: [MUSAO]  Call index = %d",i)
        MUS_LOG_TDESC("mus: [MUSAO]  Call Name = ",callInfo.iCallName);
        MUS_LOG1( "mus: [MUSAO]  Call Status = %d",callInfo.iStatus );         
        if(callInfo.iStatus == RCall::EStatusIdle || 
           callInfo.iStatus == RCall::EStatusHangingUp)
            {
            RemoveCallMonitor(callInfo.iCallName);        
            }
        else if(callInfo.iStatus == RCall::EStatusConnected ||
                callInfo.iStatus == RCall::EStatusAnswering)
            {
            AddCallMonitorL(callInfo.iCallName); 
            }                                  
        }
    // Sometimes when call get disconnected then call object disappears.So
    // check the call state or open the call , if there is an error or undesired
    // call state , remove that callmonitor from array. 
    RemoveUnUsedCallMonitors();    

    if(iCallMonitorArray.Count() > 1)
        {                
        // it is already multicall so atleast one should be hold.
        // so set the state first to hold
        SetStateL(NMusSessionInformationApi::ECallHold);               
        // still we dont know here about conference state so 
        // let the conference call monitor decide .
        iConferenceMonitor->SetStateL(); 
        }
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::MonitorCallL")
    }


// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::CallStatusL() 
// Somehow Line is not returning correct call info when call disconnected from
// remote,So open the call and ask call status
// -----------------------------------------------------------------------------
//
RMobileCall::TMobileCallStatus CMusTsyPropertyMonitor::CallStatus(const TDesC& aCallName)
    {
    MUS_LOG_TDESC( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::CallStatusL",aCallName)
    RMobileCall call;
    TInt error = call.OpenExistingCall( iLine, aCallName );      
    RMobileCall::TMobileCallStatus callStatus = RMobileCall::EStatusUnknown;
    if(error==KErrNone)
        {
        // if error occur here , then leave the status to default.
        call.GetMobileCallStatus(callStatus);
        }    
    call.Close();
    MUS_LOG1( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::CallStatusL = %d",callStatus)
    return callStatus;
    }

// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::RemoveUnUsedCallMonitors() 
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor:: RemoveUnUsedCallMonitors()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::RemoveUnUsedCallMonitors" )          
    for ( TInt i = 0; i < iCallMonitorArray.Count(); i++ )
        {
        if( CallStatus( iCallMonitorArray[i]->Name() ) == 
                                      RMobileCall::EStatusUnknown )
            {
            MUS_LOG_TDESC("mus: [MUSAO]  Removing = ",iCallMonitorArray[i]->Name());
            delete iCallMonitorArray[i];            
            iCallMonitorArray[i] = NULL;
            iCallMonitorArray.Remove(i); 
            // this lowers the call monitor index by one , so lower the i by one too
            i--;     
            }    
        }    
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::RemoveUnUsedCallMonitors" )
    }

// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::NotifyCallStateChanged() 
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::NotifyCallStateChanged(NMusSessionInformationApi::TMusCallEvent aVal, TName& aCallName)
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::NotifyCallStateChanged" )
              
    if(aVal == NMusSessionInformationApi::ENoCall )
           {
           // Call has been disconnected,
           // Remove call monitor from array
           RemoveCallMonitor(aCallName);
           if( iCallMonitorArray.Count() == 0 )
               {
               SetStateL(NMusSessionInformationApi::ENoCall);
               }
           else if( iCallMonitorArray.Count() == 1)
               {
               // Only one call left, set its state to be ECallConnected here
               // The real state of the call will be calculated by CMusCallMonitorBase
               iCallMonitorArray[0]->SetStateL(NMusSessionInformationApi::ECallConnected);
               }
           else
               {
               // it is already multicall so atleast one should be hold.
               // so set the state first to hold
               SetStateL(NMusSessionInformationApi::ECallHold);               
               // still we dont know here about conference state so 
               // let the conference call monitor decide .
               iConferenceMonitor->SetStateL(); 
               }
           }
    
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::NotifyCallStateChanged" )
    }
// End of file
