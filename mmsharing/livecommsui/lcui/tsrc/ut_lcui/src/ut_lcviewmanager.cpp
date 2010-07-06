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

#include <QtTest/QtTest>
#include <hbmainwindow.h>
#include <hbview.h>
#include <hbactivitymanager.h>
#include <HbApplication>
#include "ut_lcviewmanager.h"         
#include "lcviewmanager.h" 
#include "lcviewmanager_p.h" 
#include "ut_lcuidefs.h"
#include "lcuiengine.h"
#include "lcuidefs.h"

// Simulate completion of queued view change
#define LC_VIEW_CHANGE_COMPLETE( startup ) \
mViewManager->activateCurrentView( startup );

void UT_LcUiViewManager::initTestCase()
{
    lcutStub_LcUiEngine_reset();

}

void UT_LcUiViewManager::cleanupTestCase()
{
    lcutStub_LcUiEngine_reset();

}

void UT_LcUiViewManager::init()
{
    lcutStub_LcUiEngine_reset();
    
    mViewManagerDecorator = new LcViewManager( lcEngineName, lcDisplayName ); 
    mViewManager = mViewManagerDecorator->d_ptr; 
}

void UT_LcUiViewManager::cleanup()
{
    lcutStub_LcUiEngine_reset();
    delete mViewManagerDecorator;
}

void UT_LcUiViewManager::testConstructor()
{
    QVERIFY( mViewManager );
    QVERIFY( mViewManager->mRepository );
    QVERIFY( mViewManager->mEngine );
    QVERIFY( mViewManager->mMainWindow.viewCount() == 1 );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutNoCall ) );
    
}

void UT_LcUiViewManager::testChangeView()
{
    QVERIFY( mViewManager );
    QVERIFY( mViewManager->mMainWindow.viewCount() == 1 );
    
    mViewManager->changeView();
    LC_VIEW_CHANGE_COMPLETE( false )
    QVERIFY( mViewManager->mMainWindow.viewCount() == 1 );
    QVERIFY( mViewManager->mMainWindow.currentView()->objectName() == lcViewIdleId );
    
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    lcutStub_LcUiEngine_reset();
    
    LC_VIEW_CHANGE_COMPLETE( true )
    QVERIFY( mViewManager->mMainWindow.viewCount() == 1 );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_start ) );
    lcutStub_LcUiEngine_reset();

    lcutStub_LcUiEngine_setLayout( Lc_ReceivedViewLayout );
    mViewManager->changeView();
    LC_VIEW_CHANGE_COMPLETE( false )
    QVERIFY( mViewManager->mMainWindow.viewCount() == 2 );
    QVERIFY( mViewManager->mMainWindow.currentView()->objectName() == lcViewReceiveId );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    lcutStub_LcUiEngine_reset();
    
    lcutStub_LcUiEngine_setLayout( Lc_SharedViewLayout );
    mViewManager->changeView();
    LC_VIEW_CHANGE_COMPLETE( false )
    QVERIFY( mViewManager->mMainWindow.viewCount() == 3 );
    QVERIFY( mViewManager->mMainWindow.currentView()->objectName() == lcViewSendId );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    lcutStub_LcUiEngine_reset();

    lcutStub_LcUiEngine_setLayout( Lc_TwowayViewLayout );
    mViewManager->changeView();
    QVERIFY( mViewManager->mMainWindow.viewCount() == 4 );
    QVERIFY( mViewManager->mMainWindow.currentView()->objectName() == lcViewTwowayId );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas ) );
    lcutStub_LcUiEngine_reset();

    lcutStub_LcUiEngine_setLayout( Lc_ReceivedOnlyViewLayout );
    mViewManager->changeView();
    QVERIFY( mViewManager->mMainWindow.viewCount() == 5 );
    QVERIFY( mViewManager->mMainWindow.currentView()->objectName() == lcViewReceiveOnlyId );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas ) );
    lcutStub_LcUiEngine_reset();
    
    lcutStub_LcUiEngine_setLayout( Lc_AllInOneViewLayout );
    mViewManager->changeView();
    LC_VIEW_CHANGE_COMPLETE( false )
    QVERIFY( mViewManager->mMainWindow.viewCount() == 6 );
    QVERIFY( mViewManager->mMainWindow.currentView()->objectName() == lcViewAllInOneId );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    lcutStub_LcUiEngine_reset();
    
    lcutStub_LcUiEngine_setLayout( (LcViewLayoutHint)777 );
    mViewManager->changeView();
    LC_VIEW_CHANGE_COMPLETE( false )
    QVERIFY( mViewManager->mMainWindow.viewCount() == 6 );
    QVERIFY( mViewManager->mMainWindow.currentView()->objectName() == lcViewIdleId );    
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    lcutStub_LcUiEngine_reset();
    
}

void UT_LcUiViewManager::testTerminateSession()
{
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutNoCall ) );
    mViewManager->terminateSession();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_stop ) );
    
    
    lcutStub_LcUiEngine_reset();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutNoCall ) );
    mViewManagerDecorator->terminateSession();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_stop ) );
}

void UT_LcUiViewManager::testPrepareOrientationChange()
{
    mViewManager->prepareOrientationChange();
}

void UT_LcUiViewManager::testMainWindowEvent()
{
     QSignalSpy spy( &mViewManager->mMainWindow, SIGNAL(appFocusGained()) );
     // Not interesting event
     QEvent event(QEvent::MaxUser);
     mViewManager->mMainWindow.event(&event);
     QVERIFY( spy.count() == 0 );
     
     // Focus in event
     QEvent event2(QEvent::FocusIn);
     mViewManager->mMainWindow.event(&event2);
     QVERIFY( spy.count() == 1 );
     
     QSignalSpy spy2( &mViewManager->mMainWindow, SIGNAL(appFocusLost()) );
     // Focus in event
     QEvent event3(QEvent::FocusOut);
     mViewManager->mMainWindow.event(&event3);
     QVERIFY( spy2.count() == 1 );
}

// end of file
