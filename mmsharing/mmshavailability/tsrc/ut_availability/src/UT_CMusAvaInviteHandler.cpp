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
#include "UT_CMusAvaInviteHandler.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "musavasettingsimp.h"
#include "musavaInviteresponderobserverimp.h"
#include "musavainviteresponder.h"
#include "muslogger.h"
#include "musavaobserverimp.h"
#include "musavacapabilitytesthelper.h" //remove compilation warning


//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <stringpool.h>
#include <sipservertransaction.h>
#include <siptransactionbase.h>
#include <digia/eunit/eunitdecorators.h>
//  INTERNAL INCLUDES
#include "musavainvitehandler.h"



// CONSTRUCTION
UT_CMusAvaInviteHandler* UT_CMusAvaInviteHandler::NewL()
    {
    UT_CMusAvaInviteHandler* self = UT_CMusAvaInviteHandler::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaInviteHandler* UT_CMusAvaInviteHandler::NewLC()
    {
    UT_CMusAvaInviteHandler* self = new( ELeave ) UT_CMusAvaInviteHandler();
    CleanupStack::PushL( self );
	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaInviteHandler::~UT_CMusAvaInviteHandler()
    {
    }

// Default constructor
UT_CMusAvaInviteHandler::UT_CMusAvaInviteHandler()
    {
    }

// Second phase construct
void UT_CMusAvaInviteHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMusAvaInviteHandler::SetupL(  )
    {
    SIPStrings::OpenL();
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iInviteHandler = CMusAvaInviteHandler::NewL( *iAvailabilityObserver,
                                                 *iConcreteSettings );
    } 

void UT_CMusAvaInviteHandler::Teardown(  )
    {
    delete iAvailabilityObserver;
    delete iConcreteSettings;
    delete iInviteHandler;
    SIPStrings::Close();
    }

void UT_CMusAvaInviteHandler::UT_CMusAvaInviteHandler_NewLL(  )
    {
    EUNIT_ASSERT( iInviteHandler );
    }
void UT_CMusAvaInviteHandler::UT_CMusAvaInviteHandler_IncomingRequestL()
    {
    CSIPServerTransaction* transaction = 
    CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    CleanupStack::PushL( transaction );
    transaction->iIsClientTransaction = ETrue;
    transaction->iState = CSIPTransactionBase::ETrying; 
    EUNIT_ASSERT( !iInviteHandler->IncomingRequest( transaction ) );
    CleanupStack::Pop( transaction );
    
    // Request not INVITE
    transaction = 
    CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( transaction );
    transaction->iIsClientTransaction = ETrue;
    transaction->iState = CSIPTransactionBase::ETrying; 
    EUNIT_ASSERT( iInviteHandler->IncomingRequest( transaction ) == KErrNotFound );
    CleanupStack::PopAndDestroy( transaction );
    }

void UT_CMusAvaInviteHandler::UT_CMusAvaInviteHandler_DoExecuteLL()
    {   
    TInt ret = 0;
    TRAPD( error, iInviteHandler->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT ( error == KErrNone );

    EUNIT_ASSERT( iInviteHandler->State() ==  MMusAvaObserver::EMusAvaStatusAvailable ); 
    }

void UT_CMusAvaInviteHandler::UT_CMusAvaInviteHandler_NameL()
    {
     EUNIT_ASSERT( iInviteHandler->Name() == MMusAvaObserver::EMusAvaInviteHandler );
    }
  

void UT_CMusAvaInviteHandler::UT_CMusAvaInviteHandler_StopL()
    {
    iInviteHandler->Stop();
    EUNIT_ASSERT( iInviteHandler->iResponders.Count() == 0 );
    }

void UT_CMusAvaInviteHandler::UT_CMusAvaInviteHandler_InvitationReceivedLL()
    {
    //Fails
    CSIPServerTransaction* transaction2 = 
    CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    CleanupStack::PushL( transaction2 );
    transaction2->iState = CSIPTransactionBase::ETerminated; 
    iInviteHandler->InvitationReceivedL( transaction2 );
    EUNIT_ASSERT( iInviteHandler->iResponders.Count() == 0 );
    CleanupStack::Pop( transaction2 );
    //Ok
    CSIPServerTransaction* transaction = 
    CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    CleanupStack::PushL( transaction );
    transaction->iIsClientTransaction = EFalse;
    transaction->iState = CSIPTransactionBase::ETrying; 
    TRAPD( error, iInviteHandler->InvitationReceivedL( transaction ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    CleanupStack::Pop( transaction );
    TInt count = NULL;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count > 0 && iInviteHandler->iResponders.Count() == 0 )
        {
        User::Leave( KErrNoMemory );
        }
    EUNIT_ASSERT( iInviteHandler->iResponders.Count() > 0 );
    }

void UT_CMusAvaInviteHandler::UT_CMusAvaInviteHandler_InvitationAnsweredLDL()
    {
     //not removed, because response it not completed
    CMusAvaInviteResponderObserverImp* responderObserverImp = CMusAvaInviteResponderObserverImp::NewL();
    CleanupStack::PushL( responderObserverImp );
    CSIPServerTransaction* serverTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    CleanupStack::PushL( serverTransaction );
    CMusAvaInviteResponder* inviteResponder = CMusAvaInviteResponder::NewL( serverTransaction,
        *responderObserverImp );
    CleanupStack::Pop( serverTransaction );
    CleanupStack::PushL( inviteResponder );
    User::LeaveIfError( iInviteHandler->iResponders.Append( inviteResponder ) );
    CleanupStack::Pop( inviteResponder );
    iInviteHandler->InvitationAnsweredLD( );
    EUNIT_ASSERT( iInviteHandler->iResponders.Count() > 0 );
    iInviteHandler->iResponders.ResetAndDestroy();
    CleanupStack::PopAndDestroy( responderObserverImp );
    //Ok
    CMusAvaInviteResponderObserverImp* responderObserverImp2 = CMusAvaInviteResponderObserverImp::NewL();
    CleanupStack::PushL( responderObserverImp2 );

    CSIPServerTransaction* serverTransaction2 = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    CleanupStack::PushL( serverTransaction2 );
    CMusAvaInviteResponder* inviteResponder2 = CMusAvaInviteResponder::NewL( serverTransaction2,
        *responderObserverImp2 );
    CleanupStack::Pop( serverTransaction2 );
    CleanupStack::PushL( inviteResponder2 );
    User::LeaveIfError( iInviteHandler->iResponders.Append( inviteResponder2 ) );
    CleanupStack::Pop( inviteResponder2 );
    serverTransaction2->iState = CSIPTransactionBase::ETerminated;
     iInviteHandler->InvitationAnsweredLD( );
    EUNIT_ASSERT( iInviteHandler->iResponders.Count() == 0 );
    CleanupStack::PopAndDestroy( responderObserverImp2 );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaInviteHandler,
    "CMusAvaInviteHandler",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaInviteHandler",
    "NewL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaInviteHandler_NewLL, Teardown)
    
EUNIT_TEST(
    "IncomingRequest - test ",
    "CMusAvaInviteHandler",
    "IncomingRequest",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaInviteHandler_IncomingRequestL, Teardown)

EUNIT_TEST(
    "DoExecuteL - test ",
    "CMusAvaInviteHandler",
    "DoExecuteL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaInviteHandler_DoExecuteLL, Teardown)        
    
EUNIT_TEST(
    "Name - test ",
    "CMusAvaInviteHandler",
    "Name",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaInviteHandler_NameL, Teardown)
    

EUNIT_TEST(
    "Stop - test ",
    "CMusAvaInviteHandler",
    "Stop",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaInviteHandler_StopL, Teardown)   
     
 EUNIT_TEST(
    "InvitationReceivedL - test ",
    "CMusAvaInviteHandler",
    "InvitationReceivedL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaInviteHandler_InvitationReceivedLL, Teardown)  
    
 EUNIT_TEST(
    "InvitationAnsweredLD - test ",
    "CMusAvaInviteHandler",
    "InvitationAnsweredLD",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaInviteHandler_InvitationAnsweredLDL, Teardown)           
         

EUNIT_END_TEST_TABLE

//  END OF FILE
