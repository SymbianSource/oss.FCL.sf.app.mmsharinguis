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
* Description:  project specification
*
*/


#ifndef MUSWPADAPTERUID_H
#define MUSWPADAPTERUID_H

#include "../../../inc/musuid.hrh"

// The next two UIDs are copies from the non-public header 
// ProvisioningUIDs.h. They are defined again in this file because 
// MusAdapter is not going to be part of S60 Provisioning package 
// and thus can't require provisioning source codes.

// ECOM plugin
#define KECOMUID2 0x10009D8D

/// UID of the Provisisioning Adapter plugin interface
#define KPROVISIONINGADAPTERINTERFACE 0x101F84D5


// The following UIDs are for MusAdapter

// UID2 for the MusAdapter. ECOM plug-in.
#define KMUSWPADAPTERDLLUID2 KECOMUID2

/// UID3 for the MusAdapter DLL
#define KMUSWPADAPTERDLLUID3 KMusWpAdapterUid


#endif // MUSWPADAPTERUID_H
