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
#include "musengtwowaysession.h"
#include "musengsessionobserver.h"
#include "musenglivesessionobserver.h"
#include "musunittesting.h"
#include "musengmceutils.h"
#include "musenglogger.h"
#include "musengtwowaysessionobserver.h"

// SYSTEM
#include <mcemanager.h>
#include <mcecamerasource.h>
#include <mcevideostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcesession.h>
#include <mcertpsource.h>

const TInt KMusEngJitterBufferLength = 51; //Must be bigger than treshold
// Using following value increases treshold buffer to 1 second from 
// default 100 ms
const TInt KMusEngJitterBufferTreshold = 50;

const TUint32 KMusEngTwoWayReceivingActivityTimeout = 3000; // 3 seconds

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusEngTwoWaySession* CMusEngTwoWaySession::NewL(
                        const TRect& aRemoteRect,
                        const TRect& aLocalRect,
                        MMusEngSessionObserver& aSessionObserver,
                        MMusEngOutSessionObserver& aOutSessionObserver,
                        MMusEngLiveSessionObserver& aLiveSessionObserver,
                        MMusEngTwoWaySessionObserver& aTwoWayObserver )
    {
    CMusEngTwoWaySession* self = new( ELeave ) CMusEngTwoWaySession( 
                                                    aSessionObserver,
                                                    aOutSessionObserver,
                                                    aLiveSessionObserver, 
                                                    aTwoWayObserver,
                                                    aRemoteRect,
                                                    aLocalRect );
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
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::RecordL( TBool aRecord )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngTwoWaySession::RecordL( %d )", aRecord )

    User::Leave( KErrNotSupported );

    MUS_LOG1( "mus: [ENGINE]  <- CMusEngTwoWaySession::RecordL( %d )", aRecord ) 
    }    


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngTwoWaySession::IsRecording()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::IsRecording()" )    
    
    return EFalse;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngTwoWaySession::SetRectsL( 
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
EXPORT_C void CMusEngTwoWaySession::SetLocalRectL( const TRect& aLocalRect )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::SetLocalRectL()" )
    
    iLocalRect = aLocalRect;
    RectChangedL();

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::SetLocalRectL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TRect CMusEngTwoWaySession::LocalRect() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::LocalRect()" )
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::LocalRect()" )
    return iLocalRect;
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
            
        CMceDisplaySink* display = MusEngMceUtils::GetReceivingDisplay( *iSession );
        if ( display )
            {
            display->SetDisplayRectL( remoteRect );
            }
        CMceDisplaySink* vfDisplay = MusEngMceUtils::GetDisplayL( *iSession, ETrue );
        vfDisplay->SetDisplayRectL( iLocalRect );
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
                                                     KMusEngJitterBufferTreshold );
    videoInStream->SetSourceL( rtpSource );
    CleanupStack::Pop( rtpSource ); 

    videoStream->BindL( videoInStream ); 
    CleanupStack::Pop( videoInStream );

    iSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );    

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::CompleteSessionStructureL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::AddDisplayL( CMceMediaStream& aStream )
    {
    MusEngMceUtils::AddDisplayL( aStream, 
                                 *iManager,
                                 iLocalRect );
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
    
    MUS_ENG_LOG_STREAM_STATE( aStream )
    
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
            iBuffered = ETrue;
            }
        }
    
    CMusEngMceSession::StreamStateChanged( aStream );

    MUS_LOG( "mus: [ENGINE] <- CMusEngTwoWaySession::StreamStateChanged()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::InactivityTimeout( CMceMediaStream& aStream,
                                              CMceRtpSource& /*aSource*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::InactivityTimeout()" )
    
    if ( aStream.Type() == KMceVideo )
        {
        ReceivingStopped();
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
        // TODO: if receiving stream is disabled, inform ui with some callback
        // that it can modify display rects accordingly and continue with 
        // session.
        CMceVideoStream* stream = NULL;
        CMceVideoStream* streamout = NULL;
        TRAPD( err, stream = MusEngMceUtils::GetVideoInStreamL( *iSession ) );
        TRAPD( errout, streamout = MusEngMceUtils::GetVideoOutStreamL( *iSession ) );
        if ( err == KErrNone && stream && stream->IsEnabled() )
            {
            if ( errout != KErrNone || !streamout || !streamout->IsEnabled() )
                {
                MUS_LOG( "mus: [ENGINE] sendrecv downgraded to recvonly, terminate!" )
                consumed = ETrue;
                iSessionObserver.SessionFailed();
                }
            }
        else
            {
            if ( errout == KErrNone && streamout && streamout->IsEnabled() )
                {
                MUS_LOG( "mus: [ENGINE] sendrecv downgraded to sendonly, continue!" )
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
CMusEngTwoWaySession::CMusEngTwoWaySession(
                        MMusEngSessionObserver& aSessionObserver,
                        MMusEngOutSessionObserver& aOutSessionObserver,
                        MMusEngLiveSessionObserver& aLiveSessionObserver,
                        MMusEngTwoWaySessionObserver& aTwoWayObserver,
                        const TRect& aRemoteRect,
                        const TRect& aLocalRect ) : 
    CMusEngLiveSession( aSessionObserver,
                        aOutSessionObserver,
                        aLiveSessionObserver,
                        aRemoteRect,
                        KNullDesC ),
    iTwoWayObserver( aTwoWayObserver ),                   
    iLocalRect( aLocalRect )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::ConstructL()" )
    
    CMusEngLiveSession::ConstructL();

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::ConstructL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::ReceivingStarted()
    {
    if ( iSession && !iReceiving && iBuffered )
        {
        MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::ReceivingStarted()" )
        
        TInt err( KErrNone );
        CMceMediaSource* source = NULL;
        TRAP( err, source = 
            MusEngMceUtils::GetVideoInStreamL( *iSession )->Source() );
        if ( source && source->Type() == KMceRTPSource )
            {
            CMceRtpSource* rtpSource = static_cast<CMceRtpSource*>( source );
            TRAP( err, rtpSource->EnableInactivityTimerL( 
                            KMusEngTwoWayReceivingActivityTimeout ) ) 
            }
        
        if ( err != KErrNone )
            {
            MUS_LOG1("mus: [ENGINE] ReceivingStarted failed %d", err)
            iSessionObserver.SessionFailed();   
            }
        else
            {
            iReceiving = ETrue;
            iTwoWayObserver.ReceivingStateChanged( ETrue );
            }
        
        MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::ReceivingStarted()" )
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWaySession::ReceivingStopped()
    {
    if ( iReceiving )
        {
        MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWaySession::ReceivingStopped()" )
        
        iReceiving = EFalse;
        iTwoWayObserver.ReceivingStateChanged( EFalse );
        
        MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWaySession::ReceivingStopped()" )
        }
    }

// End of file
