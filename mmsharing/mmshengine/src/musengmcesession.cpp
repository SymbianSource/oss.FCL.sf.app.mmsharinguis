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
#include "musengmceutils.h"
#include "musuid.hrh"
#include "musenglogger.h"
#include "mussipprofilehandler.h"
#include "mussessionproperties.h"
#include "musenguriparser.h"
#include "musengtelephoneutils.h"

// SYSTEM
#include <lcsessionobserver.h>
#include <lcuiprovider.h>
#include <lcvideoplayer.h>
#include <mcedefs.h>
#include <mcemanager.h>
#include <mceinsession.h>
#include <mcevideostream.h>
#include <mceaudiostream.h>
#include <mcertpsink.h>
#include <mcedisplaysink.h>
#include <mcespeakersink.h>
#include <mcefilesource.h>
#include <mcecamerasource.h>
#include <mcertpsource.h>
#include <mceaudiocodec.h>
#include <mceavccodec.h>
#include <AudioPreference.h>
#include <e32property.h>
#include <cntitem.h>
#include <cntfield.h>
#include <cntdef.h>
#include <cntfldst.h>


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

// Names of AVC levels in string for config keys stored in CenRep 
_LIT8( KMusAvcBitrateLevel1, "AvcBrL1=" );
_LIT8( KMusAvcBitrateLevel1b, "AvcBrL1b=" );
_LIT8( KMusAvcBitrateLevel1_1, "AvcBrL1_1=" );
_LIT8( KMusAvcBitrateLevel1_2, "AvcBrL1_2=" );
_LIT8( KMusAvcBitrateLevel1_3, "AvcBrL1_3=" );
_LIT8( KMusAvcBitrateLevel2, "AvcBrL2=" );

_LIT8( KMusEncoderInfoTokenizer, ";" );


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngMceSession::CMusEngMceSession()
    : iSecondsFromLastRtcpReport ( 0 ),
      // Although speaker is constructed as muted, it is not explicitly muted
      iExplicitlyMuted( EFalse ),
      iBackground( ETrue )
    {
    iMceManagerUid.iUid = KMusUiUid;
    iBackground = IsBackgroundStartup();
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::ConstructL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::ConstructL()" )
    
    iTelephoneUtils = CMusEngTelephoneUtils::NewL( *this );

    // Creating new MCE Manager and set all needed observers to this class.
    iManager = CMceManager::NewL( iMceManagerUid,
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
    
    iRemoteDetails = HBufC::NewL( RProperty::KMaxPropertySize );
    iRemoteDetails->Des().Copy( KNullDesC );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::ConstructL()" )
    }

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
    delete iTelephoneUtils;
    delete iRemoteDetails;
    
    RProperty::Set( NMusSessionApi::KCategoryUid, 
                    NMusSessionApi::KFastMode, 
                    MusSettingsKeys::EFastModeDisabled );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::~CMusEngMceSession()" )
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
TRect CMusEngMceSession::Rect() const
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngMceSession::Rect()" )
    return iRect;
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetRectL( const TRect& aRect )
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngSession::SetRectL()" )
    iRect = aRect;
    RectChangedL();
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetSecondaryRectL( const TRect& aSecondaryRect )
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngMceSession::SetSecondaryRectL()" )
    
    // Same rect is set by primary and secondary setters
    iRect = aSecondaryRect;
    RectChangedL();
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
TRect CMusEngMceSession::SecondaryRect() const
    {
    MUS_LOG( "mus: [ENGINE]  CMusEngMceSession::SecondaryRect()" )
    return iRect;
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
MMusEngDisplayHandler::TDisplayOrientation CMusEngMceSession::OrientationL()
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
void CMusEngMceSession::SetOrientationL( TDisplayOrientation aOrientation )
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
void CMusEngMceSession::EnableDisplayL( TBool aEnable )
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
TBool CMusEngMceSession::IsDisplayEnabled()
    {
    TBool enabled( EFalse );
    if ( iSession )
        {
        CMceDisplaySink* display = 
           MusEngMceUtils::GetDisplay( *iSession, ETrue );
        if ( display )
           {
           enabled = display->IsEnabled();
           }
        }
    return enabled;
    }

// -----------------------------------------------------------------------------
// From MMusEngDisplayHandler
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsDisplayActive()
    {
    return ( IsDisplayEnabled() );
    }

// -----------------------------------------------------------------------------
// From MMusEngAudioRoutingObserver
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::AudioRoutingChanged()
    {
    InformObserverAboutPlayerUpdate( LocalVideoPlayer() );
    InformObserverAboutPlayerUpdate( RemoteVideoPlayer() );
    }

// -----------------------------------------------------------------------------
// From MLcSession 
// -----------------------------------------------------------------------------
//
MLcSession::TLcSessionState CMusEngMceSession::LcSessionState() const
    {
    TLcSessionState lcSessionState = MLcSession::EUninitialized;

    if ( iSession )
        {
        switch( iSession->State() )
            {
            case CMceSession::EIdle: 
                {
                lcSessionState = MLcSession::EInitialized;
                break;
                }
            case CMceSession::EIncoming:
            case CMceSession::EProceeding:
            case CMceSession::EReserving:
                {
                lcSessionState = MLcSession::EReceived;
                break;
                }
            
            case CMceSession::EOffering:
            case CMceSession::EAnswering:
                {
                lcSessionState = MLcSession::EOpening;
                break;
                }
            case CMceSession::EEstablished:
                {
                lcSessionState = MLcSession::EOpen;
                break;
                }
            case CMceSession::ECancelling:
            case CMceSession::ETerminating:
                {
                lcSessionState = MLcSession::EClosing;
                break;
                }
            case CMceSession::ETerminated:
                {
                lcSessionState = MLcSession::EClosed;
                break;
                }
            default:
               {
               lcSessionState = MLcSession::EUninitialized;
               break;
               }
            }
        }
    return  lcSessionState;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetLcSessionObserver( MLcSessionObserver* aObserver )
    {
    iLcSessionObserver = aObserver;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetLcUiProvider( MLcUiProvider* aUiProvider )
    {
    iLcUiProvider = aUiProvider;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngMceSession::RemoteVideoPlayer()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* CMusEngMceSession::LocalVideoPlayer()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
const TDesC& CMusEngMceSession::LocalDisplayName()
    {
    return KNullDesC;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
const TDesC& CMusEngMceSession::RemoteDisplayName()
    {
    return KNullDesC;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TInt CMusEngMceSession::SetParameter( TInt /*aId*/, TInt /*aValue*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TInt CMusEngMceSession::ParameterValue( TInt /*aId*/ )
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsBackgroundStartup()
    {
    TInt fastMode;
    TInt error = RProperty::Get( NMusSessionApi::KCategoryUid, 
                                 NMusSessionApi::KFastMode, 
                                 fastMode );
    
    return ( error == KErrNone && 
             iBackground &&
             fastMode == MusSettingsKeys::EFastModeOn );
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TInt CMusEngMceSession::SetForegroundStatus( TBool aIsForeground )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngMceSession::SetForegroundStatus() %d", 
              aIsForeground )

    if ( aIsForeground )
        {
        // Only clear the bg info, do not ever enable it again
        iBackground = EFalse;
        }
    
    return KErrNone;
    }
 
// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//   
const TDesC& CMusEngMceSession::RemoteDetails()
    {
    // Don't care about error, empty value will be used in that case
    TPtr ptrDetails( iRemoteDetails->Des() );
    RProperty::Get( NMusSessionApi::KCategoryUid,
                    NMusSessionApi::KTelNumber,
                    ptrDetails ); 
    return *iRemoteDetails;
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//   
void CMusEngMceSession::UpdateLcSessionL()
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
        
    //In Mus engine, every update of player has immediate effect. Thus, this method
    //is empty.
    }

// -----------------------------------------------------------------------------
// From MLcSession
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::SendDialTone( TChar /*aKey*/)
    {
    // Handle Key once implemented
    return ETrue;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsLcAudioMutedL()
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
              
    TBool containsAudio( EFalse );
    for ( TInt i = 0; i < iSession->Streams().Count(); ++i )
        {
        if ( iSession->Streams()[i]->Type() == KMceAudio )
            {
            containsAudio = ETrue;
            }
        }    
    
    if ( containsAudio && !iExplicitlyMuted )
        {
        return EFalse;
        }
    
    return ETrue;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::MuteLcAudioL( TBool aMute )
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    DoMuteSpeakerL( aMute );
    iExplicitlyMuted = aMute;
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsLcMicMutedL()
    {
    return iTelephoneUtils->IsMicMutedL();
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::MuteLcMicL( TBool aMute )
    {
    iTelephoneUtils->MuteMicL( aMute );
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsEnablingLcLoudspeakerAllowed()
    {
    return iTelephoneUtils->AudioRoutingCanBeChanged();
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::EnableLcLoudspeakerL( TBool aEnabled )
    {
    TRAPD( error, iTelephoneUtils->LoudspeakerL( aEnabled ) )

    if ( error != KErrNone )
        {
        // This is writed for emulator use
        if ( error != KErrNotSupported )
            {
            MUS_LOG1( "mus: [ENGINE] CMusEngSession::EnableLoudspeakerL() Leave: #%d",
                      error )
            User::Leave( error );
            }
        // Not leave in emulator if KErrNotSupported
        #if (!defined (__WINS__) && !defined(__WINSCW__))
        User::Leave( error );
        #endif
        }
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsLcLoudspeakerEnabled()
    {
    return iTelephoneUtils->IsLoudSpeakerEnabled();
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
TInt CMusEngMceSession::LcVolumeL()
    {
    return iTelephoneUtils->GetVolumeL();
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetLcVolumeL( TInt aValue )
    {
    iTelephoneUtils->SetVolumeL( aValue );
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::IncreaseLcVolumeL()
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    TInt currentVolume = iTelephoneUtils->GetVolumeL();
    iTelephoneUtils->SetVolumeL( currentVolume  + 1 );
    SetSpeakerVolumeL( LcVolumeL() );
    }

// -----------------------------------------------------------------------------
// From MLcAudioControl
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::DecreaseLcVolumeL()
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    TInt currentVolume = iTelephoneUtils->GetVolumeL();
    iTelephoneUtils->SetVolumeL( currentVolume - 1 );
    SetSpeakerVolumeL( LcVolumeL() );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MLcSessionObserver* CMusEngMceSession::LcSessionObserver()
    {
    return iLcSessionObserver;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MLcUiProvider* CMusEngMceSession::LcUiProvider()
    {
    return iLcUiProvider;
    }

// -----------------------------------------------------------------------------
// Returns estabilished session time. If not established return
// value is < 0
// -----------------------------------------------------------------------------
//
TTimeIntervalSeconds CMusEngMceSession::GetSessionTime() const
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
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutSessionStateChange()
    {
    if ( iLcSessionObserver )
        {
        iLcSessionObserver->StateChanged( *this );
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutSessionUpdate()
    {
    if ( iLcSessionObserver )
        {
        iLcSessionObserver->Updated( *this );
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutSessionFailure( TInt aReason )
    {
    if ( iLcSessionObserver )
        {
        iLcSessionObserver->Failed( *this, aReason );
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutPlayerStateChange( 
    MLcVideoPlayer* aPlayer )
    {
    if ( iLcSessionObserver && aPlayer )
        {
        iLcSessionObserver->StateChanged( *aPlayer );
        }
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformUiProviderAboutReceivingStart()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::InformUiProviderAboutReceivingStart()" )
    if ( iBackground && iLcUiProvider )
        {
        MUS_LOG( "mus: [ENGINE]     receiving started in BG mode, switching to FG" )
        iLcUiProvider->HandleForegroundStatus( ETrue );
        }
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::InformUiProviderAboutReceivingStart()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutPlayerUpdate( 
    MLcVideoPlayer* aPlayer )
    {
    if ( iLcSessionObserver && aPlayer )
        {
        iLcSessionObserver->Updated( *aPlayer );
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::InformObserverAboutPlayerFailure( 
    MLcVideoPlayer* aPlayer, 
    TInt aReason )
    {
    if ( iLcSessionObserver && aPlayer )
        {
        iLcSessionObserver->Failed( *aPlayer, aReason );
        }
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
MLcUiProvider& CMusEngMceSession::LcUiProviderL()
    {
    __ASSERT_ALWAYS( iLcUiProvider, User::Leave( KErrNotReady ) );
    return *iLcUiProvider;
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::AddSdpBandwidthAttributesL( CDesC8Array& aSdpLinesArray,
                                                    TInt aBandwidthAs )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::AddSdpBandwidthAttributesL()" )
    const TInt KMusNumberMaxLength = 10;
    HBufC8* bandwidth = HBufC8::NewLC( 
            KMusEngSessionSdpLineTiasLine().Length() + KMusNumberMaxLength );
    
    bandwidth->Des().Copy( KMusEngSessionSdpLineBandwidthLine() );
    bandwidth->Des().AppendNum( aBandwidthAs );
    bandwidth->Des().Append( KMusEngNewLine() );
    aSdpLinesArray.AppendL( *bandwidth );
    
    bandwidth->Des().Copy( KMusEngSessionSdpLineTiasLine() );
    bandwidth->Des().AppendNum( aBandwidthAs * KMusTiasMultiplier );
    bandwidth->Des().Append( KMusEngNewLine() );
    aSdpLinesArray.AppendL( *bandwidth );
    
    CleanupStack::PopAndDestroy( bandwidth );  
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::AddSdpBandwidthAttributesL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TBool CMusEngMceSession::IsBandwidthAttributeSet( MDesC8Array* aSdpLinesArray )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::IsBandwidthAttributeSet()" )
    TBool ret =
        ContainsText( aSdpLinesArray, KMusEngSessionSdpLineTiasLine() ) ||
        ContainsText( aSdpLinesArray, KMusEngSessionSdpLineBandwidthLine() );
    MUS_LOG1( "mus: [ENGINE]  <- CMusEngMceSession::IsBandwidthAttributeSet(): %d",
            ret )
    return ret;
    }

// -----------------------------------------------------------------------------
// Setting session level sdp lines. Bandwith attribute is always used in
// operator variant mode and at receiver side (using force) when TIAS
// usage is disabled. However, bandwidth attribute is preferred to be used 
// at media level (see SetMediaSdpLinesL method). It is set to session level
// only if other side is using also session level bandwidth. Media level
// preference exists because some other manufacturer's videosharing does not
// understand session level bandwidth attribute.
// In non-operator variant mode and when TIAS usage is enabled, both AS and 
// TIAS are added to session level only if the other side is using session 
// level bandwidth.
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
    TBool tiasAtSessionLevel( ContainsText( oldSessionSdpLines, 
                                   KMusEngSessionSdpLineTiasLine() ) );    
    CleanupStack::PopAndDestroy( oldSessionSdpLines );
  
    TInt bandwidthAs = 0;
    TRAPD( err, bandwidthAs = 
                MultimediaSharingSettings::VideoBandwidthSettingL() );
    __ASSERT_ALWAYS( err == KErrNone || err == KErrNotFound,
                     User::Leave( err ) );
    TBool useTias = ( bandwidthAs > 0 );

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
        
    if ( bandwidthAtSessionLevel && ( iOperatorVariant || 
         ( aForceBandwidthLine && !useTias ) ) )
        {        
        MUS_LOG( "mus: [ENGINE] setting bandwidth to session level" )
        newSessionSDPLines->AppendL( KMusEngSessionSdpLineBandwidthField() );
        }
    else if ( !iOperatorVariant && useTias && aForceBandwidthLine &&
            ( bandwidthAtSessionLevel || tiasAtSessionLevel ) )
        {
        MUS_LOG( "mus: [ENGINE] setting b=AS and b=TIAS to session level" )
        AddSdpBandwidthAttributesL( *newSessionSDPLines, bandwidthAs );
        }
   
    aSession.SetSessionSDPLinesL( newSessionSDPLines );
    
    CleanupStack::Pop( newSessionSDPLines );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::SetSessionSdpLinesL()" )
    }


// -----------------------------------------------------------------------------
// Setting media level sdp lines. In operator variant mode or when TIAS usage
// is disabled bandwidth is not set to media level if it is used already at 
// session level. In non operator variant and TIAS enabled case both AS and
// TIAS attributes are set to media level if: 1)it is an outcoming sessions
// or 2) incoming session had bandwidth attributes on media level or didn't
// have bandwidth attributes at all
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

    TInt bandwidthAs = 0;
    TRAPD( error, bandwidthAs = 
                  MultimediaSharingSettings::VideoBandwidthSettingL() );
    __ASSERT_ALWAYS( error == KErrNone || error == KErrNotFound,
                     User::Leave( error ) );
    TBool useTias = ( bandwidthAs > 0 );
    TBool tiasOrAsAtSessionLevel = EFalse;
    TBool tiasOrAsAtMediaLevel = EFalse;
    if ( useTias )
        {
        tiasOrAsAtSessionLevel = IsBandwidthAttributeSet( sessionSdpLines );
        MDesC8Array* mediaSdpLines = aStream.MediaAttributeLinesL();
        tiasOrAsAtMediaLevel = IsBandwidthAttributeSet( mediaSdpLines );
        delete mediaSdpLines;        
        }

    CleanupStack::PopAndDestroy( sessionSdpLines );

    TBool setTiasAtMediaLevel = ( tiasOrAsAtMediaLevel || 
                                ( aForceBandwidthLine && 
                                  !tiasOrAsAtSessionLevel ) );
  
    const TInt KMusMediaSdpLinesGranularity = 2;
    CDesC8Array* headers = 
        new ( ELeave ) CDesC8ArrayFlat( KMusMediaSdpLinesGranularity );
    CleanupStack::PushL( headers );
    
    if ( !bandwidthAtSessionLevel && ( iOperatorVariant || 
         ( aForceBandwidthLine && !useTias ) ) )
        {
        MUS_LOG( "mus: [ENGINE] setting bandwidth to media level" )
        headers->AppendL( KMusEngSessionSdpLineBandwidthField() );
        }
    else if ( !iOperatorVariant && useTias && setTiasAtMediaLevel )
        {
        MUS_LOG( "mus: [ENGINE] setting b=AS and b=TIAS to media level" )
        AddSdpBandwidthAttributesL( *headers, bandwidthAs );
        }

    aStream.SetMediaAttributeLinesL( headers );   
    CleanupStack::Pop( headers );

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
                 aNewVolume >= 1 &&
                 aNewVolume <= speaker->MaxVolumeL() )
                {
                speaker->SetVolumeL( aNewVolume );
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
    
    __ASSERT_ALWAYS( aVideoStream.Source(), User::Leave( KErrArgument ) );
    for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
        {
        AdjustVideoCodecL( *codecs[codecIndex], aVideoStream.Source()->Type() );
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
            // NOP
            break;
            }
        case CMceMediaStream::EStreaming: // Stream is streaming
            {
            StreamStreaming( aStream );
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
        InformObserverAboutSessionFailure( KErrGeneral );
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
            InformObserverAboutSessionFailure( MLcSession::EConnectionLost );
            }
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::\
             SessionConnectionStateChanged()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::Failed( CMceSession& aSession, TInt aError )
    {
    MUS_LOG1( "mus: [ENGINE]     -> CMusEngMceSession::Failed() error #%d", 
              aError )
    
    if ( iSession && iSession == &aSession )
        {
        MUS_LOG( "mus: [ENGINE]    CMusEngMceSession::Failed: Notify observer" )
        InformObserverAboutSessionFailure( aError );
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
        InformObserverAboutSessionFailure( KErrGeneral );
        return;
        }

    if ( iSession && iSession == &aSession )
        {
        MUS_LOG( "mus: [ENGINE]     CMusEngMceSession::UpdateFailed: \
                 Notify observer" )
        InformObserverAboutSessionFailure( KErrGeneral );
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
        InformObserverAboutPlayerStateChange( RemoteVideoPlayer() );
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
void CMusEngMceSession::ProfileRegistered()
    {
    // NOP in base class
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
            case CMceSession::EIncoming:
                {
                MUS_LOG( "mus: [ENGINE] Unexpected asynchronous \
                         state transition, consider session as failed." )
                InformObserverAboutSessionFailure( KErrGeneral );
                break;
                }
            case CMceSession::EEstablished:
                {
                if ( iStoreEncoderConfigInfo )
                    {
                    iStoreEncoderConfigInfo = EFalse;
                    TRAPD( error, StoreEncoderConfigInfoL() )
                    if ( error != KErrNone && error != KErrNotFound )
                        {
                        // Only acceptable error is absence of repository entry,
                        // otherwise we inform user about failed session.
                        InformObserverAboutSessionFailure( error );
                        return;
                        }
                    }
                
                // Check that session timer is not already running, which is 
                // the case when refreshing the session with session timer 
                if ( !iUpdateTimer->IsActive() )
                    {
                    iStartTime.HomeTime(); // Start counting session duration              
                    iUpdateTimer->Start( KMusEngTimerInterval );
                    InformObserverAboutSessionStateChange();
                    }
                
                break;
                }
            case CMceSession::EOffering:
            case CMceSession::EReserving:
            case CMceSession::EAnswering:
            case CMceSession::EProceeding:              
            case CMceSession::ECancelling:
            case CMceSession::ETerminating:
                {
                // NOP
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
            InformObserverAboutSessionStateChange();
            break;
            }
        case KMusEngSipReasonCodeOk:
            {
            // Normal session termination by this end: We have sent BYE
            // and now received 200 OK to it.
            InformObserverAboutSessionStateChange();
            break;
            }
        default:
            {
            // Termination reason cannot be determined, handle as internal
            // error.
            InformObserverAboutSessionFailure( KErrGeneral );
            break;
            }
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::HandleTermination()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::AdjustVideoCodecL( CMceVideoCodec& aVideoCodec,
                                           TMceSourceType aSourceType )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::AdjustVideoCodecL()" )
    
    aVideoCodec.SetMMFPriorityL( KAudioPrioritySwisPlayback );
    aVideoCodec.SetMMFPriorityPreferenceL( KAudioPrefSwisPlayback );

    if ( aVideoCodec.SdpName() == KMceSDPNameH264() &&
         aSourceType == KMceCameraSource )
        {
        SetCodecConfigKeyL( aVideoCodec );
        SetEncodingDeviceL( aVideoCodec );     
        }
    
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
    if ( bestBitrateCodec )
        {
        DoBitrateBasedRemovalL( aVideoStream, *bestBitrateCodec );
        }
   
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

    if ( iSecondsFromLastRtcpReport >= KMusEngRtcpInactivityThreshold )
        {
        MUS_LOG( "mus: [ENGINE] CMusEngMceSession::UpdateTimerEvent() ->EMediaInactivityTimeout" )
        InformObserverAboutSessionFailure( MLcSession::EMediaInactivityTimeout );
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
        CMceSpeakerSink* speaker = MusEngMceUtils::GetSpeaker( *streams[i] );
            
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
void CMusEngMceSession::SetEncodingDeviceL( CMceVideoCodec& aVideoCodec )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngMceSession::SetEncodingDeviceL()" )

    // We try to read encoding device UID. If we do not succeed, we use default
    
    const TUid KDefaultEncodingDevice( TUid::Uid( 0x20001C13 ) );
    TUid encodingDevice( KDefaultEncodingDevice );
    TRAPD( error, 
           encodingDevice = MultimediaSharingSettings::EncodingDeviceL() )
    __ASSERT_ALWAYS( error == KErrNone || error == KErrNotFound, 
                     User::Leave( error ) );
    aVideoCodec.SetPreferredEncodingDecodingDeviceL( encodingDevice );
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngMceSession::SetEncodingDeviceL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SetCodecConfigKeyL( CMceVideoCodec& aVideoCodec )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngMceSession::SetCodecConfigKeyL()" )
    
    HBufC8* configKey( NULL );
        
    // We try to read config key. If we do not succeed, we prepare to
    // write keys to CenRep for the next time.
    TRAPD( error, configKey = ReadCodecConfigKeyL( aVideoCodec ) )
    
    if ( error == KErrNone )
        {
        // There is a repository for config keys
        if ( configKey )
            {
            // ...and key is found.
            CleanupStack::PushL( configKey );
            aVideoCodec.SetConfigKeyL( *configKey );
            CleanupStack::PopAndDestroy( configKey );
            }
        else
            {
            // ...but key is not found, so it must be written to CenRep when
            // available. 
            iStoreEncoderConfigInfo = ETrue;
            }
        }
    else if ( error == KErrNotFound )
        {
        // There is no repository for config keys so there is no point
        // trying to write anything there. Session setup can still continue.
        MUS_LOG1( "mus: [ENGINE]    No repository for config keys [%d]", error )
        }            
    else
        {
        User::Leave( error );
        }
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngMceSession::SetCodecConfigKeyL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC8* CMusEngMceSession::ReadCodecConfigKeyL( 
    const CMceVideoCodec& aVideoCodec ) const
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngMceSession::ReadCodecConfigKeyL()" )
    
    HBufC8* key( NULL );
 
    HBufC8* encoderInfo = MultimediaSharingSettings::EncoderConfigInfoLC();
    __ASSERT_ALWAYS( encoderInfo, User::Leave( KErrNotFound ) );
    
    TPtrC8 keyId( ConfigKeyIdL( aVideoCodec ) ); 
    
    TInt tokenStartPosition( encoderInfo->FindC( keyId ) );
    if ( tokenStartPosition > KErrNotFound )
        {
        TInt tokenLength( encoderInfo->Mid( tokenStartPosition ).FindC( 
                                                KMusEncoderInfoTokenizer ) );
        if ( tokenLength > KErrNotFound )
            {
            // Separate key from token by removing keyId
            TInt keyStartPosition = tokenStartPosition + keyId.Length();
            TInt keyLength = tokenLength - keyId.Length(); 
            TPtrC8 keyPtr = encoderInfo->Mid( keyStartPosition, keyLength );
            key = keyPtr.AllocL();
            }
        }
    
    CleanupStack::PopAndDestroy( encoderInfo );
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngMceSession::ReadCodecConfigKeyL()" )
        
    return key;   
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::StoreEncoderConfigInfoL() const
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngMceSession::StoreEncoderConfigInfoL()" )
    
    HBufC8* configInfoInCenRep = MultimediaSharingSettings::EncoderConfigInfoLC();
    __ASSERT_ALWAYS( configInfoInCenRep, User::Leave( KErrNotFound ) );
    
    TBuf8<NCentralRepositoryConstants::KMaxBinaryLength> keys;
    keys.Append( *configInfoInCenRep );
    
    CMceVideoStream* stream = MusEngMceUtils::GetVideoOutStreamL( *iSession );
    const RPointerArray<CMceVideoCodec>& codecs = stream->Codecs();
    
    for ( TInt codecIndex = 0; codecIndex < codecs.Count(); ++codecIndex )
        {
        if ( codecs[codecIndex]->SdpName().FindF( KMceSDPNameH264 ) >= 0 )
            {
            const TPtrC8 keyId = ConfigKeyIdL( *codecs[codecIndex] );
            HBufC8* configKey = codecs[codecIndex]->ConfigKeyL();
            CleanupStack::PushL( configKey );
            
            if ( configKey &&
                 keys.FindF( keyId ) == KErrNotFound &&
                 configInfoInCenRep->FindF( keyId ) == KErrNotFound )
                {
                
                if ( keys.Length() + 
                     keyId.Length() + 
                     configKey->Length() +
                     KMusEncoderInfoTokenizer().Length() <
                     NCentralRepositoryConstants::KMaxBinaryLength )
                    {
                    keys.Append( keyId );                    
                    keys.Append( *configKey );
                    keys.Append( KMusEncoderInfoTokenizer );
                    }
                }
                
            CleanupStack::PopAndDestroy( configKey );
            }
        }
    
    MultimediaSharingSettings::SetEncoderConfigInfoL( keys );    
    
    CleanupStack::PopAndDestroy( configInfoInCenRep );
        
    MUS_LOG( "mus: [ENGINE] <- CMusEngMceSession::StoreEncoderConfigInfoL()" )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TPtrC8 CMusEngMceSession::ConfigKeyIdL( 
                                    const CMceVideoCodec& aVideoCodec ) const
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngMceSession::ConfigKeyIdL()" )
    
    // Currently works only for AVC
    __ASSERT_ALWAYS( aVideoCodec.SdpName().FindF( KMceSDPNameH264 ) >= 0,
                     User::Leave( KErrArgument ) );
    
    TPtrC8 configKeyTokenId;
    
    if ( aVideoCodec.AllowedBitrates() & KMceAvcBitrateLevel1 )
        {
        configKeyTokenId.Set( KMusAvcBitrateLevel1() );
        }
    else if ( aVideoCodec.AllowedBitrates() & KMceAvcBitrateLevel1b )
        {
        configKeyTokenId.Set( KMusAvcBitrateLevel1b() );
        }
    else if ( aVideoCodec.AllowedBitrates() & KMceAvcBitrateLevel1_1 )
        {
        configKeyTokenId.Set( KMusAvcBitrateLevel1_1() );
        }
    else if ( aVideoCodec.AllowedBitrates() & KMceAvcBitrateLevel1_2 )
        {
        configKeyTokenId.Set( KMusAvcBitrateLevel1_2() );
        }
    else if ( aVideoCodec.AllowedBitrates() & KMceAvcBitrateLevel1_3 )
        {
        configKeyTokenId.Set( KMusAvcBitrateLevel1_3() );
        }
    else if ( aVideoCodec.AllowedBitrates() & KMceAvcBitrateLevel2 )
        {
        configKeyTokenId.Set( KMusAvcBitrateLevel2() );
        }
    else
        {
        User::Leave( KErrNotFound );
        }
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngMceSession::ConfigKeyIdL()" )

    return configKeyTokenId;
    }

// -----------------------------------------------------------------------------
// Contact address is saved if all following apply:
// - standard variant
// - only one contact entry (mus mgr doesn't provide contact id if multiple
//   contact entries for other end is found)
// - no existing sip address info for contact
// - address is valid sip or tel uri
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::SaveContactL( const TDesC8& /*aAddress*/ )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngMceSession::SaveContactL" );
    
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngMceSession::SaveContactL" );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngMceSession::StreamStreaming( CMceMediaStream& aStream )
    {
    if ( aStream.State() == CMceMediaStream::EStreaming &&
         aStream.Type() == KMceVideo )
        {
        MLcVideoPlayer* videoPlayer = NULL;
        if ( aStream.Source() && 
             aStream.Source()->Type() == KMceRTPSource )
            {
            videoPlayer = RemoteVideoPlayer();
            }
        else if ( aStream.Sinks().Count() >= 0 &&
                  aStream.Sinks()[0]->Type() == KMceRTPSink )
            {
            videoPlayer = LocalVideoPlayer();
            }
        else
            {
            }
        InformObserverAboutPlayerStateChange( videoPlayer );
        }
    }

// End of file
