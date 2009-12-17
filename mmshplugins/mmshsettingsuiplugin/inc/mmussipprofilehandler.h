/*
* Copyright (c) 2003-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Mixin-class that provides functionality for manipulating
*                MuS specific settings.
*  Version     : %version: 4.1.3 % << Don't touch! Updated by Synergy at check-out.
*
*/


#ifndef M_MMUSSIPPROFILEHANDLER_H
#define M_MMUSSIPPROFILEHANDLER_H

#include <sipmanagedprofile.h>

/**
*  MMusSIPProfileHandler class
*  Mixin class for SIP profile manipulation.
*  Handles SIP profile manipulations.
*  @since S60 v3.2
*/
class MMusSIPProfileHandler
    {
public:

    /**
     * Returns the index of the default SIP profile.
     * @return KErrNotFound, if one does not exist, otherwise index of the
     *         default SIP profile.
     */
    virtual TInt DefaultProfileIndex() = 0;

    /**
     * Returns pointer to the SIP profile array, does not change ownership.
     * Note that previously returned value goes out of scope after a new
     * call to ProfileArrayL. So it is not advised to store return value
     * in long persistent variable, but rather to be used like stack
     * variables for safety.
     *
     * @return Pointer to internally cached SIP profile array.
     */
    virtual CArrayPtr<CSIPManagedProfile>* ProfileArrayL() = 0;

    /**
     * Returns the id of the default SIP profile.
     * @return KErrNotFound, if one does not exist, otherwise id of the
     *         default SIP profile.
     */
    virtual TUint32 DefaultProfileId() = 0;
    
    /*
     * 
     * 
     */
    virtual void DisableProfileL() = 0;

    /**
     * Returns profile id based on index.
     * @param aIndex Profile index
     * @return Profile id or KUnknownProfileId if no matching profile is found.
     */
    virtual TInt ProfileIndexByIdL( TUint32 aId ) = 0;

    /**
     * Returns SIP profile index on locally cached array based on id.
     * @param aId Profile id
     * @return Profile index or KErrNotFound if no matching profile is found.
     */
    virtual TUint32 ProfileIdByIndex( TUint aIndex ) = 0;
    };

#endif // M_MMUSSIPPROFILEHANDLER_H
