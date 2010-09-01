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

// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files
// include this file. Directive can be removed when 
// EUnit is fixed.  
#pragma warn_illtokenpasting off

// Test values

_LIT( KTestRecipientSipUri, "sip:TestRecipient@host.domain" );
_LIT( KTestRecipientSipUriNoSipPrefix, "TestRecipient@host.domain" );
_LIT( KTestRecipientTelUri, "tel:+3581111111");
_LIT( KTestRecipientTelUriNoTelPrefix, "+3581111111" );

_LIT( KTestRecipientRandomText, "sfjsfjklsjdkfsj" );
_LIT( KTestRecipientSipUriWithSpace, "s:re cipi ent@" );
_LIT( KTestRecipientTelUriWithAtSign, "tel:+3581111111@host.domain" ); //invalid

_LIT8( KTestRecipientSipUri8, "sip:TestRecipient@host.domain" );
_LIT8( KTestRecipientTelUri8, "tel:+3581111111");
_LIT8( KTestRecipientRandomText8, "sfjsfjklsjdkfsj" );

_LIT8( KTestOriginator, "Frank Swis" );

_LIT( KTestAudioFileName, "audiofile" );
_LIT( KTestVideoFileName, "videofile" );
_LIT( KTestAnotherVideoFileName, "another" );

const TInt KSipStatusCodeNoCodeSet = 0;
const TInt KSipStatusCode200OK = 200;
const TInt KSipStatusCode400BadRequest = 400;
const TInt KSipStatusCode401Unauthorized = 401;
const TInt KSipStatusCode402PaymentRequired = 402;
const TInt KSipStatusCode404RecipientNotFound = 404;
const TInt KSipStatusCode407ProxyAuthenticationRequired = 407;
const TInt KSipStatusCode408ConnectionTimeOut = 408;
const TInt KSipStatusCode415UnsupportedMediaType = 415;
const TInt KSipStatusCode487RequestCancelled = 487;
const TInt KSipStatusCodeUnknown = 10000;
const TInt KTelephonyVolumeMinValue = 1;
const TInt KTelephonyVolumeDefaultValue = 4;
const TInt KTelephonyVolumeMaxValue = 1;

// Values originally introduced in telephonyinternalcrkeys.h

const TUid KCRUidCallHandling = {0x101F8784};
const TUint32 KTelephonyIncallEarVolume                           = 0x00000001;
const TUint32 KTelephonyIncallLoudspeakerVolume                   = 0x00000002;


// Helper macros

#define MUS_TEST_FORWARD_ALLOC_FAILURE( error )\
    if ( error == KErrNoMemory )\
        {\
        User::Leave( error );\
        }

#define ESTABLISH_OUT_SESSION( outSession )\
    outSession->InviteL( KTestRecipientSipUri );\
    outSession->iSession->iState = CMceSession::EEstablished;\
    if ( outSession->iSession->iManager &&\
         outSession->iSession->iManager->iSessionObserver )\
        {\
        outSession->iSession->iManager->iSessionObserver->SessionStateChanged(\
            *(outSession->iSession),\
            outSession->iSession->iManager->iContainer );\
        }
    
    
#endif // MUSENGTESTDEFS_H

