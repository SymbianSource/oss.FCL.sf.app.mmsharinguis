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
#include "UT_CMusAvaCallEventMonitor.h"

//  EXTERNAL INCLUDES
#include <EUnitMacros.h>

//  INTERNAL INCLUDES
#include "musavacalleventmonitor.h"
#include "musavacallmonitorobserver.h"
#include "mussesseioninformationapi.h"


// CONSTRUCTION
UT_CMusAvaCallEventMonitor* UT_CMusAvaCallEventMonitor::NewL()
    {
    UT_CMusAvaCallEventMonitor* self = UT_CMusAvaCallEventMonitor::NewLC();
    CleanupStack::Pop( self );
    return self;
    }

UT_CMusAvaCallEventMonitor* UT_CMusAvaCallEventMonitor::NewLC()
    {
    UT_CMusAvaCallEventMonitor* self = new( ELeave ) UT_CMusAvaCallEventMonitor();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaCallEventMonitor::~UT_CMusAvaCallEventMonitor()
    {
    }

// Default constructor
UT_CMusAvaCallEventMonitor::UT_CMusAvaCallEventMonitor()
    {
    }

// Second phase construct
void UT_CMusAvaCallEventMonitor::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }


//  METHODS


void UT_CMusAvaCallEventMonitor::SetupL()
    {
    iObserver = CMusAvaCallMonitorObserverStub::NewL();
    iCallEventMonitor = CMusAvaCallEventMonitor::NewL( *iObserver );    
    }

void UT_CMusAvaCallEventMonitor::Teardown()
    {
    delete iObserver;
    iObserver = NULL;
    delete iCallEventMonitor;
    iCallEventMonitor = NULL;
    }
    
void UT_CMusAvaCallEventMonitor::UT_CMusAvaCallEventMonitor_GetTelNumberLL()
    {
    // Test tel number
    _LIT( KTelNbr, "12345678" );
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                        NMusSessionInformationApi::KMusTelNumber,
                                        KTelNbr ) );
    
    TBuf<CMusAvaCallEventMonitor::KMusTelNumberMaxLength> telNumber;
	TBool isSipUri( ETrue );

    iCallEventMonitor->GetTelNumberL( telNumber, isSipUri );
    
    EUNIT_ASSERT( !isSipUri );
    EUNIT_ASSERT_EQUALS( telNumber, KTelNbr );
    
    
    // Test SIP URI
    _LIT( KSipUri, "sip:12345678@host" );
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                        NMusSessionInformationApi::KMusTelNumber,
                                        KSipUri) );        
    iCallEventMonitor->GetTelNumberL( telNumber, isSipUri );
    EUNIT_ASSERT( isSipUri );
    EUNIT_ASSERT_EQUALS( telNumber, KSipUri );
    
    
#if 0
    // This case can't be run since Unit tests use a stub of RProperty::Get
    // and the stub doesn't return KErrOverflow, but results USER 11 panic
    // if the value does not fit into given buffer.
    
    
    // Test with a SIP URI that is too long to fit into telNumber buffer
    _LIT( KLongSipUri,
    "sip:567890@domain.9012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901" );

    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                        NMusSessionInformationApi::KMusTelNumber,
                                        KLongSipUri) );        
    EUNIT_ASSERT_SPECIFIC_LEAVE(
        iCallEventMonitor->GetTelNumberL( telNumber, isSipUri ), KErrOverflow );
#endif
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusAvaCallEventMonitor,
    "CMusAvaCallEventMonitor",
    "UNIT" )

EUNIT_TEST(
    "GetTelNumberL - test ",
    "CMusAvaCapability",
    "GetTelNumberL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaCallEventMonitor_GetTelNumberLL, Teardown )

EUNIT_END_TEST_TABLE

//  END OF FILE
