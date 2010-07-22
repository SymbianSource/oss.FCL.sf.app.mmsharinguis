/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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


#include "ut_musaiwprovider.h"
#include "musaiwprovider.h"
#include "musaiwprovider.hrh"
#include "musmanagerstub.h"
#include "musmanagercommon.h"
#include "musindicatorapi.h"

#include <musresourceproperties.h>
#include "mussesseioninformationapi.h"
#include "mussettings.h"
#include "mustesthelp.h"
#include "mussessionproperties.h"
#define MUS_MEMORY_LEAVE_CHECKING_ON

#include <e32property.h>
#include <FeatMgr.h>
#include <aiwmenu.h>
#include <alwaysonlinemanagercommon.h>
#include <alwaysonlinemanagerclient.h>
#include <bautils.h>
#include <digia/eunit/eunitmacros.h>
#include <aknglobalnote.h>
#include <apgtask.h>
#include <eikmenup.h>
#include <uikoninternalpskeys.h>


// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files
// include this file. Directive can be removed when
// EUnit is fixed.
#pragma warn_illtokenpasting off

_LIT( KMusAiwProviderResource, "c:\\sys\\bin\\musaiwproviderui.rsc" );
_LIT( KResourceFilenameRom,"z:\\sys\\bin\\musaiwproviderui.rsc" );

_LIT( KMusOnHold, "On hold" );

_LIT8( KMusOnHold8, "On hold" );
_LIT8( KMusNetworkIncompatible8, "Network incompatible" );
_LIT8( KMusConferenceCall8, "Conference call" );
_LIT8( KMusRecipientIncapable8, "Recipient incapable" );
_LIT8( KMusNoReg8, "No registration" );
_LIT8( KMusGeneral, "Application error" );


CMusIndicatorApi* CMusIndicatorApi::NewL()
    {
    CMusIndicatorApi* self = new (ELeave) CMusIndicatorApi( NULL );
    return self;
    
    }



CMusIndicatorApi::CMusIndicatorApi( MMusIndicatorObserver* aObserver )
    : CActive( EPriorityStandard ),
    iObserver( aObserver )
    {
    }

void CMusIndicatorApi::DoCancel()
    {
    }


TInt CMusIndicatorApi::RunError( TInt aError )
    {
    return aError;
    }

void CMusIndicatorApi::RunL()
    {
    }


void CMusIndicatorApi::PlaySoundComplete( TInt /*aError*/ )
    {
    }

CMusIndicatorApi::~CMusIndicatorApi()
    {
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusAiwProvider* UT_CMusAiwProvider::NewL()
    {
    UT_CMusAiwProvider* self = UT_CMusAiwProvider::NewLC();
    CleanupStack::Pop( self );
    return self;
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusAiwProvider* UT_CMusAiwProvider::NewLC()
    {
    UT_CMusAiwProvider* self = new (ELeave) UT_CMusAiwProvider();
    CleanupStack::PushL( self );
    self->ConstructL();
    return self;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusAiwProvider::~UT_CMusAiwProvider()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
UT_CMusAiwProvider::UT_CMusAiwProvider()
    {
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::SetupL()
    {
    iProvider = CMusAiwProvider::NewL();
    CMusManager::SetUseCase( MultimediaSharing::EMusContinue );
    CMusManager::SetStartError( KErrNone );
    CMusManager::SetAvailability( KErrNone );
    MultimediaSharingSettings::SetPropertyValueL( 
                  MusSettingsKeys::KFastStartupMode, MusSettingsKeys::EFastModeOff );
    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::Teardown(  )
    {
    delete iProvider;
    PropertyHelper::Close();
    }


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::SetEmphasis( CCoeControl* /*aMenuControl*/,
                                      TBool /*aEmphasis*/ )
    {

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::ProcessCommandL( TInt /*aCommandId*/ )
    {

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
TInt UT_CMusAiwProvider::HandleNotifyL(
    TInt /*aCmdId*/,
    TInt /*aEventId*/,
    CAiwGenericParamList& /*aEventParamList*/,
    const CAiwGenericParamList& /*aInParamList*/)
    {
    return KErrNone;
    }



// ------- Tester functions ----------------------------------------------------

/*
RFs fs;
    User::LeaveIfError( fs.Connect() );

    CleanupClosePushL( fs );

    BaflUtils::CopyFile( fs, KMusAiwProviderResource(), MusAiwResourceFinder::ResourceFilenameRom() );
    BaflUtils::CopyFile( fs, KMusAiwProviderResource(), MusAiwResourceFinder::ResourceFilenameRam() );

    BaflUtils::DeleteFile( fs, MusAiwResourceFinder::ResourceFilenameRom() );

    BaflUtils::DeleteFile( fs, MusAiwResourceFinder::ResourceFilenameRam() );

    CleanupStack::PopAndDestroy(); // fs
*/

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_CMusAiwProvider_InitialiseLL()
    {
    // No implementation, so dummy test is enough
    RCriteriaArray array;
    iProvider->InitialiseL( *this, array );
    iProvider->DoInitialiseL();
    

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_CMusAiwProvider_HandleServiceCmdLL()
    {
    // HandleServiceCmdL just forwards request to HandleMenuCmdL

    CAiwGenericParamList* pl = CAiwGenericParamList::NewLC();

    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusContinue )

    iProvider->HandleServiceCmdL( EMusCommandLiveShare, *pl, *pl, 0, this );
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusLiveVideo )

    CleanupStack::PopAndDestroy( pl );

    CMusManager::SetUseCase( MultimediaSharing::EMusContinue );
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_CMusAiwProvider_InitializeMenuPaneLL()
    {    
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    BaflUtils::CopyFile( fs, KMusAiwProviderResource(),
                         KResourceFilenameRom() );

    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();

    CEikMenuPane* eikPane = NULL;
    CAiwMenuPane* aiwPane = CreateAiwMenuPaneLC( &eikPane );

    // ResourceFile is empty 
    delete iProvider->iResourceFileName;
    
    iProvider->iResourceFileName = NULL ;

    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
    EUNIT_ASSERT( iProvider->iResourceFileName != NULL )
    // Menu item counter
    TInt menuPaneCount = eikPane->NumberOfItemsInPane();

    // test "forbidden" feature: mus NOT allowed, operator variant set to operator
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EOperatorSpecific );
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMUSForbidden,
                    NMusSessionInformationApi::EMUSForbidden );    
    EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount )

    // set operator back to standard and allow mus
    RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
                    NMusSessionInformationApi::KMUSForbidden ,
                    NMusSessionInformationApi::EMUSAllowed );
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EStandard );

    //Some Negative Cases
    CMusManager::SetAvailability( MultimediaSharing::EErrNoSipProfile );
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );

    EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount )
    EUNIT_ASSERT( iProvider->iResourceFileName != NULL )

    CMusManager::SetAvailability( MultimediaSharing::EErrActivation );
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );

    EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount )
    EUNIT_ASSERT( iProvider->iResourceFileName != NULL )

    CMusManager::SetAvailability( ( MultimediaSharing::TMusAvailabilityStatus ) KErrNone );    
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );

    EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount )
    EUNIT_ASSERT( iProvider->iResourceFileName != NULL )

    // Cases  for "camera supported"!
    FeatureManager::CameraSupported(); 
    // Set it live,clip Menu showing    

    CMusManager::SetAvailability( MultimediaSharing::ESipRegistrationPending );
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );

    //EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )       
    MUS_CHECK_MEMORY_LEAVE( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )
    
    CMusManager::SetAvailability( MultimediaSharing::EErrConferenceCall );
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );

    //EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )
    MUS_CHECK_MEMORY_LEAVE( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )
    
    // set camera support on
    FeatureManager::CameraSupported();
    CleanupStack::PopAndDestroy( aiwPane );
    CleanupStack::PopAndDestroy( list );
    BaflUtils::DeleteFile( fs, KResourceFilenameRom() );

    CleanupStack::PopAndDestroy(); //  
    }

void UT_CMusAiwProvider::UT_CMusAiwProvider_InitializeMenuPaneL2L()
    {    
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    BaflUtils::CopyFile( fs, KMusAiwProviderResource(),
                         KResourceFilenameRom() );

    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();

    CEikMenuPane* eikPane = NULL;
    CAiwMenuPane* aiwPane = CreateAiwMenuPaneLC( &eikPane );
    
    // ResourceFile is empty 
    delete iProvider->iResourceFileName;
    
    iProvider->iResourceFileName = NULL ;

    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
    EUNIT_ASSERT( iProvider->iResourceFileName != NULL )

    // Menu item counter
    TInt menuPaneCount = eikPane->NumberOfItemsInPane();

    // test "forbidden" feature: mus NOT allowed, operator variant set to operator
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EOperatorSpecific );
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMUSForbidden,
                    NMusSessionInformationApi::EMUSForbidden );    
    EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount )

    // set operator back to standard and allow mus
    RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
                    NMusSessionInformationApi::KMUSForbidden ,
                    NMusSessionInformationApi::EMUSAllowed );
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EStandard );


    CMusManager::SetAvailability( MultimediaSharing::EErrCallOnHold );
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );

    //EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )
    MUS_CHECK_MEMORY_LEAVE( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )

    // set camera support on
    FeatureManager::CameraSupported();
    CleanupStack::PopAndDestroy( aiwPane );
    CleanupStack::PopAndDestroy( list );
    BaflUtils::DeleteFile( fs, KResourceFilenameRom() );

    CleanupStack::PopAndDestroy(); // fs
    
    }


void UT_CMusAiwProvider::UT_CMusAiwProvider_InitializeMenuPaneL3L()
    {    
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    BaflUtils::CopyFile( fs, KMusAiwProviderResource(),
                         KResourceFilenameRom() );

    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();

    CEikMenuPane* eikPane = NULL;
    CAiwMenuPane* aiwPane = CreateAiwMenuPaneLC( &eikPane );

    // ResourceFile is empty 
    delete iProvider->iResourceFileName;
    
    iProvider->iResourceFileName = NULL ;
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
    EUNIT_ASSERT( iProvider->iResourceFileName != NULL )
    // Menu item counter
    TInt menuPaneCount = eikPane->NumberOfItemsInPane();

    // test "forbidden" feature: mus NOT allowed, operator variant set to operator
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EOperatorSpecific );
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMUSForbidden,
                    NMusSessionInformationApi::EMUSForbidden );    
    EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount )
 
    // set operator back to standard and allow mus
    RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
                    NMusSessionInformationApi::KMUSForbidden ,
                    NMusSessionInformationApi::EMUSAllowed );
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EStandard );
 

    CMusManager::SetAvailability(MultimediaSharing::ESipOptionsSent);
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
 
    //EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )     
    MUS_CHECK_MEMORY_LEAVE( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )
 
    // set camera support on
    FeatureManager::CameraSupported();
    CleanupStack::PopAndDestroy( aiwPane );
    CleanupStack::PopAndDestroy( list );
    BaflUtils::DeleteFile( fs, KResourceFilenameRom() );

    CleanupStack::PopAndDestroy(); // fs
    
    }


void UT_CMusAiwProvider::UT_CMusAiwProvider_InitializeMenuPaneL4L()
    {    
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    BaflUtils::CopyFile( fs, KMusAiwProviderResource(),
                         KResourceFilenameRom() );

    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();

    CEikMenuPane* eikPane = NULL;
    CAiwMenuPane* aiwPane = CreateAiwMenuPaneLC( &eikPane );

    // ResourceFile is empty 
    delete iProvider->iResourceFileName;
    
    iProvider->iResourceFileName = NULL ;
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
    EUNIT_ASSERT( iProvider->iResourceFileName != NULL )
 
    // Menu item counter
    TInt menuPaneCount = eikPane->NumberOfItemsInPane();

    // test "forbidden" feature: mus NOT allowed, operator variant set to operator
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EOperatorSpecific );
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMUSForbidden,
                    NMusSessionInformationApi::EMUSForbidden );    
    EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount )

    // set operator back to standard and allow mus
    RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
                    NMusSessionInformationApi::KMUSForbidden ,
                    NMusSessionInformationApi::EMUSAllowed );
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EStandard );


    CMusManager::SetAvailability(MultimediaSharing::ESipOptionsNotSent);
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );

    //EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )
    MUS_CHECK_MEMORY_LEAVE( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )

    // set camera support on
    FeatureManager::CameraSupported();
    CleanupStack::PopAndDestroy( aiwPane );
    CleanupStack::PopAndDestroy( list );
    BaflUtils::DeleteFile( fs, KResourceFilenameRom() );

    CleanupStack::PopAndDestroy(); // fs
    
    }

void UT_CMusAiwProvider::UT_CMusAiwProvider_InitializeMenuPaneL5L()
    {    
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    BaflUtils::CopyFile( fs, KMusAiwProviderResource(),
                         KResourceFilenameRom() );

    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();

    CEikMenuPane* eikPane = NULL;
    CAiwMenuPane* aiwPane = CreateAiwMenuPaneLC( &eikPane );

    // ResourceFile is empty 
    delete iProvider->iResourceFileName;
    
    iProvider->iResourceFileName = NULL ;
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
    EUNIT_ASSERT( iProvider->iResourceFileName != NULL )
    // Menu item counter
    TInt menuPaneCount = eikPane->NumberOfItemsInPane();

    // test "forbidden" feature: mus NOT allowed, operator variant set to operator
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EOperatorSpecific );
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMUSForbidden,
                    NMusSessionInformationApi::EMUSForbidden );    
    EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount )

    // set operator back to standard and allow mus
    RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
                    NMusSessionInformationApi::KMUSForbidden ,
                    NMusSessionInformationApi::EMUSAllowed );
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EStandard );


    CMusManager::SetAvailability(MultimediaSharing::EMultimediaSharingAvailable);
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
    //EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )        

    MUS_CHECK_MEMORY_LEAVE( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )




    // set camera support on
    FeatureManager::CameraSupported();
    CleanupStack::PopAndDestroy( aiwPane );
    CleanupStack::PopAndDestroy( list );
    BaflUtils::DeleteFile( fs, KResourceFilenameRom() );

    CleanupStack::PopAndDestroy(); // fs
    
    }

void UT_CMusAiwProvider::UT_CMusAiwProvider_InitializeMenuPaneL6L()
    {    
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    BaflUtils::CopyFile( fs, KMusAiwProviderResource(),
                         KResourceFilenameRom() );

    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();

    CEikMenuPane* eikPane = NULL;
    CAiwMenuPane* aiwPane = CreateAiwMenuPaneLC( &eikPane );

    // ResourceFile is empty 
    delete iProvider->iResourceFileName;
    
    iProvider->iResourceFileName = NULL ;
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
    EUNIT_ASSERT( iProvider->iResourceFileName != NULL )
    // Menu item counter
    TInt menuPaneCount = eikPane->NumberOfItemsInPane();

    // test "forbidden" feature: mus NOT allowed, operator variant set to operator
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EOperatorSpecific );
    RProperty::Set( NMusSessionInformationApi::KCategoryUid,
                    NMusSessionInformationApi::KMUSForbidden,
                    NMusSessionInformationApi::EMUSForbidden );    
    EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount )

    // set operator back to standard and allow mus
    RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
                    NMusSessionInformationApi::KMUSForbidden ,
                    NMusSessionInformationApi::EMUSAllowed );
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EStandard );

    // Cases  for "camera not supported"!
    FeatureManager::CameraNotSupported();
    CMusManager::SetAvailability(MultimediaSharing::EMultimediaSharingAvailable);
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );

    
    //EUNIT_ASSERT( eikPane->NumberOfItemsInPane() == menuPaneCount + 1 )     
    MUS_CHECK_MEMORY_LEAVE( eikPane->NumberOfItemsInPane() == menuPaneCount + 1 )    

    // set camera support on
    FeatureManager::CameraSupported();
    CleanupStack::PopAndDestroy( aiwPane );
    CleanupStack::PopAndDestroy( list );
    BaflUtils::DeleteFile( fs, KResourceFilenameRom() );

    CleanupStack::PopAndDestroy(); // fs
    
    }

void UT_CMusAiwProvider::UT_CMusAiwProvider_InitializeMenuPaneLFastModeL()
    {    
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    BaflUtils::CopyFile( fs, KMusAiwProviderResource(),
                         KResourceFilenameRom() );

    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();

    CEikMenuPane* eikPane = NULL;
    CAiwMenuPane* aiwPane = CreateAiwMenuPaneLC( &eikPane );

    // ResourceFile is empty 
    delete iProvider->iResourceFileName;
    
    iProvider->iResourceFileName = NULL ;
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
    EUNIT_ASSERT( iProvider->iResourceFileName != NULL )
    // Menu item counter
    TInt menuPaneCount = eikPane->NumberOfItemsInPane();

    // set operator back to standard and allow mus
    RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
                    NMusSessionInformationApi::KMUSForbidden ,
                    NMusSessionInformationApi::EMUSAllowed );
    MultimediaSharingSettings::SetOperatorVariantSettingL(
                     MusSettingsKeys::EStandard );
    MultimediaSharingSettings::SetPropertyValueL( 
              MusSettingsKeys::KFastStartupMode, MusSettingsKeys::EFastModeOn );
 
    // Options not enabled, mus shown in menu
    //
    MultimediaSharingSettings::SetCapabilityQuerySettingL(
              MusSettingsKeys::ENoOptions );
    CMusManager::SetAvailability(MultimediaSharing::EMultimediaSharingAvailable);
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
    
    MUS_CHECK_MEMORY_LEAVE( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )

    // Options supported, not yet available, mus not shown in menu
    //
    MultimediaSharingSettings::SetCapabilityQuerySettingL(
                  MusSettingsKeys::EParallel );
    CMusManager::SetAvailability(MultimediaSharing::ESipOptionsSent);
    menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
   
    MUS_CHECK_MEMORY_LEAVE( eikPane->NumberOfItemsInPane() == menuPaneCount )
    
    CleanupStack::PopAndDestroy( aiwPane );
    CleanupStack::PopAndDestroy( list );
    BaflUtils::DeleteFile( fs, KResourceFilenameRom() );

    CleanupStack::PopAndDestroy(); // fs
    
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_CMusAiwProvider_InitializeMenuPaneLFastMode2L()
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );
    
    BaflUtils::CopyFile( fs, KMusAiwProviderResource(),
                         KResourceFilenameRom() );

    CAiwGenericParamList* list = CAiwGenericParamList::NewLC();

    CEikMenuPane* eikPane = NULL;
    CAiwMenuPane* aiwPane = CreateAiwMenuPaneLC( &eikPane );
   
    // ResourceFile is empty 
    delete iProvider->iResourceFileName;
    
    iProvider->iResourceFileName = NULL ;
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );
    EUNIT_ASSERT( iProvider->iResourceFileName != NULL )

    // Options supported, available, mus shown in menu
    //
    MultimediaSharingSettings::SetPropertyValueL( 
              MusSettingsKeys::KFastStartupMode, MusSettingsKeys::EFastModeOn );

    MultimediaSharingSettings::SetCapabilityQuerySettingL(
                  MusSettingsKeys::EParallel );

    CMusManager::SetAvailability(MultimediaSharing::EMultimediaSharingAvailable);
    TInt menuPaneCount = eikPane->NumberOfItemsInPane();
    iProvider->InitializeMenuPaneL( *aiwPane, 0, 0, *list );

    MUS_CHECK_MEMORY_LEAVE( eikPane->NumberOfItemsInPane() == menuPaneCount + 3 )
    
    CleanupStack::PopAndDestroy( aiwPane );
    CleanupStack::PopAndDestroy( list );
    BaflUtils::DeleteFile( fs, KResourceFilenameRom() );

    CleanupStack::PopAndDestroy(); // fs
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_CMusAiwProvider_HandleMenuCmdLL()
    {
    CAiwGenericParamList* pl = CAiwGenericParamList::NewLC();

    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusContinue )

    iProvider->HandleMenuCmdL( EMusCommandLiveShare, *pl, *pl, 0, this );
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusLiveVideo )

    iProvider->HandleMenuCmdL( EMusCommandClipShare, *pl, *pl, 0, this );
    MUS_CHECK_MEMORY_LEAVE( CMusManager::UseCase() == MultimediaSharing::EMusClipVideo  )
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusClipVideo )

    iProvider->HandleMenuCmdL( EMusCommandImageShare, *pl, *pl, 0, this );
    MUS_CHECK_MEMORY_LEAVE( CMusManager::UseCase() == MultimediaSharing::EMusStillImage  )
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusStillImage )

    iProvider->HandleMenuCmdL( EMusCommandContinue, *pl, *pl, 0, this );
    MUS_CHECK_MEMORY_LEAVE( CMusManager::UseCase() == MultimediaSharing::EMusContinue  )
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusContinue )

    CMusManager::SetStartError(
            ( MultimediaSharing::TMusAvailabilityStatus ) KErrNotFound );

    iProvider->HandleMenuCmdL( EMusCommandLiveShare, *pl, *pl, 0, this );
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusContinue )

    iProvider->HandleMenuCmdL( EMusCommandClipShare, *pl, *pl, 0, this );
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusContinue )

    iProvider->HandleMenuCmdL( EMusCommandImageShare, *pl, *pl, 0, this );
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusContinue )

    iProvider->HandleMenuCmdL( EMusCommandContinue, *pl, *pl, 0, this );
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusContinue )

    // revert it back to original state
    CMusManager::SetStartError(
            ( MultimediaSharing::TMusAvailabilityStatus ) KErrNone );

    // Try with unknown command, should leave

    TRAPD( error, iProvider->HandleMenuCmdL( 500, *pl, *pl, 0, this ) );
    if ( error == KErrNoMemory )
        {
        User::Leave( error );
        }
    EUNIT_ASSERT( error == KErrArgument )
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusContinue )

    CleanupStack::PopAndDestroy( pl );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_CMusAiwProvider_HandleMenuCmdLFastModeL()
    {
    CAiwGenericParamList* pl = CAiwGenericParamList::NewLC();
    
    MultimediaSharingSettings::SetPropertyValueL( 
               MusSettingsKeys::KFastStartupMode, MusSettingsKeys::EFastModeOn );
    
    // Fast mode, start live sharing (handle normally)
    CMusManager::SetUseCase( MultimediaSharing::EMusContinue );
    iProvider->HandleMenuCmdL( EMusCommandLiveShare, *pl, *pl, 0, this );
    MUS_CHECK_MEMORY_LEAVE( CMusManager::UseCase() == MultimediaSharing::EMusLiveVideo  )
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusLiveVideo )

    // Fast mode, start clip sharing, app not running nor hidden (handle normally)
    CMusManager::SetUseCase( MultimediaSharing::EMusContinue );
    iProvider->HandleMenuCmdL( EMusCommandClipShare, *pl, *pl, 0, this );
    MUS_CHECK_MEMORY_LEAVE( CMusManager::UseCase() == MultimediaSharing::EMusClipVideo  )
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusClipVideo )
        
    // Fast mode, start clip sharing, app running and hidden (mus mgr is not
    // used for starting application, only usecase P&S value is changed)
    CMusManager::SetUseCase( MultimediaSharing::EMusContinue );
    
    TApaTask::iApplicationExist = ETrue;
    
    TBuf16< 10 > listOkMus; 
    TUint16 upperBits = KMusUiUid >> 16;  
    listOkMus.Append( upperBits );
    listOkMus.Append( KMusUiUid );
    listOkMus.Append( 0 );
    listOkMus.Append( 0 );
    RProperty::Set( KPSUidUikon, KUikAppHiddenList, listOkMus );  
    
    RProperty::Set( NMusSessionApi::KCategoryUid, NMusSessionApi::KUseCase, MultimediaSharing::EMusContinue );
    
    iProvider->HandleMenuCmdL( EMusCommandClipShare, *pl, *pl, 0, this );
    EUNIT_ASSERT( CMusManager::UseCase() == MultimediaSharing::EMusContinue )
    
    TInt usecaseAfterFakeStart( 0 );
    RProperty::Get( NMusSessionApi::KCategoryUid, NMusSessionApi::KUseCase, usecaseAfterFakeStart );
    EUNIT_ASSERT( usecaseAfterFakeStart == MultimediaSharing::EMusClipVideo );
    
    CleanupStack::PopAndDestroy( pl );

    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_CMusAiwProvider_ApplicationRunningLL()
    {
    TApaTask::iApplicationExist = EFalse;
    EUNIT_ASSERT( !iProvider->ApplicationRunningL() )

    // Use stub to mimic the existence of application
    TApaTask::iApplicationExist = ETrue;

    EUNIT_ASSERT( iProvider->ApplicationRunningL() )
    
    // Revert back to old state
    TApaTask::iApplicationExist = EFalse;
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_CMusAiwProvider_ShowErrorNoteLL()
    {
    // Dummy test is enough ShowErrorNoteL does not contain any branches,
    // does not return anything and does not have any side effects that could
    // be asserted
    iProvider->iIsWarning = ETrue ;
    iProvider->ShowErrorNoteL( MultimediaSharing::EErrCallOnHold );
    iProvider->iIsWarning = EFalse ;
    iProvider->ShowErrorNoteL( MultimediaSharing::EErrCallOnHold );

    //Error Case
    iProvider->ShowErrorNoteL( -1 ); // 100 should not be available    
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_CMusAiwProvider_ErrorNoteTextLCL()
    {
    HBufC* errorNote = NULL;

    // It is enough to test with whatever error code since handling is same
    // for all.
    errorNote = iProvider->ErrorNoteTextLC( MultimediaSharing::EErrCallOnHold );
    
    // NULL TEST, old way not support this comparistaion. 
    // Maybe to hard byte compare needed!
    //EUNIT_ASSERT( *errorNote == KMusOnHold() )
    
    CleanupStack::PopAndDestroy( errorNote );

    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_CMusAiwProvider_ErrorNoteText8LL()
    {
    RFs fs;
    User::LeaveIfError( fs.Connect() );
    CleanupClosePushL( fs );

    RResourceFile resourceFile;
    resourceFile.OpenL( fs, KMusAiwProviderResource() );
    CleanupClosePushL( resourceFile );

    HBufC8* errorNote = NULL;

    errorNote = iProvider->ErrorNoteText8L( MultimediaSharing::EErrCallOnHold,
                                            resourceFile );
    CleanupStack::PushL( errorNote );
    EUNIT_ASSERT( *errorNote == KMusOnHold8() )
    CleanupStack::PopAndDestroy( errorNote );


    errorNote = iProvider->ErrorNoteText8L( MultimediaSharing::EErrNetwork,
                                            resourceFile );
    CleanupStack::PushL( errorNote );
    EUNIT_ASSERT( *errorNote == KMusNetworkIncompatible8() )
    CleanupStack::PopAndDestroy( errorNote );


    errorNote = iProvider->ErrorNoteText8L(
                            MultimediaSharing::EErrConferenceCall,
                            resourceFile );
    CleanupStack::PushL( errorNote );
    EUNIT_ASSERT( *errorNote == KMusConferenceCall8() )
    CleanupStack::PopAndDestroy( errorNote );


    errorNote = iProvider->ErrorNoteText8L(
                            MultimediaSharing::ESipOptionsNotCapable,
                            resourceFile );
    CleanupStack::PushL( errorNote );
    EUNIT_ASSERT( *errorNote == KMusRecipientIncapable8() )
    CleanupStack::PopAndDestroy( errorNote );


    errorNote = iProvider->ErrorNoteText8L(
                            MultimediaSharing::EErrSipRegistration,
                            resourceFile );
    CleanupStack::PushL( errorNote );
    EUNIT_ASSERT( *errorNote == KMusNoReg8() )
    CleanupStack::PopAndDestroy( errorNote );


    errorNote = iProvider->ErrorNoteText8L(
                            MultimediaSharing::EErrNoActiveCall,
                            resourceFile );
    CleanupStack::PushL( errorNote );    
    EUNIT_ASSERT( *errorNote == KMusGeneral() )
    CleanupStack::PopAndDestroy( errorNote );

    CleanupStack::PopAndDestroy(); // resourceFile

    CleanupStack::PopAndDestroy(); // fs
    }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_ImplementationGroupProxyL()
    {
    // Dummy test just to raise coverage
    TInt tableCount;
    ImplementationGroupProxy( tableCount );
    }

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void UT_CMusAiwProvider::UT_HiddenFromFastSwapL()
    {
    // Normal mode
    MultimediaSharingSettings::SetPropertyValueL( 
                MusSettingsKeys::KFastStartupMode, MusSettingsKeys::EFastModeOff );
    EUNIT_ASSERT( !iProvider->HiddenFromFastSwapL() )
    
    // Fast mode
    //
    MultimediaSharingSettings::SetPropertyValueL( 
               MusSettingsKeys::KFastStartupMode, MusSettingsKeys::EFastModeOn );
    
    // Key not found
    PropertyHelper::SetErrorCode( KErrNotFound );
    EUNIT_ASSERT( !iProvider->HiddenFromFastSwapL() )
    
    // Malformed key value
    PropertyHelper::SetErrorCode( KErrNone );
    TBuf16< 5 > listMalformed;
    listMalformed.Copy( _L("05645") );
    RProperty::Set( KPSUidUikon, KUikAppHiddenList, listMalformed );   
    MUS_EUNIT_ASSERT_SPECIFIC_LEAVE( iProvider->HiddenFromFastSwapL(), KErrEof );
    
    // No musuid in value list
    TBuf16< 6 > listOkNoMus;
    TUint32 uid1 = 10002000;
    TUint32 uid2 = 20003000;
    // avkoncapserver publishes values in following manner
    TUint16 upperBits = uid1 >> 16;
    listOkNoMus.Append( upperBits );
    listOkNoMus.Append( uid1 );
    upperBits = uid2 >> 16;
    listOkNoMus.Append( upperBits );
    listOkNoMus.Append( uid2 );
    listOkNoMus.Append( 0 );
    listOkNoMus.Append( 0 );
    RProperty::Set( KPSUidUikon, KUikAppHiddenList, listOkNoMus );  
    EUNIT_ASSERT( !iProvider->HiddenFromFastSwapL() )
    
    // Musuid in value list
    TBuf16< 10 > listOkMus;
    TUint32 uid3 = 30005000;
    upperBits = uid1 >> 16;
    listOkMus.Append( upperBits );
    listOkMus.Append( uid1 );
    upperBits = uid2 >> 16;
    listOkMus.Append( upperBits );
    listOkMus.Append( uid2 );
    
    upperBits = KMusUiUid >> 16;  
    listOkMus.Append( upperBits );
    listOkMus.Append( KMusUiUid );
    
    upperBits = uid3 >> 16;
    listOkMus.Append( upperBits );
    listOkMus.Append( uid3 );
    
    listOkMus.Append( 0 );
    listOkMus.Append( 0 );
    RProperty::Set( KPSUidUikon, KUikAppHiddenList, listOkMus );  
    EUNIT_ASSERT( iProvider->HiddenFromFastSwapL() )
    }

// HELPERS

CAiwMenuPane* UT_CMusAiwProvider::CreateAiwMenuPaneLC( CEikMenuPane** aEikPane )
    {
    CEikMenuPane* eikPane = new( ELeave ) CEikMenuPane( this );
    CleanupStack::PushL( eikPane );

    // Constructing eikPane is not compulsory for testing and leads to a memory
    // leak.
    //eikPane->ConstructL( NULL, this );

    CAiwMenuPane* aiwPane = new( ELeave ) CAiwMenuPane( *eikPane, 0 ); //Takes ownership
    CleanupStack::Pop( eikPane );
    CleanupStack::PushL( aiwPane );
    *aEikPane = eikPane;
    return aiwPane;
    }

//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    UT_CMusAiwProvider,
    "CMusAiwProvider",
    "UT_CMusAiwProvider" )

EUNIT_TEST(
    "InitialiseL - test",
    "CMusAiwProvider",
    "InitialiseL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_InitialiseLL, Teardown)

EUNIT_TEST(
    "HandleServiceCmdL - test",
    "CMusAiwProvider",
    "HandleServiceCmdL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_HandleServiceCmdLL, Teardown)

EUNIT_TEST(
    "InitializeMenuPaneL - test",
    "CMusAiwProvider",
    "InitializeMenuPaneL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_InitializeMenuPaneLL, Teardown)

EUNIT_TEST(
    "InitializeMenuPaneL - test2",
    "CMusAiwProvider",
    "InitializeMenuPaneL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_InitializeMenuPaneL2L, Teardown)
    
EUNIT_TEST(
    "InitializeMenuPaneL - test3",
    "CMusAiwProvider",
    "InitializeMenuPaneL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_InitializeMenuPaneL3L, Teardown)
EUNIT_TEST(
    "InitializeMenuPaneL - test4",
    "CMusAiwProvider",
    "InitializeMenuPaneL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_InitializeMenuPaneL4L, Teardown)
EUNIT_TEST(
    "InitializeMenuPaneL - test5",
    "CMusAiwProvider",
    "InitializeMenuPaneL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_InitializeMenuPaneL5L, Teardown)
EUNIT_TEST(
    "InitializeMenuPaneL - test6",
    "CMusAiwProvider",
    "InitializeMenuPaneL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_InitializeMenuPaneL6L, Teardown)

EUNIT_TEST(
    "InitializeMenuPaneL - fast mode",
    "CMusAiwProvider",
    "InitializeMenuPaneL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_InitializeMenuPaneLFastModeL, Teardown)

EUNIT_TEST(
    "InitializeMenuPaneL - fast mode 2",
    "CMusAiwProvider",
    "InitializeMenuPaneL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_InitializeMenuPaneLFastMode2L, Teardown)

EUNIT_TEST(
    "HandleMenuCmdL - test",
    "CMusAiwProvider",
    "HandleMenuCmdL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_HandleMenuCmdLL, Teardown)

EUNIT_TEST(
    "HandleMenuCmdL - fastmode test",
    "CMusAiwProvider",
    "HandleMenuCmdL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_HandleMenuCmdLFastModeL, Teardown)   

EUNIT_TEST(
    "ApplicationRunningL - test",
    "CMusAiwProvider",
    "ApplicationRunningL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_ApplicationRunningLL, Teardown)

EUNIT_TEST(
    "ShowErrorNoteL - test",
    "CMusAiwProvider",
    "ShowErrorNoteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_ShowErrorNoteLL, Teardown)

EUNIT_TEST(
    "ErrorNoteTextLC - test",
    "CMusAiwProvider",
    "ErrorNoteTextLC",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_ErrorNoteTextLCL, Teardown)

EUNIT_TEST(
    "ErrorNoteText8L - test",
    "CMusAiwProvider",
    "ErrorNoteText8L",
    "FUNCTIONALITY",
    SetupL, UT_CMusAiwProvider_ErrorNoteText8LL, Teardown)

EUNIT_TEST(
    "ImplementationGroupProxy",
    "CMusAiwProvider",
    "ImplementationGroupProxy",
    "FUNCTIONALITY",
    SetupL, UT_ImplementationGroupProxyL, Teardown)
    
EUNIT_TEST(
    "HiddenFromFastSwapL",
    "CMusAiwProvider",
    "HiddenFromFastSwapL",
    "FUNCTIONALITY",
    SetupL, UT_HiddenFromFastSwapL, Teardown)

EUNIT_END_TEST_TABLE

