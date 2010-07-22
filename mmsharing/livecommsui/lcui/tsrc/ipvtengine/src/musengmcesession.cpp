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


// USER
#include "musengmcesession.h"
#include "lcsessionobserver.h"
#include "musenglogger.h"
#include "lcuiprovider.h"
#include "lcvideoplayer.h"
#include "musengremotevideoplayer.h"
#include "musenglivevideoplayer.h"
#include "mccscreen.h"

// SYSTEM
#include <audiopreference.h>
#include <e32property.h>

#include <cntitem.h>
#include <cntfield.h>
#include <cntdef.h>
#include <cntfldst.h>

#include "svptimer.h"

RDrawableWindow* testWindow = 0;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceSession::CMusEngMceSession()
    {
    iBackground = IsBackgroundStartup();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::ConstructL()" )

    iRemoteVideoPlayer = CMusEngRemoteVideoPlayer::NewL( *this, *this ); 
    iLiveVideoPlayer = CMusEngLiveVideoPlayer::NewL( *this, iCameraHandler, *this );      
    iLcSessionState = MLcSession::EUninitialized;

    iTimer = CSVPTimer::NewL( *this, 1 ); 
    iCameraHandler.SetSession( this );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::ConstructL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceSession::~CMusEngMceSession()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::~CMusEngMceSession()" )

    delete iRemoteAddress;
    delete iRemoteVideoPlayer; 
    delete iLiveVideoPlayer;    
    delete iScreen;
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::~CMusEngMceSession()" )
    }
	
// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
TRect CMusEngMceSession::Rect() const
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngMceSession::Rect()" )
    return iRect;
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetRectL( const TRect& aRect )
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngSession::SetRectL()" )
    if ( !iScreen ){
        iScreen = CMccScreen::NewL(0,TPoint(0,0), TSize(640,640), 0, 1, 0, EFalse, testWindow);
        }
    iRect = aRect;
    if ( iScreen )
        {
        iScreen->SetPrimaryRectL(iRect);
        }
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetSecondaryRectL( const TRect& aSecondaryRect )
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngMceSession::SetSecondaryRectL()" )
    if ( !iScreen ){
        iScreen = CMccScreen::NewL(0,TPoint(0,0), TSize(640,640), 0, 1, 0, EFalse, testWindow);
        }
    // Same rect is set by primary and secondary setters
    iSetLocalRect = aSecondaryRect;
    if ( iScreen )
        {
        iScreen->SetSecondaryRectL(iSetLocalRect);
        }
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
TRect CMusEngMceSession::SecondaryRect() const
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngMceSession::SecondaryRect()" )
    return iSetLocalRect;
    }

#if 0

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::SetSecondaryRectL( const TRect& aSecondaryRect )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::SetSecondaryRectL()" )
    
    iLocalRect = aSecondaryRect;

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::SetSecondaryRectL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TRect CMusEngTwoWaySession::SecondaryRect() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::SecondaryRect()" )
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::SecondaryRect()" )
    return iLocalRect;
    }

#endif

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
MMusEngDisplayHandler::TDisplayOrientation CMusEngMceSession::OrientationL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::RotationL()" )           
    return MMusEngDisplayHandler::EPortrait;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//        
void CMusEngMceSession::SetOrientationL( TDisplayOrientation aOrientation )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngMceSession::SetOrientationL() %d", 
              aOrientation )
              
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::SetOrientationL()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::EnableDisplayL( TBool aEnable )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngMceSession::EnableDisplay() %d", 
              aEnable )
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::EnableDisplay()")
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsDisplayEnabled()
    {
    TBool enabled( EFalse );
    return enabled;
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsDisplayActive()
    {
    return ( IsDisplayEnabled() );
    }

// -----------------------------------------------------------------------------
// From MLcSession 
// -----------------------------------------------------------------------------
//
MLcSession::TLcSessionState CMusEngMceSession::LcSessionState() const
    {
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::LcSessionState()" )

    TLcSessionState lcSessionState = MLcSession::EUninitialized;

/*
    if ( iSession )
        {
        switch( iSession->State() )
            {
            case CMceSession::EIdle: 
                {
                lcSessionState = MLcSession::EInitialized;
                break;
                }
            case CMceSession::EIncoming:
            case CMceSession::EProceeding:
            case CMceSession::EReserving:
                {
                lcSessionState = MLcSession::EReceived;
                break;
                }
            
            case CMceSession::EOffering:
            case CMceSession::EAnswering:
                {
                lcSessionState = MLcSession::EOpening;
                break;
                }
            case CMceSession::EEstablished:
                {
                lcSessionState = MLcSession::EOpen;
                break;
                }
            case CMceSession::ECancelling:
            case CMceSession::ETerminating:
                {
                lcSessionState = MLcSession::EClosing;
                break;
                }
            case CMceSession::ETerminated:
                {
                lcSessionState = MLcSession::EClosed;
                break;
                }
            default:
               {
               lcSessionState = MLcSession::EUninitialized;
               break;
               }
            }
        }
*/

    return  iLcSessionState;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetLcSessionObserver( MLcSessionObserver* aObserver )
    {
    iLcSessionObserver = aObserver;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetLcUiProvider( MLcUiProvider* aUiProvider )
    {
    iLcUiProvider = aUiProvider;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngMceSession::RemoteVideoPlayer()
    {
    return iRemoteVideoPlayer;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngMceSession::LocalVideoPlayer()
    {
    return iLiveVideoPlayer;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
const TDesC& CMusEngMceSession::LocalDisplayName()
    {
    return KNullDesC;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
const TDesC& CMusEngMceSession::RemoteDisplayName()
    {
    return *iRemoteAddress;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TInt CMusEngMceSession::SetParameter( TInt aId, TInt aValue )
    {
        if ( aId == 0 ){
            testWindow = reinterpret_cast<RDrawableWindow*>(aValue);
        }
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TInt CMusEngMceSession::ParameterValue( TInt /*aId*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsBackgroundStartup()
    {    
    return ETrue;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::SendDialTone( TChar aKey )
    {    
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngMceSession::SendDialTone() key = %c",aKey )
    return ETrue;
    }

 // -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsLcAudioMutedL()
    {    
    return EFalse;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::MuteLcAudioL( TBool aMute )
    {
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsLcMicMutedL()
    {
    return EFalse;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::MuteLcMicL( TBool aMute )
    {
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsEnablingLcLoudspeakerAllowed()
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::EnableLcLoudspeakerL( TBool aEnabled )
    {
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsLcLoudspeakerEnabled()
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TInt CMusEngMceSession::LcVolumeL()
    {
    return 5;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetLcVolumeL( TInt aValue )
    {
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::IncreaseLcVolumeL()
    {
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::DecreaseLcVolumeL()
    {
    }


// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::TimedOut( TInt aTimerId )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TimedOut()" )

/*
    iLcSessionState = MLcSession::EOpen;

    if ( iLcSessionObserver )
        {
    	MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TimedOut(), StateChanged" )
        iLcSessionObserver->StateChanged( *this );
        }
*/
        switch( aTimerId )
            {
            case 1: 
                {
    						MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TimedOut(), StateChanged -> Established" )
                
    					InformUiProviderAboutReceivingStart();
    						
    					iLcSessionState = MLcSession::EOpen;
        				iLcSessionObserver->StateChanged( *this );

								// starting remote playing timer        				
        				iTimer->SetTime(10000, 3);

                break;
                }
            case 2: 
                {
    						MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TimedOut(), StateChanged -> RemotePlaying" )
    						iRemoteVideoPlayer->iState = MLcVideoPlayer::EPlaying;
        				iLcSessionObserver->StateChanged( *iRemoteVideoPlayer );
                break;
                }
            case 3: 
                {
    						MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TimedOut(), StateChanged -> LocalPlaying" )
    						iLiveVideoPlayer->iState = MLcVideoPlayer::EPlaying;
        				iLcSessionObserver->StateChanged( *iLiveVideoPlayer );
        				iTimer->SetTime(5000, 2);
                break;
                }
            case 10: 
                {
    						MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TimedOut(), Camera Toggle, StateChanged -> LocalPaused" )
    						iLiveVideoPlayer->iState = MLcVideoPlayer::EPaused;
        				iLcSessionObserver->StateChanged( *iLiveVideoPlayer );
        				iTimer->SetTime(10000, 11);
                break;
                }
            case 11: 
                {
    						MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TimedOut(), Camera Toggle, StateChanged -> LocalPlaying" )
    						iLiveVideoPlayer->iState = MLcVideoPlayer::EPlaying;
        				iLcSessionObserver->StateChanged( *iLiveVideoPlayer );
                break;
                }
            case 12: 
                {
    						MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TimedOut(), Camera Play, StateChanged -> LocalPlaying" )
    						iLiveVideoPlayer->iState = MLcVideoPlayer::EPlaying;
        				iLcSessionObserver->StateChanged( *iLiveVideoPlayer );
                break;
                }
            case 13: 
                {
    						MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TimedOut(), Camera Pause, StateChanged -> LocalIdle" )
    						iLiveVideoPlayer->iState = MLcVideoPlayer::EPaused;
        				iLcSessionObserver->StateChanged( *iLiveVideoPlayer );
                break;
                }
                

            case 14:
                {
                MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TimedOut(), StateChanged -> EReceived" )
                InformUiProviderAboutReceivingStart();
                iLcSessionState = MLcSession::EReceived;
                iLcSessionObserver->StateChanged( *this );
                break;
                }

            default:
               {
               iLcSessionState = MLcSession::EUninitialized;
               break;
               }
						}
/*
        switch( aTimerId )
            {
            case CMceSession::EIdle: 
                {
                lcSessionState = MLcSession::EInitialized;
                break;
                }
            case CMceSession::EIncoming:
            case CMceSession::EProceeding:
            case CMceSession::EReserving:
                {
                lcSessionState = MLcSession::EReceived;
                break;
                }
            
            case CMceSession::EOffering:
            case CMceSession::EAnswering:
                {
                lcSessionState = MLcSession::EOpening;
                break;
                }
            case CMceSession::EEstablished:
                {
                lcSessionState = MLcSession::EOpen;
                break;
                }
            case CMceSession::ECancelling:
            case CMceSession::ETerminating:
                {
                lcSessionState = MLcSession::EClosing;
                break;
                }
            case CMceSession::ETerminated:
                {
                lcSessionState = MLcSession::EClosed;
                break;
                }
            default:
               {
               lcSessionState = MLcSession::EUninitialized;
               break;
               }
            }
*/
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::TimedOut()" )
    }



TInt CMusEngMceSession::SetForegroundStatus( TBool aIsForeground )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngMceSession::SetForegroundStatus() %d", 
              aIsForeground )
    if ( iScreen ){
        iScreen->Update(aIsForeground);
    }
    return KErrNone;
    }

_LIT( KIpVtEngStubRemoteDetails, "5556567844" );

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TDesC& CMusEngMceSession::RemoteDetails()
    {
    return KIpVtEngStubRemoteDetails();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetRemoteL( const TDesC& aRemote )
{
    iRemoteAddress = aRemote.AllocL();
}

#if 0

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutSessionStateChange()
    {
    if ( iLcSessionObserver )
        {
        iLcSessionObserver->StateChanged( *this );
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutSessionUpdate()
    {
    if ( iLcSessionObserver )
        {
        iLcSessionObserver->Updated( *this );
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutSessionFailure( TInt aReason )
    {
    if ( iLcSessionObserver )
        {
        iLcSessionObserver->Failed( *this, aReason );
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutPlayerStateChange( 
    MLcVideoPlayer* aPlayer )
    {
    if ( iLcSessionObserver && aPlayer )
        {
        iLcSessionObserver->StateChanged( *aPlayer );
        }
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutPlayerUpdate( 
    MLcVideoPlayer* aPlayer )
    {
    if ( iLcSessionObserver && aPlayer )
        {
        iLcSessionObserver->Updated( *aPlayer );
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutPlayerFailure( 
    MLcVideoPlayer* aPlayer, 
    TInt aReason )
    {
    if ( iLcSessionObserver && aPlayer )
        {
        iLcSessionObserver->Failed( *aPlayer, aReason );
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MLcUiProvider& CMusEngMceSession::LcUiProviderL()
    {
    __ASSERT_ALWAYS( iLcUiProvider, User::Leave( KErrNotReady ) );
    return *iLcUiProvider;
    }
#endif

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformUiProviderAboutReceivingStart()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::InformUiProviderAboutReceivingStart()" )
    if ( iBackground && iLcUiProvider )
        {
        MUS_LOG( "mus: [ENGINE]     receiving started in BG mode, switching to FG" )
        iBackground = EFalse;
        iLcUiProvider->HandleForegroundStatus( ETrue );
        }
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::InformUiProviderAboutReceivingStart()" )
    }

void CMusEngMceSession::ToggleCameraL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::ToggleCameraL()" )
    if( iLiveVideoPlayer->iState == MLcVideoPlayer::EPlaying )
        {
    		MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::ToggleCameraL(), start timer" )
        iTimer->SetTime(5000, 10);
        }
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::ToggleCameraL()" )
    }
    
void CMusEngMceSession::PlayCameraL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::PlayCameraL()" )
    iTimer->SetTime(5000, 12);
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::PlayCameraL()" )
    }

void CMusEngMceSession::PauseCameraL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::PauseCameraL()" )
    iTimer->SetTime(5000, 13);
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::PauseCameraL()" )
    }
    

// End of file
