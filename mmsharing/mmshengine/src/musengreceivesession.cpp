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
* Description: 
*
*/



// USER INCLUDES
#include "musengreceivesession.h"
#include "musengmceutils.h"
#include "musenglogger.h"
#include "mussipprofilehandler.h"
#include "mussessionproperties.h"
#include "musengremotevideoplayer.h"

// SYSTEM INCLUDES
#include <lcsessionobserver.h>
#include <mcemanager.h>
#include <mceinsession.h>
#include <mcestreambundle.h>
#include <mceaudiostream.h>
#include <mcevideostream.h>
#include <mcemediasource.h>
#include <mcertpsource.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <mceaudiocodec.h>
#include <mceavccodec.h>
#include <e32property.h>

#include <utf.h>

const TInt KMusEngJitterBufferLength = 51; //Must be bigger than treshold
// Using following value increases treshold buffer to 1 second from 
// default 100 ms
const TInt KMusEngJitterBufferTreshold = 50;
const TInt KMusEngTresholdToSecondsFactor = 20;
const TInt KMusEngTwoSecondInMilliSeconds = 2000; 
// Use inactivity timer value that is a little bigger than treshold in seconds
const TUint KMusEngInactivityTimer = KMusEngTresholdToSecondsFactor * 
                                     KMusEngJitterBufferTreshold + 
                                     KMusEngTwoSecondInMilliSeconds;

_LIT8( KMusEngSwisIdentifier, "Require: precondition" );
_LIT8( KMusEngAssertedIdentity, "P-Asserted-Identity" );
_LIT8( KMusEngBiggerSign, ">" );

const TInt KMusEngSipReasonCodeBusyHere = 486;
_LIT8( KMusEngSipReasonPhraseBusy, "Busy" );

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngReceiveSession* CMusEngReceiveSession::NewL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::NewL(...)" )

    CMusEngReceiveSession* self = new( ELeave )CMusEngReceiveSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::NewL(...)" )
    return self;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngReceiveSession::CMusEngReceiveSession()
    : CMusEngMceSession()
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::ConstructL()" )

    CMusEngMceSession::ConstructL();
    
    iOriginator = HBufC8::NewL( KMaxUriLength );
    
    iRemoteDisplayName = HBufC::NewL( KMaxUriLength );

    iRemoteVideoPlayer = CMusEngRemoteVideoPlayer::NewL( *this, *this );
    
    iReceivingInactivityTimeout = KMusEngInactivityTimer;
    
    iKeepaliveTimer = KMusEngRtpKeepAliveTimer;
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::ConstructL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngReceiveSession::~CMusEngReceiveSession()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::~CMusEngReceiveSession()" )
    
    if ( iOriginator )
        {
        TRAP_IGNORE( SaveContactL( *iOriginator ) )
        }
    delete iOriginator;
    
    delete iRemoteDisplayName;
    
    delete iRemoteVideoPlayer;
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::~CMusEngReceiveSession()" )
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcSession::TLcSessionState CMusEngReceiveSession::LcSessionState() const
    {
    TLcSessionState state = CMusEngMceSession::LcSessionState();
    if ( !iSession && state == MLcSession::EUninitialized )
        {
        state = MLcSession::EReceived;
        }
    return state;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::EstablishLcSessionL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::EstablishLcSessionL" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    InSession()->AcceptL();
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::EstablishLcSessionL" )
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::TerminateLcSessionL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::EstablishLcSessionL" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    if ( iSession->State() == CMceSession::EIncoming ||
         iSession->State() == CMceSession::EProceeding )
        {
        // Reject
        if ( iOperatorVariant )
            {
            // In operator variant, session is rejected with 486 instead of 603.
            // Also the reason phrase is supposed to be "Busy".
            InSession()->RejectL( KMusEngSipReasonPhraseBusy(), 
                                  KMusEngSipReasonCodeBusyHere );
            }
        else
            {
            InSession()->RejectL();
            }
        }
    else
        {
        iSession->TerminateL();
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::EstablishLcSessionL" )
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngReceiveSession::RemoteVideoPlayer()
    {
    return iRemoteVideoPlayer;
    }
    
// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
const TDesC& CMusEngReceiveSession::RemoteDisplayName()
    {
    TBuf<RProperty::KMaxPropertySize> buffer;

    TInt error = RProperty::Get( NMusSessionApi::KCategoryUid,
                                 NMusSessionApi::KContactName,
                                 buffer );
    if ( error == KErrNone && buffer.Length() )
        {
        iRemoteDisplayName->Des().Copy( buffer );
        }
    
    else if ( iOriginator )
        {
        iRemoteDisplayName->Des().Copy(*iOriginator);   
        }
    return *iRemoteDisplayName;
    }

// -----------------------------------------------------------------------------
// When reserving resources is ready, this function reports observer about
// incoming session and signals this also to other end terminal ( 180 ringing ).
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::HandleSessionStateChanged(
                                        CMceSession& aSession,
                                        TInt aStatusCode,
                                        const TDesC8& aReasonPhrase )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::\
             HandleSessionStateChanged()" )

    MUS_ENG_LOG_SESSION_STATE_AND_STATUS( aSession, aStatusCode, aReasonPhrase )

    if ( iSession && 
         iSession == &aSession &&
         aSession.State() == CMceSession::EProceeding )
        {
        // Indicates that resource reservation is ready, user can be notified
        // if we do not have to wait for PRACK
        
        if ( !iRingLCalled )
            {
            // Notify other end terminal
            TRAP_IGNORE( InSession()->RingL() )
            iRingLCalled = ETrue;
            TPtr8 ptrOriginator( iOriginator->Des() );
            ptrOriginator.Copy( iSession->Originator() );
            InformObserverAboutSessionStateChange();
            }
        else
            {
            MUS_LOG( "mus: [ENGINE]     User and remote end already notified." )
            }
            
        }
    else
        {
        // Forward all other session state changes to base class
        CMusEngMceSession::HandleSessionStateChanged( aSession,
                                                      aStatusCode,
                                                      aReasonPhrase );
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::\
             HandleSessionStateChanged()" )
    }

// -----------------------------------------------------------------------------
// Sets keepalive timer for video and calls base class variant of this function
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::AdjustVideoCodecL( CMceVideoCodec& aVideoCodec,
                                               TMceSourceType aSourceType )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::AdjustVideoCodecL()" )
    
    CMusEngMceSession::AdjustVideoCodecL( aVideoCodec, aSourceType );
    
    MUS_LOG1( "mus: [ENGINE]     Keepalive timer:%d", iKeepaliveTimer )
    aVideoCodec.SetKeepAliveTimerL( iKeepaliveTimer );
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
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::AdjustVideoCodecL()" )
    }
        
// -----------------------------------------------------------------------------
// Sets keepalive timer for audio and calls base class variant of this function
// -----------------------------------------------------------------------------
//        
void CMusEngReceiveSession::AdjustAudioCodecL( CMceAudioCodec& aAudioCodec )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::AdjustAudioCodecL()" )

    CMusEngMceSession::AdjustAudioCodecL( aAudioCodec );
    
    aAudioCodec.SetKeepAliveTimerL( iKeepaliveTimer );
    aAudioCodec.SetKeepAlivePayloadTypeL( KMusEngKeepAlivePayloadTypeAudio );
    aAudioCodec.SetKeepAliveDataL( KNullDesC8() );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::AdjustAudioCodecL()" )
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::DoCodecSelectionL( CMceVideoStream& aVideoStream )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::DoCodecSelectionL()" )
    
    const RPointerArray<CMceVideoCodec>& codecs = aVideoStream.Codecs();
    
    const CMceVideoCodec* avcSingleNal = NULL;
    const CMceVideoCodec* avcNonInterleaved = NULL;
    
    for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
        {
        if ( codecs[codecIndex]->SdpName() == KMceSDPNameH264() )
            { 
            if ( codecs[codecIndex]->CodecMode() == KMceAvcModeSingleNal )
                {
                // Store a pointer to the Single NAL codec with best bitrate
                if ( !avcSingleNal || 
                     ( avcSingleNal && codecs[codecIndex]->MaxBitRate() > 
                                       avcSingleNal->MaxBitRate() ) )
                    {
                    avcSingleNal = codecs[codecIndex];
                    }
                }  
            else if ( codecs[codecIndex]->CodecMode() == 
                                                KMceAvcModeNonInterleaved )
                {
                // Store a pointer to the Non-Interleaved codec with best bitrate
                if ( !avcNonInterleaved || 
                     ( avcNonInterleaved && codecs[codecIndex]->MaxBitRate() > 
                                            avcNonInterleaved->MaxBitRate() ) )
                    {
                    avcNonInterleaved = codecs[codecIndex];
                    }
                }
            else 
                {
                // NOP, we do not care about interleaved AVC
                }
            }
        }
    
    const CMceVideoCodec* selectedCodec = NULL;
    
    if ( avcNonInterleaved )
        {
        selectedCodec = avcNonInterleaved;
        }
    else
        {
        selectedCodec = avcSingleNal;
        }
   
    if ( selectedCodec )
        {
        // Remove all other codecs
        for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
            {
            if ( codecs[codecIndex] != selectedCodec )
                {
                aVideoStream.RemoveCodecL( *codecs[codecIndex] );
                // Since succesfull removal of a codec has decreased the amount
                // of codecs in array by one, we have to reset the index
                codecIndex = 0;
                }
            }   
        }
    else
        {
        // Let the base class do H.263 selection
        CMusEngMceSession::DoCodecSelectionL( aVideoStream );
        }
               
    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::DoCodecSelectionL()" )
    }

// -----------------------------------------------------------------------------
// If incoming session does not have valid structure and cannot be reconstructed
// as such, session is rejected automatically.
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::IncomingSession(
    CMceInSession* aSession,
    TMceTransactionDataContainer* aContainer )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::IncomingSession(...)" )

    if ( !aContainer || 
         ( iSession &&
           iSession->State() != CMceSession::ETerminated ) )
        {
        // We already have one session ongoing, or do not have container. 
        // Reject new session.
        Reject( *aSession );
        delete aSession;
        aSession = NULL;
        MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::IncomingSession( \
                 session already exists. New session rejected and deleted. )" )
        return;
        }

    delete iSession; // possibly existing terminated session
    iSession = aSession;
    
    if ( iOperatorVariant )
        {
        ParseAssertedIdentity( aContainer );    
        }
    
    // Reset variable defined for preparing for multiple state changes to
    // EProceeding. This might happen if we set 100rel to be required. 
    iRingLCalled = EFalse;
    
    // If preparing fails, session setup with VS2.2 will fail later
    TRAP_IGNORE( PrepareToRequire100RelL( aContainer ) )
        
    TRAPD( error, CompleteSessionStructureL() );

    if ( error != KErrNone )
        {
        Reject( *InSession() );
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::IncomingSession()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::IncomingUpdate(
    CMceSession& aOrigSession,
    CMceInSession* aUpdatedSession,
    TMceTransactionDataContainer* aContainer )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::IncomingUpdate(...)" )

    if ( iSession &&
         iSession == &aOrigSession )
        {
        // Old session is useless from now on
        delete iSession;
        iSession = aUpdatedSession;

        TRAPD( error, CompleteSessionStructureL() );

        if ( error != KErrNone )
            {
            MUS_LOG( "mus: [ENGINE]  Cannot handle update, reject" )
            Reject( *aUpdatedSession );
            }
        }
    else
        {
        // Cannot handle, forward to a base class
        CMusEngMceSession::IncomingUpdate( aOrigSession,
                                           aUpdatedSession,
                                           aContainer );
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::IncomingUpdate(...)" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::StreamStateChanged( CMceMediaStream& aStream )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngReceiveSession::StreamStateChanged()" )

    MUS_ENG_LOG_STREAM_STATE( aStream )

    if ( !iSession )
        {
        return;
        }

    if ( aStream.Type() == KMceVideo &&
         aStream.Source() &&
         aStream.Source()->Type() == KMceRTPSource )
        {
        if ( aStream.State() == CMceMediaStream::EBuffering )
            {
            iBuffered = ETrue;
            InformObserverAboutPlayerStateChange( iRemoteVideoPlayer );
            }
        else if ( aStream.State() == CMceMediaStream::EStreaming )
            {
            ReceivingStarted();          
            }
        else
            {
            // Cannot handle, forward to CMusEngMceSession
            CMusEngMceSession::StreamStateChanged( aStream );
            }
        }
    else
        {
        // Cannot handle, forward to CMusEngMceSession
        CMusEngMceSession::StreamStateChanged( aStream );
        }

    MUS_LOG( "mus: [ENGINE] <- CMusEngReceiveSession::StreamStateChanged()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMceInSession* CMusEngReceiveSession::InSession()
    {
    return static_cast<CMceInSession*>(iSession);
    }

// -----------------------------------------------------------------------------
// Check if incoming session is originated from VideoSharing 2.2 client, 
// which sends PRACK for 180 Ringing whether it is required or not. In 
// order to let MCE handle the PRACK correctly, force 180 Ringing to be 
// sent reliably.
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::PrepareToRequire100RelL( 
    TMceTransactionDataContainer* aContainer)
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::PrepareToRequire100RelL()" )
    
    __ASSERT_ALWAYS( aContainer, User::Leave( KErrArgument ) );
    
    CDesC8Array* headers = aContainer->GetHeaders();
    CleanupStack::PushL( headers );
     
    if ( headers )
        {
        for ( TInt i = 0; i < headers->Count(); ++i )
            {
            if ( headers->MdcaPoint(i).FindF( KMusEngSwisIdentifier() ) != 
                 KErrNotFound )
                {
                iSession->SetModifierL( KMce100Rel, KMce100RelRequired );
                MUS_LOG( "mus: [ENGINE]     Forced to require 100rel" )
                }   
            }     
        }
    
    CleanupStack::PopAndDestroy( headers );
   
    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::PrepareToRequire100RelL()" )
    }      

// -----------------------------------------------------------------------------
// Check that incoming session contains only incoming video and audio
// streams. All the other streams will be removed. If there is no
// display sinks and speakers, they will be created.
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::CompleteSessionStructureL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::CompleteSessionStructureL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    // Force bandwidth line usage in sdp as it is mandatory
    // at MT side based on GSMA VS specification IR.74. Bandwidth is set to
    // session or to media level based on sender's way of usage. If other end
    // is not using bandwidth attribute at all, media level is preferred.
    SetSessionSdpLinesL( *iSession, ETrue );

    CMceStreamBundle* localBundle =
                            CMceStreamBundle::NewLC( CMceStreamBundle::ELS );

    const RPointerArray<CMceMediaStream>& streams = iSession->Streams();

    CMceMediaStream* videoInStream = NULL;

    for( TInt i = 0; i < streams.Count(); ++i )
        {
        if ( MusEngMceUtils::IsVideoInStream( *streams[i] ) &&
             !videoInStream )
            {
            videoInStream = streams[i];
            
            // Disable possible opposite stream to indicate that sendrecv is
            // not allowed.
            if ( streams[i]->BoundStream() )
                {
                MusEngMceUtils::DisableStreamL( streams[i]->BoundStreamL() );
                }
            }
        else if ( streams[i]->BoundStream() &&
                  MusEngMceUtils::IsVideoInStream( streams[i]->BoundStreamL() ) &&
                  !videoInStream )
            {
            videoInStream = &streams[i]->BoundStreamL();
                
            // Disable opposite stream to indicate that sendrecv is not allowed.
            MusEngMceUtils::DisableStreamL( *streams[i] );
            }
        else if ( MusEngMceUtils::IsAudioInStream( *streams[i] ) && 
                  !iOperatorVariant )
            {
            MusEngMceUtils::AddSpeakerL( *streams[i] );

            localBundle->AddStreamL( *streams[i] );
            
            // Disable possible opposite stream to indicate that sendrecv is
            // not allowed.
            if ( streams[i]->BoundStream() )
                {
                MusEngMceUtils::DisableStreamL( streams[i]->BoundStreamL() );
                }
            }
        else if ( streams[i]->BoundStream() &&
                  MusEngMceUtils::IsAudioInStream( streams[i]->BoundStreamL() ) &&
                  !iOperatorVariant )
            {
            MusEngMceUtils::AddSpeakerL( streams[i]->BoundStreamL() );

            localBundle->AddStreamL( streams[i]->BoundStreamL() );
            
            // Disable opposite stream to indicate that sendrecv is not allowed.
            MusEngMceUtils::DisableStreamL( *streams[i] );
            }
        else
            {
            // We remove stream because either it 
            // 1. Is not and does not contain incoming video or audio, or
            // 2. We already have one incoming video stream or
            // 3. Stream is audio and we run operator variant where audio is 
            //    not allowed.
            iSession->RemoveStreamL( *streams[i] );

            // Since succesfull removal of a stream has decreased the amount
            // of streams in array by one, we have to modify the index
            --i;
            }
        }
        
    if ( videoInStream )
        {
        SetMediaSdpLinesL( *videoInStream, ETrue );
        
        // Complete stream
        MusEngMceUtils::AddDisplayL( *videoInStream, *iManager, Rect() );
        
        static_cast<CMceRtpSource*>(videoInStream->Source())->UpdateL( 
                                                KMusEngJitterBufferLength,
                                                KMusEngJitterBufferTreshold );
                                                 
        localBundle->AddStreamL( *videoInStream );
        }
    else
        {
        User::Leave( KErrCorrupt );
        }

    // Next line is to cause a leave if there were no incoming video streams
    MusEngMceUtils::GetVideoInStreamL( *iSession );

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
    SetSpeakerVolumeL( LcVolumeL() );

    iSipProfileHandler->CreateProfileL( iSession->Profile() );
    
    iRemoteVideoPlayer->SetMceSession( iSession );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::CompleteSessionStructureL()" )
    }
    
// -----------------------------------------------------------------------------
// Parse P-Asserted-Identity Header
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::ParseAssertedIdentity( 
                                TMceTransactionDataContainer* aContainer )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::AssertedIdentity()" )
    
    iIdentity.Zero();
    
    if ( !aContainer )
        {
        MUS_LOG( "mus: [ENGINE]  Data container is NULL" )  
        MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::AssertedIdentity()" )  
        return;
        }
    
    CDesC8Array* headers = aContainer->GetHeaders();
    
    if ( !headers )
        {
        MUS_LOG( "mus: [ENGINE]  No headers" )  
        MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::AssertedIdentity()" )  
        return;
        }    
    
    TBool found = EFalse;
    TInt length = 0;
    for( TInt i = 0; i < headers->Count() && !found; ++i )
        {
        TPtrC8 header = (*headers)[i];
        if ( header.FindF( KMusEngAssertedIdentity ) != KErrNotFound )
            {
            TInt pos = 0;
            // check for sip url, continue if found because maybe tel url exists
            if ( ( pos = header.FindF( KMusEngSipPrefix ) ) != KErrNotFound )
                {
                TInt atPos = header.FindF( KMusEngAtSign );
                length = atPos - pos - KMusEngSipPrefix().Length();
                if ( length > 0 && length <= KMaxUriLength )
                    {
                    iIdentity.Copy( 
                        header.Mid( pos + KMusEngSipPrefix().Length(), length ) );    
                    }
                }
            // check for tel url, exit if found
            else if ( ( pos = header.FindF( KMusEngTelPrefix ) ) != KErrNotFound )
                {
                length = header.Length() - pos - KMusEngTelPrefix().Length();
                if ( length > 0 && length <= KMaxUriLength )
                    {
                    iIdentity.Copy( header.Right( length ) );    
                    }
                found = ETrue;
                }
            }
        }

    // remove ending '>' if exists and whitespaces
    if ( iIdentity.Length() > 0 )
        {
        iIdentity.Trim();
        if ( iIdentity.Find( KMusEngBiggerSign ) == iIdentity.Length() - 1 )
            {
            iIdentity = iIdentity.Left( iIdentity.Length() - 1 );
            iIdentity.Trim();
            }
        }

    delete headers;  
    MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::AssertedIdentity()" )      
    }

// -----------------------------------------------------------------------------
// RTP Inactivity timeout event
// Once inactivity timeout occurs, state change is notified and receiving
// and buffering statuses are cleared. That is safe to do as once receiving
// again continues for real, buffering and streaming events will occur always 
// sequentially. If buffering status would not be cleared, some unwanted
// sreaming events would be passed towards client when it disables/enables
// display sink of remote stream while inactivity timer has expired.
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::InactivityTimeout( CMceMediaStream& aStream,
                                                  CMceRtpSource& /*aSource*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngTwoWayRecvSession::InactivityTimeout()" )
    
    if ( aStream.Type() == KMceVideo && iReceiving )
        {
        iReceiving = EFalse;
        iBuffered = EFalse;
        InformObserverAboutPlayerStateChange( iRemoteVideoPlayer );
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngTwoWayRecvSession::InactivityTimeout()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngReceiveSession::ReceivingStarted()
    {
    if ( iSession && iBuffered )
        {
        MUS_LOG( "mus: [ENGINE]  -> CMusEngReceiveSession::ReceivingStarted()" )
        
        TInt err = MusEngMceUtils::EnableInactivityTimer( 
                *iSession, iReceivingInactivityTimeout );
        
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
        
        MUS_LOG( "mus: [ENGINE]  <- CMusEngReceiveSession::ReceivingStarted()" )
        }
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CMusEngReceiveSession::IsDisplayActive()
    {
    return ( IsDisplayEnabled() && iReceiving );
    }
// End of File
