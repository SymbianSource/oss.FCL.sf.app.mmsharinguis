/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSMANAGERSERVERCOMMON_H
#define MUSMANAGERSERVERCOMMON_H

const TInt KParameterCount = 7;

enum EMusManagerServerParameter
    {
    KTelNumber = 0,
    KSipAddress,
    KSipAddressProposal,
    KContactId,
    KVideoCodec,
    KSipProfileId,
    KContactName,
    KPrivacyStatus
    };

#endif // ? MUSMANAGERSERVERCOMMON_H
