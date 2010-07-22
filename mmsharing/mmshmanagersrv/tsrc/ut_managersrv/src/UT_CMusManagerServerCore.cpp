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


#include "UT_CMusManagerServerCore.h"
#include "musunittesting.h"
#include "musmanagerservercore.h"
#include "musavailabilitypluginmanager.h"
#include "musavaavailability.h"
#include "musavaavailabilitystub.h"
#include "musavainterface.h"
#include "cmusavainterfacestub.h"
#include "mussessionproperties.h"
#include "mustesthelp.h"
#include <e32property.h>
#include <apgtask.h>
#include <digia/eunit/eunitmacros.h>

_LIT8( KMusEngineName, "MultimediaSharing" );
_LIT8( KAnotherEngineName, "AnotherEngine" );

// ======== MEMBER FUNCTIONS ========


UT_CMusManagerServerCore* UT_CMusManagerServerCore::NewL()
    {
    UT_CMusManagerServerCore* self = UT_CMusManagerServerCore::NewLC();
    CleanupStack::Pop();
    return self;
    }


UT_CMusManagerServerCore* UT_CMusManagerServerCore::NewLC()
    {
    UT_CMusManagerServerCore* self = new( ELeave ) UT_CMusManagerServerCore();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


UT_CMusManagerServerCore::~UT_CMusManagerServerCore()
    {
    }


UT_CMusManagerServerCore::UT_CMusManagerServerCore()
    {
    }


// ---------------------------------------------------------------------------
// The ConstructL from the base class CEUnitTestSuiteClass must be called.
// It generates the test case table.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// ---------------------------------------------------------------------------
// From MMusManagerServerCoreObserver.
// Stops server.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::StopServer()
    {
    iStopServerCalled = ETrue;
    }


// ---------------------------------------------------------------------------
// From MMusManagerServerCoreObserver.
// Returns count of sessions.
// ---------------------------------------------------------------------------
//
TUint UT_CMusManagerServerCore::SessionCount()
    {
    return iSessionCount;
    }


// ---------------------------------------------------------------------------
// From MMusMonitorAvailabilityObserver.
// Informs of a change in multimediasharing availability.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::AvailabilityChangedL(
    MultimediaSharing::TMusAvailabilityStatus /*aStatus*/ )
    {
    iAvailabilityChangedLCalled = ETrue;
    }

void UT_CMusManagerServerCore::RequestComplete()
	{
		
	}
// ---------------------------------------------------------------------------
// Setups the test by instantiating tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::SetupL()
    {
    iCore = CMusManagerServerCore::NewL( *this );
    iStopServerCalled = EFalse;
    iAvailabilityChangedLCalled = EFalse;
    iSessionCount = 0;
    }


// ---------------------------------------------------------------------------
// Finalizes test by deleting instance of tested class.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::Teardown()
    {
    delete iCore;
    PropertyHelper::Close();
    Dll::FreeTls(); // Used by the RProcess and TFindProcess stubs 
    }


// ======== TEST METHODS ========


// ---------------------------------------------------------------------------
// Asserts that instance creation is successful.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::UT_CMusManagerServerCore_NewLL()
    {
    EUNIT_ASSERT( iCore );
    }


// ---------------------------------------------------------------------------
// Asserts that instance creation is successful.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::UT_CMusManagerServerCore_NewLCL()
    {
    delete iCore;
    iCore = NULL;
    iCore = CMusManagerServerCore::NewLC( *this );
    EUNIT_ASSERT( iCore );
    CleanupStack::Pop( iCore );
    }


// ---------------------------------------------------------------------------
// Availability query should return MMusAvaObserver::EMusAvaStatusNotAvailable
// when videosharing is not yet available.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::UT_CMusManagerServerCore_AvailabilityQueryLL()
    {
    EUNIT_ASSERT( iCore->AvailabilityQueryL()
        == MultimediaSharing::EMultimediaSharingNotAvailable );
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::
    UT_CMusManagerServerCore_InvestigateAvailabilityLL()
    {
    iCore->InvestigateAvailabilityL();
    }


// ---------------------------------------------------------------------------
// Asserts that invitation with inappropriate UID should leave.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::UT_CMusManagerServerCore_InvitationReceivedLL()
    {
    // 1. The cases with MuS engine
    iCore->iPluginName = KMusEngineName;
    
    TUid uid = { 0x10101010 }; // Not a valid UID in musmanager.h
    // since used UID is not in appropriate, this should leave
    EUNIT_ASSERT_SPECIFIC_LEAVE(
        iCore->InvitationReceivedL( uid ), KErrNotReady );

    uid.iUid = 0x1028238D; // ESipInviteNotDesired from musmanager.h
    iCore->InvitationReceivedL( uid );

    uid.iUid = 0x10282391; // ESipInviteDesired from musmanager.h
    EUNIT_ASSERT_SPECIFIC_LEAVE(
        iCore->InvitationReceivedL( uid ), KErrNotReady );

    // 2. The cases with non-MuS engine
    iCore->iPluginName = KAnotherEngineName;
    
    uid.iUid = 0x10101010; // Not a valid UID in musmanager.h
    iCore->InvitationReceivedL( uid );

    uid.iUid = 0x1028238D; // ESipInviteNotDesired from musmanager.h
    iCore->InvitationReceivedL( uid );

    uid.iUid = 0x10282391; // ESipInviteDesired from musmanager.h
    iCore->InvitationReceivedL( uid );    
    }


// ---------------------------------------------------------------------------
// Dummy test to increase execution coverage.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::UT_CMusManagerServerCore_OptionsReceivedLL()
    {
    TUid uid = { 0x10101010 };
    iCore->iPluginName = KMusEngineName;
    iCore->iPluginManager->iPluginStarted = EFalse;
    iCore->OptionsReceivedL( uid );
    EUNIT_ASSERT( iCore->iPluginManager->iPluginStarted );
    
    iCore->iPluginName = KAnotherEngineName;
    iCore->iPluginManager->iPluginStarted = EFalse;
    iCore->OptionsReceivedL( uid );
    EUNIT_ASSERT ( !(iCore->iPluginManager->iPluginStarted) );
    }


// ---------------------------------------------------------------------------
// Asserts that tested method does not leave. In certain case it could leave
// with KErrPermissionDenied, but with this this test, used capabilities
// and used stubs, tested method shouldn't leave.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::
    UT_CMusManagerServerCore_StartMultimediaSharingLL()
    {
    CMusAvaInterface* interface = iCore->iPluginManager->iPlugin;
    CMusAvaInterface2* interface2 =  static_cast<CMusAvaInterface2*>( interface );
    CMusAvaAvailabilityStub* abilityStub =  
	static_cast<CMusAvaAvailabilityStub*>( interface2->iAvailabilities[0] ); 
    // -----------------------------------------------------------------------
    // Dummy test that shouldn't leave ->
    // -----------------------------------------------------------------------
	MUS_EUNIT_ASSERT_NO_LEAVE(
        iCore->StartMultimediaSharingL( MultimediaSharing::EMusLiveVideo ) );
    // <- Dummy test that shouldn't leave

    // -----------------------------------------------------------------------
    // Test for "if( !iApplicationManager->ApplicationRunning() &&
    // iPluginManager->ApplicationAllowed() ) branch ->
    // -----------------------------------------------------------------------
    // CMusAvailabilityPluginManager::ApplicationAllowed = ETrue
    abilityStub->iNameStub = MMusAvaObserver::EMusAvaNameRegistration;
    abilityStub->iStatusStub = MMusAvaObserver::EMusAvaStatusAvailable;
  	interface2->iCurrentAvailability =MMusAvaObserver::EMusAvaOptionHandler;

    PropertyHelper::SetErrorCode(KErrGeneral);

    // Should leave in WriteSessionPropertiesL
    MUS_EUNIT_ASSERT_SPECIFIC_LEAVE(
        iCore->StartMultimediaSharingL( MultimediaSharing::EMusLiveVideo ),
        KErrGeneral );

    PropertyHelper::SetErrorCode(KErrNone);
    
    // Should go all the way to StartApplicationL
    MUS_EUNIT_ASSERT_NO_LEAVE(
        iCore->StartMultimediaSharingL( MultimediaSharing::EMusLiveVideo ) );
    EUNIT_ASSERT( PropertyHelper::GetCalledFunction() == RProperty::EDefine );

    // <- Test for "if( !iApplicationManager->ApplicationRunning() &&
    // iPluginManager->ApplicationAllowed() ) branch

    // -----------------------------------------------------------------------
    // Test for else branch ->
    // -----------------------------------------------------------------------

    // CMusAvailabilityPluginManager::ApplicationAllowed = EFalse
    abilityStub->iNameStub = MMusAvaObserver::EMusAvaNameRegistration;
    abilityStub->iStatusStub = MMusAvaObserver::EMusAvaStatusNotExecuted;
  	interface2->iCurrentAvailability =MMusAvaObserver::EMusAvaNameRegistration;


    // CMusApplicationManager::ApplicationRunning = EFalse
    MUS_EUNIT_ASSERT_NO_LEAVE(
        iCore->StartMultimediaSharingL( MultimediaSharing::EMusLiveVideo ) );
    }


// ---------------------------------------------------------------------------
// Asserts that tested method checks if task exists and calls
// TApaTask::KillTask if task exists.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::
    UT_CMusManagerServerCore_StopMultimediaSharingLL()
    {
    // MuS not running
    iCore->StopMultimediaSharingL();
    
    // MuS running
    User::LeaveIfError( 
        Dll::SetTls( reinterpret_cast< TAny* >( 1 ) ) ); 
    // TLS is used by TFindProcess stub
    iCore->StopMultimediaSharingL();
    TInt availability = MultimediaSharing::EMultimediaSharingAvailable;
    TUint key( NMusSessionApi::KStatus );
    RProperty::Get( key, availability );
    EUNIT_ASSERT_EQUALS( PropertyHelper::GetCalledFunction(), RProperty::EDefine ) 
    EUNIT_ASSERT_EQUALS( availability, 
                         MultimediaSharing::EMultimediaSharingNotAvailable )
    }


// ---------------------------------------------------------------------------
// Asserts that observer method MMusManagerServerCoreObserver::StopServer is
// called.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::UT_CMusManagerServerCore_EventNoSessionsL()
    {
    iCore->iPluginName = KMusEngineName;
    iCore->iPluginManager->iPluginStarted = ETrue;
    iCore->EventNoSessions();
    EUNIT_ASSERT ( !(iCore->iPluginManager->iPluginStarted) );
    EUNIT_ASSERT( iStopServerCalled );

    iCore->iPluginName = KAnotherEngineName;
    iCore->iPluginManager->iPluginStarted = ETrue;
    iCore->EventNoSessions();
    EUNIT_ASSERT ( iCore->iPluginManager->iPluginStarted );
    EUNIT_ASSERT( iStopServerCalled );
    }


// ---------------------------------------------------------------------------
// Asserts that MMusManagerServerCoreObserver::StopServer is called when it
// should.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::UT_CMusManagerServerCore_PluginStoppedL()
    {
    // Should not call iStopServer when sessions still exists.
    iSessionCount = 1;
    iCore->PluginStopped();
    EUNIT_ASSERT( !iStopServerCalled );

    // Should call iStopServer when sessioncount = 0.
    iSessionCount = 0;
    iCore->PluginStopped();
    EUNIT_ASSERT( iStopServerCalled );
    }


// ---------------------------------------------------------------------------
// Asserts that KErrNone should be returned.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::
    UT_CMusManagerServerCore_AvailabilityChangedLL()
    {
    iCore->iPluginName = KMusEngineName;
    MUS_EUNIT_ASSERT_NO_LEAVE( iCore->AvailabilityChangedL(
        (MultimediaSharing::TMusAvailabilityStatus) KErrNone ) );
    EUNIT_ASSERT( iCore->iAvailabilityMonitors.Count() == 0 );

    iCore->iPluginName = KAnotherEngineName;
    MUS_EUNIT_ASSERT_NO_LEAVE( iCore->AvailabilityChangedL(
        (MultimediaSharing::TMusAvailabilityStatus) KErrNone ) );
    EUNIT_ASSERT( iCore->iAvailabilityMonitors.Count() == 0 );
    }


// ---------------------------------------------------------------------------
// Asserts that called method removes *this* monitor.
// ---------------------------------------------------------------------------
//
void UT_CMusManagerServerCore::UT_CMusManagerServerCore_RegisterObserverL()
    {
    TInt monitors = iCore->iAvailabilityMonitors.Count();
    iCore->RegisterObserverL( this );
    EUNIT_ASSERT( iCore->iAvailabilityMonitors.Count() == monitors + 1 );
    iCore->RemoveObserver( this );
    EUNIT_ASSERT( iCore->iAvailabilityMonitors.Count() == monitors );
    }

void UT_CMusManagerServerCore::UT_CMusManagerServerCore_IsMusEnginePluginL()
    {
    iCore->iPluginName = KMusEngineName;
    EUNIT_ASSERT( iCore->IsMusEnginePlugin() );
    
    iCore->iPluginName = KAnotherEngineName;
    EUNIT_ASSERT( !(iCore->IsMusEnginePlugin()) );
    }


// ======== EUNIT TEST TABLE ========


EUNIT_BEGIN_TEST_TABLE(
    UT_CMusManagerServerCore,
    "CMusManagerServerCore",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusManagerServerCore",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_NewLL, Teardown)

EUNIT_TEST(
    "NewLC - test ",
    "CMusManagerServerCore",
    "NewLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_NewLCL, Teardown)


EUNIT_TEST(
    "RegisterObserver - test ",
    "CMusManagerServerCore",
    "RegisterObserver",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_RegisterObserverL, Teardown)

EUNIT_TEST(
    "AvailabilityQueryL - test ",
    "CMusManagerServerCore",
    "AvailabilityQueryL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_AvailabilityQueryLL, Teardown)

EUNIT_TEST(
    "InvestigateAvailabilityL - test ",
    "CMusManagerServerCore",
    "InvestigateAvailabilityL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_InvestigateAvailabilityLL, Teardown)

EUNIT_TEST(
    "InvitationReceivedL - test ",
    "CMusManagerServerCore",
    "InvitationReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_InvitationReceivedLL, Teardown)

EUNIT_TEST(
    "OptionsReceivedL - test ",
    "CMusManagerServerCore",
    "OptionsReceivedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_OptionsReceivedLL, Teardown)

EUNIT_TEST(
    "StartMultimediaSharingL - test ",
    "CMusManagerServerCore",
    "StartMultimediaSharingL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_StartMultimediaSharingLL, Teardown)

EUNIT_TEST(
    "StopMultimediaSharingL - test ",
    "CMusManagerServerCore",
    "StopMultimediaSharingL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_StopMultimediaSharingLL, Teardown)

EUNIT_TEST(
    "EventNoSessions - test ",
    "CMusManagerServerCore",
    "EventNoSessions",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_EventNoSessionsL, Teardown)

EUNIT_TEST(
    "PluginStopped - test ",
    "CMusManagerServerCore",
    "PluginStopped",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_PluginStoppedL, Teardown)

EUNIT_TEST(
    "AvailabilityChangedL - test ",
    "CMusManagerServerCore",
    "AvailabilityChangedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_AvailabilityChangedLL, Teardown)

EUNIT_TEST(
    "IsMusEnginePlugin - test ",
    "CMusManagerServerCore",
    "IsMusEnginePlugin",
    "FUNCTIONALITY",
    SetupL, UT_CMusManagerServerCore_IsMusEnginePluginL, Teardown)

EUNIT_END_TEST_TABLE
