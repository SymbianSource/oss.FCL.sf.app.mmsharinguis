/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
#include <mmcccodecinformation.h>

#include "transcoderimpl.h"
#include "mcctranscoderlogs.h"

const TInt KMccTranscodingCompletedPercentage = 100;

const TInt KMccTranscoderCleanupTimerMicrosecs = 10000; // 10ms

const TInt KMccTranscoderProgressNotifFreq = 10;

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CMccTranscoderImplImpl::CMccTranscoderImpl
//
// CMccTranscoderImpl default constructor, can NOT contain any code,
// that might leave
// Phase #1 of 2-phase constructor
// -----------------------------------------------------------------------------
//
CMccTranscoderImpl::CMccTranscoderImpl() :
    iDeltaTimerCallBack( AsyncTimerExpired, this )
    {
    iDeltaTimerEntry.Set( iDeltaTimerCallBack );   
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NewL
// -----------------------------------------------------------------------------
//
CMccTranscoderImpl* CMccTranscoderImpl::NewL()
    {
    CMccTranscoderImpl* self = new ( ELeave ) CMccTranscoderImpl;
	CleanupStack::PushL ( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;   
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::ConstructL
// -----------------------------------------------------------------------------
//
void CMccTranscoderImpl::ConstructL()
    {
    __TRANSCODER( "CMccTranscoderImpl::ConstructL" )
    
    iDeltaTimer = CDeltaTimer::NewL( CActive::EPriorityStandard );
    
    __TRANSCODER( "CMccTranscoderImpl::ConstructL, exit" )
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::~CMccTranscoderImpl
// -----------------------------------------------------------------------------
//
CMccTranscoderImpl::~CMccTranscoderImpl()
    {
    __TRANSCODER( "CMccTranscoderImpl::~CMccTranscoderImpl" )
    
    // Stop cleanup timer
    if ( iDeltaTimer )
        {
        iDeltaTimer->Remove( iDeltaTimerEntry );
        }
    delete iDeltaTimer;

    // Delete sessions
    TInt sessionCount = iSessionArray.Count();
    
    for ( TInt i = ( sessionCount - 1 ); i >= 0 ; i-- )
        {
        CMccTranscoderSessionInfo* session = iSessionArray[i];
        CVedMovie* movie = &(session->Movie());
        if ( movie && !session->IsRemoved() )
            {
            movie->UnregisterMovieObserver( this );
            }
        }
       
    iSessionArray.ResetAndDestroy(); 
    iSessionArray.Close(); 
        
    __TRANSCODER( "CMccTranscoderImpl::~CMccTranscoderImpl, exit" )
    }
 
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::RegisterEventObserver
// -----------------------------------------------------------------------------
//
void CMccTranscoderImpl::RegisterEventObserver( MMccTranscoderObserver& aObserver )
    {
    __TRANSCODER( "CMccTranscoderImpl::RegisterEventObserver" )

    iEventObserver = &aObserver;
    }
   
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::UnregisterEventObserver
// -----------------------------------------------------------------------------
//    
void CMccTranscoderImpl::UnregisterEventObserver()
    {
    __TRANSCODER( "CMccTranscoderImpl::UnregisterEventObserver" )

    iEventObserver = NULL;    
    }
                                         
      
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::TranscodeFile
// -----------------------------------------------------------------------------
//  
void CMccTranscoderImpl::TranscodeFileL( 
    TUint32& aSessionId, const TDesC8& aVal )
    {
    __TRANSCODER( "CMccTranscoderImpl::TranscodeFileL" )
    
    Cleanup();
    
    const TMccTranscodeFileMsg& transcodeMsg = 
        reinterpret_cast<const TMccTranscodeFileMsgBuffer&>( aVal )();
  
    CMccTranscoderSessionInfo* session = CMccTranscoderSessionInfo::NewLC();
   
    // Create session id
    session->GenerateSessionId();
    
    // Save quality info
    session->SetQuality( transcodeMsg.iQuality );
    
    // Save destination file info
    session->SetDesFileL( transcodeMsg.iDesFile );
    
    // CVedMovie object is created
    session->CreateMovieL();
    
    // Check if video code info is not empty, save it
    session->CheckVideoCodecL( transcodeMsg.iVideoCodec ); 
    
    // Check if audio code info is not empty, save it
    session->CheckAudioCodecL( transcodeMsg.iAudioCodec );
   
    __TRANSCODER( "CMccTranscoderImpl::RegisterMovieObserverL..." )
    session->Movie().RegisterMovieObserverL( this );
   	User::LeaveIfError( iSessionArray.Append( session ) );
   	CleanupStack::Pop( session );
    
    __TRANSCODER( "CMccTranscoderImpl::InsertVideoClipL..." )

    session->Movie().InsertVideoClipL( transcodeMsg.iSourceFile, 0 );
    // Callback function ::NotifyVideoClipAdded will be called later
    
    aSessionId = session->SessionId();
    
    __TRANSCODER( "CMccTranscoderImpl::TranscodeFileL, exit" )
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::CancelTranscodeFile
// -----------------------------------------------------------------------------
// 
void CMccTranscoderImpl::CancelTranscodeFileL( TUint32 aSessionId )
    {
    __TRANSCODER( "CMccTranscoderImpl::CancelTranscodeFileL" )

    Cleanup();
    
    TInt session = FindSession( aSessionId );
    
    __ASSERT_ALWAYS( session >= 0 , User::Leave( KErrNotFound) );
    __ASSERT_ALWAYS( &( iSessionArray[session]->Movie() ), 
        User::Leave( KErrNotFound) );
        
    TUint32 sessionId = iSessionArray[session]->SessionId();
    
    iSessionArray[session]->Movie().UnregisterMovieObserver( this );
    
    iSessionArray[session]->Movie().CancelProcessing();
    
    SendTranscodeEventToClient( KMccTranscodeCancelled, KErrNone, sessionId );
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::FindSessionL
// -----------------------------------------------------------------------------
// 
TInt CMccTranscoderImpl::FindSession( CVedMovie& aMovie )
    {
    TInt sessionCount = iSessionArray.Count();
    TInt err = KErrNotFound;
    TInt sessionIndex = 0;

    for ( TInt i = 0; (i < sessionCount && err != KErrNone ); i++ )
        {
        if ( &iSessionArray[i]->Movie() == &aMovie )
            {
            sessionIndex = i;
            err = KErrNone;
            }
        }
    if ( err )
        {
        return err;   
        }
    else
        {
        return sessionIndex;
        }
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::FindSession
// -----------------------------------------------------------------------------
// 
TInt CMccTranscoderImpl::FindSession( TUint32 aSessionID )
    {
    TInt sessionCount = iSessionArray.Count();
    TInt err = KErrNotFound;
    TInt sessionIndex = 0;

    for ( TInt i = 0; (i < sessionCount && err != KErrNone ); i++ )
        {
        if ( iSessionArray[i]->SessionId() == aSessionID )
            {
            sessionIndex = i;
            err = KErrNone;
            }
        }
    if ( err )
        {
        return err;   
        }
    else
        {
        return sessionIndex; 
        }
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::RemoveSession
// -----------------------------------------------------------------------------
// 
void CMccTranscoderImpl::RemoveSession( const TInt aIndex )
    {
    TInt sessionCount = iSessionArray.Count();

    if ( aIndex < sessionCount )
        {
        iSessionArray[aIndex]->Movie().UnregisterMovieObserver( this );
        
        // Can be deleted on next cleanup
        iSessionArray[aIndex]->SetRemoved( ETrue );
        
        // Initiate async cleanup
        InitiateCleanup();
        } 
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::SendTranscodeEventToClient
// -----------------------------------------------------------------------------
//   
void CMccTranscoderImpl::SendTranscodeEventToClient( 
    TMccEventType aEventType, TInt aError , TUint32 aSessionId, TUint32 aData )
    {
    __TRANSCODER( "CMccTranscoderImpl::SendTranscodeEventToClient" )
    
    if ( !iEventObserver )
        {
        __TRANSCODER( "CMccTranscoderImpl::SendTranscodeEventToClient, no observer" )
        return;
        }
        
    TMccEvent event;
    event.iErrorCode = aError;
    event.iEventCategory = KMccEventCategoryTranscode;
    event.iEventType = aEventType;
    event.iSessionId = aSessionId;
    event.iEventNumData = aData;
    
    iEventObserver->MccTranscoderEventReceived( event );

    __TRANSCODER( "CMccTranscoderImpl::SendTranscodeEventToClient, exit" )
    }
 
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyVideoClipAdded
// -----------------------------------------------------------------------------
//    
void CMccTranscoderImpl::NotifyVideoClipAdded(
    CVedMovie& aMovie, TInt /*aIndex*/)
    {
    __TRANSCODER( "CMccTranscoderImpl::NotifyVideoClipAdded" )

    TInt session = FindSession( aMovie );
    
    // Session found
    if ( session >= 0 )
        {
        TUint32 sessionId = iSessionArray[session]->SessionId();
        
        iSessionArray[session]->ClipAdded();
        
        if ( iSessionArray[session]->QualityChangeNeeded() )
            {
            iSessionArray[session]->Movie().SetQuality( 
                iSessionArray[session]->Quality() );
            // Callback function ::NotifyMovieQualityChanged will be called
            }
        else
            {
            // No need to change quality, continue with transcoding initialization
            SetOutputParameters( aMovie );
            }
        }
    else
        {
        __TRANSCODER( "CMccTranscoderImpl::NotifyVideoClipAdded, \
session not found" )
        }  
    }
  
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyMovieQualityChanged
// -----------------------------------------------------------------------------
//    
void CMccTranscoderImpl::NotifyMovieQualityChanged(CVedMovie& aMovie)
    {
    __TRANSCODER( "CMccTranscoderImpl::NotifyMovieQualityChanged" )
    
    SetOutputParameters( aMovie );
    
    __TRANSCODER( "CMccTranscoderImpl::NotifyMovieQualityChanged, exit" )
    }


// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyMovieProcessingStartedL
// -----------------------------------------------------------------------------
//  
void CMccTranscoderImpl::NotifyMovieProcessingStartedL(CVedMovie& aMovie)
    {
    __TRANSCODER( "CMccTranscoderImpl::NotifyMovieProcessingStartedL" )
 
    TInt session = FindSession( aMovie );

    if ( session >= 0 )
        {
        TUint32 sessionId = iSessionArray[session]->SessionId();

        // Send event to client
        SendTranscodeEventToClient( KMccTranscodeInProgress, KErrNone, sessionId );
        }
    else
        {
        __TRANSCODER( "CMccTranscoderImpl::NotifyMovieProcessingStartedL, \
session not found" )
        }
    }


// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyMovieOutputParametersChanged
// -----------------------------------------------------------------------------
//     
void CMccTranscoderImpl::NotifyMovieOutputParametersChanged(CVedMovie& aMovie)
    {
    __TRANSCODER( "CMccTranscoderImpl::NotifyMovieOutputParametersChanged" )

    TInt session = FindSession( aMovie );
   
    if ( session >= 0 )
        {
        TRAPD( err, iSessionArray[session]->Movie().ProcessL( 
            iSessionArray[session]->DesFile(), *this ) );
        // Next will call 
        // MVedMovieProcessingObserver::NotifyMovieProcessingStartedL    
        if ( err )
            {
            __TRANSCODER_INT1( "CMccTranscoderImpl::NotifyMovieOutput\
    ParametersChanged, call ProcessL, Error=", err )
    
            TUint32 sessionId = iSessionArray[session]->SessionId();

            // Error happened when process transcoding, remove the clip
            RemoveSession( session );
            
            SendTranscodeEventToClient( KMccTranscodeCompleted, err, sessionId );
            }
        }
    else
        {
        __TRANSCODER( "CMccTranscoderImpl::NotifyMovieOutputParametersChanged, \
session not found" )
        }
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyMovieProcessingCompleted
// -----------------------------------------------------------------------------
//   
void CMccTranscoderImpl::NotifyMovieProcessingCompleted(
    CVedMovie& aMovie, TInt aError)
    {
    __TRANSCODER( "CMccTranscoderImpl::NotifyMovieProcessingCompleted" )
    
    __TRANSCODER_INT1( "CMccTranscoderImpl::NotifyMovieProcessingCompleted, \
aError= ", aError)

    TInt session = FindSession( aMovie );
    
    if ( session >= 0 )
        {
        TUint32 sessionId = iSessionArray[session]->SessionId();

        // Clip not needed anymore
        RemoveSession( session );
         
        // Send event 
        SendTranscodeEventToClient( KMccTranscodeCompleted, 
                                    aError, 
                                    sessionId, 
                                    KMccTranscodingCompletedPercentage );
        }
    else
        {
        __TRANSCODER( "CMccTranscoderImpl::NotifyMovieProcessingCompleted, \
session not found" )
        }
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyVideoClipAddingFailed
// -----------------------------------------------------------------------------
//     
void CMccTranscoderImpl::NotifyVideoClipAddingFailed(
    CVedMovie& aMovie, TInt aError)
    {
    __TRANSCODER( "CMccTranscoderImpl::NotifyVideoClipAddingFailed" )
    
    TInt session = FindSession( aMovie );
    
    if ( session >= 0 )
        {
        TUint32 sessionId = iSessionArray[session]->SessionId();
               
        RemoveSession( session );
        
        // Send event to client   
        SendTranscodeEventToClient( KMccTranscodeCompleted, aError, sessionId );
        }
    else
        {
        __TRANSCODER_INT1( "CMccTranscoderImpl::NotifyVideoClipAddingFailed, \
aError is ignored:", aError )
        }
    }
    
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyVideoClipRemoved
// -----------------------------------------------------------------------------
//     
void CMccTranscoderImpl::NotifyVideoClipRemoved(
    CVedMovie& aMovie, TInt /*aIndex*/ )
    {
    __TRANSCODER( "CMccTranscoderImpl::NotifyVideoClipRemoved" )
    
    TInt session = FindSession( aMovie );
    
    if ( session >= 0 )
        {
        __TRANSCODER( "CMccTranscoderImpl::NotifyVideoClipRemoved, \
removing session..." )
        RemoveSession( session );            
        }
    }
   
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::NotifyMovieProcessingProgressed
// -----------------------------------------------------------------------------
//        
void CMccTranscoderImpl::NotifyMovieProcessingProgressed( 
    CVedMovie& aMovie, TInt aPercentage )
    {
    __TRANSCODER( "CMccTranscoderImpl::NotifyMovieProcessingProgressed" )

    TInt session = FindSession( aMovie );
    
    if ( session >= 0 )
        {
        __TRANSCODER_INT1( "CMccTranscoderImpl::NotifyMovieProcessingProgressed,\
aPercenctage=", aPercentage )

        TUint32 sessionId = iSessionArray[session]->SessionId();
        
        TInt modifiedPercentage = iSessionArray[session]->Progressed( aPercentage );
        
        if ( !(modifiedPercentage % KMccTranscoderProgressNotifFreq) )
            {
            SendTranscodeEventToClient( KMccTranscodeInProgress, 
                KErrNone, sessionId, modifiedPercentage );
            }
        }
    else
        {
        __TRANSCODER( "CMccTranscoderImpl::NotifyMovieProcessingProgressed, \
session not found" )
        }
   
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::Cleanup
// -----------------------------------------------------------------------------
//
void CMccTranscoderImpl::Cleanup()
    {
    __TRANSCODER("CMccTranscoderImpl::Cleanup");
    
    TInt sessionCount = iSessionArray.Count();
    
    for ( TInt i = ( sessionCount - 1 ); i >= 0 ; i-- )
        {
        __TRANSCODER("CMccTranscoderImpl::Cleanup, iterating");
        
        CMccTranscoderSessionInfo* session = iSessionArray[i];
        if ( session->IsRemoved() )
            {
            iSessionArray.Remove( i );
            delete session;
            }
        }
        
    __TRANSCODER("CMccTranscoderImpl::Cleanup, exit");
    }

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::InitiateCleanup
// -----------------------------------------------------------------------------
//
void CMccTranscoderImpl::InitiateCleanup()
	{
	__TRANSCODER("CMccTranscoderImpl::InitiateCleanup");
	
	iDeltaTimer->Remove( iDeltaTimerEntry );
	TTimeIntervalMicroSeconds32 interval( KMccTranscoderCleanupTimerMicrosecs );
	iDeltaTimer->Queue( interval, iDeltaTimerEntry );
	
	__TRANSCODER("CMccQosController::InitiateCleanup, exit");	
	}	
 
// -----------------------------------------------------------------------------
// CMccTranscoderImpl::AsyncTimerExpired
// -----------------------------------------------------------------------------
//
TInt CMccTranscoderImpl::AsyncTimerExpired( TAny* aPtr )
	{
	__TRANSCODER("CMccTranscoderImpl::AsyncTimerExpired");

    if ( aPtr )
        {
	    CMccTranscoderImpl* self = reinterpret_cast<CMccTranscoderImpl*>( aPtr );
    	self->Cleanup();
        }
	
	__TRANSCODER("CMccTranscoderImpl::AsyncTimerExpired, exit");
  	return ETrue;
	} 

// -----------------------------------------------------------------------------
// CMccTranscoderImpl::SetOutputParameters
// -----------------------------------------------------------------------------
//
void CMccTranscoderImpl::SetOutputParameters( CVedMovie& aMovie )
    {
    __TRANSCODER( "CMccTranscoderImpl::SetOutputParameters" )

    TInt session = FindSession( aMovie );
       
    if ( session >= 0 )
        {
        // Change coding options
        TVedOutputParameters params;
         
        iSessionArray[session]->GetPutputParameters( params );
        
        __TRANSCODER_INT1( "CMccTranscoderImpl:: video type", params.iVideoType )
        __TRANSCODER_INT1( "CMccTranscoderImpl:: video width", params.iVideoResolution.iWidth )
        __TRANSCODER_INT1( "CMccTranscoderImpl:: video height", params.iVideoResolution.iHeight )
        __TRANSCODER_INT1( "CMccTranscoderImpl:: video bitrate", params.iVideoBitrate )
        __TRANSCODER_INT1( "CMccTranscoderImpl:: sync interval", params.iSyncIntervalInPicture )
        __TRANSCODER_INT1( "CMccTranscoderImpl:: segment size", params.iSegmentSizeInBytes )
        __TRANSCODER_INT1( "CMccTranscoderImpl:: audio type", params.iAudioType )
        __TRANSCODER_INT1( "CMccTranscoderImpl:: audio bitrate", params.iAudioBitrate )
        __TRANSCODER_INT1( "CMccTranscoderImpl:: audio channel mode", params.iAudioChannelMode )
        __TRANSCODER_INT1( "CMccTranscoderImpl:: audio sampling rate", params.iAudioSamplingRate )

        TRAPD( err, iSessionArray[session]->Movie().SetOutputParametersL( params ) );
        // Next call back will be ::NotifyMovieOutputParametersChanged
        
        if ( err )
            {
            __TRANSCODER_INT1( "CMccTranscoderImpl::SetOutputParameters,\
call SetOutputParametersL Error=", err )

            TUint32 sessionId = iSessionArray[session]->SessionId();
            
            // Error happend when set output parameter, remove the clip
            RemoveSession( session );

            SendTranscodeEventToClient( KMccTranscodeCompleted, err, sessionId );
            }
        else
            {
            // Clean the video codec, it is not used any more
            iSessionArray[session]->SetVideoCodec( NULL ); 
            // Clean the audio codec, it is not used any more 
            iSessionArray[session]->SetAudioCodec( NULL ); 
            }
        }
    else
        {
        __TRANSCODER( "CMccTranscoderImpl::SetOutputParameters, \
session not found" )
        }
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================

//  End of File
