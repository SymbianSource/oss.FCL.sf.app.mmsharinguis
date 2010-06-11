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


#ifndef MUSMANAGERSERVERSESSION_H
#define MUSMANAGERSERVERSESSION_H

#include "musmanager.h"
#include "musmonitoravailabilityobserver.h"
#include "musavailabilitypluginmanagerobserver.h"

class CMusManagerServer;
class MMusManagerServerSessionObserver;
class TRequestStatus;

/**
 *  This class contains
 *
 *  @since S60 v3.2
 */
class CMusManagerServerSession : public CSession2,
                                        MMusMonitorAvailabilityObserver

    {
public: // Constructors and destructor

    static CMusManagerServerSession* NewL(
        CMusManagerServer& aServer,
        MMusManagerServerSessionObserver& aObserver );

    static CMusManagerServerSession* NewLC(
        CMusManagerServer& aServer,
        MMusManagerServerSessionObserver& aObserver );

    ~CMusManagerServerSession ();
    
private:

	TInt MonitorAvailabilityL();
public: // From CSession2

    void ServiceL( const RMessage2& aMessage );

public: // from MMusMonitorAvailabilityObserver

    virtual void AvailabilityChangedL(
    					MultimediaSharing::TMusAvailabilityStatus aAvailability );
    					
   	virtual void RequestComplete();

private: // Second phase constructors

    CMusManagerServerSession(
        CMusManagerServer& aServer,
        MMusManagerServerSessionObserver& aObserver );

    void ConstructL();

public: // Helper functions



private: // Data

    CMusManagerServer& iServer;

    MMusManagerServerSessionObserver& iServerCore;

    RMessage2 iMessage;
    
    RArray<MultimediaSharing::TMusAvailabilityStatus> iAvailabilityStatus;

    MultimediaSharing::TMusAvailabilityStatus iLastReport;
    
    TInt iSessionCounter;

private: // For testing purposes
    };

#endif // ? MUSMANAGERSERVERSESSION_H
