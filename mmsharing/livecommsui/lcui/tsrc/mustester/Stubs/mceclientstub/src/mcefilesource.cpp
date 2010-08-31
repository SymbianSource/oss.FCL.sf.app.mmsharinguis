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



#include "mcefilesource.h"
#include "mcevideostream.h"
#include "mceaudiostream.h"
#include "mcemanager.h"
#include "mcesession.h"
#include "utf.h"


// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSource* CMceFileSource::NewL(
            CMceManager& aManager,
            const TFileName& aFileName )
    {
    CMceFileSource* self = CMceFileSource::NewLC( aManager, aFileName );
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSource* CMceFileSource::NewLC(
            CMceManager& aManager,
            const TFileName& aFileName )
    {
    CMceFileSource* self = new (ELeave) CMceFileSource();
    CleanupStack::PushL( self );
    self->ConstructL( &aManager, aFileName );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C CMceFileSource::~CMceFileSource()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceFileSource::EnableL()
    {
    CMceMediaSource::DoEnableL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceFileSource::DisableL()
    { 
    CMceMediaSource::DoDisableL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
EXPORT_C void CMceFileSource::UpdateL ( const TFileName& aFileName )
    {
    iFileName = aFileName;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
EXPORT_C TInt CMceFileSource::MediaElementCountL( TMceMediaType aType ) const
    {
    if ( aType == KMceAudio )
        {
        return iAudioElementCount;
        }
    else if ( aType == KMceVideo )
        {
        return iVideoElementCount;
        }
    return 0;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//    
EXPORT_C void CMceFileSource::SetCurrentMediaElementL( 
            TMceMediaType aType, 
            TInt aIndex )
    {
    __ASSERT_ALWAYS( aIndex >= 0 &&
                     aIndex < MediaElementCountL( aType ), 
                     User::Leave( KErrArgument ) );
                     
    if ( aType == KMceAudio )
        {
        iCurrentAudioElement = aIndex;
        }
    else if ( aType == KMceVideo )
        {
        iCurrentVideoElement = aIndex;
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//      
EXPORT_C TInt CMceFileSource::CurrentMediaElement( TMceMediaType aType )
    {
    if ( aType == KMceAudio )
        {
        return iCurrentAudioElement;
        }
    else if ( aType == KMceVideo )
        {
        return iCurrentVideoElement;
        }
    
    return 0;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//      
EXPORT_C void CMceFileSource::SetPositionL( 
            const TTimeIntervalMicroSeconds& aPosition )
    {
    __ASSERT_ALWAYS( aPosition <= DurationL() &&
                     aPosition >= TTimeIntervalMicroSeconds( 0 ), 
                     User::Leave( KErrArgument ) );
    
    iPosition = aPosition;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//          
EXPORT_C TTimeIntervalMicroSeconds CMceFileSource::PositionL() const
    {        
    return iPosition;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//          
EXPORT_C TTimeIntervalMicroSeconds CMceFileSource::DurationL() const
    {
    return iDuration;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
EXPORT_C void CMceFileSource::SetFastForwardL( TBool aUseFFWD )
    {
    __ASSERT_ALWAYS( this->MediaElementCountL( KMceVideo ) > 0, 
                     User::Leave( KErrNotSupported ) );


    iFastForward = aUseFFWD;
        
    
    if ( iFastForward )
        {
        iFastRewind = EFalse;
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
EXPORT_C void CMceFileSource::SetFastRewindL( TBool aUseFRWD )
    {
    __ASSERT_ALWAYS( this->MediaElementCountL( KMceVideo ) > 0, 
                     User::Leave( KErrNotSupported ) );

    iFastRewind = aUseFRWD;
        
    if ( iFastRewind )
        {
        iFastForward = EFalse;
        }
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
EXPORT_C void CMceFileSource::TranscodeL( const TFileName& aFileName )
    {
    __ASSERT_ALWAYS( aFileName != KNullDesC, User::Leave( KErrArgument ) );

    TBool transcodingNeeded = EFalse;
    for ( TInt i = 0; i < iStreams.Count(); ++i )
        {
        if ( iStreams[i]->State() == CMceMediaStream::ETranscodingRequired )
            {
            transcodingNeeded = ETrue;
            if ( iStreams[i]->Type() == KMceAudio )
                {
                CMceAudioStream* audio = 
                                    static_cast<CMceAudioStream*>(iStreams[i]);
                __ASSERT_ALWAYS( audio->Codecs().Count() > 0, 
                                 User::Leave( KErrNotReady ) );
                }
            else
                {
                CMceVideoStream* video = 
                                    static_cast<CMceVideoStream*>(iStreams[i]);
                __ASSERT_ALWAYS( video->Codecs().Count() > 0, 
                                 User::Leave( KErrNotReady ) );
                }
               
            iStreams[i]->iState = CMceMediaStream::ETranscoding;
            }
        }
    
    __ASSERT_ALWAYS( transcodingNeeded, User::Leave( KErrNotReady ) );
        
    }

   
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
EXPORT_C void CMceFileSource::CancelTranscodeL()
    {
    TBool transcodingOngoing = EFalse;
    
    for ( TInt i = 0; i < iStreams.Count(); ++i )
        {
        if ( iStreams[i]->State() == CMceMediaStream::ETranscoding )
            {
            transcodingOngoing = ETrue;
            iStreams[i]->iState = CMceMediaStream::ETranscodingRequired;
            }
        }
    
    __ASSERT_ALWAYS( transcodingOngoing, User::Leave( KErrNotReady ) );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//  
EXPORT_C TInt CMceFileSource::TranscodingProgressL() const
    {
    TBool transcodingOngoing = EFalse;
    
    if ( iFailWithCode != KErrNone )
        {
        TInt failCode = iFailWithCode;
        iFailWithCode = KErrNone;
        User::Leave( failCode );
        }
    
    for ( TInt i = 0; i < iStreams.Count(); ++i )
        {
        if ( iStreams[i]->State() == CMceMediaStream::ETranscoding )
            {
            transcodingOngoing = ETrue;
            }
        }
    
    __ASSERT_ALWAYS( transcodingOngoing, User::Leave( KErrNotReady ) );

    return iTranscodingPercentage; 
    }
        

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void CMceFileSource::InitializeL( CMceManager* aManager )
    {
    iDuration = TTimeIntervalMicroSeconds( 100000000 );
    
    iAudioElementCount = 1;
    iVideoElementCount = 1;
        
    }
    
// -----------------------------------------------------------------------------
// Factory method for inner usage
// -----------------------------------------------------------------------------
//
CMceFileSource* CMceFileSource::NewL()
    {
    CMceFileSource* self = CMceFileSource::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Factory method for inner usage
// -----------------------------------------------------------------------------
//
CMceFileSource* CMceFileSource::NewLC()
    {
    CMceFileSource* self = new (ELeave) CMceFileSource();
    CleanupStack::PushL( self );
    self->ConstructL( NULL, KNullDesC() );
    return self;
    }    
    
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
CMceFileSource::CMceFileSource()
    {
    iType = KMceFileSource;    
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//  
void CMceFileSource::ConstructL( CMceManager* aManager, 
                                 const TFileName& aFileName )
    {
    
    iFileName = aFileName;
    
    if ( aManager )
        {        
        InitializeL( aManager );
        }
   
    }



