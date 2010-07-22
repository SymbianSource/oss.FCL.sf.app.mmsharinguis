/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit tests for CMusSoundPlayer class.
*
*/



#include "ut_cmussoundplayer.h"
#include <audiopreference.h>
#include <digia/eunit/eunitmacros.h>


// Next row is to disable warning emerging from EUnit code.
#pragma warn_illtokenpasting off


// ======== MEMBER FUNCTIONS ========


UT_CMusSoundPlayer* UT_CMusSoundPlayer::NewL()
    {
    UT_CMusSoundPlayer* self = UT_CMusSoundPlayer::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


UT_CMusSoundPlayer* UT_CMusSoundPlayer::NewLC()
    {
    UT_CMusSoundPlayer* self = new( ELeave ) UT_CMusSoundPlayer();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


UT_CMusSoundPlayer::~UT_CMusSoundPlayer()
    {
    delete iSoundPlayer;
    iSoundPlayer = NULL;
    }


UT_CMusSoundPlayer::UT_CMusSoundPlayer()
    {
    }


// ---------------------------------------------------------------------------
// The ConstructL from the base class CEUnitTestSuiteClass must be called.
// It generates the test case table.
// ---------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// ---------------------------------------------------------------------------
// From MMusSoundPlayerObserver.
// Notifies of sound playing complete.
// ---------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::PlaySoundComplete( TInt /*aError*/ )
    {
    }


// ----------------------------------------------------------------------------
// Prepares a test for execution.
// ----------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::SetupL()
    {
    iSoundPlayer = CMusSoundPlayer::NewL( *this );
    }


// ----------------------------------------------------------------------------
// Finalizes a test by releasing used resources.
// ----------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::Teardown()
    {
    delete iSoundPlayer;
    }


// ======== UNIT TEST METHODS ========


// ----------------------------------------------------------------------------
// Asserts that instantiation is successful.
// ----------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::UT_CMusSoundPlayer_NewLL()
    {
    EUNIT_ASSERT( iSoundPlayer );
    }


// ----------------------------------------------------------------------------
// Asserts that instantiation is successful.
// ----------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::UT_CMusSoundPlayer_NewLCL()
    {
    delete iSoundPlayer;
    iSoundPlayer = NULL;
    iSoundPlayer = CMusSoundPlayer::NewLC( *this );
    CleanupStack::Pop( iSoundPlayer );
    EUNIT_ASSERT( iSoundPlayer );
    }


// ----------------------------------------------------------------------------
// Dummy test.
// ----------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::UT_CMusSoundPlayer_PlayLL()
    {
    iSoundPlayer->PlayL( CMusSoundPlayer::EVsAvailableSound );
    EUNIT_ASSERT( iSoundPlayer->iMdaPlayer )
    EUNIT_ASSERT_EQUALS( iSoundPlayer->iState,
                         CMusSoundPlayer::ESoundPlayerPlaying )
    
    EUNIT_ASSERT_SPECIFIC_LEAVE( 
                    iSoundPlayer->PlayL( CMusSoundPlayer::EVsAvailableSound ),
                    KErrNotReady )
    }


// ----------------------------------------------------------------------------
// Asserts that stopping works.
// ----------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::UT_CMusSoundPlayer_StopL()
    {
    iSoundPlayer->PlayL( CMusSoundPlayer::EVsAvailableSound );
    iSoundPlayer->Stop();
    EUNIT_ASSERT( CMusSoundPlayer::ESoundPlayerReady == iSoundPlayer->iState );
    
    // No harm done if called second time
    iSoundPlayer->Stop();
    }


// ----------------------------------------------------------------------------
// Asserts that failure in init deletes MDA sound player instance.
// ----------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::UT_CMusSoundPlayer_MapcInitCompleteL()
    {
    iSoundPlayer->iMdaPlayer = CMdaAudioPlayerUtility::NewFilePlayerL(
        _L( "C:\\wav.wav" ),
        *iSoundPlayer,
        EMdaPriorityMax,
        (TMdaPriorityPreference) KAudioPrefVoiceAid );
    iSoundPlayer->MapcInitComplete( KErrGeneral, 1000 );
    EUNIT_ASSERT( !iSoundPlayer->iMdaPlayer );
    }


// ----------------------------------------------------------------------------
// Asserts that completion of playback deletes MDA sound player instance.
// ----------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::UT_CMusSoundPlayer_MapcPlayCompleteL()
    {
    iSoundPlayer->iMdaPlayer = CMdaAudioPlayerUtility::NewFilePlayerL(
        _L( "C:\\wav.wav" ),
        *iSoundPlayer,
        EMdaPriorityMax,
        (TMdaPriorityPreference) KAudioPrefVoiceAid );
    iSoundPlayer->MapcPlayComplete( KErrNone );
    EUNIT_ASSERT( !iSoundPlayer->iMdaPlayer );
    }


// ----------------------------------------------------------------------------
// Asserts that sound filename is generated.
// ----------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::UT_CMusSoundPlayer_SoundFileNameLCL()
    {
    HBufC* filename = iSoundPlayer->SoundFileNameLC(
        CMusSoundPlayer::EVsAvailableSound );
    EUNIT_ASSERT( filename );
    EUNIT_ASSERT( filename->Length() );
    CleanupStack::PopAndDestroy( filename );
    }


// ----------------------------------------------------------------------------
// 
// ----------------------------------------------------------------------------
//
void UT_CMusSoundPlayer::IT_CMusSoundPlayer_DeletePlayerWhenPlaying()
    {
    iSoundPlayer->PlayL( CMusSoundPlayer::EVsAvailableSound );
    EUNIT_ASSERT( iSoundPlayer->iMdaPlayer )
    EUNIT_ASSERT_EQUALS( iSoundPlayer->iState,
                         CMusSoundPlayer::ESoundPlayerPlaying )

    delete iSoundPlayer;
    iSoundPlayer = NULL;
    }
 

// ======== EUNIT TEST TABLE ========


EUNIT_BEGIN_TEST_TABLE(
    UT_CMusSoundPlayer,
    "UT_CMusSoundPlayer",
    "UNIT" )

EUNIT_TEST(
    "NewL - test",
    "CMusSoundPlayer",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusSoundPlayer_NewLL, Teardown )

EUNIT_TEST(
    "NewLC - test ",
    "CMusSoundPlayer",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusSoundPlayer_NewLCL, Teardown )

EUNIT_TEST(
    "PlayL - test ",
    "CMusSoundPlayer",
    "PlayL",
    "FUNCTIONALITY",
    SetupL, UT_CMusSoundPlayer_PlayLL, Teardown )

EUNIT_TEST(
    "Stop - test ",
    "CMusSoundPlayer",
    "Stop",
    "FUNCTIONALITY",
    SetupL, UT_CMusSoundPlayer_StopL, Teardown )

EUNIT_TEST(
    "MapcInitComplete - test ",
    "CMusSoundPlayer",
    "MapcInitComplete",
    "FUNCTIONALITY",
    SetupL, UT_CMusSoundPlayer_MapcInitCompleteL, Teardown )

EUNIT_TEST(
    "MapcPlayComplete - test ",
    "CMusSoundPlayer",
    "MapcPlayComplete",
    "FUNCTIONALITY",
    SetupL, UT_CMusSoundPlayer_MapcPlayCompleteL, Teardown )

EUNIT_TEST(
    "SoundFileNameLC - test ",
    "CMusSoundPlayer",
    "SoundFileNameLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusSoundPlayer_SoundFileNameLCL, Teardown )

EUNIT_TEST(
    "Test deleting player when playing",
    "CMusSoundPlayer",
    "Destructor",
    "FUNCTIONALITY",
    SetupL, IT_CMusSoundPlayer_DeletePlayerWhenPlaying, Teardown )


EUNIT_END_TEST_TABLE
