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

// SYSTEM
#include <audiopreference.h>
#include <e32property.h>

#include <cntitem.h>
#include <cntfield.h>
#include <cntdef.h>



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
    
    iLiveVideoPlayer = CMusEngLiveVideoPlayer::NewL( *this, iCameraHandler, *this );      
    iLcSessionState = MLcSession::EUninitialized;
        
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
    iRect = aRect;
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetSecondaryRectL( const TRect& aSecondaryRect )
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngMceSession::SetSecondaryRectL()" )
    
    // Same rect is set by primary and secondary setters
    iRect = aSecondaryRect;
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
TRect CMusEngMceSession::SecondaryRect() const
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngMceSession::SecondaryRect()" )
    return iRect;
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
TInt CMusEngMceSession::SetParameter( TInt /*aId*/, TInt /*aValue*/ )
    {
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
    return EFalse;
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

    iLcSessionState = MLcSession::EOpen;

    if ( iLcSessionObserver )
        {
    	MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TimedOut(), StateChanged" )
        iLcSessionObserver->StateChanged( *this );
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
    /*          
    if ( aIsForeground )
        {
        // Only clear the bg info, do not ever enable it again
        iBackground = EFalse;
        }
    */    
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
void CMusEngMceSession::InformUiProviderAboutReceivingStart()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::InformUiProviderAboutReceivingStart()" )
    if ( iBackground && iLcUiProvider )
        {
        MUS_LOG( "mus: [ENGINE]     receiving started in BG mode, switching to FG" )
        iLcUiProvider->HandleForegroundStatus( ETrue );
        iBackground = EFalse;
        }
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::InformUiProviderAboutReceivingStart()" )
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
void CMusEngMceSession::UpdateLcSessionL()
		{
		}

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::SendDialTone( TChar aKey )
    {    
    return ETrue;
    }



// End of file
