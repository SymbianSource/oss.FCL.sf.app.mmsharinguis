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


#ifndef MUSMANAGERSERVERSESSIONOBSERVER_H
#define MUSMANAGERSERVERSESSIONOBSERVER_H

#include <e32base.h>

#include "musmanagercommon.h"

class MMusMonitorAvailabilityObserver;

/**
 *  This abstract class contains the essential functions 
 *  executable by manager clients
 *
 *  @since S60 v3.2
 */
class MMusManagerServerSessionObserver
	{
public: // new functions

    virtual MultimediaSharing::TMusAvailabilityStatus AvailabilityQueryL() = 0;
    
    virtual void CancelMonitoring( MMusMonitorAvailabilityObserver* aObserver) = 0;
    
    virtual void CommandL( MultimediaSharing::TCommandType aCommandType ) = 0;
    
    virtual void InvestigateAvailabilityL() = 0;
    
    virtual void InvitationReceivedL( TUid aChannelId ) = 0;
    
    virtual void OptionsReceivedL( TUid aChannelId ) = 0;
    
    virtual void RegisterObserverL( MMusMonitorAvailabilityObserver* aObserver ) = 0;
    
    virtual void RemoveObserver( MMusMonitorAvailabilityObserver* aObserver ) = 0;
    
    virtual void StartMultimediaSharingL( MultimediaSharing::TMusUseCase aUseCase ) = 0;
    
    virtual void StopMultimediaSharingL() = 0;

	};

#endif // MUSMANAGERSERVERSESSIONOBSERVER_H
