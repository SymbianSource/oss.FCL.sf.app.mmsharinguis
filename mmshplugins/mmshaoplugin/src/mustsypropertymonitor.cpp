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
#include "musvoipcallmonitor.h"
#include "muslogger.h"
#include "musfactorysettings.h"


#include <e32property.h>
#include <mmtsy_names.h>
#include <ctsydomainpskeys.h>


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusTsyPropertyMonitor* CMusTsyPropertyMonitor::NewL( RMobilePhone& aPhone, 
                                    MMusCallStateObserver& aCallStateObserver )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::NewL" )
    CMusTsyPropertyMonitor* self = new (ELeave) CMusTsyPropertyMonitor( aPhone, aCallStateObserver );
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
    iCallMonitorArray.ResetAndDestroy(); 
    iCallMonitorArray.Close();
    RemoveAllVoipCallMonitors();
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
CMusTsyPropertyMonitor::CMusTsyPropertyMonitor(RMobilePhone& aPhone, MMusCallStateObserver& aCallStateObserver )
    : CActive( EPriorityNormal ),iPhone(aPhone), iCallStateObserver ( aCallStateObserver )
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
    TPSCTsyCallState callState = EPSCTsyCallStateUninitialized;
    User::LeaveIfError(iPropertyEvent.Get( (TInt&)callState ));
    TPSCTsyCallType callType = EPSCTsyCallTypeUninitialized;
    User::LeaveIfError(RProperty::Get(KPSUidCtsyCallInformation,KCTsyCallType,
                                     (TInt&)callType));
    MUS_LOG1( "mus: [MUSAO]  iTsyCallMonitor->CallState = %d",callState )        
    switch(callState)
        {   
            case EPSCTsyCallStateNone:
                 MUS_LOG( "mus: [MUSAO] EPSCTsyCallStateNone" )
                 SetStateL(NMusSessionInformationApi::ENoCall);
                 // Sometimes disconnected call goes stright to this state
                 // so we have to call this function to remove call monitors.
                 MonitorCallL( callState,callType); 
                 break;
            case EPSCTsyCallStateDisconnecting:
                 MUS_LOG( "mus: [MUSAO] EPSCTsyCallStateDisconnecting" )                                
                 MonitorCallL( callState,callType);                        
                 break;
            case EPSCTsyCallStateAnswering:
                 MUS_LOG( "mus: [MUSAO] EPSCTsyCallStateAnswering" )
                 MonitorCallL( callState,callType);                      
                 break;
            case EPSCTsyCallStateConnected:                                 
                 MUS_LOG( "mus: [MUSAO] EPSCTsyCallStateConnected" )
                 MonitorCallL( callState,callType);                       
                 break;
            case EPSCTsyCallStateHold:
                 MUS_LOG( "mus: [MUSAO]  EPSCTsyCallStateHold" )
                 SetStateL(NMusSessionInformationApi::ECallHold);
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
        CMusCallMonitor* callMonitor = CMusCallMonitor::NewL( aCallName,
                                                              IsPttCallExist() );   
        CleanupStack::PushL( callMonitor );
        iCallMonitorArray.AppendL( callMonitor );
        CleanupStack::Pop( callMonitor );
        callMonitor->StartMonitorL(iLine, *this, iCallStateObserver );       
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
// Decides which call monitor (CS,VOIP etc ) has to be called.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::MonitorCallL( const TPSCTsyCallState& aCallState,
                                           const TPSCTsyCallType& aCallType)
    {
    MUS_LOG( "mus: [MUSAO]  -> CCMusTsyPropertyMonitor::MonitorCallL" )
    switch( aCallType )
        {
        case EPSCTsyCallTypeUninitialized:
        case EPSCTsyCallTypeNone:
        case EPSCTsyCallTypeFax:            
        case EPSCTsyCallTypeData:
        case EPSCTsyCallTypeHSCSD:
        case EPSCTsyCallTypeH324Multimedia:
            break;// dont care
        case EPSCTsyCallTypeCSVoice:           
            MonitorCSCallL();
            break;
        case EPSCTsyCallTypeVoIP:
            {
            // if error occured when reading factory setting key 
            // behave default ie all calls supported.
            TBool supported = ETrue;
            TRAP_IGNORE( supported = MusFactorySettings::IsSupportedL( aCallType ) )
            if( supported)
                {
                MonitorVoipCallL( aCallState );                                   
                }      
            break;           
            }
        default:
            break;
        }
    // Check now for multimediasharing call criteria.
    CheckCallCriteriaL();
    MUS_LOG( "mus: [MUSAO]  <- CCMusTsyPropertyMonitor::MonitorCallL" )
    }
// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::MonitorCSCallL() 
// Monitors the CS call and if needed adds/removes  call monitor.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::MonitorCSCallL()
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
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::MonitorCallL")
    }

// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::MonitorVoipCallL() 
// Monitors the voip call and if needed adds/removes  call monitor.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::MonitorVoipCallL( const TPSCTsyCallState& aCallState)
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::MonitorVoipCallL")
    // TODO : Call Name should be read from covergence api [CCE/CCH]
    //        which is not available as of now 22-Dec-2008
    TName callName(_L("Voip 1"));
    switch(aCallState)
       {   
       case EPSCTsyCallStateDisconnecting:
            MUS_LOG( "mus: [MUSAO] EPSCTsyCallStateDisconnecting" )                                
            RemoveVoipCallMonitor(callName);                       
            break;
       case EPSCTsyCallStateAnswering:
       case EPSCTsyCallStateConnected:
            MUS_LOG( "mus: [MUSAO] EPSCTsyCallStateAnswering/EPSCTsyCallStateConnected" )
            AddVoipCallMonitorL(callName);                       
            break;          
       default:
            break;              
       }
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::MonitorVoipCallL")
    }

// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::AddVoipCallMonitorL( TName aCallName )
// Adds new voip call monitor if the call is not monitored already.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::AddVoipCallMonitorL(TName& aCallName) 
    {
    MUS_LOG_TDESC( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::AddCallMonitorL"\
                                                                   ,aCallName )
    TBool isCallExist =  EFalse;  
    for ( TInt i = 0; i < iVoipCallMonitorArray.Count() && !isCallExist; i++ )
        {             
        if( iVoipCallMonitorArray[i]->IsEqual( aCallName ) )
            {
            isCallExist = ETrue;
            }
        }    
    MUS_LOG1( "mus: [MUSAO]  Call Exist = %d ",isCallExist )
    if( !isCallExist ) 
        {                
        CMusVoipCallMonitor* callMonitor = 
                    CMusVoipCallMonitor::NewL( aCallName, iCallStateObserver );   
        CleanupStack::PushL( callMonitor );
        iVoipCallMonitorArray.AppendL( callMonitor );
        CleanupStack::Pop( callMonitor );
        }
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::AddCallMonitorL" )
    }
    
    
// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::RemoveCallEventMonitorL( TName aCallName ) 
// Remove the Call Monitor if it exist in array.
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::RemoveVoipCallMonitor( TName& aCallName ) 
    {
    MUS_LOG_TDESC( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::RemoveCallMonitor "
                                                                 ,aCallName )
    for ( TInt i = 0; i < iVoipCallMonitorArray.Count(); i++ )
        {
        if( iVoipCallMonitorArray[i]->IsEqual( aCallName ) )
            {
            delete iVoipCallMonitorArray[i];
            iVoipCallMonitorArray.Remove(i);
            break;
           }
        }
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::RemoveCallMonitor" )
    }

// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::RemoveAllVoipCallMonitors 
// Remove all voip call monitors
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::RemoveAllVoipCallMonitors( ) 
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::RemoveAllVoipCallMonitors ")
    iVoipCallMonitorArray.ResetAndDestroy();
    iVoipCallMonitorArray.Close();
    MUS_LOG( "mus: [MUSAO]  <- CMusTsyPropertyMonitor::RemoveAllVoipCallMonitors" )
    }

// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::CheckCallCriteria 
// Checks the Multimediasharing call criterias
// -----------------------------------------------------------------------------
//
void CMusTsyPropertyMonitor::CheckCallCriteriaL()
    {
    TInt csCall = iCallMonitorArray.Count();
    TInt voipCall = iVoipCallMonitorArray.Count();
    switch( csCall )
        {
        case 0 : 
            {
            switch( voipCall )
                {
                /* No cs call or voip call */
                case 0 : 
                        {
                        User::LeaveIfError(
                         RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                        NMusSessionInformationApi::KMusCallEvent,
                                        0 ));   
                        SetStateL(NMusSessionInformationApi::ENoCall);
                        break;
                        }                    
                /* No cs call but one voip call */
                case 1 : 
                        {
                        iVoipCallMonitorArray[0]->
                            SetStateL(NMusSessionInformationApi::ECallConnected);
                        break;
                        }
                /* No cs call but more than one voip call */    
                default: SetStateL(NMusSessionInformationApi::ECallHold);
                         break;
                }
            break;
            }
        case 1 : 
            {
            switch( voipCall )
                {
                /* One cs call but no voip call */
                case 0  : break;// Will be handled by cs call monitors.
                /* One cs call and one or more voip calls */
                default : SetStateL(NMusSessionInformationApi::ECallHold);
                         break;                
                }
            break;
            }
        default: // 2 or more CS calls
            {
            /* Sometimes when call get disconnected then call object disappears.So
             * check the call state or open the call , if there is an error or undesired
             * call state , remove that callmonitor from array.
             */
            RemoveUnUsedCallMonitors();    

            /* It is already multicall so atleast one should be under hold.
             * so set the state first to hold and then let the conference 
             * monitor decide the conference state.
             */
            SetStateL(NMusSessionInformationApi::ECallHold);  
            
            /* Still we dont know here about conference state so 
             * let the conference call monitor decide .
             */
            iConferenceMonitor->SetStateL();         
            break;
            }
        }
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



// -----------------------------------------------------------------------------
// CMusTsyPropertyMonitor::IsDataReadyL 
// -----------------------------------------------------------------------------
//
TBool CMusTsyPropertyMonitor::IsDataReadyL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusTsyPropertyMonitor::IsDataReadyL" )

    TBool dataReady = EFalse;
    TInt csCall = iCallMonitorArray.Count();
    TInt voipCall = iVoipCallMonitorArray.Count();
    
    // In case of Conference Notify that data is ready.
    if ( iConferenceMonitor->GetConfStatus() !=  RMobileConferenceCall::EConferenceIdle )
        {
        MUS_LOG( "mus: [MUSAO]  Conference CASE" )
        dataReady = ETrue;
        }
    else if ( csCall == 1 && !voipCall  )
        {
        MUS_LOG( "mus: [MUSAO]  CS CALL" )
        dataReady = iCallMonitorArray[0]->IsDataReadyL();
        }
    else if ( voipCall == 1 && !csCall )
        {
        MUS_LOG( "mus: [MUSAO]  VOIP CALL" )
        dataReady = iVoipCallMonitorArray[0]->IsDataReadyL();
        }

    MUS_LOG1( "mus: [MUSAO] <- CMusTsyPropertyMonitor::IsDataReadyL = %d",dataReady)
    return dataReady;
    }

// End of file

