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


#ifndef MUSMANAGERSERVERCORE_H
#define MUSMANAGERSERVERCORE_H

#include <e32base.h>

#include "musmanagercommon.h"
#include "musmanagerserversessionobserver.h"
#include "musavailabilitypluginmanagerobserver.h"
#include "musmanagercommon.h"
#include "musapplicationmanager.h"

class CMusApplicationManager;
class CMusAvailabilityPlugin;
class CMusAvailabilityPluginManager;
class CMusManagerServerCloseTimer;
class MMusManagerServerCoreObserver;


/**
 *  This static class contains the very essential starting functions
 *  of MusManagerServer
 *
 *  @since S60 v3.2
 */
class CMusManagerServerCore
    : public CBase,
    public MMusManagerServerSessionObserver,
    public MMusAvailabilityPluginManagerObserver
    {
    MUS_UNITTEST( UT_CMusManagerServerCore )

public: // first phase constructors & destructor

    static CMusManagerServerCore* NewL( MMusManagerServerCoreObserver& aObserver );
    static CMusManagerServerCore* NewLC( MMusManagerServerCoreObserver& aObserver );
    ~CMusManagerServerCore();

protected: // second phase constructors

    void ConstructL();

    CMusManagerServerCore( MMusManagerServerCoreObserver& aObserver );

public: // from MMusManagerServerSessionObserver

    // functionality

    MultimediaSharing::TMusAvailabilityStatus AvailabilityQueryL();
    
    void CancelMonitoring( MMusMonitorAvailabilityObserver* aObserver );
    
    void CommandL( MultimediaSharing::TCommandType aCommandType );

    void InvestigateAvailabilityL();

    void InvitationReceivedL( TUid aChannelId );
    
    void OptionsReceivedL( TUid aChannelId );
    
    void RegisterObserverL( MMusMonitorAvailabilityObserver* aObserver );
    
    void RemoveObserver( MMusMonitorAvailabilityObserver* aObserver );
    
    void StartMultimediaSharingL( MultimediaSharing::TMusUseCase aUseCase );

    void StopMultimediaSharingL();
    
private:

	void StopMonitoring();
	
public: // from MMusManagerServerObserver

    void EventNoSessions();

public: // from MMusAvailabilityPluginManagerObserver

    virtual void PluginStopped();

    // TODO: capability -> availability

    virtual void AvailabilityChangedL( MultimediaSharing::TMusAvailabilityStatus aAvailability );

    virtual void StartSharingWithUseCaseL( MultimediaSharing::TMusUseCase aUseCase );
    
private: // private methods
    
    TBool IsMusEnginePlugin();
    
    MultimediaSharing::TMusAvailabilityStatus Availability();
    
    TBool ApplicationAllowed();
    
    void PrepareForReceivedInviteL();
    
    void WriteSessionPropertiesL(
            MultimediaSharing::TMusUseCase aUseCase,
            MultimediaSharing::TMusAvailabilityStatus aStatus,
            MDesCArray& aSessionParameters );
    
    void WriteSessionPropertiesL(
            MultimediaSharing::TMusAvailabilityStatus aStatus,
            MDesCArray& aSessionParameters );

private: // owned data:

    CMusAvailabilityPluginManager* iPluginManager;

    CMusApplicationManager* iApplicationManager;
    
    TBuf8<KMusMgrMaxPluginNameLen> iPluginName;

private: // not owned data

    MMusManagerServerCoreObserver& iObserver;

    RPointerArray<MMusMonitorAvailabilityObserver> iAvailabilityMonitors;
    
    RArray<MultimediaSharing::TMusAvailabilityStatus> iAvailabilityStatus;
    };

#endif // MUSMANAGERSERVERCORE_H
