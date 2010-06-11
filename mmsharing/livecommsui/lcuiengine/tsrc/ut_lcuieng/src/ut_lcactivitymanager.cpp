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
#include "ut_lcactivitymanager.h"
#include "lcactivitymanager.h"
#include "centralrepository_stub_helper.h"
#include <QtTest/QtTest>

const int utDisplayTimeout = 10;

//////////////////////////////////////////////////////////////////////////////
// Stub of User::InactivityTime() and User::ResetInactivityTime()
TTimeIntervalSeconds utInactivityInterval = 0;

TTimeIntervalSeconds User::InactivityTime() 
{
    return utInactivityInterval;
}
    
void User::ResetInactivityTime()
{
    utInactivityInterval = 0;
}
//////////////////////////////////////////////////////////////////////////////

    
    
void UT_LcActivityManager::initTestCase()
{
}

void UT_LcActivityManager::cleanupTestCase()
{
}


void UT_LcActivityManager::init()
{
    CentralRepositoryStubHelper::reset();
    CentralRepositoryStubHelper::setCurrentVal(utDisplayTimeout);
    mActivityManager = new LcActivityManager;   
}

void UT_LcActivityManager::cleanup()
{
    delete mActivityManager;
}

void UT_LcActivityManager::testConstructor()
{
    // Inactivity timeout initialized with value from cenrep
    QVERIFY( mActivityManager );
    QVERIFY( mActivityManager->mTimerId == -1 );
    QVERIFY( mActivityManager->mInactivityTimeout == utDisplayTimeout - 2 );
    
    // Fetching value from cenrep failed, timeout initialized with default value
    delete mActivityManager;
    mActivityManager = 0;
    CentralRepositoryStubHelper::setFailCode(-1);
    mActivityManager = new LcActivityManager;
    QVERIFY( mActivityManager->mTimerId == -1 );
    QVERIFY( mActivityManager->mInactivityTimeout == 5 ); //see lcDefaultInactivityTimeout
    QVERIFY( mActivityManager->mInactivityTimeout != utDisplayTimeout - 2 );
}

void UT_LcActivityManager::testInitInactivityTimeout()
{
    // Creation of cenrep failed, timeout value not changed
    QVERIFY( mActivityManager->mTimerId == -1 );
    QVERIFY( mActivityManager->mInactivityTimeout == utDisplayTimeout - 2 );
    CentralRepositoryStubHelper::setFailCode(KErrNoMemory);
    CentralRepositoryStubHelper::setCurrentVal(2);
    QVERIFY( mActivityManager->initInactivityTimeout() == KErrNoMemory );
    QVERIFY( mActivityManager->mTimerId == -1 );
    QVERIFY( mActivityManager->mInactivityTimeout 
             != CentralRepositoryStubHelper::currentVal() - 2 );
    QVERIFY( mActivityManager->mInactivityTimeout == utDisplayTimeout - 2 );
    
    // Fetching value from cenrep failed
    CentralRepositoryStubHelper::setFailCode(KErrNotFound);
    CentralRepositoryStubHelper::setCurrentVal(3);
    QVERIFY( mActivityManager->initInactivityTimeout() == KErrNotFound );
    QVERIFY( mActivityManager->mTimerId == -1 );
    QVERIFY( mActivityManager->mInactivityTimeout 
             == CentralRepositoryStubHelper::currentVal() - 2 );
    
    // Timeout initialized with cenrep value
    CentralRepositoryStubHelper::reset();
    CentralRepositoryStubHelper::setCurrentVal(2);
    QVERIFY( mActivityManager->initInactivityTimeout() == KErrNone );
    QVERIFY( mActivityManager->mTimerId == -1 );
    QVERIFY( mActivityManager->mInactivityTimeout 
             == CentralRepositoryStubHelper::currentVal() );
}

void UT_LcActivityManager::testEnableActivitySimulation()
{
    // Starting activity simulation for the 1st time
    QVERIFY( mActivityManager->mTimerId == -1 );
    mActivityManager->mInactivityTimeout = 5;
    utInactivityInterval = 1;
    QVERIFY( User::InactivityTime().Int() == 1 );
    mActivityManager->enableActivitySimulation(true);
    QVERIFY( mActivityManager->mTimerId > 0 );
    QVERIFY( User::InactivityTime().Int() == 1 );
    
    // Starting activity simulation again, user inactivity reached threshold
    int prevTimerID = mActivityManager->mTimerId;
    utInactivityInterval = mActivityManager->mInactivityTimeout = 5;
    QVERIFY( User::InactivityTime().Int() == 5 );
    mActivityManager->enableActivitySimulation(true);
    QVERIFY( mActivityManager->mTimerId > 0 );
    QVERIFY( mActivityManager->mTimerId != prevTimerID );
    QVERIFY( User::InactivityTime().Int() == 0 );

    // Disabling ongoing activity simulation
    QVERIFY( mActivityManager->mTimerId > 0 );
    mActivityManager->enableActivitySimulation(false);
    QVERIFY( mActivityManager->mTimerId == -1 );

    // Disabling when no activity simulation running
    QVERIFY( mActivityManager->mTimerId == -1 );
    mActivityManager->enableActivitySimulation(false);
    QVERIFY( mActivityManager->mTimerId == -1 );    
}

void UT_LcActivityManager::testIsActivitySimulationEnabled()
{
    QVERIFY( mActivityManager->mTimerId == -1 );
    QVERIFY( !mActivityManager->isActivitySimulationEnabled() );
    
    mActivityManager->mTimerId = 1;
    QVERIFY( mActivityManager->isActivitySimulationEnabled() );
}

void UT_LcActivityManager::testTimerEvent()
{
    // Time event for some other timer, nothing to do
    mActivityManager->mTimerId = 10;
    QTimerEvent dummyTimerEvent(5);
    mActivityManager->timerEvent(&dummyTimerEvent);
    QVERIFY( mActivityManager->mTimerId == 10 );
    
    // Time event for inactivity timer, timer is restarted
    QTimerEvent inactivityTimerEvent(mActivityManager->mTimerId);
    mActivityManager->timerEvent(&inactivityTimerEvent);
    QVERIFY( mActivityManager->mTimerId != 10 );
}
