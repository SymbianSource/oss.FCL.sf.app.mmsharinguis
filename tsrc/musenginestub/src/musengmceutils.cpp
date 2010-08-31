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
// Tells if parameter stream is a video stream with RTP sink
// -----------------------------------------------------------------------------
//
TBool MusEngMceUtils::IsVideoOutStream( CMceMediaStream& aStream )
    {
    TBool isOutStream( EFalse );
    if ( aStream.Type() == KMceVideo )
        {
        CMceMediaSink* rtpSink = MusEngMceUtils::GetMediaSink(
                                                   aStream,
                                                   KMceRTPSink );
        
        isOutStream = ( rtpSink != NULL );
        }
    return isOutStream;
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
        if ( MusEngMceUtils::IsVideoOutStream( *streams[i] ) )      
            {
            __ASSERT_ALWAYS( !videoOut, User::Leave( KErrOverflow ) );
            
            videoOut = static_cast<CMceVideoStream*>( streams[i] );
            }
            
            // Check if bound stream is a video stream with RTP sink.
        if ( streams[i]->BoundStream() &&
             MusEngMceUtils::IsVideoOutStream( streams[i]->BoundStreamL() ) )
            {
            __ASSERT_ALWAYS( !videoOut, User::Leave( KErrOverflow ) );
            
            videoOut = static_cast<CMceVideoStream*>( 
                                   &streams[i]->BoundStreamL() );
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
                                             TMceSinkType aType,
                                             TMceSourceType aAssociatedSourceType )
    {
    const RPointerArray<CMceMediaSink>& sinks = aStream.Sinks();
    for ( TInt i = 0; i < sinks.Count(); ++i )
        {
        if ( sinks[i]->Type() == aType && 
           ( aAssociatedSourceType == KMusEngNoAssociatedSourceType || 
             aStream.Source()->Type() == aAssociatedSourceType ) )
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
                                              TMceSinkType aType,
                                              TMceSourceType aAssociatedSourceType )
    {

    CMceMediaSink* sink = MusEngMceUtils::GetMediaSink( aStream, aType, aAssociatedSourceType );

    __ASSERT_ALWAYS( sink, User::Leave( KErrNotFound ) );

    return sink;
    }


// -----------------------------------------------------------------------------
// Gets handle to a media sink of spesified type contained by a session.
// -----------------------------------------------------------------------------
//
CMceMediaSink* MusEngMceUtils::GetMediaSink( CMceSession& aSession,
                                             TMceSinkType aType,
                                             TMceSourceType aAssociatedSourceType,
                                             TBool aStrictMatch )
    {
    CMceMediaSink* sink = NULL;

    TRAP_IGNORE( sink = MusEngMceUtils::GetMediaSinkL( 
            aSession, aType, aAssociatedSourceType, aStrictMatch ) )

    return sink;
    }


// -----------------------------------------------------------------------------
// Gets handle to a media sink of spesified type contained by a session.
// -----------------------------------------------------------------------------
//
CMceMediaSink* MusEngMceUtils::GetMediaSinkL( CMceSession& aSession,
                                              TMceSinkType aType,
                                              TMceSourceType aAssociatedSourceType,
                                              TBool aStrictMatch )
    {
    CMceMediaSink* sink = NULL;
    
    const RPointerArray<CMceMediaStream>& streams = aSession.Streams();

    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        sink = MusEngMceUtils::GetMediaSink( *streams[i], aType, aAssociatedSourceType );
        if ( sink )
            {
            return sink;
            }
        
        if ( streams[i]->BoundStream() )
            {
            sink = MusEngMceUtils::GetMediaSink( streams[i]->BoundStreamL(), 
                                                 aType,
                                                 aAssociatedSourceType );
            if ( sink )
                {
                return sink;
                }
            }
        }
    
    if ( !sink && aAssociatedSourceType != KMusEngNoAssociatedSourceType && !aStrictMatch )
        {
        // No preferred match, try without source preference
        sink = GetMediaSinkL( aSession, aType );
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
CMceDisplaySink* MusEngMceUtils::GetDisplay( 
    CMceSession& aSession, TBool aPreferViewFinder  )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetDisplay()" )
    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetDisplay()" )
    TMceSourceType preferredSource = 
        aPreferViewFinder ? KMceCameraSource : KMusEngNoAssociatedSourceType;
    return static_cast<CMceDisplaySink*>(
            MusEngMceUtils::GetMediaSink( aSession, KMceDisplaySink, preferredSource ) );

    }


// -----------------------------------------------------------------------------
// Gets handle to a display sink.
// -----------------------------------------------------------------------------
//
CMceDisplaySink* MusEngMceUtils::GetDisplayL( 
    CMceSession& aSession, TBool aPreferViewFinder )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetDisplayL()" )

    CMceDisplaySink* display = MusEngMceUtils::GetDisplay( aSession, aPreferViewFinder );

    __ASSERT_ALWAYS( display, User::Leave( KErrNotFound ) );

    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetDisplayL()" )
    return display;
    }

// -----------------------------------------------------------------------------
// Gets handle to a display sink displaying received video.
// -----------------------------------------------------------------------------
//
CMceDisplaySink* MusEngMceUtils::GetReceivingDisplay( CMceSession& aSession )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetReceivingDisplay()" )
    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetReceivingDisplay()" )
    
    // Search display which is connected with rtp source
    TMceSourceType preferredSource = KMceRTPSource;
    return static_cast<CMceDisplaySink*>( MusEngMceUtils::GetMediaSink( 
                aSession, KMceDisplaySink, preferredSource, ETrue ) );
    }

// -----------------------------------------------------------------------------
// Gets handle to a display sink displaying received video.
// -----------------------------------------------------------------------------
//
CMceDisplaySink* MusEngMceUtils::GetReceivingDisplayL( CMceSession& aSession )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetReceivingDisplayL()" )

    CMceDisplaySink* display = GetReceivingDisplay( aSession );
    __ASSERT_ALWAYS( display != NULL, User::Leave( KErrNotFound ) );
    
    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetReceivingDisplay()" )
    
    return display;
    }

// -----------------------------------------------------------------------------
// Gets handle to a display sink displaying viewfinder content.
// -----------------------------------------------------------------------------
//
CMceDisplaySink* MusEngMceUtils::GetVfDisplay( CMceSession& aSession )
    {
    MUS_LOG( "mus: [ENGINE]  -> MusEngMceUtils::GetVfDisplay()" )
    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::GetVfDisplay()" )
    
    // Search display which is connected with camera
    TMceSourceType preferredSource = KMceCameraSource;
    return static_cast<CMceDisplaySink*>( MusEngMceUtils::GetMediaSink( 
                aSession, KMceDisplaySink, preferredSource, ETrue ) );
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

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void MusEngMceUtils::DoEnableDisplayL( CMceDisplaySink& aDisplay, TBool aEnable )
    {
    MUS_LOG1( "mus: [ENGINE]     -> MusEngMceUtils::DoEnableDisplayL() %d", 
              aEnable )
    
    if ( aEnable )
        {
        if ( !aDisplay.IsEnabled() )
            {
            aDisplay.EnableL();
            MUS_LOG( "                  Display enabled" )
            }
        else
            {
            MUS_LOG( "                  Display already enabled, ignore" )
            }
        }
    else
        {
        if ( aDisplay.IsEnabled() )
            {
            aDisplay.DisableL();
            MUS_LOG( "                  Display disabled" )
            }
        else
            {
            MUS_LOG( "                  Display already disabled, ignore" )
            }
        }  
        
    MUS_LOG( "mus: [ENGINE]  <- MusEngMceUtils::DoEnableDisplayL()")
    }

