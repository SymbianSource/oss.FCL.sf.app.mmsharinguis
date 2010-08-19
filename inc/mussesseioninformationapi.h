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


#ifndef MUSSESSIONINFORMATIONAPI_H
#define MUSSESSIONINFORMATIONAPI_H

#include <e32cmn.h>
#include "musuid.hrh"

/**
 *  A namespace container for MultimediaSharing Resource API constants.
 *
 *  @since S60 v3.2
 */
namespace NMusSessionInformationApi
    {
    /**  The category of these P&S properties. */
#ifndef UNIT_TESTING  
     const TUid KCategoryUid = { 0x101F85EE }; // UID3 of AlwaysOnlineServer  
#else
  const TUid KCategoryUid = { 0x01700a21 }; // UID3 of EunitTest of AO
#endif

    /**  Contains the call event state */
    const TInt KMusCallEvent( 0x1028239F );

    /**  Contains the telephone number of call */
    const TInt KMusTelNumber( 0x102823A0 );    

    /**  Contains the Number of Active Calls */
    const TInt KMusCallCount( 0x102823A1 );    

    /** Contains the possible values of call event */
    enum TMusCallEvent
        {
        ENoCall,
        ECallConnected,
        ECallHold,       
        EConferenceCall,
        EConferenceTerminated
        };

    /**  Contains the call direction */
    const TInt KMusCallDirection( 0x102823A2 );

    /** Contains the call direction */
    enum TMusCallDirection
        {
        ENoDirection,
        ECallOrginated,
        ECallTerminated
        };

    /**  Contains the state of options response. */
    const TInt KMUSForbidden( 0x102823A3 );
    /**  Contains the possible values of options response. */
    enum TMUSForbidden
        {
        EMUSAllowed = 1,
        EMUSForbidden
        };
    
    /**  Contains the call privacy */
    const TInt KMUSPrivacy( 0x102823A4 );
    /**  Contains the possible values for call privacy*/
    enum TMusCallPrivacy
        {
        EPrivacyOn = 1,
        EPrivacyOff
        };
    
    /**  Phone's CLIR setting. This is used in MO case to determine if the
     *   caller's phone number/id is sent to remote party */
    const TInt KMusClirSetting( 0x102823A5 );
    /**  Possible values for KMusClirSetting */
    enum TMusClirSetting
        {
        ESendOwnNumber,
        EDoNotSendOwnNumber
        };
    }

#endif // MUSRESOURCEPROPERTIES_H
