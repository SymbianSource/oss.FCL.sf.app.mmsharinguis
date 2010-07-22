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
#include "e32property.h"
#include "ut_muscallmonitorbase.h"
#include "mussesseioninformationapi.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>

// Test configurations
_LIT(KEmptyProvider,"");
_LIT(KTestTelNumber,"+3585050");



CMusCallMonitorMock::CMusCallMonitorMock( const RMobileCall& aCall,
										MMusTsyPropertyObserver& aObserver,
										MMusCallStateObserver& aCallStateObserver ) 
        : CMusCallMonitorBase( aCall, aObserver, aCallStateObserver )
    {
    }


void CMusCallMonitorMock::RunL()
    {
    }

void CMusCallMonitorMock::DoCancel()
    {
    }

CMusCallMonitorMock::~CMusCallMonitorMock()
    {
    Cancel();        
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallMonitorBase* UT_CMusCallMonitorBase::NewL()
    {
    UT_CMusCallMonitorBase* self = UT_CMusCallMonitorBase::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallMonitorBase* UT_CMusCallMonitorBase::NewLC()
    {
    UT_CMusCallMonitorBase* self = new( ELeave ) UT_CMusCallMonitorBase();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusCallMonitorBase::~UT_CMusCallMonitorBase()
    {
    // None
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusCallMonitorBase::UT_CMusCallMonitorBase()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
// UT_CMusCallMonitorBase::NotifyCallStateChanged() 
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::NotifyCallStateChanged(
									NMusSessionInformationApi::TMusCallEvent /*aVal*/,
									TName& /*aCallName*/ )
    {          
    }



// -----------------------------------------------------------------------------
//  MusCallStateChanged from the MusCallStateObserver 
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::MusCallStateChanged()
    {
    iCallStateObserverInvoked = ETrue;
    }   



// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::SetupL()
    {
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     0 );
    RTelHelper::SetCallDirection( RMobileCall::EDirectionUnknown );
    iCallMonitorBase = new CMusCallMonitorMock( iCall, *this, *this );
    iCallStateObserverInvoked = EFalse;
    }


void UT_CMusCallMonitorBase::Setup2L()
    {
    iCallStateObserverInvoked = EFalse;
    iCallMonitorBase = new CMusCallMonitorMock( iCall, *this, *this );    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::Teardown()
    {    
    delete iCallMonitorBase;
    PropertyHelper::Close();
    iCall.Close();
    }



// TEST CASES


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::UT_SetStateLL()
    {
    TInt tmp;
    EUNIT_ASSERT ( RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent, tmp ) != KErrNone );
    
    EUNIT_ASSERT_LEAVE( iCallMonitorBase->SetStateL( NMusSessionInformationApi::ECallHold ) );
    
    if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     0 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
	if ( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallCount,
                     1 ) != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
	
	EUNIT_ASSERT (!iCallStateObserverInvoked )
	
    iCallMonitorBase->SetStateL( NMusSessionInformationApi::ECallHold );
	
	EUNIT_ASSERT ( iCallStateObserverInvoked )

    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallHold);
    
    iCallMonitorBase->SetStateL( NMusSessionInformationApi::ECallHold );
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallHold);

    iCallMonitorBase->SetStateL( NMusSessionInformationApi::ECallConnected );
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallEvent,
                     tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ECallConnected );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::UT_SetCallInfoLL()
    {
    _LIT( KTelnro, "1234" );
    TName telnro;
    TInt direction;
    
    iCallMonitorBase->SetCallInfoL();
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusTelNumber,
                     telnro );
    
    EUNIT_ASSERT( telnro == KTelnro );
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallDirection,
                     direction );
    
    EUNIT_ASSERT( direction == NMusSessionInformationApi::ENoDirection );
    
    RTelHelper::SetCallDirection( RMobileCall::EMobileOriginated );
    
    iCallMonitorBase->SetCallInfoL();
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusTelNumber,
                     telnro );
    
    EUNIT_ASSERT( telnro == KTelnro );

    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallDirection,
                     direction );
    
    EUNIT_ASSERT( direction == NMusSessionInformationApi::ECallOrginated );
    
    RTelHelper::SetCallDirection( RMobileCall::EMobileTerminated );
    iCallMonitorBase->SetCallInfoL();
    
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusTelNumber,
                     telnro );
    
    EUNIT_ASSERT( telnro == KTelnro );

    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusCallDirection,
                     direction );
    
    EUNIT_ASSERT( direction == NMusSessionInformationApi::ECallTerminated );
    
    /* Test for call provider */
    // Call Provider info is only Set for VoIP Calls
    iCallMonitorBase->SetCallInfoL();  
    TBuf<RProperty::KMaxPropertySize> provider;
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMUSCallProvider,
                    provider );
    
    
    EUNIT_ASSERT( provider == KEmptyProvider );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::UT_RunErrorL()
    {
    EUNIT_ASSERT( iCallMonitorBase->RunError( KErrNotFound ) == KErrNone );
    }


// -----------------------------------------------------------------------------
// UT_CMusCallMonitorBase::UT_CMusCallMonitorBase_IsDataReadyLL()
// Test if required Data is ready for CS calls
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::UT_CMusCallMonitorBase_IsDataReadyLL()
    {
    TBool dataReady = EFalse;
    
    //Test-1: Date Ready:
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                            NMusSessionInformationApi::KMusTelNumber,
                                            KTestTelNumber ));

    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                            NMusSessionInformationApi::KMusCallDirection,
                                            NMusSessionInformationApi::ECallOrginated ));
            
    dataReady = iCallMonitorBase->IsDataReadyL();
    EUNIT_ASSERT_EQUALS( dataReady, ETrue )
    
    //Test-2: Partial Data is Ready
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                                            NMusSessionInformationApi::KMusTelNumber,
                                            KNullDesC ));
    dataReady = iCallMonitorBase->IsDataReadyL();
    EUNIT_ASSERT_EQUALS( dataReady, EFalse )


    //Test-3: Data Not Ready.
    User::LeaveIfError(RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                 NMusSessionInformationApi::KMusCallEvent,
                 ( TInt ) NMusSessionInformationApi::ENoCall ));

    dataReady = iCallMonitorBase->IsDataReadyL();
    EUNIT_ASSERT_EQUALS( dataReady, EFalse )
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusCallMonitorBase,
    "UT_CMusCallMonitorBase",
    "UNIT" )

EUNIT_TEST(
    "SetStateL - test",
    "CMusCallMonitorBase",
    "SetStateL",
    "FUNCTIONALITY",
    Setup2L, UT_SetStateLL, Teardown)

EUNIT_TEST(
    "SetCallInfoL - test",
    "CMusCallMonitorBase",
    "SetCallInfoL",
    "FUNCTIONALITY",
    SetupL, UT_SetCallInfoLL, Teardown)

EUNIT_TEST(
    "RunError - test",
    "CMusCallMonitorBase",
    "RunError",
    "FUNCTIONALITY",
    SetupL, UT_RunErrorL, Teardown)
    
    
EUNIT_TEST(
    "IsDataReadyL - test",
    "CMusCallMonitorBase",
    "IsDataReadyL",
    "FUNCTIONALITY",
    SetupL, UT_CMusCallMonitorBase_IsDataReadyLL, Teardown)    
    

EUNIT_END_TEST_TABLE

//  END OF FILE
