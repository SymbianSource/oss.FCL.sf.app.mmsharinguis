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
* Description:  Main plugin class
*
*/

#include <sipstrings.h>
#include "musunittesting.h"
#include "musmanagerstub.h"
#include "mussipcrplugin.h"
#include "mussipcrplugintest.h"
#include "muslogger.h" 
#include "musmanager.h"

#include <alwaysonlinemanagercommon.h>
#include <alwaysonlinemanagerclient.h>
#include <digia/eunit/eunitmacros.h>
#include <SipResolvedClient.h>    
#include <sipstrconsts.h>
#include "FeatMgr.h"


// Next row is to disable warning emerging from EUnit code.
#pragma warn_illtokenpasting off


// --------------------------------------------------------------------------
// C++ constructor.
// --------------------------------------------------------------------------
// 
CMusSipCrPluginTest::CMusSipCrPluginTest()
    : CEUnitTestSuiteClass()
    {
    }


// --------------------------------------------------------------------------
// Symbian two-phase constructor.
// --------------------------------------------------------------------------
// 
CMusSipCrPluginTest* CMusSipCrPluginTest::NewL()
    {
    CMusSipCrPluginTest* self = new (ELeave) CMusSipCrPluginTest();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// --------------------------------------------------------------------------
// Symbian second-phase constructor.
// --------------------------------------------------------------------------
// 
void CMusSipCrPluginTest::ConstructL()
    {
    CEUnitTestSuiteClass::ConstructL();
    }


// --------------------------------------------------------------------------
// C++ destructor.
// --------------------------------------------------------------------------
// 
CMusSipCrPluginTest::~CMusSipCrPluginTest()
    {
    }



// ------------------------ Test case setup begin ---------------------------


// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
//     
void CMusSipCrPluginTest::SetupL(  )
    {
    iPlugin = CMusSipCrPlugin::NewL();
    }


// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
// 
void CMusSipCrPluginTest::Teardown(  )
    {
    delete iPlugin;
    }



// ------------------------- Test functions begin ---------------------------


// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
// 
void CMusSipCrPluginTest::UT_ChannelL()
    {
    SIPStrings::OpenL();
    
    RPointerArray<CSIPHeaderBase> array;
    
    TUid response;
    
    // Options
    response = iPlugin->ChannelL( 
                       SIPStrings::StringF( SipStrConsts::EOptions ),
                       KNullDesC8(),
                       array,
                       KNullDesC8(),
                       NULL );

    EUNIT_ASSERT( response.iUid == CMusManager::ESipOptions )

    // Everything is OK
    CMusManager::SetAvailability( 
                 ( MultimediaSharing::TMusAvailabilityStatus ) KErrNone );
    
    response = iPlugin->ChannelL( SIPStrings::StringF( SipStrConsts::EMessage ),
                       KNullDesC8(),
                       array,
                       KNullDesC8(),
                       NULL );
    
    EUNIT_ASSERT( response.iUid == CMusManager::ESipInviteDesired )
    
    // Server is shut down                     
    CMusManager::SetAvailability( MultimediaSharing::EErrServerShutDown );
    
    response = iPlugin->ChannelL( SIPStrings::StringF( SipStrConsts::EMessage ),
                       KNullDesC8(),
                       array,
                       KNullDesC8(),
                       NULL );

    EUNIT_ASSERT( response.iUid == CMusManager::ESipInviteNotDesired )

    // Mus is not supported
    CMusManager::SetAvailability(
            ( MultimediaSharing::TMusAvailabilityStatus ) KErrNotSupported );
    
    response = iPlugin->ChannelL( SIPStrings::StringF( SipStrConsts::EMessage ),
                       KNullDesC8(),
                       array,
                       KNullDesC8(),
                       NULL );
    
    EUNIT_ASSERT( response.iUid == CMusManager::ESipInviteNotDesired )
    //Feature Manager testing
    FeatureManager::MultimediaSharingNotSupported();
    response = iPlugin->ChannelL( 
    		           SIPStrings::StringF( SipStrConsts::EOptions ),
                       KNullDesC8(),
                       array,
                       KNullDesC8(),
                       NULL );

    EUNIT_ASSERT( response.iUid == CMusManager::ESipInviteNotDesired )
    // Everything is OK
    CMusManager::SetAvailability( 
                 ( MultimediaSharing::TMusAvailabilityStatus ) KErrNone );
    
    response = iPlugin->ChannelL( SIPStrings::StringF( SipStrConsts::EMessage ),
                       KNullDesC8(),
                       array,
                       KNullDesC8(),
                       NULL );
    
    EUNIT_ASSERT( response.iUid == CMusManager::ESipInviteNotDesired )
    
    // Server is shut down                     
    CMusManager::SetAvailability( MultimediaSharing::EErrServerShutDown );
    
    response = iPlugin->ChannelL( SIPStrings::StringF( SipStrConsts::EMessage ),
                       KNullDesC8(),
                       array,
                       KNullDesC8(),
                       NULL );

    EUNIT_ASSERT( response.iUid == CMusManager::ESipInviteNotDesired )

    // Mus is not supported
    CMusManager::SetAvailability(
            ( MultimediaSharing::TMusAvailabilityStatus ) KErrNotSupported );
    
    response = iPlugin->ChannelL( SIPStrings::StringF( SipStrConsts::EMessage ),
                       KNullDesC8(),
                       array,
                       KNullDesC8(),
                       NULL );
    
    EUNIT_ASSERT( response.iUid == CMusManager::ESipInviteNotDesired )
    FeatureManager::MultimediaSharingSupported();  
    SIPStrings::Close();                       
                        
    }


// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
// 
void CMusSipCrPluginTest::UT_ConnectL()
    {
    // Options
    TUid uid;
    uid.iUid = CMusManager::ESipOptions;
    iPlugin->ConnectL( uid );
    EUNIT_ASSERT_EQUALS( iPlugin->iManager->iHandleSipRequestLCalledWithParam,
                         CMusManager::ESipOptions )
    
    // Desired invite
    uid.iUid = CMusManager::ESipInviteDesired;
    iPlugin->ConnectL( uid );
    EUNIT_ASSERT_EQUALS( iPlugin->iManager->iHandleSipRequestLCalledWithParam,
                         CMusManager::ESipInviteDesired )
    
    // Not desired invite
    uid.iUid = CMusManager::ESipInviteNotDesired;
    iPlugin->ConnectL( uid );
    EUNIT_ASSERT_EQUALS( iPlugin->iManager->iHandleSipRequestLCalledWithParam,
                         CMusManager::ESipInviteNotDesired )
      
    // Unknown case, manager is not called and value does not change    
    uid.iUid = KErrNone;
    EUNIT_ASSERT_SPECIFIC_LEAVE( iPlugin->ConnectL( uid ), KErrNotSupported )
    EUNIT_ASSERT_EQUALS( iPlugin->iManager->iHandleSipRequestLCalledWithParam,
                         CMusManager::ESipInviteNotDesired )
    }


// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
// 
void CMusSipCrPluginTest::UT_Capabilities()
    {
    // Dummy test is enough
    iPlugin->Capabilities();
    }


// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
// 
void CMusSipCrPluginTest::UT_ImplementationGroupProxy()
    {
    // Dummy test
    TInt tableCount;
    
    ImplementationGroupProxy( tableCount );
    }




//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    CMusSipCrPluginTest,
    "CMusSipCrPluginTest",
    "CMusSipCrPluginTest" )

EUNIT_TEST(
    "CMusSipCrPluginTest",
    "CMusSipCrPluginTest",
    "ConnectL",
    "FUNCTIONALITY",
    SetupL, UT_ConnectL, Teardown)

EUNIT_TEST(
    "CMusSipCrPluginTest",
    "CMusSipCrPluginTest",
    "ChannelL",
    "FUNCTIONALITY",
    SetupL, UT_ChannelL, Teardown)

EUNIT_TEST(
    "CMusSipCrPluginTest",
    "CMusSipCrPluginTest",
    "Capabilities",
    "FUNCTIONALITY",
    SetupL, UT_Capabilities, Teardown)

EUNIT_TEST(
    "CMusSipCrPluginTest",
    "CMusSipCrPluginTest",
    "ImplementationGroupProxyTest",
    "FUNCTIONALITY",
    SetupL, UT_ImplementationGroupProxy, Teardown)
    
EUNIT_END_TEST_TABLE

