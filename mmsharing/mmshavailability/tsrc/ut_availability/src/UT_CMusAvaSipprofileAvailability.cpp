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
#include "UT_CMusAvaSipprofileAvailability.h"
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

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
//#include <telephonyinternalpskeys.h>
#include <etelmm.h>
#include <rconnmon.h>



//  INTERNAL INCLUDES
#include "musavasipprofileavailability.h"



// CONSTRUCTION
UT_CMusAvaSipprofileAvailability* UT_CMusAvaSipprofileAvailability::NewL()
    {
    UT_CMusAvaSipprofileAvailability* self = UT_CMusAvaSipprofileAvailability::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaSipprofileAvailability* UT_CMusAvaSipprofileAvailability::NewLC()
    {
    UT_CMusAvaSipprofileAvailability* self = new( ELeave ) UT_CMusAvaSipprofileAvailability();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaSipprofileAvailability::~UT_CMusAvaSipprofileAvailability()
    {
    }

// Default constructor
UT_CMusAvaSipprofileAvailability::UT_CMusAvaSipprofileAvailability()
    {
    }

// Second phase construct
void UT_CMusAvaSipprofileAvailability::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaSipprofileAvailability::SetupL(  )
    {   
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();    
    iSipprofileAvailability = CMusAvaSipprofileAvailability::NewL( *iAvailabilityObserver );                                        
    } 

void UT_CMusAvaSipprofileAvailability::Teardown(  )
    {
    delete iAvailabilityObserver;
    iAvailabilityObserver = NULL;
    delete iSipprofileAvailability;
    iSipprofileAvailability = NULL;   
    } 

void UT_CMusAvaSipprofileAvailability::UT_CMusAvaSipprofileAvailability_NewLL()
    {
    EUNIT_ASSERT( iSipprofileAvailability );
    }  

void UT_CMusAvaSipprofileAvailability::UT_CMusAvaSipprofileAvailability_IsProfileExistL()
    {
    TBool val = iSipprofileAvailability->IsProfileExist();    
    EUNIT_ASSERT( val ==  ETrue );
    }
	
void UT_CMusAvaSipprofileAvailability::UT_CMusAvaSipprofileAvailability_StopL()
    {
    iSipprofileAvailability->Stop();
    EUNIT_ASSERT( iSipprofileAvailability->State() == MMusAvaObserver::EMusAvaStatusNotExecuted );

    TRAPD( error, iSipprofileAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    iSipprofileAvailability->Stop();
    EUNIT_ASSERT( iSipprofileAvailability->State() == MMusAvaObserver::EMusAvaStatusNotExecuted ); 
    }
  
void UT_CMusAvaSipprofileAvailability::UT_CMusAvaSipprofileAvailability_NameL()
    {    
    EUNIT_ASSERT( iSipprofileAvailability->Name() == MMusAvaObserver::EMusAvaNameSipprofileExist );     
    }

void UT_CMusAvaSipprofileAvailability::UT_CMusAvaSipprofileAvailability_DoExecuteLL()
    {    
    TRAPD( error, iSipprofileAvailability->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    EUNIT_ASSERT( iSipprofileAvailability->State() ==  MMusAvaObserver::EMusAvaStatusAvailable );     
    }
                        
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaSipprofileAvailability,
    "CMusAvaSipprofileAvailability",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaSipprofileAvailability",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipprofileAvailability_NewLL, Teardown)

EUNIT_TEST(
    "IsProfileExist - test ",
    "CMusAvaSipprofileAvailability",
    "IsProfileExist",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipprofileAvailability_IsProfileExistL, Teardown)    

EUNIT_TEST(
    "Stop - test ",
    "CMusAvaSipprofileAvailability",
    "Stop",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipprofileAvailability_StopL, Teardown)
    
EUNIT_TEST(
    "Name - test ",
    "CMusAvaSipprofileAvailability",
    "Name",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipprofileAvailability_NameL, Teardown)

EUNIT_TEST(
    "DoExecuteL - test ",
    "CMusAvaSipprofileAvailability",
    "DoExecuteL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaSipprofileAvailability_DoExecuteLL, Teardown)
 

EUNIT_END_TEST_TABLE

//  END OF FILE
