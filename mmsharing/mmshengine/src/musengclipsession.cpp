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
#include "musengclipsession.h"
#include "musengmceoutsession.h"
#include "musenglivesession.h"
#include "musengmceutils.h"
#include "musenglogger.h"
#include "musengclipvideoplayer.h"

// SYSTEM
#include <lcsessionobserver.h>
#include <mcemanager.h>
#include <mcesession.h>
#include <mcestreambundle.h>
#include <mcefilesource.h>
#include <mcemediastream.h>
#include <mcevideostream.h>
#include <mceaudiostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <mceamrcodec.h>
#include <mcevideocodec.h>
#include <DRMCommon.h>


// CONSTANTS
const TInt KMusEngAmrBitRate = KMceAmrNbBitrate475;
const TUint KMusEngAllowedAmrBitrates = KMceAllowedAmrNbBitrate475;


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngClipSession* CMusEngClipSession::NewL()
    {
    CMusEngClipSession* self = new( ELeave )CMusEngClipSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngClipSession::CMusEngClipSession()
    : CMusEngMceOutSession()
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::ConstructL(...)" )

    CMusEngMceOutSession::ConstructL();
    
    iClipVideoPlayer = CMusEngClipVideoPlayer::NewL( *this, *this  );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::ConstructL(...)" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngClipSession::~CMusEngClipSession()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::~CMusEngClipSession()" )
    
    delete iClipVideoPlayer;
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::~CMusEngClipSession()" )
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngClipSession::LocalVideoPlayer()
    {
    return iClipVideoPlayer;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::CompleteSessionStructureL(
    CMceStreamBundle& aLocalBundle )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::CompleteSessionStructureL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    // Create outgoing video stream
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    
    CMceRtpSink* rtpsink = CMceRtpSink::NewLC();
    videoStream->AddSinkL( rtpsink );
    CleanupStack::Pop( rtpsink );

    CMceFileSource* fileSource = 
        CMceFileSource::NewLC( *iManager, iClipVideoPlayer->LcFileName() );
    fileSource->DisableL(); // Start session in pause mode.
    videoStream->SetSourceL( fileSource );
    CleanupStack::Pop( fileSource );

    iSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );

    ConstructAudioStructureL( aLocalBundle );
    
    iClipVideoPlayer->SetMceSession( iSession );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::CompleteSessionStructureL()" )
    }

// -----------------------------------------------------------------------------
// Checks that that there is no need for transcoding before calling
// overridden base class variant of this function.
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::EstablishSessionL()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::EstablishSessionL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    const RPointerArray<CMceMediaStream>& streams = iSession->Streams();

    if ( iVideoCodecList )
        {
        MUS_LOG_TDESC8( "iVideoCodecList: ", iVideoCodecList->Des() )
        }
    
    CMceVideoStream* videoStream = NULL;
    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( streams[i]->State() == CMceMediaStream::ETranscodingRequired )
            {
            User::Leave( KErrNotSupported );
            }
        else if ( streams[i]->Type() == KMceVideo &&
                  !IsH264Supported() )
            {
            MUS_LOG( "                -> video stream found!!!" )
            videoStream = static_cast<CMceVideoStream*>( streams[i] );
            
            //transcoding of H264 is not needed only if we know explicitly
            //that the peer supports it (from OPTIONS response)
                            
            const RPointerArray<CMceVideoCodec>& codecs = videoStream->Codecs();
            for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
                {
                if ( codecs[codecIndex]->SdpName() == KMceSDPNameH264() )                     
                    {
                    User::Leave( KErrNotSupported );
                    }
                }            
            } 
        }

    CMusEngMceOutSession::EstablishSessionL();
    // Now session state is right to adjust volume
    SetSpeakerVolumeL( LcVolumeL() );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::EstablishSessionL()" )
    }

// -----------------------------------------------------------------------------
// Implemented for MMceStreamObserver
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::StreamStateChanged( CMceMediaStream& aStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::StreamStateChanged()" )
    
    if ( !iSession )
        {
        return;
        }
        
    DetermineBufferingPeriod( aStream );
    
    CMusEngMceSession::StreamStateChanged( aStream );
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::StreamStateChanged()" )
    }

// -----------------------------------------------------------------------------
// Implemented for MMceStreamObserver
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::StreamStateChanged( CMceMediaStream& aStream,
                                             CMceMediaSource& aSource )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::StreamStateChanged( src )" )
    
    if ( !iSession )
        {
        return;
        }

    MUS_ENG_LOG_STREAM_STATE( aStream )
    
    DetermineBufferingPeriod( aStream );

    if ( iClipVideoPlayer->HasClipEnded() )
        {
        MUS_LOG( "mus: [ENGINE]     Clip ended." )
        InformObserverAboutPlayerStateChange( iClipVideoPlayer );
        }
    else
        {
        // Cannot handle, forward to the ancestor class
        CMusEngMceSession::StreamStateChanged( aStream, aSource );
        }

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::StreamStateChanged( src )" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::AddAmrCodecL( CMceAudioStream& aAudioStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::AddAmrCodecL" )

    // Remove old codecs
    while ( aAudioStream.Codecs().Count() > 0 )
        {
        aAudioStream.RemoveCodecL( *aAudioStream.Codecs()[0] );
        }

    // Create AMR codec instance
    const RPointerArray<const CMceAudioCodec>& supportedCodecs =
        iManager->SupportedAudioCodecs();

    CMceAudioCodec* amr = NULL;

    for ( TInt i = 0; i < supportedCodecs.Count(); ++i )
        {
        if ( supportedCodecs[i]->SdpName() == KMceSDPNameAMR() )
            {
            amr = supportedCodecs[i]->CloneL();
            CleanupStack::PushL( amr );
            User::LeaveIfError( 
                amr->SetAllowedBitrates( KMusEngAllowedAmrBitrates ) );
            User::LeaveIfError( amr->SetBitrate( KMusEngAmrBitRate ) );
            aAudioStream.AddCodecL( amr );
            CleanupStack::Pop( amr );
            break; // We must have only one codec
            }
        }

    __ASSERT_ALWAYS( amr, User::Leave( KErrNotFound ) );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::AddAmrCodecL" )
    }

// -----------------------------------------------------------------------------
// Create codec instance, H264 is used only if other end supports it for sure,
// otherwise H263 is used.
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::AddVideoCodecL( CMceVideoStream& aVideoStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::AddVideoCodecL" )

    // Remove old codecs

    while ( aVideoStream.Codecs().Count() > 0 )
        {
        aVideoStream.RemoveCodecL( *aVideoStream.Codecs()[0] );
        }

    const RPointerArray< const CMceVideoCodec >& supportedCodecs =
        iManager->SupportedVideoCodecs();

    CMceVideoCodec* addedCodec = NULL;
    
    TPtrC8 addedCodecName = 
            IsH264Supported() ? KMceSDPNameH264() : KMceSDPNameH2632000();
    
    MUS_LOG_TDESC8( "mus: [ENGINE] adding codec : ", addedCodecName ); 
            
    for ( TInt i = 0; i < supportedCodecs.Count(); ++i )
        {
        if ( supportedCodecs[i]->SdpName() == addedCodecName )
            {
            addedCodec = supportedCodecs[i]->CloneL();
            CleanupStack::PushL( addedCodec );
            aVideoStream.AddCodecL( addedCodec );
            CleanupStack::Pop( addedCodec );
            break; // We must have only one codec
            }
        }

    __ASSERT_ALWAYS( addedCodec, User::Leave( KErrNotFound ) );

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::AddVideoCodecL" )
    }

// -----------------------------------------------------------------------------
// If member file contains audio, add appropriate amount of audio streams to
// session structure
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::ConstructAudioStructureL(
    CMceStreamBundle& aLocalBundle )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::ConstructAudioStructureL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    // There is no clip audio present in operator variant
    if ( iOperatorVariant )
        {
        MUS_LOG( "                 Operator variant, no audio constructed" )
        MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::ConstructAudioStructureL()" )
        return;
        }

    CMceFileSource* fileSource = MusEngMceUtils::GetFileSourceL( *iSession );

    TInt audioElementCount = fileSource->MediaElementCountL( KMceAudio );

    MUS_LOG1( "mus: [ENGINE]    File contains %d audio elements",
              audioElementCount )

    for ( TInt i = 0; i < audioElementCount; ++i )
        {
        fileSource->SetCurrentMediaElementL( KMceAudio, i );

        MUS_LOG1( "mus: [ENGINE]     Current audio element set to : %d ", i )

        // Set up an audio outstream.
        CMceAudioStream* audioOut = CMceAudioStream::NewLC();

        CMceRtpSink* rtpSink = CMceRtpSink::NewLC();  
        audioOut->AddSinkL( rtpSink );
        CleanupStack::Pop( rtpSink );

        audioOut->SetSourceL( fileSource );

        iSession->AddStreamL( audioOut );
        CleanupStack::Pop( audioOut );

        MUS_LOG( "mus: [ENGINE]     Audio outstream created" )

        // Set up an audio stream to local speaker
        audioOut = CMceAudioStream::NewLC();
        
        MusEngMceUtils::AddSpeakerL( *audioOut );

        audioOut->SetSourceL( fileSource );

        iSession->AddStreamL( audioOut );
        CleanupStack::Pop( audioOut );

        MUS_LOG( "mus: [ENGINE]     Local audio stream created" )

        aLocalBundle.AddStreamL( *audioOut );

        MUS_LOG( "mus: [ENGINE]     Local audio stream added to bundle" )
        }

    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::ConstructAudioStructureL()" )
    }

// -----------------------------------------------------------------------------
// Calculates how long MCE buffers based on time between buffering and streaming
// events. 
// -----------------------------------------------------------------------------
//
void CMusEngClipSession::DetermineBufferingPeriod( CMceMediaStream& aStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngClipSession::DetermineBufferingPeriod()" )
    
    // Determine file position modifier from time difference between buffering
    // and streaming events
    if ( aStream.State() == CMceMediaStream::EBuffering )
        {
        iBufferingStartedTime.HomeTime();
        }
    
    if ( aStream.State() == CMceMediaStream::EStreaming )
        {
        if ( iBufferingStartedTime.Int64() != 0 )
            {
            TTime currentTime;
            currentTime.HomeTime();
            
            TTimeIntervalMicroSeconds bufferingPeriod = 
                currentTime.MicroSecondsFrom( iBufferingStartedTime );
            
            const TInt KMusMinimumBufferingPeriod( 500000 );
            if ( bufferingPeriod > KMusMinimumBufferingPeriod )
                {
                iClipVideoPlayer->SetBufferingPeriod( bufferingPeriod );
                }
                
            MUS_LOG1( "mus: [ENGINE] current buffering period:%d", 
                      bufferingPeriod.Int64() )
            
            iBufferingStartedTime = 0;
            }
        }
        
    MUS_LOG( "mus: [ENGINE] <- CMusEngClipSession::DetermineBufferingPeriod()" )
    }

// End of file
