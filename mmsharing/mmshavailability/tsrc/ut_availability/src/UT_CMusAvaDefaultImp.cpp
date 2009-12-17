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
#include "UT_CMusAvaDefaultImp.h"
#include "musavasettingsimp.h"
#include "musavaavailabilityobserverimp.h"
#include "musavaavailabilitystub.h"
#include "musavaobserverimp.h"
#include "muslogger.h"
#include "musavacapabilitytesthelper.h"
#include "mussesseioninformationapi.h"
#include "mussettings.h"
#include "musavasettingsimp.h"
#include "CSipSseTestTls.h"

#include <e32property.h>

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <CPbkContactItem.h>
#include <CPbkContactEngine.h>
#include <sipstrings.h>
#include <digia/eunit/eunitdecorators.h>

//  INTERNAL INCLUDES
#include "musavadefaultimp.h"

_LIT(KPlusNumber,"+3585012345678");

// CONSTRUCTION
UT_CMusAvaDefaultImp* UT_CMusAvaDefaultImp::NewL()
    {
    UT_CMusAvaDefaultImp* self = UT_CMusAvaDefaultImp::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaDefaultImp* UT_CMusAvaDefaultImp::NewLC()
    {
    UT_CMusAvaDefaultImp* self = new( ELeave ) UT_CMusAvaDefaultImp();
    CleanupStack::PushL( self );
	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaDefaultImp::~UT_CMusAvaDefaultImp()
    {
    }

// Default constructor
UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp()
    {
    }

// Second phase construct
void UT_CMusAvaDefaultImp::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
	}

//  METHODS

void UT_CMusAvaDefaultImp::StartL(  )
    {
    TRAPD( error, RProperty::Define(
                            NMusSessionInformationApi::KCategoryUid,
                            NMusSessionInformationApi::KMUSForbidden,
                            RProperty::EInt,
                            0 ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    TRAP( error, RProperty::Set(
    	NMusSessionInformationApi::KCategoryUid,
    	NMusSessionInformationApi::KMUSForbidden,
    	( TInt ) NMusSessionInformationApi::EMUSAllowed ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    
    SIPStrings::OpenL();
    CSipSseTestTls::OpenL();
    MultimediaSharingSettings::SetActivationSettingL(MusSettingsKeys::EAlwaysActive);
    iDefaultImp = new (ELeave) CMusAvaDefaultImp();
    iMusAvaObserver = CMusAvaObserverImp::NewL();
    iDefaultImp->SetObserver( *iMusAvaObserver );
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    } 

void UT_CMusAvaDefaultImp::Teardown(  )
    {
    delete iDefaultImp;
    delete iMusAvaObserver;
    CSipSseTestTls::Close();
    delete iConcreteSettings;
    SIPStrings::Close();
    PropertyHelper::Close();
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_NewLL(  )
    {
    EUNIT_ASSERT( iDefaultImp );
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_CreateAvailabilityModulesLL()
    {    
    
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
						NMusSessionInformationApi::KMUSForbidden ,
				        NMusSessionInformationApi::EMUSAllowed ));
    
    MultimediaSharingSettings::SetActivationSettingL(MusSettingsKeys::EAlwaysActive);
    TRAPD( error, iDefaultImp->CreateAvailabilityModulesL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iDefaultImp->iAvailabilities.Count() < 9 )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iDefaultImp->iAvailabilities.Count() == 9 );


	User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
                		NMusSessionInformationApi::KMUSForbidden ,
	            		NMusSessionInformationApi::EMUSForbidden ));
    
    TRAP( error, iDefaultImp->CreateAvailabilityModulesL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_ASSERT( iDefaultImp->iAvailabilities.Count() == 4 );
        
        
        
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
	                	NMusSessionInformationApi::KMUSForbidden ,
	                	NMusSessionInformationApi::EMUSAllowed ));
	                	
    MultimediaSharingSettings::SetActivationSettingL(MusSettingsKeys::ENever);
    TRAP( error, iDefaultImp->CreateAvailabilityModulesL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_ASSERT( iDefaultImp->iAvailabilities.Count() == 4 );
    }    
   
void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_ExecuteAvailablitityModulesL()
    {	
    /* ConstuctL will construct all availability Modules */
    iDefaultImp->ConstructL();
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && 
        iDefaultImp->iAvailabilities.Count() < 9 )
        {
        User::Leave( KErrNoMemory );
        }
    /* Execute and Check all the availability Modules */
	iDefaultImp->ExecuteAvailablitityModules();
    EUNIT_ASSERT( iDefaultImp->iAvailabilities.Count() == 9 );
    
    MMusAvaObserver::TAvailabilityStatus state;
    for ( TInt i = 0; i < iDefaultImp->iAvailabilities.Count(); i++ )
        {
        state = iDefaultImp->iAvailabilities[i]->State();
        EUNIT_ASSERT( state !=MMusAvaObserver::EMusAvaStatusNotExecuted || 
                      state != MMusAvaObserver::EMusAvaStatusInProgress );
        }
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_ConstuctHandlerAvailabilityModulesLL()
    {
    TRAPD( error, iDefaultImp->ConstuctHandlerAvailabilityModulesL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    EUNIT_ASSERT( iDefaultImp->iAvailabilities.Count() == 2 );
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_ConstructStaticAvailabilityModulesLL()
	{
    TRAPD( error, iDefaultImp->ConstructStaticAvailabilityModulesL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    EUNIT_ASSERT( iDefaultImp->iAvailabilities.Count() == 2 );	
	}

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_ConstructDynamicAvailabilityModulesLL()
    {
    TRAPD( error, iDefaultImp->ConstructDynamicAvailabilityModulesL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    EUNIT_ASSERT( iDefaultImp->iAvailabilities.Count() == 5 );	
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_SettingsL()
    {
    /* ConstuctL will construct all availability Modules */
    iDefaultImp->ConstructL();
    MMusAvaSettings& settings = iDefaultImp->Settings();
    EUNIT_ASSERT( &settings );
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_SetObserverL()
    {
    CMusAvaObserverImp* temp = CMusAvaObserverImp::NewL();
    CleanupStack::PushL( temp );
    iDefaultImp->SetObserver( *temp );
    delete iMusAvaObserver;
    iMusAvaObserver = temp;
    CleanupStack::Pop( temp );
    EUNIT_ASSERT( iDefaultImp->iObserver == iMusAvaObserver);
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_StartLL()
    {
    iDefaultImp->ConstructL();
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && 
        iDefaultImp->iAvailabilities.Count() < 9 )
        {
        User::Leave( KErrNoMemory );
        }
    iDefaultImp->StartL();
    EUNIT_ASSERT( iDefaultImp->iAvailabilities.Count() == 9 );    
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_StopL()
    {
    iDefaultImp->ConstructL();
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && 
        iDefaultImp->iAvailabilities.Count() < 9 )
        {
        User::Leave( KErrNoMemory );
        }
    iDefaultImp->StartL();
    EUNIT_ASSERT( iDefaultImp->iAvailabilities.Count() == 9 );
    iDefaultImp->Stop();
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_AvailabilityChangedL()
    {
    MMusAvaObserver::TAvailabilityStatus status = MMusAvaObserver::EMusAvaStatusAvailable;
    MMusAvaObserver::TAvailabilityName name = MMusAvaObserver::EMusAvaNameContactStatus;

    iDefaultImp->ConstructL();
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && 
        iDefaultImp->iAvailabilities.Count() < 9 )
        {
        User::Leave( KErrNoMemory );
        }
    iDefaultImp->StartL();
    EUNIT_ASSERT( iDefaultImp->iAvailabilities.Count() == 9 );

    //delete iDefaultImp->iObserver;
    delete iMusAvaObserver;
    iMusAvaObserver = NULL;
    iDefaultImp->iObserver = NULL;
    iDefaultImp->AvailabilityChanged(name,status);

    iMusAvaObserver = CMusAvaObserverImp::NewL();
    iDefaultImp->SetObserver( *iMusAvaObserver );
    iDefaultImp->iStopping=ETrue;
    iDefaultImp->AvailabilityChanged(name,status);

    iDefaultImp->iStopping=EFalse;    
    status=MMusAvaObserver::EMusAvaConnectionErr;
    iDefaultImp->AvailabilityChanged(name,status);

    iDefaultImp->iStopping=EFalse;    
    status=MMusAvaObserver::EMusAvaStatusAvailable;
    iDefaultImp->iExecuteStarted=ETrue;
    iDefaultImp->AvailabilityChanged(name,status);
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_AvailabilityErrorL()
    {
    MMusAvaObserver::TAvailabilityStatus status = iDefaultImp->AvailabilityStatus();
    MMusAvaObserver::TAvailabilityName name = iDefaultImp->CurrentAvailability();
    EUNIT_ASSERT( name == MMusAvaObserver::EMusAvaNameNotDefined );
    EUNIT_ASSERT( status == MMusAvaObserver::EMusAvaStatusNotExecuted );
    
    /* ConstuctL will construct all availability Modules */
    iDefaultImp->ConstructL();
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && 
        iDefaultImp->iAvailabilities.Count() < 9 )
        {
        User::Leave( KErrNoMemory );
        }
    iDefaultImp->StartL();
    EUNIT_ASSERT( iDefaultImp->iAvailabilities.Count() > 0 );
    
    for ( TInt i = 0; i<iDefaultImp->iAvailabilities.Count(); i++ )
        {
        iDefaultImp->iAvailabilities[i]->SetState( MMusAvaObserver::EMusAvaStatusAvailable );
        }
    TBool min = iDefaultImp->MinimumAvailability();
    TBool man = iDefaultImp->MandatoryAvailability();
    TBool ext = iDefaultImp->ExtensionAvailability();
    EUNIT_ASSERT( ext == ETrue );
    
    status = iDefaultImp->AvailabilityStatus();
    name = iDefaultImp->CurrentAvailability();
    EUNIT_ASSERT(name == MMusAvaObserver::EMusAvaFullAvailability);
    
    iDefaultImp->iAvailabilities[iDefaultImp->iAvailabilities.Count()-1]->SetState( 
                                                    MMusAvaObserver::EMusAvaOptionNotAvailable );

    iDefaultImp->AvailabilityError( iDefaultImp->iAvailabilities[iDefaultImp->iAvailabilities.Count()-1]->Name(), 
        MMusAvaObserver::EMusAvaOptionNotAvailable );
    
    CMusAvaObserverImp* observer = static_cast<CMusAvaObserverImp*>(iDefaultImp->iObserver);
    EUNIT_ASSERT( observer->iStatus == MMusAvaObserver::EMusAvaOptionNotAvailable );
    EUNIT_ASSERT( observer->iName == MMusAvaObserver::EMusAvaOptionHandler);  
              
    status = iDefaultImp->AvailabilityStatus();
    name = iDefaultImp->CurrentAvailability();
    EUNIT_ASSERT( status == MMusAvaObserver::EMusAvaOptionNotAvailable );
    EUNIT_ASSERT( name == MMusAvaObserver::EMusAvaOptionHandler);  
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_AvailableL()
    {
    MMusAvaObserver::TAvailabilityStatus status = iDefaultImp->AvailabilityStatus();
    MMusAvaObserver::TAvailabilityName name = iDefaultImp->CurrentAvailability();
    EUNIT_ASSERT( name == MMusAvaObserver::EMusAvaNameNotDefined );
    EUNIT_ASSERT( status == MMusAvaObserver::EMusAvaStatusNotExecuted );
    
    /* ConstuctL will construct all availability Modules */
    iDefaultImp->ConstructL();
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && 
        iDefaultImp->iAvailabilities.Count() < 9 )
        {
        User::Leave( KErrNoMemory );
        }
    iDefaultImp->StartL();
    EUNIT_ASSERT( !iDefaultImp->Available(MMusAvaObserver::EMusAvaOptionHandler) );
    EUNIT_ASSERT( iDefaultImp->Available(MMusAvaObserver::EMusAvaInviteHandler) );
    }

void UT_CMusAvaDefaultImp::UT_CMusAvaDefaultImp_AvailabilityStateL()
    {
    MMusAvaObserver::TAvailabilityStatus status = iDefaultImp->AvailabilityStatus();
    MMusAvaObserver::TAvailabilityName name = iDefaultImp->CurrentAvailability();
    EUNIT_ASSERT( name == MMusAvaObserver::EMusAvaNameNotDefined );
    EUNIT_ASSERT( status == MMusAvaObserver::EMusAvaStatusNotExecuted );
    
    /* ConstuctL will construct all availability Modules */
    iDefaultImp->ConstructL();
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && 
        iDefaultImp->iAvailabilities.Count() < 9 )
        {
        User::Leave( KErrNoMemory );
        }
    
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    EUNIT_ENABLE_ALLOC_DECORATOR;
    CleanupStack::PushL( contactEngine );

    // dummy initialization, other ways CPbkContactEngine leaks memory
    CPbkContactItem* pbkItem = NULL;
    TRAP_IGNORE( pbkItem = contactEngine->ReadContactL( 1 ) );
    delete pbkItem;
    
    CPbkContactItem*  contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 

    //contact  
    TPbkContactItemField* fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
    
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactEngine->AddNewContactL( *contactItem );
    EUNIT_ENABLE_ALLOC_DECORATOR;
    CleanupStack::PopAndDestroy( contactItem );
    
    iConcreteSettings->SetTelNumberL( KPlusNumber() );
    
    iDefaultImp->StartL();
    EUNIT_ASSERT( iDefaultImp->AvailabilityState(
        MMusAvaObserver::EMusAvaOptionHandler)
        == MMusAvaObserver::EMusAvaStatusNotExecuted );
    EUNIT_ASSERT( iDefaultImp->AvailabilityState(
        MMusAvaObserver::EMusAvaInviteHandler) 
        == MMusAvaObserver::EMusAvaStatusAvailable );
    CleanupStack::PopAndDestroy( contactEngine );
   
   }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaDefaultImp,
    "CMusAvaDefaultImp",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaDefaultImp",
    "NewL",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_NewLL, Teardown)
    
EUNIT_TEST(
    "CreateAvailabilityModulesL - test ",
    "CMusAvaDefaultImp",
    "CreateAvailabilityModulesL",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_CreateAvailabilityModulesLL, Teardown)
    
EUNIT_TEST(
    "ConstructHandlerAvailabilityModulesL - test ",
    "CMusAvaDefaultImp",
    "ConstuctHandlerAvailabilityModulesL",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_ConstuctHandlerAvailabilityModulesLL, Teardown)

EUNIT_TEST(
    "ConstructStaticAvailabilityModulesL - test ",
    "CMusAvaDefaultImp",
    "ConstructStaticAvailabilityModulesL",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_ConstructStaticAvailabilityModulesLL, Teardown)

EUNIT_TEST(
    "ConstructDynamicAvailabilityModulesL - test ",
    "CMusAvaDefaultImp",
    "ConstructDynamicAvailabilityModulesL",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_ConstructDynamicAvailabilityModulesLL, Teardown)

EUNIT_TEST(
    "ExecuteAvailablitityModules - test ",
    "CMusAvaDefaultImp",
    "ExecuteAvailablitityModules",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_ExecuteAvailablitityModulesL, Teardown)

EUNIT_TEST(
    "Settings - test ",
    "CMusAvaDefaultImp",
    "Settings",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_SettingsL, Teardown)
    
EUNIT_TEST(
    "SetObserver - test ",
    "CMusAvaDefaultImp",
    "SetObserver",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_SetObserverL, Teardown)
    
 EUNIT_TEST(
    "StartL - test ",
    "CMusAvaDefaultImp",
    "StartL",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_StartLL, Teardown)       

 EUNIT_TEST(
    "Stop - test ",
    "CMusAvaDefaultImp",
    "Stop",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_StopL, Teardown)
    
 EUNIT_TEST(
    "AvailabilityChanged - test ",
    "CMusAvaDefaultImp",
    "AvailabilityChanged",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_AvailabilityChangedL, Teardown)
    
 EUNIT_TEST(
    "AvailabilityError - test ",
    "CMusAvaDefaultImp",
    "AvailabilityError",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_AvailabilityErrorL, Teardown)
    
EUNIT_TEST(
    "Available - test ",
    "CMusAvaDefaultImp",
    "Available",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_AvailableL, Teardown)    
    
EUNIT_TEST(
    "AvailabilityState - test ",
    "CMusAvaDefaultImp",
    "AvailabilityState",
    "FUNCTIONALITY",
    StartL, UT_CMusAvaDefaultImp_AvailabilityStateL, Teardown)    
    
EUNIT_END_TEST_TABLE

//  END OF FILE
