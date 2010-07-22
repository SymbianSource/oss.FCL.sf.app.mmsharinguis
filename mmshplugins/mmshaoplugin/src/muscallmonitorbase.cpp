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
#include "muscallmonitorbase.h"
#include "muslogger.h"

#include <etel.h>
#include <etelmm.h>
#include <e32svr.h>
#include <mmtsy_names.h>
#include <e32property.h>


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusCallMonitorBase::~CMusCallMonitorBase()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitorBase::~CMusCallMonitorBase" )        
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitorBase::~CMusCallMonitorBase" )
    }

// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusCallMonitorBase::CMusCallMonitorBase(const RMobileCall& aCall, 
        MMusTsyPropertyObserver& aObserver, 
        MMusCallStateObserver& aCallStateObserver ) 
        : CActive( EPriorityNormal ),iCall(aCall),iTsyObserver( aObserver ), 
        iCallStateObserver( aCallStateObserver )
        
        
    {   
	// Intialize the remote call event state to Resume , means connected.
    iRemoteCallEvent = RMobileCall::ERemoteResume;
    // Intialize the local call event state to Resume , means connected.
    iLocalCallEvent = RMobileCall::ELocalResume;
    }

// -----------------------------------------------------------------------------
// CMusCallMonitorBase::SetStateL( TInt aError )
// Sets the Call State to Property
// -----------------------------------------------------------------------------
//
void CMusCallMonitorBase::SetStateL(NMusSessionInformationApi::TMusCallEvent aVal)
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusCallMonitorBase::SetStateL, aVal %d", aVal )
    TInt callCount = 0;
    User::LeaveIfError(RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMusCallCount,callCount));
    MUS_LOG1( "mus: [MUSAO]  - CallCount = %d",callCount )
    
    RMobileCall::TMobileCallInfoV3 callInfo;
    RMobileCall::TMobileCallInfoV3Pckg pckg( callInfo );
    iCall.GetMobileCallInfo( pckg );
    if( callInfo.iEmergency )
    	{
    	User::LeaveIfError(RProperty::Set( NMusSessionInformationApi::KCategoryUid,
    				 NMusSessionInformationApi::KMusCallEvent,
    				 ( TInt ) NMusSessionInformationApi::ENoCall ));
    	}
    /*
    * CountOfCallMonitors is number of CallMonitor Objects.Static Reference Count.
    * = No of active calls . 
    * React one and only if one call is present -> Mush Call criteria.
    * if call count is more than one , should be taken care by conference call or
    * multicall criteria.
    */
    else if( callCount == 1 ) 
        {
        TInt currentVal;
        User::LeaveIfError(RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMusCallEvent,currentVal));
        MUS_LOG1( "mus: [MUSAO]  - Remote State = %d",iRemoteCallEvent )
        /* If current event is callconnected and remote is not in hold set the
           property value to CallConnected.Else set it to CallHold.
           Note : We can be sure about Local is connected when it goes to
                  CallConnected State. But remote call events are just notified
                  in call event monitor. So this stores the last remotecall event
                  and checks it now.
        */        
        if( aVal==NMusSessionInformationApi::ECallConnected 
            && iRemoteCallEvent!=RMobileCall::ERemoteHold)
            {
            /* If the current property value is same of new value, there is no
               point of setting it since it will cause lot of other processes.               
            */
            if(currentVal != (TInt)aVal)
                {
                User::LeaveIfError(RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                                 NMusSessionInformationApi::KMusCallEvent,
                                                 aVal ));    
                }
            } 
        else if( aVal==NMusSessionInformationApi::ECallHold || iRemoteCallEvent==RMobileCall::ERemoteHold )
            {            
            User::LeaveIfError(RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                               NMusSessionInformationApi::KMusCallEvent,
                                               NMusSessionInformationApi::ECallHold   ));  
            }          
         else if( aVal==NMusSessionInformationApi::ENoCall )
            {            
            User::LeaveIfError(RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                               NMusSessionInformationApi::KMusCallEvent,
                                               NMusSessionInformationApi::ENoCall ));  
            }  
          else if( aVal==NMusSessionInformationApi::EConferenceCall )
            {            
            User::LeaveIfError(RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                               NMusSessionInformationApi::KMusCallEvent,
                                               NMusSessionInformationApi::EConferenceCall ));  
            }  
         	  
        /*
         If Call is Connected then set the Call info.
         */
        if( aVal==NMusSessionInformationApi::ECallConnected ) 
            {
            SetCallInfoL();
            }

        // All P/S Keys are Set
        //Check if preconditions are met to start or stop the MushClient.
        iCallStateObserver.MusCallStateChanged();
        }   
 
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitorBase::SetStateL" )    
    }

///-----------------------------------------------------------------------------
// CMusCallMonitorBase::SetCallInfoL( TInt aError )
// -----------------------------------------------------------------------------
//
void CMusCallMonitorBase::SetCallInfoL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitorBase::SetCallInfoL ")           
    RMobileCall::TMobileCallInfoV3 callInfo;
    RMobileCall::TMobileCallInfoV3Pckg callInfoPckg( callInfo );
    User::LeaveIfError(iCall.GetMobileCallInfo( callInfoPckg ));    
    RMobilePhone::TMobileAddress remoteNumber = 
                                           callInfo.iRemoteParty.iRemoteNumber;
     // Set telephone number
    HBufC* telNo( NULL );
    if( remoteNumber.iTelNumber.Length() > 0 )
        {
        MUS_LOG( "mus: [MUSAO]     remoteNumber.iTelNumber.Lenght > 0" )
        telNo = remoteNumber.iTelNumber.AllocLC();
        }
    else
        {
        MUS_LOG( "mus: [CALLMN]     remoteNumber.iTelNumber.Lenght =< 0" )
        RMobilePhone::TMobileAddress dialledParty = callInfo.iDialledParty;
        telNo = dialledParty.iTelNumber.AllocLC();
        }
    User::LeaveIfError(RProperty::Set( 
                                      NMusSessionInformationApi::KCategoryUid,
                                      NMusSessionInformationApi::KMusTelNumber,
                                      *telNo ));  
    MUS_LOG_TDESC("mus: [MUSAO]     telNo = ", (*telNo))
    CleanupStack::PopAndDestroy(telNo);
    
    // Set call direction.     
    RMobileCall::TMobileCallDirection direction =
                                            callInfo.iRemoteParty.iDirection;
    MUS_LOG1( "mus: [MUSAO]  Call Direction = %d",direction )
    if( direction == RMobileCall::EMobileOriginated )
        {
        User::LeaveIfError(RProperty::Set( 
                        NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMusCallDirection,
                        ( TInt ) NMusSessionInformationApi::ECallOrginated ));
        }
    else if ( direction  == RMobileCall::EMobileTerminated )
        {
        User::LeaveIfError(RProperty::Set( 
                        NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMusCallDirection,
                        ( TInt ) NMusSessionInformationApi::ECallTerminated ));
        }
    else
        {
        User::LeaveIfError(RProperty::Set( 
                        NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMusCallDirection,
                        ( TInt ) NMusSessionInformationApi::ENoDirection ));
        } 
    
    
    /* Set the call provider information to some dummy Value */
    User::LeaveIfError(RProperty::Set( 
                            NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMUSCallProvider,
                            KNullDesC));
    
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitorBase::SetCallInfoL" )  
    }

// -----------------------------------------------------------------------------
// CMusCallMonitorBase::RunError( TInt aError )
// Implemented for CActive.
// Will be called automatically by framework when RunL Leaves.
// -----------------------------------------------------------------------------
//

TInt CMusCallMonitorBase::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusCallMonitorBase::RunError, aError %d",\
                                                                      aError )
    aError = KErrNone; // just to avoid warnings.
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitorBase::RunError" )    
    return aError;
    }

// -----------------------------------------------------------------------------
// CMusCallMonitorBase::NotifyCallStateChanged()
// Notify TSYobserver that our call status or call event state changed
// -----------------------------------------------------------------------------
//
void CMusCallMonitorBase::NotifyCallStateChanged( NMusSessionInformationApi::TMusCallEvent aVal )
    {
    RCall::TCallInfo callInfo;
    iCall.GetInfo(callInfo);
    iTsyObserver.NotifyCallStateChanged( aVal, callInfo.iCallName );
    }


// --------------------------------------------------------------------------------
// CMusCallConferenceMonitor::IsDataReadyL()
// Checks if Data is ready CS Call.
// --------------------------------------------------------------------------------

TBool CMusCallMonitorBase::IsDataReadyL()
    {
    // This function need to be updated when new 
    // P/S key is published in Call monitor 

    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitorBase::IsDataReadyL" )
    
    // Ensure there is tel Number.
    HBufC* telNumInfo = HBufC::NewLC( RProperty::KMaxPropertySize );
    TPtr ptelInfo = telNumInfo->Des();
    TInt err  = KErrNone;
       
    err = RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMusTelNumber,
                            ptelInfo );

    TBool telInfoExisit ( err == KErrNone && telNumInfo->Des().Length() > 0 );
    CleanupStack::PopAndDestroy( telNumInfo );
    
    // Ensure Call direction info is there
    TInt callDirection = NMusSessionInformationApi::ENoDirection;
    err = RProperty::Get( 
                    NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMusCallDirection,
                    callDirection );
    
    TBool callInfoExisit ( err == KErrNone && callDirection != NMusSessionInformationApi::ENoDirection );
    
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitorBase::IsDataReadyL" )
    return telInfoExisit && callInfoExisit;
    }

// End of file

