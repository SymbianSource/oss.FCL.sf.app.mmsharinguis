/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32base.h>
#include <e32property.h>
#include <MceTransactionDataContainer.h>
#include <MceSession.h>
#include <MceInSession.h>
#include <MceOutSession.h>
#include "mceinsessionobserver.h"
#include "mcesessionobserver.h"
#include "mussessionsimulator.h"
#include "muslogger.h"
#include <musmanagercommon.h>
#include "mussessionproperties.h"



TInt KDuration = 60*1;

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CMceManager::NewL
// -----------------------------------------------------------------------------
//
CMusTestSessionSimulator* CMusTestSessionSimulator::NewL(  CMceManager& aManager)
    {
    
    CMusTestSessionSimulator* self = 
        new (ELeave) CMusTestSessionSimulator( aManager );
    CleanupStack::PushL( self );
    self->ConstructL( );
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// CMceManager::~CMceManager
// -----------------------------------------------------------------------------
//

CMusTestSessionSimulator::CMusTestSessionSimulator( CMceManager& aManager )
    :iManager(aManager)
    {
    
    }

// -----------------------------------------------------------------------------
// CMceManager::~CMceManager
// -----------------------------------------------------------------------------
//
void CMusTestSessionSimulator::ConstructL(  )
    {
    iTimer = CMusTestTimer::NewL(CActive::EPriorityStandard, *this);
    SimulateReceiveSession();
    StartL();
    }

// -----------------------------------------------------------------------------
// Callback implementation when the timer activity happens in the CMusTestTimer class
// -----------------------------------------------------------------------------
//
void CMusTestSessionSimulator::StartL()
    {
    TTimeIntervalMicroSeconds32 someInterVal(KDuration); //you can call After/At/Inactivity depending on what you want to do
    iTimer->After(someInterVal);
    }

// -----------------------------------------------------------------------------
// Callback implementation when the timer activity happens in the CMusTestTimer class
// -----------------------------------------------------------------------------
//
TBool CMusTestSessionSimulator::IsRunning()
    {
    return iTimer->IsActive();
    }

// -----------------------------------------------------------------------------
// Callback implementation when the timer activity happens in the CMusTestTimer class
// -----------------------------------------------------------------------------
//
void CMusTestSessionSimulator::Stop(  )
    {
    iTimer->Cancel();
    }

// -----------------------------------------------------------------------------
// Callback implementation when the timer activity happens in the CMusTestTimer class
// -----------------------------------------------------------------------------
//
void CMusTestSessionSimulator::TimerExpired(TAny* aTimer,TInt aError)
    {
    if(aError == KErrNone)
      {
      // Timer successfully completed, handle it
      CMusTestTimer* timer = (CMusTestTimer*)aTimer;
      MUS_LOG("CMusTestSessionSimulator TimerExipired Change the state")
      StateChangeL();
      if( (iManager.iSessions[0]->iState == CMceSession::EEstablished )||
          ( iManager.iSessions[0]->iState == CMceSession::EIncoming )
        )
          {
          timer->Cancel();          
          }
      else
          {
          TTimeIntervalSeconds seconds(KDuration);
          timer->Inactivity(seconds); //Notify inactivity after 10 seconds
          }
      }
    }

// -----------------------------------------------------------------------------
// Simulates Receive Session
// -----------------------------------------------------------------------------
//
void CMusTestSessionSimulator::SimulateReceiveSession()
    {
    MUS_LOG("CMusTestSessionSimulator::SimulateReceiveSession ->")
    TInt usecase;
    TInt err = RProperty::Get( NMusSessionApi::KCategoryUid, NMusSessionApi::KUseCase, usecase);  
    if( err == KErrNone )
        {        
        if( usecase == MultimediaSharing::EMusReceive || usecase == MultimediaSharing::EMusReceiveTwoWayVideo )
            {
            MUS_LOG("CMusTestSessionSimulator::SimulateReceiveSession receive session ")
            CMceInSession* inSession = CMceInSession::NewL();       
            inSession->iState = CMceSession::EIncoming;
            iManager.iSessions.AppendL( inSession );
            ibIncoming = ETrue;
            }
        else
            {
            ibIncoming = EFalse;
            }
        }
    else
        {
        MUS_LOG1("CMusTestSessionSimulator::SimulateReceiveSession err = %d ",err)
        }
    MUS_LOG("CMusTestSessionSimulator::SimulateReceiveSession <-")
    }
// -----------------------------------------------------------------------------
// CMceManager::~CMceManager
// -----------------------------------------------------------------------------
//
void CMusTestSessionSimulator::StateChangeL()
    {
    MUS_LOG("CMusTestSessionSimulator::StateChangeL ->")
    TMceTransactionDataContainer* container = new TMceTransactionDataContainer();
    HBufC8* resonPharase = _L8("OK").AllocL(); ;
    TInt statusCode = 200;
    switch( iManager.iSessions[0]->iState )
        {        
        case CMceSession::EIdle : // offering
            statusCode = 180;
            delete resonPharase;
            resonPharase = _L8("Ringing").AllocL();    
            iManager.iSessions[0]->iState = CMceSession::EOffering;
            MUS_LOG("CMusTestSessionSimulator Session state : Offering 180")
            break;
        case CMceSession::EOffering : // estabilished
        case CMceSession::EAnswering : // estabilished
            delete resonPharase;
            resonPharase = _L8("OK").AllocL();    
            iManager.iSessions[0]->iState = CMceSession::EEstablished;
            MUS_LOG("CMusTestSessionSimulator Session state : Answering")
            break;
        case CMceSession::EEstablished : // estabilished
            delete resonPharase;
            resonPharase = _L8("OK").AllocL();
            MUS_LOG("CMusTestSessionSimulator Session state : Estabilished")
            break;
        case CMceSession::ETerminating : // estabilished            
            delete resonPharase;
            resonPharase = _L8("OK").AllocL();    
            iManager.iSessions[0]->iState = CMceSession::ETerminated;
            MUS_LOG("CMusTestSessionSimulator Session state : Terminated")
            break;
        case CMceSession::EIncoming : // estabilished            
            // handled down
            break;
        case CMceSession::EProceeding : // estabilished 
            iManager.iSessions[0]->iState = CMceSession::EAnswering;
            MUS_LOG("CMusTestSessionSimulator Session state : Proceeding");
            break;
        case CMceSession::EReserving : // estabilished     
            iManager.iSessions[0]->iState = CMceSession::EProceeding;       
            MUS_LOG("CMusTestSessionSimulator Session state : Reserving")
        default:
            break;
        }
    if( iManager.iSessions[0]->iState == CMceSession::EIncoming )
        {       
        MUS_LOG("CMusTestSessionSimulator Session state : Incoming") 
        if( iManager.iSessionObserver && container ) 
            {
            CMceInSession* session = ( CMceInSession* ) iManager.iSessions[0];
            if( session )
                {
                MUS_LOG("CMusTestSessionSimulator Calling InComingSession")            
                iManager.iInSessionObserver->IncomingSession( session , container );     
                iManager.iSessions[0]->iState = CMceSession::EReserving; 
                }
            }
        else
            {
            MUS_LOG("CMusTestSessionSimulator insessionobserver empty ")
            }
        }
    else
        {
        MUS_LOG("CMusTestSessionSimulator Session state : Inform Session state") 
        container->SetStatusCode(statusCode);
        container->SetReasonPhrase( resonPharase ); 
        if( iManager.iSessionObserver && container ) 
            {
            MUS_LOG("CMusTestSessionSimulator Calling SessionStateChanged")
            if( iManager.iSessions.Count() > 0 )
                {
                CMceSession* session = iManager.iSessions[0];
                if( session )
                    {
                    iManager.iSessionObserver->SessionStateChanged( *session ,container);
                    }
                else
                    {
                    MUS_LOG("CMusTestSessionSimulator session empty ")
                    }
                }
            else
                {
                MUS_LOG("CMusTestSessionSimulator No Session found ")            
                }
            }
         else
            {
            MUS_LOG("CMusTestSessionSimulator Invalid pointer")
            }
        }
    delete container;
    MUS_LOG("CMusTestSessionSimulator::StateChangeL <-")
    }

// -----------------------------------------------------------------------------
// CMceManager::~CMceManager
// -----------------------------------------------------------------------------
//
EXPORT_C CMusTestSessionSimulator::~CMusTestSessionSimulator()
    {
    Stop();
    delete iTimer;
    }

//--------------------------------------------------------------------------------    
// Timer implementation
//--------------------------------------------------------------------------------
//
CMusTestTimer::CMusTestTimer(const TInt aPriority,MTimerNotify& aNotify)
:CActive(aPriority),iNotify(aNotify)
{
}
 
CMusTestTimer::~CMusTestTimer()
{   
    Cancel();
    iTimer.Close();
}
 
CMusTestTimer* CMusTestTimer::NewL(const TInt aPriority,MTimerNotify& aNotify)
{
    CMusTestTimer* me = new (ELeave) CMusTestTimer(aPriority,aNotify);
    CleanupStack::PushL(me);
    me->ConstructL();
    CleanupStack::Pop();
    return me;
}
 
void CMusTestTimer::ConstructL(void)
{
    CActiveScheduler::Add(this);
    iTimer.CreateLocal();
}
 
void CMusTestTimer::After(TTimeIntervalMicroSeconds32 aInterval)
{
    Cancel();
    iTimer.After(iStatus,aInterval);
    SetActive();
}
 
void CMusTestTimer::At(const TTime& aTime)
{
    Cancel();
    iTimer.At(iStatus,aTime);
    SetActive();
}
 
void CMusTestTimer::Inactivity(TTimeIntervalSeconds aSeconds)
{
    Cancel();
    iTimer.Inactivity(iStatus,aSeconds);
    SetActive();
}
 
void CMusTestTimer::DoCancel()
{
    iTimer.Cancel();
}
 
void CMusTestTimer::RunL()
{
    iNotify.TimerExpired(this,iStatus.Int());
}
