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
#include "UT_CMusAvaInviteResponder.h"
#include "sipservertransaction.h"
#include "siptransactionbase.h"
#include "musavaInviteresponderobserverimp.h"
#include "siptransactionbase.h"
#include "muslogger.h"
#include "CSipSseTestTls.h"
#include "musavacapabilitytesthelper.h" //remove compilation warning

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <stringpool.h>
#include <sipresponseelements.h>
//  INTERNAL INCLUDES
#include "musavainviteresponder.h"


const TUint KMUSAVA603 = 603; // Decline/Reject
_LIT8( KMusAvaStatusDecline, "Decline" );

// CONSTRUCTION
UT_CMusAvaInviteResponder* UT_CMusAvaInviteResponder::NewL()
    {
    UT_CMusAvaInviteResponder* self = UT_CMusAvaInviteResponder::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaInviteResponder* UT_CMusAvaInviteResponder::NewLC()
    {
    UT_CMusAvaInviteResponder* self = new( ELeave ) UT_CMusAvaInviteResponder();
    CleanupStack::PushL( self );

	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaInviteResponder::~UT_CMusAvaInviteResponder()
    {
    }

// Default constructor
UT_CMusAvaInviteResponder::UT_CMusAvaInviteResponder()
    {
    }

// Second phase construct
void UT_CMusAvaInviteResponder::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaInviteResponder::SetupL(  )
    {
    iResponderObserverImp = CMusAvaInviteResponderObserverImp::NewL();
    SIPStrings::OpenL();
     
    CSIPServerTransaction* serverTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    CleanupStack::PushL( serverTransaction );
    iInviteResponder = CMusAvaInviteResponder::NewL( serverTransaction,
        *iResponderObserverImp );
    CleanupStack::Pop( serverTransaction );
    } 

void UT_CMusAvaInviteResponder::Teardown(  )
    { 
    delete iResponderObserverImp;
    delete iInviteResponder;
    SIPStrings::Close();
    }


void UT_CMusAvaInviteResponder::UT_CMusAvaInviteResponder_NewLL()
    {
    EUNIT_ASSERT( iInviteResponder );
    }  

void UT_CMusAvaInviteResponder::UT_CMusAvaInviteResponder_AnswerLL()
    {
    //Busy
    iResponderObserverImp->iApplicationState = MMusAvaSettingsObserver::EApplicationIsRunning;
    iInviteResponder->iTransaction->iIsClientTransaction = EFalse;
    iInviteResponder->AnswerL();
    EUNIT_ASSERT( iInviteResponder->iTransaction->iResponseElements->StatusCode() == 486 );

    ////Decline
    iResponderObserverImp->iApplicationState = MMusAvaSettingsObserver::EApplicationStateNotDefined;
    iInviteResponder->iTransaction->iIsClientTransaction = EFalse;
    iInviteResponder->AnswerL();
    EUNIT_ASSERT( iInviteResponder->iTransaction->iResponseElements->StatusCode() == 603 );
    }  

void UT_CMusAvaInviteResponder::UT_CMusAvaInviteResponder_SendResponseLL()
    {
    iInviteResponder->iTransaction->iIsClientTransaction = EFalse;
    iInviteResponder->SendResponseL( KMUSAVA603, KMusAvaStatusDecline);
    EUNIT_ASSERT( iInviteResponder->iTransaction->iResponseElements->StatusCode() == 603 );
    }  


void UT_CMusAvaInviteResponder::UT_CMusAvaInviteResponder_ResponseCompleteL()
    {
    //Response is not client transaction
    iInviteResponder->iTransaction->iState = CSIPTransactionBase::ETrying; 
    EUNIT_ASSERT( !iInviteResponder->ResponseComplete() );
    
    //Response is trying state
    delete iInviteResponder->iTransaction;
    iInviteResponder->iTransaction = NULL;
    iInviteResponder->iTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EAck ) );
    iInviteResponder->iTransaction->iState = CSIPTransactionBase::ETerminated;
    EUNIT_ASSERT( !iInviteResponder->ResponseComplete() );
    
    //OK
    delete iInviteResponder->iTransaction;
    iInviteResponder->iTransaction = NULL;
    iInviteResponder->iTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    iInviteResponder->iTransaction->iState = CSIPTransactionBase::ETerminated;
    EUNIT_ASSERT( iInviteResponder->ResponseComplete() );
    }  

void UT_CMusAvaInviteResponder::UT_CMusAvaInviteResponder_ResponseLL()
    {
    CSIPResponseElements* responseElements = NULL;
    RStringF responseString;
    //It is not a client transaction
    iInviteResponder->iTransaction->iIsClientTransaction = ETrue;
    responseString = SIPStrings::Pool().OpenFStringL( KMusAvaStatusDecline() );
    CleanupClosePushL( responseString );
    responseElements = CSIPResponseElements::NewLC( KMUSAVA603, responseString );
    TRAPD( error, iInviteResponder->ResponseL( responseElements ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error != KErrNone );

    CleanupStack::PopAndDestroy( responseElements );
    CleanupStack::PopAndDestroy(); //responseString

    //Response not sent, becase is in ECalling state
    iInviteResponder->iTransaction->iIsClientTransaction = ETrue;
    iInviteResponder->iTransaction->iState = CSIPTransactionBase::ECalling;
    responseString = SIPStrings::Pool().OpenFStringL( KMusAvaStatusDecline() );
    CleanupClosePushL( responseString );
    responseElements = CSIPResponseElements::NewLC( KMUSAVA603, responseString );
    TRAP( error, iInviteResponder->ResponseL( responseElements ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error != KErrNone );

    CleanupStack::PopAndDestroy( responseElements );
    CleanupStack::PopAndDestroy(); //responseString
    
    //Response not sent, becase is not allowed
    delete iInviteResponder->iTransaction;
    iInviteResponder->iTransaction = NULL;
    iInviteResponder->iTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EAck ) );
    iInviteResponder->iTransaction->iIsClientTransaction = ETrue;
    iInviteResponder->iTransaction->iState = CSIPTransactionBase::ETrying; 
    responseString = SIPStrings::Pool().OpenFStringL( KMusAvaStatusDecline() );
    CleanupClosePushL( responseString );
    responseElements = CSIPResponseElements::NewLC( KMUSAVA603, responseString );
    TRAP( error, iInviteResponder->ResponseL( responseElements ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error != KErrNone );
    CleanupStack::PopAndDestroy( responseElements );
    CleanupStack::PopAndDestroy(); //responseString
    
    //Ok case
    delete iInviteResponder;
    iInviteResponder = NULL;
    CSIPServerTransaction* serverTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    CleanupStack::PushL( serverTransaction );
    iInviteResponder = CMusAvaInviteResponder::NewL( serverTransaction,
        *iResponderObserverImp );
    CleanupStack::Pop( serverTransaction );
    iInviteResponder->iTransaction->iIsClientTransaction = EFalse;
    iInviteResponder->iTransaction->iState = CSIPTransactionBase::ETrying; 
    responseString = SIPStrings::Pool().OpenFStringL( KMusAvaStatusDecline() );
    CleanupClosePushL( responseString );
    responseElements = CSIPResponseElements::NewLC( KMUSAVA603, responseString );
    iInviteResponder->ResponseL( responseElements );
    CleanupStack::Pop( responseElements );
    CleanupStack::PopAndDestroy(); //responseString
    EUNIT_ASSERT( iInviteResponder->iTransaction->iResponseElements->StatusCode() == 603 );
    }  
 


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaInviteResponder,
    "CMusAvaInviteResponder",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaSip",
    "NewL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaInviteResponder_NewLL, Teardown)
    
EUNIT_TEST(
    "AnswerL - test ",
    "CMusAvaInviteResponder",
    "AnswerL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaInviteResponder_AnswerLL, Teardown)
    
EUNIT_TEST(
    "SendResponseL - test ",
    "CMusAvaInviteResponder",
    "SendResponseL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaInviteResponder_SendResponseLL, Teardown)

EUNIT_TEST(
    "ResponseComplete - test ",
    "CMusAvaInviteResponder",
    "ResponseComplete",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaInviteResponder_ResponseCompleteL, Teardown)
    
EUNIT_TEST(
    "ResponseL - test ",
    "CMusAvaInviteResponder",
    "ResponseL",
    "FUNCTIONALITY",
    SetupL, UT_CMusAvaInviteResponder_ResponseLL, Teardown)
    

EUNIT_END_TEST_TABLE

//  END OF FILE
