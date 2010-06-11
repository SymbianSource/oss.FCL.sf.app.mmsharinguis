/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSAVAILABILITYOBSERVER_H
#define MUSAVAILABILITYOBSERVER_H

//  INCLUDES
#include <e32base.h>
#include <musmanagercommon.h>

// CLASS DECLARATION

/**
 * This observer class should be implemented by components interested in the 
 * Multimedia Sharing availability changes
 */

class MMusAvailabilityObserver
    {
public: // New functions
    
    /**
     * Method called when multimedia sharing availability changes
     *
     * @param aAvailabilityStatus state of multimedia sharing availability
     */
    virtual void MusAvailabilityChangedL( 
                MultimediaSharing::TMusAvailabilityStatus aAvailabilityStatus ) = 0;
    
    };

#endif // MUSAVAILABILITYOBSERVER_H

// End of File