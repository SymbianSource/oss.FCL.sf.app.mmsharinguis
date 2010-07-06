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

#include "ut_lcuicomponentrepository.h"
#include "ut_lcuidefs.h"
#include "lcuicomponentrepository.h"
#include "lcuidefs.h"
#include "lcview.h"
#include "lcuiengine.h"
#include <QtTest/QtTest>
#include <QObjectList>
#include <hbmainwindow.h>
#include <QAction>
#include <QString>
#include <hbmenu.h>
#include <hbtoolbar.h>
#include <hbdialog.h>
#include <hbaction.h>
#include <hbinstance.h>
#include <hbprogressdialog.h>

#define UT_SET_ORIENTATION(orient) \
HbInstance::instance()->allMainWindows().at(0)->setOrientation(orient);

void UT_LcUiComponentRepository::initTestCase()
{
}

void UT_LcUiComponentRepository::cleanupTestCase()
{
}


void UT_LcUiComponentRepository::init()
{
    mEngine = new LcUiEngine( lcEngineName, lcDisplayName );
    mRepository = new LcUiComponentRepository( *mEngine );
}

void UT_LcUiComponentRepository::cleanup()
{
    delete mRepository;
    delete mEngine;
}

void UT_LcUiComponentRepository::testConstructor()
{
    QVERIFY( mRepository );
    QVERIFY( !mRepository->mIdleView );
    QVERIFY( !mRepository->mReceiveView );
    QVERIFY( !mRepository->mReceiveOnlyView );
    QVERIFY( !mRepository->mTwowayView );
    QVERIFY( !mRepository->mSendView );
    QVERIFY( !mRepository->mAllInOneView );
    QVERIFY( !mRepository->mAcceptQuery );
    QVERIFY( !mRepository->mInvitingNote );
    QVERIFY( !mRepository->mWaitingNote );
    QVERIFY( !mRepository->mRecipientQuery );
    
}

void UT_LcUiComponentRepository::testIdleView()
{
    // View not yet loaded
    UT_SET_ORIENTATION( Qt::Horizontal );
    QVERIFY( !mRepository->mIdleView );
    LcView* view = mRepository->idleView();
    QVERIFY( mRepository->mIdleView );
    QCOMPARE( view->objectName(), QString( lcViewIdleId ) );
            
    // View loaded
    LcView* view2 = mRepository->idleView();
    QVERIFY( view == view2 );
}

void UT_LcUiComponentRepository::testReceiveView()
{
    // View not yet loaded
    // Landscape layout
    UT_SET_ORIENTATION( Qt::Horizontal );
    QVERIFY( !mRepository->mReceiveView );
    LcView* view = mRepository->receiveView();
    QVERIFY( mRepository->mReceiveView );
    QCOMPARE( view->objectName(), QString( lcViewReceiveId ) );
    
    // View loaded
    LcView* view2 = mRepository->receiveView();
    QVERIFY( view == view2 );
}


void UT_LcUiComponentRepository::testReceiveOnlyView()
{
    // View not yet loaded
    QVERIFY( !mRepository->mReceiveOnlyView );
    LcView* view = mRepository->receiveOnlyView();
    QVERIFY( mRepository->mReceiveOnlyView );
    QCOMPARE( view->objectName(), QString( lcViewReceiveOnlyId ) );
    
    // View loaded
    LcView* view2 = mRepository->receiveOnlyView();
    QVERIFY( view == view2 ); 
}


void UT_LcUiComponentRepository::testTwowayView()
{
    // View not yet loaded
    QVERIFY( !mRepository->mTwowayView );
    LcView* view = mRepository->twowayView();
    QVERIFY( mRepository->mTwowayView );
    QCOMPARE( view->objectName(), QString( lcViewTwowayId ) );
    
    // View loaded
    LcView* view2 = mRepository->twowayView();
    QVERIFY( view == view2 ); 
}

void UT_LcUiComponentRepository::testSendView()
{ 
    // View not yet loaded
    QVERIFY( !mRepository->mSendView );
    LcView* view = mRepository->sendView();
    QVERIFY( mRepository->mSendView );
    QCOMPARE( view->objectName(), QString( lcViewSendId ) );
    
    // View loaded
    LcView* view2 = mRepository->sendView();
    QVERIFY( view == view2 );
}

void UT_LcUiComponentRepository::testAllInOneView()
{  
    // View not yet loaded
    UT_SET_ORIENTATION( Qt::Horizontal );
    QVERIFY( !mRepository->mAllInOneView );    
    LcView* view = mRepository->allInOneView();
    QVERIFY( mRepository->mAllInOneView );
    QCOMPARE( view->objectName(), QString( lcViewAllInOneId ) );    
    QCOMPARE(  mRepository->mLayoutSection, QString( lcLayoutLandscapeDefaultId ) );
    // View loaded
    LcView* view2 = mRepository->allInOneView();
    QVERIFY( view == view2 );
}

void UT_LcUiComponentRepository::testAcceptQuery()
{
    // Query created and returned
    QVERIFY( !mRepository->mAcceptQuery );
    HbDialog* query = mRepository->acceptQuery();
    QVERIFY( query );
    QCOMPARE( query, mRepository->mAcceptQuery );
    QVERIFY( !query->isVisible() );

    // Existing query returned
    HbDialog* query2 = mRepository->acceptQuery();
    QVERIFY( query2 );
    QVERIFY( query2 == query ); 
}

void UT_LcUiComponentRepository::testInvitingNote()
{
    // Note created and returned
    QVERIFY( !mRepository->mInvitingNote );
    HbDialog* note = mRepository->invitingNote();
    QVERIFY( note );
    QVERIFY( note == mRepository->mInvitingNote );
    QVERIFY( !note->isVisible() );

    // Existing note returned
    HbDialog* note2 = mRepository->invitingNote();
    QVERIFY( note2 );
    QVERIFY( note2 == note );  
}

void UT_LcUiComponentRepository::testWaitingNote()
{
    // Note created and returned
    QVERIFY( !mRepository->mWaitingNote );
    HbDialog* note = mRepository->waitingNote();
    QVERIFY( note );
    QVERIFY( note == mRepository->mWaitingNote );
    QVERIFY( !note->isVisible() );

    // Existing note returned
    HbDialog* note2 = mRepository->waitingNote();
    QVERIFY( note2 );
    QVERIFY( note2 == note );
}

void UT_LcUiComponentRepository::testRecipientQuery()
{
    // Query created and returned
    QVERIFY( !mRepository->mRecipientQuery );
    HbDialog* query = mRepository->recipientQuery();
    QVERIFY( mRepository->mRecipientQuery );
    QCOMPARE( query, mRepository->mRecipientQuery );
    QVERIFY( !query->isVisible() );

    // Existing query returned
    HbDialog* query2 = mRepository->recipientQuery();
    QVERIFY( query2 == query );
}

void UT_LcUiComponentRepository::testShareOwnVideoQuery()
{
    // Query created and returned
    QVERIFY( !mRepository->mShareOwnVideoQuery );
    HbDialog* query = mRepository->shareOwnVideoQuery();
    QVERIFY( mRepository->mShareOwnVideoQuery );
    QCOMPARE( query, (HbDialog*)mRepository->mShareOwnVideoQuery );
    QVERIFY( !query->isVisible() );

    // Existing query returned
    HbDialog* query2 = mRepository->shareOwnVideoQuery();
    QVERIFY( query2 );
    QVERIFY( query2 == query ); 
}

void UT_LcUiComponentRepository::testSharedVideoContextMenu()
{
    HbMenu* menu = new HbMenu();
    QVERIFY( menu->isEmpty() );

    QVERIFY( !mRepository->mAllInOneView );    
    LcView* view = mRepository->allInOneView();
    QVERIFY( mRepository->mAllInOneView );
    QCOMPARE( view->objectName(), QString( lcViewAllInOneId ) );

    HbAction changeCamera( lcActChangeCameraId );
    HbAction disableCam( lcActMenuDisableCameraId );
    HbAction enableCam( lcActEnableCameraId );
    HbAction changeCam( lcActMenuChangeCameraId );

    mRepository->mActions.append( &changeCamera );
    mRepository->mActions.append( &disableCam );
    mRepository->mActions.append( &enableCam );
    mRepository->mActions.append( &changeCam );
    
    mRepository->sharedVideoContextMenuActions( menu, *view );
    //TODO: stub implementation of void QGraphicsWidget::addAction(QAction *action);
    //QVERIFY( !menu->isEmpty() );    
}


void UT_LcUiComponentRepository::testLoadView_FileNotFound()
{
    QObjectList viewR;
    const QString vid( "view");
    const QString file( "file not found" );
    QVERIFY( !mRepository->loadView( viewR, vid, file ) );
}

void UT_LcUiComponentRepository::testConnect()
{
    LcView* view = mRepository->idleView();
    QVERIFY( view );
    HbAction action;
    
    // Existing action
    action.setObjectName( QString( lcActMuteId ) );
    QVERIFY( mRepository->connect( action, *view ) );
    
    // Action not found
    action.setObjectName( QString( "Action not defined" ) );
    QVERIFY( !mRepository->connect( action, *view ) );    
}


void UT_LcUiComponentRepository::testLoadLayout()
{   
    bool ok( false );
    
    // View not yet loaded
    QVERIFY( mRepository->mLastLoadedView.length() == 0 );
    ok = mRepository->loadLayout( lcLayoutLandscapeDefaultId );
    QVERIFY( !ok );
    
    // View is loaded
    mRepository->mLastLoadedView = lcIdleViewFile;
    ok = mRepository->loadLayout( lcLayoutLandscapeDefaultId );
    QVERIFY( ok );
    
    // Changing layout
    ok = mRepository->loadLayout( lcLayoutLandscapeSwappedId );
    QVERIFY( ok );
    ok = mRepository->loadLayout( lcLayoutFullscreenId );
    QVERIFY( ok );
    
    // Loading non-existing section
    ok = mRepository->loadLayout( QString("dummy_layout") );
    QVERIFY( !ok );
    
    // Check for current and previously loaded layout name    
    mRepository->mLastLoadedView = lcIdleViewFile;
    ok = mRepository->loadLayout( lcLayoutLandscapeDefaultId );
    QVERIFY( ok );
    QVERIFY( mRepository->layout() == lcLayoutLandscapeDefaultId );    
    ok = mRepository->loadLayout( lcLayoutLandscapeDialpadId );
    QVERIFY( ok );
    QVERIFY( mRepository->layout() == lcLayoutLandscapeDialpadId );
    QVERIFY( mRepository->previousLayout() == lcLayoutLandscapeDefaultId );
    ok = mRepository->loadLayout( mRepository->previousLayout() );
    QVERIFY( ok );
    QVERIFY( mRepository->layout() == lcLayoutLandscapeDefaultId );
    QVERIFY( mRepository->previousLayout() == lcLayoutLandscapeDialpadId );
        
}
