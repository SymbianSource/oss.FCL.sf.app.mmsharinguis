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
#include "sipprofile.h"
#include "UT_CMusAvaRegisterAvailability.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "musavaobserver.h"
#include "musavaobserverimp.h"
#include "musavadefaultimp.h"
#include "musavaobserverimp.h"
#include "musavasettingsimp.h"
#include "mussettingskeys.h"
#include "mussettings.h"
#include "CSipSseTestTls.h"
#include "musavasharedobject.h"
#include "sipprofile.h"
#include "musavasip.h"
#include "sipprofileregistry.h"
#include "sipprofileregistryobserver.h"
#include "musavasipprofileregistryobserver.h"
#include "musavacapabilitytesthelper.h" //remove compilation warning

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
//#include <telephonyinternalpskeys.h>
#include <sipprofileregistry.h>
#include <sipprofileregistryobserver.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <digia/eunit/eunitdecorators.h>

//  INTERNAL INCLUDES
#include "musavaregisteravailability.h"

//const TInt KNumberMatchingDigitsRequired( 7 );

//_LIT(KNumberZero,"");
//_LIT(KNumber,"05012345678");
//_LIT(KPlusNumber,"+3585012345678");

// CONSTRUCTION
UT_CMusAvaRegisterAvailability* UT_CMusAvaRegisterAvailability::NewL()
    {
    UT_CMusAvaRegisterAvailability* self = UT_CMusAvaRegisterAvailability::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaRegisterAvailability* UT_CMusAvaRegisterAvailability::NewLC()
    {
    UT_CMusAvaRegisterAvailability* self = new( ELeave ) UT_CMusAvaRegisterAvailability();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaRegisterAvailability::~UT_CMusAvaRegisterAvailability()
    {
    }

// Default constructor
UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability()
    {
    }

// Second phase construct
void UT_CMusAvaRegisterAvailability::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaRegisterAvailability::SetupL(  )
    {
    SIPStrings::OpenL();
    CSipSseTestTls::OpenL();
    iProfileId = MultimediaSharingSettings::SipProfileSettingL();
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iRegisterAvailability  = CMusAvaRegisterAvailability::NewL( *iAvailabilityObserver,
                                                              *iConcreteSettings );
    iSharedObj = CMusAvaSharedObject::GetSingletonL();
    } 

void UT_CMusAvaRegisterAvailability::Teardown(  )
    {
    delete iAvailabilityObserver;
    iAvailabilityObserver = NULL;
    delete iConcreteSettings;
    iConcreteSettings = NULL;
    delete iRegisterAvailability;
    iRegisterAvailability = NULL;
    
    CSipSseTestTls::Close();
    CMusAvaSharedObject::DeleteSingleton();
    SIPStrings::Close();
    }
    
void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_NewLL()
    {
    EUNIT_ASSERT( iRegisterAvailability );
    }
    	
void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_DoExecuteLL()
    {
    MultimediaSharingSettings::SetSipProfileSettingL( 5 );
    
    TRAPD( error, iRegisterAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count == 0 )
        {
        EUNIT_ASSERT( iAvailabilityObserver->iStatus == 
            MMusAvaObserver::EMusAvaStatusAvailable );    
        }
    else
        {
        EUNIT_ASSERT( iAvailabilityObserver->iStatus == 
            MMusAvaObserver::EMusAvaStatusNoSipProfile);
        }
    
    EUNIT_ASSERT( iAvailabilityObserver->iName == MMusAvaObserver::EMusAvaNameRegistration );
    }  
void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_NameL()
    {
    EUNIT_ASSERT( iRegisterAvailability->Name() == MMusAvaObserver::EMusAvaNameRegistration );
    } 
void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_StatusL()
    {
     //No profile created
    EUNIT_ASSERT( iRegisterAvailability->State() ==  MMusAvaObserver::EMusAvaStatusNotExecuted );
    
    MultimediaSharingSettings::SetSipProfileSettingL( 5 );
 
    iRegisterAvailability->DoExecuteL();

    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count == 0 )
        {
        EUNIT_ASSERT( iAvailabilityObserver->iStatus == 
            MMusAvaObserver::EMusAvaStatusAvailable );    
        }
    else
        {
        EUNIT_ASSERT( iAvailabilityObserver->iStatus == 
            MMusAvaObserver::EMusAvaStatusNoSipProfile);
        }

    } 
void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_Stop()
    {
    //Profile is not created
    iRegisterAvailability->Stop();
    EUNIT_ASSERT( iRegisterAvailability->State() ==  MMusAvaObserver::EMusAvaStatusNotExecuted );
    
    // Test with profile
    // Create Profile
    TRAPD ( error, iSharedObj->MusAvaSip().CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    
    MultimediaSharingSettings::SetSipProfileSettingL( 1 ); 
    //Profile created
    iRegisterAvailability->Stop();
    CSIPProfileRegistry& registry = iSharedObj->MusAvaSip().ProfileRegistryL();
    EUNIT_ASSERT( registry.iProfileDisabled == ETrue);
    } 
void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_ProfileRegistryEventOccurredL()
    {
    iRegisterAvailability->ProfileRegistryEventOccurred( 1, MSIPProfileRegistryObserver::EProfileCreated );
    
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    EUNIT_ASSERT( iSharedObj->MusAvaSip().CreateProfileL() );
    iRegisterAvailability->ProfileRegistryEventOccurred( 1, MSIPProfileRegistryObserver::EProfileRegistered );        
    
    TRAPD( error, iRegisterAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
      
    iRegisterAvailability->ProfileRegistryEventOccurred( 1, MSIPProfileRegistryObserver::EProfileCreated );
    iRegisterAvailability->ProfileRegistryEventOccurred( 1, MSIPProfileRegistryObserver::EProfileUpdated );
    iRegisterAvailability->ProfileRegistryEventOccurred( 1, MSIPProfileRegistryObserver::EProfileRegistered );        
    
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==  MMusAvaObserver::EMusAvaStatusAvailable );                
    EUNIT_ASSERT( iAvailabilityObserver->iName == MMusAvaObserver::EMusAvaNameRegistration );
            
    iRegisterAvailability->ProfileRegistryEventOccurred( 1, MSIPProfileRegistryObserver::EProfileDeregistered );   
     
    EUNIT_ASSERT( iAvailabilityObserver->iStatus ==  MMusAvaObserver::EMusAvaStatusNotRegistered );                
    EUNIT_ASSERT( iAvailabilityObserver->iName == MMusAvaObserver::EMusAvaNameRegistration );
             
    iRegisterAvailability->ProfileRegistryEventOccurred( 1, MSIPProfileRegistryObserver::EProfileDestroyed );   
    EUNIT_ASSERT( iRegisterAvailability->State() ==  MMusAvaObserver::EMusAvaStatusNotRegistered ); 
    } 
    
void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_CreateSipAddressProposalLL()
    {
    //Profile is not created
    iRegisterAvailability->Stop();
    EUNIT_ASSERT( iRegisterAvailability->State() ==  MMusAvaObserver::EMusAvaStatusNotExecuted );
    
    // Test with profile
    // Create Profile
    TRAPD( error, iSharedObj->MusAvaSip().CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    iRegisterAvailability->CreateSipAddressProposalL();
    
    }

void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_ConnectionStateChangedL()
    {
    iRegisterAvailability->SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
    
    iRegisterAvailability->SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
    iRegisterAvailability->ConnectionStateChanged( CSIPConnection::EInit );
    EUNIT_ASSERT( iRegisterAvailability->State() ==  MMusAvaObserver::EMusAvaStatusNotExecuted );
    
    iRegisterAvailability->SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
    iRegisterAvailability->ConnectionStateChanged( CSIPConnection::EActive );
    EUNIT_ASSERT( iRegisterAvailability->State() ==  MMusAvaObserver::EMusAvaStatusNotExecuted );

    iRegisterAvailability->SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
    iRegisterAvailability->ConnectionStateChanged( CSIPConnection::ESuspended );
    EUNIT_ASSERT( iRegisterAvailability->State() ==  MMusAvaObserver::EMusAvaConnectionErr );
    
    iRegisterAvailability->SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
   	iRegisterAvailability->ConnectionStateChanged( CSIPConnection::EInactive );
 	EUNIT_ASSERT( iRegisterAvailability->State() ==  MMusAvaObserver::EMusAvaConnectionErr );
    
    iRegisterAvailability->SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
    iRegisterAvailability->ConnectionStateChanged( CSIPConnection::EUnavailable );
    EUNIT_ASSERT( iRegisterAvailability->State() ==  MMusAvaObserver::EMusAvaConnectionErr );
    
    iRegisterAvailability->SetState( MMusAvaObserver::EMusAvaStatusNotExecuted );
    iRegisterAvailability->ConnectionStateChanged( (CSIPConnection::TState)6 );
    EUNIT_ASSERT( iRegisterAvailability->State() ==  MMusAvaObserver::EMusAvaStatusNotExecuted );
    }

void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_ProfileRegistryErrorOccurredL()
    {
    iRegisterAvailability->ProfileRegistryErrorOccurred(5, -5 );

    // Test Profile
    TRAPD( error, iSharedObj->MusAvaSip().CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    //Profile created
    iRegisterAvailability->ProfileRegistryErrorOccurred(5, -5 );
    
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    
    iRegisterAvailability->ProfileRegistryErrorOccurred(1, -5 );
    EUNIT_ASSERT( iRegisterAvailability->State() ==  MMusAvaObserver::EMusAvaStatusNotRegistered );
    } 


void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_RegisterLL()
    {
    MultimediaSharingSettings::SetSipProfileSettingL( 5 );
    TRAPD( error, iRegisterAvailability->RegisterL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count == 0 )
        {
        EUNIT_ASSERT( iSharedObj->MusAvaSip().iSipConnection != NULL );
        EUNIT_ASSERT( iAvailabilityObserver->iStatus == 
            MMusAvaObserver::EMusAvaStatusAvailable );    
        }
    else
        {
        EUNIT_ASSERT( iAvailabilityObserver->iStatus == 
            MMusAvaObserver::EMusAvaStatusNoSipProfile);
        }
    }

void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_InitializeRegisterL()
    {  
    MultimediaSharingSettings::SetSipProfileSettingL( 5 );
    EUNIT_ASSERT( iRegisterAvailability->InitializeRegister() );   
    }

void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_EnableRegisterLL()
    {
    MultimediaSharingSettings::SetSipProfileSettingL( 1 ); 
    iSharedObj->MusAvaSip().CreateProfileL();
    iRegisterAvailability->EnableRegisterL();
    EUNIT_ASSERT( iSharedObj->MusAvaSip().ProfileRegistryL().IsEnabled(
    	*iSharedObj->MusAvaSip().Profile()  ));
    
    }

void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_UpdateSIPProfileL()
    {
    delete iSharedObj->MusAvaSip().iSipProfileRegistry;
    iSharedObj->MusAvaSip().iSipProfileRegistry = NULL;
    delete iSharedObj->MusAvaSip().iSipProfile;
    iSharedObj->MusAvaSip().iSipProfile = NULL;
    
    // Profile Id found
    iSharedObj->MusAvaSip().iSipProfileRegistry = CSIPProfileRegistry::NewL(
                                    iSharedObj->MusAvaSip().Sip(),
                                    *iSharedObj->MusAvaSip().iSipProfileRegistryObserver );
   	iRegisterAvailability->iSharedObj->MusAvaSip().iSipProfile = CSIPProfile::NewL( iSharedObj->MusAvaSip().iSipProfileRegistry );
   	iRegisterAvailability->iSharedObj->MusAvaSip().Profile()->iTUint32ValueError = KErrNone;
    EUNIT_ASSERT( iRegisterAvailability->UpdateSIPProfile() );
    // Profile Id found
    iRegisterAvailability->iSharedObj->MusAvaSip().Profile()->iTUint32Value = 1;
    EUNIT_ASSERT( iRegisterAvailability->UpdateSIPProfile() );
    // Profile not Id found
    iRegisterAvailability->iSharedObj->MusAvaSip().Profile()->iTUint32Value = 0;
    iRegisterAvailability->iSharedObj->MusAvaSip().Profile()->iTUint32ValueError = KErrNotFound;
    EUNIT_ASSERT( !iRegisterAvailability->UpdateSIPProfile() );
    delete iRegisterAvailability->iSharedObj->MusAvaSip().iSipProfile;
    iRegisterAvailability->iSharedObj->MusAvaSip().iSipProfile = NULL;
    }
    
void UT_CMusAvaRegisterAvailability::UT_CMusAvaRegisterAvailability_RegistrationStatusL()
    {
    delete iSharedObj->MusAvaSip().iSipProfileRegistry;
    iSharedObj->MusAvaSip().iSipProfileRegistry = NULL;
    delete iSharedObj->MusAvaSip().iSipProfile;
    iSharedObj->MusAvaSip().iSipProfile = NULL;
    iSharedObj->MusAvaSip().iSipProfileRegistry = CSIPProfileRegistry::NewL(
                                    iSharedObj->MusAvaSip().Sip(),
                                    *iSharedObj->MusAvaSip().iSipProfileRegistryObserver );
   	iRegisterAvailability->iSharedObj->MusAvaSip().iSipProfile = CSIPProfile::NewL( iSharedObj->MusAvaSip().iSipProfileRegistry );
   	//Not registred
   	iRegisterAvailability->iSharedObj->MusAvaSip().Profile()->iTBoolValueError = KErrNone;
   	iRegisterAvailability->iSharedObj->MusAvaSip().Profile()->iTBoolValue = EFalse;
   	EUNIT_ASSERT( !iRegisterAvailability->RegistrationStatusL() );
   	//Register error	       
	iRegisterAvailability->iSharedObj->MusAvaSip().Profile()->iTBoolValueError = KErrNotFound;
	EUNIT_ASSERT( !iRegisterAvailability->RegistrationStatusL() );
	// Registred
	iRegisterAvailability->iSharedObj->MusAvaSip().Profile()->iTBoolValueError = KErrNone;
	iRegisterAvailability->iSharedObj->MusAvaSip().Profile()->iTBoolValue = ETrue;
	EUNIT_ASSERT( iRegisterAvailability->RegistrationStatusL() );
	delete iRegisterAvailability->iSharedObj->MusAvaSip().iSipProfile;
	iRegisterAvailability->iSharedObj->MusAvaSip().iSipProfile = NULL;
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaRegisterAvailability,
    "CMusAvaRegisterAvailability",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaRegisterAvailability",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_NewLL, Teardown)

EUNIT_TEST(
    "DoExecuteL - test ",
    "CMusAvaRegisterAvailability",
    "DoExecuteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_DoExecuteLL, Teardown) 

EUNIT_TEST(
    "Name - test ",
    "CMusAvaRegisterAvailability",
    "Name",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_NameL, Teardown) 

   
EUNIT_TEST(
    "Status - test ",
    "CMusAvaRegisterAvailability",
    "Status",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_StatusL, Teardown)   
    
EUNIT_TEST(
    "StopPattern - test ",
    "CMusAvaRegisterAvailability",
    "StopPattern",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_Stop, Teardown)  
  
EUNIT_TEST(
    "ProfileRegistryEventOccurred - test ",
    "CMusAvaRegisterAvailability",
    "ProfileRegistryEventOccurred",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_ProfileRegistryEventOccurredL, Teardown)  
    
EUNIT_TEST(
    "CreateSipAddressProposalL - test ",
    "CMusAvaRegisterAvailability",
    "CreateSipAddressProposalL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_CreateSipAddressProposalLL, Teardown) 

EUNIT_TEST(
    "ConnectionStateChanged - test ",
    "CMusAvaRegisterAvailability",
    "ConnectionStateChanged",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_ConnectionStateChangedL, Teardown) 

EUNIT_TEST(
    "ErrorOccurred - test ",
    "CMusAvaRegisterAvailability",
    "ErrorOccurred",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_ProfileRegistryErrorOccurredL, Teardown) 

EUNIT_TEST(
    "RegisterL - test ",
    "CMusAvaRegisterAvailability",
    "RegisterL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_RegisterLL, Teardown) 

EUNIT_TEST(
    "InitializeRegisterL - test ",
    "CMusAvaRegisterAvailability",
    "InitializeRegisterL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_InitializeRegisterL, Teardown) 

EUNIT_TEST(
    "EnableRegisterL - test ",
    "CMusAvaRegisterAvailability",
    "EnableRegisterL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_EnableRegisterLL, Teardown) 
    
EUNIT_TEST(
    "UpdateSIPProfile - test ",
    "CMusAvaRegisterAvailability",
    "UpdateSIPProfile",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_UpdateSIPProfileL, Teardown)   
      
    
EUNIT_TEST(
    "RegistrationStatus - test ",
    "CMusAvaRegisterAvailability",
    "RegistrationStatus",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaRegisterAvailability_RegistrationStatusL, Teardown)   


EUNIT_END_TEST_TABLE

//  END OF FILE
