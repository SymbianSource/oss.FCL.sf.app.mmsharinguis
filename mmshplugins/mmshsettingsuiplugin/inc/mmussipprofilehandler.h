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
*  Version     : %version: 6 % << Don't touch! Updated by Synergy at check-out.
*
*/


#ifndef M_MMUSSIPPROFILEHANDLER_H
#define M_MMUSSIPPROFILEHANDLER_H

#include <e32def.h>
#include <e32cmn.h>

class CSIPProfile;

/**
*  MMusSIPProfileHandler class
*  Mixin class for SIP profile manipulation.
*  Handles SIP profile manipulations.
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
     * Returns a reference to the SIP profile array.
     * Note that previously returned value goes out of scope after a new
     * call to ProfileArrayL. So it is not advised to store return value
     * in long persistent variable, but rather to be used like stack
     * variables for safety.
     *
     * @return Reference to internally cached SIP profile array.
     */
    virtual RPointerArray<CSIPProfile>& ProfileArrayL() = 0;

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
