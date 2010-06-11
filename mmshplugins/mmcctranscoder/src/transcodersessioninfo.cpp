/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies).
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


// INCLUDE FILES
#include <e32math.h>
#include <mmcctranscoder.h>
#include <mmcccodecinformation.h>

#include "transcodersessioninfo.h"
#include "mcctranscoderlogs.h"

// CONSTANTS

const TInt KMccTranscoderProgressMultiplier = 2;
const TInt KMccTranscoderProgressMiddle = 50;

const TInt KMccTranscoderVideoOnlyBitrate = 64000;

const TInt KMccTranscoderAvcTargetSegmentSize = 1300; // Try to fit to MTU


// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::CMccTranscoderSessionInfo
// default constructor
// -----------------------------------------------------------------------------
//
CMccTranscoderSessionInfo::CMccTranscoderSessionInfo()
    {
    }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMccTranscoderSessionInfo::ConstructL( )
    {

    }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::NewL
// Static constructor.
// -----------------------------------------------------------------------------
//
CMccTranscoderSessionInfo* CMccTranscoderSessionInfo::NewLC()
    {
    CMccTranscoderSessionInfo* self = new (ELeave) CMccTranscoderSessionInfo;
	CleanupStack::PushL( self );
	self->ConstructL();
	return self;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::~CMccTranscoderSessionInfo
// Destructor
// -----------------------------------------------------------------------------
//
CMccTranscoderSessionInfo::~CMccTranscoderSessionInfo()
    {
    __TRANSCODER( "CMccTranscoderSessionInfo::~CMccTranscoderSessionInfo" )
    
    delete iMovie;
    delete iVideoCodec;
    delete iAudioCodec;
    
    __TRANSCODER( "CMccTranscoderSessionInfo::~CMccTranscoderSessionInfo, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::SetQuality
// -----------------------------------------------------------------------------
//
void CMccTranscoderSessionInfo::SetQuality( TUint32 aQuality )
    {
    __TRANSCODER_INT1( "CMccTranscoderSessionInfo::SetQuality, quality=", aQuality )

    switch( aQuality )
        {
        case CVedMovie::EQualityAutomatic:
            iQuality = CVedMovie::EQualityAutomatic;
            break; 
        case CVedMovie::EQualityMMSInteroperability:
            iQuality = CVedMovie::EQualityMMSInteroperability;
            break;
        case CVedMovie::EQualityResolutionCIF:
            iQuality = CVedMovie::EQualityResolutionCIF;        
            break;
        case CVedMovie::EQualityResolutionQCIF:
            iQuality = CVedMovie::EQualityResolutionQCIF;        
            break;
        case CVedMovie::EQualityLast: 
            iQuality = CVedMovie::EQualityLast; 
            break;
        default:  
            iQuality = CVedMovie::EQualityLast; 
            break;
        } 
          
    __TRANSCODER( "CMccTranscoderSessionInfo::SetQuality, exits" )

    }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::SetDesFileL
// -----------------------------------------------------------------------------
//
void CMccTranscoderSessionInfo::SetDesFileL( const TDesC& aFileName )
    {
    __TRANSCODER( "CMccTranscoderSessionInfo::SetDesFileL" )

    __ASSERT_ALWAYS( aFileName.Length() > 0, User::Leave( KErrArgument ) );
  
    iDesFile.Copy( aFileName );
        
    __TRANSCODER( "CMccTranscoderSessionInfo::SetDesFileL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::SetVideoType
// -----------------------------------------------------------------------------
//
void CMccTranscoderSessionInfo::SetVideoType( 
    const TDesC8& aSdpName,
    const TInt aVideoBitRate )
    {
    __TRANSCODER( "CMccTranscoderSessionInfo::SetVideoType" )

    if ( aSdpName.CompareF( KAVCSdpName )  == 0 )
        {
        iVideoCodec->SetVideoType( EVedVideoTypeAVCBaselineProfile ); 
        // Other than zero value will cause transcoding failure for AVC
        iSyncIntervalInPicture = 0; 
        iSegmentSizeInBytes = KMccTranscoderAvcTargetSegmentSize;
        }
    else if ( aSdpName.CompareF( KH263SdpName ) == 0 ||
              aSdpName.CompareF( KH2631998SdpName ) == 0||
              aSdpName.CompareF( KH2632000SdpName  ) == 0 )
        {
        const TInt maxBitRateOne = 64000;
        const TInt maxBitRateTwo = 128000;
        if ( aVideoBitRate <= maxBitRateOne )
            {
            iVideoCodec->SetVideoType( EVedVideoTypeH263Profile0Level10 );
            }
        if ( aVideoBitRate >  maxBitRateOne && 
             aVideoBitRate < maxBitRateTwo )
            {
            iVideoCodec->SetVideoType( EVedVideoTypeH263Profile0Level45 ); 
            }
        iSyncIntervalInPicture = 1;
        }
    else 
        {
        iVideoCodec->SetVideoType( EVedVideoTypeUnrecognized) ;
        } 
        
    __TRANSCODER( "CMccTranscoderSessionInfo::SetVideoType, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::SetVideoType
// -----------------------------------------------------------------------------
//
void CMccTranscoderSessionInfo::SetAudioType( const TDesC8& aSdpName )
   {
   __TRANSCODER( "CMccTranscoderSessionInfo::SetAudioType" )

   if ( iAudioCodec )
       {
       if ( aSdpName.CompareF( KAACSdpName )  == 0 )
            {
            iAudioCodec->SetAudioType( EVedAudioTypeAAC_LC );  
            }
       else if ( aSdpName.CompareF( KAMRSdpName )  == 0  )
            {
            iAudioCodec->SetAudioType( EVedAudioTypeAMR );
            }
       else
            {
            iAudioCodec->SetAudioType( EVedAudioTypeUnrecognized );  
            } 
       }
   __TRANSCODER( "CMccTranscoderSessionInfo::SetAudioType, exit" )

   }
   
// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::SetVideoType
// -----------------------------------------------------------------------------
//
TUint32 CMccTranscoderSessionInfo::SessionId()
   {
   return iSessionId;    
   }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::SetVideoType
// -----------------------------------------------------------------------------
//
void CMccTranscoderSessionInfo::GenerateSessionId()
   {
   __TRANSCODER( "CMccTranscoderSessionInfo::GenerateSessionId" )

   iSessionId = 0;
    
   while( iSessionId == 0 )
        {
        iSessionId = static_cast<TUint32>( Math::Random() );
        }
   
   __TRANSCODER( "CMccTranscoderSessionInfo::GenerateSessionId, exit" )
   }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::CreateMovieL()
// -----------------------------------------------------------------------------
//
// ownership is transferred
void CMccTranscoderSessionInfo::CreateMovieL( )
   {
   __TRANSCODER( "CMccTranscoderSessionInfo::CreateMovieL" )

   if ( iMovie )
       {
       delete iMovie;
       iMovie = NULL;
       }
   iMovie = CVedMovie::NewL( 0 );
   
   __TRANSCODER( "CMccTranscoderSessionInfo::CreateMovieL, exit" )
   }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::Movie
// -----------------------------------------------------------------------------
//
CVedMovie& CMccTranscoderSessionInfo::Movie()
   {
   return *iMovie;   
   }


// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::Quality
// -----------------------------------------------------------------------------
//                    
CVedMovie::TVedMovieQuality CMccTranscoderSessionInfo::Quality()
   {
   return iQuality;   
   }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::DesFile
// -----------------------------------------------------------------------------
//
const TDesC& CMccTranscoderSessionInfo::DesFile()
   {
   return iDesFile;
   }


// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::VideoCodec
// -----------------------------------------------------------------------------
//
CMccTranscoderVideoCodec& CMccTranscoderSessionInfo::VideoCodec()
   {
   return *iVideoCodec;    
   }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::SetVideoCodec
// -----------------------------------------------------------------------------
//
void CMccTranscoderSessionInfo::SetVideoCodec( CMccTranscoderVideoCodec* aCodec )
   {
   __TRANSCODER( "CMccTranscoderSessionInfo::SetVideoCodec" )

   if ( iVideoCodec )
       {
       delete iVideoCodec;
       }
   iVideoCodec = aCodec;
   
   __TRANSCODER( "CMccTranscoderSessionInfo::SetVideoCodec, exit" )

   }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::AudioCodec
// -----------------------------------------------------------------------------
//
CMccTranscoderAudioCodec& CMccTranscoderSessionInfo::AudioCodec()
   {
   return *iAudioCodec;
   }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::SetAudioCodec
// -----------------------------------------------------------------------------
//
void CMccTranscoderSessionInfo::SetAudioCodec( CMccTranscoderAudioCodec* aCodec )
   {
   __TRANSCODER( "CMccTranscoderSessionInfo::SetAudioCodec " )

   if ( iAudioCodec )
       {
       delete iAudioCodec;
       }
       
   iAudioCodec = aCodec;
   
   __TRANSCODER( "CMccTranscoderSessionInfo::SetAudioCodec, exit" )

   }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::CheckVideoCodecL
// -----------------------------------------------------------------------------
//
void CMccTranscoderSessionInfo::CheckVideoCodecL( 
    const TMccTranscoderCodecInfo& aCodecInfo )
    {
    __TRANSCODER( "CMccTranscoderSessionInfo::CheckVideoCodecL " )

      // check if video code info is not empty, save it
    if ( aCodecInfo.iMimeType.Length() )
        {
        if ( iVideoCodec )
            {
            delete iVideoCodec;
            iVideoCodec = NULL;  
            }
        iVideoCodec = CMccTranscoderVideoCodec::NewL();
        
        iVideoCodec->SetBitrate( aCodecInfo.iBitrate );
        iVideoCodec->SetFrameRate( aCodecInfo.iFramerate );
        iVideoCodec->SetVideoResolution( 
            TSize(aCodecInfo.iVideoWidth, aCodecInfo.iVideoHeight) );
        
        SetVideoType( aCodecInfo.iMimeType, aCodecInfo.iBitrate );       
        }   
    
    __TRANSCODER( "CMccTranscoderSessionInfo::CheckVideoCodecL, exit " )

    }
        
// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::CheckAudioCodecL
// -----------------------------------------------------------------------------
//       
void CMccTranscoderSessionInfo::CheckAudioCodecL( 
    const TMccTranscoderCodecInfo& aCodecInfo )
    {
    __TRANSCODER( "CMccTranscoderSessionInfo::CheckAudioCodecL " )

      // check if audio code info is not empty, save it
    if (aCodecInfo.iMimeType.Length() )
        {
        if ( iAudioCodec )
            {
            delete iAudioCodec; 
            iAudioCodec = NULL; 
            }
        iAudioCodec = CMccTranscoderAudioCodec::NewL();
        iAudioCodec->SetBitrate( aCodecInfo.iBitrate );
        iAudioCodec->SetAudioChannelMode( EVedAudioChannelModeSingleChannel );
        iAudioCodec->SetSamplingRate( aCodecInfo.iSamplingFreq );   
        SetAudioType( aCodecInfo.iMimeType );
        }
        
    __TRANSCODER( "CMccTranscoderSessionInfo::CheckAudioCodecL, exit " )
    }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::CheckAudioCodecL
// -----------------------------------------------------------------------------
//  
void CMccTranscoderSessionInfo::GetPutputParameters( 
    TVedOutputParameters& params )
    {
    __TRANSCODER( "CMccTranscoderSessionInfo::GetPutputParameters " )

    if ( iVideoCodec )
        {
        __TRANSCODER( "Set video output values from codec" )
        params.iVideoType = iVideoCodec->VideoType();
        params.iVideoResolution = iVideoCodec->VideoResolution();
        params.iVideoBitrate = iVideoCodec->Bitrate();
        params.iVideoFrameRate = iVideoCodec->FrameRate();  
        }
    else if ( iMovie )// set video parameters from clip
        {
        __TRANSCODER( "Set video output values from mov" )
        params.iVideoType = iMovie->VideoType();
        params.iVideoResolution = iMovie->Resolution();
        params.iVideoBitrate = CalculateVideoBitrate( *iMovie );
        params.iVideoFrameRate = iMovie->VideoFrameRate();
        }
        
    //insert GOBs into clip
    params.iSyncIntervalInPicture = iSyncIntervalInPicture;  
    params.iSegmentSizeInBytes = iSegmentSizeInBytes;

    // audio parameters are stored in session, 
    // copy those parameters from session, 
    if ( iAudioCodec )
        {
        __TRANSCODER( "Set audio output values from codec" )
        params.iAudioType = iAudioCodec->AudioType();
        params.iAudioBitrate = iAudioCodec->Bitrate();
        params.iAudioChannelMode = EVedAudioChannelModeSingleChannel;
        params.iAudioSamplingRate = iAudioCodec->SamplingRate();
        }
    else if ( iMovie )// set audio parameters from clip
        {
        params.iAudioType = iMovie->AudioType();
        if ( params.iAudioType != EVedAudioTypeNoAudio )
            {
            __TRANSCODER( "Set audio output values from mov" )
            params.iAudioBitrate = iMovie->AudioBitrate();
            params.iAudioChannelMode = iMovie->AudioChannelMode();
            params.iAudioSamplingRate = iMovie->AudioSamplingRate();
            }
        else if ( params.iVideoType == EVedVideoTypeAVCBaselineProfile )
            {
            __TRANSCODER( "No audio for avc, set compatibility values" )
            
            // VideoEditorEngine strangely wants still these specific
            // audio values if video type is AVC and clip does not contain
            // audio.
            const TInt KMccTranscoderVedCompatibility = 16000;
            params.iAudioBitrate = KMccTranscoderVedCompatibility;
            params.iAudioSamplingRate = KMccTranscoderVedCompatibility;
            }
        else
            {
            __TRANSCODER( "No audio, do not set audio output values" )
            }
        }   
       
    __TRANSCODER( "CMccTranscoderSessionInfo::GetPutputParameters, exit " )

    }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::SetRemoved
// -----------------------------------------------------------------------------
//
void CMccTranscoderSessionInfo::SetRemoved( TBool aIsRemoved )
    {
    iIsRemoved = aIsRemoved;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::IsRemoved
// -----------------------------------------------------------------------------
//       
TBool CMccTranscoderSessionInfo::IsRemoved() const
    {
    return iIsRemoved;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::Progressed
// -----------------------------------------------------------------------------
//
TInt CMccTranscoderSessionInfo::Progressed( TInt aProgressPercentage )
    {
    // If only one media element is present, ved engine progress
    // percentage has to be multiplied by two (otherwise it goes only to 50%)
    if ( iAudioNotPresent && 
       ( aProgressPercentage <= KMccTranscoderProgressMiddle ) )
        {
        aProgressPercentage = 
            ( aProgressPercentage * KMccTranscoderProgressMultiplier );
        }
    return aProgressPercentage;
    }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::Progressed
// -----------------------------------------------------------------------------
//   
void CMccTranscoderSessionInfo::ClipAdded()
    {
     if ( iMovie )
        {
        __TRANSCODER_INT1( "CMccTranscoderSessionInfo::ClipAdded, audio type:", 
                           iMovie->AudioType() )
        
        iAudioNotPresent =
            ( iMovie->AudioType() == EVedAudioTypeNoAudio || 
              iMovie->AudioType() == EVedAudioTypeUnrecognized );
        }
    }

// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::QualityChangeNeeded
// If client has set output video codec, quality setting is ignored
// -----------------------------------------------------------------------------
//
TBool CMccTranscoderSessionInfo::QualityChangeNeeded()
    {
    return ( iQuality != CVedMovie::EQualityAutomatic && !iVideoCodec );
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderSessionInfo::CalculateVideoBitrate
// -----------------------------------------------------------------------------
// 
TInt CMccTranscoderSessionInfo::CalculateVideoBitrate( CVedMovie& aMovie )
    {
    __TRANSCODER( "CMccTranscoderSessionInfo::CalculateVideoBitrate" )
    
    TInt videoBitrate( KMccTranscoderVideoOnlyBitrate );
    if ( aMovie.AudioType() != EVedAudioTypeNoAudio && 
         aMovie.AudioBitrate() > 0 &&
         aMovie.AudioBitrate() < videoBitrate )
        {
        videoBitrate -= aMovie.AudioBitrate();
        }
    
    __TRANSCODER_INT1( "CMccTranscoderSessionInfo::CalculateVideoBitrate, exit bitrate:", 
                       videoBitrate )    
    return videoBitrate;
    }
    
// End of file

