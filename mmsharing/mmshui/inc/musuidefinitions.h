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
* Description:  The MUS application's UI class.
*
*/



#ifndef MUSUIDEFINITIONS_H
#define MUSUIDEFINITIONS_H

#include <e32base.h>


// View UIDs
const TUid KMusUidNullView        = { 0 };
const TUid KMusUidReceivingView   = { 1 };
const TUid KMusUidLiveSharingView = { 2 };
const TUid KMusUidClipSharingView = { 3 };
const TUid KMusUidGeneralView     = { 4 };
const TUid KMusUidSendView        = { 5 };

// Volume settings
const TInt KMusVolumeMuted 		=  0;
const TInt KMusVolumeMaximum 	= 10;
const TInt KMusVolumeDefault 	=  5;

// protocol prefixes and address components
const TInt KMusSipAddressesMaxAmount  =   8;
const TInt KMusPrefixMaxLength     =   4;
const TInt KMusSipAddressMaxLength = 256;
_LIT( KMusPrefixSipUri,            "sip:" );
_LIT( KMusPrefixTelUri,            "tel:" );
_LIT( KMusAtSymbol,                "@" );
_LIT( KMusSpaceSymbol,             " " );
_LIT( KMusEmptyStringSymbol,       "" );
_LIT( KMusCommaSymbol,             "," );
_LIT( KMusVideoFileNameExtension,  ".3gp" );
_LIT( KMusVideoFileNameBegining,   "VS" );
_LIT( KMusVideoFileNameVersion,    "V" );

const TInt KMusDriveAndColonStringLength = 2;
const TInt KMusPercentUStringLength = 2;
const TInt KMusVideoFileNameNumberStringStartPos = 7;
const TInt KMusVideoFileNameNumberStringLength = 3;

const TInt KMusDefaultArrayGranularity = 5;
const TInt KMusStringSubstArrayGranularity = 2;

const TInt KMusOneHundredPercent = 100;

const TInt KMusErrorMessageMaxLength = 512;
const TInt KMusNameFieldMaxLength    = 512;
const TInt KMusFileNameMaxLength    = 256;

// time constants
const TInt KMusOneSecond           =  1000000; // one second in microseconds
const TInt KMusInviteExpireTimeout 	= 60; // seconds
const TInt KMusReceiveTimeout 	    = 10 * KMusOneSecond;   // ten seconds
const TInt KMusOneMinuteInSeconds   = 60;
const TInt KMusNumberValueTen          = 10;
const TInt KMusLocaleHoursAndMinutesSeparator = 1;
const TInt KMusLocaleMinutesAndSecondsSeparator = 2;
const TInt KMusBacklightTimeOut = 4;    // seconds

const TInt KMusNumberValueTwo          = 2;

/** asynchronous UI events */
enum TMusUiAsyncEvent
    {
    EMusUiAsyncUninitialized = 0,
    EMusUiAsyncInviteCancel,
    EMusUiAsyncManualAddressEntry,
    EMusUiAsyncInviteAccepted,
    EMusUiAsyncStartInvitation,
    EMusUiAsyncStartMediaGallery,
    EMusUiAsyncStartTranscoding,
    EMusUiAsyncHandleExit,
    EMusUiAsyncRefreshView
    };

enum TMusUiNaviMediaDecorator
	{
	EMusUiNaviIconNone = -1,
	EMusUiNaviIconForward,
	EMusUiNaviIconPause,
	EMusUiNaviIconPlay,
	EMusUiNaviIconRecord,
	EMusUiNaviIconRecordNot,
	EMusUiNaviIconRew,
	EMusUiNaviIconStop,
	EMusUiNaviIconWaitAnim
	};

enum TMusUiLoudspeakerStatus
	{
	ELoudspeakerDisabled = 0,
	ELoudspeakerEnabled = 1
	};

enum TMusUiLoudspeakerDialogStatus
	{
	ELoudspeakerDialogDisabled = 0,
	ELoudspeakerDialogEnabled = 1
	};

enum TMusUiVolumeStatus
	{
	EVolumeDown = 0,
	EVolumeUp = 1
	};
	
enum TMusUiIndicatorType
    {
    EMusUiIndicatorTypeZoom = 0,
    EMusUiIndicatorTypeBrightness,
    EMusUiIndicatorTypeDuration
    };



#endif // MUSUIDEFINITIONS_H


// end of file
