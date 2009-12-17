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
* Description: 
*
*/



#ifndef MUSAVACALLEVENTMONITOR_H
#define MUSAVACALLEVENTMONITOR_H


#include "musunittesting.h"
#include <e32base.h>
#include <e32property.h>

class MMusCallMonitorObserver;

/**
 * this class is listening call monitor P&S keys
 *
 * @lib musaoplugin.dll
 */
class CMusAvaCallEventMonitor : public CActive
    {
public:

    /**
     * get call count
     *
     * @return call count
     */
    TInt CallStatusL();
    
    /**
     * get call Direction
     *
     * @return call direction
     */
    TInt CallDirectionL();
    
    /**
     * get call Privacy
     *
     * @return call privacy
     */
    TInt CallPrivacyL();


    void GetTelNumberL( TDes& aTelNumber );

	 /**
     * Start to monitoring call state events
     *
     */
    void StartMonitoringL();


    /**
     * Two-phased constructor
     *
     * @return New CMusAoPlugin instance
     */
    static CMusAvaCallEventMonitor* NewL( MMusCallMonitorObserver& aObserver );


    /**
     * C++ destructor.
     */
    virtual ~CMusAvaCallEventMonitor();

    // from etelmm.h::RMobilePhone::KMaxMobileTelNumberSize
    static const TInt KMusTelNumberMaxLength = 100;

protected:

// from base class CActive

    /**
     * From CActive.
     * Cancels an outstanding asynchronous request.
     */
    virtual void DoCancel();

    /**
     * From CActive.
     * Handles an active object's request completion event.
     */
    virtual void RunL();

    /**
     * From CActive.
     * Handles a leave error in RunL method.
     */
    virtual TInt RunError( TInt aError );

private:

    /**
     * C++ constructor.
     */
    CMusAvaCallEventMonitor( MMusCallMonitorObserver& aObserver );

    /**
     * Symbian second-phase constructor.
     */
    void ConstructL();


private: // data

    /**
     * properties for P&S
     */
    RProperty iPropertyEvent;

    /**
     * Reference to the observer object
     */
    MMusCallMonitorObserver& iObserver;
    };

#endif // MUSAVACALLEVENTMONITOR_H
