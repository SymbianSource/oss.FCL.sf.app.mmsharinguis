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
#include "musengsessionobserver.h"
#include "musenglivesessionobserver.h"
#include "musunittesting.h"
#include "musengmceutils.h"
#include "musenglogger.h"
#include "mussettings.h"

// SYSTEM
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

const TInt KMaxBrightness = 100;
const TInt KMinBrightness = -100;
const TInt KBrightnessStepSize = 10;

// Names of AVC levels in string for config keys stored in CenRep 
_LIT8( KMusAvcBitrateLevel1, "AvcBrL1=" );
_LIT8( KMusAvcBitrateLevel1b, "AvcBrL1b=" );
_LIT8( KMusAvcBitrateLevel1_1, "AvcBrL1_1=" );
_LIT8( KMusAvcBitrateLevel1_2, "AvcBrL1_2=" );
_LIT8( KMusAvcBitrateLevel1_3, "AvcBrL1_3=" );
_LIT8( KMusAvcBitrateLevel2, "AvcBrL2=" );

_LIT8( KMusEncoderInfoTokenizer, ";" );

//Number of big and small zoom steps on Zoom scale
const TInt KZoomBigStepCount = 15;
const TInt KZoomSmallStepCount = KZoomBigStepCount*2;
const TInt KZoomStepMinSize = 1;

const TInt64 KZoomFasterTime = 333333; // 1/3 second

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusEngLiveSession* CMusEngLiveSession::NewL(
                        const TDesC& aFileName,
                        const TRect& aRect,
                        MMusEngSessionObserver& aSessionObserver,
                        MMusEngOutSessionObserver& aOutSessionObserver,
                        MMusEngLiveSessionObserver& aLiveSessionObserver,
                        TUint aSipProfileId )
    {
    CMusEngLiveSession* self = new( ELeave ) CMusEngLiveSession( 
                                                    aSessionObserver,
                                                    aOutSessionObserver,
                                                    aLiveSessionObserver, 
                                                    aRect,
                                                    aFileName );
    CleanupStack::PushL( self );
    self->ConstructL( aSipProfileId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMusEngLiveSession* CMusEngLiveSession::NewL(
                        const TRect& aRect,
                        MMusEngSessionObserver& aSessionObserver,
                        MMusEngOutSessionObserver& aOutSessionObserver,
                        MMusEngLiveSessionObserver& aLiveSessionObserver,
                        TUint aSipProfileId )
    {
    CMusEngLiveSession* self = new( ELeave ) CMusEngLiveSession( 
                                                    aSessionObserver,
                                                    aOutSessionObserver,
                                                    aLiveSessionObserver, 
                                                    aRect );
    CleanupStack::PushL( self );
    self->ConstructL( aSipProfileId );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession::~CMusEngLiveSession()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::~CMusEngLiveSession()" )
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::~CMusEngLiveSession()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMusEngLiveSession::CurrentZoomL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::CurrentZoomL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    TInt currentZoom = camera->ZoomFactorL() + camera->DigitalZoomFactorL();

    MUS_LOG1( "mus: [ENGINE]  <- CMusEngLiveSession::CurrentZoomL( %d )",
              currentZoom )

    return currentZoom;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::SetZoomL( TInt aNewZoomFactor )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngLiveSession::SetZoomL( %d )", 
              aNewZoomFactor )
              
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    __ASSERT_ALWAYS( aNewZoomFactor <= MaxZoomL() &&
                     aNewZoomFactor >= MinZoomL(), 
                     User::Leave( KErrArgument ) );
    
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    if ( aNewZoomFactor <= iCameraInfo.iMaxZoom )
        {
        MUS_LOG( "mus: [ENGINE]     Optical zoom factor increased" )
        camera->SetZoomFactorL( aNewZoomFactor ); 
        }
    
    if ( aNewZoomFactor - iCameraInfo.iMaxZoom > 0 )
        {
        camera->SetDigitalZoomFactorL( aNewZoomFactor - iCameraInfo.iMaxZoom );
        MUS_LOG1( "mus: [ENGINE]     Digital zoom factor increased to %d",
                  aNewZoomFactor - iCameraInfo.iMaxZoom )
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::SetZoomL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMusEngLiveSession::MinZoomL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::MinZoomL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    // Although we do not need camera for return value, we have to have it in
    // order to have proper iCameraInfo
    MusEngMceUtils::GetCameraL( *iSession );

    MUS_LOG1( "mus: [ENGINE]  <- CMusEngLiveSession::MinZoomL( %d )",
              iCameraInfo.iMinZoom )

    return iCameraInfo.iMinZoom;
    }
    
        
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMusEngLiveSession::MaxZoomL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::MaxZoomL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    // Although we do not need camera for return value, we have to have it in
    // order to have proper iCameraInfo
    MusEngMceUtils::GetCameraL( *iSession );

    TInt maxZoom = iCameraInfo.iMaxZoom + iCameraInfo.iMaxDigitalZoom;

    MUS_LOG1( "mus: [ENGINE]     Max optical zoom( %d )",
              iCameraInfo.iMaxZoom )
    MUS_LOG1( "mus: [ENGINE]     Max digital zoom( %d )",
              iCameraInfo.iMaxDigitalZoom )
    MUS_LOG1( "mus: [ENGINE]  <- CMusEngLiveSession::MaxZoomL( %d )",
              maxZoom )

    return maxZoom;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::ZoomInL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::ZoomInL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    // First try to increase optical zoom factor. If in maximum value, try to
    // increase digital zoom factor.
    TInt stepSize = ZoomStepSize( iZoomInTime );
    TInt zoomFactor = camera->ZoomFactorL();
    TInt zoomDigitalFactor = camera->DigitalZoomFactorL();
 
    //increate optical zoom factor
    if ( zoomFactor + stepSize <= iCameraInfo.iMaxZoom )
        {
        //optical zoom factor is enough
        camera->SetZoomFactorL( zoomFactor + stepSize );
        stepSize = 0;
        }
    else if (zoomFactor <  iCameraInfo.iMaxZoom)
       {
            stepSize -=  iCameraInfo.iMaxZoom - zoomFactor;
            camera->SetZoomFactorL( iCameraInfo.iMaxZoom );
       }

    //increate digital zoom factor
    if (stepSize > 0)
        {
        if ( zoomDigitalFactor + stepSize <= iCameraInfo.iMaxDigitalZoom )
            {
            camera->SetDigitalZoomFactorL( zoomDigitalFactor + stepSize );
            }
        else 
            {
            camera->SetDigitalZoomFactorL( iCameraInfo.iMaxDigitalZoom );
            MUS_LOG( "mus: [ENGINE]     CMusEngLiveSession::ZoomInL(): Optical \
                             and digital zoom factors are in maximum value" )
            }
        }
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::ZoomInL()" )

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::ZoomOutL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::ZoomOutL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    // First try to decrease digital zoom factor. If already zero, try to
    // decrease optical zoom factor.
    TInt stepSize = ZoomStepSize( iZoomOutTime );
    TInt zoomFactor = camera->ZoomFactorL();
    TInt zoomDigitalFactor = camera->DigitalZoomFactorL();
    
    //decreate digital zoom factor firstly
    if ( zoomDigitalFactor - stepSize >= 0  )
        {
        //digital zoom factor is enough
        camera->SetDigitalZoomFactorL( zoomDigitalFactor - stepSize );
        stepSize = 0;
        }
    else if ( zoomDigitalFactor > 0 )
        {
        stepSize -= zoomDigitalFactor;
        camera->SetDigitalZoomFactorL( 0 );
        MUS_LOG("mus: [ENGINE] Digigal Factor to zero")
        }
   
    //decreate optical zoom factor firstly
    if ( stepSize > 0 )
        {
        if ( zoomFactor - stepSize > iCameraInfo.iMinZoom )
            {
            camera->SetZoomFactorL( zoomFactor - stepSize );
            }
        else 
            {
            MUS_LOG( "mus: [ENGINE]    CMusEngLiveSession::ZoomOutL(): Optical and \
                        digital zoom factors are in minimum value" )
            camera->SetZoomFactorL( iCameraInfo.iMinZoom );
            }
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::ZoomOutL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::ZoomDefaultL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::ZoomDefaultL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    camera->SetZoomFactorL( iDefaultZoomFactor );
    camera->SetDigitalZoomFactorL( 0 );

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::ZoomDefaultL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::SetBrightnessL( TInt aBrightness ) const
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngLiveSession::SetBrightnessL( %d )",
              aBrightness )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );
    camera->SetBrightnessL( aBrightness );

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::SetBrightnessL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMusEngLiveSession::CurrentBrightnessL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::CurrentBrightnessL()" )
    
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );
    TInt currentBrightness = camera->BrightnessL();

    MUS_LOG1( "mus: [ENGINE]  <- CMusEngLiveSession::CurrentBrightnessL(): %d",
              currentBrightness )

    return currentBrightness;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMusEngLiveSession::MaxBrightnessL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::MaxBrightnessL()" )
    
    // maximum brightness is not in camera info, but a constant 100

    MUS_LOG1( "mus: [ENGINE]  <- CMusEngLiveSession::MaxBrightnessL(): %d",
              KMaxBrightness )

    return KMaxBrightness;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CMusEngLiveSession::MinBrightnessL() const
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::MinBrightnessL()" )
 
    // maximum brightness is not in camera info, but a constant -100

    MUS_LOG1( "mus: [ENGINE]  <- CMusEngLiveSession::MinBrightnessL(): %d",
              KMinBrightness )

    return KMinBrightness;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::IncreaseBrightnessL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::IncreaseBrightnessL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    TInt newBrightness = camera->BrightnessL() + KBrightnessStepSize;
    
    if ( newBrightness < KMaxBrightness )
        {
        camera->SetBrightnessL( newBrightness );
        }
    else
        {
        camera->SetBrightnessL( KMaxBrightness );
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::IncreaseBrightnessL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::DecreaseBrightnessL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::DecreaseBrightnessL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    TInt newBrightness = camera->BrightnessL() - KBrightnessStepSize;

    if ( newBrightness > KMinBrightness )
        {
        camera->SetBrightnessL( newBrightness );
        }
    else
        {
        camera->SetBrightnessL( KMinBrightness );
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::DecreaseBrightnessL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::BrightnessDefaultL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::BrightnessDefaultL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    camera->SetBrightnessL( iDefaultBrightness );

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::BrightnessDefaultL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::BrightnessAutoL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::BrightnessAutoL()" )
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    camera->SetBrightnessL( CCamera::EBrightnessAuto );

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::BrightnessAutoL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::RecordL( TBool aRecord )
    {
    MUS_LOG1( "mus: [ENGINE]  -> CMusEngLiveSession::RecordL( %d )", aRecord )    
    
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceMediaSink* fileSink = MusEngMceUtils::GetMediaSink( *iSession, 
                                                            KMceFileSink );
    
    __ASSERT_ALWAYS( fileSink, User::Leave( KErrNotReady ) );
    
    if( aRecord && !fileSink->IsEnabled() )
        {
        fileSink->EnableL();        
        }
    else if ( !aRecord && fileSink->IsEnabled() )
        {
        fileSink->DisableL();        
        }
    else
        {
        // NOP
        }

    MUS_LOG1( "mus: [ENGINE]  <- CMusEngLiveSession::RecordL( %d )", aRecord ) 
    }    


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngLiveSession::IsRecording()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::IsRecording()" )    
    
    TBool isRecording( EFalse );
  
    CMceMediaSink* fileSink = MusEngMceUtils::GetMediaSink( *iSession, 
                                                            KMceFileSink );
    if ( fileSink )
        {
        isRecording = fileSink->IsEnabled();
        }
        
    MUS_LOG1( "mus: [ENGINE]  <- CMusEngLiveSession::IsRecording(%d)",
              isRecording )
                 
    return isRecording;
    }


// -----------------------------------------------------------------------------
// Enable camera if not already enabled
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::PlayL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::PlayL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    if ( !camera->IsEnabled() )
        {
        camera->EnableL();
        }
    else
        {
        MUS_LOG( "mus: [ENGINE]    Camera already enabled, ignore request" )
        }
        
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::PlayL()" )
    }


// -----------------------------------------------------------------------------
// Disable camera if not already disabled
// -----------------------------------------------------------------------------
//
EXPORT_C void CMusEngLiveSession::PauseL()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::PauseL()" )

    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );

    CMceCameraSource* camera = MusEngMceUtils::GetCameraL( *iSession );

    if ( camera->IsEnabled() )
        {
        camera->DisableL();
        }
    else
        {
        MUS_LOG( "mus: [ENGINE]    Camera already disabled, ignore request" )
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::PauseL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CMusEngLiveSession::IsPlayingL()
    {
    __ASSERT_ALWAYS( iSession, User::Leave( KErrNotReady ) );
    
    return ( MusEngMceUtils::GetCameraL( *iSession )->IsEnabled() );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::InitializeZoomStepSize()
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::InitializeZoomStepSize()" )
    
    iBigZoomStep = ( iCameraInfo.iMaxZoom + iCameraInfo.iMaxDigitalZoom - 
            iCameraInfo.iMinZoom ) / KZoomBigStepCount;
            
    if ( iBigZoomStep < KZoomStepMinSize )
        iBigZoomStep = KZoomStepMinSize;

    iSmallZoomStep = ( iCameraInfo.iMaxZoom + iCameraInfo.iMaxDigitalZoom - 
            iCameraInfo.iMinZoom ) / KZoomSmallStepCount;
    
    if ( iSmallZoomStep < KZoomStepMinSize )
        iSmallZoomStep = KZoomStepMinSize;

    MUS_LOG2( "mus: [ENGINE]  iSmallZoomStep = %d, iBigZoomStep = %d", 
            iSmallZoomStep, iBigZoomStep )
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::InitializeZoomStepSize()" )   
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

    // Create outgoing video stream
    CMceVideoStream* videoStream = CMceVideoStream::NewLC();
    
    CMceRtpSink* rtpsink = CMceRtpSink::NewLC();
    videoStream->AddSinkL( rtpsink );
    CleanupStack::Pop( rtpsink );

    CMceCameraSource* camera = CMceCameraSource::NewLC( *iManager );
    camera->DisableL(); // Start session in pause mode.
    camera->GetCameraInfo( iCameraInfo );
    iDefaultZoomFactor = camera->ZoomFactorL();
    
    InitializeZoomStepSize();
        
    videoStream->SetSourceL( camera );
    CleanupStack::Pop( camera );

    iSession->AddStreamL( videoStream );
    CleanupStack::Pop( videoStream );    

    // Construct recording stream if needed
    if ( iRecordedFile != KNullDesC )
        {
        CMceVideoStream* streamForRecording = CMceVideoStream::NewLC();
        
        CMceFileSink* fileSink = CMceFileSink::NewLC( iRecordedFile );
        fileSink->DisableL(); // Start in not recording mode
        streamForRecording->AddSinkL( fileSink );
        CleanupStack::Pop( fileSink );
        
        streamForRecording->SetSourceL( camera );
        iSession->AddStreamL( streamForRecording );
        CleanupStack::Pop( streamForRecording );
        }

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::CompleteSessionStructureL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::HandleSessionStateChanged( 
                                                CMceSession& aSession,
                                                TInt aStatusCode,
                                                const TDesC8& aReasonPhrase )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::HandleSessionStateChanged" )
    
    MUS_ENG_LOG_SESSION_STATE_AND_STATUS( aSession, aStatusCode, aReasonPhrase )
    
    if ( iSession && 
         iSession == &aSession && 
         aSession.State() == CMceSession::EEstablished &&
         iStoreEncoderConfigInfo )
        {
        iStoreEncoderConfigInfo = EFalse;
        TRAPD( error, StoreEncoderConfigInfoL() )
        if ( error != KErrNone && error != KErrNotFound )
            {
            // Only acceptable error is absence of repository entry,
            // otherwise we inform user about failed session.
            iSessionObserver.SessionFailed();
            }
        }
     
    CMusEngMceSession::HandleSessionStateChanged( aSession,
                                                  aStatusCode, 
                                                  aReasonPhrase );
    
    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::HandleSessionStateChanged" )
    }


// -----------------------------------------------------------------------------
// Sets video codec attributes
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::AdjustVideoCodecL( CMceVideoCodec& aVideoCodec )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngLiveSession::AdjustVideoCodecL()" )
    
    CMusEngMceOutSession::AdjustVideoCodecL( aVideoCodec );
    
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
        
        SetCodecConfigKeyL( aVideoCodec );
        SetEncodingDeviceL( aVideoCodec );                             
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
    
    if ( iVideoCodecList &&
         iVideoCodecList->Length() > 0 &&
         iVideoCodecList->FindF( KMceSDPNameH264 ) == KErrNotFound )
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
        iLiveSessionObserver.DiskFull();
        }
    else
        {
        // Cannot handle, forward to a base class
        CMusEngMceSession::StreamStateChanged( aStream, aSink );
        }
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngLiveSession::StreamStateChanged( sink )" )
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
CMusEngLiveSession::CMusEngLiveSession(
                        MMusEngSessionObserver& aSessionObserver,
                        MMusEngOutSessionObserver& aOutSessionObserver,
                        MMusEngLiveSessionObserver& aLiveSessionObserver,
                        const TRect& aRect,
                        const TDesC& aRecordedFile )
    : CMusEngMceOutSession( aRect,
                            aSessionObserver,
                            aOutSessionObserver ),
      iLiveSessionObserver( aLiveSessionObserver ),
      iDefaultZoomFactor( -1 ),
      iDefaultBrightness( 0 ),
      iRecordedFile( aRecordedFile ),
      iZoomInTime(0),
      iZoomOutTime(0),
      iSmallZoomStep( KZoomStepMinSize ),
      iBigZoomStep( KZoomStepMinSize )
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::ConstructL( TUint aSipProfileId )
    {
    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::ConstructL()" )

    CMusEngMceOutSession::ConstructL( aSipProfileId );

    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::ConstructL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::SetEncodingDeviceL( CMceVideoCodec& aVideoCodec )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngLiveSession::SetEncodingDeviceL()" )

    // We try to read encoding device UID. If we do not succeed, we use default
    
    const TUid KDefaultEncodingDevice( TUid::Uid( 0x20001C13 ) );
    TUid encodingDevice( KDefaultEncodingDevice );
    TRAPD( error, 
           encodingDevice = MultimediaSharingSettings::EncodingDeviceL() )
    __ASSERT_ALWAYS( error == KErrNone || error == KErrNotFound, 
                     User::Leave( error ) );
    aVideoCodec.SetPreferredEncodingDecodingDeviceL( encodingDevice );
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngLiveSession::SetEncodingDeviceL()" )
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::SetCodecConfigKeyL( CMceVideoCodec& aVideoCodec )
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngLiveSession::SetCodecConfigKeyL()" )
    
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
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngLiveSession::SetCodecConfigKeyL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
HBufC8* CMusEngLiveSession::ReadCodecConfigKeyL( 
                                    const CMceVideoCodec& aVideoCodec ) const
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngLiveSession::ReadCodecConfigKeyL()" )
    
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
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngLiveSession::ReadCodecConfigKeyL()" )
        
    return key;   
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMusEngLiveSession::StoreEncoderConfigInfoL() const
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngLiveSession::StoreEncoderConfigInfoL()" )
    
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
        
    MUS_LOG( "mus: [ENGINE] <- CMusEngLiveSession::StoreEncoderConfigInfoL()" )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
const TPtrC8 CMusEngLiveSession::ConfigKeyIdL(
                                    const CMceVideoCodec& aVideoCodec ) const
    {
    MUS_LOG( "mus: [ENGINE] -> CMusEngLiveSession::ConfigKeyIdL()" )
    
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
    
    MUS_LOG( "mus: [ENGINE] <- CMusEngLiveSession::ConfigKeyIdL()" )

    return configKeyTokenId;
    }


// -----------------------------------------------------------------------------
//  Calculate the zoom step based time escaped
// -----------------------------------------------------------------------------
TInt CMusEngLiveSession::ZoomStepSize( TInt64& aTime )
    {
//    MUS_LOG( "mus: [ENGINE]  -> CMusEngLiveSession::ZoomStepSize()" )    
    TTime now;
    now.HomeTime();
    TInt64 timeNow = now.Int64();   
    TInt stepSize = ( ( timeNow - aTime ) <= KZoomFasterTime ) ? iBigZoomStep : iSmallZoomStep;
    
    aTime = now.Int64();
    
    MUS_LOG1( "mus: [ENGINE]  stepSize = %d", stepSize );    
//    MUS_LOG( "mus: [ENGINE]  <- CMusEngLiveSession::ZoomStepSize()" )
    return stepSize;
    }


