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


#ifndef MCEDEFS_H
#define MCEDEFS_H

#include <e32base.h>

const TInt KMceMaxSdpNameLength = 20;
const TUint8 KMceMaxPacketrate = 10;
const TInt KMcePTimeFactor = 1000;

const TInt KMceMaxSizeInReadStream = 1000;
const TInt KMceExternalizeBufferExpandSize = 100;
const TInt KMceArrayGranularity = 5;


_LIT8( KMceSDPNamePCMU, "PCMU" );
_LIT8( KMceSDPNamePCMA, "PCMA" );
_LIT8( KMceSDPNameAMR, "AMR" );

_LIT8( KMceSDPNameH263, "H263" );
_LIT8( KMceSDPNameH2632000, "H263-2000" );
_LIT8( KMceSDPNameH2631998, "H263-1998" );
_LIT8( KMceSDPNameH264, "H264" );

typedef TInt TMceReturnStatus;
const TMceReturnStatus KMceAsync = 1;
const TMceReturnStatus KMceReady = 0;

// SESSION MODIFIERS

/**
* Data type for session modifiers.
*/
typedef TUint32 TMceSessionModifier;


/** 
* Usage of QOS preconditions within session;
* This modifier can be set only when session is in state MceSession::EIdle. 
* By default, session is in preconditions supported mode.
*
* KMcePreconditionsNotUsed indicates that preconditions are not used.
* KMcePreconditionsSupported indicates that preconditions are supported
*/
// KEY
const TMceSessionModifier KMcePreconditions = 0;
// VALUES
const TUint KMcePreconditionsNotUsed = 0;
const TUint KMcePreconditionsSupported = 1;

/** 
* Usage of reliable provisional responses (aka 100rel) within session.
* This modifier can be set only when session is in state MceSession::EIdle 
* or MceSession::EIncoming.
* By default, session is in 100rel supported mode.
* 
* KMce100RelNotUsed indicates that 100rel is not used.
* KMce100RelSupported indicates that 100rel is supported.
* KMce100RelRequired indicates that 100rel is required.
*/
// KEY
const TMceSessionModifier KMce100Rel = 1;
// VALUES
const TUint KMce100RelNotUsed = 0;
const TUint KMce100RelSupported = 1;
const TUint KMce100RelRequired = 2;



#ifdef __MWERKS__
#pragma warn_emptydecl off
#endif

#ifdef MCE_UNIT_TEST
#include "mcetestdefs.h"
#else
#define MCE_UNIT_TEST_DEFS
#endif

#endif
