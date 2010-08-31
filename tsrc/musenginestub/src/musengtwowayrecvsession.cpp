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
#include "musengtwowayrecvsession.h"
#include "musengsessionobserver.h"
#include "musengreceivesessionobserver.h"
#include "musunittesting.h"
#include "musengmceutils.h"
#include "musenglogger.h"
#include "mussipprofilehandler.h"

// SYSTEM
#include <mcemanager.h>
#include <mcecamerasource.h>
#include <mcevideostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcesession.h>
#include <mcertpsource.h>
#include <mcestreambundle.h>

const TInt KMusEngJitterBufferLength = 51; //Must be bigger than treshold
// Using following value increases treshold buffer to 1 second from 
// default 100 ms
const TInt KMusEngJitterBufferTreshold = 50;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusEngTwoWayRecvSession* CMusEngTwoWayRecvSession::NewL(
                        const TRect& aRemoteRect,
                        const TRect& aLocalRect,
                        MMusEngSessionObserver& aSessionObserver,
                        MMusEngReceiveSessionObserver& aReceiveSessionObserver )
    {
    CMusEngTwoWayRecvSession* self = new( ELeave ) CMusEngTwoWayRecvSession( 
                                                    aSessionObserver,
                                                    aReceiveSessionObserver,
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
CMusEngTwoWayRecvSession::~CMusEngTwoWayRecvSession()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::~CMusEngTwoWayRecvSession()" )
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::~CMusEngTwoWayRecvSession()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngTwoWayRecvSession::EnableDisplayL( TBool aEnable )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngTwoWayRecvSession::EnableDisplayL() %d", 
              aEnable )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceDisplaySink* display = MusEngMceUtils::GetReceivingDisplayL( *iSession );
    MusEngMceUtils::DoEnableDisplayL( *display, aEnable );
                
    CMceDisplaySink* vfDisplay = MusEngMceUtils::GetVfDisplay( *iSession );
    if ( vfDisplay )
        {
        MusEngMceUtils::DoEnableDisplayL( *vfDisplay, aEnable );  
        }
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::EnableDisplayL()")
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceSession::TDisplayOrientation CMusEngTwoWayRecvSession::OrientationL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::RotationL()" )
    
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    CMceDisplaySink* display = MusEngMceUtils::GetReceivingDisplayL( *iSession );
    
    TDisplayOrientation displayOrientation;
    CMceDisplaySink::TRotation rotation( display->RotationL() );
    
    MUS_LOG1( "mus: [ENGINE]     MCE rotation is %d", rotation )
    
    if ( rotation == CMceDisplaySink::ENone )
        {
        displayOrientation = CMusEngMceSession::EPortrait;
        }
    else
        {
        displayOrientation = CMusEngMceSession::ELandscape;
        }
    
    MUS_LOG1( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::RotationL() %d", 
              displayOrientation )
    
    return displayOrientation;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//        
void CMusEngTwoWayRecvSession::SetOrientationL( TDisplayOrientation aOrientation )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::SetOrientationL() %d", 
              aOrientation )
              
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    CMceDisplaySink* display = MusEngMceUtils::GetReceivingDisplayL( *iSession );
            
    CMceDisplaySink* vfDisplay = MusEngMceUtils::GetVfDisplay( *iSession );

    CMceDisplaySink::TRotation rotation = ( aOrientation == EPortrait ) ? 
        CMceDisplaySink::ENone : CMceDisplaySink::EClockwise90Degree;
    
    display->SetRotationL( rotation );
    if ( vfDisplay )
        {
        vfDisplay->SetRotationL( rotation );
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::SetOrientationL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngTwoWayRecvSession::SetRectsL( 
    const TRect& aRemoteRect,
    const TRect& aLocalRect )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::SetRectsL()" )  
     
    iLocalRect = aLocalRect;
    iRect = aRemoteRect;
    RectChangedL();
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::SetRectsL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngTwoWayRecvSession::SetLocalRectL( const TRect& aLocalRect )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::SetLocalRectL()" )
    
    iLocalRect = aLocalRect;
    RectChangedL();

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::SetLocalRectL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TRect CMusEngTwoWayRecvSession::LocalRect() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::LocalRect()" )
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::LocalRect()" )
    return iLocalRect;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TMusEngCameraHandler& CMusEngTwoWayRecvSession::Camera()
    {
    return iCameraHandler;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//

void CMusEngTwoWayRecvSession::RectChangedL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::RectChangedL()" )


    // If session is not yet created, do nothing
    if ( iSession && iSession->State() != CMceSession::ETerminated )
        {
        TRect remoteRect( Rect() );
        MUS_LOG2( "mus: [ENGINE]  remote tl.ix=%d tl.iy=%d", 
                  remoteRect.iTl.iX, remoteRect.iTl.iY )
        MUS_LOG2( "mus: [ENGINE]  remote brc%d br.iy=%d", 
                  remoteRect.iBr.iX, remoteRect.iBr.iY )    
        MUS_LOG2( "mus: [ENGINE]  local tl.ix=%d tl.iy=%d", 
                  iLocalRect.iTl.iX, iLocalRect.iTl.iY )
        MUS_LOG2( "mus: [ENGINE]  local br.ix=%d br.iy=%d", 
                  iLocalRect.iBr.iX, iLocalRect.iBr.iY )
            
        CMceDisplaySink* display = MusEngMceUtils::GetReceivingDisplayL( *iSession );
        display->SetDisplayRectL( remoteRect );
        
        CMceDisplaySink* vfDisplay = MusEngMceUtils::GetVfDisplay( *iSession );
        if ( vfDisplay )
            {
            vfDisplay->SetDisplayRectL( iLocalRect );
            }
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::RectChangedL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWayRecvSession::CompleteSessionStructureL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::CompleteSessionStructureL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    iCameraHandler.SetSession( iSession );
    
    // Force bandwidth line usage in sdp as it is mandatory
    // at MT side based on GSMA VS specification IR.74. Bandwidth is set to
    // session or to media level based on sender's way of usage. If other end
    // is not using bandwidth attribute at all, media level is preferred.
    SetSessionSdpLinesL( *iSession, ETrue );

    CMceStreamBundle* localBundle =
                              CMceStreamBundle::NewLC( CMceStreamBundle::ELS );

    const RPointerArray<CMceMediaStream>& streams = iSession->Streams();
    
    CMceMediaStream* videoInStream = NULL;
    CMceMediaStream* videoOutStream = NULL;
    
    // Search interesting video streams, sendrecv is preferred
    TBool sendRecvVideoFound( EFalse );
    for( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( MusEngMceUtils::IsVideoInStream( *streams[i] ) &&
             !sendRecvVideoFound )
            {
            videoInStream = streams[i];
            
            if ( streams[i]->BoundStream() )
                {
                videoOutStream = &streams[i]->BoundStreamL();
                }
            }
        else if ( streams[i]->BoundStream() &&
                  MusEngMceUtils::IsVideoInStream( streams[i]->BoundStreamL() ) &&
                  !sendRecvVideoFound )
            {
            videoInStream = &streams[i]->BoundStreamL();
                
            videoOutStream = streams[i];
            }
        else
            {
            // NOP
            }
        
        sendRecvVideoFound = ( videoInStream && videoOutStream );
        }
    
    CompleteSessionStructureAudioPartL( streams, *localBundle, videoInStream, videoOutStream );
    
    if ( videoInStream )
        {
        CompleteSessionStructureInStreamL( *videoInStream, *localBundle );
        }
    else
        {
        // At least receiving video stream is required
        User::Leave( KErrCorrupt );
        }
    
    if ( videoOutStream )
        {
        CompleteSessionStructureOutStreamL( *videoOutStream );
        }
    
    // Destroy bundle if it is not needed or transfer ownership
    if ( localBundle->Streams().Count() > 1 )
        {
        iSession->AddBundleL( localBundle );
        CleanupStack::Pop( localBundle );
        }
    else
        {
        CleanupStack::PopAndDestroy( localBundle );
        }
    
    AdjustStreamsAndCodecsL();
    
    iSession->UpdateL();
    
    // Now session state is right to adjust volume
    SetSpeakerVolumeL( VolumeL() );
    
    iSipProfileHandler->CreateProfileL( iSession->Profile() );

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::CompleteSessionStructureL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWayRecvSession::StreamStateChanged( CMceMediaStream& aStream,
                                             CMceMediaSink& aSink )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngTwoWayRecvSession::StreamStateChanged( sink )" )
    if ( !iSession )
        {
        return;
        }
    
    MUS_ENG_LOG_STREAM_STATE( aStream )
    
    // TODO: if display sink buffers, inform ui with some callback
    // so that it can modify display rects accordingly
    
    CMusEngMceSession::StreamStateChanged( aStream, aSink );

    MUS_LOG( "mus: [ENGINE] <- CMusEngTwoWayRecvSession::StreamStateChanged( sink )" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWayRecvSession::StreamStateChanged( CMceMediaStream& aStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngTwoWayRecvSession::StreamStateChanged()" )
    if ( !iSession )
        {
        return;
        }
    
    MUS_ENG_LOG_STREAM_STATE( aStream )
    
    // TODO: if receiving video stream buffers, inform ui with some callback
    // so that it can modify display rects accordingly
    
    CMusEngReceiveSession::StreamStateChanged( aStream );

    MUS_LOG( "mus: [ENGINE] <- CMusEngTwoWayRecvSession::StreamStateChanged()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngTwoWayRecvSession::CMusEngTwoWayRecvSession(
                        MMusEngSessionObserver& aSessionObserver,
                        MMusEngReceiveSessionObserver& aReceiveSessionObserver,
                        const TRect& aRemoteRect,
                        const TRect& aLocalRect ) : 
    CMusEngReceiveSession( aSessionObserver,
                           aReceiveSessionObserver,
                           aRemoteRect ),
    iLocalRect( aLocalRect )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWayRecvSession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::ConstructL()" )
    
    iCameraHandler.ReadCameraUsageKeyL();
    CMusEngReceiveSession::ConstructL();

    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::ConstructL()" )
    }

// -----------------------------------------------------------------------------
// When checking audio streams also not interesting streams are removed from
// session. Stream if removed if one of following apply:
// 1. Is not and does not contain incoming video or audio
// 2. We already have one incoming video stream
// 3. Stream is audio and we run operator variant where audio is 
//    not allowed.
// 4. Two-way video exists and this one is audio
// -----------------------------------------------------------------------------
//
void CMusEngTwoWayRecvSession::CompleteSessionStructureAudioPartL( 
    const RPointerArray<CMceMediaStream>& aStreams, 
    CMceStreamBundle& aLocalBundle, 
    CMceMediaStream* aVideoInStream,
    CMceMediaStream* aVideoOutStream )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::CompleteSessionStructureAudioPartL()" )
    
    // Audio streams not allowed in two-way session or in operator variant
    TBool audioAllowed(  !( aVideoInStream && aVideoOutStream ) && !iOperatorVariant );
    
    for( TInt i = 0; i < aStreams.Count(); ++i )
        {
        // Audio supported currently only in recvonly case
        if ( audioAllowed &&
             MusEngMceUtils::IsAudioInStream( *aStreams[i] ) )
            {
            MusEngMceUtils::AddSpeakerL( *aStreams[i] );
            
            aLocalBundle.AddStreamL( *aStreams[i] );
            
            // Disable possible opposite stream to indicate that sendrecv audio is
            // not allowed.
            if ( aStreams[i]->BoundStream() )
                {
                MusEngMceUtils::DisableStreamL( aStreams[i]->BoundStreamL() );
                }
            }
        else if ( audioAllowed &&
                  aStreams[i]->BoundStream() &&
                  MusEngMceUtils::IsAudioInStream( aStreams[i]->BoundStreamL() ) )
            {
            MusEngMceUtils::AddSpeakerL( aStreams[i]->BoundStreamL() );
    
            aLocalBundle.AddStreamL( aStreams[i]->BoundStreamL() );
            
            // Disable opposite stream to indicate that sendrecv audio is not allowed.
            MusEngMceUtils::DisableStreamL( *aStreams[i] );
            }
        else if ( aStreams[ i ] != aVideoInStream && aStreams[ i ] != aVideoOutStream )
            {
           iSession->RemoveStreamL( *aStreams[i] );
        
            // Since succesfull removal of a stream has decreased the amount
            // of streams in array by one, we have to modify the index
            --i;
            }
        else
            {
            // NOP
            }
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::CompleteSessionStructureAudioPartL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWayRecvSession::CompleteSessionStructureInStreamL( 
    CMceMediaStream& aVideoInStream, CMceStreamBundle& aLocalBundle )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::CompleteSessionStructureInStreamL()" )
    
    SetMediaSdpLinesL( aVideoInStream, ETrue );

    MusEngMceUtils::AddDisplayL( aVideoInStream, *iManager, Rect() );
    
    static_cast<CMceRtpSource*>(aVideoInStream.Source())->UpdateL( 
                                           KMusEngJitterBufferLength,
                                           KMusEngJitterBufferTreshold );
                                            
    aLocalBundle.AddStreamL( aVideoInStream );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::CompleteSessionStructureInStreamL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngTwoWayRecvSession::CompleteSessionStructureOutStreamL( 
    CMceMediaStream& aVideoOutStream )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::CompleteSessionStructureOutStreamL()" )
    
    SetMediaSdpLinesL( aVideoOutStream, ETrue );
        
    CMceCameraSource* camera = NULL;
    TRAP_IGNORE( camera = MusEngMceUtils::GetCameraL( *iSession ) )
    if ( !camera )
        {
        camera = CMceCameraSource::NewLC( *iManager );
        aVideoOutStream.SetSourceL( camera );
        CleanupStack::Pop( camera );
        }
       
    camera->DisableL(); // Start session in pause mode.
    
    iCameraHandler.InitializeL( *camera );
    
    CMceVideoStream* vfStream = CMceVideoStream::NewLC();

    vfStream->SetSourceL( aVideoOutStream.Source() );
         
    // Complete stream
    MusEngMceUtils::AddDisplayL( *vfStream, *iManager, LocalRect() );
    
    iSession->AddStreamL( vfStream );
    CleanupStack::Pop( vfStream );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::CompleteSessionStructureOutStreamL()" )
    }

// End of file
