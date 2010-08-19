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
#include "UT_CMusAvaNetworkAvailability.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <sdpmediafield.h>
#include <e32property.h>


//  INTERNAL INCLUDES
#include "musavanetworkavailability.h"
#include "musavaavailabilityobserverimp.h"
#include "musavasettingsimp.h"
#include "mussesseioninformationapi.h"

_LIT( KTelNumber, "12345" );
_LIT( KAnotherTelNumber, "54321" );

// CONSTRUCTION
UT_CMusAvaNetworkAvailability* UT_CMusAvaNetworkAvailability::NewL()
    {
    UT_CMusAvaNetworkAvailability* self = UT_CMusAvaNetworkAvailability::NewLC();
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaNetworkAvailability* UT_CMusAvaNetworkAvailability::NewLC()
    {
    UT_CMusAvaNetworkAvailability* self = new( ELeave ) UT_CMusAvaNetworkAvailability();
    CleanupStack::PushL( self );

    self->ConstructL();

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaNetworkAvailability::~UT_CMusAvaNetworkAvailability()
    {
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
	                    NMusSessionInformationApi::KMusCallEvent,
    	                0 ));
    }

// Default constructor
UT_CMusAvaNetworkAvailability::UT_CMusAvaNetworkAvailability()
    {
    }

// Second phase construct
void UT_CMusAvaNetworkAvailability::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    RProperty::Define( NMusSessionInformationApi::KCategoryUid,
                       NMusSessionInformationApi::KMusCallEvent,
                       RProperty::EInt,
                       0 );

    RProperty::Define( NMusSessionInformationApi::KCategoryUid,
                       NMusSessionInformationApi::KMusTelNumber,
                       RProperty::EText,
                       0 );
                       
    RProperty::Define( NMusSessionInformationApi::KCategoryUid,
                       NMusSessionInformationApi::KMusCallDirection,
                       RProperty::EInt,
                       0 );
    
	
    RProperty::Define( NMusSessionInformationApi::KCategoryUid,
                       NMusSessionInformationApi::KMUSPrivacy,
                       RProperty::EInt,
                       0 );
                       
    }

//  METHODS



void UT_CMusAvaNetworkAvailability::SetupL()
    {
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iSettings = CMusAvaSettingsImp::NewL();
    iNetworkAvailability = CMusAvaNetworkAvailability::NewL( 
                                                    *iAvailabilityObserver, 
                                                    *iSettings );   
    }

void UT_CMusAvaNetworkAvailability::Teardown()
    {
    delete iNetworkAvailability;
    delete iSettings;
    delete iAvailabilityObserver;
    PropertyHelper::Close();
    }
    
void UT_CMusAvaNetworkAvailability::UT_CMusAvaNetworkAvailability_DoExecuteLL()
    {
    // no call

    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                    	NMusSessionInformationApi::KMusCallEvent,
                    	0 ));

    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMUSPrivacy,
                        0 ));
    
    iNetworkAvailability->DoExecuteL();    
    EUNIT_ASSERT_EQUALS( iNetworkAvailability->State(), 
                         MMusAvaObserver::EMusAvaStatusNoCall );
              
    // one call, tel number given
    
    User::LeaveIfError(	RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                    	NMusSessionInformationApi::KMusCallEvent,
                    	1 ));
                    
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMusTelNumber,
                    KTelNumber ));
                
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                NMusSessionInformationApi::KMusCallDirection,
                0 ));
                                                                         
    iNetworkAvailability->DoExecuteL();
    EUNIT_ASSERT_EQUALS( iNetworkAvailability->State(), 
                         MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT_EQUALS( iSettings->TelNumber(), KTelNumber );
    EUNIT_ASSERT_EQUALS( iSettings->CallDirection(), 0 );
    
    }
    
void UT_CMusAvaNetworkAvailability::UT_CMusAvaNetworkAvailability_StopL()
    {
    iNetworkAvailability->Stop();
    EUNIT_ASSERT_EQUALS( iNetworkAvailability->State(), 
                         MMusAvaObserver::EMusAvaStatusNotExecuted );
    }
    
void UT_CMusAvaNetworkAvailability::UT_CMusAvaNetworkAvailability_NameL()
    {
    EUNIT_ASSERT_EQUALS( iNetworkAvailability->Name(),
                         MMusAvaObserver::EMusAvaNameNetworkStatus );
    }    
    
void UT_CMusAvaNetworkAvailability::UT_CMusAvaNetworkAvailability_CallConnectedLL()
    {
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                   		NMusSessionInformationApi::KMusCallDirection,
                    	0 ));

    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMUSPrivacy,
                        0 ));
    
    iNetworkAvailability->iConfcall = ETrue;
    iNetworkAvailability->iSettings.SetOptionSentNumber( KTelNumber );
    iNetworkAvailability->CallConnectedL( KTelNumber );
    EUNIT_ASSERT_EQUALS( iNetworkAvailability->State(), 
                         MMusAvaObserver::EMusAvaStatusAvailable );
    
    
    iNetworkAvailability->iSettings.SetOptionSentNumber( KAnotherTelNumber );
    iNetworkAvailability->CallConnectedL( KTelNumber );   
    EUNIT_ASSERT_EQUALS( iNetworkAvailability->State(), 
                         MMusAvaObserver::EMusAvaStatusInProgress );
    EUNIT_ASSERT( iNetworkAvailability->iSettings.OptionSentTelNumber().Length() == 0 );
    
    
    EUNIT_ASSERT_EQUALS( iSettings->TelNumber(), KTelNumber );    
    }
    
void UT_CMusAvaNetworkAvailability::UT_CMusAvaNetworkAvailability_ConferenceCallLL()
    {
    iNetworkAvailability->ConferenceCallL();
    EUNIT_ASSERT_EQUALS( iNetworkAvailability->State(), 
                         MMusAvaObserver::EMusAvaStatusConferenceCall );
    }
    
void UT_CMusAvaNetworkAvailability::UT_CMusAvaNetworkAvailability_CallHoldLL()
    {
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                   		NMusSessionInformationApi::KMusCallDirection,
                    	0 ));
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                        NMusSessionInformationApi::KMUSPrivacy,
                        0 ));
    
    
    iNetworkAvailability->CallHoldL( KTelNumber );
    EUNIT_ASSERT_EQUALS( iNetworkAvailability->State(), 
                         MMusAvaObserver::EMusAvaStatusCallOnHold );
    EUNIT_ASSERT_EQUALS( iSettings->TelNumber(), KTelNumber );    
    }
    
void UT_CMusAvaNetworkAvailability::UT_CMusAvaNetworkAvailability_NoActiveCallLL()
    {
    iNetworkAvailability->NoActiveCallL();
    EUNIT_ASSERT_EQUALS( iNetworkAvailability->State(), 
                         MMusAvaObserver::EMusAvaStatusNoCall );
    EUNIT_ASSERT_EQUALS( iSettings->TelNumber(), KNullDesC );    
    }                



//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusAvaNetworkAvailability,
    "CMusAvaNetworkAvailability",
    "UNIT" )

EUNIT_TEST(
    "DoExecuteL - test ",
    "CMusAvaNetworkAvailability",
    "DoExecuteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkAvailability_DoExecuteLL, Teardown)

EUNIT_TEST(
    "Stop - test ",
    "CMusAvaNetworkAvailability",
    "Stop",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkAvailability_StopL, Teardown)
    
EUNIT_TEST(
    "Name - test ",
    "CMusAvaNetworkAvailability",
    "Name",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkAvailability_NameL, Teardown)    
    
EUNIT_TEST(
    "CallConnectedL - test ",
    "CMusAvaNetworkAvailability",
    "CallConnectedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkAvailability_CallConnectedLL, Teardown ) 
    
EUNIT_TEST(
    "ConferenceCallL - test ",
    "CMusAvaNetworkAvailability",
    "ConferenceCallL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkAvailability_ConferenceCallLL, Teardown )     
    
EUNIT_TEST(
    "CallHoldL - test ",
    "CMusAvaNetworkAvailability",
    "CallHoldL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkAvailability_CallHoldLL, Teardown )     
    
EUNIT_TEST(
    "NoActiveCallL - test ",
    "CMusAvaNetworkAvailability",
    "NoActiveCallL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaNetworkAvailability_NoActiveCallLL, Teardown )      
    

EUNIT_END_TEST_TABLE

//  END OF FILE
