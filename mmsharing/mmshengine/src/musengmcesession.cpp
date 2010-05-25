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
#include "mussettings.h"
#include "musengmcesession.h"
#include "musengsessiondurationtimer.h"
#include "musengsessionobserver.h"
#include "musengmceutils.h"
#include "musuid.hrh"
#include "musenglogger.h"
#include "musengclipsessionobserver.h"
#include "mussipprofilehandler.h"

// SYSTEM
#include <mcedefs.h>
#include <mcemanager.h>
#include <mceinsession.h>
#include <mcevideostream.h>
#include <mceaudiostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <mcefilesource.h>
#include <mcertpsource.h>
#include <mceaudiocodec.h>
#include <mcevideocodec.h>
#include <AudioPreference.h>


const TInt KMusEngTimerInterval = 1000000; // 1 second
const TInt KMusEngRtcpInactivityThreshold = 20; // seconds
const TInt KMusEngArrayGranularity3 = 3;

const TInt KMusEngSipReasonCodeOk = 200;
const TInt KMusEngSipReasonCodeBusyHere = 486;
_LIT8( KMusEngSipReasonPhraseBusyHere, "Busy Here" );
_LIT8( KMusEngSipReasonPhraseBusy, "Busy" );

const TUint KMusEngDedicatedVideoPort = 49152;
const TUint KMusEngDedicatedAudioPort = 57344;

#define MUS_CODEC_ARR_CONST_CAST( codecArr ) \
( const_cast< RPointerArray< CMceVideoCodec >& >( codecArr ) )

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceSession::~CMusEngMceSession()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::~CMusEngMceSession()" )
    delete iSipProfileHandler;
    delete iSession;
    delete iManager;
    delete iUpdateTimer;
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::~CMusEngMceSession()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::TerminateL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::TerminateL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    iSession->TerminateL();
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::TerminateL()" )
    }


// -----------------------------------------------------------------------------
// Returns estabilished session time. If not established return
// value is < 0
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalSeconds CMusEngMceSession::GetSessionTime() const
    {
    if ( iSession && iSession->State() == CMceSession::EEstablished )
        {
        TTime time;
        TTimeIntervalSeconds seconds;
        time.HomeTime();

        time.SecondsFrom( iStartTime, seconds );

        return seconds;
        }

    return TTimeIntervalSeconds( KErrNotReady );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngMceSession::ConnectionActive() const
    {
    if ( iSession )
        {
        return iSession->ConnectionActive();
        }
    return EFalse;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngMceSession::ContainsAudioL()
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
                                            
    for ( TInt i = 0; i < iSession->Streams().Count(); ++i )
        {
        if ( iSession->Streams()[i]->Type() == KMceAudio )
            {
            return ETrue;
            }
        }

    return EFalse;
    }
       
        
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngMceSession::IsMutedL()
    {
    // Precondition checked in ContainsAudioL
    
    if ( ContainsAudioL() && !iExplicitlyMuted )
        {
        return EFalse;
        }
    
    return ETrue;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusEngMceSession::TDisplayOrientation 
                                            CMusEngMceSession::OrientationL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::RotationL()" )
    
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    CMceDisplaySink* display = MusEngMceUtils::GetDisplayL( *iSession );
    
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
    
    MUS_LOG1( "mus: [ENGINE]  <- CMusEngMceSession::RotationL() %d", 
              displayOrientation )
    
    return displayOrientation;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//        
EXPORT_C void CMusEngMceSession::SetOrientationL( 
                                        TDisplayOrientation aOrientation )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngMceSession::SetOrientationL() %d", 
              aOrientation )
              
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    CMceDisplaySink* display = MusEngMceUtils::GetDisplayL( *iSession );

    if ( aOrientation == EPortrait )
        {
        display->SetRotationL( CMceDisplaySink::ENone );
        }
    else
        {
        display->SetRotationL( CMceDisplaySink::EClockwise90Degree );
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::SetOrientationL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::VolumeUpL()
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMusEngSession::VolumeUpL();

    SetSpeakerVolumeL( VolumeL() );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::VolumeDownL()
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMusEngSession::VolumeDownL();

    SetSpeakerVolumeL( VolumeL() );
    }


// -----------------------------------------------------------------------------
// 
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::SetVolumeL( TInt aVal )
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMusEngSession::SetVolumeL( aVal );

    SetSpeakerVolumeL( VolumeL() );
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::EnableDisplayL( TBool aEnable )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngMceSession::EnableDisplay() %d", 
              aEnable )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceDisplaySink* display = MusEngMceUtils::GetDisplayL( *iSession );
    
    if ( aEnable )
        {
        if ( !display->IsEnabled() )
            {
            display->EnableL();
            MUS_LOG( "                  Display enabled" )
            if ( !iExplicitlyMuted )
                {
                // Since speaker is not explicitly muted, but disabled as
                // a consequence of disabling bundled display, it must be
                // unmuted.
                DoMuteSpeakerL( EFalse );
                MUS_LOG( "                  Bundled speaker enabled" )
                }
            }
        else
            {
            MUS_LOG( "                  Display already enabled, ignore" )
            }
        }
    else
        {
        if ( display->IsEnabled() )
            {
            display->DisableL();
            MUS_LOG( "                  Display disabled" )
            if ( !iExplicitlyMuted )
                {
                // Speaker will not be explicitly muted, but disabled as
                // a consequence of disabling bundled display.
                DoMuteSpeakerL( ETrue );
                MUS_LOG( "                  Bundled speaker disabled" )
                }
            }
        else
            {
            MUS_LOG( "                  Display already disabled, ignore" )
            }
        }
      
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::EnableDisplay()")
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngMceSession::IsDisplayEnabledL()
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    CMceDisplaySink* display = MusEngMceUtils::GetDisplayL( *iSession );
    return display->IsEnabled();
    }


// -----------------------------------------------------------------------------
// Mutes playback of sended audio streams. Audio data is still streamed.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::MuteL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::Mute()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    DoMuteSpeakerL( ETrue );
    
    // Mark speaker as explicitly muted instead of muted because of disabling
    // bundled display
    iExplicitlyMuted = ETrue; 

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::Mute()" )
    }


// -----------------------------------------------------------------------------
// Unmutes playback of sended audio streams.
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngMceSession::UnmuteL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::Unmute()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    DoMuteSpeakerL( EFalse );

    // Mark speaker as explicitly unmuted instead of unmuted because of 
    // enabling bundled display
    iExplicitlyMuted = EFalse;

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::Unmute()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceSession::CMusEngMceSession( const TRect& aRect,
                                      MMusEngSessionObserver& aSessionObserver )
    : CMusEngSession( aRect ),
      iSessionObserver( aSessionObserver ),
      iSecondsFromLastRtcpReport ( 0 ),
      // Although speaker is constructed as muted, it is not explicitly muted
      iExplicitlyMuted( EFalse ) 
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::ConstructL()" )
    CMusEngSession::ConstructL(); // Base class ConstructL -first

    // Creating new MCE Manager and set all needed observers to this class.
    iManager = CMceManager::NewL( TUid::Uid( KMusUiUid ),
                                  &iTransactionDataContainer );

    iManager->SetSessionObserver( this );
    iManager->SetInSessionObserver( this );
    iManager->SetMediaObserver( this );
    iManager->SetRtpObserver( this );

    // Check if operator specific behavior is expected
    iOperatorVariant = ( MultimediaSharingSettings::OperatorVariantSettingL() ==
                         MusSettingsKeys::EOperatorSpecific );
    
    // Update timer initialization
    iUpdateTimer = CMusEngSessionDurationTimer::NewL( *this );
    	
  	iSipProfileHandler = CMusSipProfileHandler::NewL( *this );

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::ConstructL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::RectChangedL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::RectChangedL()" )

    // If session is not yet created, do nothing
    if ( iSession && iSession->State() != CMceSession::ETerminated )
        {
        // Rely on having just one display
        CMceDisplaySink* display = MusEngMceUtils::GetDisplayL( *iSession );
        display->SetDisplayRectL( Rect() );
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::RectChangedL()" )
    }


// -----------------------------------------------------------------------------
// Setting session level sdp lines. Bandwith attribute is always used in
// operator variant mode and at receiver side (using force). 
// However, bandwidth attribute is preferred to be used at media level
// (see SetMediaSdpLinesL method). It is set to session level only if other
// side is using also session level bandwidth. Media level preference exists
// because some other manufacturer's videosharing does not understand session
// level bandwidth attribute.
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetSessionSdpLinesL( 
    CMceSession& aSession, 
    TBool aForceBandwidthLine )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::SetSessionSdpLinesL()" )
    
    MDesC8Array* oldSessionSdpLines = aSession.SessionSDPLinesL();
    CleanupDeletePushL( oldSessionSdpLines );
    TBool bandwidthAtSessionLevel( ContainsText( oldSessionSdpLines, 
                                   KMusEngSessionSdpLineBandwidthLine() ) );
    CleanupStack::PopAndDestroy( oldSessionSdpLines );
    
    CDesC8Array* newSessionSDPLines = 
                    new ( ELeave ) CDesC8ArrayFlat( KMusEngArrayGranularity3 );
    CleanupStack::PushL( newSessionSDPLines );
    
    if ( iOperatorVariant )
        {
        newSessionSDPLines->AppendL( KMusEngSessionSdpLineApplication() );
        newSessionSDPLines->AppendL( KMusEngSessionSdpLineType() );
        }
    else
    	{
    	newSessionSDPLines->AppendL( KMusEngSessionSdpLineXApplication() );	
    	}
    
    if ( bandwidthAtSessionLevel && ( iOperatorVariant || aForceBandwidthLine ) )
	    {
	    MUS_LOG( "mus: [ENGINE] setting bandwidth to session level" )
        newSessionSDPLines->AppendL( KMusEngSessionSdpLineBandwidthField() );
	    }
    	        
    aSession.SetSessionSDPLinesL( newSessionSDPLines );
    
    CleanupStack::Pop( newSessionSDPLines );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::SetSessionSdpLinesL()" )
    }

// -----------------------------------------------------------------------------
// Setting media level sdp lines. Bandwidth is not set to media level if
// it is used already at session level.
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetMediaSdpLinesL( 
    CMceMediaStream& aStream, 
    TBool aForceBandwidthLine )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::SetMediaSdpLinesL()" )
    
    MDesC8Array* sessionSdpLines = aStream.Session()->SessionSDPLinesL();
    CleanupDeletePushL( sessionSdpLines );
    TBool bandwidthAtSessionLevel( ContainsText( sessionSdpLines, 
                                   KMusEngSessionSdpLineBandwidthLine() ) );
    CleanupStack::PopAndDestroy( sessionSdpLines );
    
    if ( !bandwidthAtSessionLevel && ( iOperatorVariant || aForceBandwidthLine ) )
        {
    	MUS_LOG( "mus: [ENGINE] setting bandwidth to media level" )
		
		//Add media attribute to sdp
		const TInt KMusMediaSdpLinesGranularity = 1;
		CDesC8Array* headers = 
		    new ( ELeave ) CDesC8ArrayFlat( KMusMediaSdpLinesGranularity );
		CleanupStack::PushL( headers );
		headers->AppendL( KMusEngSessionSdpLineBandwidthField() );
		aStream.SetMediaAttributeLinesL( headers );   
		CleanupStack::Pop( headers );
        }
		
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::SetMediaSdpLinesL()" )
    }
    
// -----------------------------------------------------------------------------
// Changes volume of all speaker sinks in the session structure
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetSpeakerVolumeL( TInt aNewVolume )
    {
    TInt sessionState = iSession->State();

    if ( sessionState != CMceSession::EIdle &&
         sessionState != CMceSession::EIncoming )
        {
        for ( TInt i = 0; i < iSession->Streams().Count(); ++i )
            {
            CMceSpeakerSink* speaker = 
                MusEngMceUtils::GetSpeaker( *( iSession->Streams()[i] ) );

            if ( speaker &&        
                 aNewVolume >= KMusEngMinVolume &&
                 aNewVolume <= KMusEngMaxVolume )
                {
                // MCE might have different scale for volume than MUS
                // so adjust MUS volume to MCE scale before setting.
                TInt maxVol = speaker->MaxVolumeL();
                TInt setVol = maxVol * aNewVolume / KMusEngMaxVolume;
                setVol = Max(setVol, 1);
                
                MUS_LOG2(
"mus: [ENGINE]  -> CMusEngMceSession::SetSpeakerVolumeL() orig:%d, adjusted:%d", 
aNewVolume, setVol );
                
                if ( setVol <= maxVol )
                    {
                    speaker->SetVolumeL( setVol );
                    }
                }
            }
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//   
void CMusEngMceSession::AdjustStreamsAndCodecsL()
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngMceSession::AdjustStreamsAndCodecsL" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    const RPointerArray<CMceMediaStream>& streams = iSession->Streams();

    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( streams[i]->Type() == KMceVideo )
            {
            CMceVideoStream* videoStream =
                                    static_cast<CMceVideoStream*>( streams[i] );
            
            AdjustVideoStreamL( *videoStream );
            
            if ( videoStream->BoundStream() )
                {
                AdjustVideoStreamL( static_cast<CMceVideoStream&>
                                        ( videoStream->BoundStreamL() ) );
                }
            
            }
        else // audio
            {
            CMceAudioStream* audioStream = 
                                    static_cast<CMceAudioStream*>( streams[i] );
            
            AdjustAudioStreamL( *audioStream );
            
            if ( audioStream->BoundStream() )
                {
                AdjustAudioStreamL( static_cast<CMceAudioStream&>
                                        ( audioStream->BoundStreamL() ) );
                }
        
            }
        }
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngMceSession::AdjustStreamsAndCodecsL" )
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void CMusEngMceSession::AdjustVideoStreamL( CMceVideoStream& aVideoStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngMceSession::AdjustVideoStreamL" )
    
    aVideoStream.SetLocalMediaPortL( KMusEngDedicatedVideoPort );
    
    DoCodecSelectionL( aVideoStream );
    
    const RPointerArray<CMceVideoCodec>& codecs = aVideoStream.Codecs();
        
    for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
        {
        AdjustVideoCodecL( *codecs[codecIndex] );
        }
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngMceSession::AdjustVideoStreamL" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
void CMusEngMceSession::AdjustAudioStreamL( CMceAudioStream& aAudioStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngMceSession::AdjustAudioStreamL" )
    
    aAudioStream.SetLocalMediaPortL( KMusEngDedicatedAudioPort );

    const RPointerArray<CMceAudioCodec> codecs = aAudioStream.Codecs();
    
    for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
        {
        AdjustAudioCodecL( *codecs[codecIndex] );
        }
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngMceSession::AdjustAudioStreamL" )
    }
    

// -----------------------------------------------------------------------------
// Calls CMceInSession::RejectL() inside TRAP_IGNORE
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::Reject( CMceInSession& aSession,
                                const TDesC8& aReason,
                                TUint32 aCode )
    {
    if ( aCode != 0 || aReason != KNullDesC8() )
        {
        TRAP_IGNORE( aSession.RejectL( aReason, aCode, NULL, NULL, NULL ) )
        }
    else
        {
        if ( iOperatorVariant )
            {
            // In operator variant, session is rejected with 486 instead of 603.
            // Also the reason phrase is supposed to be "Busy".
            TRAP_IGNORE( aSession.RejectL( KMusEngSipReasonPhraseBusy(), 
                                           KMusEngSipReasonCodeBusyHere ) )
            }
        else
            {
            // Normal case
            TRAP_IGNORE( aSession.RejectL() )
            }
        }
    }
    

// -----------------------------------------------------------------------------
// By default rejects all incoming sessions immediately without notifying UI
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::IncomingSession( 
                      CMceInSession* aSession,
                      TMceTransactionDataContainer* /*aContainer*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::IncomingSession()" )
    
    Reject( *aSession, 
            KMusEngSipReasonPhraseBusyHere(),
            KMusEngSipReasonCodeBusyHere );
             
    delete aSession;
    aSession = NULL;    
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::IncomingSession()" )
    }


// -----------------------------------------------------------------------------
// By default rejects all incoming updates immediately without notifying UI
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::IncomingUpdate( 
                     CMceSession& aOrigSession, 
                     CMceInSession* aUpdatedSession,
                     TMceTransactionDataContainer* /*aContainer*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::IncomingUpdate()" )
                                    
    if ( iSession &&
         iSession == &aOrigSession )
        {
        // Old session is useless from now on
        delete iSession;
        iSession = aUpdatedSession;
        MUS_LOG( "mus: [ENGINE]  Unexpected update, reject" )
        Reject( *aUpdatedSession );    
        }
    else
        {
        // This should never happen
        MUS_LOG( "mus: [ENGINE]  Unknown update, reject and delete" )
        Reject( *aUpdatedSession );    
        delete aUpdatedSession;
        } 
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::IncomingUpdate()" )    
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::StreamStateChanged( CMceMediaStream& aStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngMceSession::StreamStateChanged()" )
    
    if ( !iSession )
        {
        return; // We cannot do anything
        }
    
    MUS_ENG_LOG_STREAM_STATE( aStream )
        
    switch ( aStream.State() )
        {
        case CMceMediaStream::EUninitialized: // Stream is created
            {
            // Unexpected state change
            break;
            }
        case CMceMediaStream::EInitialized: // Stream is initialized
            {
            // Should be handled in sibling classes if needed
            break;
            }
        case CMceMediaStream::EBuffering: // Stream is buffering
            {
            // Should be handled in sibling classes if needed
            break;
            }
        case CMceMediaStream::EIdle: // Stream is not receiving RTP
            {
            iSessionObserver.StreamIdle();
            break;
            }
        case CMceMediaStream::EStreaming: // Stream is streaming
            {
            // If streaming stream is complete video out- or instream, inform UI 
            if ( aStream.Type() == KMceVideo &&
                 aStream.Source() && 
                 aStream.Sinks().Count() >= 0 &&
                 ( aStream.Source()->Type() == KMceRTPSource ||
                   aStream.Sinks()[0]->Type() == KMceRTPSink ) )
                {
                iSessionObserver.StreamStreaming();
                }
            break;
            }
        case CMceMediaStream::EDisabled: // Stream is explicitly disabled
            {
            break;
            }
        case CMceMediaStream::ENoResources: 
            {
            // Stream has no needed resources to stream
            break;
            }         
        case CMceMediaStream::ETranscodingRequired: 
            {
            // Stream requires non-realtime transcoding
            // Should be handled in sibling classes
            break;
            }   
        case CMceMediaStream::ETranscoding: 
            {
            // Stream is transcoding in non-realtime
            // Should be handled in sibling classes
            break;
            }       
        default:
            {
            break;
            }
        }
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngMceSession::StreamStateChanged()" )
                
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::StreamStateChanged( CMceMediaStream& aStream,
                                            CMceMediaSource& /*aSource*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::StreamStateChanged( src )" )
    // Use default logic
    StreamStateChanged( aStream );
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::StreamStateChanged( src )" )       
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::StreamStateChanged( CMceMediaStream& aStream,
                                            CMceMediaSink& /*aSink*/ )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngMceSession::StreamStateChanged( sink )" )
    // Use default logic
    StreamStateChanged( aStream );
    MUS_LOG( "mus: [ENGINE] <- CMusEngMceSession::StreamStateChanged( sink )" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SessionStateChanged(
                        CMceSession& aSession,
                        TMceTransactionDataContainer* aContainer )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::SessionStateChanged()" )
        
    if ( !aContainer )
        {
        // Container should never be NULL, but if it is, handle as
        // internal error
        iSessionObserver.SessionFailed();
        return;
        }

    // This is the only point to get statuscode and reasonphrase. With this call
    // they are zeroed and thus cannot be got anymore.
    TInt statusCode = aContainer->GetStatusCode();
    HBufC8* reasonPhrase = aContainer->GetReasonPhrase();
    
    if ( reasonPhrase )
        {        
        HandleSessionStateChanged( aSession, statusCode, *reasonPhrase ); 
        delete reasonPhrase ;       
        }
    else
        {
        HandleSessionStateChanged( aSession, statusCode, KNullDesC8() );
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::SessionStateChanged()" )
    }
            
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SessionConnectionStateChanged( CMceSession& aSession,
                                                       TBool aActive )
    {
    MUS_LOG1( "mus: [ENGINE]     CMusEngMceSession::\
              SessionConnectionStateChanged() active = %b", aActive )

    if ( iSession && iSession == &aSession )
        {
        if ( !aActive )
            {
            MUS_LOG( "mus: [ENGINE]     CMusEngMceSession::\
                     SessionConnectionStateChanged: Notify observer" )
            iSessionObserver.SessionConnectionLost();
            }
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::\
             SessionConnectionStateChanged()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//

#if ( defined( _DEBUG ) && !defined( UNIT_TESTING ) ) 
void CMusEngMceSession::Failed( CMceSession& aSession, TInt aError )
#else
void CMusEngMceSession::Failed( CMceSession& aSession, TInt /*aError*/ )
#endif
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngMceSession::Failed() error #%d", 
              aError )
    
    if ( iSession && iSession == &aSession )
        {
        MUS_LOG( "mus: [ENGINE]    CMusEngMceSession::Failed: Notify observer" )
        iSessionObserver.SessionFailed();
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::Failed()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::UpdateFailed(
                   CMceSession& aSession,
                   TMceTransactionDataContainer* aContainer )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngMceSession::UpdateFailed()" )

    if ( !aContainer )
        {
        // Container should never be NULL, but if it is, handle as
        // internal error
        iSessionObserver.SessionFailed();
        return;
        }

    if ( iSession && iSession == &aSession )
        {
        MUS_LOG( "mus: [ENGINE]     CMusEngMceSession::UpdateFailed: \
                 Notify observer" )
        iSessionObserver.SessionFailed();
        }

    MUS_LOG( "mus: [ENGINE] <- CMusEngMceSession::UpdateFailed()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SRReceived( CMceSession& aSession,
					                CMceMediaStream& aStream)
    {
    // No logging because of amount of reports
    if ( iSession &&
         iSession == &aSession )
        {
        for ( TInt i = 0; i < iSession->Streams().Count(); ++i )
            {
            if ( &aStream == iSession->Streams()[i] )
                {
                iSecondsFromLastRtcpReport = 0;
                }
            }
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::RRReceived( CMceSession& aSession,
					                CMceMediaStream& aStream)
    {
    // No logging because of amount of reports

    if ( iSession &&
         iSession == &aSession )
        {
        for ( TInt i = 0; i < iSession->Streams().Count(); ++i )
            {
            if ( &aStream == iSession->Streams()[i] )
                {
                iSecondsFromLastRtcpReport = 0;
                }
            }
        }
    }	             
					             

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InactivityTimeout( CMceMediaStream& aStream,
                                           CMceRtpSource& /*aSource*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::InactivityTimeout()" )
    // This function may be deprecated in future and after that similar 
    // functionality can be obtained by observing stream state EIdle.
    // Anyway it does not work yet and until then, informing UI about 
    // RTP inactivity is done in this function.
    
    if ( aStream.Type() == KMceVideo )
        {
        iSessionObserver.StreamIdle();
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::InactivityTimeout()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SsrcAdded( CMceMediaStream& /*aStream*/,
                                   CMceRtpSource& /*aSource*/,
                                   TUint /*aSsrc*/ )
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngMceSession::SsrcAdded( ... )" )
    // NOP, We are not at all interested about SSRCs
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SsrcRemoved( CMceMediaStream& /*aStream*/,
                                     CMceRtpSource& /*aSource*/,
                                     TUint /*aSsrc*/ )
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngMceSession::SsrcRemoved(... )" )
    // NOP, We are not at all interested about SSRCs
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsRoamingBetweenAPsAllowed()
    {
    TBool allowed( ETrue );
    if ( iSession && 
         iSession->State() != CMceSession::EIdle &&
         iSession->State() != CMceSession::ETerminated )
        {
        allowed = EFalse;
        }
    return allowed;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::HandleSessionStateChanged( CMceSession& aSession,
                                                   TInt aStatusCode,
                                                   const TDesC8& aReasonPhrase )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::HandleSessionStateChanged" )
    
    MUS_ENG_LOG_SESSION_STATE_AND_STATUS( aSession, aStatusCode, aReasonPhrase ) 

    if ( iSession && iSession == &aSession )
        {
        switch ( aSession.State() )
            {
            case CMceSession::EIdle:
                {
                MUS_LOG( "mus: [ENGINE] Unexpected asynchronous \
                         state transition, consider session as failed." )
                iSessionObserver.SessionFailed();
                break;
                }
            case CMceSession::EOffering :
                {
                break;
                }
            case CMceSession::EIncoming:
                {
                MUS_LOG( "mus: [ENGINE] Unexpected asynchronous \
                         state transition, consider session as failed." )
                iSessionObserver.SessionFailed();
                break;
                }
            case CMceSession::EReserving :
                {
                // UpdateL called to incoming session during
                // session establishment
                break;
                }
            case CMceSession::EAnswering :  // Answering an incoming call
                {
                break;
                }
            case CMceSession::EProceeding :
                {
                break;
                }
            case CMceSession::EEstablished:
                {
                // Check that session timer is not already running, which is 
                // the case when refreshing the session with session timer 
                if ( !iUpdateTimer->IsActive() )
                    {
                    iStartTime.HomeTime(); // Start counting session duration              
                    iUpdateTimer->Start( KMusEngTimerInterval );
                    iSessionObserver.SessionEstablished();
                    }
                    
                break;
                }
            case CMceSession::ECancelling:
                {
                // MCE has for some reason started to cancel session
                break;
                }
            case CMceSession::ETerminating:
                {
                // MCE has for some reason started to terminate session
                break;
                }
            case CMceSession::ETerminated:
                {
                HandleTermination( aStatusCode, aReasonPhrase );
                break;
                }
            default:
                {
                MUS_LOG( "mus: [ENGINE]  CMusEngMceSession::SessionStateChanged(), \
                         default case" )
                break;
                }
            }
        }
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::HandleSessionStateChanged" )
    }


// -----------------------------------------------------------------------------
// This function should be called only if sibling classes cannot handle
// termination reason by themselves.
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::HandleTermination( TInt aStatusCode,
                                           const TDesC8& /*aReasonPhrase*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::HandleTermination()" )

	iSipProfileHandler->RefreshIapAvailabilities();

    switch ( aStatusCode )
        {
        case 0:
            {
            // Normal session termination or session by another end.
            iSessionObserver.SessionTerminated();
            break;
            }
        case KMusEngSipReasonCodeOk:
            {
            // Normal session termination by this end: We have sent BYE
            // and now received 200 OK to it.
            iSessionObserver.SessionTerminated();
            break;
            }
        default:
            {
            // Termination reason cannot be determined, handle as internal
            // error.
            iSessionObserver.SessionFailed();
            break;
            }
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::HandleTermination()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::AdjustVideoCodecL( CMceVideoCodec& aVideoCodec )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::AdjustVideoCodecL()" )
    
    aVideoCodec.SetMMFPriorityL( KAudioPrioritySwisPlayback );
    aVideoCodec.SetMMFPriorityPreferenceL( KAudioPrefSwisPlayback );
    MUS_LOG( "mus: [ENGINE]     Video MMF priority and preference set" )

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::AdjustVideoCodecL()" )
    }

 
// -----------------------------------------------------------------------------
// 1. Sets MMF audio priority and preference
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::AdjustAudioCodecL( CMceAudioCodec& aAudioCodec )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::AdjustAudioCodecL()" )
    
    aAudioCodec.SetMMFPriorityL( KAudioPrioritySwisPlayback );
    aAudioCodec.SetMMFPriorityPreferenceL( KAudioPrefSwisPlayback );
    MUS_LOG( "mus: [ENGINE]     Audio MMF priority and preference set" )
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::AdjustAudioCodecL()" )
    }


// -----------------------------------------------------------------------------
// Remove multiples of H.263 codec, prefer H263-2000 over H263-1998.
// Additionally select just the one with best quality from selected mode.
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::DoCodecSelectionL( CMceVideoStream& aVideoStream )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::DoCodecSelectionL()" )
    
    const RPointerArray<CMceVideoCodec>& codecs = aVideoStream.Codecs();
    TBool codecModeBasedRemovalNeeded( EFalse );
    // Label:H263
    TBool H2632000CodecFound( EFalse );
    TBool H2631998CodecFound( EFalse );
    for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
        {
        const CMceVideoCodec& currentCodec = *codecs[codecIndex];
        if ( currentCodec.SdpName().FindF( KMceSDPNameH263 ) != KErrNotFound )
            { 
            if ( !currentCodec.SdpName().CompareF( KMceSDPNameH2632000 ) )
                {
                H2632000CodecFound = ETrue;
                codecModeBasedRemovalNeeded = H2631998CodecFound;
                }  
            else if ( !currentCodec.SdpName().CompareF( KMceSDPNameH2631998 ) )
                {
                H2631998CodecFound = ETrue;
                codecModeBasedRemovalNeeded = H2632000CodecFound;
                }
            else 
                {
                // NOP
                }
            }
        }
    if ( codecModeBasedRemovalNeeded )
        {
        DoCodecModeBasedRemovalL( aVideoStream );
        }
    
    const RPointerArray<CMceVideoCodec>& codecs2 = aVideoStream.Codecs();
    const CMceVideoCodec* bestBitrateCodec( NULL );
    for ( TInt codecIndex = 0; codecIndex < codecs2.Count(); ++codecIndex )
        {
        const CMceVideoCodec& currentCodec = *codecs2[codecIndex];
        if ( currentCodec.SdpName().FindF( KMceSDPNameH263 ) != KErrNotFound )
            {
            if ( !bestBitrateCodec || 
                 currentCodec.MaxBitRate() > bestBitrateCodec->MaxBitRate() )
                {
                bestBitrateCodec = &currentCodec;
                } 
            }
        }        
    if ( bestBitrateCodec != NULL )
        {
        DoBitrateBasedRemovalL( aVideoStream, *bestBitrateCodec );
        }
   
    /* Codec removal based on configuration */
    DoCodecConfigurationBasedRemovalL( aVideoStream );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::DoCodecSelectionL()" )
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::UpdateTimerEvent()
    {
    // Update timer is used also to detect RTCP inactivity
    ++iSecondsFromLastRtcpReport;
    
    iSessionObserver.SessionTimeChanged( GetSessionTime() );

    if ( iSecondsFromLastRtcpReport >= KMusEngRtcpInactivityThreshold )
        {
        iSessionObserver.InactivityTimeout();    
        // Disable calling multiple times by reseting timer
        iSecondsFromLastRtcpReport = 0;
        }

    iUpdateTimer->Start( KMusEngTimerInterval );
    }


// -----------------------------------------------------------------------------
// Enables or disables all the speaker sinks of all the audio streams
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::DoMuteSpeakerL( TBool aMute )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngMceSession::MuteL( %d )", aMute )
    
    const RPointerArray<CMceMediaStream>& streams = iSession->Streams();

    for ( TInt i = 0; i < streams.Count(); ++i )
        {
        CMceSpeakerSink* speaker = 
                MusEngMceUtils::GetSpeaker( *streams[i] );
            
        if ( speaker )
            {
            if( aMute )
                {
                if ( speaker->IsEnabled() )
                    {
                    speaker->DisableL();
                    }
                else
                    {
                    MUS_LOG( "mus: [ENGINE]     Speaker already muted, NOP" )
                    }
                }
            else
                {
                if ( !speaker->IsEnabled() )
                    {
                    speaker->EnableL();
                    }
                else
                    {
                    MUS_LOG( "mus: [ENGINE]     Speaker already unmuted, NOP" )
                    }
                }
            }
        }
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::MuteL( TBool aMute )" )
    }

// -----------------------------------------------------------------------------
// Checks if some of array items contains specific text 
// (exact match not required)
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::ContainsText( MDesC8Array* aArray, const TDesC8& aItem )
    {
    for ( TInt i = 0; aArray && i < aArray->MdcaCount(); i++ )
        {
        if ( aArray->MdcaPoint( i ).FindF( aItem ) != KErrNotFound )
            {
            return ETrue;
            }
        }
    return EFalse;
    }
    

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::DoBitrateBasedRemovalL( 
    CMceVideoStream& aVideoStream, 
    const CMceVideoCodec& aBestBitrateVideoCodec )
    {   
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::DoBitrateBasedRemovalL()" )
    
    RPointerArray<CMceVideoCodec>& codecs = 
        MUS_CODEC_ARR_CONST_CAST( aVideoStream.Codecs() );
                          
    TInt codecIndex = 0;
    while ( codecIndex < codecs.Count() )
        {
        CMceVideoCodec& currentCodec = *codecs[codecIndex++];
        if ( currentCodec.SdpName().FindF( KMceSDPNameH263 ) != KErrNotFound &&
             &currentCodec != &aBestBitrateVideoCodec )
            {
            MUS_LOG( "mus: [ENGINE]         removing" )
            aVideoStream.RemoveCodecL( currentCodec );
            codecs = MUS_CODEC_ARR_CONST_CAST( aVideoStream.Codecs() );
            codecIndex = 0;
            }
        }
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::DoBitrateBasedRemovalL()" )
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::DoCodecModeBasedRemovalL( CMceVideoStream& aVideoStream )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::DoCodecModeBasedRemovalL()" )
    
    RPointerArray<CMceVideoCodec>& codecs = 
        MUS_CODEC_ARR_CONST_CAST( aVideoStream.Codecs() );
        
    TInt codecIndex = 0;
    while ( codecIndex < codecs.Count() )
        {
        CMceVideoCodec& currentCodec = *codecs[codecIndex++];
        if ( !currentCodec.SdpName().CompareF( KMceSDPNameH2631998 ) )
            {
            MUS_LOG( "mus: [ENGINE]         removing" )
            aVideoStream.RemoveCodecL( currentCodec );
            codecs = MUS_CODEC_ARR_CONST_CAST( aVideoStream.Codecs() );
            codecIndex = 0;
            }
        }
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::DoCodecModeBasedRemovalL()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::DoCodecConfigurationBasedRemovalL( CMceVideoStream& aVideoStream )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::DoCodecConfigurationBasedRemovalL()" )
    if( MultimediaSharingSettings::IsAvcDisabled())
        {
        RPointerArray<CMceVideoCodec>& codecs = 
            MUS_CODEC_ARR_CONST_CAST( aVideoStream.Codecs() );            
        TInt codecIndex = 0;
        while ( codecIndex < codecs.Count() )
            {
            CMceVideoCodec& currentCodec = *codecs[codecIndex++];            
            if ( !currentCodec.SdpName().CompareF( KMceSDPNameH264 ) )
                {
                MUS_LOG( "mus: [ENGINE]  - Removing avc from supported codec list" )
                aVideoStream.RemoveCodecL( currentCodec );
                codecs = MUS_CODEC_ARR_CONST_CAST( aVideoStream.Codecs() );
                codecIndex = 0;
                }
            }
        }    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::DoCodecConfigurationBasedRemovalL()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::VolumeChanged( TInt aVolume, TBool aAudioRouteChanged )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngMceSession::VolumeChanged(): %d", aVolume )
    if ( iSession )
        {
        CMusEngSession::VolumeChanged( aVolume, aAudioRouteChanged );
        TRAP_IGNORE( SetSpeakerVolumeL( aVolume ) );
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::VolumeChanged()" )
    }
