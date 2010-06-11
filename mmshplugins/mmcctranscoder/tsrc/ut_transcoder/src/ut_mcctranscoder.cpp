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
#include "ut_mcctranscoder.h"
#include "mmcctranscoder.h"
#include "transcoderimpl.h"

_LIT( KTestMccTranscodeFile, "C:\\data\\videos\\big.3gp" );
_LIT( KTestMccTranscodedFile, "C:\\data\\videos\\normal.3gp" );
_LIT8( KTestMccTranscodedVideoMimeType, "H264" );
_LIT8( KTestMccTranscodedAudioMimeType, "AAC" );


//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMccTranscoder* UT_CMccTranscoder::NewL()
    {
    UT_CMccTranscoder* self = UT_CMccTranscoder::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMccTranscoder* UT_CMccTranscoder::NewLC()
    {
    UT_CMccTranscoder* self = new( ELeave ) UT_CMccTranscoder();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMccTranscoder::~UT_CMccTranscoder()
    {
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMccTranscoder::UT_CMccTranscoder()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoder::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoder::SetupL()
    {
    iMccTranscoder = CMccTranscoder::NewL();
    iMccTranscoder->RegisterEventObserver( *this );

    iData.iSourceFile = KTestMccTranscodeFile();
    iData.iDesFile = KTestMccTranscodedFile();
    iData.iQuality = CVedMovie::EQualityAutomatic;
    iData.iVideoCodec.iMimeType = KTestMccTranscodedVideoMimeType();
    iData.iAudioCodec.iMimeType = KTestMccTranscodedAudioMimeType();

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoder::Teardown()
    {
    delete iMccTranscoder;    
    }

void UT_CMccTranscoder::MccTranscoderEventReceived( TMccEvent& aEvent )
    {
    iMccEvent = aEvent;
    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoder::UT_CMccTranscoder_RegisterEventObserverL()
    {
    iMccTranscoder->RegisterEventObserver( *this );
    delete iMccTranscoder->iTranscoderImpl;
    iMccTranscoder->iTranscoderImpl = NULL;
    
    iMccTranscoder->RegisterEventObserver( *this );
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoder::UT_CMccTranscoder_UnregisterEventObserverL()
    {
    iMccTranscoder->UnregisterEventObserver();
    delete iMccTranscoder->iTranscoderImpl;
    iMccTranscoder->iTranscoderImpl = NULL;
    
    iMccTranscoder->UnregisterEventObserver();
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoder::UT_CMccTranscoder_TranscodeFileLL()
    {

    TMccTranscodeFileMsgBuffer data( iData );
    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    
    delete iMccTranscoder->iTranscoderImpl;
    iMccTranscoder->iTranscoderImpl = NULL;
    
    EUNIT_ASSERT_LEAVE( iMccTranscoder->TranscodeFileL( sessionId, data ) );

    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMccTranscoder::UT_CMccTranscoder_CancelTranscodeFileLL()
    {
    
    EUNIT_ASSERT_LEAVE( iMccTranscoder->CancelTranscodeFileL( 11 ) );

    TMccTranscodeFileMsgBuffer data( iData );
    TUint32 sessionId = 0;
    iMccTranscoder->TranscodeFileL( sessionId, data );
    EUNIT_ASSERT( sessionId );
    
    iMccTranscoder->CancelTranscodeFileL( sessionId );
    
    }



   


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMccTranscoder,
    "UT_CMccTranscoder",
    "UNIT" )

EUNIT_TEST(
    "RegisterEventObserver - test",
    "CMccTranscoder",
    "RegisterEventObserver",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoder_RegisterEventObserverL, Teardown)

EUNIT_TEST(
    "UnregisterEventObserver - test",
    "CMccTranscoder",
    "UnregisterEventObserver",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoder_UnregisterEventObserverL, Teardown)

EUNIT_TEST(
    "TranscodeFileL - test",
    "CMccTranscoder",
    "TranscodeFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoder_TranscodeFileLL, Teardown)

EUNIT_TEST(
    "CancelTranscodeFileL - test",
    "CMccTranscoder",
    "CancelTranscodeFileL",
    "FUNCTIONALITY",
    SetupL, UT_CMccTranscoder_CancelTranscodeFileLL, Teardown)



EUNIT_END_TEST_TABLE

//  END OF FILE


