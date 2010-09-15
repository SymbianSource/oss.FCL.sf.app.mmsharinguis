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
    iTelephoneUtils = CMusEngTelephoneUtils::NewL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::Teardown()
    {
    delete iObserver;
    delete iTelephoneUtils;
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
    EUNIT_ASSERT( iTelephoneUtils->AudioRoutingCanBeChanged() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::EBTAudioAccessory;
    EUNIT_ASSERT( iTelephoneUtils->AudioRoutingCanBeChanged() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ETTY;
    EUNIT_ASSERT( !iTelephoneUtils->AudioRoutingCanBeChanged() );
            
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//	   
void UT_CMusEngTelephoneUtils::UT_IsAudioRoutingHeadsetL()
    {
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::EBTAudioAccessory;
    EUNIT_ASSERT( iTelephoneUtils->IsAudioRoutingHeadset());
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ENotActive;
    EUNIT_ASSERT( !iTelephoneUtils->IsAudioRoutingHeadset() );
       
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ENone;
    EUNIT_ASSERT( !iTelephoneUtils->IsAudioRoutingHeadset() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::EHandset;
    EUNIT_ASSERT( !iTelephoneUtils->IsAudioRoutingHeadset() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ELoudspeaker;
    EUNIT_ASSERT( !iTelephoneUtils->IsAudioRoutingHeadset() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::EWiredAudioAccessory;
    EUNIT_ASSERT( iTelephoneUtils->IsAudioRoutingHeadset() );
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ETTY;
    EUNIT_ASSERT( !iTelephoneUtils->IsAudioRoutingHeadset() );
            
    }

void UT_CMusEngTelephoneUtils::UT_IsAudioRoutingLoudSpeakerL()
	{
	iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
	              CTelephonyAudioRouting::ELoudspeaker;
	iTelephoneUtils->iAudioOutputAtStartup = 
			      CTelephonyAudioRouting::ELoudspeaker;
	EUNIT_ASSERT( !iTelephoneUtils->IsAudioRoutingLoudSpeaker());
	
	iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
				  CTelephonyAudioRouting::ELoudspeaker;
	iTelephoneUtils->iAudioOutputAtStartup = 
			      CTelephonyAudioRouting::EHandset;
	EUNIT_ASSERT( iTelephoneUtils->IsAudioRoutingLoudSpeaker() );
	
	iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
				  CTelephonyAudioRouting::ELoudspeaker;
    iTelephoneUtils->iAudioOutputAtStartup = 
    		      CTelephonyAudioRouting::EBTAudioAccessory;
    EUNIT_ASSERT( iTelephoneUtils->IsAudioRoutingLoudSpeaker() );
		
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ELoudspeaker;
    iTelephoneUtils->iAudioOutputAtStartup = 
    		      CTelephonyAudioRouting::EWiredAudioAccessory;
    EUNIT_ASSERT( iTelephoneUtils->IsAudioRoutingLoudSpeaker() );
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
    iTelephoneUtils->LoudspeakerL( ETrue, ETrue );
    EUNIT_ASSERT( iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput ==
                  CTelephonyAudioRouting::ELoudspeaker );
    EUNIT_ASSERT( iTelephoneUtils->iTelephonyAudioRouting->iShowNoteMode ==
                  EFalse );
    EUNIT_ASSERT( iTelephoneUtils->iShowDialog == ETrue );
    
    // Change conditions
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::EWiredAudioAccessory;              
    
    // Try to set loudspeaker on, succeed
    iTelephoneUtils->LoudspeakerL( ETrue, ETrue );
    EUNIT_ASSERT( iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput ==
    		      CTelephonyAudioRouting::ELoudspeaker );  
    
    // Change conditions
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ELoudspeaker;           
    iTelephoneUtils->iAudioOutputAtStartup =
                  CTelephonyAudioRouting::ELoudspeaker;  
                              
    // Try to set loudspeaker off, goes to handset
    iTelephoneUtils->LoudspeakerL( EFalse, ETrue );   
    EUNIT_ASSERT( iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput ==
                  CTelephonyAudioRouting::EHandset );  
    
    // Change conditions
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput =
                  CTelephonyAudioRouting::ELoudspeaker;           
    iTelephoneUtils->iAudioOutputAtStartup =
                  CTelephonyAudioRouting::EWiredAudioAccessory; 
    
    // Try to set loudspeaker off, goes to wired
    iTelephoneUtils->LoudspeakerL( EFalse, ETrue );   
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
    iTelephoneUtils->iAudioOutputAtStartup = CTelephonyAudioRouting::EHandset;
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput = 
                                        CTelephonyAudioRouting::ELoudspeaker;
    iTelephoneUtils->iRepository->Set( KTelIncallLoudspeakerVolume, 8 );
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 4 )
    EUNIT_ASSERT( iObserver->iVolume == 0 );
    
    iTelephoneUtils->OutputChanged( *iTelephoneUtils->iTelephonyAudioRouting );
                                                                
    EUNIT_ASSERT_EQUALS( iTelephoneUtils->iAudioOutputAtStartup, 
                         CTelephonyAudioRouting::ELoudspeaker )
    
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 8 )
    EUNIT_ASSERT( iObserver->iVolume == 0 );
    // Test observer
    
    iTelephoneUtils->SetAudioRoutingObserver( iObserver );
    iTelephoneUtils->SetVolumeChangeObserver(iObserver);
    
    iTelephoneUtils->iTelephonyAudioRouting->iCurrentOutput = 
                                        CTelephonyAudioRouting::EHandset;
    
    iTelephoneUtils->OutputChanged( *iTelephoneUtils->iTelephonyAudioRouting );
    
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 4 )
    EUNIT_ASSERT( iObserver->iVolume == 4 );                                                               
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
    iTelephoneUtils->SetAudioRoutingObserver( iObserver );
    iTelephoneUtils->SetVolumeChangeObserver(iObserver);
    EUNIT_ASSERT( iObserver->iVolume == 0 );
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 4 )

    // Setoutput fails
    iTelephoneUtils->SetOutputComplete( 
                            *iTelephoneUtils->iTelephonyAudioRouting,
                            KErrGeneral );
                            
    EUNIT_ASSERT( !iObserver->iAudioRoutingChangedCalled )
    EUNIT_ASSERT( iObserver->iVolume == 0 );
    
    // Setoutput succesful and note is shown by audiorouting api
    // There's already next pending setoutput for which we are going to
    // show note -> that cannot be forgotten
    iTelephoneUtils->iRepository->Set( KTelIncallEarVolume, 5 );
    iTelephoneUtils->iShowDialog = ETrue;
    iTelephoneUtils->iTelephonyAudioRouting->SetShowNote( ETrue );
    iTelephoneUtils->SetOutputComplete( 
                            *iTelephoneUtils->iTelephonyAudioRouting,
                            KErrNone );
    EUNIT_ASSERT( iObserver->iAudioRoutingChangedCalled == ETrue )
    EUNIT_ASSERT( iObserver->iShowNote == EFalse )
    EUNIT_ASSERT( iTelephoneUtils->iShowDialog == ETrue )
    EUNIT_ASSERT( iObserver->iVolume == 5 );
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 5 )
    
    iObserver->Reset();
    
    // Setoutput succesful and note is shown by us
    iTelephoneUtils->iTelephonyAudioRouting->SetShowNote( EFalse );
    iTelephoneUtils->SetOutputComplete( 
                            *iTelephoneUtils->iTelephonyAudioRouting,
                            KErrNone );
    EUNIT_ASSERT( iObserver->iAudioRoutingChangedCalled == ETrue )
    EUNIT_ASSERT( iObserver->iShowNote == ETrue )
    EUNIT_ASSERT( iTelephoneUtils->iShowDialog == EFalse )
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
    CMusEngTelephoneUtils* utils = CMusEngTelephoneUtils::NewL();
    CleanupStack::PushL( utils );
    delete utils->iTelephonyAudioRouting;
    utils->iTelephonyAudioRouting = NULL;
    CleanupStack::PopAndDestroy( utils );
    
    // Simulate that current audio output mode is same as original
    utils = CMusEngTelephoneUtils::NewL();
    CleanupStack::PushL( utils );
    utils->iAudioOutputAtStartup = CTelephonyAudioRouting::EHandset;
    utils->iTelephonyAudioRouting->iCurrentOutput = 
                                        CTelephonyAudioRouting::EHandset;
    CTelephonyAudioRouting::iPreviousOutput = 
                                        CTelephonyAudioRouting::ELoudspeaker;
    CleanupStack::PopAndDestroy( utils );
    EUNIT_ASSERT( CTelephonyAudioRouting::iPreviousOutput == 
                  CTelephonyAudioRouting::ELoudspeaker )
    
    // Simulate that current audio output mode is not the same as original,
    // Setting fails
    utils = CMusEngTelephoneUtils::NewL();
    CleanupStack::PushL( utils );
    utils->iAudioOutputAtStartup = CTelephonyAudioRouting::EHandset;
    utils->iTelephonyAudioRouting->iCurrentOutput = 
                                    CTelephonyAudioRouting::ELoudspeaker;
    CTelephonyAudioRouting::iPreviousOutput = 
                                    CTelephonyAudioRouting::EHandset;
    utils->iTelephonyAudioRouting->iForceFailWithCode = KErrGeneral;
    CleanupStack::PopAndDestroy( utils );
    EUNIT_ASSERT( CTelephonyAudioRouting::iPreviousOutput == 
                  CTelephonyAudioRouting::EHandset )
    EUNIT_ASSERT( CTelephonyAudioRouting::iCurrentOutput == 
                  CTelephonyAudioRouting::ELoudspeaker )
    
    // Simulate that current audio output mode is not the same as original,
    // Setting succeeds
    utils = CMusEngTelephoneUtils::NewL();
    CleanupStack::PushL( utils );
    utils->iAudioOutputAtStartup = CTelephonyAudioRouting::EHandset;
    utils->iTelephonyAudioRouting->iCurrentOutput = 
                                    CTelephonyAudioRouting::ELoudspeaker;
    CTelephonyAudioRouting::iPreviousOutput = 
                                    CTelephonyAudioRouting::EHandset;
    CleanupStack::PopAndDestroy( utils );
    EUNIT_ASSERT( CTelephonyAudioRouting::iPreviousOutput == 
                  CTelephonyAudioRouting::ELoudspeaker )
    EUNIT_ASSERT( CTelephonyAudioRouting::iCurrentOutput == 
                  CTelephonyAudioRouting::EHandset )
    
    // Simulate that current audio output mode is not the same as original,
    // Setting not done due to special case handling (ENotActive)
    utils = CMusEngTelephoneUtils::NewL();
    CleanupStack::PushL( utils );
    utils->iAudioOutputAtStartup = CTelephonyAudioRouting::EHandset;
    utils->iTelephonyAudioRouting->iCurrentOutput = 
                                    CTelephonyAudioRouting::ENotActive;
    CTelephonyAudioRouting::iPreviousOutput = 
                                    CTelephonyAudioRouting::ELoudspeaker;
    CleanupStack::PopAndDestroy( utils );
    EUNIT_ASSERT( CTelephonyAudioRouting::iPreviousOutput == 
                  CTelephonyAudioRouting::ELoudspeaker )
    EUNIT_ASSERT( CTelephonyAudioRouting::iCurrentOutput == 
                  CTelephonyAudioRouting::ENotActive )                  
                  
    // Simulate that current audio output mode is not the same as original,
    // Setting does not succeed as observer does not allow changes anymore
    utils = CMusEngTelephoneUtils::NewL();
    iObserver->iAudioRouteChangeAllowed = EFalse;
    utils->SetAudioRoutingObserver( iObserver );
    CleanupStack::PushL( utils );
    utils->iAudioOutputAtStartup = CTelephonyAudioRouting::EHandset;
    utils->iTelephonyAudioRouting->iCurrentOutput = 
                                    CTelephonyAudioRouting::ELoudspeaker;
    CTelephonyAudioRouting::iPreviousOutput = 
                                    CTelephonyAudioRouting::EHandset;
    CleanupStack::PopAndDestroy( utils );
    EUNIT_ASSERT( CTelephonyAudioRouting::iPreviousOutput == 
                  CTelephonyAudioRouting::EHandset )
    EUNIT_ASSERT( CTelephonyAudioRouting::iCurrentOutput == 
                  CTelephonyAudioRouting::ELoudspeaker )
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_UpdateCurrentVolumeL()
    {    
    //Volume changed, no observer
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 4 )
    iTelephoneUtils->iRepository->Set( KTelIncallEarVolume, 5 );
    iTelephoneUtils->UpdateCurrentVolume(EFalse);
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 5 )
    EUNIT_ASSERT( iObserver->iVolume == 0 );

    //Volume changed, observer set
    iTelephoneUtils->SetVolumeChangeObserver(iObserver);
    iTelephoneUtils->iRepository->Set( KTelIncallEarVolume, 6 );
    iTelephoneUtils->UpdateCurrentVolume(EFalse);
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 6 )
    EUNIT_ASSERT( iObserver->iVolume == 6 );

    //Volume didn't change, observer not notified
    iObserver->iVolume = 0;
    iTelephoneUtils->UpdateCurrentVolume(EFalse);
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 6 )
    EUNIT_ASSERT( iObserver->iVolume == 0 );    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusEngTelephoneUtils::UT_HandleNotifyGenericL()
    {
    //Loudspeakers volume changed, no observer
    iTelephoneUtils->iRepository->Set( KTelIncallLoudspeakerVolume, 8 );
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 4 )
    iTelephoneUtils->HandleNotifyGeneric(KTelIncallLoudspeakerVolume);    
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 8 )
    EUNIT_ASSERT( iObserver->iVolume == 0 );
    
    //Ear volume changed, observer set => observer notified
    iTelephoneUtils->SetVolumeChangeObserver(iObserver);
    iTelephoneUtils->HandleNotifyGeneric(KTelIncallEarVolume);    
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 4 )
    EUNIT_ASSERT( iObserver->iVolume == 4 );
    
    //Volume hasn't changed => observer not notifies
    iObserver->iVolume = 0;
    iTelephoneUtils->HandleNotifyGeneric(KTelIncallEarVolume);    
    EUNIT_ASSERT( iTelephoneUtils->iCurrentVolume == 4 )
    EUNIT_ASSERT( iObserver->iVolume == 0 );
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
    "IsAudioRoutingHeadset - test ",
    "CMusEngTelephoneUtils",
    "IsAudioRoutingHeadset",
    "FUNCTIONALITY",
    SetupL, UT_IsAudioRoutingHeadsetL, Teardown)
    
EUNIT_TEST(
    "IsAudioRoutingLoudSpeaker - test ",
    "CMusEngTelephoneUtils",
    "IsAudioRoutingLoudSpeaker",
    "FUNCTIONALITY",
    SetupL, UT_IsAudioRoutingLoudSpeakerL, Teardown)    
    
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

EUNIT_TEST(
    "UpdateCurrentVolume - test ",
    "UpdateCurrentVolume",
    "Destructor",
    "FUNCTIONALITY",
    SetupL, UT_UpdateCurrentVolumeL, Teardown)
    
EUNIT_TEST(
    "HandleNotifyGeneric - test ",
    "CMusEngTelephoneUtils",
    "HandleNotifyGeneric",
    "FUNCTIONALITY",
    SetupL, UT_HandleNotifyGenericL, Teardown)

    
EUNIT_END_TEST_TABLE

//  END OF FILE


