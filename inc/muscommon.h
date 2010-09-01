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



#ifndef MUSCOMMON_H
#define MUSCOMMON_H

#include <e32std.h>
#include <e32cmn.h>

namespace Mus
    {
    // sip uids for client resolver cooperation
    // for invitations
    const TUid KAllowedSipInvite = { 0x1002000 };
    const TUid KNotAllowedSipInvite = { 0x1002001 };
        
    // for options
    const TUid KSipOptions = { 0x1002002 };
    
    // resource availability
    enum TMusResourceAvailability
        {
        EMusAvailable,          // resource available
        EMusNotAvailable        // resource not available
        };
    
    // multimediasharing environmental error codes
    enum TMusError
        {
        ENoActiveCall = 1,      // no active call -> sharing not possible, error
        EConferenceCall,        // current call is a conf call -> error
        ECallInHold,            // current call is on hold -> error
        ENoNetwork,             // there's no packet network -> error
        ERoamingNetwork,        // sharing not allowed when roaming -> error
        ENoSipProfile,          // there's no sip profile -> error
        ESipRegistrationFailed, // sip registration failed -> error
        ESipOptionsFailed       // sip options revealed that the remote host is 
                                // not capable of video sharing -> error
        };
    }

#endif // MUSMANAGERCOMMON_H

// End of File
