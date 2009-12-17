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
#include "UT_CMusAvaSipConnectionObserver.h"
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
#include "musavasipadapterimp.h"
#include "musavasipconnectionobserver.h"
#include "musavacapabilitytesthelper.h" //remove compilation warning

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
//#include <telephonyinternalpskeys.h>
#include <sipprofileregistry.h>
#include <sip.h>
#include <siprefresh.h>

//  INTERNAL INCLUDES
#include "musavasipobserver.h"



// CONSTRUCTION
UT_CMusAvaSipConnectionObserver* UT_CMusAvaSipConnectionObserver::NewL()
    {
    UT_CMusAvaSipConnectionObserver* self = UT_CMusAvaSipConnectionObserver::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaSipConnectionObserver* UT_CMusAvaSipConnectionObserver::NewLC()
    {
    UT_CMusAvaSipConnectionObserver* self = new( ELeave ) UT_CMusAvaSipConnectionObserver();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaSipConnectionObserver::~UT_CMusAvaSipConnectionObserver()
    {
    }

// Default constructor
UT_CMusAvaSipConnectionObserver::UT_CMusAvaSipConnectionObserver()
    {
    }

// Second phase construct
void UT_CMusAvaSipConnectionObserver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaSipConnectionObserver::SetupL(  )
    { 
    SIPStrings::OpenL();
    iAdapterImp = CMusAvaSipAdapterImp::NewL();
    iAdapterImp2 = CMusAvaSipAdapterImp::NewL();
    iAdapterImp3 = CMusAvaSipAdapterImp::NewL();
    iMusAvaSipConnectionObserver = CMusAvaSipConnectionObserver::NewL();	
    } 
        
void UT_CMusAvaSipConnectionObserver::Teardown(  )
    {
    delete iMusAvaSipConnectionObserver;
    iMusAvaSipConnectionObserver = NULL; 
    delete iAdapterImp;
    iAdapterImp = NULL;
    delete iAdapterImp2;
    iAdapterImp2 = NULL;
    delete iAdapterImp3;
    iAdapterImp3 = NULL;
    SIPStrings::Close();
    }

void UT_CMusAvaSipConnectionObserver::RemoveDefaultHandler()
    {
    TInt defaultHandlerIndex( iMusAvaSipConnectionObserver->iObservers.Count() - 1 );
    MMusAvaSipConnectionAdapter* defaultHandler = 
        iMusAvaSipConnectionObserver->iObservers[ defaultHandlerIndex ];
    iMusAvaSipConnectionObserver->iObservers.Remove( defaultHandlerIndex );
    delete iMusAvaSipConnectionObserver->iDefaultHandler;
    iMusAvaSipConnectionObserver->iDefaultHandler = NULL;
    }
    
void UT_CMusAvaSipConnectionObserver::AddObserversL()
    {
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers.Count() == 1 ); // default observer
    iMusAvaSipConnectionObserver->AddObserverL( *iAdapterImp, KErrNotFound );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers.Count() == 2 );
    
    }

void UT_CMusAvaSipConnectionObserver::UT_CMusAvaSipConnectionObserver_AddObserverL()
    {
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers.Count() == 1 ); // default observer
    iMusAvaSipConnectionObserver->AddObserverL( *iAdapterImp, KErrNotFound );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers.Count() == 2 );
    
    iMusAvaSipConnectionObserver->AddObserverL( *iAdapterImp, KErrNotFound );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers.Count() == 2 );

    iMusAvaSipConnectionObserver->AddObserverL( *iAdapterImp2, 0 );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers.Count() == 3 );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers[0] == iAdapterImp2 );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers[1] == iAdapterImp );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers[2] != NULL );

    iMusAvaSipConnectionObserver->iObservers.Remove(1);
    iMusAvaSipConnectionObserver->iObservers.Remove(0);
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers[0] != iAdapterImp );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers[0] != iAdapterImp2 );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers[0] != NULL );
    
    iMusAvaSipConnectionObserver->AddObserverL( *iAdapterImp, KErrNotFound );     // invite handler
    iMusAvaSipConnectionObserver->AddObserverL( *iAdapterImp2, KErrNotFound );    // default option handler
    iMusAvaSipConnectionObserver->AddObserverL( *iAdapterImp3, 1 ); // option handler
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers[0] == iAdapterImp );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers[1] == iAdapterImp3 );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers[2] == iAdapterImp2 );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers[3] != NULL );
    
    iMusAvaSipConnectionObserver->iObservers.Remove(2);
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers.Count() == 3 );

    iMusAvaSipConnectionObserver->AddObserverL( *iAdapterImp2, 10 );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers.Count() == 4 );

    } 

void UT_CMusAvaSipConnectionObserver::UT_CMusAvaSipConnectionObserver_RemoveObserverL()
    {
    AddObserversL();
    iMusAvaSipConnectionObserver->RemoveObserver( *iAdapterImp );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers.Count() == 1 );
    iMusAvaSipConnectionObserver->RemoveObserver( *iAdapterImp );
    EUNIT_ASSERT( iMusAvaSipConnectionObserver->iObservers.Count() == 1 );
    } 

void UT_CMusAvaSipConnectionObserver::UT_CMusAvaSipConnectionObserver_ConnectionStateChangedL()
    {
    AddObserversL();
    
    RemoveDefaultHandler();
    
    iMusAvaSipConnectionObserver->ConnectionStateChanged( CSIPConnection::EActive );
    EUNIT_ASSERT( iAdapterImp->iState == CSIPConnection::EActive );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->ConnectionStateChanged( CSIPConnection::EActive );
    EUNIT_ASSERT( iAdapterImp->iState != CSIPConnection::EActive );
    }

void UT_CMusAvaSipConnectionObserver::UT_CMusAvaSipConnectionObserver_ErrorOccuredL()
    {
    AddObserversL();
    
    RemoveDefaultHandler();
    
    // ErrorOccured variant 1
    //
    CSIPClientTransaction* clientTransaction = 
        CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( clientTransaction );
    TInt regBindingFake( 3 );
    CSIPRegistrationBinding* regBinding = reinterpret_cast<CSIPRegistrationBinding*>( &regBindingFake );
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *clientTransaction, *regBinding );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNotFound );
    EUNIT_ASSERT( iAdapterImp->iClientTransaction == clientTransaction );
    EUNIT_ASSERT( iAdapterImp->iRegBinding == regBinding );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *clientTransaction, *regBinding );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNone );
    
    // ErrorOccured variant 2
    //
    iAdapterImp->Reset();
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );
    TInt dialogAssocFake( 4 );
    CSIPInviteDialogAssoc* dialogAssoc = reinterpret_cast<CSIPInviteDialogAssoc*>( &dialogAssocFake );
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *dialogAssoc );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNotFound );
    EUNIT_ASSERT( iAdapterImp->iDialogAssoc == dialogAssoc );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *dialogAssoc );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNone );
    
    // ErrorOccured variant 3
    //
    iAdapterImp->Reset();
    TInt refreshFake( 5 );
    CSIPRefresh* refresh = reinterpret_cast<CSIPRefresh*>( &refreshFake );
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *refresh );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNotFound );
    EUNIT_ASSERT( iAdapterImp->iRefresh == refresh );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *refresh );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNone );
    
    // ErrorOccured variant 4
    //
    iAdapterImp->Reset();
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *regBinding );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNotFound );
    EUNIT_ASSERT( iAdapterImp->iRegBinding == regBinding );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *regBinding );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNone );
    
    // ErrorOccured variant 5
    //
    iAdapterImp->Reset();
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *clientTransaction );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNotFound );
    EUNIT_ASSERT( iAdapterImp->iTransaction == clientTransaction );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *clientTransaction );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNone );
    
    // ErrorOccured variant 6
    //
    iAdapterImp->Reset();
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *clientTransaction, *dialogAssoc );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNotFound );
    EUNIT_ASSERT( iAdapterImp->iTransaction == clientTransaction );
    EUNIT_ASSERT( iAdapterImp->iDialogAssoc == dialogAssoc );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->ErrorOccured( KErrNotFound, *clientTransaction, *dialogAssoc );
    EUNIT_ASSERT( iAdapterImp->iError == KErrNone );
    
    CleanupStack::PopAndDestroy( serverTransaction );
    CleanupStack::PopAndDestroy( clientTransaction );
    }

void UT_CMusAvaSipConnectionObserver::UT_CMusAvaSipConnectionObserver_IncomingRequestL()
    {
    AddObserversL();
    
    RemoveDefaultHandler();
    
    // Variant 1
    //
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );

    iMusAvaSipConnectionObserver->IncomingRequest( serverTransaction );
    EUNIT_ASSERT( iAdapterImp->iServerTransaction == serverTransaction );
    
    serverTransaction = NULL;
    serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->IncomingRequest( serverTransaction );
    EUNIT_ASSERT( iAdapterImp->iServerTransaction == NULL );
    delete serverTransaction;
    
    // Variant 2
    //
    serverTransaction = NULL;
    serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    iAdapterImp->Reset();
    TInt dialogFake( 4 );
    CSIPDialog* dialog = reinterpret_cast<CSIPDialog*>( &dialogFake );
    iMusAvaSipConnectionObserver->IncomingRequest( serverTransaction, *dialog );
    EUNIT_ASSERT( iAdapterImp->iServerTransaction == serverTransaction );
    EUNIT_ASSERT( iAdapterImp->iDialog == dialog );
    
    serverTransaction = NULL;
    serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->IncomingRequest( serverTransaction, *dialog );
    EUNIT_ASSERT( iAdapterImp->iServerTransaction == NULL );
    delete serverTransaction;
    }

void UT_CMusAvaSipConnectionObserver::UT_CMusAvaSipConnectionObserver_IncomingResponseL()
    {
    AddObserversL();
    
    RemoveDefaultHandler();
    
    // Variant 1
    //
    CSIPClientTransaction* clientTransaction = 
        CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( clientTransaction );
    iMusAvaSipConnectionObserver->IncomingResponse( *clientTransaction );
    EUNIT_ASSERT( iAdapterImp->iClientTransaction == clientTransaction );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->IncomingResponse( *clientTransaction );
    EUNIT_ASSERT( iAdapterImp->iClientTransaction == NULL );
     
    // Variant 2
    //
    iAdapterImp->Reset();
    TInt dialogAssocFake( 4 );
    CSIPInviteDialogAssoc* dialogAssoc = reinterpret_cast<CSIPInviteDialogAssoc*>( &dialogAssocFake );
    iMusAvaSipConnectionObserver->IncomingResponse( *clientTransaction, *dialogAssoc );
    EUNIT_ASSERT( iAdapterImp->iClientTransaction == clientTransaction );
    EUNIT_ASSERT( iAdapterImp->iDialogAssoc == dialogAssoc );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->IncomingResponse( *clientTransaction, *dialogAssoc );
    EUNIT_ASSERT( iAdapterImp->iClientTransaction == NULL );
    
    // Variant 3
    //
    iAdapterImp->Reset();
    iMusAvaSipConnectionObserver->IncomingResponse( *clientTransaction, NULL );
    EUNIT_ASSERT( iAdapterImp->iClientTransaction == clientTransaction );
    EUNIT_ASSERT( iAdapterImp->iInviteAssoc == NULL );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->IncomingResponse( *clientTransaction, NULL );
    EUNIT_ASSERT( iAdapterImp->iClientTransaction == NULL );
    
    // Variant 4
    //
    iAdapterImp->Reset();
    TInt regBindingFake( 3 );
    CSIPRegistrationBinding* regBinding = reinterpret_cast<CSIPRegistrationBinding*>( &regBindingFake );
    iMusAvaSipConnectionObserver->IncomingResponse( *clientTransaction, *regBinding );
    EUNIT_ASSERT( iAdapterImp->iClientTransaction == clientTransaction );
    EUNIT_ASSERT( iAdapterImp->iRegBinding == regBinding );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->IncomingResponse( *clientTransaction, *regBinding );
    EUNIT_ASSERT( iAdapterImp->iClientTransaction == NULL );
    
    CleanupStack::PopAndDestroy( clientTransaction );
    }

void UT_CMusAvaSipConnectionObserver::UT_CMusAvaSipConnectionObserver_InviteCanceledL()
    {
    AddObserversL();
    
    RemoveDefaultHandler();
    
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );
        
    iMusAvaSipConnectionObserver->InviteCanceled( *serverTransaction );
    EUNIT_ASSERT( iAdapterImp->iServerTransaction == serverTransaction );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->InviteCanceled( *serverTransaction );
    EUNIT_ASSERT( iAdapterImp->iServerTransaction == NULL );
    
    CleanupStack::PopAndDestroy( serverTransaction );
    }

void UT_CMusAvaSipConnectionObserver::UT_CMusAvaSipConnectionObserver_InviteCompletedL()
    {
    AddObserversL();
    
    RemoveDefaultHandler();
    
    CSIPClientTransaction* clientTransaction = 
        CSIPClientTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( clientTransaction );
        
    iMusAvaSipConnectionObserver->InviteCompleted( *clientTransaction );
    EUNIT_ASSERT( iAdapterImp->iClientTransaction == clientTransaction );
    
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    iMusAvaSipConnectionObserver->InviteCompleted( *clientTransaction );
    EUNIT_ASSERT( iAdapterImp->iClientTransaction == NULL );
    
    CleanupStack::PopAndDestroy( clientTransaction );
    }
    
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaSipConnectionObserver,
    "CMusAvaSipConnectionObserver",
    "UNIT" )
       
EUNIT_TEST(
    "AddObserverL - test ",
    "CMusAvaSipConnectionObserver",
    "AddObserverL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipConnectionObserver_AddObserverL, Teardown)

EUNIT_TEST(
    "RemoveObserverL - test ",
    "CMusAvaSipConnectionObserver",
    "RemoveObserverL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipConnectionObserver_RemoveObserverL, Teardown)

EUNIT_TEST(
    "ConnectionStateChangedL - test ",
    "CMusAvaSipConnectionObserver",
    "ConnectionStateChangedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipConnectionObserver_ConnectionStateChangedL, Teardown)
    
EUNIT_TEST(
    "ErrorOccuredL - test ",
    "CMusAvaSipConnectionObserver",
    "ErrorOccuredL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipConnectionObserver_ErrorOccuredL, Teardown)
    
EUNIT_TEST(
    "IncomingRequestL - test ",
    "CMusAvaSipConnectionObserver",
    "IncomingRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipConnectionObserver_IncomingRequestL, Teardown)

EUNIT_TEST(
    "IncomingResponseL - test ",
    "CMusAvaSipConnectionObserver",
    "IncomingResponseL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipConnectionObserver_IncomingResponseL, Teardown)

EUNIT_TEST(
    "InviteCanceledL - test ",
    "CMusAvaSipConnectionObserver",
    "InviteCanceledL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipConnectionObserver_InviteCanceledL, Teardown)

EUNIT_TEST(
    "InviteCompletedL - test ",
    "CMusAvaSipConnectionObserver",
    "InviteCompletedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipConnectionObserver_InviteCompletedL, Teardown)
                                
EUNIT_END_TEST_TABLE

//  END OF FILE
