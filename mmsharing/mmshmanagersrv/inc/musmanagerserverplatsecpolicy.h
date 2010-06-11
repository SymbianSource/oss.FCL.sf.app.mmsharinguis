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


#ifndef MUSMANAGERSERVERPLATSECPOLICY_H
#define MUSMANAGERSERVERPLATSECPOLICY_H

/**
 * Number of different IPC ranges 
 */
const TUint KMusManagerServerPlatSecRangeCount = 4;

/**
 * IPC ranges segmented into groups based on their capability requirements
 */
const TInt KMusManagerServerPlatSecRanges[KMusManagerServerPlatSecRangeCount] =
    {
    0,  // IPC message 0 - 3
    4,	// IPC message 4 - 6
    7,	// IPC message 7 - 9
    10	// Not Supported 10 ->
    };

/**
 * IPC segments and their capability requirements
 */
const TUint8 KMusManagerServerPlatSecElementsIndex[KMusManagerServerPlatSecRangeCount] =
    {
	CPolicyServer::EAlwaysPass, // Security policy for ITC 0-3
    0, // Security policy for IPC 0-2 (WriteDeviceData)
    1, // Security policy for IPC 3-9 (ReadDeviceData)
    CPolicyServer::ENotSupported // Not Supported 10 ->
    };
	
/**
 * Availability sets and the action performed in case of failure
 */	
const CPolicyServer::TPolicyElement KMusManagerServerPlatSecElements[2] =
    {	
    // Requires the user to have WriteDeviceData capability, otherwise the message is 
    // completed with KErrPermissionDenied
    { _INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData), CPolicyServer::EFailClient },
	
	// Requires the user to have ReadDeviceData capability, otherwise the message is 
	// completed with KErrPermissionDenied
	{ _INIT_SECURITY_POLICY_C1(ECapabilityReadDeviceData), CPolicyServer::EFailClient },
	
    };
	
/**
 * Platform security policy that SIP Server uses. 
 */	
const CPolicyServer::TPolicy KMusManagerServerPlatSecPolicy =
    {
    CPolicyServer::EAlwaysPass, 	            // Specifies all connect attempts should pass
    KMusManagerServerPlatSecRangeCount,		// Number of IPC ranges
    KMusManagerServerPlatSecRanges,			// IPC Ranges
    KMusManagerServerPlatSecElementsIndex,		// Policy to ITC mapping array
    KMusManagerServerPlatSecElements			// Policies used in MusManagerServer
    };

#endif // ? MUSMANAGERSERVERPLATSECPOLICY_H
