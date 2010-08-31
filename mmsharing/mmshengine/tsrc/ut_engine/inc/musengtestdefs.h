/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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



#ifndef MUSENGTESTDEFS_H
#define MUSENGTESTDEFS_H

#include <mcesession.h>
#include <mcemanager.h>

#include <digia/eunit/eunitmacros.h>
// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files
// include this file. Directive can be removed when 
// EUnit is fixed.  
#pragma warn_illtokenpasting off

// Test values

_LIT( KTestRecipientSipUri, "sip:TestRecipient@host.domain" );
_LIT( KTestRecipient2SipUri, "sip:TestRecipient2@host.domain" );
_LIT( KTestRecipientSipUriPrefixUpperCase, "SIP:TestRecipient@host.domain" );
_LIT( KTestRecipientSipUriNoSipPrefix, "TestRecipient@host.domain" );
_LIT( KTestRecipientTelUri, "tel:+3581111111");
_LIT( KTestRecipientTelUriNoTelPrefix, "+3581111111" );

_LIT8( KTestRecipientSipUri8, "sip:TestRecipient@host.domain" );
_LIT8( KTestRecipient2SipUri8, "sip:TestRecipient2@host.domain" );
_LIT8( KTestRecipientSipUriPrefixUpperCase8, "SIP:TestRecipient@host.domain" );
_LIT8( KTestRecipientSipUriNoSipPrefix8, "TestRecipient@host.domain" );
_LIT8( KTestRecipientTelUri8, "tel:+3581111111");
_LIT8( KTestRecipientTelUriNoTelPrefix8, "+3581111111" );

_LIT8( KTestRecipientRandomText8, "sfjsfjklsjdkfsj" );
_LIT8( KTestRecipientSipUriWithSpace8, "s:re cipi ent@" );
_LIT8( KTestRecipientTelUriWithAtSign8, "tel:+3581111111@host.domain" ); //invalid
_LIT8( KTestRecipientTelUriWithXCharAtEnd8, "tel:+3581111111X" ); //invalid
_LIT8( KTestRecipientTelUriWithXCharAtStart8, "tel:+X3581111111" ); //invalid

_LIT8( KTestRecipientLocalTelUriNoTelPrefix8, "3581111111" );
_LIT8( KTestRecipientLocalTelUriNoTelPrefixWithXCharAtEnd8, "3581111111X" ); //invalid
_LIT8( KTestRecipientLocalTelUriNoTelPrefixWithXCharAtStart8, "X3581111111" ); //invalid
_LIT8( KTestRecipientLocalTelUriNoTelPrefixWithXCharAtMiddle8, "35811X11111" ); //invalid

_LIT8( KTestRecipientLocalTelUri8, "tel:3581111111");
_LIT8( KTestRecipientLocalTelUriPrefixUpperCase8, "TEL:3581111111");
_LIT8( KTestRecipientLocalTelUriWithXCharAtEnd8, "tel:3581111111X" ); //invalid
_LIT8( KTestRecipientLocalTelUriWithXCharAtStart8, "tel:X3581111111" ); //invalid
_LIT8( KTestRecipientLocalTelUriWithXCharAtMiddle8, "tel:35811X11111" ); //invalid
_LIT8( KTestRecipientLocalTelUriTelPrefixAtMiddle8, "358tel:11X11111" ); //invalid

_LIT8( KTestOriginator, "Frank Swis" );

_LIT( KTestAudioFileName, "audiofile" );
_LIT( KTestVideoFileName, "H263videofile" );
_LIT( KTestAvcVideoFileName, "H264videofile" );

const TInt KSipStatusCodeNoCodeSet = 0;
const TInt KSipStatusCode200OK = 200;
const TInt KSipStatusCode400BadRequest = 400;
const TInt KSipStatusCode401Unauthorized = 401;
const TInt KSipStatusCode402PaymentRequired = 402;
const TInt KSipStatusCode404RecipientNotFound = 404;
const TInt KSipStatusCode407ProxyAuthenticationRequired = 407;
const TInt KSipStatusCode408ConnectionTimeOut = 408;
const TInt KSipStatusCode415UnsupportedMediaType = 415;
const TInt KSipStatusCode416UnsupportedUriScheme = 416;
const TInt KSipStatusCode479NotAbleToProcessURI = 479;
const TInt KSipStatusCode480TemporarilyNotAvailable = 480;
const TInt KSipStatusCode486BusyHere = 486;
const TInt KSipStatusCode487RequestCancelled = 487;
const TInt KSipStatusCode488NotAcceptableHere = 488;
const TInt KSipStatusCode603Decline = 603;
const TInt KSipStatusCode606NotAcceptable = 606;
const TInt KSipStatusCodeUnknown = 10000;

const TInt KTelephonyVolumeMinValue = 1;
const TInt KTelephonyVolumeDefaultValue = 4;
const TInt KTelephonyVolumeMaxValue = 1;

const TTimeIntervalMicroSeconds KMusEngTestFileDuration( 120000000 ); // 2 min
const TTimeIntervalMicroSeconds KMusEngTestFilePosition( 60000000 ); // 2 min

const TInt KTestMaxBrightness = 100;
const TInt KTestMoreThanMaxBrightness = 101;
const TInt KTestDefaultBrightness = 0;
const TInt KTestMinBrightness = -100;
const TInt KTestLessThanMinBrightness = -101;
const TInt KTestSomeBrightness = 25;
const TInt KTestBrightnessStepSize = 10;


// Values originally introduced in telephonydomaincrkeys.h

const TUid KCRUidInCallVolume = {0x102828B1};
const TUint32 KTelIncallEarVolume                           = 0x00000001;
const TUint32 KTelIncallLoudspeakerVolume                   = 0x00000002;


// Helper macros

#define MUS_TEST_FORWARD_ALLOC_FAILURE( error )\
    if ( error == KErrNoMemory )\
        {\
        User::Leave( error );\
        }

// NOTE! NMusSessionApi::KCategoryUid, NMusSessionApi::KRemoteSipAddress p&s property
// should contain recipient address to be used for invitation.
//
#define ESTABLISH_OUT_SESSION( outSession )\
    outSession->EstablishLcSessionL();\
    outSession->iSession->iState = CMceSession::EEstablished;\
    for ( TInt i = 0; i < outSession->iSession->Streams().Count(); ++i )\
            {\
            outSession->iSession->Streams()[i]->iState =\
                                                CMceMediaStream::EStreaming;\
            }\
    if ( outSession->iSession->iManager &&\
         outSession->iSession->iManager->iSessionObserver )\
        {\
        outSession->iSession->iManager->iSessionObserver->SessionStateChanged(\
            *(outSession->iSession),\
            outSession->iSession->iManager->iContainer );\
        }

#define ESTABLISH_OUT_SESSION_AND_START_STREAMING( outSession )\
    ESTABLISH_OUT_SESSION( outSession );\
    outSession->EnableDisplayL( ETrue );\
    outSession->LocalVideoPlayer()->LcPlayL();


#define MUSENG_EUNIT_ASSERT_EQUALS( func, val ) \
{\
TInt musUnitTestRetVal = func;\
if ( musUnitTestRetVal != KErrNoMemory ){\
    EUNIT_ASSERT_EQUALS( musUnitTestRetVal, val );\
    }\
else{\
    User::Leave( KErrNoMemory );\
    }\
}

#define MUSENG_EUNIT_ASSERT_SPECIFIC_LEAVE( func, val ) \
{\
TRAPD( musUnitTestRetVal, func );\
if ( val != KErrNoMemory && musUnitTestRetVal == KErrNoMemory ){\
    User::Leave( KErrNoMemory );\
    }\
else{\
    EUNIT_ASSERT_EQUALS( musUnitTestRetVal, val );\
    }\
}
    
#endif // MUSENGTESTDEFS_H

 
