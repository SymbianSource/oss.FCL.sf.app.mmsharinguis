/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit test
*
*/



//  INTERNAL INCLUDES
#include "ut_mcctranscoderimpl.h"
#include "transcoderimpl.h"


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>

static TInt error;

_LIT( KTestMccTranscodeFile, "C:\\data\\videos\\big.3gp" );
_LIT( KTestMccTranscodedFile, "C:\\data\\videos\\normal.3gp" );
_LIT8( KTestMccTranscodedVideoMimeType, "H264" );
_LIT8( KTestMccTranscodedAudioMimeType, "AAC" );

#define TC_MEMORY_LEAVE_CHECKING_ON

#ifdef TC_MEMORY_LEAVE_CHECKING_ON
#define TC_CHECK_MEMORY_LEAVE( conditions ) \
if ( !(conditions) )\
    {\
    User::Leave( KErrNoMemory );\
    }

#else
#define TC_CHECK_MEMORY_LEAVE( conditions ) \
    EUNIT_ASSERT( conditions )
#endif

#ifdef TC_MEMORY_LEAVE_CHECKING_ON
#define TC_CHECK_MEMORY_LEAVE_NEG( conditions ) \
if ( conditions )\
    {\
    User::Leave( KErrNoMemory );\
    }

#else
#define TC_CHECK_MEMORY_LEAVE_NEG( conditions )
#endif


void CDeltaTimer::RunL()
    {
    }

TInt CFileMan::Copy(const TDesC& /*anOld*/,const TDesC& /*aNew*/,TUint /*aSwitches*/)
    {
    return error;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMccTranscoderImpl* UT_CMccTranscoderImpl::NewL()
    {
    UT_CMccTranscoderImpl* self = UT_CMccTranscoderImpl::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMccTranscoderImpl* UT_CMccTranscoderImpl::NewLC()
    {
    UT_CMccTranscoderImpl* self = new( ELeave ) UT_CMccTranscoderImpl();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMccTranscoderImpl::~UT_CMccTranscoderImpl()
    {
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMccTranscoderImpl::UT_CMccTranscoderImpl()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::SetupL()
    {
    RFs fs;
    CleanupClosePushL( fs );
    iMovie = CVedMovie::NewL( &fs );
    CleanupStack::PopAndDestroy();//fs
    
    iMccTranscoder = CMccTranscoderImpl::NewL();

    iData.iSourceFile = KTestMccTranscodeFile();
    iData.iDesFile = KTestMccTranscodedFile();
    iData.iQuality = CVedMovie::EQualityAutomatic;
    iData.iVideoCodec.iMimeType = KTestMccTranscodedVideoMimeType();
    iData.iAudioCodec.iMimeType = KTestMccTranscodedAudioMimeType();
    iMccTranscoder->RegisterEventObserver( *this );
    
    Reset();
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::Setup2L()
    {
    RFs fs;
    CleanupClosePushL( fs );
    iMovie = CVedMovie::NewL( &fs );
    CleanupStack::PopAndDestroy();//fs
    
    iMccTranscoder = CMccTranscoderImpl::NewL();

    iData.iSourceFile = KTestMccTranscodeFile();
    iData.iDesFile = KTestMccTranscodedFile();
    iData.iQuality = CVedMovie::EQualityMMSInteroperability;
    iData.iVideoCodec.iMimeType = KNullDesC8;
    iData.iAudioCodec.iMimeType = KNullDesC8; 
    iMccTranscoder->RegisterEventObserver( *this );
    
    Reset();
    
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::Teardown()
    {
    delete iMccTranscoder;
    delete iMovie;

    }


void UT_CMccTranscoderImpl::MccTranscoderEventReceived( TMccEvent& aEvent )
    {
    iMccEvent = aEvent;
    }


void UT_CMccTranscoderImpl::Reset( TInt aError )
    {
    iMccEvent = TMccEvent();
    error = aError;
    
    }

// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_RegisterEventObserverL()
    {
    EUNIT_ASSERT( iMccTranscoder->iEventObserver );
    iMccTranscoder->RegisterEventObserver( *this );
    EUNIT_ASSERT( iMccTranscoder->iEventObserver == this);
    iMccTranscoder->iEventObserver = NULL;
    iMccTranscoder->RegisterEventObserver( *this );
    EUNIT_ASSERT( iMccTranscoder->iEventObserver == this);
    
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_UnregisterEventObserverL()
    {
    EUNIT_ASSERT( iMccTranscoder->iEventObserver );
    iMccTranscoder->UnregisterEventObserver();
    EUNIT_ASSERT( !iMccTranscoder->iEventObserver );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_TranscodeFileLL()
    {
    EUNIT_ASSERT( iMccTranscoder->FindSession( 1 ) == KErrNotFound );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    
    TMccTranscodeFileMsgBuffer data( iData );
    TUint32 sessionId1 = 0;
    iMccTranscoder->TranscodeFileL( sessionId1, data );
    EUNIT_ASSERT( sessionId1 != 0 );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    TUint32 sessionId2 = 0;
    iMccTranscoder->TranscodeFileL( sessionId2, data );
    EUNIT_ASSERT( sessionId2 != 0 );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 2 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( sessionId2 != sessionId1 );
    
    EUNIT_ASSERT( iMccTranscoder->FindSession( sessionId1 ) == 0 );
    EUNIT_ASSERT( iMccTranscoder->FindSession( sessionId2 ) == 1 );
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_CancelTranscodeFileLL()
    {
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    TMccTranscodeFileMsgBuffer data( iData );
    TUint32 sessionId = 0;

    EUNIT_ASSERT_LEAVE( iMccTranscoder->CancelTranscodeFileL( sessionId ) );
    
    iMccTranscoder->TranscodeFileL( sessionId, data );
    iMccTranscoder->CancelTranscodeFileL( sessionId );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccTranscodeCancelled );
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_NotifyVideoClipAddedL()
    {
    // Quality change is not needed after clip has been added
    //
    
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    TMccTranscodeFileMsgBuffer data( iData );
    
    iMccTranscoder->NotifyVideoClipAdded( *iMovie, 0 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    
    iMccTranscoder->NotifyVideoClipAdded( iMccTranscoder->iSessionArray[0]->Movie(), 0 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT_EQUALS( iMccTranscoder->iSessionArray[0]->Movie().iCurrentCallback, 
                         CVedMovie::ENotifyMovieOutputParametersChanged );
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_NotifyVideoClipAdded2L()
    {
    // Quality change is done once clip is added
    //
    
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    TMccTranscodeFileMsgBuffer data( iData );
    
    iMccTranscoder->NotifyVideoClipAdded( *iMovie, 0 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    
    iMccTranscoder->NotifyVideoClipAdded( iMccTranscoder->iSessionArray[0]->Movie(), 0 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT_EQUALS( iMccTranscoder->iSessionArray[0]->Movie().iCurrentCallback, 
                         CVedMovie::ENotifyMovieQualityChanged );
    
    }
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_NotifyVideoClipAddingFailedL()
    {
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( iMccEvent.iErrorCode == KErrNone );
    TMccTranscodeFileMsgBuffer data( iData );

    iMccTranscoder->NotifyVideoClipAddingFailed( *iMovie, KErrArgument );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    
    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    iMccTranscoder->NotifyVideoClipAddingFailed( iMccTranscoder->iSessionArray[0]->Movie(), KErrArgument );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccTranscodeCompleted );
    EUNIT_ASSERT( iMccEvent.iErrorCode != KErrNone );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_NotifyVideoClipRemovedL()
    {
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( iMccEvent.iErrorCode == KErrNone );
    TMccTranscodeFileMsgBuffer data( iData );

    iMccTranscoder->NotifyVideoClipRemoved( *iMovie, 0 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    
    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    iMccTranscoder->NotifyVideoClipRemoved( iMccTranscoder->iSessionArray[0]->Movie(), 0 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( iMccEvent.iErrorCode == KErrNone );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_NotifyMovieQualityChangedL()
    {
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( iMccEvent.iErrorCode == KErrNone );
    TMccTranscodeFileMsgBuffer data( iData );

    iMccTranscoder->NotifyMovieQualityChanged( *iMovie );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    
    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    iMccTranscoder->NotifyMovieQualityChanged( iMccTranscoder->iSessionArray[0]->Movie() );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( iMccEvent.iErrorCode == KErrNone );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );

    /* error in SetOutputParametersL
     * 
    Reset();
    iMccTranscoder->NotifyMovieQualityChanged( iMccTranscoder->iSessionArray[0]->Movie() );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccTranscodeCompleted );
    EUNIT_ASSERT( iMccEvent.iErrorCode != KErrNone );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    */

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_NotifyMovieProcessingStartedLL()
    {
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( iMccEvent.iErrorCode == KErrNone );
    TMccTranscodeFileMsgBuffer data( iData );

    iMccTranscoder->NotifyMovieProcessingStartedL( *iMovie );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    
    iMccTranscoder->NotifyMovieProcessingStartedL( iMccTranscoder->iSessionArray[0]->Movie() );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccTranscodeInProgress );
    EUNIT_ASSERT( iMccEvent.iErrorCode == KErrNone );

    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_NotifyMovieOutputParametersChangedL()
    {
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( iMccEvent.iErrorCode == KErrNone );
    TMccTranscodeFileMsgBuffer data( iData );

    iMccTranscoder->NotifyMovieOutputParametersChanged( *iMovie );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    
    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    iMccTranscoder->NotifyMovieOutputParametersChanged( iMccTranscoder->iSessionArray[0]->Movie() );
    TC_CHECK_MEMORY_LEAVE( iMccEvent.iEventType == KMccEventNone );
    
    //processL fails
    Reset( KErrNotFound );
    iMccTranscoder->NotifyMovieOutputParametersChanged( iMccTranscoder->iSessionArray[0]->Movie() );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccTranscodeCompleted );
    EUNIT_ASSERT( iMccEvent.iErrorCode != KErrNone );

    

    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_NotifyMovieProcessingProgressedL()                                                                                                                                                                                                                                                                                    
    {
    TInt testData1 = 5;
    TInt testData2 = 10;
    
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( iMccEvent.iErrorCode == KErrNone );
    TMccTranscodeFileMsgBuffer data( iData );

    iMccTranscoder->NotifyMovieProcessingProgressed( *iMovie, 0 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    // Progress is notified only at 10%, 20% etc.
    iMccTranscoder->NotifyMovieProcessingProgressed( iMccTranscoder->iSessionArray[0]->Movie(), testData1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    
    iMccTranscoder->NotifyMovieProcessingProgressed( iMccTranscoder->iSessionArray[0]->Movie(), testData2 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccTranscodeInProgress );
    EUNIT_ASSERT( iMccEvent.iEventNumData == testData2 );
    
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_NotifyMovieProcessingCompletedL() 
    {
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    EUNIT_ASSERT( iMccEvent.iErrorCode == KErrNone );
    TMccTranscodeFileMsgBuffer data( iData );

    iMccTranscoder->NotifyMovieProcessingCompleted( *iMovie, 0 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    iMccTranscoder->NotifyMovieProcessingCompleted( iMccTranscoder->iSessionArray[0]->Movie(), KErrArgument );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccTranscodeCompleted );
    EUNIT_ASSERT( iMccEvent.iErrorCode == KErrArgument );
    EUNIT_ASSERT( iMccEvent.iEventNumData == 100 );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoderImpl::UT_CMccTranscoderImpl_AsyncTimerExpiredL()
    {

    iMccTranscoder->AsyncTimerExpired( NULL );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 0 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    iMccTranscoder->AsyncTimerExpired( iMccTranscoder );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 0 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    TMccTranscodeFileMsgBuffer data( iData );
    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );

    iMccTranscoder->AsyncTimerExpired( iMccTranscoder );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 1 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    
    iMccTranscoder->RemoveSession( 0 );
    iMccTranscoder->AsyncTimerExpired( iMccTranscoder );
    EUNIT_ASSERT( iMccTranscoder->iSessionArray.Count() == 0 );
    EUNIT_ASSERT( iMccEvent.iEventType == KMccEventNone );
    

    }

   


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccTranscoderImpl,
    "UT_CMccTranscoderImpl",
    "UNIT" )

EUNIT_TEST(
    "RegisterEventObserver - test",
    "CMccTranscoderImpl",
    "RegisterEventObserver",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_RegisterEventObserverL, Teardown)

EUNIT_TEST(
    "UnregisterEventObserver - test",
    "CMccTranscoderImpl",
    "UnregisterEventObserver",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_UnregisterEventObserverL, Teardown)

EUNIT_TEST(
    "TranscodeFileL - test",
    "CMccTranscoderImpl",
    "TranscodeFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_TranscodeFileLL, Teardown)

EUNIT_TEST(
    "CancelTranscodeFileL - test",
    "CMccTranscoderImpl",
    "CancelTranscodeFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_CancelTranscodeFileLL, Teardown)

EUNIT_TEST(
    "NotifyVideoClipAdded - test",
    "CMccTranscoderImpl",
    "NotifyVideoClipAdded",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_NotifyVideoClipAddedL, Teardown)

EUNIT_TEST(
    "NotifyVideoClipAdded 2 - test",
    "CMccTranscoderImpl",
    "NotifyVideoClipAdded 2",
    "FUNCTIONALITY",
    Setup2L, UT_CMccTranscoderImpl_NotifyVideoClipAdded2L, Teardown)

EUNIT_TEST(
    "NotifyVideoClipAddingFailed - test",
    "CMccTranscoderImpl",
    "NotifyVideoClipAddingFailed",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_NotifyVideoClipAddingFailedL, Teardown)

EUNIT_TEST(
    "NotifyVideoClipRemoved - test",
    "CMccTranscoderImpl",
    "NotifyVideoClipRemoved",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_NotifyVideoClipRemovedL, Teardown)

EUNIT_TEST(
    "NotifyMovieQualityChanged - test",
    "CMccTranscoderImpl",
    "NotifyMovieQualityChanged",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_NotifyMovieQualityChangedL, Teardown)

EUNIT_TEST(
    "NotifyMovieOutputParametersChanged - test",
    "CMccTranscoderImpl",
    "NotifyMovieOutputParametersChanged",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_NotifyMovieOutputParametersChangedL, Teardown)


EUNIT_TEST(
    "NotifyMovieProcessingStartedL - test",
    "CMccTranscoderImpl",
    "NotifyMovieProcessingStartedL",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_NotifyMovieProcessingStartedLL, Teardown)

EUNIT_TEST(
    "NotifyMovieProcessingProgressed - test",
    "CMccTranscoderImpl",
    "NotifyMovieProcessingProgressed",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_NotifyMovieProcessingProgressedL, Teardown)

EUNIT_TEST(
    "NotifyMovieProcessingCompleted - test",
    "CMccTranscoderImpl",
    "NotifyMovieProcessingCompleted",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_NotifyMovieProcessingCompletedL, Teardown)

EUNIT_TEST(
    "AsyncTimerExpired - test",
    "CMccTranscoderImpl",
    "AsyncTimerExpired",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoderImpl_AsyncTimerExpiredL, Teardown)


EUNIT_END_TEST_TABLE

//  END OF FILE


