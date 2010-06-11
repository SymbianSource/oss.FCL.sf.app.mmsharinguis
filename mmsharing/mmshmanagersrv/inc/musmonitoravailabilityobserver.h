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


#ifndef MUSMONITORAVAILABILITYOBSERVER_H
#define MUSMONITORAVAILABILITYOBSERVER_H

#include "musmanagercommon.h"

#include <e32base.h>

class TRequestStatus;

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 v3.2
 */
class MMusMonitorAvailabilityObserver 
    {
public:
    virtual void AvailabilityChangedL(
    					MultimediaSharing::TMusAvailabilityStatus aAvailability ) = 0;
    					
   	virtual void RequestComplete() = 0;
    					
    };


#endif // ? MUSMONITORAVAILABILITYOBSERVER_H
