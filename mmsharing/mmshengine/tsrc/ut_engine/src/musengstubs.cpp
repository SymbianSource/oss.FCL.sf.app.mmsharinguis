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



#include "musengstubs.h"
#include <sipprofilealrcontroller.h>


// ---- MEMBER FUNCTIONS OF CMusEngObserverStub --------------------------------


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusEngObserverStub::CMusEngObserverStub()
    {
    Reset();
    }
        

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CMusEngObserverStub::~CMusEngObserverStub()
    {
    delete iIncomingSessionOriginator;
    }


// -------- Functions from MMusEngSessionDurationTimerObserver -----------------


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::UpdateTimerEvent()
    {
    iUpdateTimerEventCalled = ETrue;
    }


// ----------- Functions from MusEngSessionObserver ----------------------------




// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
void CMusEngObserverStub::SessionEstablished()
    {
    iSessionEstablishedCalled = ETrue;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionTerminated()
    {
    iSessionTerminatedCalled = ETrue;
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionConnectionLost()
    {
    iSessionConnectionLostCalled = ETrue;
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionFailed()
    {
    iSessionFailedCalled = ETrue;
    }
    
            
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::StreamIdle()
    {
    iStreamIdleCalled = ETrue;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::StreamStreaming()
    {
    iStreamStreamingCalled = ETrue;
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionTimeChanged( 
                        const TTimeIntervalSeconds& /*aSeconds*/ )
    {
    iSessionTimeChangedCalled = ETrue;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::InactivityTimeout()
    {
    iInactivityTimeoutCalled = ETrue;
    }






// ---- FUNCTIONS OF MMusEngOutSessionObserver --------------------------------


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionRejected()
    {
    iSessionRejectedCalled = ETrue;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
void CMusEngObserverStub::SessionBadRequest()
    {
    iSessionBadRequestCalled = ETrue;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionUnauthorized()
    {
    iSessionUnauthorizedCalled = ETrue;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionPaymentRequired()
    {
    iSessionPaymentRequiredCalled = ETrue;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionRecipientNotFound()
    {
    iSessionRecipientNotFoundCalled = ETrue;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionProxyAuthenticationRequired()
    {
    iSessionProxyAuthenticationRequiredCalled = ETrue;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionRequestTimeOut()
    {
    iSessionRequestTimeOutCalled = ETrue;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionUnsupportedMediaType()
    {
    iSessionUnsupportedMediaTypeCalled = ETrue;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
void CMusEngObserverStub::SessionBusyHere()
    {
    iSessionBusyHereCalled = ETrue;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::SessionRequestCancelled()
    {
    iSessionRequestCancelledCalled = ETrue;
    }


void CMusEngObserverStub::SessionTemporarilyNotAvailable()
    {
    iSessionTemporarilyNotAvailable = ETrue;
    }



// ---- FUNCTIONS OF MMusEngLiveSessionObserver ----------------------------------



// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::DiskFull()
    {
    iDiskFullCalled = ETrue;
    }




// ---- FUNCTIONS OF MMusEngClipSessionObserver ----------------------------------


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::EndOfClip()
    {
    iEndOfClipCalled = ETrue;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::TranscodingNeeded(TBool aDueUnknowCapas)
    {
    iTranscodingNeededCalled = ETrue;
    iDueUnknowCapas = aDueUnknowCapas;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//        
void CMusEngObserverStub::TranscodingProgressed( TInt aPercentage )
    {
    iTranscodingProgressedPercentage = aPercentage;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
void CMusEngObserverStub::TranscodingCompletedInit()
    {
    iTranscodingCompletedInitCalled = ETrue;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
void CMusEngObserverStub::TranscodingCompletedFinalize()
    {
    iTranscodingCompletedFinalizeCalled = ETrue;
    }    
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
void CMusEngObserverStub::TranscodingFailed()
    {
    iTranscodingFailedCalled = ETrue;
    }
 

// ---- FUNCTIONS OF MMusEngInSessionObserver ----------------------------------


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
void CMusEngObserverStub::IncomingSessionPreNotification()
    {
    iIncomingSessionPreNotificationCalled = ETrue;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
void CMusEngObserverStub::IncomingSession( const TDesC& aOriginator,
                                           const TDesC& /*aOriginatorIdentity*/ )
    {
    iIncomingSessionCalled = ETrue;
    iIncomingSessionOriginator = aOriginator.Alloc();
    
    if ( !iIncomingSessionOriginator )
        {
        // TODO: Handle out of memory.
        }
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
void CMusEngObserverStub::StreamBuffering()
    {
    iStreamBufferingCalled = ETrue;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
void CMusEngObserverStub::AudioRoutingChanged( TBool aShowNote )
    {
    iAudioRoutingChangedCalled = ETrue;
    iShowNote = aShowNote;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
TBool CMusEngObserverStub::AudioRouteChangeAllowed() const
    {
    return iAudioRouteChangeAllowed;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//    
void CMusEngObserverStub::VolumeChanged( TInt aVolume, TBool /*aAudioRouteChanged*/ )
    {
    iVolume = aVolume;
    }

// ----- HELPERS ---------------------------------------------------------------


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
TBool CMusEngObserverStub::IsReseted()
    {
    return ( !iUpdateTimerEventCalled &&
             !iSessionEstablishedCalled &&
             !iSessionTerminatedCalled &&
             !iSessionConnectionLostCalled &&
             !iSessionFailedCalled &&
             !iStreamIdleCalled &&
             !iStreamStreamingCalled &&
             !iSessionTimeChangedCalled &&
             !iInactivityTimeoutCalled &&
             !iSessionRejectedCalled &&
             !iSessionBadRequestCalled &&
        	 !iSessionUnauthorizedCalled &&
        	 !iSessionPaymentRequiredCalled &&
        	 !iSessionRecipientNotFoundCalled &&
        	 !iSessionProxyAuthenticationRequiredCalled &&
        	 !iSessionRequestTimeOutCalled &&
        	 !iSessionUnsupportedMediaTypeCalled &&
        	 !iSessionBusyHereCalled  &&
        	 !iSessionRequestCancelledCalled &&
        	 !iDiskFullCalled &&
        	 !iEndOfClipCalled &&
        	 !iTranscodingNeededCalled &&
        	 !iTranscodingCompletedInitCalled &&
        	 !iTranscodingCompletedFinalizeCalled &&
        	 !iTranscodingFailedCalled &&
        	 !iIncomingSessionPreNotificationCalled &&
             !iIncomingSessionCalled &&
             !iStreamBufferingCalled &&
             !iAudioRoutingChangedCalled &&
             iTranscodingProgressedPercentage == 0 &&
             !iSessionTemporarilyNotAvailable &&
             !iIncomingSessionOriginator &&
             !iShowNote &&
             iAudioRouteChangeAllowed &&
             !iVolume );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
void CMusEngObserverStub::Reset()
    {
    iUpdateTimerEventCalled = EFalse;
    iSessionEstablishedCalled = EFalse;
    iSessionTerminatedCalled = EFalse;
    iSessionConnectionLostCalled = EFalse;
    iSessionFailedCalled = EFalse;
    iStreamIdleCalled = EFalse;
    iStreamStreamingCalled = EFalse;
    iSessionTimeChangedCalled = EFalse;
    iInactivityTimeoutCalled = EFalse;
    iSessionRejectedCalled = EFalse;
	iSessionBadRequestCalled = EFalse;
	iSessionUnauthorizedCalled = EFalse;
	iSessionPaymentRequiredCalled = EFalse;
	iSessionRecipientNotFoundCalled = EFalse;
	iSessionProxyAuthenticationRequiredCalled = EFalse;
	iSessionRequestTimeOutCalled = EFalse;
	iSessionUnsupportedMediaTypeCalled = EFalse;
	iSessionBusyHereCalled = EFalse;
	iSessionRequestCancelledCalled = EFalse;
	iDiskFullCalled = EFalse;
	iEndOfClipCalled = EFalse;
	iTranscodingNeededCalled = EFalse;
    iTranscodingCompletedInitCalled = EFalse;
    iTranscodingCompletedFinalizeCalled = EFalse;
	iTranscodingFailedCalled = EFalse;
	iIncomingSessionPreNotificationCalled = EFalse;
    iIncomingSessionCalled = EFalse;
    iStreamBufferingCalled = EFalse;
    iAudioRoutingChangedCalled = EFalse;
    iTranscodingProgressedPercentage = 0;
    delete iIncomingSessionOriginator;
    iSessionTemporarilyNotAvailable = EFalse;   
    iIncomingSessionOriginator = NULL;
    iShowNote = EFalse;
    iAudioRouteChangeAllowed = ETrue;
    iVolume = 0;
    iDueUnknowCapas = EFalse;
    }


// CSipProfileAlrController STUB
CSipProfileAlrController* CSipProfileAlrController::NewL( 
    CSIPProfileRegistryBase& aRegistry, 
    MSipProfileAlrObserver& /*aObserver*/ )
    {
    return new( ELeave )CSipProfileAlrController( aRegistry );
    }

CSipProfileAlrController::~CSipProfileAlrController()
    {
    }

void CSipProfileAlrController::AllowMigrationL(
    TUint32 /*aProfileId*/,
    TUint32 aIapId )
    {
    delete HBufC::NewL( 1 );
    iAllowMigrationCalled = ETrue;
    iLastUsedIap = aIapId;
    }

void CSipProfileAlrController::DisallowMigrationL(
    TUint32 /*aProfileId*/,
    TUint32 aIapId )
    {
    delete HBufC::NewL( 1 );
    iDisallowMigrationCalled = ETrue;
    iLastUsedIap = aIapId;
    }

void CSipProfileAlrController::RefreshIapAvailabilityL( 
    TUint32 /*aProfileId*/ )
    {
    delete HBufC::NewL( 1 );
    }

CSipProfileAlrController::CSipProfileAlrController( 
    CSIPProfileRegistryBase& aRegistry )
    : iRegistry( aRegistry )
    {
    }
