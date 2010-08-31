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


#include "musunittesting.h"
#include "musmanagerstub.h"
#include "mussipcrplugin.h"
#include "mussipcrplugintest.h"
#include "muslogger.h" 
#include "musmanager.h"
#include "mussettings.h"

#include <alwaysonlinemanagercommon.h>
#include <alwaysonlinemanagerclient.h>
#include <digia/eunit/eunitmacros.h>
#include <SipResolvedClient.h>    
#include <sipstrings.h>
#include <sipstrconsts.h>
#include <SdpCodecStringPool.h>
#include <sdpdocument.h>

// Next row is to disable warning emerging from EUnit code.
#pragma warn_illtokenpasting off

_LIT8( KSdpSendOnly, "v=0\r\n\
o=Inviter2008 63241204263093750 132223800 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=video 49152 RTP/AVP 98\r\n\
a=sendonly\r\n\
a=rtpmap:98 H264/90000\r\n" );

_LIT8( KSdpSendRecv, "v=0\r\n\
o=Inviter2008 63241204263093750 132223800 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=video 49152 RTP/AVP 98\r\n\
a=sendrecv\r\n\
a=rtpmap:98 H264/90000\r\n" );

_LIT8( KSdpNoMedia, "v=0\r\n\
o=Inviter2008 63241204263093750 132223800 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n" );

_LIT8( KSdpNoVideo, "v=0\r\n\
o=Inviter2008 63241204263093750 132223800 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=audio 5000 RTP/AVP 106 8 0\r\n\
a=sendrecv\r\n\
a=rtpmap:106 AMR/8000\r\n" );

_LIT8( KSdpVideoNoAttributes, "v=0\r\n\
o=Inviter2008 63241204263093750 132223800 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=video 49152 RTP/AVP 98\r\n" );

_LIT8( KSdpRecvOnly, "v=0\r\n\
o=Inviter2008 63241204263093750 132223800 IN IP4 10.0.0.6\r\n\
s=-\r\n\
c=IN IP4 10.0.0.6\r\n\
t=0 0\r\n\
m=video 49152 RTP/AVP 98\r\n\
a=recvonly\r\n\
a=rtpmap:98 H264/90000\r\n" );


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
    SIPStrings::OpenL();
    }


// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
// 
void CMusSipCrPluginTest::Teardown(  )
    {
    SIPStrings::Close();
    delete iPlugin;
    }



// ------------------------- Test functions begin ---------------------------

// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
// 
void CMusSipCrPluginTest::UT_ConstructL()
    {
    // 1. String pool was opend by us
    EUNIT_ASSERT( iPlugin->iCloseStringPool );
        
    // 2. String pool was already opened by someone else, KErrAlreadyExists
    delete iPlugin;
    iPlugin = NULL;
    SdpCodecStringPool::OpenL();
    TRAPD( err, iPlugin = CMusSipCrPlugin::NewL() );
    if ( err != KErrNone )
        {
        SdpCodecStringPool::Close();
        User::Leave(err);
        }
    EUNIT_ASSERT( !iPlugin->iCloseStringPool );
    SdpCodecStringPool::Close();    
    }


// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
// 
void CMusSipCrPluginTest::UT_ChannelL()
    {
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

    // Everything is OK, 2-way video not supported
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
       
    // 2-way video is supported and sendrecv not present in SDP    
    MultimediaSharingSettings::iVideoDirection = MusSettingsKeys::ETwoWayVideo;    
    CMusManager::SetAvailability( 
                 ( MultimediaSharing::TMusAvailabilityStatus ) KErrNone );

    response = iPlugin->ChannelL( SIPStrings::StringF( SipStrConsts::EMessage ),
                       KNullDesC8(),
                       array,
                       KSdpSendOnly(),
                       NULL );

    EUNIT_ASSERT( response.iUid == CMusManager::ESipInviteDesired )
    
    // 2-way video is supported and sendrecv is present in SDP    
    response = iPlugin->ChannelL( SIPStrings::StringF( SipStrConsts::EMessage ),
                       KNullDesC8(),
                       array,
                       KSdpSendRecv(),
                       NULL );
        
    EUNIT_ASSERT( response.iUid == CMusManager::ESipInviteDesired2WayVideo )
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


// --------------------------------------------------------------------------
// 
// --------------------------------------------------------------------------
// 
void CMusSipCrPluginTest::UT_CheckForSendRecvAttributeL()
    {
    CSdpDocument* sdpDocument;
    TBool sendRecv;
    
    // 1. SDP doesn't have a single media field
    sdpDocument = CSdpDocument::DecodeLC( KSdpNoMedia() );
    sendRecv = iPlugin->CheckForSendRecvAttributeL( sdpDocument->MediaFields() );
    CleanupStack::PopAndDestroy( sdpDocument );
    EUNIT_ASSERT( !sendRecv );
    
    // 2. SDP has audio field, but no video
    sdpDocument = CSdpDocument::DecodeLC( KSdpNoVideo() );
    sendRecv = iPlugin->CheckForSendRecvAttributeL( sdpDocument->MediaFields() );
    CleanupStack::PopAndDestroy( sdpDocument );
    EUNIT_ASSERT( !sendRecv );
    
    // 3. SDP has video field without any attributes
    sdpDocument = CSdpDocument::DecodeLC( KSdpVideoNoAttributes() );
    sendRecv = iPlugin->CheckForSendRecvAttributeL( sdpDocument->MediaFields() );
    CleanupStack::PopAndDestroy( sdpDocument );
    EUNIT_ASSERT( sendRecv );
        
    // 4. SDP has video with "sendrecv" attribute
    sdpDocument = CSdpDocument::DecodeLC( KSdpSendRecv() );
    sendRecv = iPlugin->CheckForSendRecvAttributeL( sdpDocument->MediaFields() );
    CleanupStack::PopAndDestroy( sdpDocument );
    EUNIT_ASSERT( sendRecv );    
    
    // 5. SDP has video with "recvonly" attribute
    sdpDocument = CSdpDocument::DecodeLC( KSdpRecvOnly() );
    sendRecv = iPlugin->CheckForSendRecvAttributeL( sdpDocument->MediaFields() );
    CleanupStack::PopAndDestroy( sdpDocument );
    EUNIT_ASSERT( !sendRecv );
    }


//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE(
    CMusSipCrPluginTest,
    "CMusSipCrPluginTest",
    "CMusSipCrPluginTest" )
   
EUNIT_TEST(
    "ConstructL - test",
    "CMusSipCrPluginTest",
    "UT_ConstructL",
    "FUNCTIONALITY",
    SetupL, UT_ConstructL, Teardown)

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
    
EUNIT_TEST(
    "CheckForSendRecvAttribute - stest",
    "CMusSipCrPluginTest",
    "UT_CheckForSendRecvAttribute",
    "FUNCTIONALITY",
    SetupL, UT_CheckForSendRecvAttributeL, Teardown)
    
EUNIT_END_TEST_TABLE

