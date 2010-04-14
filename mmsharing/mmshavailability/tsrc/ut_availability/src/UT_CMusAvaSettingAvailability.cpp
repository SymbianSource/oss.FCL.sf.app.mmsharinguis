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
#include "UT_CMusAvaSettingAvailability.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "musavaavailability.h"
#include "muslogger.h"
#include "musavaobserver.h"
#include "musavaobserverimp.h"
#include "musavadefaultimp.h"
#include "musavaobserverimp.h"
#include "musavasharedobject.h"
#include "mussettingskeys.h"
#include "mussettings.h"
#include "mussesseioninformationapi.h"
#include "CSipSseTestTls.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
#include <sipstrings.h>
#include <digia/eunit/eunitdecorators.h>


//  INTERNAL INCLUDES
#include "musavasettingavailability.h"



// CONSTRUCTION
UT_CMusAvaSettingAvailability* UT_CMusAvaSettingAvailability::NewL()
    {
    UT_CMusAvaSettingAvailability* self = UT_CMusAvaSettingAvailability::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaSettingAvailability* UT_CMusAvaSettingAvailability::NewLC()
    {
    UT_CMusAvaSettingAvailability* self = new( ELeave ) UT_CMusAvaSettingAvailability();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaSettingAvailability::~UT_CMusAvaSettingAvailability()
    {
    }

// Default constructor
UT_CMusAvaSettingAvailability::UT_CMusAvaSettingAvailability()
    {
    }

// Second phase construct
void UT_CMusAvaSettingAvailability::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaSettingAvailability::SetupL(  )
    {   
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();    
    iSettingAvailability = CMusAvaSettingAvailability::NewL( *iAvailabilityObserver );                                        
    TRAP_IGNORE( SIPStrings::OpenL() )
    CSipSseTestTls::OpenL();
    } 

void UT_CMusAvaSettingAvailability::Teardown(  )
    {
    CSipSseTestTls::Close();
    SIPStrings::Close();
    delete iAvailabilityObserver;
    iAvailabilityObserver = NULL;
    delete iSettingAvailability;
    iSettingAvailability = NULL;   
    PropertyHelper::Close();
    } 

void UT_CMusAvaSettingAvailability::UT_CMusAvaSettingAvailability_NewLL()
    {
    EUNIT_ASSERT( iSettingAvailability );
    }  

void UT_CMusAvaSettingAvailability::UT_CMusAvaSettingAvailability_CheckActivationStateL()
    {
    MultimediaSharingSettings::SetActivationSettingL(MusSettingsKeys::EAlwaysActive);
    RProperty::Define( NMusSessionInformationApi::KCategoryUid ,
	                NMusSessionInformationApi::KMUSForbidden,
                    RProperty::EInt,0);
    
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
	                	NMusSessionInformationApi::KMUSForbidden ,
	                	NMusSessionInformationApi::EMUSAllowed ));
	                	
    iSettingAvailability->CheckActivationState();
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iSettingAvailability->iState 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iSettingAvailability->iState == MMusAvaObserver::EMusAvaStatusAvailable );

    MultimediaSharingSettings::SetActivationSettingL(MusSettingsKeys::EActiveInHomeNetworks);
    iSettingAvailability->CheckActivationState();
    EUNIT_ASSERT( iSettingAvailability->iState == MMusAvaObserver::EMusActivationError );
    
    MultimediaSharingSettings::SetActivationSettingL(MusSettingsKeys::ENever);    
    iSettingAvailability->CheckActivationState();
    EUNIT_ASSERT( iSettingAvailability->iState == MMusAvaObserver::EMusActivationError );

    MultimediaSharingSettings::SetActivationSettingL(MusSettingsKeys::EAlwaysActive);    
    
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
	                	NMusSessionInformationApi::KMUSForbidden ,
	                	NMusSessionInformationApi::EMUSForbidden ));
	                	
    iSettingAvailability->CheckActivationState();
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iSettingAvailability->iState 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iSettingAvailability->iState == MMusAvaObserver::EMusAvaFailureCode );         
    }
	
void UT_CMusAvaSettingAvailability::UT_CMusAvaSettingAvailability_IsForbiddenByOptionsResponseL()
    {    
    RProperty::Define( NMusSessionInformationApi::KCategoryUid ,
	            NMusSessionInformationApi::KMUSForbidden,
                RProperty::EInt,0 );
                
                
	User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
	                		NMusSessionInformationApi::KMUSForbidden ,
			                NMusSessionInformationApi::EMUSForbidden ));
    EUNIT_ASSERT(iSettingAvailability->IsForbiddenByOptionsResponse());    
    
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
	                		NMusSessionInformationApi::KMUSForbidden ,
	                		NMusSessionInformationApi::EMUSAllowed ));

    EUNIT_ASSERT(!iSettingAvailability->IsForbiddenByOptionsResponse());    
    }


void UT_CMusAvaSettingAvailability::UT_CMusAvaSettingAvailability_StopL()
    {
    iSettingAvailability->Stop();
    EUNIT_ASSERT( iSettingAvailability->State() == MMusAvaObserver::EMusAvaStatusNotExecuted );

    TRAPD( error, iSettingAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    iSettingAvailability->Stop();
    EUNIT_ASSERT( iSettingAvailability->State() == MMusAvaObserver::EMusAvaStatusNotExecuted ); 
    }
  
void UT_CMusAvaSettingAvailability::UT_CMusAvaSettingAvailability_NameL()
    {    
    EUNIT_ASSERT( iSettingAvailability->Name() == MMusAvaObserver::EMusAvaNameSetting );     
    }

void UT_CMusAvaSettingAvailability::UT_CMusAvaSettingAvailability_DoExecuteLL()
    {    
    MultimediaSharingSettings::SetActivationSettingL(MusSettingsKeys::EAlwaysActive);
    TRAPD( error, iSettingAvailability->DoExecuteL() );
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iSettingAvailability->iState 
        == MMusAvaObserver::EMusActivationError )
        {
        User::Leave( KErrNoMemory );
        }
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );
    
    
    User::LeaveIfError( RProperty::Set( NMusSessionInformationApi::KCategoryUid ,
	                	NMusSessionInformationApi::KMUSForbidden ,
	                	NMusSessionInformationApi::EMUSAllowed ));
	                	
    EUNIT_ASSERT( iSettingAvailability->State() ==  MMusAvaObserver::EMusAvaStatusAvailable );     
    }
                        
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaSettingAvailability,
    "CMusAvaSettingAvailability",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaSettingAvailability",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingAvailability_NewLL, Teardown)

EUNIT_TEST(
    "CheckActivationState - test ",
    "CMusAvaSettingAvailability",
    "CheckActivationState",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingAvailability_CheckActivationStateL, Teardown)    

EUNIT_TEST(
    "IsForbiddenByOptionsResponse - test ",
    "CMusAvaSettingAvailability",
    "IsForbiddenByOptionsResponse",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingAvailability_IsForbiddenByOptionsResponseL, Teardown)    

EUNIT_TEST(
    "Stop - test ",
    "CMusAvaSettingAvailability",
    "Stop",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingAvailability_StopL, Teardown)
    
EUNIT_TEST(
    "Name - test ",
    "CMusAvaSettingAvailability",
    "Name",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingAvailability_NameL, Teardown)

EUNIT_TEST(
    "DoExecuteL - test ",
    "CMusAvaSettingAvailability",
    "DoExecuteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSettingAvailability_DoExecuteLL, Teardown)
 

EUNIT_END_TEST_TABLE

//  END OF FILE
