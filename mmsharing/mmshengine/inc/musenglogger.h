/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/


#ifndef MUSENGLOGGER_H
#define MUSENGLOGGER_H


#include "muslogger.h"

#if ( !_DEBUG || UNIT_TESTING ) // release or unit test build; no logs

#define MUS_ENG_LOG_SESSION_STATE( aSession )
#define MUS_ENG_LOG_SESSION_STATE_AND_STATUS( aSession, aStatusCode, aReasonPhrase )
#define MUS_ENG_LOG_STREAM_STATE( aStream )
#define MUS_ENG_LOG_SINK_STATES( aStream )
#define MUS_ENG_LOG_SOURCE_STATE( aSource )
#define MUS_ENG_LOG_SINK_STATE( aSink )
#define MUS_ENG_LOG_STATUS( aStatusCode, aReasonPhrase )

#else // debug build; logs

#include <mcesession.h>
#include <mceaudiostream.h>
#include <mcecamerasource.h>
#include <mcefilesource.h>
#include <mcertpsource.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <mcefilesink.h>
#include <mcertpsink.h>

#define MUS_ENG_LOG_SESSION_STATE( aSession )\
    {\
    TBuf<15> state;\
    switch ( (aSession).State() )\
        {\
        case CMceSession::EIdle:\
            {\
            state = _L("EIdle");\
            break;\
            }\
        case CMceSession::EOffering :\
            {\
            state = _L("EOffering");\
            break;\
            }\
        case CMceSession::EIncoming:\
            {\
            state = _L("EIncoming");\
            break;\
            }\
        case CMceSession::EReserving :\
            {\
            state = _L("EReserving");\
            break;\
            }\
        case CMceSession::EAnswering :\
            {\
            state = _L("EAnswering");\
            break;\
            }\
        case CMceSession::EProceeding :\
            {\
            state = _L("EProceeding");\
            break;\
            }\
        case CMceSession::EEstablished:\
            {\
            state = _L("EEstablished");\
            break;\
            }\
        case CMceSession::ECancelling:\
            {\
            state = _L("ECancelling");\
            break;\
            }\
        case CMceSession::ETerminating:\
            {\
            state = _L("ETerminating");\
            break;\
            }\
        case CMceSession::ETerminated:\
            {\
            state = _L("ETerminated");\
            break;\
            }\
        default:\
            {\
            state = _L("Unknown");\
            break;\
            }\
        }\
    MUS_LOG_TDESC( "mus: [ENGINE]     Session state is ", state )\
    }


#define MUS_ENG_LOG_SESSION_STATE_AND_STATUS( aSession, aStatusCode, aReasonPhrase )\
    {\
    MUS_ENG_LOG_SESSION_STATE( aSession )\
    MUS_ENG_LOG_STATUS( aStatusCode, aReasonPhrase )\
    }


#define MUS_ENG_LOG_STREAM_STATE( aStream )\
    {\
    TBuf<20> streamState;\
    switch ( (aStream).State() )\
        {\
        case CMceMediaStream::EUninitialized:\
            {\
            streamState = _L("EIdle");\
            break;\
            }\
        case CMceMediaStream::EInitialized:\
            {\
            streamState = _L("EInitialized");\
            break;\
            }\
        case CMceMediaStream::EBuffering:\
            {\
            streamState = _L("EBuffering");\
            break;\
            }\
        case CMceMediaStream::EIdle:\
            {\
            streamState = _L("EIdle");\
            break;\
            }\
        case CMceMediaStream::EStreaming:\
            {\
            streamState = _L("EStreaming");\
            break;\
            }\
        case CMceMediaStream::EDisabled:\
            {\
            streamState = _L("EDisabled");\
            break;\
            }\
        case CMceMediaStream::ENoResources:\
            {\
            streamState = _L("ENoResources");\
            break;\
            }\
        case CMceMediaStream::ETranscodingRequired:\
            {\
            streamState = _L("ETranscodingRequired");\
            break;\
            }\
        case CMceMediaStream::ETranscoding:\
            {\
            streamState = _L("ETranscoding");\
            break;\
            }\
        default:\
            {\
            streamState = _L("Unknown");\
            break;\
            }\
        }\
        if ( (aStream).Type() == KMceAudio )\
            {\
            MUS_LOG_TDESC( "mus: [ENGINE]     AudioStream state is ", streamState )\
            }\
        else\
            {\
            MUS_LOG_TDESC( "mus: [ENGINE]     VideoStream state is ", streamState )\
            }\
        MUS_ENG_LOG_SOURCE_STATE( (aStream).Source() )\
        MUS_ENG_LOG_SINK_STATES( (aStream) )\
    }


#define MUS_ENG_LOG_SINK_STATES( aStream )\
    {\
    for ( TInt sinkIndex = 0; sinkIndex < aStream.Sinks().Count(); ++sinkIndex )\
        {\
        MUS_ENG_LOG_SINK_STATE( aStream.Sinks()[ sinkIndex ] )\
        }\
    }
    

#define MUS_ENG_LOG_SOURCE_STATE( aSource )\
    {\
    if ( aSource )\
        {\
        TBuf<10> sourceState;\
        if ( aSource->IsEnabled() )\
            {\
            sourceState = _L("enabled");\
            }\
        else\
            {\
            sourceState = _L("disabled");\
            }\
        switch ( aSource->Type() )\
            {\
            case KMceCameraSource:\
                {\
                MUS_LOG_TDESC( "                    CameraSource is ", sourceState )\
                break;\
                }\
            case KMceFileSource:\
                {\
                MUS_LOG_TDESC( "                    FileSource is ", sourceState )\
                break;\
                }\
            case KMceRTPSource:\
                {\
                MUS_LOG_TDESC( "                    RtpSource is ", sourceState )\
                break;\
                }\
            default:\
                {\
                MUS_LOG_TDESC( "                    UnknownSource is ", sourceState )\
                break;\
                }\
            }\
        }\
    else\
        {\
        MUS_LOG( "                    No source in stream" )\
        }\
    }

#define MUS_ENG_LOG_SINK_STATE( aSink )\
    {\
    if ( aSink )\
        {\
        TBuf<10> sinkState;\
        if ( aSink->IsEnabled() )\
            {\
            sinkState = _L("enabled");\
            }\
        else\
            {\
            sinkState = _L("disabled");\
            }\
        switch ( aSink->Type() )\
            {\
            case KMceDisplaySink:\
                {\
                MUS_LOG_TDESC( "                    DisplaySink is ", sinkState )\
                break;\
                }\
            case KMceSpeakerSink:\
                {\
                MUS_LOG_TDESC( "                    SpeakerSink is ", sinkState )\
                break;\
                }\
            case KMceRTPSink:\
                {\
                MUS_LOG_TDESC( "                    RtpSink is ", sinkState )\
                break;\
                }\
            case KMceFileSink:\
                {\
                MUS_LOG_TDESC( "                    FileSink is ", sinkState )\
                break;\
                }\
            default:\
                {\
                MUS_LOG_TDESC( "                    UnknownSink is ", sinkState )\
                break;\
                }\
            }\
        }\
    }
    

#define MUS_ENG_LOG_STATUS( aStatusCode, aReasonPhrase )\
    {\
    MUS_LOG1( "                  StatusCode is %d", aStatusCode )\
    HBufC16* reasonPhrase = HBufC16::NewLC( aReasonPhrase.Length() );\
    reasonPhrase->Des().Copy( aReasonPhrase );\
    MUS_LOG_TDESC( "                  ReasonPhrase is ",(*reasonPhrase) )\
    CleanupStack::PopAndDestroy( reasonPhrase );\
    }
    
    

#endif // _DEBUG


#endif // MUSENGLOGGER_H

// End of File
