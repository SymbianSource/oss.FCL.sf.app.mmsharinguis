/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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


//  CLASS HEADER
#include "UT_CMusAvaSettingsImp.h"
#include "musavasettingsobserverimp.h"
#include "muslogger.h"
#include "musavacapabilitytesthelper.h" //remove compilation warning

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <badesca.h>

//  INTERNAL INCLUDES
#include "musavasettingsimp.h"

_LIT(KContactName,"Matti Meikalainen");

// CONSTRUCTION
UT_CMusAvaSettingsImp* UT_CMusAvaSettingsImp::NewL()
    {
    UT_CMusAvaSettingsImp* self = UT_CMusAvaSettingsImp::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaSettingsImp* UT_CMusAvaSettingsImp::NewLC()
    {
    UT_CMusAvaSettingsImp* self = new( ELeave ) UT_CMusAvaSettingsImp();
    CleanupStack::PushL( self );
	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaSettingsImp::~UT_CMusAvaSettingsImp()
    {
    }

// Default constructor
UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp()
    {
    }

// Second phase construct
void UT_CMusAvaSettingsImp::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMusAvaSettingsImp::SetupL()
    {
    iSettingsImp = CMusAvaSettingsImp::NewL();
    } 

void UT_CMusAvaSettingsImp::Teardown()
    {
    delete iSettingsImp;
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_NewLL()
    {
    EUNIT_ASSERT( iSettingsImp );
    }
    
void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_DestructorL()
    {
    if( iSettingsImp->iSipAddresses )
        {
        iSettingsImp->iSipAddresses->Reset();
        delete iSettingsImp->iSipAddresses;
		iSettingsImp->iSipAddresses = NULL ;
        }
    
	if( iSettingsImp->iVideoCodecs )
        {
        iSettingsImp->iVideoCodecs->Reset();
        delete iSettingsImp->iVideoCodecs;
        iSettingsImp->iVideoCodecs = NULL ;
        }	
   
    if( iSettingsImp->iAudioCodecs )
        {
        iSettingsImp->iAudioCodecs->Reset();
        delete iSettingsImp->iAudioCodecs;
        iSettingsImp->iAudioCodecs = NULL ;
        }
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_TelNumberL()
    {
    EUNIT_ASSERT( iSettingsImp->TelNumber().Length() == 0 );    
    }   	

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_ContactNameL()
    {
    _LIT(KContactName,"Matti Meikalainen");
    iSettingsImp->SetContactNameL( KContactName );
    EUNIT_ASSERT( iSettingsImp->iContactName->Compare( KContactName ) == 0 ); 
    EUNIT_ASSERT( iSettingsImp->ContactName().Compare( KContactName ) == 0 );        
    }
    
void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SipAddressProposalL()
    {
    EUNIT_ASSERT( iSettingsImp->SipAddressProposal().Length() == 0 );
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SipAddressesL()
    {
    EUNIT_ASSERT( iSettingsImp->SipAddresses().MdcaCount() == 0 );    
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_ContactIdL()
    {
    EUNIT_ASSERT( iSettingsImp->ContactId() == KErrNotFound );    
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_VideoCodecsL()
    {
    EUNIT_ASSERT( iSettingsImp->VideoCodecs().MdcaCount() == 0 );    
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_AudioCodecsL()
    {
    EUNIT_ASSERT( iSettingsImp->AudioCodecs().MdcaCount() == 0 );      
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SipProfileIdL()
    {
    EUNIT_ASSERT( iSettingsImp->SipProfileId() == KErrNotFound ); 
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SetObserverL()
    {
    CMusAvaSettingsObserverImp* observer = CMusAvaSettingsObserverImp::NewL();
    CleanupStack::PushL( observer );
    iSettingsImp->SetObserver( *observer );
    EUNIT_ASSERT( iSettingsImp->iObserver == observer);
    CleanupStack::PopAndDestroy( observer );
    }
 
void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_ManualActivationL()
    {
    EUNIT_ASSERT(iSettingsImp->iManualActivation ==  MMusAvaSettings::EActivationNotExecuted );
    iSettingsImp->SetManualActivation( MMusAvaSettings::EActivationAllowed );
    EUNIT_ASSERT(iSettingsImp->ManualActivation() ==  MMusAvaSettings::EActivationAllowed );
    }
    
void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_ApplicationStateL()
    {
    EUNIT_ASSERT( iSettingsImp->ApplicationState() == 
    MMusAvaSettingsObserver::EApplicationStateNotDefined ); 
    
    CMusAvaSettingsObserverImp* observer = CMusAvaSettingsObserverImp::NewL();
    CleanupStack::PushL( observer );
    iSettingsImp->SetObserver( *observer );
    EUNIT_ASSERT( iSettingsImp->ApplicationState() == 
    MMusAvaSettingsObserver::EApplicationIsRunning ); 
    EUNIT_ASSERT( iSettingsImp->iObserver == observer );
    CleanupStack::PopAndDestroy( observer );
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_CallDirectionL()
    {
    TInt callDirection = iSettingsImp->CallDirection();
    EUNIT_ASSERT( callDirection == 0 );
    callDirection = 1;
    iSettingsImp->SetCallDirection( callDirection );
    EUNIT_ASSERT( callDirection == iSettingsImp->CallDirection() );
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SetTelNumberLL()
    {
    _LIT( KNumber ,   "123456789" );
    iSettingsImp->SetTelNumberL( KNumber );
    EUNIT_ASSERT( iSettingsImp->iTelNumber->Compare( KNumber ) == 0 );    
    }


void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SetOptionSentNumber()
	{
    _LIT( KNumber ,   "123456789" );
    iSettingsImp->SetOptionSentNumber( KNumber );
    EUNIT_ASSERT( iSettingsImp->iOptionSentTelNumber->Compare( KNumber ) == 0 ); 
	}

const UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_OptionSentTelNumber()
	{
	EUNIT_ASSERT( iSettingsImp->OptionSentTelNumber().Length() == 0 );
	}

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_ReleaseOptionSentNumber()
	{
    _LIT( KNumber ,   "123456789" );
    iSettingsImp->SetOptionSentNumber( KNumber );
    EUNIT_ASSERT( iSettingsImp->iOptionSentTelNumber->Compare( KNumber ) == 0 );    
    iSettingsImp->ReleaseOptionSentNumber(); 
    EUNIT_ASSERT( iSettingsImp->iOptionSentTelNumber->Length() == 0 )
	}

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SetSipAddressProposalLL( )
    {
    _LIT( KNumber ,   "123456789" );
    _LIT( KDomain ,   "test.com" );
    _LIT( KSipAddress ,   "123456789@test.com" );
    iSettingsImp->SetSipAddressProposalL( KNumber, KDomain );
    EUNIT_ASSERT( iSettingsImp->iSipAddressProposal->Des().Compare( KSipAddress() ) == KErrNone );     
    }
    
void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_ReleseTelNumberL()
    {
    _LIT( KNumber ,   "123456789" );
    iSettingsImp->SetTelNumberL( KNumber );
    EUNIT_ASSERT( iSettingsImp->iTelNumber->Compare( KNumber ) == 0 );    
    iSettingsImp->ReleseTelNumberL(); 
    EUNIT_ASSERT( iSettingsImp->iTelNumber->Length() == 0 )
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SetSipAddressesLL()
    {
    _LIT16( KSipAddress ,   "test@123456789" );
    _LIT16( KSipAddress1 ,   "test1@123456789" );
    TBufC16<16> str(KSipAddress);
    TBufC16<16> str1(KSipAddress);
    
    HBufC16* ptr;
    ptr = str.AllocLC(); 

    CDesCArrayFlat* sipAddress = new( ELeave ) CDesCArrayFlat( 1 );
    CleanupStack::PushL( sipAddress );
    
    sipAddress->AppendL( *ptr );
   
    iSettingsImp->SetSipAddressesL( *sipAddress );
    
    EUNIT_ASSERT( iSettingsImp->iSipAddresses->MdcaCount() > KErrNone );
    EUNIT_ASSERT( iSettingsImp->iSipAddresses->MdcaPoint( 0 ).Compare( *ptr ) == 0);
    sipAddress->Reset();
    CleanupStack::PopAndDestroy( sipAddress );
    CleanupStack::PopAndDestroy();
    
    // set sipaddresses again
    HBufC16* ptr1;
    ptr1 = str1.AllocLC(); 
    sipAddress = new( ELeave ) CDesCArrayFlat( 1 );
    CleanupStack::PushL( sipAddress );
    
    sipAddress->AppendL( *ptr1 );
   
    iSettingsImp->SetSipAddressesL( sipAddress );
    
    EUNIT_ASSERT( iSettingsImp->iSipAddresses->MdcaCount() == 1 );
    EUNIT_ASSERT( iSettingsImp->iSipAddresses->MdcaPoint( 0 ).Compare( *ptr1 ) == 0);
    CleanupStack::Pop( sipAddress );
    CleanupStack::PopAndDestroy();
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SetContactIdL()
    {
    iSettingsImp->SetContactId( 1 );
    EUNIT_ASSERT( iSettingsImp->iContactId == 1 );    
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SetVideoCodecsLL()
    {
     _LIT16( KSipAddress ,   "amr" );
    TBufC16<16> str(KSipAddress);
    HBufC16* ptr;
    ptr = str.AllocLC(); 
   
    CDesCArrayFlat* videoCodecs = new( ELeave ) CDesCArrayFlat( 1 );
    CleanupStack::PushL( videoCodecs );
    
    videoCodecs->AppendL( *ptr );
    iSettingsImp->SetVideoCodecsL( *videoCodecs );
    
    EUNIT_ASSERT( iSettingsImp->iVideoCodecs->MdcaCount() > KErrNone ); 
    EUNIT_ASSERT( iSettingsImp->iVideoCodecs->MdcaPoint( 0 ).Compare( *ptr ) == 0);
    videoCodecs->Reset();
    CleanupStack::PopAndDestroy( videoCodecs );
    CleanupStack::PopAndDestroy();    
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SetAudioCodecsLL()
    {
     _LIT16( KSipAddress ,   "H.263" );
    TBufC16<16> str(KSipAddress);
    HBufC16* ptr;
    ptr = str.AllocLC(); 
   
    CDesCArrayFlat* audioCodecs = new( ELeave ) CDesCArrayFlat( 1 );
    CleanupStack::PushL( audioCodecs );
    
    audioCodecs->AppendL( *ptr );
    iSettingsImp->SetAudioCodecsL( *audioCodecs );
    audioCodecs->Reset();
    CleanupStack::PopAndDestroy( audioCodecs );
    
    EUNIT_ASSERT( iSettingsImp->iAudioCodecs->MdcaCount() > KErrNone );      
    EUNIT_ASSERT( iSettingsImp->iAudioCodecs->MdcaPoint( 0 ).Compare( *ptr ) == 0);
    EUNIT_ASSERT( iSettingsImp->iAudioCodecs->MdcaPoint( 0 ).Compare( KSipAddress ) == 0);
    
    CleanupStack::PopAndDestroy();
    }

void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_SetSipProfileIdL()
    {
    iSettingsImp->SetSipProfileId(1);
    EUNIT_ASSERT( iSettingsImp->iSipProfileId == 1 );
    }
     
void UT_CMusAvaSettingsImp::UT_CMusAvaSettingsImp_CopyDescArrayLL()
    {
    CDesCArrayFlat* array = new( ELeave ) CDesCArrayFlat( 2 );
    CleanupStack::PushL( array );
    CDesCArrayFlat* target = new( ELeave ) CDesCArrayFlat( 2 );
    CleanupStack::PushL( target );
    
    _LIT16( KDummy1 ,   "Dummy1" );
    TBufC16<16> str1(KDummy1);
    HBufC16* ptr1;
    ptr1 = str1.AllocLC();
    
    _LIT16( KDummy2 ,   "Dummy1" );
    TBufC16<16> str2(KDummy2);
    HBufC16* ptr2;
    ptr2 = str2.AllocLC();
    
    array->AppendL( *ptr1 );
    array->AppendL( *ptr2 );
    
    
    iSettingsImp->CopyDescArrayL( *target, *array );
    EUNIT_ASSERT( target->MdcaCount() == 2 );
    EUNIT_ASSERT( target->MdcaPoint( 0 ).Compare( *ptr1 ) == 0);
    EUNIT_ASSERT( target->MdcaPoint( 1 ).Compare( *ptr2 ) == 0);
    
    CleanupStack::PopAndDestroy( 2 );
    CleanupStack::PopAndDestroy( target );
    CleanupStack::PopAndDestroy( array );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaSettingsImp,
    "CMusAvaSettingsImp",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaSettingsImp",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_NewLL, Teardown)
    
EUNIT_TEST(
    "~CMusAvaSettingsImp - test ",
    "CMusAvaSettingsImp",
    "~CMusAvaSettingsImp",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_DestructorL, Teardown)
    
EUNIT_TEST(
    "TelNumber - test ",
    "CMusAvaSettingsImp",
    "TelNumber",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_TelNumberL, Teardown)
    
EUNIT_TEST(
    "ContactName - test ",
    "CMusAvaSettingsImp",
    "ContactName",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_ContactNameL, Teardown)
    
 EUNIT_TEST(
    "SipAddressProposal - test ",
    "CMusAvaSettingsImp",
    "SipAddressProposal",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SipAddressProposalL, Teardown)    
    
 EUNIT_TEST(
    "SipAddresses - test ",
    "CMusAvaSettingsImp",
    "SipAddresses",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SipAddressesL, Teardown)
    
  EUNIT_TEST(
    "ContactId - test ",
    "CMusAvaSettingsImp",
    "ContactId",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_ContactIdL, Teardown)
    
  EUNIT_TEST(
    "VideoCodecs - test ",
    "CMusAvaSettingsImp",
    "VideoCodecs",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_VideoCodecsL, Teardown)

EUNIT_TEST(
    "AudioCodecs - test ",
    "CMusAvaSettingsImp",
    "AudioCodecs",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_AudioCodecsL, Teardown)
    
 EUNIT_TEST(
    "SipProfileId - test ",
    "CMusAvaSettingsImp",
    "SipProfileId",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SipProfileIdL, Teardown)
    
EUNIT_TEST(
    "SetObserver - test ",
    "CMusAvaSettingsImp",
    "SetObserver",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SetObserverL, Teardown)
    
EUNIT_TEST(
    "ManualActivation - test ",
    "CMusAvaSettingsImp",
    "ManualActivation",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_ManualActivationL, Teardown)
    
EUNIT_TEST(
    "ApplicationState - test ",
    "CMusAvaSettingsImp",
    "ApplicationState",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_ApplicationStateL, Teardown)
 
EUNIT_TEST(
    "CallDirection - test ",
    "CMusAvaSettingsImp",
    "CallDirection",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_CallDirectionL, Teardown)
 
 EUNIT_TEST(
    "SetTelNumberL - test ",
    "CMusAvaSettingsImp",
    "SetTelNumberL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SetTelNumberLL, Teardown)
    
 EUNIT_TEST(
    "SetSipAddressProposalL - test ",
    "CMusAvaSettingsImp",
    "SetSipAddressProposalL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SetSipAddressProposalLL, Teardown)
    
 EUNIT_TEST(
    "ReleseTelNumberL - test ",
    "CMusAvaSettingsImp",
    "ReleseTelNumberL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_ReleseTelNumberL, Teardown)
    
 EUNIT_TEST(
    "SetTelNumberL - test ",
    "CMusAvaSettingsImp",
    "SetTelNumberL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SetTelNumberLL, Teardown)
    
 EUNIT_TEST(
    "SetSipAddressesL - test ",
    "CMusAvaSettingsImp",
    "SetSipAddressesL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SetSipAddressesLL, Teardown)
    
 EUNIT_TEST(
    "SetContactId - test ",
    "CMusAvaSettingsImp",
    "SetContactId",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SetContactIdL, Teardown)
    
 EUNIT_TEST(
    "SetVideoCodecsL - test ",
    "CMusAvaSettingsImp",
    "SetVideoCodecsL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SetVideoCodecsLL, Teardown)
    
 EUNIT_TEST(
    "SetAudioCodecsL - test ",
    "CMusAvaSettingsImp",
    "SetAudioCodecsL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SetAudioCodecsLL, Teardown)      
    
EUNIT_TEST(
    "SetSipProfileId - test ",
    "CMusAvaSettingsImp",
    "SetSipProfileId",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SetSipProfileIdL, Teardown)
    
EUNIT_TEST(
    "Sip - test ",
    "CMusAvaSettingsImp",
    "Sip",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_CopyDescArrayLL, Teardown)    
    
EUNIT_TEST(
    "SetOptionSentNumber - test ",
    "CMusAvaSettingsImp",
    "SetOptionSentNumber",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_SetOptionSentNumber, Teardown) 
    
EUNIT_TEST(
    "OptionSentTelNumber - test ",
    "CMusAvaSettingsImp",
    "OptionSentTelNumber",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_OptionSentTelNumber, Teardown) 
    
EUNIT_TEST(
    "ReleaseOptionSentNumber - test ",
    "CMusAvaSettingsImp",
    "ReleaseOptionSentNumber",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingsImp_ReleaseOptionSentNumber, Teardown) 
    

EUNIT_END_TEST_TABLE

//  END OF FILE
