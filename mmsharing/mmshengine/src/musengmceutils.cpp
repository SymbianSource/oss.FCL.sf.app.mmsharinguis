/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Utilities to get and set profile used with SWIS.
*
*/



// USER

#include "musengmceutils.h"
#include "muslogger.h"

// SYSTEM

#include <mcesession.h>
#include <mcevideostream.h>
#include <mceaudiostream.h>
#include <mcertpsink.h>
#include <mcemediasource.h>
#include <mcertpsource.h>
#include <mcecamerasource.h>
#include <mcedisplaysink.h>
#include <mcefilesource.h>
#include <mcefilesink.h>
#include <mcespeakersink.h>





// -----------------------------------------------------------------------------
// Tells if parameter stream is a video stream with RTP source
// -----------------------------------------------------------------------------
//
TBool MusEngMceUtils::IsVideoInStream( CMceMediaStream& aStream )
    {
    return ( aStream.Type() == KMceVideo &&
             aStream.Source() && 
             aStream.Source()->Type() == KMceRTPSource );
    }


// -----------------------------------------------------------------------------
// Tells if parameter stream is an audio stream with RTP source
// -----------------------------------------------------------------------------
//
TBool MusEngMceUtils::IsAudioInStream( CMceMediaStream& aStream )
    {
    return ( aStream.Type() == KMceAudio &&
             aStream.Source() && 
             aStream.Source()->Type() == KMceRTPSource );
    }
    

// -----------------------------------------------------------------------------
// Gets handle to video stream with RTP sink.
// -----------------------------------------------------------------------------
//
CMceVideoStream* MusEngMceUtils::GetVideoOutStreamL( CMceSession& aSession )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetVideoOutStreamL()" )

    const RPointerArray<CMceMediaStream>& streams = aSession.Streams();

    CMceVideoStream* videoOut = NULL;

    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( streams[i]->Type() == KMceVideo )
            {
            CMceMediaSink* rtpSink = MusEngMceUtils::GetMediaSink(
                                            *streams[i],
                                            KMceRTPSink );
            if ( rtpSink )
                {
                __ASSERT_ALWAYS( !videoOut, User::Leave( KErrOverflow ) );

                videoOut = static_cast<CMceVideoStream*>( streams[i] );
                }
            
            // There is no need to investigate bound stream since
            // outstream is always constructed by Mus instead of MCE
            }
        }

    __ASSERT_ALWAYS( videoOut, User::Leave( KErrNotFound ) );

    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetVideoOutStreamL()" )

    return videoOut;
    }


// -----------------------------------------------------------------------------
// Gets handle to video stream with RTP source.
// -----------------------------------------------------------------------------
//
CMceVideoStream* MusEngMceUtils::GetVideoInStreamL( CMceSession& aSession )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetVideoInStreamL()" )

    const RPointerArray<CMceMediaStream>& streams = aSession.Streams();

    CMceVideoStream* videoIn = NULL;

    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( MusEngMceUtils::IsVideoInStream( *streams[i] ) )
            
            {
            __ASSERT_ALWAYS( !videoIn, User::Leave( KErrOverflow ) );

            videoIn = static_cast<CMceVideoStream*>( streams[i] );
            }
            
        // Check if bound stream is a video stream with RTP souce.
        if ( streams[i]->BoundStream() &&
             MusEngMceUtils::IsVideoInStream( streams[i]->BoundStreamL() ) )
            {
            __ASSERT_ALWAYS( !videoIn, User::Leave( KErrOverflow ) );

            videoIn = static_cast<CMceVideoStream*>( 
                                    &streams[i]->BoundStreamL() );
            }       
        }

    __ASSERT_ALWAYS( videoIn, User::Leave( KErrNotFound ) );

    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetVideoInStreamL()" )

    return videoIn;
    }


// -----------------------------------------------------------------------------
// Gets handle to video stream with file sink
// -----------------------------------------------------------------------------
//
CMceVideoStream* MusEngMceUtils::GetRecordingStream( CMceSession& aSession )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetRecordingStreamL()" )
    
    const RPointerArray<CMceMediaStream>& streams = aSession.Streams();

    CMceVideoStream* recordingStream = NULL;

    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( streams[i]->Type() == KMceVideo )
            {
            if ( MusEngMceUtils::GetMediaSink( *streams[i], KMceFileSink ) )
                {
                recordingStream = static_cast<CMceVideoStream*>( streams[i] );
                }
            }
        }
    
    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetRecordingStreamL()" )
    
    return recordingStream;
    }


// -----------------------------------------------------------------------------
// Gets handle to a media sink of spesified type contained by a mediastream.
// -----------------------------------------------------------------------------
//
CMceMediaSink* MusEngMceUtils::GetMediaSink( CMceMediaStream& aStream,
                                             TMceSinkType aType )
    {
    const RPointerArray<CMceMediaSink>& sinks = aStream.Sinks();
    for ( TInt i = 0; i < sinks.Count(); ++i )
        {
        if ( sinks[i]->Type() == aType )
            {
            return sinks[i];
            }
        }
        
    return NULL;
    }


// -----------------------------------------------------------------------------
// Gets handle to a media sink of spesified type contained by a mediastream.
// -----------------------------------------------------------------------------
//
CMceMediaSink* MusEngMceUtils::GetMediaSinkL( CMceMediaStream& aStream,
                                              TMceSinkType aType )
    {

    CMceMediaSink* sink = MusEngMceUtils::GetMediaSink( aStream, aType );

    __ASSERT_ALWAYS( sink, User::Leave( KErrNotFound ) );

    return sink;
    }


// -----------------------------------------------------------------------------
// Gets handle to a media sink of spesified type contained by a session.
// -----------------------------------------------------------------------------
//
CMceMediaSink* MusEngMceUtils::GetMediaSink( CMceSession& aSession,
                                             TMceSinkType aType )
    {
    CMceMediaSink* sink = NULL;

    TRAP_IGNORE( sink = MusEngMceUtils::GetMediaSinkL( aSession, aType ) )

    return sink;
    }


// -----------------------------------------------------------------------------
// Gets handle to a media sink of spesified type contained by a session.
// -----------------------------------------------------------------------------
//
CMceMediaSink* MusEngMceUtils::GetMediaSinkL( CMceSession& aSession,
                                              TMceSinkType aType )
    {
    CMceMediaSink* sink = NULL;
    
    const RPointerArray<CMceMediaStream>& streams = aSession.Streams();

    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        sink = MusEngMceUtils::GetMediaSink( *streams[i], aType );
        if ( sink )
            {
            return sink;
            }
        
        if ( streams[i]->BoundStream() )
            {
            sink = MusEngMceUtils::GetMediaSink( streams[i]->BoundStreamL(), 
                                                 aType );
            if ( sink )
                {
                return sink;
                }
            }
        }

    __ASSERT_ALWAYS( sink, User::Leave( KErrNotFound ) );

    return sink;
    }


// -----------------------------------------------------------------------------
// Gets handle to a speaker sink contained by specified stream or bound stream.
// -----------------------------------------------------------------------------
//
CMceSpeakerSink* MusEngMceUtils::GetSpeaker( CMceMediaStream& aStream )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetSpeaker()" )

    CMceSpeakerSink* speaker = NULL;
    
    if ( aStream.Type() == KMceAudio )
        {
        speaker = static_cast<CMceSpeakerSink*>(
                    MusEngMceUtils::GetMediaSink( aStream, KMceSpeakerSink ) );
    
        if ( !speaker && aStream.BoundStream() )
            {
            CMceMediaStream* boundStream = NULL;
            TRAPD( error, boundStream = &aStream.BoundStreamL() )
            
            if ( error == KErrNone )
                {
                speaker = static_cast<CMceSpeakerSink*>(
                            MusEngMceUtils::GetMediaSink( *boundStream, 
                                                          KMceSpeakerSink ) );
                }
            }
        }
    
    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetSpeaker()" )

    return speaker;
    }
    

// -----------------------------------------------------------------------------
// Gets handle to a camera source.
// -----------------------------------------------------------------------------
//
CMceCameraSource* MusEngMceUtils::GetCameraL( CMceSession& aSession )
    {
    // Camera can be only in out stream
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetCameraL()" )

    CMceVideoStream* videoOut = MusEngMceUtils::GetVideoOutStreamL( aSession );

    if ( !( videoOut->Source() &&
            videoOut->Source()->Type() == KMceCameraSource ) )
        {
        User::Leave( KErrNotFound );
        }

    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetCameraL()" )
    return static_cast<CMceCameraSource*>( videoOut->Source() );
    }


// -----------------------------------------------------------------------------
// Gets handle to a file source.
// -----------------------------------------------------------------------------
//
CMceFileSource* MusEngMceUtils::GetFileSourceL( CMceSession& aSession )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetFileSourceL()" )

    CMceVideoStream* videoOut = MusEngMceUtils::GetVideoOutStreamL( aSession );

    if ( !( videoOut->Source() &&
            videoOut->Source()->Type() == KMceFileSource ) )
        {
        User::Leave( KErrNotFound );
        }

    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetFileSourceL()" )
    return static_cast<CMceFileSource*>( videoOut->Source() );
    }


// -----------------------------------------------------------------------------
// Gets handle to a display sink.
// -----------------------------------------------------------------------------
//
CMceDisplaySink* MusEngMceUtils::GetDisplay( CMceSession& aSession )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetDisplay()" )
    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetDisplay()" )
    return static_cast<CMceDisplaySink*>(
            MusEngMceUtils::GetMediaSink( aSession, KMceDisplaySink ) );

    }


// -----------------------------------------------------------------------------
// Gets handle to a display sink.
// -----------------------------------------------------------------------------
//
CMceDisplaySink* MusEngMceUtils::GetDisplayL( CMceSession& aSession )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetDisplayL()" )

    CMceDisplaySink* display = MusEngMceUtils::GetDisplay( aSession );

    __ASSERT_ALWAYS( display, User::Leave( KErrNotFound ) );

    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetDisplayL()" )
    return display;
    }


// -----------------------------------------------------------------------------
// Adds display sink to specified stream if one does not exist already.
// Display rect is set in both cases.
// -----------------------------------------------------------------------------
//
void MusEngMceUtils::AddDisplayL( CMceMediaStream& aStream, 
                                  CMceManager& aManager,
                                  const TRect& aDisplayRect )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::AddDisplayL()" )
    
    __ASSERT_ALWAYS( aStream.Type() == KMceVideo, User::Leave( KErrArgument ) );

    CMceDisplaySink* display = static_cast<CMceDisplaySink*>(
                                    MusEngMceUtils::GetMediaSink( 
                                                            aStream,
                                                            KMceDisplaySink ) );

    if ( !display )
        {
        display = CMceDisplaySink::NewLC( aManager );
        aStream.AddSinkL( display );
        CleanupStack::Pop( display );
        }

    display->SetDisplayRectL( aDisplayRect );
    
    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::AddDisplayL()" )
    }


// -----------------------------------------------------------------------------
// Adds display sink to specified stream if one does not exist already.
// -----------------------------------------------------------------------------
//
void MusEngMceUtils::AddSpeakerL( CMceMediaStream& aStream )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::AddSpeakerL()" )
    
    __ASSERT_ALWAYS( aStream.Type() == KMceAudio, User::Leave( KErrArgument ) );

    CMceSpeakerSink* speaker = static_cast<CMceSpeakerSink*>(
                                    MusEngMceUtils::GetMediaSink( 
                                                            aStream,
                                                            KMceSpeakerSink ) );

    if ( !speaker )
        {
        speaker = CMceSpeakerSink::NewLC();
        aStream.AddSinkL( speaker );
        CleanupStack::Pop( speaker );
        }
    
    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::AddSpeakerL()" )
    }


// -----------------------------------------------------------------------------
// Disables parameter stream, its' source and all the sinks.
// -----------------------------------------------------------------------------
//
void MusEngMceUtils::DisableStreamL( CMceMediaStream& aStream )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::DisableStreamL()" )
    
    aStream.DisableL();
    
    if ( aStream.Source() )
        {
        aStream.Source()->DisableL();
        }
        
    for ( TInt sinkIndex = 0; sinkIndex < aStream.Sinks().Count(); ++sinkIndex )
        {
        aStream.Sinks()[ sinkIndex ]->DisableL();
        }
        
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::DisableStreamL()" )
    }







