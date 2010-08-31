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
* Description:  Utilities to locate and manipulate MCE sessions, streams,
*                sources and sinks
*
*/



#ifndef MUSENGMCEUTILS_H
#define MUSENGMCEUTILS_H


#include <mcemediastream.h>
#include <mcemediasink.h>
#include <mcemediasource.h>


class CMceSession;
class CMceDisplaySink;
class CMceCameraSource;
class CMceFileSource;
class CMceVideoStream;
class CMceSpeakerSink;

const TUint KMusEngNoAssociatedSourceType = KMaxTUint;

/**
* Static class including utility methods to locate and manipulate MCE 
* sessions, streams, sources and sinks
* @lib musengine.lib
*/
class MusEngMceUtils
    {

    public:
        
        /**
        * Tells if a stream is a video stream with rtp source.
        *
        * @param aStream Stream to be investigated
        * @return ETrue if parameter is a video stream with a RTP source
        */
        static TBool IsVideoInStream( CMceMediaStream& aStream );

        /**
        * Tells if a stream is a video stream with rtp sink.
        *
        * @param aStream Stream to be investigated
        * @return ETrue if parameter is a video stream with a RTP sink
        */
        static TBool IsVideoOutStream( CMceMediaStream& aStream );
        
        /**
        * Tells if a stream is an audio stream with rtp source.
        *
        * @param aStream Stream to be investigated
        * @return ETrue if parameter is an audio stream with a RTP source
        */
        static TBool IsAudioInStream( CMceMediaStream& aStream );

        /**
        * Gets handle to video stream with RTP sink. 
        * Ownership is not transferred.
        *
        * @return Outgoing video stream
        * @leave KErrNotFound if none exists
        * @leave KErrOverflow if session contains more than one outgoing
        *        video stream
        */
        static CMceVideoStream* GetVideoOutStreamL( CMceSession& aSession );

        /**
        * Gets handle to video stream with RTP source.
        * Ownership is not transferred.
        *
        * @return Incoming video stream
        * @leave KErrNotFound if none exists
        * @leave KErrOverflow if session contains more than one incoming
        *        video stream
        */
        static CMceVideoStream* GetVideoInStreamL( CMceSession& aSession );

        /**
        * Gets handle to video stream with file sink. 
        * Ownership is not transferred.
        *
        * @return Recording video stream, NULL if none exists
        */
        static CMceVideoStream* GetRecordingStream( CMceSession& aSession );
        
        /**
        * Gets handle to a media sink of spesified type contained by
        * a mediastream. If there are several sink instances, first one is 
        * returned. If associated source type is defined, sink in stream having
        * the source is tried to be searched. If no such match, first matching 
        * sink is returned. Ownership is not transferred.
        *
        * @return Media sink of the spesified type. NULL if none exists
        */
        static CMceMediaSink* GetMediaSink( 
                CMceMediaStream& aStream, 
                TMceSinkType aType,
                TMceSourceType aAssociatedSourceType = KMusEngNoAssociatedSourceType );
        
        /**
        * Gets handle to a media sink of spesified type contained by
        * a mediastream. If there are several sink instances, first one is 
        * returned. If associated source type is defined, sink in stream having
        * the source is tried to be searched. If no such match, first matching 
        * sink is returned. Ownership is not transferred.
        *
        * @return Media sink of the spesified type.
        * @leave KErrNotFound if there is no sink of spesified type
        */
        static CMceMediaSink* GetMediaSinkL( 
                CMceMediaStream& aStream, 
                TMceSinkType aType,
                TMceSourceType aAssociatedSourceType = KMusEngNoAssociatedSourceType );

        /**
        * Gets handle to a media sink of spesified type contained by
        * a session. If there are several sink instances, first one is 
        * returned. If associated source type is defined, sink in stream having
        * the source is tried to be searched. If no such match, first matching 
        * sink is returned. Ownership is not transferred.
        *
        * @return Media sink of the spesified type. NULL if none exists
        */
        static CMceMediaSink* GetMediaSink( 
                CMceSession& aSession, 
                TMceSinkType aType,
                TMceSourceType aAssociatedSourceType = KMusEngNoAssociatedSourceType,
                TBool aStrictMatch = EFalse );
        
        /**
        * Gets handle to a media sink of spesified type contained by
        * a session. If there are several sink instances, first one is 
        * returned. If associated source type is defined, sink in stream having
        * the source is tried to be searched. If no such match, first matching 
        * sink is returned. Ownership is not transferred.
        *
        * @return Media sink of the spesified type.
        * @leave KErrNotFound if there is no sink of spesified type
        */
        static CMceMediaSink* GetMediaSinkL( 
                CMceSession& aSession, 
                TMceSinkType aType,
                TMceSourceType aAssociatedSourceType = KMusEngNoAssociatedSourceType,
                TBool aStrictMatch = EFalse );
        
        /**
        * Gets handle to a speaker sink contained by specified stream 
        * or its' bound stream. Ownership is not transferred.
        *
        * @return Speaker sink, NULL if none exists
        */
        static CMceSpeakerSink* GetSpeaker( CMceMediaStream& aStream );        
                                                                                  
        /**
        * Gets handle to a camera source. Ownership is not transferred.
        *
        * @return Camera source
        * @leave KErrNotFound if none exists
        */
        static CMceCameraSource* GetCameraL( CMceSession& aSession );        

        /**
        * Gets handle to a file source. Ownership is not transferred.
        *
        * @return File source
        * @leave KErrNotFound if none exists 
        */
        static CMceFileSource* GetFileSourceL( CMceSession& aSession );  
 
        /**
        * Gets handle to a display sink. Ownership is not transferred.
        *
        * @return Display sink, NULL if none exists
        */
        static CMceDisplaySink* GetDisplay( CMceSession& aSession,
                                            TBool aPreferViewFinder = ETrue );        

        /**
        * Gets handle to a display sink. If there are several display
        * sink instances view finder is preferred with default arguments.
        * Ownership is not transferred.
        *
        * @return Display sink
        * @leave KErrNotFound if there is no display
        */
        static CMceDisplaySink* GetDisplayL( CMceSession& aSession, 
                                             TBool aPreferViewFinder = ETrue );     

        /**
        * Gets handle to a display sink. Ownership is not transferred.
        *
        * @return Display sink, NULL if none exists
        */
        static CMceDisplaySink* GetReceivingDisplay( CMceSession& aSession );          
        
        /**
        * Gets handle to a receiving display sink. Ownership is not transferred.
        *
        * @return Display sink
        * @leave KErrNotFound if there is no display
        */
        static CMceDisplaySink* GetReceivingDisplayL( CMceSession& aSession );    
        
        /**
        * Gets handle to a viewfinder display sink. Ownership is not transferred.
        *
        * @return Display sink
        * @leave KErrNotFound if there is no display
        */
        static CMceDisplaySink* GetVfDisplay( CMceSession& aSession );
        
        /**
        * Adds display sink to specified stream if one does not exist already.
        * Display rect is set in both cases to a specified one.
        * Display is disabled by default.
        * 
        * @param aStream is a stream where display is supposed to be added
        * @param aManager reference to used MCE manager
        * @param aDisplayRect is desired display area
        * @leave KErrArgument if offered stream is not a video stream
        */
        static void AddDisplayL( CMceMediaStream& aStream, 
                                 CMceManager& aManager, 
                                 const TRect& aDisplayRect );

        /**
        * Adds speaker sink to specified stream if one does not exist already.
        * Speaker is disabled by default.
        * 
        * @param aStream is a stream where speaker is supposed to be added
        * @leave KErrArgument if offered stream is not a video stream
        */
        static void AddSpeakerL( CMceMediaStream& aStream );
        
        /**
        * Disables a specified stream, its' source and all the sinks
        * 
        * @param aStream is a stream to be disabled
        */
        static void DisableStreamL( CMceMediaStream& aStream );
        
        /**
        * Enables or disables display sink. If the state is already correct,
        * nothing is done.
        */
        static void DoEnableDisplayL( CMceDisplaySink& aDisplay, TBool aEnable );
        
    };


#endif // MUSENGMCEUTILS_H
