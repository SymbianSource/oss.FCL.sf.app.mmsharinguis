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
#include "etelmm.h"
#include "e32property.h"
#include "centralrepository.h"
#include "ut_mustsypropertymonitor.h"
#include "mustsypropertymonitor.h"
#include "muscallmonitor.h"
#include "musvoipcallmonitor.h"
#include "muscallconferencemonitor.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <CTSYDomainPSKeys.h>
#include <telremotepartyinformationpskeys.h>


_LIT(KTelNo,"1234");
_LIT( KTestProvideName, "VoIP" );
_LIT( KDefaultSipUri, "sip:default@uri" );

TName cs_call_1(_L("cs_call_1"));
TName cs_call_2(_L("cs_call_2"));
TName voip_call_1(_L("voip_call_1"));
TName voip_call_2(_L("voip_call_2"));

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusTsyPropertyMonitor* UT_CMusTsyPropertyMonitor::NewL()
    {
    UT_CMusTsyPropertyMonitor* self = UT_CMusTsyPropertyMonitor::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusTsyPropertyMonitor* UT_CMusTsyPropertyMonitor::NewLC()
    {
    UT_CMusTsyPropertyMonitor* self = new( ELeave ) UT_CMusTsyPropertyMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusTsyPropertyMonitor::~UT_CMusTsyPropertyMonitor()
    {
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::SetupL()
    {
    iMusCentralRepository = CRepository::NewL( MusSettingsKeys::KRepositoryUid );
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     0 );
    
    iMusTsyPropertyMonitor = CMusTsyPropertyMonitor::NewL( iPhone, *this );
    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateUninitialized;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::Teardown()
    {
    delete iMusCentralRepository;
    iMusCentralRepository = NULL;
    delete iMusTsyPropertyMonitor;
    PropertyHelper::Close();
    iPhone.Close();

    }



// TEST CASES

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_RunLL()
    {

    // Set intial call type too unintialiazed.
    if ( RProperty::Set( KPSUidCtsyCallInformation,KCTsyCallType,
                             EPSCTsyCallTypeUninitialized ))
        {
        User::Leave( KErrNoMemory );
        }
    // Set intial call count
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                         NMusSessionInformationApi::KMusCallCount,
                         0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    
    EUNIT_ASSERT( iMusTsyPropertyMonitor);
    TInt val = KErrNone;
    
    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );

    
    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateNone;
    iMusTsyPropertyMonitor->SetStateL( NMusSessionInformationApi::ENoCall );

    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);
    

    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateDisconnecting;
    iMusTsyPropertyMonitor->SetStateL( NMusSessionInformationApi::ENoCall );

    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);

    
    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateAnswering;
    iMusTsyPropertyMonitor->SetStateL( NMusSessionInformationApi::ENoCall );

    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);

    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateConnected;
    iMusTsyPropertyMonitor->SetStateL( NMusSessionInformationApi::ENoCall );

    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ENoCall);

    iMusTsyPropertyMonitor->iPropertyEvent.iPValue = EPSCTsyCallStateHold;
    iMusTsyPropertyMonitor->SetStateL( NMusSessionInformationApi::ENoCall );
	if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     1 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    //simulate multicall scenario
    TName callName(_L("Voice1"));
    iMusTsyPropertyMonitor->AddCallMonitorL( callName );
    callName.Copy(_L("Voice2"));
    iMusTsyPropertyMonitor->AddCallMonitorL( callName );
    iMusTsyPropertyMonitor->Cancel();
    iMusTsyPropertyMonitor->RunL();
    //
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     val );
    EUNIT_ASSERT( val==NMusSessionInformationApi::ECallHold);
    
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_DoCancelL()
    {
    EUNIT_ASSERT( iMusTsyPropertyMonitor );
    iMusTsyPropertyMonitor->Cancel();
    EUNIT_ASSERT( PropertyHelper::GetCalledFunction() == 
                                        RProperty::ECancel )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_RunErrorL()
    {
    EUNIT_ASSERT( iMusTsyPropertyMonitor );
    TInt error = KErrNotFound;
    iMusTsyPropertyMonitor->RunError(error);
    }



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_AddCallMonitorLL()
    {
    TName name = KTelNo();
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    iMusTsyPropertyMonitor->AddCallMonitorL( name );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_RemoveCallMonitorL()
    {
    TName name = KTelNo();
    iMusTsyPropertyMonitor->RemoveCallMonitor( name );
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_MonitorCSCallLL()
    {
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMusCallCount,
                            0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusIdle );
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );    
    iMusTsyPropertyMonitor->MonitorCSCallL();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    
    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusHangingUp );    
    iMusTsyPropertyMonitor->MonitorCSCallL();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    
    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusConnected );    
    iMusTsyPropertyMonitor->MonitorCSCallL();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 1 );
    
    iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusAnswering );    
    iMusTsyPropertyMonitor->MonitorCSCallL();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 1 );

    iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusIdle );
    iMusTsyPropertyMonitor->MonitorCSCallL();    
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_MonitorVoipCallLL()
    {    
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iVoipCallMonitorArray.Count() == 0 );
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    
    // State connected    
    TPSCTsyCallState callState = EPSCTsyCallStateConnected;    
    iMusTsyPropertyMonitor->MonitorVoipCallL( callState );
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iVoipCallMonitorArray.Count() == 1 );
    
    // State answering
    iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iVoipCallMonitorArray.Count() == 0 );
    callState = EPSCTsyCallStateAnswering ;    
    iMusTsyPropertyMonitor->MonitorVoipCallL( callState );
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iVoipCallMonitorArray.Count() == 1 );
    
    // State disconnecting ( Atleast one previous voip call is present )    
    callState = EPSCTsyCallStateDisconnecting;    
    iMusTsyPropertyMonitor->MonitorVoipCallL( callState );
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iVoipCallMonitorArray.Count() == 0 );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_MonitorCallLL()
    {
    TInt tmp = 0;
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                         NMusSessionInformationApi::KMusCallCount,
                         0 ) != KErrNone )
       {
       User::Leave( KErrNoMemory );
       }
    iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
    iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy();
    // Test : Cs call and call state is connected
    TPSCTsyCallState callState = EPSCTsyCallStateConnected;
    RTelHelper::SetCallStatus( (RMobileCall::TMobileCallStatus)RCall::EStatusConnected );    
    TPSCTsyCallType callType = EPSCTsyCallTypeCSVoice;
    iMusTsyPropertyMonitor->MonitorCallL(callState,callType);    
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 1 );
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iVoipCallMonitorArray.Count() == 0 );
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusCallEvent,
                          tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallConnected);  
    
    // Test : unknown call type   
    iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();              
    callType = EPSCTsyCallTypeNone;
    iMusTsyPropertyMonitor->MonitorCallL(callState,callType);    
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iVoipCallMonitorArray.Count() == 0 );
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusCallEvent,
                          tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ENoCall);    
    
    // Test : Voip call and call state is connected and factory settings not found  
    // if key not found then it should behave default = AllowedAllCalls.    
    callState = EPSCTsyCallStateConnected;      
    callType = EPSCTsyCallTypeVoIP;
    iMusTsyPropertyMonitor->MonitorCallL(callState,callType);    
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iVoipCallMonitorArray.Count() == 1 );
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusCallEvent,
                          tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallConnected);    
    
    
    // Test : Voip call and call state is connected and factory settings set to 
    // all call supported.
    iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy();
    iMusCentralRepository->Set(MusSettingsKeys::KAllowOnlyWithActiveCSCall,
                                                      MusSettingsKeys::EAllowedAllCalls);
    callState = EPSCTsyCallStateConnected;      
    callType = EPSCTsyCallTypeVoIP;
    iMusTsyPropertyMonitor->MonitorCallL(callState,callType);    
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iVoipCallMonitorArray.Count() == 1 );
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusCallEvent,
                          tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallConnected);    
        
    // Test : Voip call and call state is connected but factory settings are cs only
    iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy();
    iMusCentralRepository->Set(MusSettingsKeys::KAllowOnlyWithActiveCSCall,MusSettingsKeys::EAllowedCSOnly);    
    callState = EPSCTsyCallStateConnected;      
    callType = EPSCTsyCallTypeVoIP;
    iMusTsyPropertyMonitor->MonitorCallL(callState,callType);    
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count() == 0 );
    if ( iMusTsyPropertyMonitor->iVoipCallMonitorArray.Count() != 0 )
        {
        User::Leave( KErrNoMemory ); // Call was not removed because of memory running out was "silenty" handled.
        }
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iVoipCallMonitorArray.Count() == 0 ); 
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMusCallEvent,
                          tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ENoCall);    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_RemoveUnUsedCallMonitorsL()
    {
    TName callName(_L("Voice1"));
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    iMusTsyPropertyMonitor->AddCallMonitorL( callName );
    callName.Copy(_L("Voice2"));
    iMusTsyPropertyMonitor->AddCallMonitorL( callName );
    RTelHelper::SetCallStatus(RMobileCall::EStatusUnknown);
    iMusTsyPropertyMonitor->RemoveUnUsedCallMonitors();
    EUNIT_ASSERT( iMusTsyPropertyMonitor->iCallMonitorArray.Count()==0 );    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_CheckCallCriteriaLL()
    {
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMusCallCount,
                            0 ) != KErrNone )
          {
          User::Leave( KErrNoMemory );
          }
    TInt tmp = 0;
    iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
    iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy();

    User::LeaveIfError( RProperty::Set( KPSUidTelRemotePartyInformation,
                                        KTelCLINumber,
                                        _L( "sip:somebody@somewhere" ) ) );
   
    // Test : no cs call , one voip call    
    iMusTsyPropertyMonitor->AddVoipCallMonitorL( voip_call_1 );
    iMusTsyPropertyMonitor->CheckCallCriteriaL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                             NMusSessionInformationApi::KMusCallEvent,
                             tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallConnected);
    // Test : one cs call , multiple voip calls
    iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
    iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy();
    iMusTsyPropertyMonitor->AddCallMonitorL( cs_call_1 );
    iMusTsyPropertyMonitor->AddVoipCallMonitorL( voip_call_1 );
    iMusTsyPropertyMonitor->AddVoipCallMonitorL( voip_call_2 );
    iMusTsyPropertyMonitor->CheckCallCriteriaL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                                 NMusSessionInformationApi::KMusCallEvent,
                                 tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallHold);
    // Test: multiple cs call, one voip call
    iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
    iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy();
    iMusTsyPropertyMonitor->AddCallMonitorL( cs_call_1 );
    iMusTsyPropertyMonitor->AddCallMonitorL( cs_call_2 );
    iMusTsyPropertyMonitor->AddVoipCallMonitorL( voip_call_1 );
    iMusTsyPropertyMonitor->CheckCallCriteriaL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                                 NMusSessionInformationApi::KMusCallEvent,
                                 tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallHold);    
    
    // Test : multiple cs call , multiple voip calls
    iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
    iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy();  
    iMusTsyPropertyMonitor->AddCallMonitorL( cs_call_1 );
    iMusTsyPropertyMonitor->AddCallMonitorL( cs_call_2 );
    iMusTsyPropertyMonitor->AddVoipCallMonitorL( voip_call_1 );
    iMusTsyPropertyMonitor->AddVoipCallMonitorL( voip_call_2 );
    iMusTsyPropertyMonitor->CheckCallCriteriaL();
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                                  NMusSessionInformationApi::KMusCallEvent,
                                  tmp );
     EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallHold);       
    // Test : one cs call , one voip call
     iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
     iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy();  
     iMusTsyPropertyMonitor->AddCallMonitorL( cs_call_1 );     
     iMusTsyPropertyMonitor->AddVoipCallMonitorL( voip_call_1 );  
     iMusTsyPropertyMonitor->CheckCallCriteriaL();
     RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                                    NMusSessionInformationApi::KMusCallEvent,
                                    tmp );
     EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallHold);
     // Test: one cs call , no voip call
     iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
     iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy(); 
     RTelHelper::SetCallStatus( RMobileCall::EStatusConnected );
     iMusTsyPropertyMonitor->AddCallMonitorL( cs_call_1 );    
     iMusTsyPropertyMonitor->CheckCallCriteriaL();
     RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                         NMusSessionInformationApi::KMusCallEvent,
                         tmp );
     EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallConnected);
    }



// -----------------------------------------------------------------------------
// UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_IsDataReadyLL()
// Test to query from corresponding monitors if they are ready.
// -----------------------------------------------------------------------------
//


void UT_CMusTsyPropertyMonitor::UT_CMusTsyPropertyMonitor_IsDataReadyLL()
    {
    TBool dataReady = EFalse;

    // Test-1: CS Call Ready:
    iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
    iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy();

    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMusCallCount,
                            0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }

    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                            NMusSessionInformationApi::KMusTelNumber,
                                            KTelNo ));

    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                            NMusSessionInformationApi::KMusCallDirection,
                                            NMusSessionInformationApi::ECallOrginated ));
    
    iMusTsyPropertyMonitor->AddCallMonitorL( cs_call_1 );
    dataReady = iMusTsyPropertyMonitor->IsDataReadyL();
    EUNIT_ASSERT_EQUALS( dataReady, ETrue )
    
    // Test-2: CS Call NotReady: 
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                            NMusSessionInformationApi::KMusTelNumber,
                                            KNullDesC ));
    
    dataReady = iMusTsyPropertyMonitor->IsDataReadyL();
    EUNIT_ASSERT_EQUALS( dataReady, EFalse )

    // Test-3 VoIP Call Ready
    iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
    iMusTsyPropertyMonitor->AddVoipCallMonitorL( voip_call_1 );
    
    User::LeaveIfError( RProperty::Set(
                              NMusSessionInformationApi::KCategoryUid,
                              NMusSessionInformationApi::KMusTelNumber, 
                              KDefaultSipUri ));

    User::LeaveIfError(RProperty::Set( 
                              NMusSessionInformationApi::KCategoryUid,
                              NMusSessionInformationApi::KMUSCallProvider,
                              KTestProvideName ));
    
    dataReady = iMusTsyPropertyMonitor->IsDataReadyL();
    EUNIT_ASSERT_EQUALS( dataReady, ETrue )
    
    
    // Test-4 VoIP Call Not Ready
    User::LeaveIfError( RProperty::Set(
                          NMusSessionInformationApi::KCategoryUid,
                          NMusSessionInformationApi::KMUSCallProvider, 
                          KNullDesC ));
    
    dataReady = iMusTsyPropertyMonitor->IsDataReadyL();

    EUNIT_ASSERT_EQUALS( dataReady, EFalse )

    // Test-5: Conference Case
    iMusTsyPropertyMonitor->iCallMonitorArray.ResetAndDestroy();
    iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy(); 

    iMusTsyPropertyMonitor->AddVoipCallMonitorL( voip_call_1 );
    iMusTsyPropertyMonitor->AddVoipCallMonitorL( voip_call_2 );
    
    dataReady = iMusTsyPropertyMonitor->IsDataReadyL();
    EUNIT_ASSERT_EQUALS( dataReady, EFalse )
    iMusTsyPropertyMonitor->iVoipCallMonitorArray.ResetAndDestroy();
    
    // Test-3 Handling conference case.
    iMusTsyPropertyMonitor->iConferenceMonitor->iConfStatus = RMobileConferenceCall::EConferenceActive;
    dataReady = iMusTsyPropertyMonitor->IsDataReadyL();
    EUNIT_ASSERT_EQUALS( dataReady, ETrue )
    iMusTsyPropertyMonitor->iConferenceMonitor->iConfStatus = RMobileConferenceCall::EConferenceIdle;
    }


// -----------------------------------------------------------------------------
//  MusCallStateChanged from the MusCallStateObserver 
// -----------------------------------------------------------------------------
//
void UT_CMusTsyPropertyMonitor::MusCallStateChanged()
    {
    // NOP
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusTsyPropertyMonitor,
    "UT_CMusTsyPropertyMonitor",
    "UNIT" )

EUNIT_TEST(
    "RunL - test",
    "CMusTsyPropertyMonitor",
    "RunL",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_RunLL, Teardown)

EUNIT_TEST(
    "DoCancel - test",
    "CMusTsyPropertyMonitor",
    "DoCancel",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_DoCancelL, Teardown)

EUNIT_TEST(
    "RunError - test",
    "CMusTsyPropertyMonitor",
    "RunError",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_RunErrorL, Teardown)

EUNIT_TEST(
    "UT_CMusTsyPropertyMonitor_RemoveUnUsedCallMonitorsL",
    "CMusCallStatusMonitor",
    "RemoveUnUsedCallMonitors",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_RemoveUnUsedCallMonitorsL, Teardown)

EUNIT_TEST(
    "AddCallMonitorL - test",
    "CMusTsyPropertyMonitor",
    "AddCallMonitorL",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_AddCallMonitorLL, Teardown)

EUNIT_TEST(
    "RemoveCallMonitor - test",
    "CMusTsyPropertyMonitor",
    "RemoveCallMonitor",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_RemoveCallMonitorL, Teardown)
    
EUNIT_TEST(
    "MonitorCallL - test",
    "CMusTsyPropertyMonitor",
    "MonitorCallL",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_MonitorCallLL, Teardown)
    
EUNIT_TEST(
    "MonitorCsCallL - test",
    "CMusTsyPropertyMonitor",
    "MonitorCsCallL",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_MonitorCSCallLL, Teardown)
    
EUNIT_TEST(
    "MonitorVoipCallL - test",
    "CMusTsyPropertyMonitor",
    "MonitorVoipCallL",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_MonitorVoipCallLL, Teardown)

EUNIT_TEST(
    "CheckCallCriteriaL - test",
    "CMusTsyPropertyMonitor",
    "CheckCallCriteriaL",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_CheckCallCriteriaLL, Teardown)
    
EUNIT_TEST(
    "IsDataReadyL - test",
    "CMusTsyPropertyMonitor",
    "IsDataReadyL",
    "FUNCTIONALITY",
    SetupL, UT_CMusTsyPropertyMonitor_IsDataReadyLL, Teardown)
    
EUNIT_END_TEST_TABLE

//  END OF FILE


