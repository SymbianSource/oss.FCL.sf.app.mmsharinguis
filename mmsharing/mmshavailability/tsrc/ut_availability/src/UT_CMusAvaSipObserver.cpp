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
#include "UT_CMusAvaSipObserver.h"
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
#include "musavacapabilitytesthelper.h" //remove compilation warning

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
//#include <telephonyinternalpskeys.h>
#include <sipprofileregistry.h>
#include <sip.h>


//  INTERNAL INCLUDES
#include "musavasipobserver.h"



// CONSTRUCTION
UT_CMusAvaSipObserver* UT_CMusAvaSipObserver::NewL()
    {
    UT_CMusAvaSipObserver* self = UT_CMusAvaSipObserver::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaSipObserver* UT_CMusAvaSipObserver::NewLC()
    {
    UT_CMusAvaSipObserver* self = new( ELeave ) UT_CMusAvaSipObserver();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaSipObserver::~UT_CMusAvaSipObserver()
    {
    }

// Default constructor
UT_CMusAvaSipObserver::UT_CMusAvaSipObserver()
    {
    }

// Second phase construct
void UT_CMusAvaSipObserver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaSipObserver::SetupL(  )
    { 
    SIPStrings::OpenL();
    iAdapterImp = CMusAvaSipAdapterImp::NewL();
    iAdapterImp2 = CMusAvaSipAdapterImp::NewL();
    iAdapterImp3 = CMusAvaSipAdapterImp::NewL();

    iMusAvaSipObserver = CMusAvaSipObserver::NewL();	
    } 

void UT_CMusAvaSipObserver::Teardown(  )
    {
    delete iMusAvaSipObserver;
    iMusAvaSipObserver = NULL; 
    delete iAdapterImp;
    iAdapterImp = NULL;
    delete iAdapterImp2;
    iAdapterImp2 = NULL;
    delete iAdapterImp3;
    iAdapterImp3 = NULL;
    SIPStrings::Close();
    }

void UT_CMusAvaSipObserver::AddObserversL()
    {
    iMusAvaSipObserver->AddObserverL( *iAdapterImp, KErrNotFound );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers.Count() == 1 );
    
    // Do it twice, duplicates are ignored
    iMusAvaSipObserver->AddObserverL( *iAdapterImp, KErrNotFound );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers.Count() == 1 );
    }

void UT_CMusAvaSipObserver::UT_CMusAvaSipObserver_AddObserverL()
    {
    iMusAvaSipObserver->AddObserverL( *iAdapterImp, KErrNotFound );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers.Count() == 1 );
    
    // Do it twice, duplicates are ignored
    iMusAvaSipObserver->AddObserverL( *iAdapterImp, KErrNotFound );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers.Count() == 1 );

    iMusAvaSipObserver->AddObserverL( *iAdapterImp2, 0 );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers.Count() == 2 );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers[0] == iAdapterImp2 );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers[1] == iAdapterImp );

    iMusAvaSipObserver->iObservers.Reset();
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers.Count() == 0 );
    
    iMusAvaSipObserver->AddObserverL( *iAdapterImp, KErrNotFound );     // invite handler
    iMusAvaSipObserver->AddObserverL( *iAdapterImp2, KErrNotFound );    // default option handler
    iMusAvaSipObserver->AddObserverL( *iAdapterImp3, 1 ); // option handler
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers[0] == iAdapterImp );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers[1] == iAdapterImp3 );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers[2] == iAdapterImp2 );
    
    iMusAvaSipObserver->iObservers.Remove(2);
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers.Count() == 2 );

    iMusAvaSipObserver->AddObserverL( *iAdapterImp2, 10 );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers.Count() == 3 );

    
    } 

void UT_CMusAvaSipObserver::UT_CMusAvaSipObserver_RemoveObserverL()
    {
    AddObserversL();
    iMusAvaSipObserver->RemoveObserver( *iAdapterImp );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers.Count() == 0 );
    iMusAvaSipObserver->RemoveObserver( *iAdapterImp );
    EUNIT_ASSERT( iMusAvaSipObserver->iObservers.Count() == 0 );
    } 
        
void UT_CMusAvaSipObserver::UT_CMusAvaSipObserver_IncomingRequestL()
    {
    AddObserversL();
    
    // Ok
    TUint32 iapId( 6 );
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    iMusAvaSipObserver->IncomingRequest( iapId, serverTransaction );
    EUNIT_ASSERT( iAdapterImp->iServerTransaction == serverTransaction );
    EUNIT_ASSERT( iAdapterImp->iIapId == iapId );
    
    // Not ok
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
    CSIPServerTransaction* serverTransaction2 = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    iMusAvaSipObserver->IncomingRequest( iapId, serverTransaction2 );
    EUNIT_ASSERT( iAdapterImp->iServerTransaction == NULL );
    
    }  
    
void UT_CMusAvaSipObserver::UT_CMusAvaSipObserver_TimedOutL()
    {
    AddObserversL();
    
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );
    iMusAvaSipObserver->TimedOut( *serverTransaction );
    EUNIT_ASSERT( iAdapterImp->iServerTransaction == serverTransaction );
    
    // Not ok
    iAdapterImp->Reset();
    iAdapterImp->iReturnCode = KErrGeneral;
      iMusAvaSipObserver->TimedOut( *serverTransaction );
    EUNIT_ASSERT( iAdapterImp->iServerTransaction == NULL );
    CleanupStack::PopAndDestroy( serverTransaction );
    } 

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaSipObserver,
    "CMusAvaSipObserver",
    "UNIT" )
       
EUNIT_TEST(
    "AddObserverL - test ",
    "CMusAvaSipObserver",
    "AddObserverL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipObserver_AddObserverL, Teardown)

       
EUNIT_TEST(
    "RemoveObserverL - test ",
    "CMusAvaSipObserver",
    "RemoveObserverL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipObserver_RemoveObserverL, Teardown)
           
EUNIT_TEST(
    "IncomingRequestL - test ",
    "CMusAvaSipObserver",
    "IncomingRequestL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipObserver_IncomingRequestL, Teardown)

       
EUNIT_TEST(
    "TimedOutL - test ",
    "CMusAvaSipObserver",
    "TimedOutL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipObserver_TimedOutL, Teardown)
       
EUNIT_END_TEST_TABLE

//  END OF FILE
