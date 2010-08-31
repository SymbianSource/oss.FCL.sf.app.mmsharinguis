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
#include "musenglivesession.h"
#include "musunittesting.h"
#include "musengmceutils.h"
#include "musenglogger.h"
#include "mussessionproperties.h"
#include "musenglivevideoplayer.h"

// SYSTEM
#include <lcsessionobserver.h>
#include <mcemanager.h>
#include <mcecamerasource.h>
#include <mcevideostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcefilesink.h>
#include <mcesession.h>
#include <mcevideocodec.h>
#include <mceh263codec.h>
#include <mceavccodec.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession* CMusEngLiveSession::NewL()
    {
    CMusEngLiveSession* self = new( ELeave )CMusEngLiveSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession::CMusEngLiveSession()
    : CMusEngMceOutSession()
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::ConstructL()" )
    
    iCameraHandler.ReadCameraUsageKeyL();
    CMusEngMceOutSession::ConstructL();
    
    iLiveVideoPlayer = 
        CMusEngLiveVideoPlayer::NewL( *this, iCameraHandler, *this );      
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::ConstructL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession::~CMusEngLiveSession()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::~CMusEngLiveSession()" )    
    delete iLiveVideoPlayer;
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::~CMusEngLiveSession()" )
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngLiveSession::LocalVideoPlayer()
    {
    return iLiveVideoPlayer;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::CompleteSessionStructureL( 
    CMceStreamBundle& /*aLocalBundle*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::CompleteSessionStructureL()" )

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

    iSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );    

    // Construct recording stream if needed
    if ( iLiveVideoPlayer->LcFileName().Length() > 0 )
        {
        CMceVideoStream* streamForRecording = CMceVideoStream::NewLC();
        
        CMceFileSink* fileSink = 
            CMceFileSink::NewLC( iLiveVideoPlayer->LcFileName() );
        fileSink->DisableL(); // Start in not recording mode
        streamForRecording->AddSinkL( fileSink );
        CleanupStack::Pop( fileSink );
        
        streamForRecording->SetSourceL( camera );
        iSession->AddStreamL( streamForRecording );
        CleanupStack::Pop( streamForRecording );
        }
    
    iLiveVideoPlayer->SetMceSession( iSession );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::CompleteSessionStructureL()" )
    }

// -----------------------------------------------------------------------------
// Sets video codec attributes
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::AdjustVideoCodecL( CMceVideoCodec& aVideoCodec,
                                            TMceSourceType aSourceType )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngLiveSession::AdjustVideoCodecL()" )
    
    CMusEngMceOutSession::AdjustVideoCodecL( aVideoCodec, aSourceType );
    
    // Starting with 80Kbps  will give better quality than starting with
    // 64kbps.And if network behaves bad than anyhow we will drop down or
    // if network is good guy than we climp up to 128. 
    // Esp in operator variance. No constant defined in MCE so defining
    // one here.
    const TInt  KMushInitialBitrate = 80000;  
    MUS_LOG1( "mus: [ENGINE] - Intial bit rate set to %d",KMushInitialBitrate);
    User::LeaveIfError( aVideoCodec.SetBitrate( KMushInitialBitrate ) );
                
    if ( aVideoCodec.SdpName() == KMceSDPNameH263() ||
         aVideoCodec.SdpName() == KMceSDPNameH2632000() )
        {
        // Set H.263 codec to allow all bitrates, set maximum to level 45 and
        // start using level 10 and let the rate control raise it if possible
        // Label:H263
        User::LeaveIfError( aVideoCodec.SetAllowedBitrates( 
                                                KMceAllowedH263BitrateAll ) );
        aVideoCodec.SetMaxBitrateL( KMceH263Level45Bitrate );        
        }
    else if ( aVideoCodec.SdpName() == KMceSDPNameH264() )
        {
        User::LeaveIfError( aVideoCodec.SetAllowedBitrates( 
                                        KMceAvcCodecProfileIdBaseline | 
                                        KMceAvcCodecProfileIopConstraintSet | 
                                        KMceAvcBitrateLevel1b ) );                           
        }
    else
        {
        // NOP
        }
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngLiveSession::AdjustVideoCodecL()" )
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::AdjustAudioCodecL( CMceAudioCodec& aAudioCodec )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngLiveSession::AdjustAudioCodecL()" )
    
    CMusEngMceOutSession::AdjustAudioCodecL( aAudioCodec );
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngLiveSession::AdjustAudioCodecL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::DoCodecSelectionL( CMceVideoStream& aVideoStream )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::DoCodecSelectionL()" )
    
    CMusEngMceSession::DoCodecSelectionL( aVideoStream );
    
    if ( iVideoCodecList && !IsH264Supported() )
        {        
        // We know that recipient doesn't support AVC, so we do not offer it
        const RPointerArray<CMceVideoCodec>& codecs = aVideoStream.Codecs();
    
        for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
            {
            if ( codecs[codecIndex]->SdpName() == KMceSDPNameH264() )
                {
                aVideoStream.RemoveCodecL( *codecs[codecIndex] );
                // Since succesfull removal of a codec has changed the
                // indexing, we have to reset the index
                codecIndex = 0;
                }
            }
        }
     
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::DoCodecSelectionL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::StreamStateChanged( CMceMediaStream& aStream,
                                             CMceMediaSink& aSink )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngLiveSession::StreamStateChanged( sink )" )
    
    if ( !iSession )
        {
        return;
        }
    
    MUS_ENG_LOG_STREAM_STATE( aStream )
    
    CMceVideoStream* recordingStream = 
        MusEngMceUtils::GetRecordingStream( *iSession );
    
    if ( recordingStream &&
         recordingStream == &aStream &&
         aStream.State() == CMceMediaStream::ENoResources &&
         aSink.IsEnabled() == EFalse )
        {
        InformObserverAboutSessionFailure( MLcSession::EDiskFull );
        }
    else
        {
        // Cannot handle, forward to a base class
        CMusEngMceSession::StreamStateChanged( aStream, aSink );
        }
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngLiveSession::StreamStateChanged( sink )" )
    }
