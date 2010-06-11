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
#include "UT_CMusAvaContactAvailability.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "musavaobserver.h"
#include "musavaobserverimp.h"
#include "musavasettingsimp.h"
#include "muslogger.h"
#include "musavadefaultimp.h"
#include "musavacapabilitytesthelper.h"
#include "mussettings.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <cntfldst.h>
#include <cntfield.h>
#include <cntfilt.h>
#include <digia/eunit/eunitdecorators.h>

//  INTERNAL INCLUDES
#include "musavacontactavailability.h"

const TInt KNumberMatchingDigitsRequired( 7 );

_LIT(KNumberZero,"");
_LIT(KNumber,"05012345678");
_LIT(KPlusNumber,"+3585012345678");
_LIT(KFirstName,"Matti");
_LIT(KLastName,"Meikalainen");
_LIT(KFirstLastName,"Matti Meikalainen");
_LIT(KLastFirstName,"Meikalainen Matti");

// CONSTRUCTION
UT_CMusAvaContactAvailability* UT_CMusAvaContactAvailability::NewL()
    {
    UT_CMusAvaContactAvailability* self = UT_CMusAvaContactAvailability::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaContactAvailability* UT_CMusAvaContactAvailability::NewLC()
    {
    UT_CMusAvaContactAvailability* self = new( ELeave ) UT_CMusAvaContactAvailability();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaContactAvailability::~UT_CMusAvaContactAvailability()
    {
    TRAP_IGNORE(
        MultimediaSharingSettings::SetOperatorVariantSettingL( iSaveOperatorVariant ) )    
    }

// Default constructor
UT_CMusAvaContactAvailability::UT_CMusAvaContactAvailability()
    {
    }

// Second phase construct
void UT_CMusAvaContactAvailability::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    iSaveOperatorVariant = MultimediaSharingSettings::OperatorVariantSettingL();    
    }

//  METHODS



void UT_CMusAvaContactAvailability::SetupL(  )
    {
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                    MusSettingsKeys::EStandard );
    
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iContactAvailability  = CMusAvaContactAvailability::NewL( *iAvailabilityObserver,
                                                              *iConcreteSettings );                                                        
    } 
    
void UT_CMusAvaContactAvailability::SetupOpSpecificL()
    {
    MultimediaSharingSettings::SetOperatorVariantSettingL( 
                                    MusSettingsKeys::EOperatorSpecific );
    
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iContactAvailability  = CMusAvaContactAvailability::NewL( *iAvailabilityObserver,
                                                              *iConcreteSettings );                                                        
    }     

void UT_CMusAvaContactAvailability::Teardown(  )
    {
    delete iContactAvailability;
    delete iAvailabilityObserver;
    delete iConcreteSettings;
    
   /* // obtain contact no in contacts DB
    TInt contactNo = iListBox->CurrentItemIndex();
    contactNo = iPositionInContactsDB.operator[](contactNo);

    // connect to contact database
    CContactDatabase* contactsDb = CContactDatabase::OpenL();
    CleanupStack::PushL(contactsDb);

    // set iterator for contacts DB
    TContactIter iter(*contactsDb);
    TContactItemId cardId;

    // index of contacts in contacs DB
    TInt round(0);

    // go to correct contact card
    while( ( cardId = iter.NextL() ) != KNullContactId )
    {
    // if currect index is index of contact we're looking - break
    if(round == contactNo)
      break;
    round++;
    }

    // delete contact
    contactsDb->DeleteContactL(cardId);

    CleanupStack::PopAndDestroy(); // contactsDb

    // some function to recreate and redraw List Box should be called here
    // to reflect changes*/
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_NewLL(  )
    {
  //  EUNIT_ASSERT( iContactAvailability );
    }  
		
void UT_CMusAvaContactAvailability::UT_CMusAvaController_DoExecuteLL()
    {
   // Non cont or phone number added to contact DB
    TRAPD( error, iContactAvailability->DoExecuteL( ) );
    if ( error == KErrNoMemory ) User::Leave( KErrNoMemory );
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && error != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    }
        
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_DoExecuteL1_L()
    {
    }
    
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_DoExecuteL2_L()
    {

    iConcreteSettings->SetTelNumberL( KPlusNumber );
    
    TRAPD( error, iContactAvailability->DoExecuteL() );
    EUNIT_ASSERT( error == KErrNoMemory || error == KErrNone );
    if ( error == KErrNoMemory ) User::Leave( error );
    TInt count;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && error != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    
    iConcreteSettings->SetTelNumberL( KPlusNumber );
        
    TRAP( error, iContactAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && error != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    }
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_DoExecuteL3_L()
    {
    iConcreteSettings->SetTelNumberL( KPlusNumber );
    
    TRAPD( error, iContactAvailability->DoExecuteL() );
    EUNIT_ASSERT( error == KErrNoMemory || error == KErrNone );
    if ( error == KErrNoMemory ) User::Leave( error );
    TInt count;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && error != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    
    iConcreteSettings->SetTelNumberL( KPlusNumber );
        
    TRAP( error, iContactAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && error != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    
    }

    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_NameL()
    {
    EUNIT_ASSERT( iContactAvailability->Name() == MMusAvaObserver::EMusAvaNameContactStatus );
    }
  
void UT_CMusAvaContactAvailability::UT_CMusAvaController_StatusL()
    {
    EUNIT_ASSERT( iContactAvailability->State() == MMusAvaObserver::EMusAvaStatusNotExecuted );
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_StopL()
    {
    //EUNIT_ASSERT( iContactAvailability->Stop() == KErrNone );
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_ActiveCallLL()   
    {
    //No prefix
    CDesCArrayFlat* array = new ( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( array );
    
    iConcreteSettings->SetTelNumberL( KNumber );    
    EUNIT_ASSERT( !iContactAvailability->ActiveCallL( array ) );
    EUNIT_ASSERT( array->MdcaCount() == 0 );


    //Prefix
    iConcreteSettings->SetTelNumberL( KPlusNumber );    
    EUNIT_ASSERT( iContactAvailability->ActiveCallL( array ) );
    EUNIT_ASSERT( array->Count() == 1 )
    array->Reset();
   
    
    //No number
    iConcreteSettings->ReleseTelNumberL();    
    EUNIT_ASSERT( !iContactAvailability->ActiveCallL( array ) );
    EUNIT_ASSERT( array->MdcaCount() == 0 );
    CleanupStack::PopAndDestroy( array );
    }
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ActiveCallLOpSpecificL()   
    {
    //No prefix
    CDesCArrayFlat* array = new ( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( array );
    
    iConcreteSettings->SetTelNumberL( KNumber );    
    EUNIT_ASSERT( iContactAvailability->ActiveCallL( array ) );
    EUNIT_ASSERT( array->MdcaCount() == 1 );
    array->Reset();

    //Prefix
    iConcreteSettings->SetTelNumberL( KPlusNumber );    
    EUNIT_ASSERT( iContactAvailability->ActiveCallL( array ) );
    EUNIT_ASSERT( array->Count() == 1 )
    array->Reset();
    
    //No number
    iConcreteSettings->ReleseTelNumberL();    
    EUNIT_ASSERT( !iContactAvailability->ActiveCallL( array ) );
    EUNIT_ASSERT( array->MdcaCount() == 0 );
    CleanupStack::PopAndDestroy( array );
    }    
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ContactLL()
    {
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_ContactArrayLL()
    {
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_ContactItemL()   
    {
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_PhoneResulutionLL()         
    {   
    }   
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ContactIdL()
    {
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_ContactIdSearchL()
    {
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_ResolveAddressesLL()
    {
    }
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ResolveAddressesLOpSpecificL()
    {
    }
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ResolvePhoneContactLL()  
    {
    }
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ResolvePhoneContactLOpSpecificL()  
    {
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_IsExistAlreadyL()
    {
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_UpdateContactNameLL()
    {
    }
 
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaContactAvailability,
    "CMusAvaContactAvailability",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaContactAvailability",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_NewLL, Teardown)

EUNIT_TEST(
    "DoExecuteL - test ",
    "CMusAvaContactAvailability",
    "DoExecuteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_DoExecuteLL, Teardown) 
    
EUNIT_TEST(
    "DoExecuteL_1 - test ",
    "CMusAvaContactAvailability",
    "DoExecuteL_1",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_DoExecuteL1_L, Teardown) 
    
EUNIT_TEST(
    "DoExecuteL_2 - test ",
    "CMusAvaContactAvailability",
    "DoExecuteL_2",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_DoExecuteL2_L, Teardown) 

EUNIT_TEST(
    "DoExecuteL_3 - test ",
    "CMusAvaContactAvailability",
    "DoExecuteL_3",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_DoExecuteL3_L, Teardown)
    
EUNIT_TEST(
    "Name - test ",
    "CMusAvaController",
    "Name",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_NameL, Teardown)
    
EUNIT_TEST(
    "Status - test ",
    "CMusAvaController",
    "Status",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_StatusL, Teardown)
    
EUNIT_TEST(
    "Stop - test ",
    "CMusAvaController",
    "Stop",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_StopL, Teardown)
    
EUNIT_TEST(
    "ActiveCallL - test ",
    "CMusAvaContactAvailability",
    "ActiveCallL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_ActiveCallLL, Teardown)   
    
EUNIT_TEST(
    "ActiveCallL OpSpecific - test ",
    "CMusAvaContactAvailability",
    "ActiveCallL",
    "FUNCTIONALITY",
    SetupOpSpecificL, UT_CMusAvaController_ActiveCallLOpSpecificL, Teardown)       
    
EUNIT_TEST(
    "ContactL - test ",
    "CMusAvaContactAvailability",
    "ContactL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_ContactLL, Teardown)  
  
EUNIT_TEST(
    "ContactArrayL - test ",
    "CMusAvaContactAvailability",
    "ContactArrayL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_ContactArrayLL, Teardown)  
    
EUNIT_TEST(
    "ContactItem - test ",
    "CMusAvaContactAvailability",
    "ContactItem",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_ContactItemL, Teardown) 
        
EUNIT_TEST(
    "PhoneResolutionL - test ",
    "CMusAvaContactAvailability",
    "PhoneResolutionL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_PhoneResulutionLL, Teardown)
    
EUNIT_TEST(
    "ContactIdSearch - test ",
    "CMusAvaContactAvailability",
    "ContactIdSearch",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_ContactIdSearchL, Teardown)
    
EUNIT_TEST(
    "ContactId - test ",
    "CMusAvaContactAvailability",
    "ContactId",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_ContactIdL, Teardown)
    
EUNIT_TEST(
    "ResolveAddressesL - test ",
    "CMusAvaContactAvailability",
    "ResolveAddressesL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_ResolveAddressesLL, Teardown)

EUNIT_TEST(
    "ResolveAddressesL OpSpecific- test ",
    "CMusAvaContactAvailability",
    "ResolveAddressesL",
    "FUNCTIONALITY",
    SetupOpSpecificL, UT_CMusAvaController_ResolveAddressesLOpSpecificL, Teardown)

EUNIT_TEST(
    "ResolvePhoneContactL - test ",
    "CMusAvaContactAvailability",
    "ResolvePhoneContactL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_ResolvePhoneContactLL, Teardown)

EUNIT_TEST(
    "ResolvePhoneContactL OpSpecific - test ",
    "CMusAvaContactAvailability",
    "ResolvePhoneContactL",
    "FUNCTIONALITY",
    SetupOpSpecificL, UT_CMusAvaController_ResolvePhoneContactLOpSpecificL, Teardown)

EUNIT_TEST(
    "IsExistAlready - test ",
    "CMusAvaContactAvailability",
    "IsExistAlready",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_IsExistAlreadyL, Teardown)

EUNIT_TEST(
    "UpdateContactNameL - test ",
    "CMusAvaContactAvailability",
    "UpdateContactNameL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_UpdateContactNameLL, Teardown)

EUNIT_END_TEST_TABLE

//  END OF FILE

