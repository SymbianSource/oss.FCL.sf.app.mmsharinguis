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


#ifndef MUSPSKEYS_H
#define MUSPSKEYS_H

#include "musuid.hrh"
#include "musdomainpskeys.h"

namespace NMusSessionApi
    {
    // category uid
    const TUid KCategoryUid = { KMusManagerServerUid };
        
    // session p&s keys
    const TInt KUseCase( 0x11 ); // of type MultimediaSharing::TMusUseCase
    const TInt KStatus( KMusAvailabilityStatus ); // of type MultimediaSharing::TMusAvailabilityStatus
    const TInt KTelNumber( 0x13 );
    const TInt KRemoteSipAddress( 0x14 );
    const TInt KRemoteSipAddressProposal( 0x15);
    const TInt KContactId( 0x16 );
    
    // read this property bitwise
    const TInt KAudioCodecs( 0x17 );
    enum TAudioCodecs
        {
        EAmrNb = 0x1,
        EAmrWb = 0x2
        };
    
    // read this property bitwise
    const TInt KVideoCodecs( 0x18 );
    enum TVideoCodecs
        {
        EH263Video = 0x1,
        EH264Video = 0x2
        };
    
    const TInt KSignalingMode( 0x19 );
    enum TSignalingMode 
        {
        EIetfMode,
        EImsMode
        };
    
    const TInt KSipProfileId( 0x1A );
    const TInt KContactName( 0x1B );
    const TInt KMusIndicatorStatus( KMusIndicator );
    const TInt KFastMode( 0x1D );
    }

#endif // MUSPSKEYS_H
