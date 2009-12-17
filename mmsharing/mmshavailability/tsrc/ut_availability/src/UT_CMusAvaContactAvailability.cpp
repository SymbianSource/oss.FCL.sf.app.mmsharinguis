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
#include <CPbkContactItem.h>
#include <CPbkContactEngine.h>
#include <CPbkFieldInfo.h>
#include <cntfldst.h>
#include <cntfield.h>
#include <PbkFields.hrh>
#include <CPbkFieldsInfo.h>
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
_LIT( KTelNumber, "12345" );

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
    CContactIdArray* contactIdArray = NULL;
    CPbkContactItem*  contactItem = NULL;
    CPbkContactItem*  contactItemTmp = NULL;
    
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( contactEngine );

    // dummy initialization, other ways CPbkContactEngine leaks memory
    CPbkContactItem* pbkItem = NULL;
    TRAP_IGNORE( pbkItem = contactEngine->ReadContactL( 1 ) );
    delete pbkItem;
    
    contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 
        
     //contact  
    TPbkContactItemField* fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
    
    contactEngine->AddNewContactL( *contactItem );
    EUNIT_ENABLE_ALLOC_DECORATOR;
    
    iConcreteSettings->SetTelNumberL( KPlusNumber() );
    
    TRAPD( error, iContactAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    TInt count;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && error != KErrNone )
        {
        User::Leave( KErrNoMemory );
        }
    CleanupStack::PopAndDestroy( contactItem );
    CleanupStack::PopAndDestroy( contactEngine );
    }
    
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_DoExecuteL2_L()
    {
    CContactIdArray* contactIdArray = NULL;
    CPbkContactItem*  contactItem = NULL;
    CPbkContactItem*  contactItemTmp = NULL;
    
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( contactEngine );

    // dummy initialization, other ways CPbkContactEngine leaks memory
    CPbkContactItem* pbkItem = NULL;
    TRAP_IGNORE( pbkItem = contactEngine->ReadContactL( 1 ) );
    delete pbkItem;
    
    contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 
        
    CContactTextField* urlAtSignField = 
    EditableContactFieldL( *contactEngine, *contactItem, EPbkFieldIdSIPID );
    if ( urlAtSignField )
        {
        urlAtSignField->SetTextL( _L("sip:test@10.21.30.50") );
        }
    else
        {
        EUNIT_ASSERT( !urlAtSignField );
        }
    EUNIT_ENABLE_ALLOC_DECORATOR;

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
    
    CleanupStack::PopAndDestroy( contactItem );
    CleanupStack::PopAndDestroy( contactEngine );
    }
void UT_CMusAvaContactAvailability::UT_CMusAvaController_CallConnectedLL()
    {
    TRAPD( error,  iContactAvailability->CallConnectedL( KTelNumber ) );
    if ( error == KErrNone )
    	{
    	EUNIT_ASSERT_EQUALS( iContactAvailability->State(), 
                         MMusAvaObserver::EMusAvaStatusAvailable );
    	EUNIT_ASSERT_EQUALS( iConcreteSettings->TelNumber(), KTelNumber ); 
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
    CContactIdArray* contactIdArray = NULL;
    CPbkContactItem*  contactItem = NULL;

    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    EUNIT_ENABLE_ALLOC_DECORATOR;
    CleanupStack::PushL( contactEngine );
    
    // Non cont or phone number added to contact DB
    TBool ret;
    TRAPD( error, ret = iContactAvailability->ContactL( contactIdArray, *contactEngine ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( !ret );
    EUNIT_ASSERT( contactIdArray == NULL );
    
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 
        
     //contact  
    TPbkContactItemField* fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
    
    contactEngine->AddNewContactL( *contactItem );
    EUNIT_ENABLE_ALLOC_DECORATOR;
    
    iConcreteSettings->SetTelNumberL( KPlusNumber );
    
    TRAP( error, ret = iContactAvailability->ContactL( contactIdArray, *contactEngine ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret );
    EUNIT_ASSERT( contactIdArray != NULL );

    delete contactIdArray;
    contactIdArray = NULL;    
    
    CleanupStack::PopAndDestroy( contactItem );
    CleanupStack::PopAndDestroy( contactEngine );
    
    }
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ContactArrayLL()
    {
    CContactIdArray* array = NULL;
    
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* pbkContactEngine = CPbkContactEngine::NewL();
    EUNIT_ENABLE_ALLOC_DECORATOR;
    CleanupStack::PushL( pbkContactEngine );
    
    //No number found
    TInt ret = NULL;
    TRAPD( error, ret = iContactAvailability->ContactArrayL( array, 
        *pbkContactEngine ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( ret == KErrNotFound );
    EUNIT_ASSERT( array == NULL );
    //Number length 0
    iConcreteSettings->SetTelNumberL( _L("") );
    TRAP( error, ret = iContactAvailability->ContactArrayL( array, 
        *pbkContactEngine ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( ret == KErrNotFound );
    EUNIT_ASSERT( array == NULL );
    //Number found
    iConcreteSettings->SetTelNumberL( KNumber );
    TRAP( error, ret = iContactAvailability->ContactArrayL( array, 
        *pbkContactEngine ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( ret == KErrNone );
    EUNIT_ASSERT( array != NULL );
    EUNIT_ASSERT( array->Count() > 0 );

    CleanupStack::PopAndDestroy( pbkContactEngine );

    delete array;
    array = NULL;
    }
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ContactItemL()   
    {
    CPbkContactItem* tmpItem = NULL;
    CContactIdArray* tmpContactIdArray = NULL;
    
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( contactEngine );
    
    CPbkContactItem* contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 
    
    // CContactIdArray == 0
    //tmpContactIdArray = CContactIdArray::NewLC();
    //EUNIT_ASSERT_EQUALS( KErrNotFound, iContactAvailability->ContactItem( 
    //*tmpContactIdArray, tmpItem ) ); 
    //CleanupStack::PopAndDestroy( ); 
    
    
    // CContactIdArray  > 0
    TPbkContactItemField* fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
    contactEngine->AddNewContactL( *contactItem );

    CContactDatabase* contactDatabase = CContactDatabase::OpenL(); 
    CleanupStack::PushL( contactDatabase );  
    CCntFilter* newFilter = CCntFilter::NewL();
    CleanupStack::PushL( newFilter );
    newFilter->Reset();
    newFilter->SetContactFilterTypeALL( ETrue );
    contactDatabase->FilterDatabaseL( *newFilter );
    EUNIT_ENABLE_ALLOC_DECORATOR;
    
    //CContactIdArray* tmpaArray = newFilter->iIds;
    //Item found
    
    TInt err = iContactAvailability->ContactItem( *( newFilter->iIds ), tmpItem, *contactEngine );
    // NOTE: Multiple contact id's could be in database and KErrNotSupported is 
    // returned if more than one. Maybe first run could get result KErrNone and 
    // then KErrNotSupported
    EUNIT_ASSERT( ( KErrNotSupported == err ) || ( KErrNone == err ) );  

    //EUNIT_ASSERT( tmpItem != NULL );
    //delete tmpaArray;
    //contactDatabase->CommitContactL( *tmpItem );
    delete tmpItem;
    //tmpItem = NULL;   
    //End test
  
    CleanupStack::PopAndDestroy( newFilter );
    
    contactDatabase->CloseTables();
    
    CleanupStack::PopAndDestroy( contactDatabase );
    CleanupStack::PopAndDestroy( contactItem );
    CleanupStack::PopAndDestroy( contactEngine );
    

    
    }
void UT_CMusAvaContactAvailability::UT_CMusAvaController_PhoneResulutionLL()         
    {   
    CDesCArrayFlat* array = new ( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( array ); 
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( contactEngine );
    CPbkContactItem* contactItem = contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 

    // dummy initialization, other ways CPbkContactEngine leaks memory
    CPbkContactItem* pbkItem = NULL;
    TRAP_IGNORE( pbkItem = contactEngine->ReadContactL( 1 ) );
    delete pbkItem;

     //No prefix
    contactItem->RemoveDefaultPhoneNumberField();
    EUNIT_ENABLE_ALLOC_DECORATOR;
    iConcreteSettings->SetTelNumberL( KNumber );    
    TBool ret; 
    TRAPD( error, ret = iContactAvailability->PhoneResolutionL( *contactItem, array ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( !ret );
    
    EUNIT_ASSERT( array->MdcaCount() == 0);
   
    //Prefix
    iConcreteSettings->SetTelNumberL( KPlusNumber );
        
    TRAP( error, ret = iContactAvailability->PhoneResolutionL( *contactItem, array ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( ret );
    EUNIT_ASSERT( array->MdcaCount() > 0 );
    //remove object from the array
    array->Reset();
    
    ///Number plan is in national mode
    EUNIT_DISABLE_ALLOC_DECORATOR;
    TPbkContactItemField* fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL( KNumber() );
    contactEngine->AddNewContactL( *contactItem );
    EUNIT_ENABLE_ALLOC_DECORATOR;

    iConcreteSettings->ReleseTelNumberL();
    TRAP( error, ret = iContactAvailability->PhoneResolutionL( *contactItem, array ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( !ret );
    EUNIT_ASSERT( array->MdcaCount() == 0 );


    //Number plan is in internatiolan mode 
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactItem->RemoveDefaultPhoneNumberField();
    fieldMobile = contactItem->FindField( EPbkFieldIdPhoneNumberMobile );
    fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
    contactEngine->AddNewContactL( *contactItem );
    EUNIT_ENABLE_ALLOC_DECORATOR;
    iConcreteSettings->ReleseTelNumberL();
    TRAP( error, ret = iContactAvailability->PhoneResolutionL( *contactItem, array ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( ret );
    EUNIT_ASSERT( array->MdcaCount() > 0 );
  
    CleanupStack::PopAndDestroy( contactItem );
    CleanupStack::PopAndDestroy( contactEngine ); 
    CleanupStack::PopAndDestroy( array );                                               
    }   

    

    
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ContactIdL()
    {
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( contactEngine ); 
    
    CPbkContactItem* contactItem = NULL;
    
    // dummy initialization, other ways CPbkContactEngine leaks memory
    TRAP_IGNORE( contactItem = contactEngine->ReadContactL( 1 ) );
    delete contactItem;
    
    contactItem = contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 
    EUNIT_ENABLE_ALLOC_DECORATOR;
    
    //Contact field not created
    EUNIT_ASSERT( !iContactAvailability->ContactId( *contactItem, EPbkFieldIdSIPID ) );
    
    //Contact URi not defined
    CContactTextField* urlFieldNotDefined = 
    EditableContactFieldL( *contactEngine, *contactItem, EPbkFieldIdShareView );
    if ( urlFieldNotDefined )
        {
        urlFieldNotDefined->SetTextL( _L("sip:10.21.32.51") );
        }
    else
        {
        EUNIT_ASSERT( !urlFieldNotDefined );
        }
        
    EUNIT_ASSERT( !iContactAvailability->ContactId( *contactItem, EPbkFieldIdShareView ) );
     
    //Contact field not found
    CContactTextField* urlField = 
    EditableContactFieldL( *contactEngine, *contactItem, EPbkFieldIdShareView );
    if ( urlField )
        {
        urlField->SetTextL( _L("test@10.21.32.51") );
        }
    else
        {
        EUNIT_ASSERT( !urlField );
        }
        
    EUNIT_ASSERT( !iContactAvailability->ContactId( *contactItem, EPbkFieldIdSIPID ) );
    
    //Contact field found
    EUNIT_ASSERT( iContactAvailability->ContactId( *contactItem, EPbkFieldIdShareView ) );
        
    CleanupStack::PopAndDestroy( contactItem );
    CleanupStack::PopAndDestroy( contactEngine );
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_ContactIdSearchL()
    {
    CPbkContactItem* contactItem = NULL;
    CContactIdArray* contactIdArray = NULL;
    CContactTextField* phoneNumberMobiled = NULL;
    CContactTextField* country = NULL;
    CContactTextField* urlFieldNotDefined = NULL;
    CPbkContactEngine* contactEngine = NULL;
    
    
     //No contact //////////////////////////////////////////////////////////////
    //General info
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( contactEngine ); 
    
    //Clear contacts 
    contactIdArray = contactEngine->MatchPhoneNumberL(
                                              KNumber(),
                                             KNumberMatchingDigitsRequired );                                       
    CleanupStack::PushL( contactIdArray );                                          
    contactEngine->DeleteContactsL( *contactIdArray );                                        

    CleanupStack::PopAndDestroy( contactIdArray );

    
    // dummy initialization, other ways CPbkContactEngine leaks memory
    TRAP_IGNORE( contactItem = contactEngine->ReadContactL( 1 ) );
    delete contactItem; 
    
    AddToMyContact( *contactEngine,
                    KNumber(), 
                    EPbkFieldIdPhoneNumberMobile,
                    _L("nokia"), 
                    EPbkFieldIdCompanyName );    
                      
    contactIdArray = contactEngine->MatchPhoneNumberL(
                                              KNumber(),
                                             KNumberMatchingDigitsRequired );   
    CleanupStack::PushL( contactIdArray );                                          
    EUNIT_ENABLE_ALLOC_DECORATOR;
                                             
    EUNIT_ASSERT( 
        iContactAvailability->ContactIdSearch( *contactIdArray, *contactEngine ) == 
        EPbkFieldIdNone );
    
    contactEngine->DeleteContactsL( *contactIdArray );  
    CleanupStack::PopAndDestroy( contactIdArray );
    
    // Field Phone number general found /////////////////////////////////////////////////////////
     
    //Clear contacts 
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactIdArray = contactEngine->MatchPhoneNumberL(
                                              KNumber(),
                                             KNumberMatchingDigitsRequired );                                       
    CleanupStack::PushL( contactIdArray );                                          
    contactEngine->DeleteContactsL( *contactIdArray );                                        

    CleanupStack::PopAndDestroy( contactIdArray );
    
    AddToMyContact( *contactEngine,
                    KNumber(), 
                    EPbkFieldIdPhoneNumberMobile,
                    _L("+35805123456789"), 
                    EPbkFieldIdPhoneNumberGeneral );    
        
    contactIdArray = contactEngine->MatchPhoneNumberL(
                                              KNumber(),
                                             KNumberMatchingDigitsRequired );
        
    CleanupStack::PushL( contactIdArray );                                          
    EUNIT_ENABLE_ALLOC_DECORATOR;
                                             
    EUNIT_ASSERT( 
        iContactAvailability->ContactIdSearch( *contactIdArray, *contactEngine ) == 
        EPbkFieldIdNone );
    //EUNIT_ASSERT( ret == EPbkFieldIdPhoneNumberGeneral );
    contactEngine->DeleteContactsL( *contactIdArray );  
    CleanupStack::PopAndDestroy( contactIdArray );
 
    // Field SIP id found /////////////////////////////////////////////////////////
     
    //Clear contacts 
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactIdArray = contactEngine->MatchPhoneNumberL(
                                              KNumber(),
                                             KNumberMatchingDigitsRequired );                                       
    CleanupStack::PushL( contactIdArray );                                          
    contactEngine->DeleteContactsL( *contactIdArray );                                        

    CleanupStack::PopAndDestroy( contactIdArray );
    
    AddToMyContact( *contactEngine,
                    KNumber(), 
                    EPbkFieldIdPhoneNumberMobile,
                    _L("test@10.21.32.51"), 
                    EPbkFieldIdSIPID );    
        
    contactIdArray = contactEngine->MatchPhoneNumberL(
                                              KNumber(),
                                             KNumberMatchingDigitsRequired );
        
    CleanupStack::PushL( contactIdArray );                                          
    EUNIT_ENABLE_ALLOC_DECORATOR;
                                             
    TPbkFieldId ret = iContactAvailability->ContactIdSearch( *contactIdArray, *contactEngine );
    if ( ret == EPbkFieldIdNone )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( ret == EPbkFieldIdSIPID );
    contactEngine->DeleteContactsL( *contactIdArray );  
    CleanupStack::PopAndDestroy( contactIdArray );
 
    // Share View found 
    //Clear contacts 
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactIdArray = contactEngine->MatchPhoneNumberL(
                                              KNumber(),
                                             KNumberMatchingDigitsRequired );                                       
    CleanupStack::PushL( contactIdArray );                                          
    contactEngine->DeleteContactsL( *contactIdArray );                                        

    CleanupStack::PopAndDestroy( contactIdArray );

    AddToMyContact( *contactEngine,
                    KNumber(), 
                    EPbkFieldIdPhoneNumberMobile,
                    _L("test@10.21.32.51"), 
                    EPbkFieldIdShareView );    
        
    contactIdArray = contactEngine->MatchPhoneNumberL(
                                              KNumber(),
                                             KNumberMatchingDigitsRequired );
        
    CleanupStack::PushL( contactIdArray );                                          
    EUNIT_ENABLE_ALLOC_DECORATOR;
                                             
    ret = iContactAvailability->ContactIdSearch( *contactIdArray, *contactEngine );
    if ( ret == EPbkFieldIdNone )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( ret == EPbkFieldIdShareView );
    
    contactEngine->DeleteContactsL( *contactIdArray );  
    CleanupStack::PopAndDestroy( contactIdArray );
    CleanupStack::PopAndDestroy( contactEngine );
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_ResolveAddressesLL()
    {
    CDesCArrayFlat* array = new ( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( array ); 
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( contactEngine ); 
    
    // dummy initialization, other ways CPbkContactEngine leaks memory
    CPbkContactItem* pbkItem = NULL;
    TRAP_IGNORE( pbkItem = contactEngine->ReadContactL( 1 ) );
    delete pbkItem;
    
    CPbkContactItem* contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 
    EUNIT_ENABLE_ALLOC_DECORATOR;
    
    //Test cases
     //Sip Uri without At sign
    CContactTextField* urlSipField = 
    EditableContactFieldL( *contactEngine, *contactItem, EPbkFieldIdSIPID );
  
    
     if ( urlSipField )
        {
        urlSipField->SetTextL( _L("sip:10.21.32.51") );
        }
    else
        {
        EUNIT_ASSERT( !urlSipField );
        }
  
    TBool ret;
    TRAPD( error, ret = iContactAvailability->ResolveAddressesL( *contactItem, 
                                             array,
                                             EPbkFieldIdSIPID  ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( !ret );
    EUNIT_ASSERT( array->Count() == 0 );
   
    
    //Undefined Uri
    CContactTextField* urlUndefinedField = 
    EditableContactFieldL( *contactEngine, *contactItem, EPbkFieldIdPushToTalk );
    if ( urlUndefinedField )
        {
        urlUndefinedField->SetTextL( _L("test@10.21.32.51") );
        }
    else
        {
        EUNIT_ASSERT( !urlUndefinedField );
        }
    contactEngine->AddNewContactL( *contactItem );
    TRAP( error, ret = iContactAvailability->ResolveAddressesL( *contactItem, 
                                             array,
                                             EPbkFieldIdSIPID ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( !ret );
                                                 
    EUNIT_ASSERT( array->Count() == 0 );
    
      //Sip Uri
    CContactTextField* urlAtSignField = 
    EditableContactFieldL( *contactEngine, *contactItem, EPbkFieldIdSIPID );
    if ( urlAtSignField )
        {
        urlAtSignField->SetTextL( _L("sip:test@10.21.32.51") );
        }
    else
        {
        EUNIT_ASSERT( !urlAtSignField );
        }
    contactEngine->AddNewContactL( *contactItem );
    TRAP( error, ret = iContactAvailability->ResolveAddressesL( *contactItem, 
                                             array,
                                             EPbkFieldIdSIPID ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( ret );
    EUNIT_ASSERT( array->Count() > 0 );
    
    //Clear array
    array->Reset();
    
    //ShareView Uri
    CContactTextField* urlShareViewField = 
    EditableContactFieldL( *contactEngine, *contactItem, EPbkFieldIdShareView );
    if ( urlShareViewField )
        {
        urlShareViewField->SetTextL( _L("test@10.21.32.51") );
        }
    else
        {
        EUNIT_ASSERT( !urlShareViewField );
        }
  
    TRAP( error, ret = iContactAvailability->ResolveAddressesL( *contactItem, 
                                             array,
                                             EPbkFieldIdShareView ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( ret );
    EUNIT_ASSERT( array != NULL );
    EUNIT_ASSERT( array->Count() > 0 );
    array->Reset();  
  
 
    //End test
    CleanupStack::PopAndDestroy( contactItem );
    CleanupStack::PopAndDestroy( contactEngine );
    CleanupStack::PopAndDestroy( array );
    
    }
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ResolveAddressesLOpSpecificL()
    {
    CDesCArrayFlat* array = new ( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( array ); 
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( contactEngine ); 
    
    // dummy initialization, other ways CPbkContactEngine leaks memory
    CPbkContactItem* pbkItem = NULL;
    TRAP_IGNORE( pbkItem = contactEngine->ReadContactL( 1 ) );
    delete pbkItem;
    
    CPbkContactItem* contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 
    EUNIT_ENABLE_ALLOC_DECORATOR;
    
    //Test cases
    //Sip Uri without test string
    CContactTextField* urlAtSignField = 
            EditableContactFieldL( *contactEngine, *contactItem, EPbkFieldIdSIPID );
    if ( urlAtSignField )
        {
        urlAtSignField->SetTextL( _L("sip:test@10.21.32.51") );
        }
    else
        {
        EUNIT_ASSERT( !urlAtSignField );
        }
    contactEngine->AddNewContactL( *contactItem );
    TBool ret;
    TRAPD( error, ret = iContactAvailability->ResolveAddressesL( *contactItem, 
                                             array,
                                             EPbkFieldIdSIPID ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( !ret );
    EUNIT_ASSERT_EQUALS( array->Count(), 0 );
    
    //Clear array
    array->Reset();
    
    //Sip Uri with test string (*#)
    urlAtSignField = 
            EditableContactFieldL( *contactEngine, *contactItem, EPbkFieldIdSIPID );
    if ( urlAtSignField )
        {
        urlAtSignField->SetTextL( _L("sip:test*#@10.21.32.51") );
        }
    else
        {
        EUNIT_ASSERT( !urlAtSignField );
        }
    contactEngine->AddNewContactL( *contactItem );
    TRAP( error, ret = iContactAvailability->ResolveAddressesL( *contactItem, 
                                             array,
                                             EPbkFieldIdSIPID ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( ret );
    EUNIT_ASSERT( array->Count() > 0 );
    
    //Clear array
    array->Reset();    

    //End test
    CleanupStack::PopAndDestroy( contactItem );
    CleanupStack::PopAndDestroy( contactEngine );
    CleanupStack::PopAndDestroy( array );
    
    }
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ResolvePhoneContactLL()  
    {
    CDesCArrayFlat* array = new ( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( array );
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( contactEngine );

    CPbkContactItem* contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 
     ///Number plan is in national mode
    TPbkContactItemField* fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL(_L("0401234567"));
    EUNIT_ENABLE_ALLOC_DECORATOR;

    iContactAvailability->ResolvePhoneContactL( *contactItem, array );
    EUNIT_ASSERT( array->MdcaCount() == 0 );
    //Number plan is in internatiolan mode 
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactItem->RemoveDefaultPhoneNumberField();
    fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL(_L("+358401234567"));
    EUNIT_ENABLE_ALLOC_DECORATOR;
    iContactAvailability->ResolvePhoneContactL( *contactItem, array );
    EUNIT_ASSERT( array->MdcaCount() > 0 );
    array->Reset();
   
    //tel uri
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactItem->RemoveDefaultPhoneNumberField();
    fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL(_L("tel:+358401234567"));
    EUNIT_ENABLE_ALLOC_DECORATOR;
    iContactAvailability->ResolvePhoneContactL( *contactItem, array );
    EUNIT_ASSERT( array->Count() == 0 );
    
    //End test
    CleanupStack::PopAndDestroy( contactItem );
    CleanupStack::PopAndDestroy( contactEngine );
    CleanupStack::PopAndDestroy( array );
    }
    
void UT_CMusAvaContactAvailability::UT_CMusAvaController_ResolvePhoneContactLOpSpecificL()  
    {
    CDesCArrayFlat* array = new ( ELeave ) CDesCArrayFlat( 10 );
    CleanupStack::PushL( array );
    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( contactEngine );

    CPbkContactItem* contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 
     ///Number plan is in national mode
    TPbkContactItemField* fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL(_L("0401234567"));
    EUNIT_ENABLE_ALLOC_DECORATOR;

    iContactAvailability->ResolvePhoneContactL( *contactItem, array );
    EUNIT_ASSERT( array->MdcaCount() > 0  );
    //Number plan is in internatiolan mode 
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactItem->RemoveDefaultPhoneNumberField();
    fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL(_L("+358401234567"));
    EUNIT_ENABLE_ALLOC_DECORATOR;
    iContactAvailability->ResolvePhoneContactL( *contactItem, array );
    EUNIT_ASSERT( array->MdcaCount() > 0 );
    array->Reset();
   
    //tel uri
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactItem->RemoveDefaultPhoneNumberField();
    fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL(_L("tel:+358401234567"));
    EUNIT_ENABLE_ALLOC_DECORATOR;
    iContactAvailability->ResolvePhoneContactL( *contactItem, array );
    EUNIT_ASSERT( array->Count() > 0 );
    
    //End test
    CleanupStack::PopAndDestroy( contactItem );
    CleanupStack::PopAndDestroy( contactEngine );
    CleanupStack::PopAndDestroy( array );
    }    
    
CContactTextField* UT_CMusAvaContactAvailability::EditableContactFieldL(
        CPbkContactEngine& aContactEngine, 
        CPbkContactItem& aContact, 
        TPbkFieldId aFieldId )
    {
   /* CContactDatabase* contactDatabase = NULL;
    contactDatabase = CContactDatabase::OpenL(); 
    CleanupStack::PushL( contactDatabase );  
    CCntFilter* newFilter = CCntFilter::NewLC();
    newFilter->Reset();
    newFilter->SetContactFilterTypeALL( ETrue );
    contactDatabase->FilterDatabaseL( *newFilter );
    
    CContactIdArray* contactsId = newFilter->iIds;
    if ( contactsId->Count() )
        {
        aContactEngine.DeleteContactsL( *contactsId );
        }
    //contactDatabase->CommitContactL():

    CleanupStack::PopAndDestroy( 2 );
    CleanupStack::PopAndDestroy( contactDatabase );*/
    
    EUNIT_DISABLE_ALLOC_DECORATOR;
    const CPbkFieldsInfo& fieldsInfo = aContactEngine.FieldsInfo();
    // Try to get the SIP URI storage field for this contact
    TPbkContactItemField* itemField = aContact.FindField( aFieldId );

    if ( !itemField )
        {
        CPbkFieldInfo* fieldInfo = fieldsInfo.Find( aFieldId );
        // Add field to this contact
        if ( fieldInfo )
            {
            CleanupStack::PushL( fieldInfo );
            // Contact takes ownership of the field.
            aContact.AddFieldL( *fieldInfo );
            CleanupStack::Pop( fieldInfo );
            itemField = aContact.FindField( aFieldId );
            if ( itemField )
                {
                return itemField->TextStorage();
                }
            else
                {
                EUNIT_ASSERT( !itemField );
                }
            }
        else
            {
            EUNIT_ASSERT( !fieldInfo );
            }
        }
    else
        {
        return itemField->TextStorage();
        }
    EUNIT_ENABLE_ALLOC_DECORATOR;
    return NULL; // Should never reach here, just to avoid warnings.
    }

void UT_CMusAvaContactAvailability::AddFields( CPbkContactEngine& aContactEngine,
                                               CPbkContactItem& contact,
                                               const TDesC& aText, 
                                               TPbkFieldId aFieldId )
    {
    const CPbkFieldsInfo& fieldsInfo = aContactEngine.FieldsInfo();
    // Try to get the storage field for this contact
    TPbkContactItemField* itemField = contact.FindField( aFieldId );

    if ( itemField == NULL )
        {
        CPbkFieldInfo* fieldInfo = fieldsInfo.Find( aFieldId );
        // Add field to this contact
        if ( fieldInfo )
            {
            CleanupStack::PushL( fieldInfo );
            // Contact takes ownership of the field.
            contact.AddFieldL( *fieldInfo );
            CleanupStack::Pop( fieldInfo );
            itemField = contact.FindField( aFieldId );
            if ( itemField )
                {
                itemField->TextStorage()->SetTextL( aText );
                }
            else
                {
                EUNIT_ASSERT( !itemField );
                }
            }
        else
            {
            EUNIT_ASSERT( !fieldInfo );
            }
        }
    else
        {
        // Set some default fields
        itemField->TextStorage()->SetTextL(aText );
        }
    } 
    
void UT_CMusAvaContactAvailability::AddToMyContact( CPbkContactEngine& aContactEngine,
                                                    const TDesC& aText, 
                                                    TPbkFieldId aFieldId,
                                                    const TDesC& aText2, 
                                                    TPbkFieldId aFieldId2 )
    {
    // Create a contact with few default fields
    // All the default fields are empty and won't be displayed
    // until some information is stored in them
    /*CPbkContactItem* contact = aContactEngine.CreateEmptyContactL();
    CleanupStack::PushL( contact );

    // Set some default fields
    contact->FindField( aFieldId )->TextStorage()->SetTextL(aText );
     // Set some default fields
    contact->FindField( aFieldId2 )->TextStorage()->SetTextL(aText2 );
     
    // Store the contact to the phonebook
    aContactEngine.AddNewContactL( *contact );
    CleanupStack::PopAndDestroy( contact );*/
    
    /*CPbkContactItem* contact = aContactEngine.CreateEmptyContactL();
    CleanupStack::PushL( contact );
        
    const CPbkFieldsInfo& fieldsInfo = aContactEngine.FieldsInfo();
    // Try to get the storage field for this contact
    TPbkContactItemField* itemField = contact->FindField( aFieldId );

    if ( itemField == NULL )
        {
        CPbkFieldInfo* fieldInfo = fieldsInfo.Find( aFieldId );
        // Add field to this contact
        if ( fieldInfo )
            {
            CleanupStack::PushL( fieldInfo );
            // Contact takes ownership of the field.
            contact->AddFieldL( *fieldInfo );
            CleanupStack::Pop( fieldInfo );
            itemField = contact->FindField( aFieldId );
            if ( itemField )
                {
                itemField->TextStorage()->SetTextL( aText );
                }
            else
                {
                EUNIT_ASSERT( !itemField );
                }
            }
        else
            {
            EUNIT_ASSERT( !fieldInfo );
            }
        }
    else
        {
        // Set some default fields
        itemField->TextStorage()->SetTextL(aText );
        }
    // Store the contact to the phonebook
    aContactEngine.AddNewContactL( *contact );
    CleanupStack::PopAndDestroy( contact );*/
    
    CPbkContactItem* contact = aContactEngine.CreateEmptyContactL();
    CleanupStack::PushL( contact );
        
    AddFields( aContactEngine, *contact, aText, aFieldId );
    AddFields( aContactEngine, *contact, aText2, aFieldId2 );
    // Store the contact to the phonebook
    aContactEngine.AddNewContactL( *contact );
    CleanupStack::PopAndDestroy( contact );
    
    } 

void UT_CMusAvaContactAvailability::UT_CMusAvaController_IsExistAlreadyL()
    {
    _LIT( KTestUri1, "sip:1@test.net");
    _LIT( KTestUri2, "sip:2@test.net");
    _LIT( KTestUri3, "sip:3@test.net");
    _LIT( KTestUri5, "sip:5@test.net");
    
    TBufC16<16> str1(KTestUri1);
    HBufC16* ptr1;
    ptr1 = str1.AllocLC(); 
    
    TBufC16<16> str2(KTestUri2);
    HBufC16* ptr2;
    ptr2 = str2.AllocLC(); 
    
    TBufC16<16> str3(KTestUri3);
    HBufC16* ptr3;
    ptr3 = str3.AllocLC(); 
    
    CDesCArrayFlat* array = new( ELeave ) CDesCArrayFlat(1);
    CleanupStack::PushL(array);
    array->AppendL(*ptr1);
    array->AppendL(*ptr2);
    array->AppendL(*ptr3);
    
    TBool exist = EFalse;

    // Test 1 :
    exist = iContactAvailability->IsExistAlready(*array,KTestUri1);
    EUNIT_ASSERT(exist);
    // Test 2 :
    exist = iContactAvailability->IsExistAlready(*array,KTestUri5);
    EUNIT_ASSERT(!exist);  

    array->Reset();
    CleanupStack::PopAndDestroy(array);
    CleanupStack::PopAndDestroy(ptr3);
    CleanupStack::PopAndDestroy(ptr2);
    CleanupStack::PopAndDestroy(ptr1);
    }

void UT_CMusAvaContactAvailability::UT_CMusAvaController_UpdateContactNameLL()
    {
    CPbkContactItem*  contactItem = NULL;

    EUNIT_DISABLE_ALLOC_DECORATOR;
    CPbkContactEngine* contactEngine = CPbkContactEngine::NewL();
    CleanupStack::PushL( contactEngine );

    // dummy initialization, other ways CPbkContactEngine leaks memory
    CPbkContactItem* pbkItem = NULL;
    TRAP_IGNORE( pbkItem = contactEngine->ReadContactL( 1 ) );
    delete pbkItem;
    
    contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 

    //contact firstname  && iBeginWithFirstName = ETrue
    TPbkContactItemField* fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
    
    TPbkContactItemField* fieldMobile1 = contactItem->FindField(EPbkFieldIdFirstName);
    fieldMobile1->TextStorage()->SetTextL( KFirstName() );
    
    contactEngine->AddNewContactL( *contactItem );
    EUNIT_ENABLE_ALLOC_DECORATOR;

    iConcreteSettings->SetTelNumberL( KPlusNumber() );
        
    iContactAvailability->iBeginWithFirstName = ETrue;
    TRAPD( error, 
        iContactAvailability->UpdateContactNameL( contactItem->ContactItem() ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( iContactAvailability->iContactName->Compare( KFirstName ) == NULL );

    CleanupStack::PopAndDestroy( contactItem );
    CContactIdArray* array = NULL;
    iContactAvailability->ContactArrayL( array, *contactEngine );
    CleanupStack::PushL( array );
    contactEngine->DeleteContactsL( *array, ETrue );
    CleanupStack::PopAndDestroy( array );
    
    //contact firstname  && iBeginWithFirstName = EFalse
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 

    fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
    
    fieldMobile1 = contactItem->FindField(EPbkFieldIdFirstName);
    fieldMobile1->TextStorage()->SetTextL( KFirstName() );
    
    contactEngine->AddNewContactL( *contactItem );
    EUNIT_ENABLE_ALLOC_DECORATOR;

    iConcreteSettings->SetTelNumberL( KPlusNumber() );
        
    iContactAvailability->iBeginWithFirstName = EFalse;
    TRAP( error, 
        iContactAvailability->UpdateContactNameL( contactItem->ContactItem() ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( iContactAvailability->iContactName->Compare( KFirstName ) == NULL );

    CleanupStack::PopAndDestroy( contactItem );
    array = NULL;
    iContactAvailability->ContactArrayL( array, *contactEngine );
    CleanupStack::PushL( array );
    contactEngine->DeleteContactsL( *array, ETrue );
    CleanupStack::PopAndDestroy( array );
    
    //contact lastname && iBeginWithFirstName = ETrue
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 

    fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
    
    fieldMobile1 = contactItem->FindField(EPbkFieldIdLastName);
    fieldMobile1->TextStorage()->SetTextL( KLastName() );
    
    contactEngine->AddNewContactL( *contactItem );
    EUNIT_ENABLE_ALLOC_DECORATOR;

    iConcreteSettings->SetTelNumberL( KPlusNumber() );
        
    iContactAvailability->iBeginWithFirstName = ETrue;
    TRAP( error, 
        iContactAvailability->UpdateContactNameL( contactItem->ContactItem() ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( iContactAvailability->iContactName->Compare( KLastName ) == NULL );

    CleanupStack::PopAndDestroy( contactItem );
    array = NULL;
    iContactAvailability->ContactArrayL( array, *contactEngine );
    CleanupStack::PushL( array );
    contactEngine->DeleteContactsL( *array, ETrue );
    CleanupStack::PopAndDestroy( array );
    
    //contact lastname && iBeginWithFirstName = EFalse
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 
 
    fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
    
    fieldMobile1 = contactItem->FindField(EPbkFieldIdLastName);
    fieldMobile1->TextStorage()->SetTextL( KLastName() );
    
    contactEngine->AddNewContactL( *contactItem );
    EUNIT_ENABLE_ALLOC_DECORATOR;

    iConcreteSettings->SetTelNumberL( KPlusNumber() );
        
    iContactAvailability->iBeginWithFirstName = EFalse;
    TRAP( error, 
         iContactAvailability->UpdateContactNameL( contactItem->ContactItem() ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( iContactAvailability->iContactName->Compare( KLastName ) == NULL );

    CleanupStack::PopAndDestroy( contactItem );
    array = NULL;
    iContactAvailability->ContactArrayL( array, *contactEngine );
    CleanupStack::PushL( array );
    contactEngine->DeleteContactsL( *array, ETrue );
    CleanupStack::PopAndDestroy( array );
    
    //contact firstname && lastname && iBeginWithFirstName = ETrue
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 

    fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
    
    fieldMobile1 = contactItem->FindField(EPbkFieldIdFirstName);
    fieldMobile1->TextStorage()->SetTextL( KFirstName() );
    
    TPbkContactItemField* fieldMobile2 = contactItem->FindField(EPbkFieldIdLastName);
    fieldMobile2->TextStorage()->SetTextL( KLastName() );
    
    contactEngine->AddNewContactL( *contactItem );
    EUNIT_ENABLE_ALLOC_DECORATOR;

    iConcreteSettings->SetTelNumberL( KPlusNumber() );
        
    iContactAvailability->iBeginWithFirstName = ETrue;
    TRAP( error, 
         iContactAvailability->UpdateContactNameL( contactItem->ContactItem() ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( iContactAvailability->iContactName->Compare( KFirstLastName ) == NULL );
//
    CleanupStack::PopAndDestroy( contactItem );
    array = NULL;
    iContactAvailability->ContactArrayL( array, *contactEngine );
    CleanupStack::PushL( array );
    contactEngine->DeleteContactsL( *array, ETrue );
    CleanupStack::PopAndDestroy( array );
    
    //contact firstname && lastname && iBeginWithFirstName = EFalse
    EUNIT_DISABLE_ALLOC_DECORATOR;
    contactItem= contactEngine->CreateEmptyContactL();
    CleanupStack::PushL( contactItem ); 

    fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
    fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
    
    fieldMobile1 = contactItem->FindField(EPbkFieldIdFirstName);
    fieldMobile1->TextStorage()->SetTextL( KFirstName() );
    
    fieldMobile2 = contactItem->FindField(EPbkFieldIdLastName);
    fieldMobile2->TextStorage()->SetTextL( KLastName() );
    
    contactEngine->AddNewContactL( *contactItem );
    EUNIT_ENABLE_ALLOC_DECORATOR;

    iConcreteSettings->SetTelNumberL( KPlusNumber() );
        
    iContactAvailability->iBeginWithFirstName = EFalse;
    TRAP( error, 
         iContactAvailability->UpdateContactNameL( contactItem->ContactItem() ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( iContactAvailability->iContactName->Compare( KLastFirstName ) == NULL );

    CleanupStack::PopAndDestroy( contactItem );
    array = NULL;
    iContactAvailability->ContactArrayL( array, *contactEngine );
    CleanupStack::PushL( array );
    contactEngine->DeleteContactsL( *array, ETrue );
    CleanupStack::PopAndDestroy( array );
    
    // contact name only contain 1 character
    _LIT(KNameWithOneChar,"M");
    for (int i = 0; i < 1; ++i )
        {
        EUNIT_DISABLE_ALLOC_DECORATOR;
        contactItem= contactEngine->CreateEmptyContactL();
        CleanupStack::PushL( contactItem ); 

        fieldMobile = contactItem->FindField(EPbkFieldIdPhoneNumberMobile);
        fieldMobile->TextStorage()->SetTextL( KPlusNumber() );
            
        fieldMobile1 = contactItem->FindField(EPbkFieldIdFirstName);
        fieldMobile1->TextStorage()->SetTextL( KNameWithOneChar() );
            
        contactEngine->AddNewContactL( *contactItem );
        EUNIT_ENABLE_ALLOC_DECORATOR;

        iConcreteSettings->SetTelNumberL( KPlusNumber() );
                
        iContactAvailability->iBeginWithFirstName = i;
        TRAP( error, 
               iContactAvailability->UpdateContactNameL( contactItem->ContactItem() ) );
        if ( error == KErrNoMemory ) User::Leave( error );
        EUNIT_ASSERT( error == KErrNone );
        EUNIT_ASSERT( iContactAvailability->iContactName->Compare( KNameWithOneChar ) == -1 );

        CleanupStack::PopAndDestroy( contactItem );
        array = NULL;
        iContactAvailability->ContactArrayL( array, *contactEngine );
        CleanupStack::PushL( array );
        contactEngine->DeleteContactsL( *array, ETrue );
        CleanupStack::PopAndDestroy( array );
        }
    
    CleanupStack::PopAndDestroy( contactEngine );
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

EUNIT_TEST(
    "UpdateContactNameL - test ",
    "CMusAvaContactAvailability",
    "UpdateContactNameL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaController_CallConnectedLL, Teardown)    
EUNIT_END_TEST_TABLE

//  END OF FILE

