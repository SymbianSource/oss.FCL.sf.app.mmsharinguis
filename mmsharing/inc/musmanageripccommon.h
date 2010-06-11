/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/



#ifndef MUSMANAGERIPCCOMMON_H
#define MUSMANAGERIPCCOMMON_H

#include <e32std.h>

#include "musuid.hrh"

_LIT( KMusManagerServerName,"musmanagerserver" );
_LIT( KMusManagerServerFilename, "musmanagerserver" );
_LIT( KMusManagerServerSemaphoreName, "musmanagerserverSemaphore" );

const TUid KServerUid3 = { KMusManagerServerUid };

#if (defined (__WINS__) || defined(__WINSCW__))
static const TUint KServerMinHeapSize =  0x1000;  //   4K
static const TUint KServerMaxHeapSize = 0x80000;  // 512K (Should be enough)
#endif

// The server version.
// A version must be specified when creating a session with the server
const TUint KMusManagerServerMajorVersionNumber=0;
const TUint KMusManagerServerMinorVersionNumber=1;
const TUint KMusManagerServerBuildVersionNumber=1;

// A container for incoming buffer sizes
class TMusManagerMessageBufSizes
    {
public:

    TMusManagerMessageBufSizes()
        : iHeaderBufSize(0),
          iContentBufSize(0) {}

    TMusManagerMessageBufSizes(TInt aHeaderBufSize, TInt aContentBufSize)
        : iHeaderBufSize(aHeaderBufSize),
          iContentBufSize(aContentBufSize) {}

	TMusManagerMessageBufSizes(const TMusManagerMessageBufSizes& aSizes)
        : iHeaderBufSize(aSizes.iHeaderBufSize),
          iContentBufSize(aSizes.iContentBufSize) {}

    TInt iHeaderBufSize;
    TInt iContentBufSize;
    };


// Oper. codes used in passing IPC messages between C/S-client and C/S-server
enum TMusManagerIpcFunctions
    {
    // No capabilities required 0-3
    EMusManagerIpcAvailabilityQuery, // Asynchronous receive
    EMusManagerIpcInvestigateAvailability,
    EMusManagerIpcInvitationReceived,
    EMusManagerIpcOptionsReceived,
    EMusManagerIpcStartMultimediaSharing,
    EMusManagerIpcStopMultimediaSharing,
    EMusManagerIpcStartObservingAvailability,
    EMusManagerIpcMonitorAvailability,
    EMusManagerIpcCancelMonitoring,
    EMusManagerIpcHandleCommand
    };


// Indexes used in IPC parameter array passed between C/S-client and C/S-server
//
// All the parameters except EMusManagerIpcArgMessage are packed into package buffers
// to enable IPC parameter in-out behaviour.
//
// NOTE: Remember to update the types in comments if the actual types change.
//
enum TMusManagerIpcArguments
    {
    EMusManagerIpcArgAvailability = 0,        // Type: TUint32
    EMusManagerIpcArgChannelUid = 0,        // Type: TUid
    EMusManagerIpcArgUseCase = 0,           // Type: TInt
    EMusManagerIpcArgSipResponse = 1,       // Type: TDes8/TDesC8
    EMusManagerIpcArgSipRequest = 1,        // Type: TDesC8
    EMusManagerIpcArgBufSizes = 2,          // Type: TMusManagerMessageBufSizes
    EMusManagerIpcArgSipMessageContent = 2, // Type: TDes8/TDesC8
    EMusManagerIpcArgResolverUid = 3,       // Type: TUid
    EMusManagerIpcArgRequestStatus = 0,     // Type: TRequestStatus
    EMusManagerIpcArgAvailabilityStatus = 1, // Type: TAvailabilityStatus
    EMusManagerIpcArgCommandType = 0		// Type: TCommandType
    };
    
enum TMusManagerIpcResponses
    {
    EMusManagerChannelComplete = 1,
    EMusManagerClientNotFound
    };

#endif // MUSMANAGERIPCCOMMON_H

// End of File
