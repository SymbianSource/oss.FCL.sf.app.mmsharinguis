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
#include "muscallconferencemonitor.h"
#include "muscallconferenceeventmonitor.h"

#include <etelmm.h>
#include <e32svr.h>
#include <mmtsy_names.h>
#include <e32property.h>


// -----------------------------------------------------------------------------
// Symbian two-phase constructor.
// -----------------------------------------------------------------------------
//
CMusCallConferenceMonitor* CMusCallConferenceMonitor::NewL( RMobilePhone& aPhone,
                              RMobileLine& aLine,
                              RPointerArray<CMusCallMonitor>& aCallMonitorArray )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceMonitor::NewL" )
    CMusCallConferenceMonitor* self = new (ELeave) 
                              CMusCallConferenceMonitor(aLine,aCallMonitorArray);
    CleanupStack::PushL( self );
    self->ConstructL( aPhone );
    CleanupStack::Pop( self );
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceMonitor::NewL" )
    return self;
    }


// -----------------------------------------------------------------------------
// C++ destructor.
// -----------------------------------------------------------------------------
//
CMusCallConferenceMonitor::~CMusCallConferenceMonitor()
    {
    MUS_LOG( "mus: [MUSAO]  -> \
                        CMusCallConferenceMonitor::~CMusCallConferenceMonitor" )        
    Cancel();   
    iConfCall.Close();
    delete iConfCallEvent;
    MUS_LOG( "mus: [MUSAO]  <- \
                        CMusCallConferenceMonitor::~CMusCallConferenceMonitor" )
    }

// -----------------------------------------------------------------------------
// Symbian second-phase constructor.
// -----------------------------------------------------------------------------
//
void CMusCallConferenceMonitor::ConstructL( RMobilePhone& aPhone )
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceMonitor::ConstructL" )   
    CActiveScheduler::Add( this );            
    User::LeaveIfError(iConfCall.Open(aPhone));    
    iConfCall.NotifyConferenceStatusChange(iStatus,iConfStatus);   
    SetActive();    
    iConfCallEvent = CMusCallConferenceEventMonitor::NewL(aPhone,
                                                          iLine,
                                                          iCallMonitorArray); 
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceMonitor::ConstructL" )
    }


// -----------------------------------------------------------------------------
// C++ constructor.
// -----------------------------------------------------------------------------
//
CMusCallConferenceMonitor::CMusCallConferenceMonitor(RMobileLine& aLine,
                            RPointerArray<CMusCallMonitor>& aCallMonitorArray) 
        : CActive( EPriorityNormal ),
          iLine(aLine),
          iCallMonitorArray(aCallMonitorArray)
          
        
    {    
    }


// --------------------------------------------------------------------------------
// CMusCallConferenceMonitor::RunL()
// Implemented for CActive.It will be called any NotifyMobileLineStatusChange
// Event happens.
// --------------------------------------------------------------------------------
//
void CMusCallConferenceMonitor::RunL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceMonitor::RunL" )    
    iConfCall.NotifyConferenceStatusChange(iStatus,iConfStatus); 
    SetActive();       
    MUS_LOG1( "mus: [MUSAO]  Conference->iCallStatus =  %d", iConfStatus ) 
    switch(iConfStatus)    
        {         
            case RMobileConferenceCall::EConferenceIdle:
                 MonitorCallL();                                	
                 break;
            case RMobileConferenceCall::EConferenceActive:	                 
                 SetStateL( NMusSessionInformationApi::EConferenceCall );
                 break;
            case RMobileConferenceCall::EConferenceHold:
                 SetStateL(NMusSessionInformationApi::ECallHold);	
                 break;
            default:
                 break;
        }
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceMonitor::RunL" )
    }


// --------------------------------------------------------------------------------
// CMusCallConferenceMonitor::DoCancel()
// Implemented for CActive.
// Will be called when Cancel() method is called.
// --------------------------------------------------------------------------------
//
void CMusCallConferenceMonitor::DoCancel()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceMonitor::DoCancel" )
    iConfCall.CancelAsyncRequest( EMobileConferenceCallNotifyConferenceStatusChange );
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceMonitor::DoCancel" )
    }

// --------------------------------------------------------------------------------
// CMusCallConferenceMonitor::RunError( TInt aError )
// Implemented for CActive.
// Will be called automatically by framework when RunL Leaves.
// --------------------------------------------------------------------------------
//

TInt CMusCallConferenceMonitor::RunError( TInt aError )
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusCallConferenceMonitor::RunError, aError %d", \
                                                                          aError )
    aError = KErrNone;
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceMonitor::RunError" )    
    return aError;
    }


// --------------------------------------------------------------------------------
// CMusCallConferenceMonitor::SetStateL()
// Sets the Call State to Property
// --------------------------------------------------------------------------------
//
void CMusCallConferenceMonitor::SetStateL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceMonitor::SetStateL")    
    User::LeaveIfError(iConfCall.GetConferenceStatus(iConfStatus));    
    MUS_LOG1( "mus: [MUSAO]  Conference->iCallStatus =  %d", iConfStatus ) 
    switch(iConfStatus)    
        {         
            case RMobileConferenceCall::EConferenceIdle:                 
                 break;
            case RMobileConferenceCall::EConferenceActive:	                 
                 SetStateL( NMusSessionInformationApi::EConferenceCall );
                 break;
            case RMobileConferenceCall::EConferenceHold:
                 SetStateL(NMusSessionInformationApi::ECallHold);	
                 break;
            default:
                 break;
        }
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceMonitor::SetStateL" )    
    }


// --------------------------------------------------------------------------------
// CMusCallConferenceMonitor::SetStateL( TInt aError )
// Sets the Call State to Property
// --------------------------------------------------------------------------------
//
void CMusCallConferenceMonitor::SetStateL(
                                      NMusSessionInformationApi::TMusCallEvent aVal)
    {
    MUS_LOG1( "mus: [MUSAO]  -> CMusCallConferenceMonitor::SetStateL, aVal %d",\
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
        }    
    MUS_LOG( "mus: [MUSAO]  <- CMusCallConferenceMonitor::SetStateL" )    
    }

// --------------------------------------------------------------------------------
// CMusCallConferenceMonitor::MonitorCallL() 
// If the call in conference is dropped that has to be removed.
// from call monitoring array.and start monitoring.
// --------------------------------------------------------------------------------
//
void CMusCallConferenceMonitor::MonitorCallL()
    {
    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceMonitor::MonitorCallL")
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
                    delete iCallMonitorArray[j];
                    iCallMonitorArray.Remove(j);                   
                    break;
                    }
                }
            }                   
        MUS_LOG( "mus: [MUSAO]  -----------------------------------\n" ) 
        }

    MUS_LOG( "mus: [MUSAO]  -> CMusCallConferenceMonitor::CheckCallStatus")
    }


// --------------------------------------------------------------------------------
// TMobileConferenceStatus GetCconfStatus()
// Get the Conference Call Status
// --------------------------------------------------------------------------------
//
RMobileConferenceCall::TMobileConferenceStatus CMusCallConferenceMonitor::GetConfStatus()
    {
    return iConfStatus;
    }



// End of file


