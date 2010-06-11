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
#include "musengtestdefs.h"
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
    }

// -----------------------------------------------------------------------------
// From MMusEngSessionDurationTimerObserver
// -----------------------------------------------------------------------------
//
void CMusEngObserverStub::UpdateTimerEvent()
    {
    iUpdateTimerEventCalled = ETrue;
    }

// -----------------------------------------------------------------------------
// From MMusEngAudioRoutingObserver
// -----------------------------------------------------------------------------
//	
void CMusEngObserverStub::AudioRoutingChanged()
    {
    iAudioRoutingChangedCalled = ETrue;
    }

// -----------------------------------------------------------------------------
// From MMusSipProfileUser
// -----------------------------------------------------------------------------
//   
TBool CMusEngObserverStub::IsRoamingBetweenAPsAllowed()
    {
    return iRoamingBetweenAPsAllowed;
    }
    
// -----------------------------------------------------------------------------
// From MMusSipProfileUser
// -----------------------------------------------------------------------------
//   
void CMusEngObserverStub::ProfileRegistered()
    {
    iProfileRegisteredCalled = ETrue;
    }

// ----- HELPERS ---------------------------------------------------------------


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
TBool CMusEngObserverStub::IsReseted()
    {
    return ( iUpdateTimerEventCalled == EFalse &&
             iAudioRoutingChangedCalled == EFalse &&
             iRoamingBetweenAPsAllowed == EFalse &&
             iProfileRegisteredCalled == EFalse );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//	
void CMusEngObserverStub::Reset()
    {
    iUpdateTimerEventCalled = EFalse;
    iAudioRoutingChangedCalled = EFalse;
    iRoamingBetweenAPsAllowed = EFalse;
    iProfileRegisteredCalled = EFalse;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
TMusEngDisplayHandlerStub::TMusEngDisplayHandlerStub()
    {
    Reset();
    }

void TMusEngDisplayHandlerStub::Reset()
    {
    iRect = TRect( 0, 0, 0, 0 );
    iSecondaryRect = TRect( 0, 0, 0, 0 );
    iIsEnabled = EFalse;
    iOrientation = EPortrait;
    iIsActive = ETrue;
    }

TRect TMusEngDisplayHandlerStub::Rect() const
    {
    return iRect;
    }

void TMusEngDisplayHandlerStub::SetRectL( const TRect& aRect )
    {
    iRect = aRect;
    }
            
void TMusEngDisplayHandlerStub::SetSecondaryRectL( const TRect& aSecondaryRect )
    {
    iSecondaryRect = aSecondaryRect;
    }
            
TRect TMusEngDisplayHandlerStub::SecondaryRect() const
    {
    return iSecondaryRect;
    }
            
void TMusEngDisplayHandlerStub::EnableDisplayL( TBool aEnable )
    {
    iIsEnabled = aEnable;
    }
            
TBool TMusEngDisplayHandlerStub::IsDisplayEnabled()
    {
    return iIsEnabled;
    }
            
MMusEngDisplayHandler::TDisplayOrientation TMusEngDisplayHandlerStub::OrientationL()
    {
    return iOrientation;
    }
            
void TMusEngDisplayHandlerStub::SetOrientationL( TDisplayOrientation aOrientation )
    {
    iOrientation = aOrientation;
    }
            
TBool TMusEngDisplayHandlerStub::IsDisplayActive()
    {
    return iIsActive;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//        
CLcSessionObserverStub::CLcSessionObserverStub()
    {
    } 

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CLcSessionObserverStub::~CLcSessionObserverStub()
    {
    }
        
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//     
void CLcSessionObserverStub::StateChanged( MLcSession& aSession )
    {
    iCalledFunction = ESessionStateChanged;
    iCurrentSession = &aSession;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CLcSessionObserverStub::StateChanged( MLcVideoPlayer& aPlayer )
    {
    iCalledFunction = EPlayerStateChanged;
    iCurrentPlayer = &aPlayer;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CLcSessionObserverStub::Updated( MLcSession& aSession )
    {
    iCalledFunction = ESessionUpdated;
    iCurrentSession = &aSession;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CLcSessionObserverStub::Updated( MLcVideoPlayer& aPlayer )
    {
    iCalledFunction = EPlayerUpdated;
    iCurrentPlayer = &aPlayer;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CLcSessionObserverStub::Failed( MLcSession& aSession, TInt aError )
    {
    iCalledFunction = ESessionFailed;
    iCurrentSession = &aSession;
    iError = aError;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CLcSessionObserverStub::Failed( MLcVideoPlayer& aPlayer, TInt aError )
    {
    iCalledFunction = EPlayerFailed;
    iCurrentPlayer = &aPlayer;
    iError = aError;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CLcSessionObserverStub::SessionTimeChanged(
    MLcSession& aSession,
    const TTimeIntervalSeconds& aSeconds )
    {
    iCalledFunction = ESessionTimeChanged;
    iCurrentSession = &aSession;
    iSessionTime = aSeconds.Int();
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CLcSessionObserverStub::Reset()
    {
    iCalledFunction = EUnknown;
    iCurrentSession = NULL;
    iCurrentPlayer = NULL;
    iSessionTime = 0;
    iError = KErrNone;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CLcSessionObserverStub::IsReseted()
    {
    return ( iCalledFunction == EUnknown && 
             iCurrentSession == NULL &&
             iCurrentPlayer == NULL &&
             iSessionTime == 0 &&
             iError == KErrNone );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CLcUiProviderStub::CLcUiProviderStub()
    {
    
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CLcUiProviderStub::~CLcUiProviderStub()
    {
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CLcUiProviderStub::SelectRecipient( 
    MDesCArray& aRecipientAddresses, 
    TDes& aSelectedRecipientAddress )
    {
    iCalledFunction = ESelectRecipient;
    if ( aRecipientAddresses.MdcaCount() > 0 )
        {
        aSelectedRecipientAddress = aRecipientAddresses.MdcaPoint( 0 );
        }
    return iSimulatedReturnValue;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CLcUiProviderStub::InputRecipient( TDes& aRecipientAddress )
    {
    iCalledFunction = EInputRecipient;
    aRecipientAddress = iRecipient;
    return iSimulatedReturnValue;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CLcUiProviderStub::HandleForegroundStatus( TBool aForeground )
    {
    iCalledFunction = EHandleForegroundStatus;
    iForeground = aForeground;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CLcUiProviderStub::BlockUi( TBool /*aBlocked*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CLcUiProviderStub::Reset()
    {
    iCalledFunction = EUnknown;
    iRecipient.Set( KNullDesC );
    iSimulatedReturnValue = EFalse;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TLcWindowStub::TLcWindowStub() :
    iEnabled( EFalse ),
    iRect(),
    iOrientation( MLcWindow::EPortrait )
    {
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void TLcWindowStub::EnableLcWindowL( TBool aEnable )
    {
    iEnabled = aEnable;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool TLcWindowStub::IsLcWindowEnabled()
    {
    return iEnabled;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void TLcWindowStub::SetLcWindowRectL( TRect aRect )
    {
    iRect = aRect;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TRect TLcWindowStub::LcWindowRect()
    {
    delete HBufC::NewL( 1 );
    return iRect;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void TLcWindowStub::SetLcWindowOrientationL( TLcWindowOrientation aOrientation )
    {
    delete HBufC::NewL( 1 );
    iOrientation = aOrientation;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void TLcWindowStub::Reset()
    {
    iEnabled = EFalse;
    iOrientation = MLcWindow::EPortrait;
    iRect.SetSize( TSize() );
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
MLcWindow::TLcWindowOrientation TLcWindowStub::LcWindowOrientationL()
    {
    delete HBufC::NewL( 1 );
    return iOrientation;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TLcAudioControlStub::TLcAudioControlStub()
    {
    Reset();
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool TLcAudioControlStub::IsLcAudioMutedL()
    {
    delete HBufC::NewL( 1 );
    return iAudioMuted;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void TLcAudioControlStub::MuteLcAudioL( TBool aMute )
    {
    delete HBufC::NewL( 1 );
    iAudioMuted = aMute;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool TLcAudioControlStub::IsLcMicMutedL()
    {
    delete HBufC::NewL( 1 );
    return iMicMuted;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void TLcAudioControlStub::MuteLcMicL( TBool aMute )
    {
    delete HBufC::NewL( 1 );
    iMicMuted = aMute;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool TLcAudioControlStub::IsEnablingLcLoudspeakerAllowed()
    {
    return iEnablingLoudspeakerAllowed;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void TLcAudioControlStub::EnableLcLoudspeakerL( TBool aEnabled )
    {
    delete HBufC::NewL( 1 );
    iLoudspeakerEnabled = aEnabled;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool TLcAudioControlStub::IsLcLoudspeakerEnabled()
    {
    return iLoudspeakerEnabled;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt TLcAudioControlStub::LcVolumeL()
    {
    delete HBufC::NewL( 1 );
    return iVolume;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void TLcAudioControlStub::SetLcVolumeL( TInt aValue )
    {
    delete HBufC::NewL( 1 );
    iVolume = aValue;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void TLcAudioControlStub::IncreaseLcVolumeL()
    {
    delete HBufC::NewL( 1 );
    iVolume++;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void TLcAudioControlStub::DecreaseLcVolumeL()
    {
    delete HBufC::NewL( 1 );
    iVolume--;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void TLcAudioControlStub::Reset()
    {
    iAudioMuted = EFalse;
    iMicMuted = EFalse;
    iEnablingLoudspeakerAllowed = EFalse;
    iLoudspeakerEnabled = ETrue;
    iVolume = 0;
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

void REComSession::DestroyedImplementation(TUid /*aDtorIDKey*/)
    {
    }

