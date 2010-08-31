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

#include "musengsessiondurationtimerobserver.h"
#include "musengaudioroutingobserver.h"
#include "musengdisplayhandler.h"
#include "mussipprofileuser.h"
#include "muspropertyobserver.h"

#include <lcsessionobserver.h>
#include <lcuiprovider.h>
#include <lcwindow.h>
#include <lcaudiocontrol.h>
#include <e32base.h>
#include <ecom.h>

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
                            public MMusEngAudioRoutingObserver,
                            public MMusSipProfileUser
    {
    public: // Constructors and destructor

        CMusEngObserverStub();
        ~CMusEngObserverStub();

    public: // From MMusEngSessionDurationTimerObserver
    
        void UpdateTimerEvent();        
        
    public: // From MMusEngAudioRoutingObserver
        
        void AudioRoutingChanged();
        
    public: // From MMusSipProfileUser    
            
        TBool IsRoamingBetweenAPsAllowed();
        void ProfileRegistered();
        
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
        TBool iAudioRoutingChangedCalled;       
        TBool iRoamingBetweenAPsAllowed;
        TBool iProfileRegisteredCalled;
    };

class TMusEngDisplayHandlerStub : public MMusEngDisplayHandler
    {
public:
    
    TMusEngDisplayHandlerStub();
    
    void Reset();
    
public: // From MMusEngDisplayHandler
    
    TRect Rect() const;

    void SetRectL( const TRect& aRect );
            
    void SetSecondaryRectL( const TRect& aSecondaryRect );
            
    TRect SecondaryRect() const;
            
    void EnableDisplayL( TBool aEnable );
            
    TBool IsDisplayEnabled();
            
    TDisplayOrientation OrientationL();
            
    void SetOrientationL( TDisplayOrientation aOrientation );
            
    TBool IsDisplayActive();

public:
    
    TRect iRect;
    TRect iSecondaryRect;
    TBool iIsEnabled;
    TDisplayOrientation iOrientation;
    TBool iIsActive;
    };

class CLcSessionObserverStub : public CBase, public MLcSessionObserver
    {
    public: // Constructors and destructor

        CLcSessionObserverStub();
        ~CLcSessionObserverStub();
        
    public: // From MLcSessionObserver
        
        void StateChanged( MLcSession& aSession );
    
        void StateChanged( MLcVideoPlayer& aPlayer );
    
        void Updated( MLcSession& aSession );

        void Updated( MLcVideoPlayer& aPlayer );
    
        void Failed( 
            MLcSession& aSession,
            TInt aError );
    
        void Failed( 
            MLcVideoPlayer& aPlayer, 
            TInt aError );
    
        void SessionTimeChanged( 
            MLcSession& aSession,
            const TTimeIntervalSeconds& aSeconds );

    public: // New functions
        
        void Reset();    
        TBool IsReseted();
        
    public: // Data
        
        enum TCalledFunction
            {
            EUnknown,
            ESessionStateChanged,
            EPlayerStateChanged,
            ESessionUpdated,
            EPlayerUpdated,
            ESessionFailed,
            EPlayerFailed,
            ESessionTimeChanged
            };  
        
        TInt iCalledFunction;
        MLcSession* iCurrentSession;
        MLcVideoPlayer* iCurrentPlayer;
        TInt iSessionTime;
        TInt iError;
    }; 


class CLcUiProviderStub : public CBase, public MLcUiProvider
    {
    public: // Constructors and destructor

        CLcUiProviderStub();
        ~CLcUiProviderStub();
        
    public: // From MLcUiProvider
        
        TBool SelectRecipient( 
            MDesCArray& aRecipientAddresses, 
            TDes& aSelectedRecipientAddress );        
        
        TBool InputRecipient( TDes& aRecipientAddress );
        
        void HandleForegroundStatus( TBool aForeground );
        
        void BlockUi( TBool aBlocked );
        
    public: // New functions
        
        void Reset();
        
    public: // Data
        
        enum TCalledFunction
            {
            EUnknown,
            ESelectRecipient,
            EInputRecipient,
            EHandleForegroundStatus
            };  
        
        TInt iForeground;
        TInt iCalledFunction;
        TPtrC iRecipient;
        TBool iSimulatedReturnValue;     
    };


class TLcWindowStub : public MLcWindow
    {
    public: // Constructor     
        TLcWindowStub();
    
    public: // From MLcWindow      
        void EnableLcWindowL( TBool aEnable );
        TBool IsLcWindowEnabled();    
        void SetLcWindowRectL( TRect aRect );
        TRect LcWindowRect();
        void SetLcWindowOrientationL( TLcWindowOrientation aOrientation );
        TLcWindowOrientation LcWindowOrientationL();
        
    public: // New functions
        
        void Reset();
        
    private: // Data
        TBool iEnabled;
        TRect iRect;
        TLcWindowOrientation iOrientation;
    };


class TLcAudioControlStub : public MLcAudioControl
    {
    public: // Constructor
        TLcAudioControlStub();    
    
    public:
        TBool IsLcAudioMutedL();
        void MuteLcAudioL( TBool aMute );       
        TBool IsLcMicMutedL();    
        void MuteLcMicL( TBool aMute );
        TBool IsEnablingLcLoudspeakerAllowed();   
        void EnableLcLoudspeakerL( TBool aEnabled );
        TBool IsLcLoudspeakerEnabled();
        TInt LcVolumeL();
        void SetLcVolumeL( TInt aValue );    
        void IncreaseLcVolumeL();
        void DecreaseLcVolumeL();
 
    public: // New functions
        
        void Reset();        
        
    private: // Data      
        TBool iAudioMuted;
        TBool iMicMuted;
        TBool iEnablingLoudspeakerAllowed;
        TBool iLoudspeakerEnabled;
        TInt iVolume;
    };

class TMusPropertyObserverStub : public MMusPropertyObserver
    {
    public:
    
        enum TMusPropertyObserverFunction
            {
            ENone, // default value
            EPropertyChanged,
            EHandlePropertyError
            };

    
        TMusPropertyObserverStub()
            {
            iCalledFunction = ENone;
            iKey = 0;
            iValue = 0;
            iReason = 0;
            }
    
        void PropertyChanged( const TUint aKey, const TInt aValue )
            {
            iCalledFunction = EPropertyChanged;
            iKey = aKey;
            iValue = aValue;
            }


        void HandlePropertyError( const TInt aReason )
            {
            iCalledFunction = EHandlePropertyError;
            iReason = aReason;
            }

    public: // data

        TMusPropertyObserverFunction iCalledFunction;
        TInt iKey;
        TInt iValue;
        TInt iReason;
    };


// Dummy implementations
inline void CActiveScheduler::Start()
    {
    }
    
inline void CActiveScheduler::Stop()
    {
    }


#endif // UT_MUSSTUBS_H

