/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

//  INCLUDES
#include <e32base.h>
#include <centralrepository.h>
#include <hwrmlightdomaincrkeys.h>
#include <QtCore>
#include "lcactivitymanager.h"
#include "lclogger.h"


const int lcDefaultInactivityTimeout = 5; //timeout in seconds, used if fetching 
                                          //value from cenrep fails
const int lcInvalidTimerId = -1;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LcActivityManager::LcActivityManager() : 
    mTimerId(lcInvalidTimerId)
{
    if (initInactivityTimeout() != 0) {
        mInactivityTimeout = lcDefaultInactivityTimeout;
    }    
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LcActivityManager::~LcActivityManager()
{
    stopTimer();
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcActivityManager::enableActivitySimulation(bool enable)
{
    if (enable) {
        restartTimer();
    } else {
        stopTimer();
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
bool LcActivityManager::isActivitySimulationEnabled()
{
    return (mTimerId != lcInvalidTimerId);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
int LcActivityManager::initInactivityTimeout()
{
    CRepository* repository = 0;
    TRAPD(err, repository = CRepository::NewL(KCRUidLightSettings));
    if (err == KErrNone) {
        // Let's fetching display light timeout value (in seconds )
        TInt displayTimeOut(0);
        err = repository->Get(KDisplayLightsTimeout, displayTimeOut);
        if (displayTimeOut > 2) {
            //taking away 2 seconds just in case, to ensure we keep
            //simulating user activity frequently enough
            displayTimeOut -= 2;
        }
        mInactivityTimeout = displayTimeOut;
    }
    delete repository;
    return err;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcActivityManager::stopTimer()
{
    if (mTimerId != lcInvalidTimerId) {
        killTimer(mTimerId);
        mTimerId = lcInvalidTimerId;
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcActivityManager::restartTimer()
{
    stopTimer();

    int timeLeftInSeconds = mInactivityTimeout - User::InactivityTime().Int();
    if ( timeLeftInSeconds < 1 ) {
        User::ResetInactivityTime();
        timeLeftInSeconds = mInactivityTimeout;
    }
    
    LC_QDEBUG_2( "livecomms [UI] -> LcActivityManager::startTimer(), interval: ", timeLeftInSeconds);
    
    mTimerId = startTimer(timeLeftInSeconds*1000);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcActivityManager::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == mTimerId) {
        LC_QDEBUG( "livecomms [UI] -> LcActivityManager::timerEvent()" );
        restartTimer(); 
    }
}
