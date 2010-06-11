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
#include <CTSYDomainPSKeys.h>
#include "etelmm.h"
#include "ut_musfactorysettings.h"
#include "musfactorysettings.h"
#include "centralrepository.h"
#include "e32property.h"


//  SYSTEM INCLUDES
#include <eunitmacros.h>

// -----------------------------------------------------------------------------
// Two phase symbian constructor
// -----------------------------------------------------------------------------
//
UT_CMusFactorySettings* UT_CMusFactorySettings::NewL()
    {
    UT_CMusFactorySettings* self = UT_CMusFactorySettings::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
// Two phase symbian constructor
// -----------------------------------------------------------------------------
//
UT_CMusFactorySettings* UT_CMusFactorySettings::NewLC()
    {
    UT_CMusFactorySettings* self = new( ELeave ) UT_CMusFactorySettings();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
UT_CMusFactorySettings::~UT_CMusFactorySettings()
    {  
    
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusFactorySettings::UT_CMusFactorySettings()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusFactorySettings::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusFactorySettings::SetupL()
    {
    iMusCentralRepository = CRepository::NewL( MusSettingsKeys::KRepositoryUid );    
    CRepository::ResetStubGlobal();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusFactorySettings::Teardown()
    {  
    delete iMusCentralRepository;   
    iMusCentralRepository = NULL;    
    CRepository::ResetStubGlobal();
    PropertyHelper::Close(); // CRepository puts stuff to RProperty static
    }

// TEST CASES

// -----------------------------------------------------------------------------
// SetCallInfo test function
// -----------------------------------------------------------------------------
//
void UT_CMusFactorySettings::UT_CMusFactorySettings_IsSupportedLL()
    {    
    // call cases    
    // all call types allowed    
    TPSCTsyCallType callType = EPSCTsyCallTypeCSVoice;
    User::LeaveIfError( CRepository::SetStubGlobal(MusSettingsKeys::KAllowOnlyWithActiveCSCall,
                                                   MusSettingsKeys::EAllowedAllCalls) );
    EUNIT_ASSERT( MusFactorySettings::IsSupportedL( callType ));
   
    // allowed csonly and call type is voip
    callType = EPSCTsyCallTypeVoIP;
    User::LeaveIfError( CRepository::SetStubGlobal(MusSettingsKeys::KAllowOnlyWithActiveCSCall,
                              MusSettingsKeys::EAllowedCSOnly) );
    EUNIT_ASSERT( !MusFactorySettings::IsSupportedL( callType ));
    
    // allowed csonly and call type is cs
    callType = EPSCTsyCallTypeCSVoice;
    User::LeaveIfError( CRepository::SetStubGlobal(MusSettingsKeys::KAllowOnlyWithActiveCSCall,
                              MusSettingsKeys::EAllowedCSOnly) );
    EUNIT_ASSERT( MusFactorySettings::IsSupportedL( callType ));
               
    // allowed csonly and call type is EPSCTsyCallTypeNone
    callType = EPSCTsyCallTypeNone;
    User::LeaveIfError( CRepository::SetStubGlobal(MusSettingsKeys::KAllowOnlyWithActiveCSCall,
                              MusSettingsKeys::EAllowedCSOnly) );
    EUNIT_ASSERT( !MusFactorySettings::IsSupportedL( callType )); 
 
    // network cases
    // all bearer types allowed
    RMobilePhone::TMobilePhoneNetworkMode networkType = RMobilePhone::ENetworkModeGsm;
    User::LeaveIfError( CRepository::SetStubGlobal(MusSettingsKeys::KAllowOnlyIn3GNetwork,
                               MusSettingsKeys::EAllowedAllBearers) );
    EUNIT_ASSERT( MusFactorySettings::IsSupportedL( networkType ));
    
    // 3g only and network type is gsm
    networkType = RMobilePhone::ENetworkModeGsm;
    User::LeaveIfError( CRepository::SetStubGlobal(MusSettingsKeys::KAllowOnlyIn3GNetwork,
                                MusSettingsKeys::EAllowed3GOnly) );
    EUNIT_ASSERT( !MusFactorySettings::IsSupportedL( networkType ));  
    
    // 3g only and network type is 3g
    networkType = RMobilePhone::ENetworkModeWcdma;
    User::LeaveIfError( CRepository::SetStubGlobal(MusSettingsKeys::KAllowOnlyIn3GNetwork,
                                 MusSettingsKeys::EAllowed3GOnly) );
    EUNIT_ASSERT( MusFactorySettings::IsSupportedL( networkType ));   
    
    // if allowed3gonly setting cenrep key not found
    // then function should not leave and should behave like in default
    // case ie all bearer supported. so it should return ETrue
    networkType = RMobilePhone::ENetworkModeGsm;
    User::LeaveIfError( CRepository::SetStubGlobal(MusSettingsKeys::KAllowOnlyIn3GNetwork,
                                     MusSettingsKeys::EAllowed3GOnly) );
    CRepository::iForceFailWithCode = KErrNotFound;
    TBool supported = EFalse;
    EUNIT_ASSERT_NO_LEAVE( supported = MusFactorySettings::IsSupportedL( networkType ))
    EUNIT_ASSERT( supported );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusFactorySettings,
    "UT_CMusFactorySettings",
    "UNIT" )

EUNIT_TEST(
    "IsSupportedL - test",
    "CMusFactorySettings",
    "IsSupportedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusFactorySettings_IsSupportedLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE


