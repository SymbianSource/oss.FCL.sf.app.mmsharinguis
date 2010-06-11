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
* Description: 
*
*/


// USER
#include "musengtwowaysession.h"
#include "musunittesting.h"
#include "musengmceutils.h"
#include "musenglogger.h"
#include "musengremotevideoplayer.h"
#include "musenglivevideoplayer.h"

// SYSTEM
#include <lcsessionobserver.h>
#include <mcemanager.h>
#include <mcecamerasource.h>
#include <mcevideostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcesession.h>
#include <mcertpsource.h>
#include <mcevideocodec.h>

const TInt KMusEngJitterBufferLength = 51; //Must be bigger than treshold
// Using following value increases treshold buffer to 1 second from 
// default 100 ms
const TInt KMusEngJitterBufferTreshold = 50;

const TUint32 KMusEngTwoWayReceivingActivityTimeout = 5000; // 5 seconds
// 6 seconds keepalive timer, needs to be more than receiving timeout
const TUint8 KMusEngTwoWayRtpKeepAliveTimer = 6; 

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTwoWaySession* CMusEngTwoWaySession::NewL()
    {
    CMusEngTwoWaySession* self = new( ELeave )CMusEngTwoWaySession();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTwoWaySession::~CMusEngTwoWaySession()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::~CMusEngTwoWaySession()" )
    
    delete iRemoteVideoPlayer; 
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::~CMusEngTwoWaySession()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::EnableDisplayL( TBool aEnable )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngTwoWaySession::EnableDisplayL() %d", 
              aEnable )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceDisplaySink* display = MusEngMceUtils::GetReceivingDisplay( *iSession );
    if ( display )
        {
        MusEngMceUtils::DoEnableDisplayL( *display, aEnable );
        }     
    CMceDisplaySink* vfDisplay = MusEngMceUtils::GetDisplayL( *iSession, ETrue );
    MusEngMceUtils::DoEnableDisplayL( *vfDisplay, aEnable );     
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::EnableDisplayL()")
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//        
void CMusEngTwoWaySession::SetOrientationL( TDisplayOrientation aOrientation )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngTwoWaySession::SetOrientationL() %d", 
              aOrientation )
              
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    CMceDisplaySink* display = MusEngMceUtils::GetReceivingDisplay( *iSession );            
    CMceDisplaySink* vfDisplay = MusEngMceUtils::GetDisplayL( *iSession, ETrue );

    CMceDisplaySink::TRotation rotation = ( aOrientation == EPortrait ) ? 
         CMceDisplaySink::ENone : CMceDisplaySink::EClockwise90Degree;
     
    vfDisplay->SetRotationL( rotation );
    if ( display )
        {
        display->SetRotationL( rotation );
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::SetOrientationL()" )
    }

// -----------------------------------------------------------------------------
// From MLcFileControl
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::EnableLcFileL( TBool /*aEnable*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::EnableLcFileL" )

    User::Leave( KErrNotSupported );

    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::EnableLcFileL" ) 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::SetRectsL( 
    const TRect& aRemoteRect,
    const TRect& aLocalRect )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::SetRectsL()" )  
     
    iLocalRect = aLocalRect;
    iRect = aRemoteRect;
    RectChangedL();
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::SetRectsL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::SetSecondaryRectL( const TRect& aSecondaryRect )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::SetSecondaryRectL()" )
    
    iLocalRect = aSecondaryRect;
    RectChangedL();

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

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CMusEngTwoWaySession::IsDisplayEnabled()
    {
    TBool enabled( EFalse );
    if ( iSession )
        {
        CMceDisplaySink* display = MusEngMceUtils::GetReceivingDisplay( *iSession );
        if ( !display )
            {
            display = MusEngMceUtils::GetVfDisplay( *iSession );
            }
        if ( display )
           {
           enabled = display->IsEnabled();
           }
        }
    return enabled;
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CMusEngTwoWaySession::IsDisplayActive()
    {
    return ( IsDisplayEnabled() && iReceiving );
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngTwoWaySession::RemoteVideoPlayer()
    {
    return iRemoteVideoPlayer;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::RectChangedL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::RectChangedL()" )

    // If session is not yet created, do nothing
    if ( iSession && iSession->State() != CMceSession::ETerminated )
        {
        TRect remoteRect( Rect() );
        MUS_LOG2( "mus: [ENGINE]  remote tl.ix=%d tl.iy=%d", 
                  remoteRect.iTl.iX, remoteRect.iTl.iY )
        MUS_LOG2( "mus: [ENGINE]  remote br.ix=%d br.iy=%d", 
                  remoteRect.iBr.iX, remoteRect.iBr.iY )    
        MUS_LOG2( "mus: [ENGINE]  local tl.ix=%d tl.iy=%d", 
                  iLocalRect.iTl.iX, iLocalRect.iTl.iY )
        MUS_LOG2( "mus: [ENGINE]  local br.ix=%d br.iy=%d", 
                  iLocalRect.iBr.iX, iLocalRect.iBr.iY )           

        if ( remoteRect != iSetRemoteRect )
            {
            CMceDisplaySink* display = MusEngMceUtils::GetReceivingDisplay( *iSession );
            if ( display )
                {
                MUS_LOG( "mus: [ENGINE]  set remote rect" )
                display->SetDisplayRectL( remoteRect );
                iSetRemoteRect = remoteRect;
                }
            }

        if ( iLocalRect != iSetLocalRect )
            {
            MUS_LOG( "mus: [ENGINE]  set local rect" )
            CMceDisplaySink* vfDisplay = MusEngMceUtils::GetDisplayL( *iSession, ETrue );
            vfDisplay->SetDisplayRectL( iLocalRect );
            iSetLocalRect = iLocalRect;
            }
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::RectChangedL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::CompleteSessionStructureL( 
                                            CMceStreamBundle& /*aLocalBundle*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::CompleteSessionStructureL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    iCameraHandler.SetSession( iSession );
    
    // Create outgoing video stream
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    
    CMceRtpSink* rtpsink = CMceRtpSink::NewLC();
    videoStream->AddSinkL( rtpsink );
    CleanupStack::Pop( rtpsink );

    CMceCameraSource* camera = CMceCameraSource::NewLC( *iManager );
    camera->DisableL(); // Start session in pause mode.
    
    iCameraHandler.InitializeL( *camera );

    videoStream->SetSourceL( camera );
    CleanupStack::Pop( camera );
    
    // Create incoming video stream
    CMceVideoStream* videoInStream = CMceVideoStream::NewL();
    CleanupStack::PushL( videoInStream );
       
    MusEngMceUtils::AddDisplayL( *videoInStream, *iManager, Rect() );

    CMceRtpSource* rtpSource = CMceRtpSource::NewLC( KMusEngJitterBufferLength,
                                                     KMusEngJitterBufferTreshold,
                                                     KMusEngTwoWayReceivingActivityTimeout );
    videoInStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource ); 

    videoStream->BindL( videoInStream ); 
    CleanupStack::Pop( videoInStream );

    iSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );    

    iLiveVideoPlayer->SetMceSession( iSession );
    iRemoteVideoPlayer->SetMceSession( iSession );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::CompleteSessionStructureL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::CreateMceSessionStructureL( 
    TBool /*aForceSdpBandwidth*/ ) 
    {
    //Force SDP bandwidth attributes to be included in the INVITE
    CMusEngMceOutSession::CreateMceSessionStructureL( ETrue );
    }

// -----------------------------------------------------------------------------
// Sets video codec attributes
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::AdjustVideoCodecL( CMceVideoCodec& aVideoCodec,
                                              TMceSourceType aSourceType )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngTwoWaySession::AdjustVideoCodecL()" )
    
    CMusEngLiveSession::AdjustVideoCodecL( aVideoCodec, aSourceType );
    
    aVideoCodec.SetKeepAliveTimerL( KMusEngTwoWayRtpKeepAliveTimer );
    if ( aVideoCodec.SdpName() == KMceSDPNameH263() ||
         aVideoCodec.SdpName() == KMceSDPNameH2632000() )
        {
        aVideoCodec.SetKeepAlivePayloadTypeL( KMusEngKeepAlivePayloadTypeVideoH263 );   
        }
    else if ( aVideoCodec.SdpName() == KMceSDPNameH264() )
        {
        aVideoCodec.SetKeepAlivePayloadTypeL( KMusEngKeepAlivePayloadTypeVideoAvc );                          
        }
    else
        {
        // NOP
        }
    
    aVideoCodec.SetKeepAliveDataL( KNullDesC8() );
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngTwoWaySession::AdjustVideoCodecL()" )
    }
	
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::AddDisplayL( CMceMediaStream& aStream )
    {
    // Vf display is disabled at creation phase if doing bg startup
    MusEngMceUtils::AddDisplayL( aStream, 
                                 *iManager,
                                 iLocalRect,
                                 IsBackgroundStartup() );
    }
 
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::StreamStateChanged( CMceMediaStream& aStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngTwoWaySession::StreamStateChanged()" )
    if ( !iSession )
        {
        return;
        }
    
    if ( aStream.Type() == KMceVideo &&
         aStream.Source() &&
         aStream.Source()->Type() == KMceRTPSource )
        {
        if ( aStream.State() == CMceMediaStream::EStreaming )
            {
            ReceivingStarted();
            }
        else if ( aStream.State() == CMceMediaStream::EBuffering )
            {
            MUS_LOG( "mus: [ENGINE] CMusEngTwoWaySession buffered" )
            
            iBuffered = ETrue;
            }
        else
            {
            // NOP
            }
        }
    
    CMusEngMceSession::StreamStateChanged( aStream );

    MUS_LOG( "mus: [ENGINE] <- CMusEngTwoWaySession::StreamStateChanged()" )
    }

// -----------------------------------------------------------------------------
// Once inactivity timeout occurs, state change is notified and receiving
// and buffering statuses are cleared. That is safe to do as once receiving
// again continues for real, buffering and streaming events will occur always 
// sequentially. If buffering status would not be cleared, some unwanted
// sreaming events would be passed towards client when it disables/enables
// display sink of remote stream while inactivity timer has expired.
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::InactivityTimeout( CMceMediaStream& aStream,
                                              CMceRtpSource& /*aSource*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::InactivityTimeout()" )
    
    if ( aStream.Type() == KMceVideo && iReceiving )
        {
        iReceiving = EFalse;
        iBuffered = EFalse;
        if ( iRemoteVideoPlayer )
            {
            InformObserverAboutPlayerStateChange( iRemoteVideoPlayer );
            }
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::InactivityTimeout()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::HandleSessionStateChanged( 
    CMceSession& aSession,
    TInt aStatusCode,
    const TDesC8& aReasonPhrase )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::HandleSessionStateChanged" )
    
    MUS_ENG_LOG_SESSION_STATE_AND_STATUS( aSession, aStatusCode, aReasonPhrase )
    
    TBool consumed( EFalse );
    if ( iSession && 
         iSession == &aSession && 
         aSession.State() == CMceSession::EEstablished )
        {   
        CMceVideoStream* stream = NULL;
        CMceVideoStream* streamout = NULL;
        TRAPD( err, stream = MusEngMceUtils::GetVideoInStreamL( *iSession ) )
        if ( err == KErrNone && stream && stream->IsEnabled() )
            {
            TRAP( err, streamout = MusEngMceUtils::GetVideoOutStreamL( *iSession ) )
            if ( err != KErrNone || !streamout || !streamout->IsEnabled() )
                {
                MUS_LOG( "mus: [ENGINE] sendrecv downgraded to recvonly, terminate!" )
                consumed = ETrue;
                InformObserverAboutSessionFailure( err );
                }
            }
        }
    
    if ( !consumed )
        {
        CMusEngLiveSession::HandleSessionStateChanged( aSession,
                                                       aStatusCode, 
                                                       aReasonPhrase );
        }
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::HandleSessionStateChanged" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTwoWaySession::CMusEngTwoWaySession() 
    : CMusEngLiveSession()
    {
    iSetRemoteRect = TRect( 
        KMusEngRectNotInit, KMusEngRectNotInit, KMusEngRectNotInit, KMusEngRectNotInit );
    iSetLocalRect = TRect(
        KMusEngRectNotInit, KMusEngRectNotInit, KMusEngRectNotInit, KMusEngRectNotInit );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::ConstructL()" )
    
    CMusEngLiveSession::ConstructL();
    
    iRemoteVideoPlayer = CMusEngRemoteVideoPlayer::NewL( *this, *this ); 

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::ConstructL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::ReceivingStarted()
    {
    if ( iSession && iBuffered )
        {
        MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::ReceivingStarted()" )
        
        TInt err = MusEngMceUtils::EnableInactivityTimer( 
            *iSession, KMusEngTwoWayReceivingActivityTimeout );
        
        if ( err != KErrNone )
            {
            MUS_LOG1("mus: [ENGINE] ReceivingStarted failed %d", err)
            InformObserverAboutSessionFailure( err );   
            }
        else
            {
            iReceiving = ETrue;
            InformObserverAboutPlayerStateChange( iRemoteVideoPlayer );
            InformUiProviderAboutReceivingStart();
            }
        
        MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::ReceivingStarted()" )
        }
    }

// End of file
