/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "vedmovie.h"

_LIT( KMccTranscodedFile, "C:\\data\\videos\\timeraudio.3gp" );
_LIT( KMccTranscodedAvcFile, "C:\\data\\videos\\H264_QCIF_AMR_output.3gp" );


// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
CVedMovie* CVedMovie::NewL(RFs* aFs)
    {
    CVedMovie* movie = CVedMovie::NewLC( aFs );
    CleanupStack::Pop( movie );
    return movie;
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
CVedMovie* CVedMovie::NewLC(RFs* /*aFs*/)
    {
    CVedMovie* movie = new ( ELeave ) CVedMovie;
    CleanupStack::PushL( movie );
    movie->ConstructL();
    return movie;
    }    

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
CVedMovie::~CVedMovie()
    {
    CancelProcessing();
   
    delete iDeltaTimer;
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
CVedMovie::CVedMovie() :
    iDeltaTimerCallBack(AsyncTimerExpired, this)
    {
    iDeltaTimerEntry.Set(iDeltaTimerCallBack);
    iAudioType = EVedAudioTypeAMR;
    }
    
// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
void CVedMovie::ConstructL()
    {
    iDeltaTimer = CDeltaTimer::NewL(CActive::EPriorityStandard);
    }
    
// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//    
void CVedMovie::SetQuality(TVedMovieQuality aQuality)
    {
    iQuality = aQuality;
    IssueCallback( ENotifyMovieQualityChanged );
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
TVedVideoType CVedMovie::VideoType() const
    {
    return EVedVideoTypeH263Profile0Level45;
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
TSize CVedMovie::Resolution() const
    {
    return TSize( 176, 144 );
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
TVedAudioType CVedMovie::AudioType() const
    {
    return iAudioType;
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
TInt CVedMovie::AudioSamplingRate() const
    {
    return 8000;
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
TVedAudioChannelMode CVedMovie::AudioChannelMode() const
    {
    return EVedAudioChannelModeSingleChannel;
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
void CVedMovie::InsertVideoClipL(const TDesC& /*aFileName*/, TInt /*aIndex*/)
    {
	IssueCallback( ENotifyVideoClipAdded );
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
void CVedMovie::ProcessL(const TDesC& aFileName,
                           MVedMovieProcessingObserver& aObserver)
    {
    TFileName sourcePath;
    
    if ( iOutputParams.iVideoType == EVedVideoTypeAVCBaselineProfile )
        {
        // AVC output
        sourcePath.Copy( KMccTranscodedAvcFile );
        }
    else
        {
        // H263 output
        sourcePath.Copy( KMccTranscodedFile );
        }
   	
    RFs fs;
	User::LeaveIfError( fs.Connect() );
	CleanupClosePushL( fs );
	
	CFileMan* fileMan = CFileMan::NewL(fs);
	CleanupStack::PushL(fileMan); 
	
	// Make sure path exists, ignore errors since path might exist already
	fs.MkDirAll( aFileName );	
	
	// Make the destination file writeable, ignore errors since most probably
	// file doesn't exist yet
    fileMan->Attribs( aFileName, 0, KEntryAttReadOnly, TTime( 0 ), 0 );
    
	User::LeaveIfError( fileMan->Copy( sourcePath, aFileName ) );
    
	CleanupStack::PopAndDestroy( 2 ); // fileMan, fs
	
    iProcessObserver = &aObserver;
    iProgress = 0;
    
    IssueCallback( ENotifyMovieProcessingStartedL );
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
void CVedMovie::CancelProcessing()
    {
     if ( iDeltaTimer )
        {
        iDeltaTimer->Remove( iDeltaTimerEntry );
        }
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
void CVedMovie::RegisterMovieObserverL(MVedMovieObserver* aObserver)
    {
    iObserver = aObserver;
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
void CVedMovie::UnregisterMovieObserver(MVedMovieObserver* /*aObserver*/)
    {
    iObserver = NULL;
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
TInt CVedMovie::AudioBitrate() const
    {
    return 12200;
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
TReal CVedMovie::VideoFrameRate() const
    {
    return 15.0;
    }

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
void CVedMovie::SetOutputParametersL(TVedOutputParameters& aOutputParams)
    {
    iOutputParams = aOutputParams;
    IssueCallback( ENotifyMovieOutputParametersChanged );
    }
    
// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
void CVedMovie::IssueCallback( TVedMovieCurrentCallbackType aCallback )
	{
	iDeltaTimer->Remove(iDeltaTimerEntry);
	iCurrentCallback = aCallback;
	TTimeIntervalMicroSeconds32 interval(200000);
	iDeltaTimer->Queue(interval, iDeltaTimerEntry);	
	}	
 
// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//
TInt CVedMovie::AsyncTimerExpired(TAny* aPtr)
	{
	CVedMovie* self = reinterpret_cast<CVedMovie*>(aPtr);
	
	TRAP_IGNORE( self->DoCurrentCallbackL() )

  	return ETrue;
	} 

// -----------------------------------------------------------------------------
// CVedMovie
// -----------------------------------------------------------------------------
//	
void CVedMovie::DoCurrentCallbackL()
    {
    __ASSERT_ALWAYS( iObserver, User::Leave( KErrNotReady ) );
    
    switch ( iCurrentCallback )
        {
        case ENotifyVideoClipAdded:
            {
            iObserver->NotifyVideoClipAdded( *this, 0 );
            break;
            }
        case ENotifyMovieQualityChanged:
            {
            iObserver->NotifyMovieQualityChanged( *this );
            break;
            }
        case ENotifyMovieOutputParametersChanged:
            {
            iObserver->NotifyMovieOutputParametersChanged( *this );
            break;
            }
        case ENotifyMovieProcessingStartedL:
            {
            __ASSERT_ALWAYS( iProcessObserver, User::Leave( KErrNotReady ) );
            
            iProcessObserver->NotifyMovieProcessingStartedL( *this );
            IssueCallback( ENotifyMovieProcessingProgressed );
            break;
            }
        case ENotifyMovieProcessingProgressed:
            {
            __ASSERT_ALWAYS( iProcessObserver, User::Leave( KErrNotReady ) );
            
            iProgress += 20;
            if ( iProgress < 100 )
                {
                iProcessObserver->NotifyMovieProcessingProgressed( *this, iProgress );
                IssueCallback( ENotifyMovieProcessingProgressed );
                }
            else
                {
                IssueCallback( ENotifyMovieProcessingCompleted );
                }
            break;
            }
        case ENotifyMovieProcessingCompleted:
            {
            __ASSERT_ALWAYS( iProcessObserver, User::Leave( KErrNotReady ) );
            
            iProgress = 100;
            iProcessObserver->NotifyMovieProcessingCompleted( *this, KErrNone );
            break;
            }
        default:
            {
            break;
            }
        }
    }
    
