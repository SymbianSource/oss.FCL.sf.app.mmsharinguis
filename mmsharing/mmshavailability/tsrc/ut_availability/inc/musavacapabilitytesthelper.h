/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Capability Test helper
*
*/


#ifndef MUSAVACAPABILITYTESTHELPER_H
#define MUSAVACAPABILITYTESTHELPER_H


#include <e32std.h>
#include <bamdesca.h>
#include <siperr.h>
#include <sipaddress.h>
#include <sipextensionheader.h>
#include <sipcontactheader.h>
#include <sipacceptcontactheader.h>
#include <sipacceptheader.h>

#include <sipaddress.h>
#include <sipcontenttypeheader.h>
#include <sipfromheader.h>
#include <siptoheader.h>
#include <sipstrings.h>
#include <SipStrConsts.h>
#include <Uri8.h>
#include <SdpDocument.h>
#include <sdpcodecstringconstants.h>
#include <sdpcodecstringpool.h>



//  INTERNAL INCLUDES
#include "SIP.h"
#include "SIPConnection.h"
#include "SIPProfile.h"
#include "SIPClientTransaction.h"
#include "SIPServerTransaction.h"
#include "SIPRequestElements.h"
#include "SIPResponseElements.h"
#include "SIPMessageElements.h"


#include "musavacapabilitycontext.h"
#include "musavacapability.h"
#include "musavacapabilityexchange.h"
 
// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files
// include this file. Directive can be removed when 
// EUnit is fixed.  
#pragma warn_illtokenpasting off

_LIT8( KCapabilityTestTerminalID_A, "Terminal_A");
_LIT8( KCapabilityTestTerminalID_B, "Terminal_B");
_LIT8( KPAssertedIdentity_B, "sip:4455667788@foobar.com" );
_LIT( KPAssertedIdentity_B_16, "sip:4455667788@foobar.com" );
_LIT8( KPAssertedIdentity_B1, "tel:+4455667788");
_LIT( KPAssertedIdentity_B1_16, "tel:+4455667788");
_LIT8( KCapabilityTestAddress_A, "sip:a@foobar.com");
_LIT8( KCapabilityTestAddress_B, "sip:b@foobar.com");
_LIT8( KCapabilityPAssertedIdentity, "sip:passertedid@foobar.com");



_LIT8( KCapabilityTestSDP,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=application:com.gsma.rts\r\n\
a=type:videolive\r\n\
m=video 0 RTP/AVP 96\r\n\
a=rtpmap:96 H263-2000/90000\r\n");

_LIT8( KCapabilityTestSDPWrongCodec,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=application:com.gsma.rts\r\n\
a=type:videolive\r\n\
m=video 0 RTP/AVP 96\r\n\
a=rtpmap:96 MPEG-4/90000\r\n");

_LIT8( KCapabilityTestSDPWrongType,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=application:com.gsma.rts\r\n\
a=type:wrongtype\r\n\
m=video 0 RTP/AVP 96\r\n\
a=rtpmap:96 H263-2000/90000\r\n");

_LIT8( KCapabilityTestSDPWrongApplication,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=application:com.gsma.xxx\r\n\
a=type:videolive\r\n\
m=video 0 RTP/AVP 96\r\n\
a=rtpmap:96 H263-2000/90000\r\n");

_LIT8( KCapabilityTestSDPNoApplication,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=type:videolive\r\n\
m=video 0 RTP/AVP 96\r\n\
a=rtpmap:96 H263-2000/90000\r\n");

_LIT8( KCapabilityTestSDPNoAppNoType,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
m=video 0 RTP/AVP 96\r\n\
a=rtpmap:96 H263-2000/90000\r\n");

_LIT8( KCapabilityTestSDPNoAppWrongType,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=type:wrongtype\r\n\
m=video 0 RTP/AVP 96\r\n\
a=rtpmap:96 H263-2000/90000\r\n");

_LIT8( KCapabilityTestSDPNoType,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=application:com.gsma.rts\r\n\
m=video 0 RTP/AVP 96\r\n\
a=rtpmap:96 H263-2000/90000\r\n");

_LIT8( KCapabilityTestSDP264,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=application:com.gsma.rts\r\n\
a=type:videolive\r\n\
m=video 0 RTP/AVP 98\r\n\
a=rtpmap:98 H264/90000\r\n");

_LIT8( KCapabilityTestSDP264And263,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=application:com.gsma.rts\r\n\
a=type:videolive\r\n\
m=video 0 RTP/AVP 98 96\r\n\
a=rtpmap:98 H264/90000\r\n\
a=rtpmap:96 H263-2000/90000\r\n");

_LIT8( KCapabilityTestSDP263And264,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=application:com.gsma.rts\r\n\
a=type:videolive\r\n\
m=video 0 RTP/AVP 96 98\r\n\
a=rtpmap:98 H264/90000\r\n\
a=rtpmap:96 H263-2000/90000\r\n");

_LIT8( KCapabilityTestSDP264And263AndMpeg,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=application:com.gsma.rts\r\n\
a=type:videolive\r\n\
m=video 0 RTP/AVP 98 96 99\r\n\
a=rtpmap:98 H264/90000\r\n\
a=rtpmap:96 H263-2000/90000\r\n\
a=rtpmap:99 MPEG4/90000\r\n");

_LIT8( KCapabilityTestSDPFastMode,"v=0\r\n\
o=- 452027953 452027953 IN IP4 foobar.com\r\n\
s=-\r\n\
c=IN IP4 foobar.com\r\n\
t=0 0\r\n\
a=application:com.gsma.rts\r\n\
a=type:videolive\r\n\
a=keywds:fastmode\r\n\
m=video 0 RTP/AVP 96\r\n\
a=rtpmap:96 H263-2000/90000\r\n");


_LIT8( KCapabilityTestContentTypeOther, "application/text");

_LIT8( KRegisteredContact, "sip:registeredcontact@domain.com" );

_LIT8( KCapabilityTestAcceptContactHeader, "Accept-Contact: *;+g.3gpp.cs-voice;explicit");
_LIT8( KCapabilityTestAcceptHeader, "Accept: application/sdp");
_LIT8( KCapabilityTestContactHeader_A, "Contact: <sip:a@foobar.com>;+g.3gpp.cs-voice");
_LIT8( KCapabilityTestContactHeaderNoFeaturetag_A, "Contact: <sip:a@foobar.com>");
_LIT8( KCapabilityTestContactHeader_A1, "Contact: <user@aa>;+g.3gpp.cs-voice");
_LIT8( KCapabilityTestContactHeader_A1Updated, "Contact: <sip:registeredcontact@domain.com>;+g.3gpp.cs-voice");
_LIT8( KCapabilityTestContactHeader_B, "Contact: sip:b@foobar.com;+g.3gpp.cs-voice");
_LIT8( KCapabilityTestSIPAddress_B, "b@localhost");
_LIT8( KCapabilityTestUserAgent_A, "User-Agent: Terminal_A");
_LIT8( KCapabilityTestUserAgent_B, "User-Agent: Terminal_B");
_LIT( KCapabilityTestSIPAddressUri_B, "b@foobar.com");
_LIT( KCapabilityTestSIPAddressUri_BSIP, "sip:b@foobar.com");
_LIT8( KCapabilityTestSIPAddressUri8_B, "b@foobar.com");

_LIT8( KCapabilityTestDataFeature, "+g.3gpp.cs-data");
_LIT8( KCapabilitySwisFeatureFalse, "+g.3gpp.cs-voice;false");

#define MUS_HEADER_ASSERT( r, ndx, test )\
    if ( r )\
        {\
        HBufC8* header = r->MessageElements().UserHeaders()[ ndx ]->ToTextLC();\
        EUNIT_ASSERT( header->Compare( test ) == 0 );\
        CleanupStack::PopAndDestroy( header );\
        }
        

/**
 * Capability Test helper
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CapabilityTestHelper
    {

public: // static API


    static CSIPServerTransaction* OptionsRequestL( const TDesC8& aRequestUri,
                                                   const TDesC8& aFrom,
                                                   const TDesC8& aAcceptContact, 
                                                   const TDesC8& aContact,
                                                   const TDesC8& aAccept,
                                                   const TDesC8& aUserAgent );

    static CSIPServerTransaction* OptionsRequestL( const TDesC8& aRequestUri,
                                                   const TDesC8& aFrom,
                                                   const TDesC8& aAcceptContact, 
                                                   const TDesC8& aContact,
                                                   const TDesC8& aAccept,
                                                   const TDesC8& aUserAgent,
                                                   const TDesC8& aAssertId);


    static CSIPClientTransaction* OptionsResponseL( TUint aResponse,
                                                    TInt aPhrase,
                                                    const TDesC8& aFrom,
                                                    const TDesC8& aContact,
                                                    const TDesC8& aUserAgent,
                                                    const TDesC8& aPAssertedId,
                                                    const TDesC8& aSdp );

    static void SetResponseL( CSIPClientTransaction& aTrx,
                              TUint aResponse, 
                              TInt aPhrase,
                              const TDesC8& aFrom,
                              const TDesC8& aContact,
                              const TDesC8& aUserAgent,
                              const TDesC8& aPAssertedId,
                              const TDesC8& aSdp,
                              const TDesC8& aContentType = KNullDesC8 );

    static void SetResponse2L( CSIPClientTransaction& aTrx,
                              TUint aResponse, 
                              TInt aPhrase,
                              const TDesC8& aFrom,
                              const TDesC8& aContact,
                              const TDesC8& aUserAgent,
                              const TDesC8& aPAssertedId,
                              const TDesC8& aPAssertedId2,
                              const TDesC8& aSdp,
                              const TDesC8& aContentType = KNullDesC8 );

    
    };


class HelpFlags
    {
public:
    static CSIPAddress* iTestSipAddress;
    static TBool iUseTestSipAddress;
    static TBool iEmptyContactHeader;    
    };


#endif // MUSAVACAPABILITYTESTHELPER_H
