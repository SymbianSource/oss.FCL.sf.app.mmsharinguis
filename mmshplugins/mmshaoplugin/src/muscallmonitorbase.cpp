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
#include "mussettings.h"

#include <etel.h>
#include <etelmm.h>
#include <e32svr.h>
#include <mmtsy_names.h>
#include <e32property.h>

_LIT( KDoNotSendOwnNumber, "#31#" );
_LIT( KDoSendOwnNumber, "*31#" );


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
CMusCallMonitorBase::CMusCallMonitorBase(const RMobileCall& aCall, MMusTsyPropertyObserver& aObserver ) 
        : CActive( EPriorityNormal ),iCall(aCall),iTsyObserver( aObserver )
        
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
    /*
    * CountOfCallMonitors is number of CallMonitor Objects.Static Reference Count.
    * = No of active calls . 
    * React one and only if one call is present -> Mush Call criteria.
    * if call count is more than one , should be taken care by conference call or
    * multicall criteria.
    */
     if( callCount == 1) 
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
        if( aVal != NMusSessionInformationApi::ENoCall)
            {
            RMobileCall::TMobileCallInfoV3 callInfo;
            RMobileCall::TMobileCallInfoV3Pckg pckg( callInfo );
    
            User::LeaveIfError(iCall.GetMobileCallInfo( pckg ));  
            if( callInfo.iEmergency )
                    {
                    User::LeaveIfError(RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                             NMusSessionInformationApi::KMusCallEvent,
                                             ( TInt ) NMusSessionInformationApi::ENoCall ));
                    }
            }
        /*
         If Call is Connected then set the Call info.
         */
        if( aVal==NMusSessionInformationApi::ECallConnected ) 
            {
            SetCallInfoL();
            }
        
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
    HBufC* cleanTelNo( NULL );
    if( remoteNumber.iTelNumber.Length() > 0 )
        {
        MUS_LOG( "mus: [MUSAO]     remoteNumber.iTelNumber.Lenght > 0" )
        telNo = remoteNumber.iTelNumber.AllocLC();
        }
    else
        {
        MUS_LOG( "mus: [MUSAO]     remoteNumber.iTelNumber.Lenght =< 0" )
        RMobilePhone::TMobileAddress dialledParty = callInfo.iDialledParty;
        telNo = dialledParty.iTelNumber.AllocLC();
        
        }
    if ( MultimediaSharingSettings::PrivacySetting() )
        {
        SetClirSetting( *telNo );
        }
    cleanTelNo = RemovePrefix( *telNo );
    CleanupStack::PushL( cleanTelNo );  
    
    User::LeaveIfError(RProperty::Set( 
                                      NMusSessionInformationApi::KCategoryUid,
                                      NMusSessionInformationApi::KMusTelNumber,
                                      *cleanTelNo ));  
    MUS_LOG_TDESC("mus: [MUSAO]     telNo = ", (*cleanTelNo))
    CleanupStack::PopAndDestroy(cleanTelNo);
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
        SetTerminatingPrivacy( iCall );
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

// --------------------------------------------------------------------------
// void CMusCallMonitorBase::SetClirSetting()
// Determine CLIR setting from dialled digits
// --------------------------------------------------------------------------
//
void CMusCallMonitorBase::SetClirSetting( const TDesC& aDialledNumber ) const
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitorBase::SetClirSetting" );

    NMusSessionInformationApi::TMusClirSetting 
        sendNumber( NMusSessionInformationApi::ESendOwnNumber );

    TPtrC prefix = aDialledNumber.Left( KDoNotSendOwnNumber().Length() );
    if ( prefix == KDoNotSendOwnNumber() )
        {
        sendNumber = NMusSessionInformationApi::EDoNotSendOwnNumber;
        }
    RProperty::Set( NMusSessionInformationApi::KCategoryUid, 
                    NMusSessionInformationApi::KMusClirSetting, sendNumber );
    MUS_LOG1( "mus: [MUSAO]  <- CMusCallMonitorBase::SetClirSetting send nbr=%d",
    sendNumber == NMusSessionInformationApi::ESendOwnNumber );
    }


// --------------------------------------------------------------------------
// void CMusCallMonitorBase::RemovePrefix()
// --------------------------------------------------------------------------
//
HBufC* CMusCallMonitorBase::RemovePrefix( const TDesC& aOriginator ) const
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitorBase::removePrefix" );

    HBufC * withoutPrefix(NULL);
    
    TPtrC prefix = aOriginator.Left( KDoNotSendOwnNumber().Length() );
    if ( prefix == KDoNotSendOwnNumber()|| prefix == KDoSendOwnNumber() )
        {
        MUS_LOG( "Number have a prefix" );
        withoutPrefix = aOriginator.AllocL();
        *withoutPrefix = aOriginator;
        TPtr ptr = withoutPrefix->Des();
        ptr.Delete(0,KDoNotSendOwnNumber().Length());
        }
    else
        {
        withoutPrefix = aOriginator.AllocL();
        }
    
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitorBase::RemovePrefix" );
    return withoutPrefix;
    }


// --------------------------------------------------------------------------
// void CMusCallMonitorBase::SetTerminatingPrivacy()
// --------------------------------------------------------------------------
//
void CMusCallMonitorBase::SetTerminatingPrivacy( const RMobileCall& aCall ) const
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallMonitorBase::SetTerminatingPrivacy" )
    RMobileCall::TMobileCallInfoV3 callInfo;
    RMobileCall::TMobileCallInfoV3Pckg callInfoPckg( callInfo );
    if( aCall.GetMobileCallInfo( callInfoPckg ) != KErrNone )
        {
        return; // error cannot do
        }
    MUS_LOG( "mus: [MUSAO] Get remote status" )
    RMobileCall::TMobileCallRemoteIdentityStatus remoteStatus = 
                    callInfo.iRemoteParty.iRemoteIdStatus;
            
    if( remoteStatus == RMobileCall::ERemoteIdentitySuppressed )
        {
         RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMUSPrivacy,
                        ( TInt ) NMusSessionInformationApi::EPrivacyOn );
        }
    else
        {
        RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMUSPrivacy,
                        ( TInt ) NMusSessionInformationApi::EPrivacyOff );
        }
    MUS_LOG1( "mus: [MUSAO] Remote status = %d",remoteStatus )
    MUS_LOG( "mus: [MUSAO]  <- CMusCallMonitorBase::SetTerminatingPrivacy" )
    }

// End of file

