/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit tests for CMusAvailabilityPluginManager class
*
*/



#include "musunittesting.h"
#include "UT_CmusavailabilityPluginManager.h"
#include "mmusavasettingsstub.h"
#include "musavailabilitypluginmanager.h"
#include "musavaavailability.h"
#include "musavaavailabilitystub.h"
#include "musavainterface.h"
#include "cmusavainterfacestub.h"
#include "musmanagerservercommon.h"
#include "musapplicationmanager.h"
#include "mussesseioninformationapi.h"
#include <digia/eunit/eunitmacros.h>
#include <E32Math.h>
#include <E32Property.h>


/*
 * Video codec set in CMusAvailabilityManager.
 */
_LIT( KUsedVideoCodec, "H264" );

/*
 * Tel number from MusAvaSettingsStub.
 */
_LIT( KUsedTelNumber, "123" );

/*
 * SIP address from MusAvaSettingsStub.
 */
_LIT( KUsedSipAddress, "sip:stadi@hesa.fi" );

/*
 * Privacy status from MusAvaSettingsStub.
 */
_LIT( KUsedPrivacyStatus, "1" );

// ======== MEMBER FUNCTIONS ========


UT_CMusAvailabilityPluginManager* UT_CMusAvailabilityPluginManager::NewL()
    {
    UT_CMusAvailabilityPluginManager* self = UT_CMusAvailabilityPluginManager::NewLC();
    CleanupStack::Pop();
    return self;
    }


UT_CMusAvailabilityPluginManager* UT_CMusAvailabilityPluginManager::NewLC()
    {
    UT_CMusAvailabilityPluginManager* self = new( ELeave ) UT_CMusAvailabilityPluginManager();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


UT_CMusAvailabilityPluginManager::~UT_CMusAvailabilityPluginManager()
    {
    }


UT_CMusAvailabilityPluginManager::UT_CMusAvailabilityPluginManager()
    : CEUnitTestSuiteClass()
    {
    }


// ---------------------------------------------------------------------------
// The ConstructL from the base class CEUnitTestSuiteClass must be called.
// It generates the test case table.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// ---------------------------------------------------------------------------
// From class MMusAvailabilityPluginManagerObserver.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::PluginStopped()
    {
    iCalledPluginStoppedL = ETrue;
    }


// ---------------------------------------------------------------------------
// From class MMusAvailabilityPluginManagerObserver.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::AvailabilityChangedL(
    MultimediaSharing::TMusAvailabilityStatus /*aAvailability*/ )
    {
    iCalledAvailabilityChangedL = ETrue;
    }
// ---------------------------------------------------------------------------
// From class MMusAvailabilityPluginManagerObserver.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::StartLiveSharingL()
    {
    iCalledAStartLiveSharing = ETrue;
    }

// ---------------------------------------------------------------------------
// Setups a test by instantiating tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::SetupL()
    {
    iAppManager = CMusApplicationManager::NewL();
    iManager = CMusAvailabilityPluginManager::NewL( *this, *iAppManager );
    iCalledAvailabilityChangedL = EFalse;
    iCalledPluginStoppedL = EFalse;
    }


// ---------------------------------------------------------------------------
// Finalizes a test by deleting instance of tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::Teardown()
    {
    delete iManager;
    iManager = NULL;
    delete iAppManager;
    PropertyHelper::Close(); 
    
    }


// ======== TEST METHODS ========


// ---------------------------------------------------------------------------
// Asserts a successful instantiation.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::UT_CMusAvailabilityPluginManager_NewLL()
    {
    delete iManager;
    iManager = NULL;
    iManager = CMusAvailabilityPluginManager::NewL( *this, *iAppManager );
    EUNIT_ASSERT( iManager );
    }


// ---------------------------------------------------------------------------
// Asserts a successful instantiation.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::UT_CMusAvailabilityPluginManager_NewLCL()
    {
    delete iManager;
    iManager = NULL;
    iManager = CMusAvailabilityPluginManager::NewLC( *this, *iAppManager );
    EUNIT_ASSERT( iManager );
    CleanupStack::Pop();
    }
    
// ---------------------------------------------------------------------------
//
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::UT_CMusAvailabilityPluginManager_AvailabilityL()
    {
    iManager->Availability();
    }

// ---------------------------------------------------------------------------
// Asserts that plugin is started after a call to InvestigateAvailabilityL.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::UT_CMusAvailabilityPluginManager_InvestigateAvailabilityLL()
    {
    iManager->InvestigateAvailabilityL();
    EUNIT_ASSERT( iManager->iPluginStarted );

    // same code again skipping !iPluginStarted
    iManager->InvestigateAvailabilityL();
    EUNIT_ASSERT( iManager->iPluginStarted );
    }


// ---------------------------------------------------------------------------
// Asserts that plugin is started after a call to InvitationReceivedL.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::UT_CMusAvailabilityPluginManager_InvitationReceivedLL()
    {
    iManager->InvitationReceivedL();
    EUNIT_ASSERT( iManager->iPluginStarted );

    // same code again skipping !iPluginStarted
    iManager->InvitationReceivedL();
    EUNIT_ASSERT( iManager->iPluginStarted );
    }


// ---------------------------------------------------------------------------
// Asserts that plugin is started after a call to OptionsReceivedL.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::UT_CMusAvailabilityPluginManager_OptionsReceivedLL()
    {
    iManager->OptionsReceivedL();
    EUNIT_ASSERT( iManager->iPluginStarted );

    // same code again skipping !iPluginStarted
    iManager->OptionsReceivedL();
    EUNIT_ASSERT( iManager->iPluginStarted );
    }


// ---------------------------------------------------------------------------
// Asserts that plugin is no more started after a call to StopPluginL.
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::UT_CMusAvailabilityPluginManager_StopPluginLL()
    {
    iManager->StopPluginL();
    EUNIT_ASSERT( !iManager->iPluginStarted );
    iManager->InvestigateAvailabilityL();
    EUNIT_ASSERT( iManager->iPluginStarted );
    iManager->StopPluginL();
    EUNIT_ASSERT( !iManager->iPluginStarted );
    }


// ---------------------------------------------------------------------------
// Asserts that session parameters comply with defaults from
// CMusAvaSettings stub (tel number, SIP address) and
// CMusAvailabilityPluginManager (video codec).
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::UT_CMusAvailabilityPluginManager_SessionParametersL()
    {
    CMusAvaInterface* interface = iManager->iPlugin;
    CMusAvaInterface2* interface2 =  static_cast<CMusAvaInterface2*>( interface );
    interface2->iSettings->iCallPrivacy = 1;
    
    MDesCArray* params = &iManager->SessionParametersL();
    EUNIT_ASSERT_EQUALS( KUsedTelNumber(), params->MdcaPoint( KTelNumber ) );
    EUNIT_ASSERT_EQUALS( KUsedSipAddress(), params->MdcaPoint( KSipAddress ) );
    EUNIT_ASSERT_EQUALS( KUsedVideoCodec(), params->MdcaPoint( KVideoCodec ) );
    EUNIT_ASSERT( !params->MdcaPoint( KPrivacyStatus ).Compare( KUsedPrivacyStatus() ) );
    }
    
// ---------------------------------------------------------------------------
// 
// ---------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::
    UT_CMusAvailabilityPluginManager_ApplicationStateL()
    {
    EUNIT_ASSERT( iManager->ApplicationState() == 
    	MMusAvaSettingsObserver::EApplicationNotRunning );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::UT_CMusAvailabilityPluginManager_OptionAllowedL()
    {
    CMusAvaInterface* interface = iManager->iPlugin;
    CMusAvaInterface2* interface2 =  static_cast<CMusAvaInterface2*>( interface );
   
	CMusAvaAvailabilityStub* abilityStub =  
		static_cast<CMusAvaAvailabilityStub*>( interface2->iAvailabilities[0] );    
    EUNIT_ASSERT( !iManager->OptionAllowed() );
    abilityStub->iNameStub = MMusAvaObserver::EMusAvaOptionHandler;
    EUNIT_ASSERT( iManager->OptionAllowed() );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAvailabilityPluginManager::UT_CMusAvailabilityPluginManager_StartLiveSharingLL()
    {
    iManager->StartLiveSharingL();
    }
// ---------------------------------------------------------------------------
// Asserts that availabilitymapping is done correctly.
// ---------------------------------------------------------------------------
//
void  UT_CMusAvailabilityPluginManager::
    UT_CMusAvailabilityPluginManager_MapAvailabilitiesL()
    {
    CMusAvaInterface* interface = iManager->iPlugin;
    CMusAvaInterface2* interface2 =  static_cast<CMusAvaInterface2*>( interface );
    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaStatusNotExecuted )
        == MultimediaSharing::EMultimediaSharingNotAvailable );
    
    	//EMusAvaStatusInProgress
    	//It is not registered
   		interface2->iCurrentAvailability = 
		MMusAvaObserver::EMusAvaNameNetworkStatus;  
    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaStatusNotExecuted )
        == MultimediaSharing::EMultimediaSharingNotAvailable );
    	//It is registered
    	interface2->iCurrentAvailability = 
    	MMusAvaObserver::EMusAvaOptionHandler;    
    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaStatusInProgress )
        == MultimediaSharing::EMultimediaSharingAvailable );
		//registration is pending
    	interface2->iCurrentAvailability = 
		MMusAvaObserver::EMusAvaNameRegistration;
    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaStatusInProgress )
        == MultimediaSharing::ESipRegistrationPending );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaStatusNoSipProfile )
        == MultimediaSharing::EErrNoSipProfile );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaStatusNotRegistered )
        == MultimediaSharing::EErrSipRegistration );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaStatusNoCall )
        == MultimediaSharing::EErrNoActiveCall );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaStatusCallOnHold )
        == MultimediaSharing::EErrCallOnHold );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaStatusConferenceCall )
        == MultimediaSharing::EErrConferenceCall );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaStatusOptionsSent )
        == MultimediaSharing::ESipOptionsSent );
        
    EUNIT_ASSERT( iManager->MapAvailabilities(
    	MMusAvaObserver::EMusAvaStatusOptionsNotSent )
    	== MultimediaSharing::ESipOptionsNotSent );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaOptionNotAvailable )
        == MultimediaSharing::ESipOptionsNotCapable );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaNetworkType )
        == MultimediaSharing::EErrNetwork );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaRoamingError )
        == MultimediaSharing::EErrRoaming );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaConnectionErr )
        == MultimediaSharing::EErrConnection );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusAvaManualActivation )
        == MultimediaSharing::EManualActivation );
        
    EUNIT_ASSERT( iManager->MapAvailabilities(
        MMusAvaObserver::EMusActivationError )
        == MultimediaSharing::EErrActivation );

    EUNIT_ASSERT( iManager->MapAvailabilities(
        (MMusAvaObserver::TAvailabilityStatus)KErrUnknown )
        == (MultimediaSharing::TMusAvailabilityStatus)KErrUnknown );
    
    
    }


// ---------------------------------------------------------------------------
// Asserts that session parameters are updated.
// ---------------------------------------------------------------------------
//
void  UT_CMusAvailabilityPluginManager::
    UT_CMusAvailabilityPluginManager_UpdateSessionParametersLL()
    {       
    MDesCArray* params = &iManager->SessionParametersL();
    CMusAvaInterface* interface = iManager->iPlugin;
    CMusAvaInterface2* interface2 =  static_cast<CMusAvaInterface2*>( interface );
    interface2->iSettings->iCallPrivacy = 1;
    EUNIT_ASSERT_EQUALS( KUsedTelNumber(), iManager->SessionParametersL().MdcaPoint( KTelNumber ) );
    EUNIT_ASSERT_EQUALS( KUsedSipAddress(), iManager->SessionParametersL().MdcaPoint( KSipAddress ) );
    EUNIT_ASSERT_EQUALS( KUsedVideoCodec(), iManager->SessionParametersL().MdcaPoint( KVideoCodec ) );
    EUNIT_ASSERT( !iManager->SessionParametersL().MdcaPoint( KPrivacyStatus ).Compare( KUsedPrivacyStatus() ) );
    }


// ---------------------------------------------------------------------------
// Asserts that session parameters are updated.
// ---------------------------------------------------------------------------
//
void  UT_CMusAvailabilityPluginManager::
    UT_CMusAvailabilityPluginManager_SipAddressesLCL()
    {    
    EUNIT_ASSERT( iManager->SipAddressesLC() );
    CleanupStack::PopAndDestroy();

    CMusAvaSettings2& settings = STATIC_CAST(CMusAvaSettings2&,iManager->iPlugin->Settings());
    settings.iSipAddresses->Reset();
    // append enough sip address so that it will exceed 
    // RProperty::KMaxPropertySize = 512 bytes
    // 512/29(per sip address ) = 18 ( approx )
    settings.iSipAddresses->AppendL(_L("sip:testtesttest1@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest2@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest4@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest5@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest6@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest7@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest8@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest9@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest10@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest11@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest12@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest13@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest14@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest15@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest16@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest17@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest18@10.21.32.51"));
    settings.iSipAddresses->AppendL(_L("sip:testtesttest19@10.21.32.51"));
    
    HBufC* retDes = iManager->SipAddressesLC();
    EUNIT_ASSERT( retDes->Length()<RProperty::KMaxPropertySize );    
    CleanupStack::PopAndDestroy();
    }


// ---------------------------------------------------------------------------
// Asserts that tel number is instantiated and put to cleanupstack.
// ---------------------------------------------------------------------------
//
void  UT_CMusAvailabilityPluginManager::
    UT_CMusAvailabilityPluginManager_TelNumberLCL()
    {
    EUNIT_ASSERT( iManager->TelNumberLC() );
    CleanupStack::PopAndDestroy();
    }


// ---------------------------------------------------------------------------
// Asserts that contact name is instantiated and put to cleanupstack.
// ---------------------------------------------------------------------------
//
void  UT_CMusAvailabilityPluginManager::
    UT_CMusAvailabilityPluginManager_ContactNameLCL()
    {
    EUNIT_ASSERT( iManager->ContactNameLC() );
    CleanupStack::PopAndDestroy();
    }
void UT_CMusAvailabilityPluginManager::
    UT_CMusAvailabilityPluginManager_IndicateAvailabilityLL()
    {
    iManager->IndicateAvailabilityL();
    }

// ======== EUNIT TEST TABLE ========


EUNIT_BEGIN_TEST_TABLE(
    UT_CMusAvailabilityPluginManager,
    "CMusAvailabilityPluginManager",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvailabilityPluginManager",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_NewLL, Teardown )

EUNIT_TEST(
    "NewLC - test ",
    "CMusAvailabilityPluginManager",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_NewLCL, Teardown )

EUNIT_TEST(
    "InvestigateAvailabilityL - test ",
    "CMusAvailabilityPluginManager",
    "InvestigateAvailabilityL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_InvestigateAvailabilityLL, Teardown )

EUNIT_TEST(
    "InvitationReceivedL - test ",
    "CMusAvailabilityPluginManager",
    "InvitationReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_InvitationReceivedLL, Teardown )

EUNIT_TEST(
    "OptionsReceivedL - test ",
    "CMusAvailabilityPluginManager",
    "OptionsReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_OptionsReceivedLL, Teardown )

EUNIT_TEST(
    "StopPluginL - test ",
    "CMusAvailabilityPluginManager",
    "StopPluginL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_StopPluginLL, Teardown )

EUNIT_TEST(
    "SessionParameters - test ",
    "CMusAvailabilityPluginManager",
    "SessionParameters",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_SessionParametersL, Teardown )

EUNIT_TEST(
    "ApplicationState - test ",
    "CMusAvailabilityPluginManager",
    "ApplicationState",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_ApplicationStateL, Teardown )

EUNIT_TEST(
    "Availability - test ",
    "CMusAvailabilityPluginManager",
    "Availability",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_AvailabilityL, Teardown )


EUNIT_TEST(
    "StartLiveSharingL - test ",
    "CMusAvailabilityPluginManager",
    "StartLiveSharingL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_StartLiveSharingLL, Teardown )
// NEW

EUNIT_TEST(
    "MapAvailabilities - test ",
    "CMusAvailabilityPluginManager",
    "MapAvailabilities",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_MapAvailabilitiesL, Teardown )

EUNIT_TEST(
    "UpdateSessionParametersL - test ",
    "CMusAvailabilityPluginManager",
    "UpdateSessionParametersL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_UpdateSessionParametersLL, Teardown )

EUNIT_TEST(
    "SipAddressesLC - test ",
    "CMusAvailabilityPluginManager",
    "SipAddressesLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_SipAddressesLCL, Teardown )

EUNIT_TEST(
    "TelNumberLC - test ",
    "CMusAvailabilityPluginManager",
    "TelNumberLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_TelNumberLCL, Teardown )

EUNIT_TEST(
    "ContactNameLC - test ",
    "CMusAvailabilityPluginManager",
    "ContactNameLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_ContactNameLCL, Teardown )

EUNIT_TEST(
    "IndicateAvailabilityL - test ",
    "CMusAvailabilityPluginManager",
    "IndicateAvailabilityL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvailabilityPluginManager_IndicateAvailabilityLL, Teardown )
    

EUNIT_END_TEST_TABLE
