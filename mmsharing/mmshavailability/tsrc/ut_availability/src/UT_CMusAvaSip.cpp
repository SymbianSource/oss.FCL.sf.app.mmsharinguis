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
#include "UT_CMusAvaSip.h"
#include "muslogger.h"
#include "mussettings.inl"
#include "CSipSseTestTls.h"
#include "mussettingskeys.h"
#include "musavasipdefaulthandler.h"
#include "sipconnectionobserver.h"
#include "musavasipdefaulthandler.h"
#include "musavasipconnectionadapter.h"
#include "musavasettingsimp.h"
#include "musavaregisteravailability.h"
#include "musavasipprofileregistryobserver.h"
#include "musavacapabilitytesthelper.h" //remove compilation warning
#include "CSipSseTestTls.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
//#include <telephonyinternalpskeys.h>
#include <sipprofileregistry.h>
#include <sip.h>
#include <siperr.h>
#include <digia/eunit/eunitdecorators.h>


//  INTERNAL INCLUDES
#include "musavasip.h"



// CONSTRUCTION
UT_CMusAvaSip* UT_CMusAvaSip::NewL()
    {
    UT_CMusAvaSip* self = UT_CMusAvaSip::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaSip* UT_CMusAvaSip::NewLC()
    {
    UT_CMusAvaSip* self = new( ELeave ) UT_CMusAvaSip();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaSip::~UT_CMusAvaSip()
    {
    }

// Default constructor
UT_CMusAvaSip::UT_CMusAvaSip()
    {
    }

// Second phase construct
void UT_CMusAvaSip::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaSip::SetupL(  )
    {
    SIPStrings::OpenL();
    CSipSseTestTls::OpenL();
    iProfileId = MultimediaSharingSettings::SipProfileSettingL();    
    iMusAvaSip = CMusAvaSip::NewL();	
    } 

void UT_CMusAvaSip::Teardown(  )
    {
    delete iMusAvaSip;
    iMusAvaSip = NULL;
    MultimediaSharingSettings::SetSipProfileSettingL( iProfileId );    
    CSipSseTestTls::Close();
    SIPStrings::Close();
    }

void UT_CMusAvaSip::UT_CMusAvaSip_NewLL(  )
    {
    EUNIT_ASSERT( iMusAvaSip->iSip );
    EUNIT_ASSERT( iMusAvaSip->iSipObserver );
    EUNIT_ASSERT( iMusAvaSip->iSipConnectionObserver );
    EUNIT_ASSERT( iMusAvaSip->iSipProfileRegistryObserver );
    }  

void UT_CMusAvaSip::UT_CMusAvaSip_AddAdapterLL()
    {
    CMusAvaSipDefaultHandler* defaultHandler 
                                = new( ELeave ) CMusAvaSipDefaultHandler();
    CleanupStack::PushL( defaultHandler );
    iMusAvaSip->AddAdapterL( *defaultHandler );
    EUNIT_ASSERT( iMusAvaSip->iSipConnectionObserver );
    CleanupStack::PopAndDestroy( defaultHandler ); 
    }
 		
void UT_CMusAvaSip::UT_CMusAvaSip_RemoveAdapterL()
    {
    CMusAvaSipDefaultHandler* defaultHandler 
                                = new( ELeave ) CMusAvaSipDefaultHandler();
    CleanupStack::PushL( defaultHandler );
    iMusAvaSip->AddAdapterL( *defaultHandler );
    EUNIT_ASSERT( iMusAvaSip->iSipConnectionObserver );
    iMusAvaSip->RemoveAdapter( *defaultHandler );
 //   EUNIT_ASSERT( !iMusAvaSip->iSipConnectionObserver->iObservers.Count() );
    CleanupStack::PopAndDestroy( defaultHandler ); 
    }
void UT_CMusAvaSip::UT_CMusAvaSip_CreateProfileL()
    {
    // Test Profile
    //No profile
    TInt ret = NULL;
    TRAPD( error, ret = iMusAvaSip->CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && ret == KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT ( ret );

    // Test Profile
    //Profile created
    delete iMusAvaSip;
    iMusAvaSip = NULL;
    iMusAvaSip = CMusAvaSip::NewL();
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    iMusAvaSip->iSipProfileRegistry = CSIPProfileRegistry::NewL(
                                    *iMusAvaSip->iSip,
                                    *iMusAvaSip->iSipProfileRegistryObserver );
    iMusAvaSip->iSipProfile = 
            iMusAvaSip->GetMusProfileL( *iMusAvaSip->iSipProfileRegistry );
    TRAP( error, ret = iMusAvaSip->CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( KErrNoMemory );
    EUNIT_ASSERT ( ret );

    delete iMusAvaSip;
    iMusAvaSip = NULL;
    iMusAvaSip = CMusAvaSip::NewL();
    //Test sip profile registry
    //No profile registry, registry creation fails
    CSipSseTestTls::Storage()->iRegistryBehavior = KRegistryLeaveAtConstruct;
    TRAP( error, ret = iMusAvaSip->CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( !ret );
    CSipSseTestTls::Storage()->Reset();
       
    //Create sip profile registry
    delete iMusAvaSip;
    iMusAvaSip = NULL;
    iMusAvaSip = CMusAvaSip::NewL();
    
    iMusAvaSip->iSipProfileRegistry = CSIPProfileRegistry::NewL(
                                    *iMusAvaSip->iSip,
                                    *iMusAvaSip->iSipProfileRegistryObserver );
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    TRAP( error, ret = iMusAvaSip->CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( ret );

    //Test sip profile registry
    //No profile registry, registry creation ok
    TRAP( error, ret = iMusAvaSip->CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( ret );
    //Create sip profile registry
    delete iMusAvaSip;
    iMusAvaSip = NULL;
    iMusAvaSip = CMusAvaSip::NewL();
    iMusAvaSip->iSipProfileRegistry = CSIPProfileRegistry::NewL(
                                    *iMusAvaSip->iSip,
                                    *iMusAvaSip->iSipProfileRegistryObserver );
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    TRAP( error, ret = iMusAvaSip->CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( ret );
    // Test profile settings
    // Resolve profile fail
    delete iMusAvaSip;
    iMusAvaSip = NULL;
    iMusAvaSip = CMusAvaSip::NewL();
    // Profile found, profile id not found
    CSipSseTestTls::Storage()->iProfileBehavior = KSIPProfileIdFetchFails;
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    TRAP( error, ret = iMusAvaSip->CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( ret );
    CSipSseTestTls::Storage()->Reset();
    
    // Profile found, iap id not found
    delete iMusAvaSip;
    iMusAvaSip = NULL;
    iMusAvaSip = CMusAvaSip::NewL();
    CSipSseTestTls::Storage()->iProfileBehavior = KSIPIapIdFetchFails;
    TRAP( error, ret = iMusAvaSip->CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( !ret );
    CSipSseTestTls::Storage()->Reset();

    // All ok
    delete iMusAvaSip;
    iMusAvaSip = NULL;
    iMusAvaSip = CMusAvaSip::NewL();
    TRAP( error, ret = iMusAvaSip->CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( ret );

    }

void UT_CMusAvaSip::UT_CMusAvaSip_CreateSipProfileL()
	{
	// Test Profile
    //No profile
    TInt ret = NULL;
    TRAPD( error, ret = iMusAvaSip->CreateSipProfile() );
    if ( error == KErrNoMemory ) User::Leave( error );
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && ret == KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT ( ret );
    // Test Profile
    //Profile created
    delete iMusAvaSip;
    iMusAvaSip = NULL;
    iMusAvaSip = CMusAvaSip::NewL();
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    iMusAvaSip->iSipProfileRegistry = CSIPProfileRegistry::NewL(
                                    *iMusAvaSip->iSip,
                                    *iMusAvaSip->iSipProfileRegistryObserver );
    iMusAvaSip->iSipProfile = 
            iMusAvaSip->GetMusProfileL( *iMusAvaSip->iSipProfileRegistry );
	TRAP( error, ret = iMusAvaSip->CreateSipProfile() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && ret == KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT ( ret );
    }

void UT_CMusAvaSip::UT_CMusAvaSip_ProfileL()
    {
    //No profile
    EUNIT_ASSERT( iMusAvaSip->Profile() == NULL );
    //Profile created
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    EUNIT_ASSERT( iMusAvaSip->CreateProfileL());  
    EUNIT_ASSERT( iMusAvaSip->Profile() != NULL );
    }

void UT_CMusAvaSip::UT_CMusAvaSip_ProfileRegistryL()
    {
    //Create SIP Profile Registry
    CSIPProfileRegistry* registry = &iMusAvaSip->ProfileRegistryL();
    EUNIT_ASSERT( registry != NULL );
    //SIP Profile Registry is created
    registry = &iMusAvaSip->ProfileRegistryL();
    EUNIT_ASSERT( registry != NULL );
    }

void UT_CMusAvaSip::UT_CMusAvaSip_ConnectionL()
    {
    //No connection
    EUNIT_ASSERT( iMusAvaSip->iSipConnection == NULL );
    //Connection is created when SIP profile is created
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    EUNIT_ASSERT( iMusAvaSip->CreateProfileL() );  
    
    //Still no connection
    EUNIT_ASSERT( iMusAvaSip->iSipConnection == NULL );
    
    TRAPD( err, iMusAvaSip->ConnectionL( EFalse ) );
    EUNIT_ASSERT( err == KErrSIPInvalidRegistrationState );
    
    // Forced creation (done normally when profile is registered).
    EUNIT_ASSERT( iMusAvaSip->ConnectionL() != NULL );
    }

void UT_CMusAvaSip::UT_CMusAvaSip_ConnectionObserverL()
    {
    MSIPConnectionObserver& con = iMusAvaSip->ConnectionObserver();
    EUNIT_ASSERT( &con != NULL );
    }

void UT_CMusAvaSip::UT_CMusAvaSip_SipL()
    {
     CSIP& sip = iMusAvaSip->Sip();
    EUNIT_ASSERT( &sip != NULL );
    }

void UT_CMusAvaSip::UT_CMusAvaSip_CreateSIPConnectionLL()
    {
    TUint32 iapId;
    //Profile created
    delete iMusAvaSip;
    iMusAvaSip = NULL;
    iMusAvaSip = CMusAvaSip::NewL();
    MultimediaSharingSettings::SetSipProfileSettingL( 0 );
    iMusAvaSip->CreateSipProfile();
    iMusAvaSip->Profile()->GetParameter( KSIPAccessPointId, iapId );
    iMusAvaSip->iIapId = iapId;
    iMusAvaSip->CreateSIPConnectionL();
   	EUNIT_ASSERT( iMusAvaSip->ConnectionL( EFalse )->IapId() == iapId );
    }

void UT_CMusAvaSip::UT_CMusAvaSip_OwnDomainL()
    {
    // Profile does not exist
    HBufC* ownDomain = iMusAvaSip->OwnDomainLC();
    EUNIT_ASSERT( ownDomain->Length() == 0 );
    CleanupStack::PopAndDestroy( ownDomain );
    ownDomain = NULL;
    
    // Profile exists, no aors 
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    TInt ret = NULL;
    TRAPD( error, ret = iMusAvaSip->CreateProfileL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && ret == KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT ( ret );
    ownDomain = iMusAvaSip->OwnDomainLC();
    EUNIT_ASSERT( ownDomain->Length() == 0 );
    CleanupStack::PopAndDestroy( ownDomain );
    ownDomain = NULL;
    
    // Empty aors
    CSIPProfile* profile = iMusAvaSip->Profile();
    delete profile->iArray;
    profile->iArray = NULL;
    profile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
    ownDomain = iMusAvaSip->OwnDomainLC();
    EUNIT_ASSERT( ownDomain->Length() == 0 );
    CleanupStack::PopAndDestroy( ownDomain );
    ownDomain = NULL;
    
    // Aors found
    profile->iArray->AppendL( _L8("sip:user@domain.com") );
    ownDomain = iMusAvaSip->OwnDomainLC();
    EUNIT_ASSERT( ownDomain->Length() > 0 );
    CleanupStack::PopAndDestroy( ownDomain );
    }
   
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaSip,
    "CMusAvaSip.",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaSip",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_NewLL, Teardown)
    
EUNIT_TEST(
    "AddAdapterL - test ",
    "CMusAvaSip",
    "AddAdapterL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_AddAdapterLL, Teardown)
    
EUNIT_TEST(
    "RemoveAdapter - test ",
    "CMusAvaSip",
    "RemoveAdapter",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_RemoveAdapterL, Teardown)    


EUNIT_TEST(
    "CreateProfile - test ",
    "CMusAvaSip",
    "CreateProfile",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_CreateProfileL, Teardown)

EUNIT_TEST(
    "CreateSipProfile - test ",
    "CMusAvaSip",
    "CreateSipProfile",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_CreateSipProfileL, Teardown)    
    

EUNIT_TEST(
    "ProfileL - test ",
    "CMusAvaSip",
    "ProfileL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_ProfileL, Teardown)
    
EUNIT_TEST(
    "ProfileRegistry - test ",
    "CMusAvaSip",
    "ProfileRegistry",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_ProfileRegistryL, Teardown)
    
    EUNIT_TEST(
    "Connection - test ",
    "CMusAvaSip",
    "Connection",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_ConnectionL, Teardown)
    
EUNIT_TEST(
    "ConnectionObserver - test ",
    "CMusAvaSip",
    "ConnectionObserver",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_ConnectionObserverL, Teardown)
    
EUNIT_TEST(
    "Sip - test ",
    "CMusAvaSip",
    "Sip",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_SipL, Teardown)
    
EUNIT_TEST(
    "CreateSIPConnectionL - test ",
    "CMusAvaSip",
    "CreateSIPConnectionL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_CreateSIPConnectionLL, Teardown)

EUNIT_TEST(
    "OwnDomainLC - test ",
    "CMusAvaSip",
    "OwnDomainLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSip_OwnDomainL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE
