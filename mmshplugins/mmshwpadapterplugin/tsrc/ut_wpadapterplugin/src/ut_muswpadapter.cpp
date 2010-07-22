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
#include "ut_muswpadapter.h"
#include "muswpadapterstubs.h"

#include "muswpadapter.h"
#include "muswpitem.h"

//  SYSTEM INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <cwpcharacteristic.h>
#include <cwpparameter.h>



_LIT( KMusSomeName, "Some name" );
_LIT( KMusSomeProvider, "SomeProvider" );
_LIT( KMusSomeToAppRef, "SomeToAppRef" );
_LIT( KMusOurToAppRef, "OurToAppRef" );
_LIT( KMusAppID, "w9017" );
_LIT( KSipAppID, "w9010" );
_LIT( KMUsWpCharacteristicNameCapability, "CAPABILITY" );
_LIT( KMUsWpCharacteristicNameActivation, "ACTIVATION" );
_LIT( KMUsWpCharacteristicNameUnknown, "UNKNOWN" );
_LIT( KMusWpParamNameEdgeDtm, "EDGEDTM" );
_LIT( KMusWpParamNameInternetSignaling, "INTERNETSIGNALING" );
_LIT( KMusWpParamNameQuery, "QUERY" );
_LIT( KMusWpParamNameCondition, "CONDITION" );
_LIT( KMusWpParamNameUnknown, "UNKNOWN" );
_LIT( KMusWpParamValue1, "1" );
_LIT( KMusWpParamValue2, "2" );
_LIT( KMusWpParamValue3, "3" );

_LIT8( KMusSomeAppID8, "w0000" );
_LIT8( KSipAppID8, "w9010" );
_LIT8( KMusSomeStorageIdValue8, "9999" );
_LIT8( KMusSomeToAppRef8, "SomeToAppRef" );
_LIT8( KMusOurToAppRef8, "OurToAppRef" );

const TUint32 KMusWpNotSet = 0;
const TUint32 KMusSomeStorageIdValueAsTUint = 9999;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusWpAdapter* UT_CMusWpAdapter::NewL()
    {
    UT_CMusWpAdapter* self = UT_CMusWpAdapter::NewLC();
    CleanupStack::Pop( self );
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusWpAdapter* UT_CMusWpAdapter::NewLC()
    {
    UT_CMusWpAdapter* self = new( ELeave ) UT_CMusWpAdapter();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusWpAdapter::~UT_CMusWpAdapter()
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Default constructor
// -----------------------------------------------------------------------------
//
UT_CMusWpAdapter::UT_CMusWpAdapter()
    :iWpAdapter( NULL )
    {
    // NOP
    }


// -----------------------------------------------------------------------------
// Second phase construct
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }
    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::SetupL()
    {
    iWpAdapter = CMusWpAdapter::NewL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::Teardown()
    {
    delete iWpAdapter;
    }



// TEST CASES



    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_NewLL()
    {
    EUNIT_ASSERT( iWpAdapter )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_ItemCountL()
    {
    EUNIT_ASSERT( iWpAdapter->ItemCount() == 0 )
    
    iWpAdapter->iWpItem = CMusWpItem::NewL();
    
    EUNIT_ASSERT( iWpAdapter->ItemCount() == 1 )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_SummaryTitleL()
    {
    EUNIT_ASSERT( iWpAdapter->SummaryTitle( 0 ) == KMusSummaryTitle )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_SummaryTextL()
    {
    // Try to get text when no settings received, default returned
    EUNIT_ASSERT( !iWpAdapter->iWpItem )
    EUNIT_ASSERT( iWpAdapter->SummaryText( 0 ) == KMusSummaryTitle )
    
    // Try to get text when name received, name returned
    iWpAdapter->iWpItem = CMusWpItem::NewL();
    iWpAdapter->iWpItem->iName = KMusSomeName().AllocL();

    EUNIT_ASSERT( iWpAdapter->SummaryText( 0 ) == KMusSomeName() )    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_SaveLL()
    {
    // dummy, we cannot assert anything
    iWpAdapter->SaveL( 0 );
    iWpAdapter->iWpItem = CMusWpItem::NewL();
    iWpAdapter->SaveL( 0 );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_CanSetAsDefaultL()
    {
    EUNIT_ASSERT( iWpAdapter->CanSetAsDefault( 0 ) == EFalse )
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_SetAsDefaultLL()
    {
    TRAPD( error, iWpAdapter->SetAsDefaultL( 0 ) )
    EUNIT_ASSERT( error == KErrNotSupported )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_DetailsLL()
    {
    TPairVisitorStub visitor;
        
    EUNIT_ASSERT( iWpAdapter->DetailsL( 0, visitor ) == KErrNotSupported )
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_SettingsSavedLL()
    {
    // Try to tell that some other adapter than SIPAdapter has saved something,
    // do nothing
    
    iWpAdapter->SettingsSavedL( KMusSomeAppID8(), 
                                KMusSomeToAppRef8(), 
                                KMusSomeStorageIdValue8() );
    EUNIT_ASSERT( !iWpAdapter->iProfileIdReceived );
    EUNIT_ASSERT( iWpAdapter->iProfileId == KMusWpNotSet );
    
    // Try to tell that sip adapter has saved profile for some other app, 
    // and we have not received any settings, do nothing
    iWpAdapter->SettingsSavedL( KSipAppID8(), 
                                KMusSomeToAppRef8(), 
                                KMusSomeStorageIdValue8() );
    EUNIT_ASSERT( !iWpAdapter->iProfileIdReceived );
    EUNIT_ASSERT( iWpAdapter->iProfileId == KMusWpNotSet );
    
    // Try to tell that sip adapter has saved profile for some other app, 
    // and we have also received settings, but not TO-APPREF, do nothing
    iWpAdapter->iWpItem = CMusWpItem::NewL();
    
    iWpAdapter->SettingsSavedL( KSipAppID8(), 
                                KMusSomeToAppRef8(), 
                                KMusSomeStorageIdValue8() );
    EUNIT_ASSERT( !iWpAdapter->iProfileIdReceived );
    EUNIT_ASSERT( iWpAdapter->iProfileId == KMusWpNotSet );
    
    // Try to tell that sip adapter has saved profile for us, prepare to save
    iWpAdapter->iWpItem->iToAppRef = KMusOurToAppRef().AllocL();
    
    iWpAdapter->SettingsSavedL( KSipAppID8(), 
                                KMusOurToAppRef8(), 
                                KMusSomeStorageIdValue8() );
    EUNIT_ASSERT( iWpAdapter->iProfileIdReceived );
    EUNIT_ASSERT( iWpAdapter->iProfileId == KMusSomeStorageIdValueAsTUint );
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_SavingFinalizedLL()
    {
    // dummy, we cannot assert anything
    iWpAdapter->SavingFinalizedL();
    iWpAdapter->iProfileIdReceived = ETrue;
    iWpAdapter->SavingFinalizedL();
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_VisitCharacteristicLL()
    {
    // Try without name and with unknown type, does nothing
    CWPCharacteristic* characteristic = CWPCharacteristic::NewLC( -1 );
    iWpAdapter->VisitL( *characteristic );
    CleanupStack::PopAndDestroy( characteristic );
    
    // Try with named characteristic outside application-characteristic
    characteristic = CWPCharacteristic::NewLC( KWPNamedCharacteristic );
    iWpAdapter->VisitL( *characteristic );
    CleanupStack::PopAndDestroy( characteristic );
    
    // Try with application characteristic, but without any id
    characteristic = CWPCharacteristic::NewLC( KWPApplication );
    
    iWpAdapter->VisitL( *characteristic );
    EUNIT_ASSERT( !iWpAdapter->iAppID )
    EUNIT_ASSERT( !iWpAdapter->iWpItem )
    
    CleanupStack::PopAndDestroy( characteristic );
    
    // Try with application characteristic, but without our id (w9017)
    characteristic = CWPCharacteristic::NewLC( KWPApplication );
   
    CWPParameter* param = CWPParameter::NewLC();
    param->SetID( EWPParameterAppID );
    param->SetValueL( KSipAppID() );
    characteristic->InsertL( param );
    CleanupStack::Pop( param );
    
    param = CWPParameter::NewLC();
    param->SetID( EWPParameterName );
    param->SetValueL( KMusSomeName() );
    characteristic->InsertL( param );
    CleanupStack::Pop( param );
    
    iWpAdapter->VisitL( *characteristic );
    EUNIT_ASSERT( *iWpAdapter->iAppID == KSipAppID()  )
    EUNIT_ASSERT( !iWpAdapter->iWpItem )
    
    CleanupStack::PopAndDestroy( characteristic );
      
    // Try with application, capability, activation and unknown 
    // characteristic, our id and proper params
    characteristic = CWPCharacteristic::NewLC( KWPApplication );
   
    param = CWPParameter::NewLC();
    param->SetID( EWPParameterAppID );
    param->SetValueL( KMusAppID() );
    characteristic->InsertL( param );
    CleanupStack::Pop( param );
    
    param = CWPParameter::NewLC();
    param->SetID( EWPParameterProviderID );
    param->SetValueL( KMusSomeProvider() );
    characteristic->InsertL( param );
    CleanupStack::Pop( param );
    
    param = CWPParameter::NewLC();
    param->SetID( EWPParameterName );
    param->SetValueL( KMusSomeName() );
    characteristic->InsertL( param );
    CleanupStack::Pop( param );
    
    param = CWPParameter::NewLC();
    param->SetID( EWPParameterToAppRef );
    param->SetValueL( KMusSomeToAppRef() );
    characteristic->InsertL( param );
    CleanupStack::Pop( param );
    
    CWPCharacteristic* capabilityCharacteristic = 
                        CWPCharacteristic::NewLC( KWPNamedCharacteristic );
    capabilityCharacteristic->SetNameL( KMUsWpCharacteristicNameCapability() );
    characteristic->InsertL( capabilityCharacteristic );
    CleanupStack::Pop( capabilityCharacteristic );
    
    CWPCharacteristic* activityCharacteristic = 
                        CWPCharacteristic::NewLC( KWPNamedCharacteristic );
    activityCharacteristic->SetNameL( KMUsWpCharacteristicNameActivation() );
    characteristic->InsertL( activityCharacteristic );
    CleanupStack::Pop( activityCharacteristic );
    
    CWPCharacteristic* unknownCharacteristic = 
                        CWPCharacteristic::NewLC( KWPNamedCharacteristic );
    unknownCharacteristic->SetNameL( KMUsWpCharacteristicNameUnknown() );
    characteristic->InsertL( unknownCharacteristic );
    CleanupStack::Pop( unknownCharacteristic );                      
    
    iWpAdapter->VisitL( *characteristic );
    EUNIT_ASSERT( *iWpAdapter->iAppID == KMusAppID()  )
    EUNIT_ASSERT( iWpAdapter->iWpItem )
    EUNIT_ASSERT( *iWpAdapter->iWpItem->iProviderId == KMusSomeProvider() )
    EUNIT_ASSERT( *iWpAdapter->iWpItem->iName == KMusSomeName() )
    EUNIT_ASSERT( *iWpAdapter->iWpItem->iToAppRef == KMusSomeToAppRef() )
    
    CleanupStack::PopAndDestroy( characteristic );
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_VisitParameterLL()
    {
    iWpAdapter->iNewWpItem = CMusWpItem::NewL();
    
    // Named parameter EDGEDTM
    CWPParameter* param = CWPParameter::NewLC();
    param->SetID( EWPNamedParameter );
    param->SetNameL( KMusWpParamNameEdgeDtm() );
    iWpAdapter->VisitL( *param );
    EUNIT_ASSERT( !iWpAdapter->iAppID )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iProviderId )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iName )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iToAppRef )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeAllowed)
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iForceInternetSignaling ==
                  MusSettingsKeys::EFollowProfileConfiguration )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iCapabilityQuery ==
                  MusSettingsKeys::ENoOptions )
    // Default value is changed for a while so test is conformed to that              
    //EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
    //              MusSettingsKeys::EActiveInHomeNetworks )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
                  MusSettingsKeys::EAlwaysActive )
    CleanupStack::PopAndDestroy( param );
    
    // Named parameter INTERNETSIGNALING
    param = CWPParameter::NewLC();
    param->SetID( EWPNamedParameter );
    param->SetNameL( KMusWpParamNameInternetSignaling() );
    iWpAdapter->VisitL( *param );
    EUNIT_ASSERT( !iWpAdapter->iAppID )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iProviderId )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iName )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iToAppRef )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeAllowed)
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iForceInternetSignaling ==
                  MusSettingsKeys::EForceInternetSignaling )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iCapabilityQuery ==
                  MusSettingsKeys::ENoOptions )
    // Default value is changed for a while so test is conformed to that              
    //EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
    //              MusSettingsKeys::EActiveInHomeNetworks )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
                  MusSettingsKeys::EAlwaysActive )
    CleanupStack::PopAndDestroy( param );

    // Named parameter QUERY
    param = CWPParameter::NewLC();
    param->SetID( EWPNamedParameter );
    param->SetNameL( KMusWpParamNameQuery() );
    param->SetValueL( KMusWpParamValue2() ); // Sequential
    iWpAdapter->VisitL( *param );
    EUNIT_ASSERT( !iWpAdapter->iAppID )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iProviderId )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iName )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iToAppRef )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeAllowed)
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iForceInternetSignaling ==
                  MusSettingsKeys::EForceInternetSignaling )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iCapabilityQuery ==
                  MusSettingsKeys::ESequential )
    // Default value is changed for a while so test is conformed to that              
    //EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
    //              MusSettingsKeys::EActiveInHomeNetworks )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
                  MusSettingsKeys::EAlwaysActive )
    CleanupStack::PopAndDestroy( param );
    
    // Named parameter CONDITION
    param = CWPParameter::NewLC();
    param->SetID( EWPNamedParameter );
    param->SetNameL( KMusWpParamNameCondition() );
    param->SetValueL( KMusWpParamValue3() ); // Never
    iWpAdapter->VisitL( *param );
    EUNIT_ASSERT( !iWpAdapter->iAppID )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iProviderId )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iName )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iToAppRef )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeAllowed)
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iForceInternetSignaling ==
                  MusSettingsKeys::EForceInternetSignaling )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iCapabilityQuery ==
                  MusSettingsKeys::ESequential )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
                  MusSettingsKeys::ENever )
    CleanupStack::PopAndDestroy( param );

    // Unknown named parameter
    param = CWPParameter::NewLC();
    param->SetID( EWPNamedParameter );
    param->SetNameL( KMusWpParamNameUnknown() );
    iWpAdapter->VisitL( *param );
    EUNIT_ASSERT( !iWpAdapter->iAppID )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iProviderId )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iName )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iToAppRef )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeAllowed)
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iForceInternetSignaling ==
                  MusSettingsKeys::EForceInternetSignaling )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iCapabilityQuery ==
                  MusSettingsKeys::ESequential )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
                  MusSettingsKeys::ENever )
    CleanupStack::PopAndDestroy( param );
    
    // Id
    param = CWPParameter::NewLC();
    param->SetID( EWPParameterAppID );
    param->SetValueL( KMusAppID() );
    iWpAdapter->VisitL( *param );
    EUNIT_ASSERT( *iWpAdapter->iAppID == KMusAppID() )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iProviderId )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iName )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iToAppRef )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeAllowed)
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iForceInternetSignaling ==
                  MusSettingsKeys::EForceInternetSignaling )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iCapabilityQuery ==
                  MusSettingsKeys::ESequential )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
                  MusSettingsKeys::ENever )
    CleanupStack::PopAndDestroy( param );
    
    // Provider Id
    param = CWPParameter::NewLC();
    param->SetID( EWPParameterProviderID );
    param->SetValueL( KMusSomeProvider() );
    iWpAdapter->VisitL( *param );
    EUNIT_ASSERT( *iWpAdapter->iAppID == KMusAppID() )
    EUNIT_ASSERT( *iWpAdapter->iNewWpItem->iProviderId == KMusSomeProvider() )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iName )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iToAppRef )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeAllowed)
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iForceInternetSignaling ==
                  MusSettingsKeys::EForceInternetSignaling )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iCapabilityQuery ==
                  MusSettingsKeys::ESequential )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
                  MusSettingsKeys::ENever )
    CleanupStack::PopAndDestroy( param );
    
    // Name
    param = CWPParameter::NewLC();
    param->SetID( EWPParameterName );
    param->SetValueL( KMusSomeName() );
    iWpAdapter->VisitL( *param );
    EUNIT_ASSERT( *iWpAdapter->iAppID == KMusAppID() )
    EUNIT_ASSERT( *iWpAdapter->iNewWpItem->iProviderId == KMusSomeProvider() )
    EUNIT_ASSERT( *iWpAdapter->iNewWpItem->iName == KMusSomeName() )
    EUNIT_ASSERT( !iWpAdapter->iNewWpItem->iToAppRef )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeAllowed)
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iForceInternetSignaling ==
                  MusSettingsKeys::EForceInternetSignaling )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iCapabilityQuery ==
                  MusSettingsKeys::ESequential )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
                  MusSettingsKeys::ENever )
    CleanupStack::PopAndDestroy( param );
    
    // To-AppRef
    param = CWPParameter::NewLC();
    param->SetID( EWPParameterToAppRef );
    param->SetValueL( KMusSomeToAppRef() );
    iWpAdapter->VisitL( *param );
    EUNIT_ASSERT( *iWpAdapter->iAppID == KMusAppID() )
    EUNIT_ASSERT( *iWpAdapter->iNewWpItem->iProviderId == KMusSomeProvider() )
    EUNIT_ASSERT( *iWpAdapter->iNewWpItem->iName == KMusSomeName() )
    EUNIT_ASSERT( *iWpAdapter->iNewWpItem->iToAppRef == KMusSomeToAppRef() )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeAllowed)
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iForceInternetSignaling ==
                  MusSettingsKeys::EForceInternetSignaling )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iCapabilityQuery ==
                  MusSettingsKeys::ESequential )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
                  MusSettingsKeys::ENever )
    CleanupStack::PopAndDestroy( param );
    
    // Unknown, maps to default, nothing changes
    param = CWPParameter::NewLC();
    param->SetID( EWPLastParameter );
    iWpAdapter->VisitL( *param );
    EUNIT_ASSERT( *iWpAdapter->iAppID == KMusAppID() )
    EUNIT_ASSERT( *iWpAdapter->iNewWpItem->iProviderId == KMusSomeProvider() )
    EUNIT_ASSERT( *iWpAdapter->iNewWpItem->iName == KMusSomeName() )
    EUNIT_ASSERT( *iWpAdapter->iNewWpItem->iToAppRef == KMusSomeToAppRef() )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iEdgeDtmSupport == 
                  MusSettingsKeys::EDtmModeAllowed)
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iForceInternetSignaling ==
                  MusSettingsKeys::EForceInternetSignaling )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iCapabilityQuery ==
                  MusSettingsKeys::ESequential )
    EUNIT_ASSERT( iWpAdapter->iNewWpItem->iActivationCondition ==
                  MusSettingsKeys::ENever )
    CleanupStack::PopAndDestroy( param );
    
    }
    
    
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusWpAdapter::UT_CMusWpAdapter_VisitLinkLL()
    {
    // Dummy case
    CWPCharacteristic* characteristic = CWPCharacteristic::NewLC( 0 );
    iWpAdapter->VisitLinkL( *characteristic );
    CleanupStack::PopAndDestroy( characteristic );
    }





//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusWpAdapter,
    "UT_CMusWpAdapter",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusWpAdapter",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_NewLL, Teardown)

EUNIT_TEST(
    "ItemCount - test ",
    "CMusWpAdapter",
    "ItemCount",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_ItemCountL, Teardown)

EUNIT_TEST(
    "SummaryTitle - test ",
    "CMusWpAdapter",
    "SummaryTitle",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_SummaryTitleL, Teardown)

EUNIT_TEST(
    "SummaryText - test ",
    "CMusWpAdapter",
    "SummaryText",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_SummaryTextL, Teardown)
    
EUNIT_TEST(
    "SaveL - test ",
    "CMusWpAdapter",
    "SaveL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_SaveLL, Teardown)
    
EUNIT_TEST(
    "CanSetAsDefault - test ",
    "CMusWpAdapter",
    "CanSetAsDefault",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_CanSetAsDefaultL, Teardown)

EUNIT_TEST(
    "SetAsDefaultL - test ",
    "CMusWpAdapter",
    "SetAsDefaultL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_SetAsDefaultLL, Teardown)

EUNIT_TEST(
    "DetailsL - test ",
    "CMusWpAdapter",
    "DetailsL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_DetailsLL, Teardown) 

EUNIT_TEST(
    "SettingsSavedL - test ",
    "CMusWpAdapter",
    "SettingsSavedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_SettingsSavedLL, Teardown) 

EUNIT_TEST(
    "SavingFinalizedL - test ",
    "CMusWpAdapter",
    "SavingFinalizedL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_SavingFinalizedLL, Teardown)

EUNIT_TEST(
    "VisitCharacteristicL - test ",
    "CMusWpAdapter",
    "VisitCharacteristicL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_VisitCharacteristicLL, Teardown)

EUNIT_TEST(
    "VisitParameterL - test ",
    "CMusWpAdapter",
    "VisitParameterL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_VisitParameterLL, Teardown)  

EUNIT_TEST(
    "VisitLinkL - test ",
    "CMusWpAdapter",
    "VisitLinkL",
    "FUNCTIONALITY",
    SetupL, UT_CMusWpAdapter_VisitLinkLL, Teardown)      
       
       
   
     
    
EUNIT_END_TEST_TABLE

//  END OF FILE


