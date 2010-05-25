/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef UT_MUSSTUBS_H
#define UT_MUSSTUBS_H


#include "musengsessionobserver.h"
#include "musengoutsessionobserver.h"
#include "musenglivesessionobserver.h"
#include "musengclipsessionobserver.h"
#include "musengreceivesessionobserver.h"
#include "musengsessiondurationtimerobserver.h"
#include "musengaudioroutingobserver.h"
#include "musengvolumechangeobserver.h"

#include <e32base.h>

// DUMMY CLASSES

class CSIPTransactionBase
    {
    };
    
class CSIPServerTransaction
    {
    };

class CSIPDialog
    {
    };

class CSIPClientTransaction
    {
    };

class CSIPDialogAssocBase
    {
    };

class CSIPRegistrationBinding
    {
    };

class CSIPRefresh
    {
    };
 
/**
* Class to implement all the interfaces provided by MusEngine
*/        
class CMusEngObserverStub : public CBase, 
                            public MMusEngSessionDurationTimerObserver,
                            public MMusEngSessionObserver,
                            public MMusEngOutSessionObserver,
                            public MMusEngLiveSessionObserver,
                            public MMusEngClipSessionObserver,
                            public MMusEngReceiveSessionObserver,
                            public MMusEngAudioRoutingObserver,
                            public MMusEngVolumeChangeObserver
    {
    
    public:

        CMusEngObserverStub();
            
        ~CMusEngObserverStub();
        
        // From MMusEngSessionDurationTimerObserver
        
        void UpdateTimerEvent();
        
        // From MMusEngSessionObserver
        
        void SessionEstablished();
        void SessionTerminated(); 
        void SessionConnectionLost();
        void SessionFailed();
        void StreamIdle();
        void StreamStreaming();
        void SessionTimeChanged( const TTimeIntervalSeconds& aSeconds );
        void InactivityTimeout();


        // From MMusEngOutSessionObserver

        void SessionRejected();
    	void SessionBadRequest(); // 400 
    	void SessionUnauthorized(); // 401 
    	void SessionPaymentRequired(); // 402
    	void SessionRecipientNotFound(); // 404 
    	void SessionProxyAuthenticationRequired(); // 407
    	void SessionRequestTimeOut(); // 408
    	void SessionUnsupportedMediaType(); // 415
    	void SessionBusyHere(); // 486
    	void SessionRequestCancelled(); // 487 
    	void SessionTemporarilyNotAvailable(); // 480

        // From MMusEngLiveSessionObserver
        
        void DiskFull();
        
        // From MMusEngClipSessionObserver    
        
        void EndOfClip(); 
        
        void TranscodingNeeded(TBool aDueUnknowCapas);
        
        void TranscodingProgressed( TInt aPercentage );     

        void TranscodingCompletedInit();

        void TranscodingCompletedFinalize();

        void TranscodingFailed();
        
        void RewindFromEndL();


        // From MMusEngReceiveSessionObserver    
        
        void IncomingSessionPreNotification();

        void IncomingSession( const TDesC& aOriginator, 
                              const TDesC& aOriginatorIdentity );
        
        void StreamBuffering();
    
    
        // From MMusEngAudioRoutingObserver
        
        void AudioRoutingChanged( TBool aShowNote );
        
        TBool AudioRouteChangeAllowed() const;
        
        // From MMusEngVolumeChangeObserver
        void VolumeChanged( TInt aVolume, TBool aAudioRouteChanged );
        
    public: // Helper
    
        /**
        * ETrue if all boolean member variables are set to EFalse and all the
        * pointers are NULL
        */
        TBool IsReseted();
        
        /**
        * Resets all the boolean member variable values to EFalse and deletes
        * and nullifies all the pointer variables.
        */
        void Reset();

    public: // Data  
        
        TBool iUpdateTimerEventCalled;
        
        TBool iSessionEstablishedCalled;
        TBool iSessionTerminatedCalled;
        TBool iSessionConnectionLostCalled;
        TBool iSessionFailedCalled;
        TBool iStreamIdleCalled;
        TBool iStreamStreamingCalled;
        TBool iSessionTimeChangedCalled;
        TBool iInactivityTimeoutCalled;
        
        TBool iSessionRejectedCalled;
    	TBool iSessionBadRequestCalled;
    	TBool iSessionUnauthorizedCalled;
    	TBool iSessionPaymentRequiredCalled;
    	TBool iSessionRecipientNotFoundCalled;
    	TBool iSessionProxyAuthenticationRequiredCalled;
    	TBool iSessionRequestTimeOutCalled;
    	TBool iSessionUnsupportedMediaTypeCalled;
    	TBool iSessionBusyHereCalled;
    	TBool iSessionRequestCancelledCalled;
    	TBool iDiskFullCalled;
    	
    	TBool iEndOfClipCalled;
    	TBool iRewindFromEnd;
    	TBool iTranscodingNeededCalled;
    	TBool iTranscodingCompletedInitCalled;
    	TBool iTranscodingCompletedFinalizeCalled;
    	TBool iTranscodingFailedCalled;
    	
    	TBool iIncomingSessionPreNotificationCalled;
        TBool iIncomingSessionCalled;
        TBool iStreamBufferingCalled;
        
        TBool iAudioRoutingChangedCalled;
        TBool iShowNote;
        TBool iAudioRouteChangeAllowed;
        
        TInt iTranscodingProgressedPercentage;
        TBool iSessionTemporarilyNotAvailable;
        HBufC* iIncomingSessionOriginator;
        
        TInt iVolume;
        TBool iDueUnknowCapas;
    };


// Dummy implementations
inline void CActiveScheduler::Start()
    {
    }
    
inline void CActiveScheduler::Stop()
    {
    }


#endif // UT_MUSSTUBS_H

