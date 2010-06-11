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
#include "UT_CMusAvaSipProfileRegistryObserver.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>

//  INTERNAL INCLUDES
#include "musavasipprofileregistryobserver.h"
#include "musavaprofileregistryadapterstub.h"

// CONSTRUCTION
UT_CMusAvaSipProfileRegistryObserver* UT_CMusAvaSipProfileRegistryObserver::NewL()
    {
    UT_CMusAvaSipProfileRegistryObserver* self = UT_CMusAvaSipProfileRegistryObserver::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaSipProfileRegistryObserver* UT_CMusAvaSipProfileRegistryObserver::NewLC()
    {
    UT_CMusAvaSipProfileRegistryObserver* self = new( ELeave ) UT_CMusAvaSipProfileRegistryObserver();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaSipProfileRegistryObserver::~UT_CMusAvaSipProfileRegistryObserver()
    {
    }

// Default constructor
UT_CMusAvaSipProfileRegistryObserver::UT_CMusAvaSipProfileRegistryObserver()
    {
    }

// Second phase construct
void UT_CMusAvaSipProfileRegistryObserver::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMusAvaSipProfileRegistryObserver::SetupL(  )
    {
    iSipprofileRegistryObserver = CMusAvaSipProfileRegistryObserver::NewL();
    } 

void UT_CMusAvaSipProfileRegistryObserver::Teardown(  )
    {
    delete iSipprofileRegistryObserver;
    }
    
void UT_CMusAvaSipProfileRegistryObserver::UT_CMusAvaSipProfileRegistryObserver_NewLL()
    {
    EUNIT_ASSERT( iSipprofileRegistryObserver );
    }
    	
void UT_CMusAvaSipProfileRegistryObserver::UT_CMusAvaSipProfileRegistryObserver_AddAdapterLL()
    {
    CMusAvaProfileRegistryAdapterStub* profileRegistryAdapter = CMusAvaProfileRegistryAdapterStub::NewLC();
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == NULL );
    iSipprofileRegistryObserver->AddAdapterL( *profileRegistryAdapter );
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == profileRegistryAdapter );
    iSipprofileRegistryObserver->AddAdapterL( *profileRegistryAdapter );
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == profileRegistryAdapter );
    iSipprofileRegistryObserver->iAdapter = NULL;
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == NULL );
    CleanupStack::PopAndDestroy();
    }

void UT_CMusAvaSipProfileRegistryObserver::UT_CMusAvaSipProfileRegistryObserver_RemoveAdapterL()
    {
    CMusAvaProfileRegistryAdapterStub* profileRegistryAdapter = CMusAvaProfileRegistryAdapterStub::NewLC();
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == NULL );
    iSipprofileRegistryObserver->AddAdapterL( *profileRegistryAdapter );
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == profileRegistryAdapter );
    CMusAvaProfileRegistryAdapterStub* profileRegistryAdapter2 = CMusAvaProfileRegistryAdapterStub::NewLC();
    iSipprofileRegistryObserver->AddAdapterL( *profileRegistryAdapter2 );
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == profileRegistryAdapter2 );
    iSipprofileRegistryObserver->RemoveAdapter( *profileRegistryAdapter );
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == profileRegistryAdapter2 );
    iSipprofileRegistryObserver->RemoveAdapter( *profileRegistryAdapter2 );
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == NULL );
    CleanupStack::PopAndDestroy(2);
    }

void UT_CMusAvaSipProfileRegistryObserver::UT_CMusAvaSipProfileRegistryObserver_ProfileRegistryEventOccurredL()
    {
    CMusAvaProfileRegistryAdapterStub* profileRegistryAdapter = CMusAvaProfileRegistryAdapterStub::NewLC();
    TUint32 profileId = 1;
    MSIPProfileRegistryObserver::TEvent event = MSIPProfileRegistryObserver::EProfileUpdated;
    iSipprofileRegistryObserver->ProfileRegistryEventOccurred( profileId, event );
    EUNIT_ASSERT( profileRegistryAdapter->iProfileId == 0 );
    EUNIT_ASSERT( profileRegistryAdapter->iEvent == MSIPProfileRegistryObserver::EProfileCreated );
    
    
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == NULL );
    iSipprofileRegistryObserver->AddAdapterL( *profileRegistryAdapter );
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == profileRegistryAdapter );
    iSipprofileRegistryObserver->ProfileRegistryEventOccurred( profileId, event );
    EUNIT_ASSERT( profileRegistryAdapter->iProfileId == profileId );
    EUNIT_ASSERT( profileRegistryAdapter->iEvent == event );
    CleanupStack::PopAndDestroy();
    } 
    
void UT_CMusAvaSipProfileRegistryObserver::UT_CMusAvaSipProfileRegistryObserver_ProfileRegistryErrorOccurredL()
    {
    CMusAvaProfileRegistryAdapterStub* profileRegistryAdapter = CMusAvaProfileRegistryAdapterStub::NewLC();
    TUint32 profileId = 1;
    TInt error = -5;
    iSipprofileRegistryObserver->ProfileRegistryErrorOccurred( profileId, error );
    EUNIT_ASSERT( profileRegistryAdapter->iProfileId == 0 );
    EUNIT_ASSERT( profileRegistryAdapter->iError == -1 );
    
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == NULL );
    iSipprofileRegistryObserver->AddAdapterL( *profileRegistryAdapter );
    EUNIT_ASSERT( iSipprofileRegistryObserver->iAdapter == profileRegistryAdapter );
    iSipprofileRegistryObserver->ProfileRegistryErrorOccurred( profileId, error );
    EUNIT_ASSERT( profileRegistryAdapter->iProfileId == profileId );
    EUNIT_ASSERT( profileRegistryAdapter->iError == error );
    CleanupStack::PopAndDestroy();
    } 


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaSipProfileRegistryObserver,
    "CMusAvaSipProfileRegistryObserver",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaSipProfileRegistryObserver",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipProfileRegistryObserver_NewLL, Teardown)

EUNIT_TEST(
    "AddAdapterL - test ",
    "CMusAvaSipProfileRegistryObserver",
    "AddAdapterL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipProfileRegistryObserver_AddAdapterLL, Teardown) 

EUNIT_TEST(
    "RemoveAdapter - test ",
    "CMusAvaSipProfileRegistryObserver",
    "RemoveAdapter",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipProfileRegistryObserver_RemoveAdapterL, Teardown) 

   
EUNIT_TEST(
    "ProfileRegistryEventOccurred - test ",
    "CMusAvaSipProfileRegistryObserver",
    "ProfileRegistryEventOccurred",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipProfileRegistryObserver_ProfileRegistryEventOccurredL, Teardown)   
    
EUNIT_TEST(
    "ProfileRegistryErrorOccurred - test ",
    "CMusAvaSipProfileRegistryObserver",
    "ProfileRegistryErrorOccurred",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipProfileRegistryObserver_ProfileRegistryErrorOccurredL, Teardown)  
  

EUNIT_END_TEST_TABLE

//  END OF FILE
