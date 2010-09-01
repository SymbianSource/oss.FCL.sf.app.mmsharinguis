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


CMusCallMonitorMock::CMusCallMonitorMock( const RMobileCall& aCall,
										MMusTsyPropertyObserver& aObserver ) 
        : CMusCallMonitorBase( aCall, aObserver )
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
void 
UT_CMusCallMonitorBase::NotifyCallStateChanged( NMusSessionInformationApi::TMusCallEvent /*aVal*/,
                                                TName& /*aCallName*/ )
    {          
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
    iCallMonitorBase = new CMusCallMonitorMock( iCall, *this );    
    }


void UT_CMusCallMonitorBase::Setup2L()
    {
    iCallMonitorBase = new CMusCallMonitorMock( iCall, *this );    
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

	
    iCallMonitorBase->SetStateL( NMusSessionInformationApi::ECallHold );
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
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::UT_SetClirSettingL()
    {
    TInt tmp;
    _LIT( KTestNumber1, "11111" );
    _LIT( KTestNumber2, "#31#11111" );
    
    iCallMonitorBase->SetClirSetting( KTestNumber1() );
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusClirSetting,
                     tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::ESendOwnNumber);
    
    iCallMonitorBase->SetClirSetting( KTestNumber2() );
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMusClirSetting,
                     tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::EDoNotSendOwnNumber);
        
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::UT_SetTerminatingPrivacyL()
    {
    TInt tmp;
        
    iCallMonitorBase->SetTerminatingPrivacy( iCallMonitorBase->iCall );
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMUSPrivacy,
                     tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::EPrivacyOff);
        
    RTelHelper::SetRemoteIdStatus( RMobileCall::ERemoteIdentitySuppressed );
    iCallMonitorBase->SetTerminatingPrivacy( iCallMonitorBase->iCall );
    RProperty::Get( NMusSessionInformationApi::KCategoryUid,
                     NMusSessionInformationApi::KMUSPrivacy,
                     tmp );
    EUNIT_ASSERT( tmp==NMusSessionInformationApi::EPrivacyOn);
    
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusCallMonitorBase::UT_RemovePrefixL()
    {
    _LIT( KTestNumber1, "11111" );
    _LIT( KTestNumber2, "#31#11111" );
    _LIT( KTestNumber3, "*31#11111" );

    
    HBufC* test1 = iCallMonitorBase->RemovePrefix(  KTestNumber1 );
    CleanupStack::PushL( test1 );
    EUNIT_ASSERT( *test1 == KTestNumber1 );
    CleanupStack::PopAndDestroy( test1 );
    
    HBufC* test2 = iCallMonitorBase->RemovePrefix(  KTestNumber3 );
    CleanupStack::PushL( test2 );
    EUNIT_ASSERT( *test2 == KTestNumber1 );
    CleanupStack::PopAndDestroy( test2 );
    
    HBufC* test3 = iCallMonitorBase->RemovePrefix(  KTestNumber2 );
    CleanupStack::PushL( test3 );
    EUNIT_ASSERT( *test3 == KTestNumber1 );
    CleanupStack::PopAndDestroy( test3 );
    
    
        
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
    "SetClirSetting - test",
    "CMusCallMonitorBase",
    "SetClirSetting",
    "FUNCTIONALITY",
    SetupL, UT_SetClirSettingL, Teardown)

EUNIT_TEST(
    "SetTerminatingPrivacy - test",
    "CMusCallMonitorBase",
    "SetTerminatingPrivacy",
    "FUNCTIONALITY",
    SetupL, UT_SetTerminatingPrivacyL, Teardown)

EUNIT_TEST(
    "RemovePrefix - test",
    "CMusCallMonitorBase",
    "RemovePrefix",
    "FUNCTIONALITY",
    SetupL, UT_RemovePrefixL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE


