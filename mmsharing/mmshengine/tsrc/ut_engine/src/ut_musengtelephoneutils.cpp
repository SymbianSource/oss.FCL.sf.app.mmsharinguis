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
#include "ut_musengtelephoneutils.h"
#include "musengstubs.h"
#include "musengtestdefs.h"

#include "musengtelephoneutils.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <centralrepository.h>
#include <e32property.h>



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngTelephoneUtils* UT_CMusEngTelephoneUtils::NewL()
    {
    UT_CMusEngTelephoneUtils* self = UT_CMusEngTelephoneUtils::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngTelephoneUtils* UT_CMusEngTelephoneUtils::NewLC()
    {
    UT_CMusEngTelephoneUtils* self = new( ELeave ) UT_CMusEngTelephoneUtils();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusEngTelephoneUtils::~UT_CMusEngTelephoneUtils()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusEngTelephoneUtils::UT_CMusEngTelephoneUtils()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::SetupL()
    {
    iObserver = new( ELeave ) CMusEngObserverStub;
    iTelephoneUtils = CMusEngTelephoneUtils::NewL( *iObserver );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::Teardown()
    {
    delete iTelephoneUtils;
    iTelephoneUtils = NULL;
    delete iObserver;
    iObserver = NULL;
    PropertyHelper::Close();
    }


// TEST CASES  

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_NewLL()
    {
    EUNIT_ASSERT( iTelephoneUtils );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_AudioRoutingCanBeChangedL()
    {
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ENotActive;
    EUNIT_ASSERT( iTelephoneUtils->AudioRoutingCanBeChanged() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ENone;
    EUNIT_ASSERT( iTelephoneUtils->AudioRoutingCanBeChanged() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::EHandset;
    EUNIT_ASSERT( iTelephoneUtils->AudioRoutingCanBeChanged() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ELoudspeaker;
    EUNIT_ASSERT( iTelephoneUtils->AudioRoutingCanBeChanged() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::EWiredAudioAccessory;
    EUNIT_ASSERT( !iTelephoneUtils->AudioRoutingCanBeChanged() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::EBTAudioAccessory;
    EUNIT_ASSERT( !iTelephoneUtils->AudioRoutingCanBeChanged() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ETTY;
    EUNIT_ASSERT( !iTelephoneUtils->AudioRoutingCanBeChanged() );         
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_LoudspeakerLL()
    {
    // Precondition of test
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::EHandset;
    
    // Set loudspeaker on, succeeds
    iTelephoneUtils->LoudspeakerL( ETrue );
    EUNIT_ASSERT( iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput ==
                  CTelephonyAudioRouting::ELoudspeaker );
    EUNIT_ASSERT( iTelephoneUtils->iTelephonyAudioRouting->iShowNoteMode ==
                  EFalse );
    
    // Change conditions
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::EWiredAudioAccessory;              
    
    // Try to set loudspeaker on, fails because current set is not handset
    iTelephoneUtils->LoudspeakerL( ETrue );
    EUNIT_ASSERT( iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput ==
                  CTelephonyAudioRouting::EWiredAudioAccessory );  
    
    // Change conditions
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ELoudspeaker;           
    iTelephoneUtils->iAudioOutputAtStartup =
                  CTelephonyAudioRouting::ELoudspeaker;  
                              
    // Try to set loudspeaker off, goes to handset
    iTelephoneUtils->LoudspeakerL( EFalse );   
    EUNIT_ASSERT( iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput ==
                  CTelephonyAudioRouting::EHandset );  
    
    // Change conditions
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ELoudspeaker;           
    iTelephoneUtils->iAudioOutputAtStartup =
                  CTelephonyAudioRouting::EWiredAudioAccessory; 
    
    // Try to set loudspeaker off, goes to wired
    iTelephoneUtils->LoudspeakerL( EFalse );   
    EUNIT_ASSERT( iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput ==
                  CTelephonyAudioRouting::EWiredAudioAccessory ); 
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_IsLoudSpeakerEnabledL()
    {
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                                        CTelephonyAudioRouting::ELoudspeaker;
    EUNIT_ASSERT( iTelephoneUtils->IsLoudSpeakerEnabled() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                                        CTelephonyAudioRouting::EHandset;
    EUNIT_ASSERT( !iTelephoneUtils->IsLoudSpeakerEnabled() );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_GetVolumeLL()
    {
    TInt volume = 0;
    
    volume = iTelephoneUtils->GetVolumeL();
    EUNIT_ASSERT( volume == 4 );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                                        CTelephonyAudioRouting::ELoudspeaker;
    iTelephoneUtils->iRepository->Set( KTelIncallLoudspeakerVolume, 8 );
    
    volume = iTelephoneUtils->GetVolumeL();
    EUNIT_ASSERT( volume == 8 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_SetVolumeLL()
    {
    TInt handsetVolume = 0;
    TInt loudSpeakerVolume = 0;
    
    // Try with current value, does nothing
    iTelephoneUtils->SetVolumeL( 4 );
    
    iTelephoneUtils->iRepository->Get( KTelIncallEarVolume, 
                                       handsetVolume );
    EUNIT_ASSERT( handsetVolume == 4 );
    
    // Try to change handset volume
    iTelephoneUtils->SetVolumeL( 5 );
    
    iTelephoneUtils->iRepository->Get( KTelIncallEarVolume, 
                                       handsetVolume );
    EUNIT_ASSERT( handsetVolume == 5 );
    
    iTelephoneUtils->iRepository->Get( KTelIncallLoudspeakerVolume, 
                                       loudSpeakerVolume );
    EUNIT_ASSERT( loudSpeakerVolume == 4 );
    
    // Try to change loudspeaker volume
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                                        CTelephonyAudioRouting::ELoudspeaker;
    
    iTelephoneUtils->SetVolumeL( 3 );
    
    iTelephoneUtils->iRepository->Get( KTelIncallEarVolume, 
                                       handsetVolume );
    EUNIT_ASSERT( handsetVolume == 5 );
    
    iTelephoneUtils->iRepository->Get( KTelIncallLoudspeakerVolume, 
                                       loudSpeakerVolume );
    EUNIT_ASSERT( loudSpeakerVolume == 3 );
    
    // Check that volume gets validated
    
    iTelephoneUtils->SetVolumeL( 100 );
    
    iTelephoneUtils->iRepository->Get( KTelIncallLoudspeakerVolume, 
                                       loudSpeakerVolume );
    EUNIT_ASSERT( loudSpeakerVolume == 10 );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_AvailableOutputsChangedL()
    {
    // At the monent function is expected to do nothing so just 
    // call it to avoid coverage loss.
    iTelephoneUtils->AvailableOutputsChanged(  
                            *iTelephoneUtils->iTelephonyAudioRouting );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_OutputChangedL()
    {
    // Loudspeaker
    iTelephoneUtils->iAudioOutputAtStartup = CTelephonyAudioRouting::EHandset;
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput = 
        CTelephonyAudioRouting::ELoudspeaker;
    
    iTelephoneUtils->OutputChanged( *iTelephoneUtils->iTelephonyAudioRouting );
                                                                
    EUNIT_ASSERT_EQUALS( iTelephoneUtils->iAudioOutputAtStartup, 
                         CTelephonyAudioRouting::ELoudspeaker )
    EUNIT_ASSERT( iObserver->iAudioRoutingChangedCalled )
    
    // Handset   
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput = 
        CTelephonyAudioRouting::EHandset;
    
    iTelephoneUtils->OutputChanged( *iTelephoneUtils->iTelephonyAudioRouting );
                                                                
    EUNIT_ASSERT_EQUALS( iTelephoneUtils->iAudioOutputAtStartup,
                         CTelephonyAudioRouting::EHandset )
    
    EUNIT_ASSERT( iObserver->iAudioRoutingChangedCalled )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_SetOutputCompleteL()
    {
    // Setoutput fails
    iTelephoneUtils->SetOutputComplete( 
                            *iTelephoneUtils->iTelephonyAudioRouting,
                            KErrGeneral );                         
    EUNIT_ASSERT( !iObserver->iAudioRoutingChangedCalled )
    iObserver->Reset();
    
    // Setoutput ok
    iTelephoneUtils->SetOutputComplete( 
                            *iTelephoneUtils->iTelephonyAudioRouting,
                            KErrNone );
    EUNIT_ASSERT( iObserver->iAudioRoutingChangedCalled )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_ValidateVolumeL()
    {
    EUNIT_ASSERT( iTelephoneUtils->ValidateVolume( 0 ) == 0 );
    EUNIT_ASSERT( iTelephoneUtils->ValidateVolume( 11 ) == 10 );
    EUNIT_ASSERT( iTelephoneUtils->ValidateVolume( 5 ) == 5 );
    }

// -----------------------------------------------------------------------------
// We cannot assert anything after destruction. All we can do is to make
// such condition before destruction that coverage will be achieved.
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_DestructorL()
    {
    // Simulate that 2nd phase construction has not succeeded
    CMusEngTelephoneUtils* utils = CMusEngTelephoneUtils::NewL( *iObserver );
    CleanupStack::PushL( utils );
    delete utils->iTelephonyAudioRouting;
    utils->iTelephonyAudioRouting = NULL;
    CleanupStack::PopAndDestroy( utils );
    
    // Simulate that current audio output mode is same as original
    utils = CMusEngTelephoneUtils::NewL( *iObserver );
    CleanupStack::PushL( utils );
    utils->iAudioOutputAtStartup = CTelephonyAudioRouting::EHandset;
    utils->iTelephonyAudioRouting->iCurrentOutput = 
                                            CTelephonyAudioRouting::EHandset;
    CleanupStack::PopAndDestroy( utils );
    
    // Simulate that current audio output mode is not the same as original,
    // Setting fails
    utils = CMusEngTelephoneUtils::NewL( *iObserver );
    CleanupStack::PushL( utils );
    utils->iAudioOutputAtStartup = CTelephonyAudioRouting::EHandset;
    utils->iTelephonyAudioRouting->iCurrentOutput = 
                                    CTelephonyAudioRouting::ELoudspeaker;
    utils->iTelephonyAudioRouting->iForceFailWithCode = KErrGeneral;
    CleanupStack::PopAndDestroy( utils );
    
    // Simulate that current audio output mode is not the same as original,
    // Setting succeeds
    utils = CMusEngTelephoneUtils::NewL( *iObserver );
    CleanupStack::PushL( utils );
    utils->iAudioOutputAtStartup = CTelephonyAudioRouting::EHandset;
    utils->iTelephonyAudioRouting->iCurrentOutput = 
                                    CTelephonyAudioRouting::ELoudspeaker;
    CleanupStack::PopAndDestroy( utils );
    
    // Simulate that current audio output mode is not the same as original,
    // Setting does not succeed as observer does not allow changes anymore
    utils = CMusEngTelephoneUtils::NewL( *iObserver );
    CleanupStack::PushL( utils );
    utils->iAudioOutputAtStartup = CTelephonyAudioRouting::EHandset;
    utils->iTelephonyAudioRouting->iCurrentOutput = 
                                    CTelephonyAudioRouting::ELoudspeaker;
    CleanupStack::PopAndDestroy( utils );
    // Cannot really assert anything
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusEngTelephoneUtils,
    "UT_CMusEngTelephoneUtils",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusEngTelephoneUtils",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_NewLL, Teardown)

EUNIT_TEST(
    "AudioRoutingCanBeChanged - test ",
    "CMusEngTelephoneUtils",
    "AudioRoutingCanBeChanged",
    "FUNCTIONALITY",
    SetupL, UT_AudioRoutingCanBeChangedL, Teardown)

EUNIT_TEST(
    "LoudspeakerL - test ",
    "CMusEngTelephoneUtils",
    "LoudspeakerL",
    "FUNCTIONALITY",
    SetupL, UT_LoudspeakerLL, Teardown)

EUNIT_TEST(
    "IsLoudSpeakerEnabled - test ",
    "CMusEngTelephoneUtils",
    "IsLoudSpeakerEnabled",
    "FUNCTIONALITY",
    SetupL, UT_IsLoudSpeakerEnabledL, Teardown)    
    
EUNIT_TEST(
    "GetVolumeL - test ",
    "CMusEngTelephoneUtils",
    "GetVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_GetVolumeLL, Teardown)

EUNIT_TEST(
    "SetVolumeL - test ",
    "CMusEngTelephoneUtils",
    "SetVolumeL",
    "FUNCTIONALITY",
    SetupL, UT_SetVolumeLL, Teardown)

EUNIT_TEST(
    "AvailableOutputsChanged - test ",
    "CMusEngTelephoneUtils",
    "AvailableOutputsChanged",
    "FUNCTIONALITY",
    SetupL, UT_AvailableOutputsChangedL, Teardown)

EUNIT_TEST(
    "OutputChanged - test ",
    "CMusEngTelephoneUtils",
    "OutputChanged",
    "FUNCTIONALITY",
    SetupL, UT_OutputChangedL, Teardown)    
    
EUNIT_TEST(
    "SetOutputComplete - test ",
    "CMusEngTelephoneUtils",
    "SetOutputComplete",
    "FUNCTIONALITY",
    SetupL, UT_SetOutputCompleteL, Teardown)

EUNIT_TEST(
    "ValidateVolume - test ",
    "CMusEngTelephoneUtils",
    "ValidateVolume",
    "FUNCTIONALITY",
    SetupL, UT_ValidateVolumeL, Teardown)

EUNIT_TEST(
    "Destructor - test ",
    "CMusEngTelephoneUtils",
    "Destructor",
    "FUNCTIONALITY",
    SetupL, UT_DestructorL, Teardown)

     
    
EUNIT_END_TEST_TABLE

//  END OF FILE


