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
#include "ut_muswpitem.h"

#include "muswpitem.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>

// CONSTANTS
_LIT( KMusSomeName, "Some name" );
_LIT( KMusSomeProvider, "SomeProvider" );
_LIT( KMusSomeToAppRef, "SomeToAppRef" );

_LIT( KMusWpParamValue0, "0" );
_LIT( KMusWpParamValue1, "1" );
_LIT( KMusWpParamValue2, "2" );
_LIT( KMusWpParamValue3, "3" );

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusWpItem* UT_CMusWpItem::NewL()
    {
    UT_CMusWpItem* self = UT_CMusWpItem::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusWpItem* UT_CMusWpItem::NewLC()
    {
    UT_CMusWpItem* self = new( ELeave ) UT_CMusWpItem();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusWpItem::~UT_CMusWpItem()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusWpItem::UT_CMusWpItem()
    :iWpItem( NULL )
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::SetupL()
    {
    iWpItem = CMusWpItem::NewL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::Teardown()
    {
    delete iWpItem;
    }



// TEST CASES



    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_NewLL()
    {
    EUNIT_ASSERT( iWpItem )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_ProviderIdL()
    {
    EUNIT_ASSERT( iWpItem->ProviderId() == KNullDesC() )
    iWpItem->iProviderId = KMusSomeProvider().AllocL();    
    EUNIT_ASSERT( iWpItem->ProviderId() == KMusSomeProvider() )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_SetProviderIdLL()
    {
    EUNIT_ASSERT( !iWpItem->iProviderId )
    iWpItem->SetProviderIdL( KMusSomeProvider() );
    EUNIT_ASSERT( *iWpItem->iProviderId == KMusSomeProvider() )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_NameL()
    {
    EUNIT_ASSERT( iWpItem->Name() == KNullDesC() )
    iWpItem->iName = KMusSomeName().AllocL();   
    EUNIT_ASSERT( iWpItem->Name() == KMusSomeName() )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_SetNameLL()
    {
    EUNIT_ASSERT( !iWpItem->iName )
    iWpItem->SetNameL( KMusSomeName() );
    EUNIT_ASSERT( *iWpItem->iName == KMusSomeName() )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_ToAppRefL()
    {
    EUNIT_ASSERT( iWpItem->ToAppRef() == KNullDesC() )
    iWpItem->iToAppRef = KMusSomeToAppRef().AllocL();  
    EUNIT_ASSERT( iWpItem->ToAppRef() == KMusSomeToAppRef() )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_SetToAppRefLL()
    {
    EUNIT_ASSERT( !iWpItem->iToAppRef )
    iWpItem->SetToAppRefL( KMusSomeToAppRef() );
    EUNIT_ASSERT( *iWpItem->iToAppRef == KMusSomeToAppRef() )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_SetEdgeDtmSupportL()
    {
    EUNIT_ASSERT( iWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeNotAllowed )
    iWpItem->SetEdgeDtmSupport( ETrue );
    EUNIT_ASSERT( iWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeAllowed )
    iWpItem->SetEdgeDtmSupport( EFalse );              
    EUNIT_ASSERT( iWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeNotAllowed )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_SetForceInternetSignalingL()
    {
    EUNIT_ASSERT( iWpItem->iForceInternetSignaling == 
                  MusSettingsKeys::EFollowProfileConfiguration )
    iWpItem->SetForceInternetSignaling( ETrue );
    EUNIT_ASSERT( iWpItem->iForceInternetSignaling == 
                  MusSettingsKeys::EForceInternetSignaling )
    iWpItem->SetForceInternetSignaling( EFalse );              
    EUNIT_ASSERT( iWpItem->iForceInternetSignaling == 
                  MusSettingsKeys::EFollowProfileConfiguration )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_SetCapabilityQueryL()
    {
    EUNIT_ASSERT( iWpItem->iCapabilityQuery == 
                  MusSettingsKeys::ENoOptions )
    iWpItem->SetCapabilityQuery( KMusWpParamValue0() ); // Unknown
    EUNIT_ASSERT( iWpItem->iCapabilityQuery == 
                  MusSettingsKeys::ENoOptions )
    iWpItem->SetCapabilityQuery( KMusWpParamValue1() ); // Parallel
    EUNIT_ASSERT( iWpItem->iCapabilityQuery == 
                  MusSettingsKeys::EParallel )
    iWpItem->SetCapabilityQuery( KMusWpParamValue2() ); // Sequential
    EUNIT_ASSERT( iWpItem->iCapabilityQuery == 
                  MusSettingsKeys::ESequential )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_SetActivationConditionL()
    {
    // Default value is changed for a while so test is conformed to that
    
    //EUNIT_ASSERT( iWpItem->iActivationCondition == 
    //              MusSettingsKeys::EActiveInHomeNetworks )
    EUNIT_ASSERT( iWpItem->iActivationCondition == 
                  MusSettingsKeys::EAlwaysActive )
    iWpItem->SetActivationCondition( KMusWpParamValue0() ); // Unknown
    //EUNIT_ASSERT( iWpItem->iActivationCondition == 
    //              MusSettingsKeys::EActiveInHomeNetworks )
    EUNIT_ASSERT( iWpItem->iActivationCondition == 
                  MusSettingsKeys::EAlwaysActive )
    iWpItem->SetActivationCondition( KMusWpParamValue1() ); // Always
    EUNIT_ASSERT( iWpItem->iActivationCondition == 
                  MusSettingsKeys::EAlwaysActive )
    iWpItem->SetActivationCondition( KMusWpParamValue2() ); // Home
    EUNIT_ASSERT( iWpItem->iActivationCondition == 
                  MusSettingsKeys::EAlwaysActive )
    iWpItem->SetActivationCondition( KMusWpParamValue3() ); // Never
    EUNIT_ASSERT( iWpItem->iActivationCondition == 
                  MusSettingsKeys::ENever )
    }
    
        
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpItem::UT_CMusWpItem_SaveLL()
    {
    // Dummy test
    iWpItem->SaveL();
    }





//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusWpItem,
    "UT_CMusWpItem",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusWpItem",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_NewLL, Teardown)

EUNIT_TEST(
    "ProviderId - test ",
    "CMusWpItem",
    "ProviderId",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_ProviderIdL, Teardown)

EUNIT_TEST(
    "SetProviderIdL - test ",
    "CMusWpItem",
    "SetProviderIdL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_SetProviderIdLL, Teardown)

EUNIT_TEST(
    "Name - test ",
    "CMusWpItem",
    "Name",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_NameL, Teardown)

EUNIT_TEST(
    "SetNameL - test ",
    "CMusWpItem",
    "SetNameL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_SetNameLL, Teardown)

EUNIT_TEST(
    "ToAppRef - test ",
    "CMusWpItem",
    "ToAppRef",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_ToAppRefL, Teardown)

EUNIT_TEST(
    "SetToAppRefL - test ",
    "CMusWpItem",
    "SetToAppRefL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_SetToAppRefLL, Teardown)

EUNIT_TEST(
    "SetEdgeDtmSupport - test ",
    "CMusWpItem",
    "SetEdgeDtmSupport",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_SetEdgeDtmSupportL, Teardown)

EUNIT_TEST(
    "SetForceInternetSignaling - test ",
    "CMusWpItem",
    "SetForceInternetSignaling",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_SetForceInternetSignalingL, Teardown)

EUNIT_TEST(
    "SetCapabilityQuery - test ",
    "CMusWpItem",
    "SetCapabilityQuery",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_SetCapabilityQueryL, Teardown)

EUNIT_TEST(
    "SetActivationCondition - test ",
    "CMusWpItem",
    "SetActivationCondition",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_SetActivationConditionL, Teardown)
    
EUNIT_TEST(
    "SaveL - test ",
    "CMusWpItem",
    "SaveL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpItem_SaveLL, Teardown)

       
   
     
    
EUNIT_END_TEST_TABLE

//  END OF FILE


