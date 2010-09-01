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
#include "UT_CMusAvaDefaultOptionHandler.h"
#include "musavaavailabilityobserver.h"
#include "musavaavailabilityobserverimp.h"
#include "musavasettingsimp.h"
#include "musavaobserverimp.h"
#include "musavadefaultoptionhandler.h"
#include "musavasettingsobserverimp.h"
#include "mussettingskeys.h"
#include "mussettings.h"
#include "muslogger.h"
#include "musavacapabilitytesthelper.h"
#include "CSipSseTestTls.h"
#include "musavacapabilitycontext.h"



//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <sipstrings.h>
#include <stringpool.h>
#include <sipservertransaction.h>
#include <siptransactionbase.h>
#include <sipheaderbase.h>
#include <SipStrConsts.h>
#include <sipresponseelements.h>
#include <sipmessageelements.h>
#include <siprequestelements.h>
#include <Uri8.h>
//  INTERNAL INCLUDES


_LIT8( KMUSAVATESTSipAddress, "sip:test@10.20.30.40");

// CONSTRUCTION
UT_CMusAvaDefaultOptionHandler* UT_CMusAvaDefaultOptionHandler::NewL()
    {
    UT_CMusAvaDefaultOptionHandler* self = UT_CMusAvaDefaultOptionHandler::NewLC(); 
    CleanupStack::Pop();

    return self;
    }

UT_CMusAvaDefaultOptionHandler* UT_CMusAvaDefaultOptionHandler::NewLC()
    {
    UT_CMusAvaDefaultOptionHandler* self = new( ELeave ) UT_CMusAvaDefaultOptionHandler();
    CleanupStack::PushL( self );
	self->ConstructL(); 

    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaDefaultOptionHandler::~UT_CMusAvaDefaultOptionHandler()
    {
    }

// Default constructor
UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler()
    {
    }

// Second phase construct
void UT_CMusAvaDefaultOptionHandler::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS

void UT_CMusAvaDefaultOptionHandler::SetupL(  )
    {
    TRAP_IGNORE( SIPStrings::OpenL() )
    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EStandard );
    MultimediaSharingSettings::SetEdgeDtmSupportSettingL( MusSettingsKeys::EDtmModeAllowed );
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iSettingsObserverImp = CMusAvaSettingsObserverImp::NewL();
    iConcreteSettings->SetObserver( *iSettingsObserverImp );
    iOptionDefaultHandler = CMusAvaDefaultOptionHandler::NewL( *iAvailabilityObserver,
                                                 *iConcreteSettings );
    MultimediaSharingSettings::SetCapabilityQuerySettingL(
                                                MusSettingsKeys::ENoOptions );
    } 
    
void UT_CMusAvaDefaultOptionHandler::SetupOpSpecificL(  )
    {
    TRAP_IGNORE( SIPStrings::OpenL() )
    CSipSseTestTls::OpenL();
    iStorage = CSipSseTestTls::Storage();
    MultimediaSharingSettings::SetOperatorVariantSettingL( MusSettingsKeys::EOperatorSpecific );
    MultimediaSharingSettings::SetEdgeDtmSupportSettingL( MusSettingsKeys::EDtmModeAllowed );
    iAvailabilityObserver = CMusAvaAvailabilityObserverImp::NewL();
    iConcreteSettings = CMusAvaSettingsImp::NewL();
    iSettingsObserverImp = CMusAvaSettingsObserverImp::NewL();
    iConcreteSettings->SetObserver( *iSettingsObserverImp );
    iOptionDefaultHandler = CMusAvaDefaultOptionHandler::NewL( *iAvailabilityObserver,
                                                 *iConcreteSettings );
    MultimediaSharingSettings::SetCapabilityQuerySettingL(
                                                MusSettingsKeys::ENoOptions );
    }     

void UT_CMusAvaDefaultOptionHandler::Teardown(  )
    {
    if ( iStorage )
	   {
       iStorage->Clear();
       CSipSseTestTls::Close();
       iStorage = NULL;
       }

    SIPStrings::Close();
    delete iAvailabilityObserver;
    delete iConcreteSettings;
    delete iSettingsObserverImp;
    delete iOptionDefaultHandler;
    }

void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_NewLL(  )
    {
    EUNIT_ASSERT( iOptionDefaultHandler );
    }
    
void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_IncomingRequestL()
    {   
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    CSIPServerTransaction* serverTransaction = NULL;

    serverTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );

    TUriParser8 Uri;
    User::LeaveIfError( Uri.Parse( KCapabilityTestAddress_A ) );
    
  	CUri8* uri = CUri8::NewLC( Uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    serverTransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );

    TInt ret = iOptionDefaultHandler->IncomingRequest( serverTransaction );
    if ( ret == KErrNoMemory ) User::Leave( ret );
    EUNIT_ASSERT( ret == KErrNone );
    
    CleanupStack::Pop( serverTransaction ); 

    serverTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    CleanupStack::PushL( serverTransaction );
    ret = iOptionDefaultHandler->IncomingRequest( serverTransaction );
    if ( ret == KErrNoMemory ) User::Leave( ret );
    EUNIT_ASSERT( ret == KErrNotFound );
    CleanupStack::PopAndDestroy( serverTransaction ); 

    iSettingsObserverImp->iOptionAllowed = EFalse;
    iAvailabilityObserver->AvailabilityChanged(
                            MMusAvaObserver::EMusAvaDefaultOptionHandler,
                            MMusAvaObserver::EMusAvaStatusAvailable );
    serverTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );
    
    uri = CUri8::NewLC( Uri );
  	
    requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    serverTransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );

    ret = iOptionDefaultHandler->IncomingRequest( serverTransaction );
    if ( ret == KErrNoMemory ) User::Leave( ret );
    EUNIT_ASSERT( ret == KErrNone );
    
    CleanupStack::Pop( serverTransaction );     

    //IAP Request
    iAvailabilityObserver->AvailabilityChanged(
                            MMusAvaObserver::EMusAvaDefaultOptionHandler,
                            MMusAvaObserver::EMusAvaStatusNotExecuted );    
    serverTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );
    
    uri = CUri8::NewLC( Uri );
  	
    requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    serverTransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );

    ret = iOptionDefaultHandler->IncomingRequest( 1, serverTransaction );
    if ( ret == KErrNoMemory ) User::Leave( ret );
    EUNIT_ASSERT( ret == KErrNone );
    
    CleanupStack::Pop( serverTransaction ); 
                
    serverTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EInvite ) );
    CleanupStack::PushL( serverTransaction );
    ret = iOptionDefaultHandler->IncomingRequest( 1, serverTransaction );
    if ( ret == KErrNoMemory ) User::Leave( ret );
    EUNIT_ASSERT( ret == KErrNotFound );
    CleanupStack::PopAndDestroy( serverTransaction );
    
    iSettingsObserverImp->iOptionAllowed = EFalse;
    iAvailabilityObserver->AvailabilityChanged(
                            MMusAvaObserver::EMusAvaOptionHandler,
                            MMusAvaObserver::EMusAvaStatusAvailable );    
    serverTransaction = CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );
    uri = CUri8::NewLC( Uri );
  	
    requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    serverTransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );
    ret = iOptionDefaultHandler->IncomingRequest( 1, serverTransaction );
    if ( ret == KErrNoMemory ) User::Leave( ret );
    EUNIT_ASSERT( ret == KErrNotFound );
    CleanupStack::PopAndDestroy( serverTransaction );     
    }
    
void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_DoExecuteLL()
    {   
    TRAPD( error, iOptionDefaultHandler->DoExecuteL() );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNone );
    EUNIT_ASSERT( iOptionDefaultHandler->State() ==  MMusAvaObserver::EMusAvaStatusAvailable ); 
    }

void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_NameL()
    {
     EUNIT_ASSERT( iOptionDefaultHandler->Name() == MMusAvaObserver::EMusAvaDefaultOptionHandler );
    }

void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_StatusL()
    {
    EUNIT_ASSERT( iOptionDefaultHandler->State() == MMusAvaObserver::EMusAvaStatusAvailable );
    }

void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_StopL()
    {
    iOptionDefaultHandler->Stop();
    }

void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_IncomingOptionRequestLL()
    {
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );

    TUriParser8 Uri;
    User::LeaveIfError( Uri.Parse( KCapabilityTestAddress_A ) );
    
  	CUri8* uri = CUri8::NewLC( Uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    serverTransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );

    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EAlwaysActive );
    TRAPD( ret, iOptionDefaultHandler->IncomingOptionRequestL( serverTransaction ) );
    if ( ret == KErrNoMemory ) User::Leave( ret );
    EUNIT_ASSERT( ret == KErrNone );
    
    TUint statusCode = iStorage->iClientResponse->StatusCode();
    EUNIT_ASSERT( statusCode == KMUSAVASIPTemporarilyUnavailable );
    
    iSettingsObserverImp->iOptionAllowed = ETrue;
    iAvailabilityObserver->AvailabilityChanged(
                            MMusAvaObserver::EMusAvaDefaultOptionHandler,
                            MMusAvaObserver::EMusAvaStatusAvailable );
    TRAP( ret, iOptionDefaultHandler->IncomingOptionRequestL( serverTransaction ) );
    if ( ret == KErrNoMemory ) User::Leave( ret );
    EUNIT_ASSERT( ret == KErrNone );
    
    iSettingsObserverImp->iOptionAllowed = EFalse;
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::EActiveInHomeNetworks );
    TRAP( ret, iOptionDefaultHandler->IncomingOptionRequestL( serverTransaction ) );
    if ( ret == KErrNoMemory ) User::Leave( ret );
    EUNIT_ASSERT( ret == KErrNone );
    
    MultimediaSharingSettings::SetActivationSettingL( MusSettingsKeys::ENever );
    TRAP( ret, iOptionDefaultHandler->IncomingOptionRequestL( serverTransaction ) );
    if ( ret == KErrNoMemory ) User::Leave( ret );
    EUNIT_ASSERT( ret == KErrNone );
    statusCode = iStorage->iClientResponse->StatusCode();
    EUNIT_ASSERT( statusCode == KMUSAVASIPDecline );
    
    CleanupStack::PopAndDestroy( serverTransaction );
    }
    
void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_IncomingOptionRequestLOpSpecificL()    
    {
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );    
    TUriParser8 Uri;
    User::LeaveIfError( Uri.Parse( KCapabilityTestAddress_A ) );
    CUri8* uri = CUri8::NewLC( Uri );
  	CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    serverTransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );

    TRAPD( ret, iOptionDefaultHandler->IncomingOptionRequestL( serverTransaction ) );
    if ( ret == KErrNoMemory ) User::Leave( ret );
    EUNIT_ASSERT( ret == KErrNone );
    TUint statusCode = iStorage->iClientResponse->StatusCode();
    EUNIT_ASSERT( statusCode == KMUSAVASIP200 );

    CleanupStack::PopAndDestroy( serverTransaction );    
    
    }

void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_ResponseLL()
    {
    TInt statusCode = KMUSAVASIPDecline;
    RStringF reasonPhrase =
        SIPStrings::StringF( SipStrConsts::EPhraseDecline);
    CSIPResponseElements* respElem =
        CSIPResponseElements::NewL( statusCode, reasonPhrase );
    CleanupStack::PushL( respElem );
    
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );    
    
    TUriParser8 Uri;
    User::LeaveIfError( Uri.Parse( KCapabilityTestAddress_A ) );
    
  	CUri8* uri = CUri8::NewLC( Uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    serverTransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );
    
    //This transaction is not client TR 
    serverTransaction->iIsClientTransaction = ETrue;
    serverTransaction->iState = CSIPTransactionBase::ETerminated; 
    TRAPD( error, iOptionDefaultHandler->ResponseL( respElem, serverTransaction ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNotReady );
    
    // Transaction state proceeding
    serverTransaction->ChangeState( CSIPTransactionBase::EProceeding );
    TRAP( error, iOptionDefaultHandler->ResponseL( respElem, serverTransaction ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    EUNIT_ASSERT( error == KErrNone );
        
    CleanupStack::PopAndDestroy( serverTransaction );     
    CleanupStack::Pop( respElem );   
   
     //Sent response
    serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );         
    respElem =
        CSIPResponseElements::NewL( statusCode, reasonPhrase );
    CleanupStack::PushL( respElem );
    
    uri = CUri8::NewLC( Uri );
    requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    serverTransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );

    serverTransaction->iIsClientTransaction = ETrue;
    serverTransaction->iState = CSIPTransactionBase::ETrying; 
    iOptionDefaultHandler->ResponseL( respElem, serverTransaction );
    CleanupStack::Pop( respElem );
    statusCode = iStorage->iClientResponse->StatusCode();
    EUNIT_ASSERT( statusCode == KMUSAVASIPDecline );
           
    CleanupStack::PopAndDestroy( serverTransaction );  
    serverTransaction = NULL;
    
    // response not allowed
    serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EAck ) );
    CleanupStack::PushL( serverTransaction );      
    respElem =
        CSIPResponseElements::NewL( statusCode, reasonPhrase );
    CleanupStack::PushL( respElem );
    
    TRAP( error, iOptionDefaultHandler->ResponseL( respElem, serverTransaction ) );
    if ( error == KErrNoMemory ) User::Leave( error );
    CleanupStack::PopAndDestroy( respElem );
    EUNIT_ASSERT( error == KErrNotReady );
        
    CleanupStack::PopAndDestroy( serverTransaction );                
    }

void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_NoCapabilityResponseLL()
    {
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );

    TUriParser8 Uri;
    User::LeaveIfError( Uri.Parse( KCapabilityTestAddress_A ) );
    
  	CUri8* uri = CUri8::NewLC( Uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    serverTransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );

    serverTransaction->iIsClientTransaction = ETrue;
    serverTransaction->iState = CSIPTransactionBase::ETrying; 
    iSettingsObserverImp->iOptionAllowed = ETrue;
    iOptionDefaultHandler->NoCapabilityResponseL( serverTransaction,
                                                  KMUSAVASIPTemporarilyUnavailable, 
                                                  SipStrConsts::EPhraseTemporarilyNotAvailable );
    TUint statusCode = iStorage->iClientResponse->StatusCode();
    EUNIT_ASSERT( statusCode == KMUSAVASIPTemporarilyUnavailable );
    
    CleanupStack::PopAndDestroy( serverTransaction );
    }
    
void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_NoCapabilityResponseLOpSpecificL() 
    {
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );  
    
    TUriParser8 Uri;
    User::LeaveIfError( Uri.Parse( KCapabilityTestAddress_A ) );
    
  	CUri8* uri = CUri8::NewLC( Uri );
  	
    CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    requestelements->SetMethodL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    serverTransaction->SetRequestElements( requestelements );
    CleanupStack::Pop( requestelements );    
    
    serverTransaction->iIsClientTransaction = ETrue;
    serverTransaction->iState = CSIPTransactionBase::ETrying; 
    iSettingsObserverImp->iOptionAllowed = ETrue;
    iOptionDefaultHandler->NoCapabilityResponseL( serverTransaction, 
                                                  KMUSAVASIPTemporarilyUnavailable, 
                                                  SipStrConsts::EPhraseTemporarilyNotAvailable );
    TUint statusCode = iStorage->iClientResponse->StatusCode();
    EUNIT_ASSERT( statusCode == KMUSAVASIP200 );
          
    CleanupStack::PopAndDestroy( serverTransaction );   
    }

void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_CreateResponseLL()
    {
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );
    CleanupStack::PushL( serverTransaction );
    TUriParser8 Uri;
    User::LeaveIfError( Uri.Parse( KMUSAVATESTSipAddress() ) );
    
  	CUri8* uri = CUri8::NewLC( Uri );
 	CSIPRequestElements* requestelements = CSIPRequestElements::NewL( uri ); 
    CleanupStack::Pop( uri );
    CleanupStack::PushL( requestelements );
    serverTransaction->SetRequestElements( requestelements );
   	CleanupStack::Pop( requestelements );
 
    //Sent response
    serverTransaction->iIsClientTransaction = ETrue;
    serverTransaction->iState = CSIPTransactionBase::ETrying; 
    iSettingsObserverImp->iOptionAllowed = ETrue;
    CSIPResponseElements* resp = 
        iOptionDefaultHandler->CreateResponseL( *serverTransaction );
    CleanupStack::PopAndDestroy( serverTransaction );
    CleanupStack::PushL( resp );
    EUNIT_ASSERT( resp->MessageElements().Content().Length() > 0);
    EUNIT_ASSERT( resp->StatusCode() == KMUSAVASIP200 );
    CleanupStack::PopAndDestroy( resp );
    }
    
    
void UT_CMusAvaDefaultOptionHandler::UT_CMusAvaDefaultOptionHandler_TimedOutL() 
    {
    CSIPServerTransaction* serverTransaction = 
        CSIPServerTransaction::NewL( SIPStrings::StringF( SipStrConsts::EOptions ) );    
    EUNIT_ASSERT_EQUALS( 
            iOptionDefaultHandler->TimedOut( *serverTransaction ), 
            KErrNotSupported );
    delete serverTransaction;
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaDefaultOptionHandler,
    "CMusAvaDefaultOptionHandler",
    "UNIT" )

EUNIT_TEST(
    "NewL - test ",
    "CMusAvaDefaultOptionHandler",
    "NewL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaDefaultOptionHandler_NewLL, Teardown)
    
EUNIT_TEST(
    "IncomingRequest - test ",
    "CMusAvaDefaultOptionHandler",
    "IncomingRequest",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaDefaultOptionHandler_IncomingRequestL, Teardown)    
    
EUNIT_TEST(
    "DoExecuteL - test ",
    "CMusAvaDefaultOptionHandler",
    "DoExecuteL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaDefaultOptionHandler_DoExecuteLL, Teardown)        

EUNIT_TEST(
    "Name - test ",
    "CMusAvaDefaultOptionHandler",
    "Name",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaDefaultOptionHandler_NameL, Teardown)
    

EUNIT_TEST(
    "Status - test ",
    "CMusAvaDefaultOptionHandler",
    "Status",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaDefaultOptionHandler_StatusL, Teardown)

EUNIT_TEST(
    "Stop - test ",
    "CMusAvaDefaultOptionHandler",
    "Stop",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaDefaultOptionHandler_StopL, Teardown)   

EUNIT_TEST(
    "IncomingOptionRequestL - test ",
    "CMusAvaDefaultOptionHandler",
    "IncomingOptionRequestL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaDefaultOptionHandler_IncomingOptionRequestLL, Teardown)   
    
EUNIT_TEST(
    "IncomingOptionRequestL - Op specific test ",
    "CMusAvaDefaultOptionHandler",
    "IncomingOptionRequestL",
    "FUNCTIONALITY",
    SetupOpSpecificL, UT_CMusAvaDefaultOptionHandler_IncomingOptionRequestLOpSpecificL, Teardown)       

EUNIT_TEST(
    "ResponseL - test ",
    "CMusAvaDefaultOptionHandler",
    "ResponseL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaDefaultOptionHandler_ResponseLL, Teardown)   

EUNIT_TEST(
    "NoCapabilityResponseL - test ",
    "CMusAvaDefaultOptionHandler",
    "NoCapabilityResponseL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaDefaultOptionHandler_NoCapabilityResponseLL, Teardown)
    
EUNIT_TEST(
    "NoCapabilityResponseL Op specific- test ",
    "CMusAvaDefaultOptionHandler",
    "NoCapabilityResponseL",
    "FUNCTIONALITY",
    SetupOpSpecificL,UT_CMusAvaDefaultOptionHandler_NoCapabilityResponseLOpSpecificL, Teardown)    
    
EUNIT_TEST(
    "CreateResponseL - test ",
    "CMusAvaDefaultOptionHandler",
    "CreateResponseL",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaDefaultOptionHandler_CreateResponseLL, Teardown)    
    
EUNIT_TEST(
    "TimedOut - test ",
    "CMusAvaDefaultOptionHandler",
    "TimedOut",
    "FUNCTIONALITY",
    SetupL,UT_CMusAvaDefaultOptionHandler_TimedOutL, Teardown)        
    
          
EUNIT_END_TEST_TABLE

//  END OF FILE
