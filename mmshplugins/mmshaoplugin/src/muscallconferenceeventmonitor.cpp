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
* Description:  Monitor the status of CS Conference call
*
*/
 

#include "mussesseioninformationapi.h"
#include "muslogger.h"
#include "muscallconferenceeventmonitor.h"

#include <etelmm.h>
#include <e32svr.h>
#include <mmtsy_names.h>
#include <e32property.h>


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusCallConferenceEventMonitor* CMusCallConferenceEventMonitor::NewL( RMobilePhone& aPhone,
                              RMobileLine& aLine,
                              RPointerArray<CMusCallMonitor>& aCallMonitorArray )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceEventMonitor::NewL" )
    CMusCallConferenceEventMonitor* self = new (ELeave) 
                              CMusCallConferenceEventMonitor(aLine,aCallMonitorArray);
    CleanupStack::PushL( self );
    self->ConstructL( aPhone );
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceEventMonitor::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusCallConferenceEventMonitor::~CMusCallConferenceEventMonitor()
    {
    MUS_LOG( "mus: [MUSAO]  -> \
                        CMusCallConferenceEventMonitor::~CMusCallConferenceEventMonitor" )        
    Cancel();
    iConfCall.Close();        
    MUS_LOG( "mus: [MUSAO]  <- \
                        CMusCallConferenceEventMonitor::~CMusCallConferenceEventMonitor" )
    }

// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusCallConferenceEventMonitor::ConstructL( RMobilePhone& aPhone )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceEventMonitor::ConstructL" )   
    CActiveScheduler::Add( this );            
    User::LeaveIfError(iConfCall.Open(aPhone));    
    iConfCall.NotifyConferenceEvent(iStatus,iConfEvent,iCallName);   
    SetActive();        
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceEventMonitor::ConstructL" )
    }


// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusCallConferenceEventMonitor::CMusCallConferenceEventMonitor(RMobileLine& aLine,
                            RPointerArray<CMusCallMonitor>& aCallMonitorArray) 
        : CActive( EPriorityNormal ),
          iLine(aLine),
          iCallMonitorArray(aCallMonitorArray)
          
        
    {    
    }


// --------------------------------------------------------------------------------
// CMusCallConferenceEventMonitor::RunL()
// Implemented for CActive.It will be called any NotifyMobileLineStatusChange
// Event happens.
// --------------------------------------------------------------------------------
//
void CMusCallConferenceEventMonitor::RunL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceEventMonitor::RunL" )    
    iConfCall.NotifyConferenceEvent(iStatus,iConfEvent,iCallName); 
    SetActive();       
    MUS_LOG1( "mus: [MUSAO]  Conference->iConfEvent =  %d", iConfEvent ) 
    switch(iConfEvent)    
        {         
            case RMobileConferenceCall::EConferenceCallAdded:
                 SetStateL( NMusSessionInformationApi::EConferenceCall );                           	
                 break;
            case RMobileConferenceCall::EConferenceCallRemoved:	                 
                 MonitorCallL();
                 break;
            case RMobileConferenceCall::EConferenceBuilt:
                 SetStateL( NMusSessionInformationApi::EConferenceCall ); 
                 break;
            case RMobileConferenceCall::EConferenceTerminated:
                 SetStateL( NMusSessionInformationApi::EConferenceTerminated );
                 MonitorCallL();
                 break;
            case RMobileConferenceCall::EConferenceSwapped:
                 SetStateL( NMusSessionInformationApi::EConferenceCall );	
                 break;
            case RMobileConferenceCall::EConferenceSplit:
                 SetStateL( NMusSessionInformationApi::EConferenceCall );	
                 break;
            default:
                 break;
        }
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceEventMonitor::RunL" )
    }


// --------------------------------------------------------------------------------
// CMusCallConferenceEventMonitor::DoCancel()
// Implemented for CActive.
// Will be called when Cancel() method is called.
// --------------------------------------------------------------------------------
//
void CMusCallConferenceEventMonitor::DoCancel()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceEventMonitor::DoCancel" )
    iConfCall.CancelAsyncRequest( EMobileConferenceCallNotifyConferenceEvent );
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceEventMonitor::DoCancel" )
    }

// --------------------------------------------------------------------------------
// CMusCallConferenceEventMonitor::RunError( TInt aError )
// Implemented for CActive.
// Will be called automatically by framework when RunL Leaves.
// --------------------------------------------------------------------------------
//

TInt CMusCallConferenceEventMonitor::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusCallConferenceEventMonitor::RunError, aError %d", \
                                                                          aError )
    aError = KErrNone;
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceEventMonitor::RunError" )    
    return aError;
    }

// --------------------------------------------------------------------------------
// CMusCallConferenceEventMonitor::SetStateL( TInt aError )
// Sets the Call State to Property
// --------------------------------------------------------------------------------
//
void CMusCallConferenceEventMonitor::SetStateL(
                                      NMusSessionInformationApi::TMusCallEvent aVal)
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusCallConferenceEventMonitor::SetStateL, aVal %d",\
                                                                              aVal )
    TInt currentVal;
    User::LeaveIfError(RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,currentVal));
    if(currentVal != (TInt)aVal)
        {
        User::LeaveIfError(RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     aVal ));    
        }
    else
        {
         // No point of reporting it .
         //EConferenceTerminated must be set every time to notify avaeventmonitor
        if ( aVal == NMusSessionInformationApi::EConferenceTerminated )
        	{
			User::LeaveIfError(RProperty::Set( NMusSessionInformationApi::KCategoryUid,
						 NMusSessionInformationApi::KMusCallEvent,
						 aVal ));  
        	}
    
        }    
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceEventMonitor::SetStateL" )    
    }

// --------------------------------------------------------------------------------
// CMusCallConferenceEventMonitor::MonitorCallL() 
// If the call in conference is dropped that has to be removed.
// from call monitoring array.and start monitoring.
// --------------------------------------------------------------------------------
//
void CMusCallConferenceEventMonitor::MonitorCallL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceEventMonitor::MonitorCallL")
    TInt callCount;
    iLine.EnumerateCall(callCount);
    MUS_LOG1( "mus: [MUSAO]  -> callCount = %d",callCount)    
    RLine::TCallInfo callInfo;
    for(TInt i=0;i<callCount;i++)
        {
        MUS_LOG( "mus: [MUSAO]  -----------------------------------\n" )
        iLine.GetCallInfo(i,callInfo);
        MUS_LOG1( "mus: [MUSAO]  Call index = %d",i)
        MUS_LOG_TDESC("mus: [MUSAO]  Call Name",callInfo.iCallName);
        MUS_LOG1( "mus: [MUSAO]  Call Status = %d",callInfo.iStatus );        
        if(callInfo.iStatus == RCall::EStatusIdle)
            {
            for ( TInt j = 0; j < iCallMonitorArray.Count(); j++ )
                {
                if( iCallMonitorArray[j]->IsEqual( callInfo.iCallName ) )
                    {
                    iCallMonitorArray[0]->StopMonitor();   
                    delete iCallMonitorArray[j];
                    iCallMonitorArray.Remove(j);                     
                    break;
                    }
                }
            }                   
        MUS_LOG( "mus: [MUSAO]  -----------------------------------\n" ) 
        }        
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceEventMonitor::CheckCallStatus")
    }

// End of file


