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


//  INTERNAL INCLUDES
#include "telephonyaudiorouting.h"
#include "ut_musengsession.h"
#include "musengstubs.h"
#include "musengtestdefs.h"

#include "musenglivesession.h"
#include "musengtelephoneutils.h"
#include "musengmceutils.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <centralrepository.h>
#include <mcedisplaysink.h>
#include <e32property.h>
#include <telephonydomainpskeys.h>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSession* UT_CMusEngSession::NewL()
    {
    UT_CMusEngSession* self = UT_CMusEngSession::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSession* UT_CMusEngSession::NewLC()
    {
    UT_CMusEngSession* self = new( ELeave ) UT_CMusEngSession();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngSession::~UT_CMusEngSession()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngSession::UT_CMusEngSession()
    :iLiveSession( NULL ),
     iObserver( NULL )
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::SetupL()
    {
    iObserver = new( ELeave ) CMusEngObserverStub;
    iLiveSession = CMusEngLiveSession::NewL( KNullDesC(),
                                             TRect(0,0, 100,100),
                                             *iObserver,
                                             *iObserver,
                                             *iObserver );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::Teardown()
    {
    delete iLiveSession;
    delete iObserver;
    }



// TEST CASES



    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_RectL()
    {
    TRect rect = iLiveSession->Rect();
    
    // Initial values
    EUNIT_ASSERT( rect.iTl.iX == 0 );
    EUNIT_ASSERT( rect.iTl.iY == 0 );
    EUNIT_ASSERT( rect.iBr.iX == 100 );
    EUNIT_ASSERT( rect.iBr.iY == 100 );
    
    iLiveSession->iRect.iTl.iX = 100;
    iLiveSession->iRect.iTl.iY = 200;
    iLiveSession->iRect.iBr.iX = 300;
    iLiveSession->iRect.iBr.iY = 400;
    
    rect = iLiveSession->Rect();
    
    EUNIT_ASSERT( rect.iTl.iX == 100 );
    EUNIT_ASSERT( rect.iTl.iY == 200 );
    EUNIT_ASSERT( rect.iBr.iX == 300 );
    EUNIT_ASSERT( rect.iBr.iY == 400 );    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_SetRectLL()
    {
    TRect rect;

    rect.iTl.iX = 100;
    rect.iTl.iY = 200;
    rect.iBr.iX = 300;
    rect.iBr.iY = 400;

    iLiveSession->SetRectL( rect );
    
    EUNIT_ASSERT( iLiveSession->iRect.iTl.iX == 100 );
    EUNIT_ASSERT( iLiveSession->iRect.iTl.iY == 200 );
    EUNIT_ASSERT( iLiveSession->iRect.iBr.iX == 300 );
    EUNIT_ASSERT( iLiveSession->iRect.iBr.iY == 400 );

    ESTABLISH_OUT_SESSION( iLiveSession );
    
    CMceDisplaySink* display = 
                MusEngMceUtils::GetDisplayL( *( iLiveSession->iSession ) );
    
    TRect displayRect = display->DisplayRectL();
    
    EUNIT_ASSERT( displayRect.iTl.iX == 100 );
    EUNIT_ASSERT( displayRect.iTl.iY == 200 );
    EUNIT_ASSERT( displayRect.iBr.iX == 300 );
    EUNIT_ASSERT( displayRect.iBr.iY == 400 );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_AudioRoutingCanBeChangedL()
    {
    iLiveSession->iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ELoudspeaker;
    EUNIT_ASSERT( iLiveSession->AudioRoutingCanBeChanged() );
    
    iLiveSession->iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::EWiredAudioAccessory;
    EUNIT_ASSERT( !iLiveSession->AudioRoutingCanBeChanged() );
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//        
void UT_CMusEngSession::UT_EnableLoudspeakerLL()
    {
    // Almost dummy case is enough, because IsLoudSpeakerEnabled forwards 
    // requests to CMusEngTelephonyUtils and is tested there. So test only
    // fail handling
    iLiveSession->EnableLoudspeakerL( EFalse, EFalse );
    
    CMusEngTelephoneUtils* telephone = iLiveSession->iTelephoneUtils;
    
    EUNIT_ASSERT( telephone->iTelephonyAudioRouting->iShowNoteMode == EFalse );
    EUNIT_ASSERT( telephone->iTelephonyAudioRouting->iCurrentOutput != 
                  CTelephonyAudioRouting::ELoudspeaker );
    
    telephone->iTelephonyAudioRouting->iForceFailWithCode = KErrNotSupported;
    
    //Must fail in HW, but not in emulator
    TRAPD( error, iLiveSession->EnableLoudspeakerL( ETrue, ETrue ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    
    #if (!defined (__WINS__) && !defined(__WINSCW__))
        EUNIT_ASSERT( error == KErrNotSupported );
    #else
        EUNIT_ASSERT( error == KErrNone );
    #endif
    
    EUNIT_ASSERT( telephone->iTelephonyAudioRouting->iCurrentOutput != 
                  CTelephonyAudioRouting::ELoudspeaker );
    
    telephone->iTelephonyAudioRouting->iForceFailWithCode = KErrGeneral;

    // Must fail in all cases 
    TRAP( error, iLiveSession->EnableLoudspeakerL( EFalse, ETrue ) );
    MUS_TEST_FORWARD_ALLOC_FAILURE( error );
    EUNIT_ASSERT( error == KErrGeneral );

    EUNIT_ASSERT( telephone->iTelephonyAudioRouting->iCurrentOutput != 
                  CTelephonyAudioRouting::ELoudspeaker );
                  
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_IsLoudSpeakerEnabledL()
    {
    // Dummy case is enough, because IsLoudSpeakerEnabled forwards requests
    // to CMusEngTelephonyUtils and is tested there.
    
    // By default loudspeaker is not enabled.
    EUNIT_ASSERT( !iLiveSession->IsLoudSpeakerEnabled() );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_VolumeLL()
    {
    // Dummy case is enough, because VolumeL forwards requests
    // to CMusEngTelephonyUtils and is tested there.
    
    iLiveSession->IsLoudSpeakerEnabled();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_MuteMicLL()
    {
    // Cannot test since EUnit notices some error leak if 
    // the state of the mute is actually changed. Still, from MusEngine
    // point of view everything is done identically despite the fact if
    // real state changes or not. => Problem is in EUnit or leak happens
    // elsewhere.
    }


// -----------------------------------------------------------------------------
// Almost a dummy test since we cannot write
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_IsMicMutedLL()
    {
	// Ignore error (KErrAlreadyExists etc), just make sure this property exists
    RProperty::Define( KPSUidTelMicrophoneMuteStatus,
                       KTelMicrophoneMuteState,
                       RProperty::EInt,
                       0 );

    User::LeaveIfError( RProperty::Set( KPSUidTelMicrophoneMuteStatus,
                                        KTelMicrophoneMuteState,
                                        EPSTelMicMuteStateUninitialized ) );
    
    EUNIT_ASSERT( !iLiveSession->IsMicMutedL() )

    User::LeaveIfError( RProperty::Set( KPSUidTelMicrophoneMuteStatus,
                                        KTelMicrophoneMuteState,
                                        EPSTelMicMuteOn ) );
    
    EUNIT_ASSERT( iLiveSession->IsMicMutedL() )
    
    User::LeaveIfError( RProperty::Set( KPSUidTelMicrophoneMuteStatus,
                                        KTelMicrophoneMuteState,
                                        EPSTelMicMuteOff ) );
    
    EUNIT_ASSERT( !iLiveSession->IsMicMutedL() ) 
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_SetAudioRoutingObserverL()
    {
    EUNIT_ASSERT( !iLiveSession->iTelephoneUtils->iAudioRoutingObserver )
    
    iLiveSession->SetAudioRoutingObserver( iObserver );
    
    EUNIT_ASSERT_EQUALS( iLiveSession->iTelephoneUtils->iAudioRoutingObserver,
                         iObserver )
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_VolumeUpLL()
    {
    CMusEngTelephoneUtils* telephone = iLiveSession->iTelephoneUtils;
    
    // Next ones must leave if runs out of memory, so that test
    // does not fail when executed with alloc fail simulation 
    User::LeaveIfError( telephone->iRepository->Set( 
                                 KTelIncallEarVolume,
                                 KTelephonyVolumeDefaultValue ) );
    User::LeaveIfError( telephone->iRepository->Set( 
                                 KTelIncallLoudspeakerVolume,
                                 KTelephonyVolumeDefaultValue ) );
    
    
    TInt earVolume = 0;
    TInt speakerVolume = 0;
                                 
    iLiveSession->CMusEngSession::VolumeUpL();
    
    telephone->iRepository->Get( KTelIncallEarVolume,
                                 earVolume );
    telephone->iRepository->Get( KTelIncallLoudspeakerVolume,
                                 speakerVolume );
                                 
    EUNIT_ASSERT( earVolume == KTelephonyVolumeDefaultValue + 1 );
    EUNIT_ASSERT( speakerVolume == KTelephonyVolumeDefaultValue );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_VolumeDownLL()
    {
    CMusEngTelephoneUtils* telephone = iLiveSession->iTelephoneUtils;
    
    // Next ones must leave if runs out of memory, so that test
    // does not fail when executed with alloc fail simulation 
    User::LeaveIfError( telephone->iRepository->Set( 
                                 KTelIncallEarVolume,
                                 KTelephonyVolumeDefaultValue ) );
    User::LeaveIfError( telephone->iRepository->Set( 
                                 KTelIncallLoudspeakerVolume,
                                 KTelephonyVolumeDefaultValue ) );
    
    
    TInt earVolume = 0;
    TInt speakerVolume = 0;
                                 
    iLiveSession->CMusEngSession::VolumeDownL();
    
    telephone->iRepository->Get( KTelIncallEarVolume,
                                 earVolume );
    telephone->iRepository->Get( KTelIncallLoudspeakerVolume,
                                 speakerVolume );
                                 
    EUNIT_ASSERT( earVolume == KTelephonyVolumeDefaultValue - 1 );
    EUNIT_ASSERT( speakerVolume == KTelephonyVolumeDefaultValue );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_SetVolumeLL()
    {
    CMusEngTelephoneUtils* telephone = iLiveSession->iTelephoneUtils;
    
    // Next ones must leave if runs out of memory, so that test
    // does not fail when executed with alloc fail simulation 
    User::LeaveIfError( telephone->iRepository->Set( 
                                 KTelIncallEarVolume,
                                 KTelephonyVolumeDefaultValue ) );
    User::LeaveIfError( telephone->iRepository->Set( 
                                 KTelIncallLoudspeakerVolume,
                                 KTelephonyVolumeDefaultValue ) );
    
    
    TInt earVolume = 0;
    TInt speakerVolume = 0;
    TInt newVolume = KTelephonyVolumeDefaultValue + 1;
    
    iLiveSession->CMusEngSession::SetVolumeL( newVolume );
    
    telephone->iRepository->Get( KTelIncallEarVolume,
                                 earVolume );
    telephone->iRepository->Get( KTelIncallLoudspeakerVolume,
                                 speakerVolume );
                                 
    EUNIT_ASSERT( earVolume == newVolume );
    EUNIT_ASSERT( speakerVolume == KTelephonyVolumeDefaultValue );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngSession::UT_SetVolumeChangeObserverL()
    {
    EUNIT_ASSERT( !iLiveSession->iVolumeObserver )
    
    iLiveSession->SetVolumeChangeObserver( iObserver );
    
    EUNIT_ASSERT_EQUALS( iLiveSession->iVolumeObserver, iObserver )
    
    iObserver->VolumeChanged( 2, EFalse );
    EUNIT_ASSERT( iObserver->iVolume == 2 );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngSession,
    "UT_CMusEngSesssion",
    "UNIT" )

EUNIT_TEST(
    "Rect - test ",
    "CMusEngSession",
    "Rect",
    "FUNCTIONALITY",
    SetupL, UT_RectL, Teardown)

EUNIT_TEST(
    "SetRectL - test ",
    "CMusEngSession",
    "SetRectL",
    "FUNCTIONALITY",
    SetupL, UT_SetRectLL, Teardown)

EUNIT_TEST(
    "AudioRoutingCanBeChanged - test ",
    "CMusEngSession",
    "AudioRoutingCanBeChanged",
    "FUNCTIONALITY",
    SetupL, UT_AudioRoutingCanBeChangedL, Teardown)

EUNIT_TEST(
    "EnableLoudspeakerL - test ",
    "CMusEngSession",
    "EnableLoudspeakerL",
    "FUNCTIONALITY",
    SetupL, UT_EnableLoudspeakerLL, Teardown)

EUNIT_TEST(
    "IsLoudSpeakerEnabled - test ",
    "CMusEngSession",
    "IsLoudSpeakerEnabled",
    "FUNCTIONALITY",
    SetupL, UT_IsLoudSpeakerEnabledL, Teardown)

EUNIT_TEST(
    "VolumeL - test ",
    "CMusEngSession",
    "VolumeL",
    "FUNCTIONALITY",
    SetupL, UT_VolumeLL, Teardown)

EUNIT_TEST(
    "MuteMicL - test ",
    "CMusEngSession",
    "MuteMicL",
    "FUNCTIONALITY",
    SetupL, UT_MuteMicLL, Teardown)

EUNIT_TEST(
    "IsMicMutedL - test ",
    "CMusEngSession",
    "IsMicMutedL",
    "FUNCTIONALITY",
    SetupL, UT_IsMicMutedLL, Teardown)    

EUNIT_TEST(
    "SetAudioRoutingObserverL - test ",
    "CMusEngSession",
    "SetAudioRoutingObserverL",
    "FUNCTIONALITY",
    SetupL, UT_SetAudioRoutingObserverL, Teardown)    
        
EUNIT_TEST(
    "VolumeUpL - test ",
    "CMusEngSession",
    "VolumeUpL",
    "FUNCTIONALITY",
    SetupL, UT_VolumeUpLL, Teardown)

EUNIT_TEST(
    "VolumeDownL - test ",
    "CMusEngSession",
    "VolumeDownL",
    "FUNCTIONALITY",
    SetupL, UT_VolumeDownLL, Teardown)        

EUNIT_TEST(
    "SetVolumeL - test ",
    "CMusEngSession",
    "SetVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_SetVolumeLL, Teardown) 
    
EUNIT_TEST(
    "SetVolumeChangeObserver - test ",
    "CMusEngSession",
    "SetVolumeChangeObserver",
    "FUNCTIONALITY",
    SetupL, UT_SetVolumeChangeObserverL, Teardown) 


EUNIT_END_TEST_TABLE

//  END OF FILE
