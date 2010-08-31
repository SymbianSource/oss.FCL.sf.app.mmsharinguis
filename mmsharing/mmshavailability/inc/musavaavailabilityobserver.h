/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This observer is used by
*                availabilities to observer function from the availability
*
*/



#ifndef MMUSAVAAVAILABILITYOBSERVER
#define MMUSAVAAVAILABILITYOBSERVER

#include <e32def.h>

#include "musavaobserver.h"

/**
 *  MusAvailability Observer base class
 *
 *  Observer interface for ECOM clients.
 *
 *  @lib musavailabilityplugin.lib
 */

class MMusAvaAvailabilityObserver
    {
    
public:

    /**
     * Availability report
     *
     * @param aStatus New status for availability.
     */
    virtual void AvailabilityChanged(
        MMusAvaObserver::TAvailabilityName aName,
        MMusAvaObserver::TAvailabilityStatus aStatus ) = 0;

    virtual void AvailabilityError(
        MMusAvaObserver::TAvailabilityName aName,
        MMusAvaObserver::TAvailabilityStatus aStatus ) = 0;
        
    virtual TBool Available( 
        MMusAvaObserver::TAvailabilityName aAvailability ) = 0;
        
        
    virtual MMusAvaObserver::TAvailabilityStatus 
    	AvailabilityPluginState() = 0;
   
    };


#endif //MMUSAVAAVAILABILITYOBSERVER
