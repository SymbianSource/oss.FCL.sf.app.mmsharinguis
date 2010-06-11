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
#include "UT_CMusAvaSipheaderUtil.h"
#include "musavasipheaderutil.h"
#include "muslogger.h"
#include "musavasipobserver.h"
#include "musavasipconnectionobserver.h"

//  EXTERNAL INCLUDES
#include <digia/eunit/eunitmacros.h>
#include <e32base.h>
#include <sipstrings.h>
#include <SdpCodecStringPool.h>
#include <siprequestelements.h>
#include <SipMessageElements.h>
#include <sip.h>
#include <sipconnection.h>
#include <sipheaderbase.h>
#include <digia/eunit/eunitdecorators.h>



// CONSTRUCTION
UT_CMusAvaSipheaderUtil* UT_CMusAvaSipheaderUtil::NewL()
    {
    UT_CMusAvaSipheaderUtil* self = UT_CMusAvaSipheaderUtil::NewLC(); 
    CleanupStack::Pop();
    return self;
    }

UT_CMusAvaSipheaderUtil* UT_CMusAvaSipheaderUtil::NewLC()
    {
    UT_CMusAvaSipheaderUtil* self = new( ELeave ) UT_CMusAvaSipheaderUtil();
    CleanupStack::PushL( self );
	self->ConstructL(); 
    return self;
    }

// Destructor (virtual by CBase)
UT_CMusAvaSipheaderUtil::~UT_CMusAvaSipheaderUtil()
    {
    }

// Default constructor
UT_CMusAvaSipheaderUtil::UT_CMusAvaSipheaderUtil()
    {
    }

// Second phase construct
void UT_CMusAvaSipheaderUtil::ConstructL()
    {
    // The ConstructL from the base class CEUnitTestSuiteClass must be called.
    // It generates the test case table.
    CEUnitTestSuiteClass::ConstructL();
    }

//  METHODS



void UT_CMusAvaSipheaderUtil::SetupL(  )
    {   
    SIPStrings::OpenL();
    SdpCodecStringPool::OpenL();
    } 

void UT_CMusAvaSipheaderUtil::Teardown(  )
    {    
    iRequestHeaders.ResetAndDestroy();
    SIPStrings::Close();
    SdpCodecStringPool::Close();
    } 

void UT_CMusAvaSipheaderUtil::UT_AddAcceptContactHeaderLL()
    {    
    _LIT8( KCapabilitySwisFeature, "+g.3gpp.cs-voice");
    RStringF featureTag = SIPStrings::Pool().OpenFStringL( KCapabilitySwisFeature );
    CleanupClosePushL( featureTag );
    CMusAvaSipheaderUtil::AddAcceptContactHeaderL(iRequestHeaders,featureTag);
    CleanupStack::PopAndDestroy();
    EUNIT_ASSERT( iRequestHeaders.Count()==1 );
    }  

void UT_CMusAvaSipheaderUtil::UT_AddAcceptEncodingHeaderLL()
    {          
    CMusAvaSipheaderUtil::AddAcceptEncodingHeaderL(iRequestHeaders);
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count == 0 )
        {
        EUNIT_ASSERT( iRequestHeaders.Count()==1 );
        }
    else
        {
        EUNIT_ASSERT( iRequestHeaders.Count()==0 );
        }
    }  

void UT_CMusAvaSipheaderUtil::UT_AddAcceptLanguageHeaderLL()
    {          
    CMusAvaSipheaderUtil::AddAcceptLanguageHeaderL(iRequestHeaders);
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count == 0 )
        {
        EUNIT_ASSERT( iRequestHeaders.Count()==1 );
        }
    else
        {
        EUNIT_ASSERT( iRequestHeaders.Count()==0 );
        }
    }  

void UT_CMusAvaSipheaderUtil::UT_AddPreferredIdentityHeaderLL()
    {          
    _LIT8(val,"sip:siva@sofia.net");
    CMusAvaSipheaderUtil::AddPreferredIdentityHeaderL(iRequestHeaders,val);
    EUNIT_ASSERT( iRequestHeaders.Count()==1 );
    }  

void UT_CMusAvaSipheaderUtil::UT_AddAcceptSdpHeaderLL()
    {          
    CMusAvaSipheaderUtil::AddAcceptSdpHeaderL(iRequestHeaders);
    TInt count = 0;
    EUNIT_GET_ALLOC_DECORATOR_FAILCOUNT( count );
    if ( count == 0 )
        {
        EUNIT_ASSERT( iRequestHeaders.Count()==1 );
        }
    else
        {
        EUNIT_ASSERT( iRequestHeaders.Count()==0 );
        }
    }  

void UT_CMusAvaSipheaderUtil::UT_AddSdpLL()
    {              
    CUri8* remoteUri = CUri8::NewL();
    CleanupStack::PushL( remoteUri );
    _LIT8(uri,"siva@sofia.net");
    remoteUri->SetComponentL(uri,EUriHost);
    CSIPRequestElements* rElements = CSIPRequestElements::NewL(remoteUri);
    CleanupStack::Pop( remoteUri );
    CleanupStack::PushL( rElements );
    _LIT8(contentDes,"v=0\n/");
    /*o=- 63359607229508250 63359607229508250 IN IP4 10.21.32.54\n/
    s=-\n/c=IN IP4 10.21.32.51\n/
    t=0 0\na=X-application:com.nokia.rtvs\nm=video 49152 RTP/AVP 96\n/
    a=sendonly\na=curr:qos local send\na=curr:qos remote none\n/
    a=des:qos mandatory local send\na=des:qos none remote send\n/
    a=rtpmap:96 H263-2000/90000\na=framerate:15\na=framesize:96 176-144\n/
    a=fmtp:96 profile=0; level=45\n");    */
    HBufC8* content = HBufC8::NewLC(contentDes().Length());
    TPtr8 contentPtr = content->Des();
    contentPtr.Copy(contentDes);
    CMusAvaSipheaderUtil::AddSdpL(rElements,content);
    CleanupStack::Pop(content);    
    EUNIT_ASSERT(rElements->MessageElements().Content().Length()>0);    
    CleanupStack::PopAndDestroy(rElements);
    }  

void UT_CMusAvaSipheaderUtil::UT_LocalHostLL()
    {
    TUid uid = TUid::Uid(0);
    // just for compilation this will be ignored in stub.
    CMusAvaSipObserver* tempObs = CMusAvaSipObserver::NewL();
    CleanupStack::PushL(tempObs);
    CSIP* sip = CSIP::NewLC(uid,*tempObs);
    TUint32 aIapId(0);
    CMusAvaSipConnectionObserver* connObs = CMusAvaSipConnectionObserver::NewL();
    CleanupStack::PushL(connObs);
    CSIPConnection* connection = CSIPConnection::NewL(*sip,aIapId,*connObs);
    CleanupStack::PushL(connection);
    TPtrC8 host;
    CMusAvaSipheaderUtil::LocalHostL(*connection,host);
    CleanupStack::PopAndDestroy(4);
    EUNIT_ASSERT(host.Length() > 0);  
    }
                        
	
//  TEST TABLE

EUNIT_BEGIN_TEST_TABLE( 
    UT_CMusAvaSipheaderUtil,
    "CMusAvaSipheaderUtil",
    "UNIT" )

EUNIT_TEST(
    "AddAcceptContactHeaderL - test ",
    "CMusAvaSipheaderUtil",
    "AddAcceptContactHeaderL",
    "FUNCTIONALITY",
     SetupL, UT_AddAcceptContactHeaderLL, Teardown)

EUNIT_TEST(
    "AddAcceptEncodingHeaderL - test ",
    "CMusAvaSipheaderUtil",
    "AddAcceptEncodingHeaderL",
    "FUNCTIONALITY",
     SetupL, UT_AddAcceptEncodingHeaderLL, Teardown)

EUNIT_TEST(
    "AddAcceptLanguageHeaderL - test ",
    "CMusAvaSipheaderUtil",
    "AddAcceptLanguageHeaderL",
    "FUNCTIONALITY",
     SetupL, UT_AddAcceptLanguageHeaderLL, Teardown)

EUNIT_TEST(
    "AddPreferredIdentityHeaderL - test ",
    "CMusAvaSipheaderUtil",
    "AddPreferredIdentityHeaderL",
    "FUNCTIONALITY",
     SetupL, UT_AddPreferredIdentityHeaderLL, Teardown)

EUNIT_TEST(
    "AddAcceptSdpHeaderL - test ",
    "CMusAvaSipheaderUtil",
    "AddAcceptSdpHeaderL",
    "FUNCTIONALITY",
     SetupL, UT_AddAcceptSdpHeaderLL, Teardown)

EUNIT_TEST(
    "AddSdpL - test ",
    "CMusAvaSipheaderUtil",
    "AddSdpL",
    "FUNCTIONALITY",
     SetupL, UT_AddSdpLL, Teardown)

EUNIT_TEST(
    "LocalHostL - test ",
    "CMusAvaSipheaderUtil",
    "LocalHostL",
    "FUNCTIONALITY",
     SetupL, UT_LocalHostLL, Teardown)
 

EUNIT_END_TEST_TABLE

//  END OF FILE
