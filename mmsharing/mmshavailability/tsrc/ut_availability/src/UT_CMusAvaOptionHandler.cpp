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
#include "UT_CMusAvaOptionHandler.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <digia/eunit/eunitdecorators.h>
#include <EscapeUtils.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <stringpool.h>
#include <sipservertransaction.h>
#include <siptransactionbase.h>
//  INTERNAL INCLUDES
#include "musavaoptionhandler.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "musavasettingsimp.h"
#include "musavaobserverimp.h"
#include "muslogger.h"
#include "musavasharedobject.h"
#include "mussettingskeys.h"
#include "mussettings.h"
#include "musavasip.h"
#include "CSipSseTestTls.h"
#include "musavacapabilitytesthelper.h"
#include "musavasettingsobserverimp.h"


// CONSTRUCTION
UT_CMusAvaOptionHandler* UT_CMusAvaOptionHandler::NewL()
    {
    UT_CMusAvaOptionHandler* self = UT_CMusAvaOptionHandler::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaOptionHandler* UT_CMusAvaOptionHandler::NewLC()
    {
    UT_CMusAvaOptionHandler* self = new( ELeave ) UT_CMusAvaOptionHandler();
    CleanupStack::PushL( self );
	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaOptionHandler::~UT_CMusAvaOptionHandler()
    {
    }

// Default constructor
UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler()
    {
    }

// Second phase construct
void UT_CMusAvaOptionHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMusAvaOptionHandler::SetupL(  )
    {
    iSharedObject = CMusAvaSharedObject::GetSingletonL();
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iSettingsObserver = CMusAvaSettingsObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iConcreteSettings->SetObserver( *iSettingsObserver );
    iOptionHandler = CMusAvaOptionHandler::NewL( *iAvailabilityObserver,
                                    *iConcreteSettings );

    MSIPConnectionObserver& connectionObserver = 
                            iSharedObject->MusAvaSip().ConnectionObserver();
    CSIP& sip = iSharedObject->MusAvaSip().Sip();
    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
    iStorage->Set( MusSettingsKeys::KFastStartupMode, 
        MusSettingsKeys::EFastModeOff );

    iSharedObject->MusAvaSip().CreateProfileL();
    iProfile = iSharedObject->MusAvaSip().Profile();
    iSharedObject->MusAvaSip().ConnectionL( );

    if ( !iProfile->iArray )
        {
        iProfile->iArray = new ( ELeave ) CDesC8ArrayFlat( 1 );
        iProfile->iArray->AppendL( _L8("sip:user@domain.com") );
        }
    if ( !iProfile->iRegisteredContact )
        {
        iProfile->iRegisteredContact = KRegisteredContact().AllocL();
        }

    MultimediaSharingSettings::SetCapabilityQuerySettingL(
                                                MusSettingsKeys::ESequential );
    } 

void UT_CMusAvaOptionHandler::Teardown(  )
    {
    CMusAvaSharedObject::DeleteSingleton();
    delete iAvailabilityObserver;
    delete iConcreteSettings;
    if ( iStorage )
        {
        iStorage->Clear();
        CSipSseTestTls::Close();
        iStorage = NULL;
        }
    delete iOptionHandler;
    delete iSettingsObserver;
    
    PropertyHelper::Close();
    }

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_NewLL(  )
    {
    EUNIT_ASSERT( iOptionHandler );
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaStatusNotExecuted );
    }

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_DoExecuteLL()
    {   
    // test for B-party && Cingular variant
    iOptionHandler->iSettings.SetCallDirection( 2 );
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EOperatorSpecific );
    iOptionHandler->DoExecuteL();
    EUNIT_ASSERT( iOptionHandler->State() ==  
                            MMusAvaObserver::EMusAvaStatusOptionsNotSent ); 
    iOptionHandler->iSettings.SetCallDirection( 1 );
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                        MusSettingsKeys::EStandard );
                            
    // usual case    
    TBufC16<16> str(KCapabilityTestSIPAddressUri_B);
    HBufC16* ptr;
    ptr = str.AllocLC(); 
    CDesCArrayFlat* sipAddress = new( ELeave ) CDesCArrayFlat( 1 );
    CleanupStack::PushL( sipAddress );
    sipAddress->AppendL( *ptr );
    iOptionHandler->iSettings.SetSipAddressesL( *sipAddress );
    sipAddress->Reset();
    CleanupStack::PopAndDestroy( sipAddress );
    CleanupStack::PopAndDestroy(); //ptr   
    
    MultimediaSharingSettings::SetCapabilityQuerySettingL(
                                                MusSettingsKeys::ESequential );
    EUNIT_ASSERT( iSharedObject->MusAvaSip().CreateProfileL() == ETrue );
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    TRAPD( error, iOptionHandler->DoExecuteL() );
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    EUNIT_ASSERT( error == KErrNoMemory || error == KErrNone );
    if ( error == KErrNoMemory ) User::Leave( error );
        
    if ( count > 0 )
        {
        EUNIT_ASSERT( iOptionHandler->State() ==  
                            MMusAvaObserver::EMusAvaOptionNotAvailable ); 
        }
    else
        {
        EUNIT_ASSERT( iOptionHandler->State() ==  
                            MMusAvaObserver::EMusAvaStatusOptionsSent ); 
    
        EUNIT_ASSERT( iStorage->iClientRequest->Method() == 
                            SIPStrings::StringF( SipStrConsts::EOptions ) );
                            
        EUNIT_ASSERT( 
            iStorage->iClientRequest->MessageElements().Content().Length() > 0 );
        EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().ContentType() );
        EUNIT_ASSERT( iStorage->iClientRequest->RemoteUri().Uri().UriDes().Compare( 
            KCapabilityTestSIPAddressUri8_B ) == 0);
        EUNIT_ASSERT( iStorage->iClientRequest->MessageElements().UserHeaders().
            Count() == 6 );
        MUS_HEADER_ASSERT( iStorage->iClientRequest, 
                    0, 
                    KCapabilityTestAcceptContactHeader );
        MUS_HEADER_ASSERT( iStorage->iClientRequest, 
                    5, 
                    KCapabilityTestContactHeader_A1Updated );
        }

    // when needed sip profile and not registered yet. means sip connection is still
    // null
    MultimediaSharingSettings::SetCapabilityQuerySettingL(
                                                MusSettingsKeys::ESequential );
    EUNIT_ASSERT( iSharedObject->MusAvaSip().CreateProfileL() == ETrue );
    MultimediaSharingSettings::SetSipProfileSettingL( 1 );
    iSharedObject->MusAvaSip().ConnectionL(EFalse);
    
    delete iSharedObject->MusAvaSip().iSipConnection;
    iSharedObject->MusAvaSip().iSipConnection = NULL;
    
    iOptionHandler->SetState( MMusAvaObserver::EMusAvaStatusOptionsNotSent );
    
    iOptionHandler->DoExecuteL();
    EUNIT_ASSERT( iOptionHandler->State() ==  
                            MMusAvaObserver::EMusAvaStatusNotExecuted );
    }

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_NameL()
    {
     EUNIT_ASSERT( iOptionHandler->Name() == 
                            MMusAvaObserver::EMusAvaOptionHandler );
    }
  

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_StatusL()
    {
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaStatusNotExecuted );
    }

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_StopL()
    {
    iOptionHandler->Stop();
    }
    
void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_CapabilitiesResolvedL()
    {
    _LIT( KSipAddress ,   "b@foobar.com" );
    _LIT( KSipAddressA ,   "a@foobar.com" );
    _LIT( KEmpty, "" );
    
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaStatusNotExecuted );
	iOptionHandler->CapabilitiesResolvedL( KEmpty );
	EUNIT_ASSERT( iOptionHandler->iSettings.SipAddresses().MdcaCount() == 0 );
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaStatusNotExecuted );
	
    iOptionHandler->iState = MMusAvaObserver::EMusAvaStatusNotExecuted;
	iOptionHandler->CapabilitiesResolvedL( KSipAddress );
    EUNIT_ASSERT( iOptionHandler->iSettings.SipAddresses().MdcaPoint( 0 ).
        Compare( KSipAddress() ) == KErrNone ); 
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaStatusNotExecuted );
        
    iOptionHandler->CapabilitiesResolvedL( KSipAddressA );    
    EUNIT_ASSERT( iOptionHandler->iSettings.SipAddresses().MdcaPoint( 0 ).
        Compare( KSipAddressA() ) == KErrNone ); 
                
    EUNIT_ASSERT( iOptionHandler->State() == 
                            MMusAvaObserver::EMusAvaStatusNotExecuted );
    }

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_CapabilitiesResolvedForCingularL()
    {
    EUNIT_ASSERT( iOptionHandler->CapabilitiesResolvedForCingular() == EFalse );
    iOptionHandler->SetCapabilitiesResolvedForCingular();
    EUNIT_ASSERT( iOptionHandler->CapabilitiesResolvedForCingular() == ETrue );
    }

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_SipHeadersL()
    {    
    CDesCArrayFlat* headers = new( ELeave ) CDesCArrayFlat( 1 );    
    CleanupStack::PushL(headers);
    headers->AppendL(_L("sip:test1@10.21.32.51"));
    iOptionHandler->SipHeadersL(*headers,SipStrConsts::EToHeader);    
    EUNIT_ASSERT( iOptionHandler->iSettings.SipAddresses().MdcaCount()==1 );

    _LIT( KPAssertedIdentityTelNumber, "+4455667788");
    headers->Reset();
    headers->AppendL(KPAssertedIdentity_B_16);
    headers->AppendL(KPAssertedIdentity_B1_16);
    iOptionHandler->SipHeadersL(*headers,SipStrConsts::EToHeader);    
    EUNIT_ASSERT( iOptionHandler->iSettings.SipAddresses().MdcaCount()==1 );
    EUNIT_ASSERT( iOptionHandler->iSettings.SipAddresses().MdcaPoint( 0 ).Compare( 
        KPAssertedIdentity_B_16() ) == KErrNone );
    EUNIT_ASSERT( iOptionHandler->iSettings.TelNumber().Compare( 
        KPAssertedIdentityTelNumber ) == 0 );

    headers->Reset();
    iOptionHandler->SipHeadersL(*headers,SipStrConsts::EToHeader);            
    EUNIT_ASSERT( iOptionHandler->iSettings.SipAddresses().MdcaCount()==0 );

    headers->Reset();
    headers->AppendL(_L("sip:test1@10.21.32.51"));
    iOptionHandler->SipHeadersL(*headers,SipStrConsts::EFromHeader);        
    EUNIT_ASSERT( iOptionHandler->iSettings.SipAddresses().MdcaCount()==0 );

    CleanupStack::PopAndDestroy(headers);
    
    
    }

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_VideoCodecsResolvedLL()
    {
    _LIT16( KVideoCodec , "H.264" );
    TBufC16<16> str(KVideoCodec);
    HBufC16* ptr;
    ptr = str.AllocLC();

    CDesCArrayFlat* videoCodecs = new( ELeave ) CDesCArrayFlat( 1 );
    CleanupStack::PushL( videoCodecs );

    videoCodecs->AppendL( *ptr );
    iOptionHandler->VideoCodecsResolvedL( *videoCodecs );
    EUNIT_ASSERT ( iOptionHandler->iSettings.iVideoCodecs->MdcaCount() > KErrNone );
    EUNIT_ASSERT ( iOptionHandler->iSettings.iVideoCodecs->MdcaPoint(0).Compare ( *ptr ) == 0 );

    videoCodecs->Reset();
    CleanupStack::PopAndDestroy( videoCodecs );
    CleanupStack::PopAndDestroy();
    
    }

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_DoSetStateL()
    {
    // Set some state which does not cause special handling
    iOptionHandler->iFastModeCapable = EFalse;
    iOptionHandler->DoSetState( MMusAvaObserver::EMusAvaStatusOptionsSent );
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaStatusOptionsSent );
    
    // Set special handling state, but fast mode is not enabled
    iOptionHandler->DoSetState( MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT( !iSettingsObserver->iApplicationStarted );
        
    // Set special handling state, fast capable but mode is not enabled
    iOptionHandler->iFastModeCapable = ETrue;
    iConcreteSettings->SetFastMode( MusSettingsKeys::EFastModeOff );
    iOptionHandler->DoSetState( MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT( !iSettingsObserver->iApplicationStarted );
    }

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_DoSetStateFastModeL()
    {
    iOptionHandler->iFastModeCapable = ETrue;
    iConcreteSettings->SetFastMode( MusSettingsKeys::EFastModeOn );
    iOptionHandler->iSettings.SetCallDirection( 1 );
    
    // State which does not have special handling
    iOptionHandler->DoSetState( MMusAvaObserver::EMusAvaFailureCode );
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaFailureCode );
    EUNIT_ASSERT( !iOptionHandler->iFastModeAvailabilityDelayed );
    
    // OptionsNotSent handling
    //
        
    // MT, not answered
    iOptionHandler->iSettings.SetCallDirection( 2 );
    iOptionHandler->iCapabilityQueryAnswered = EFalse;
    iOptionHandler->DoSetState( MMusAvaObserver::EMusAvaStatusOptionsNotSent );
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaStatusOptionsNotSent );
    EUNIT_ASSERT( !iOptionHandler->iFastModeAvailabilityDelayed );
        
    // MT, answered (makes state available delayed)
    iOptionHandler->iCapabilityQueryAnswered = ETrue;
    iOptionHandler->DoSetState( MMusAvaObserver::EMusAvaStatusOptionsNotSent );
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaStatusOptionsNotSent );
    EUNIT_ASSERT( iOptionHandler->iFastModeAvailabilityDelayed );
    
    // Available handling
    //
    
    // MO, start app
    iOptionHandler->iSettings.SetCallDirection( 1 );
    iSettingsObserver->iApplicationStarted = EFalse;
    iOptionHandler->iFastModeAvailabilityDelayed = EFalse;
    iOptionHandler->DoSetState( MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT( !iOptionHandler->iFastModeAvailabilityDelayed );
    EUNIT_ASSERT( iSettingsObserver->iApplicationStarted );
    
    // App already started by going to available state, do not do twice
    iSettingsObserver->iApplicationStarted = EFalse;
    iOptionHandler->DoSetState( MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT( !iOptionHandler->iFastModeAvailabilityDelayed );
    EUNIT_ASSERT( !iSettingsObserver->iApplicationStarted );
     
    // MT, delayed availability publishing (state not changed yet)
    iOptionHandler->iSettings.SetCallDirection( 2 );
    iOptionHandler->iFastModeAvailabilityDelayed = EFalse;
    iSettingsObserver->iApplicationStarted = EFalse;
    iOptionHandler->iState = MMusAvaObserver::EMusAvaStatusOptionsNotSent;
    iOptionHandler->DoSetState( MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaStatusOptionsNotSent );
    EUNIT_ASSERT( iOptionHandler->iFastModeAvailabilityDelayed );
    EUNIT_ASSERT( !iSettingsObserver->iApplicationStarted );
    
    // Invite ends availability delaying
    iOptionHandler->PrepareForReceivedInviteL();
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaStatusAvailable );
    EUNIT_ASSERT( !iOptionHandler->iFastModeAvailabilityDelayed );
    
    // No effect if no delay pending
    iOptionHandler->iState = MMusAvaObserver::EMusAvaStatusOptionsNotSent;
    iOptionHandler->PrepareForReceivedInviteL();
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaStatusOptionsNotSent );
    
    // OptionNotAvailable handling
    //
    
    // Normal handling when no pending availabilty publishing nor state is
    // not already available
    iOptionHandler->iFastModeAvailabilityDelayed = EFalse;
    iOptionHandler->DoSetState( MMusAvaObserver::EMusAvaOptionNotAvailable );
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaOptionNotAvailable );
    
    // If already set to available by succesfull answer, sending failure
    // does not clear availability
    iOptionHandler->iState = MMusAvaObserver::EMusAvaStatusAvailable;
    iOptionHandler->DoSetState( MMusAvaObserver::EMusAvaOptionNotAvailable );
    EUNIT_ASSERT( iOptionHandler->iState == MMusAvaObserver::EMusAvaStatusAvailable );
    }

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_FastModeResolvedL()
    {
    iOptionHandler->iFastModeCapable = ETrue;
    
    // Normal case  
    iOptionHandler->iSettings.SetFastMode( MusSettingsKeys::EFastModeOff );
    iOptionHandler->FastModeResolved( MusSettingsKeys::EFastModeOn );
    EUNIT_ASSERT( iOptionHandler->iSettings.FastMode() == MusSettingsKeys::EFastModeOn )
    
    // MO side fast mode negotiation has failed, value is not set
    iOptionHandler->iSettings.SetFastMode( MusSettingsKeys::EFastModeOff );
    iOptionHandler->iSettings.SetCallDirection( 1 );
    iOptionHandler->iState = MMusAvaObserver::EMusAvaOptionNotAvailable;
    iOptionHandler->FastModeResolved( MusSettingsKeys::EFastModeOn );
    EUNIT_ASSERT( iOptionHandler->iSettings.FastMode() == MusSettingsKeys::EFastModeOff )
    }

void UT_CMusAvaOptionHandler::UT_CMusAvaOptionHandler_CapabilityQueryAnsweredL()
    {
    iOptionHandler->iFastModeCapable = ETrue;
    
    // If fast mode MO side, no special handling for answer
    iOptionHandler->CapabilityQueryAnswered( ETrue );
    EUNIT_ASSERT( iOptionHandler->iCapabilityQueryAnswered )
    EUNIT_ASSERT( !iOptionHandler->iFastModeAvailabilityDelayed );
    
    // If fast mode MT side, MT can start waiting for invite already after answering
    iOptionHandler->iCapabilityQueryAnswered = EFalse;
    iOptionHandler->iSettings.SetFastMode( MusSettingsKeys::EFastModeOn );
    iOptionHandler->iSettings.SetCallDirection( 2 );
    iOptionHandler->CapabilityQueryAnswered( ETrue );
    EUNIT_ASSERT( iOptionHandler->iCapabilityQueryAnswered )
    EUNIT_ASSERT( iOptionHandler->iFastModeAvailabilityDelayed );
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaOptionHandler,
    "CMusAvaOptionHandler",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaOptionHandler",
    "NewL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_NewLL, Teardown)
    
EUNIT_TEST(
    "DoExecuteL - test ",
    "CMusAvaOptionHandler",
    "DoExecuteL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_DoExecuteLL, Teardown)        

EUNIT_TEST(
    "Name - test ",
    "CMusAvaOptionHandler",
    "Name",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_NameL, Teardown)
    

EUNIT_TEST(
    "Status - test ",
    "CMusAvaOptionHandler",
    "Status",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_StatusL, Teardown)

EUNIT_TEST(
    "Stop - test ",
    "CMusAvaOptionHandler",
    "Stop",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_StopL, Teardown)   
     
EUNIT_TEST(
    "CapabilitiesResolved - test ",
    "CMusAvaOptionHandler",
    "CapabilitiesResolved",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_CapabilitiesResolvedL, Teardown)   
     
EUNIT_TEST(
    "CapabilitiesResolvedForCingular - test ",
    "CMusAvaOptionHandler",
    "CapabilitiesResolvedForCingular",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_CapabilitiesResolvedForCingularL, 
    Teardown)   

EUNIT_TEST(
    "SipHeadersL - test ",
    "CMusAvaOptionHandler",
    "SipHeadersL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_SipHeadersL, 
    Teardown)  
     
EUNIT_TEST(
    "VideoCodecsResolvedL - test ",
    "CMusAvaOptionHandler",
    "VideoCodecsResolvedL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_VideoCodecsResolvedLL, 
    Teardown) 

EUNIT_TEST(
    "DoSetState - test ",
    "CMusAvaOptionHandler",
    "DoSetState",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_DoSetStateL, 
    Teardown) 

EUNIT_TEST(
    "DoSetState - fastmode test ",
    "CMusAvaOptionHandler",
    "DoSetState",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_DoSetStateFastModeL, 
    Teardown) 

EUNIT_TEST(
    "FastModeResolved - test ",
    "CMusAvaOptionHandler",
    "FastModeResolved",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_FastModeResolvedL, 
    Teardown) 

EUNIT_TEST(
    "CapabilityQueryAnswered - test ",
    "CMusAvaOptionHandler",
    "CapabilityQueryAnswered",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaOptionHandler_CapabilityQueryAnsweredL, 
    Teardown) 

EUNIT_END_TEST_TABLE

//  END OF FILE
