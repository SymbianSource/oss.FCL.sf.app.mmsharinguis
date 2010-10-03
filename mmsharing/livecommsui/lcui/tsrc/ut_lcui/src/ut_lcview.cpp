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


#include "ut_lcview.h"
#include "ut_lcuidefs.h"
#include "lcview.h"
#include "lcuiengine.h"         
#include "lcuicomponentrepository.h"
#include "lcuidefs.h"
#include "lcvideowidget.h"
#include "lceffecthandler.h"
#include <QtTest/QtTest>
#include <hbmainwindow.h>
#include <hblabel.h>
#include <hbmessagebox.h>
#include <hbaction.h>
#include <hbtoolbar.h>
#include <hbmenu.h>
#include <hbinstance.h>
#include <hblineedit.h>
#include <hbpushbutton.h>
#include <hbtransparentwindow.h>
#include <qgraphicssceneevent.h>
#include <qgraphicsitem.h>
#include <QGesture.h>
#include <dialpad.h>
#include <dialpadvtkeyhandler.h>
#include <HbTapGesture.h>
#include <hbvolumesliderpopup.h>

// Stub Helper
#include <hbstub_helper.h>

#define UT_SET_ORIENTATION(orient) \
HbInstance::instance()->allMainWindows().at(0)->setOrientation(orient);

void UT_LcView::initTestCase()
{
    lcutStub_LcUiEngine_reset();
    //must have main window, even if it not used
    mMainWindow = new HbMainWindow();
}

void UT_LcView::cleanupTestCase()
{
    lcutStub_LcUiEngine_reset();
    delete mMainWindow;
}


void UT_LcView::init()
{
    lcutStub_LcUiEngine_reset();
    mRecipient = new HbLabel();
    mEndCallButton = new HbPushButton();
    mEngine = new LcUiEngine( lcEngineName, lcDisplayName );
    mRepository = new LcUiComponentRepository( *mEngine );
    mView = new LcView( *mEngine, *mRepository );
    mView->mRecipient = mRecipient;
    mView->mEndCallButton = mEndCallButton;
    mView->mChangeCameraAction = new HbAction();
    mView->mMuteAction = new HbAction();
    mView->mSpeakerAction = new HbAction();
    mView->mDisableCameraAction = new HbAction();   
    mView->mDisableCameraMenuAction = new HbAction(); 
}

void UT_LcView::cleanup()
{
    lcutStub_LcUiEngine_reset();
    delete mRecipient;
    delete mView;
    delete mEngine;
    delete mRepository;    
    delete mEndCallButton;
}

void UT_LcView::testConstructor()
{
    QVERIFY( mView );
}

void UT_LcView::testActivated()
{
    mView->init();
    lcutStub_LcUiEngine_reset();
    QVERIFY( lcutStub_LcUiEngine_getCurrentView() == 0 );
    mView->activated();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_getCurrentView() == mView );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    
    lcutStub_LcUiEngine_reset();
    mView->activated();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    
    lcutStub_LcUiEngine_reset();
    lcutStub_LcUiEngine_setCameraMain( true);
    lcutStub_LcUiEngine_setMuted( true);
    lcutStub_LcUiEngine_setSpeaker( true );
    
    mView->activated();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    
    // normal scenario
    lcutStub_LcUiEngine_reset();
    mView->mChangeCameraAction = new HbAction();
    mView->mMuteAction = new HbAction();
    mView->mSpeakerAction = new HbAction();
    mView->mDisableCameraAction = new HbAction();   
     
    mView->mEngine.setMainCamera( true );
    lcutStub_LcUiEngine_setMuted( true);
    lcutStub_LcUiEngine_setSpeaker( true );
    lcutStub_LcUiEngine_setLocalPlaying( true );
    mView->activated();
    QVERIFY( mView->mChangeCameraAction );
    QVERIFY( mView->mChangeCameraAction->icon() ==  HbIcon( lcIconNameSecondaryCamera ) );
    QVERIFY( mView->mMuteAction );
    QVERIFY( mView->mMuteAction->icon() ==  HbIcon( lcIconNameUnmuteMic ) );
    QVERIFY( mView->mSpeakerAction );
    QVERIFY( mView->mSpeakerAction->icon() ==  HbIcon( lcIconNameHandset ) );
    QVERIFY( mView->mDisableCameraAction );
    QVERIFY( mView->mDisableCameraAction->icon() ==  HbIcon( lcIconNameDisableCamera ) );
    
    // dude trust me , ofcourse not everything normal everytime
    mView->mEngine.setMainCamera( false );    
    lcutStub_LcUiEngine_setMuted( false );
    lcutStub_LcUiEngine_setSpeaker( false );
    lcutStub_LcUiEngine_setLocalPlaying( false );
    mView->activated();
    QVERIFY( mView->mChangeCameraAction );
    QVERIFY( mView->mChangeCameraAction->icon() ==  HbIcon( lcIconNameMainCamera ) );
    QVERIFY( mView->mMuteAction );
    QVERIFY( mView->mMuteAction->icon() ==  HbIcon( lcIconNameMuteMic ) );
    QVERIFY( mView->mSpeakerAction );
    QVERIFY( mView->mSpeakerAction->icon() ==  HbIcon( lcIconNameLoudspeaker ) ); 
    QVERIFY( mView->mDisableCameraAction );
    QVERIFY( mView->mDisableCameraAction->icon() ==  HbIcon( lcIconNameEnableCamera ) );
}

void UT_LcView::testInit()
{
    mRepository->mReturnSendVideo = false;
    mView->init();
    QVERIFY( mView->mRecipient );
    QVERIFY( !mView->mSharedVideoWidget );
    QVERIFY( mView->mDialpad );
    QVERIFY( mView->mDialpadVtKeyHandler );
    
    mRepository->mReturnSendVideo = true;
    mView->init();
    QVERIFY( mView->mRecipient );
    QVERIFY( mView->mSharedVideoWidget );
    QVERIFY( mView->menu());
    QVERIFY( mView->mDialpad );
    QVERIFY( mView->mDialpadVtKeyHandler );
    
    // emergency call     
    mView->mDialpadVtKeyHandler->contentChanged("112");
    QVERIFY( mView->mDialpad->mIsCallButtonEnabled );
    mView->mDialpadVtKeyHandler->contentChanged("100");
    QVERIFY( !mView->mDialpad->mIsCallButtonEnabled );
}

void UT_LcView::testDeactivated()
{
    mView->deactivated();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas ) );
}

void UT_LcView::testSetCameraActionToMain()
{
    mView->setCameraActionToMain();
    QVERIFY( mView->mChangeCameraAction->text().isEmpty() );
    QCOMPARE( mView->mChangeCameraAction->icon(), HbIcon(lcIconNameMainCamera) );
}

void UT_LcView::testSetCameraActionToSecondary()
{
    mView->setCameraActionToSecondary();
    QVERIFY( mView->mChangeCameraAction->text().isEmpty() );
    QCOMPARE( mView->mChangeCameraAction->icon(), HbIcon(lcIconNameSecondaryCamera) );
}

void UT_LcView::testSetMuteActionToUnmute()
{
    mView->setMuteActionToUnmute();
    QVERIFY( mView->mMuteAction->text().isEmpty() );
    QCOMPARE (mView->mMuteAction->icon(), HbIcon(lcIconNameUnmuteMic));
}

void UT_LcView::testSetMuteActionToMute()
{
    mView->setMuteActionToMute();
    QVERIFY( mView->mMuteAction->text().isEmpty() );
    QCOMPARE (mView->mMuteAction->icon(), HbIcon(lcIconNameMuteMic));
}

void UT_LcView::testSetSpeakerActionToHandset()
{
    mView->setSpeakerActionToHandset();
    QVERIFY( mView->mSpeakerAction->text().isEmpty() );
    QCOMPARE( mView->mSpeakerAction->icon(), HbIcon(lcIconNameHandset) );
}

void UT_LcView::testSetSpeakerActionToSpeaker()
{
    mView->setSpeakerActionToSpeaker();
    QVERIFY( mView->mSpeakerAction->text().isEmpty() );
    QCOMPARE( mView->mSpeakerAction->icon(), HbIcon(lcIconNameLoudspeaker) );
}

void UT_LcView::testSetCameraActionToEnable()
{
    mView->setCameraActionToEnable();
    QVERIFY( mView->mDisableCameraAction->text().isEmpty() );
    QCOMPARE( mView->mDisableCameraAction->icon(), HbIcon(lcIconNameEnableCamera) );
    QCOMPARE( mView->mDisableCameraMenuAction->text(), hbTrId( "txt_vt_menu_enable_camera" ) );   
}

void UT_LcView::testSetCameraActionToDisable()
{    
    mView->setCameraActionToDisable();
    QVERIFY( mView->mDisableCameraAction->text().isEmpty() );
    QCOMPARE( mView->mDisableCameraAction->icon(), HbIcon(lcIconNameDisableCamera) );
    QCOMPARE( mView->mDisableCameraMenuAction->text(), hbTrId( "txt_vt_menu_disable_camera" ) );   
}


void UT_LcView::testDisableControls()
{
    mView->disableControls();
    QVERIFY( !mView->menu()->isEnabled() );
    QVERIFY( !mView->toolBar()->isEnabled() );
}

void UT_LcView::testEnableControls()
{
    mView->menu()->setEnabled( false );
    mView->toolBar()->setEnabled( false );
    
    mView->enableControls();
    QVERIFY( mView->menu()->isEnabled() );
    QVERIFY( mView->toolBar()->isEnabled() );
}


void UT_LcView::testEndVideoSession()
{
    mView->endVideoSession();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_stop ) );   
}

void UT_LcView::testMute()
{    
    mView->mute();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_mute ) );
    QVERIFY( !mView->timerId );
    QVERIFY( !mEngine->fullScreenMode());
}


void UT_LcView::testChangeCamera()
{
    mView->init();    
    mView->changeCamera();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_toggleCamera ) );    
    QVERIFY( !mEngine->fullScreenMode() );
}

void UT_LcView::testSwitchToVoiceCall()
{
    mView->switchToVoiceCall();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_stop ) );
}

void UT_LcView::testDisableCameraWhenOnFullScreenMode()
{
    mView->init();
    mEngine->setFullScreenMode(true);
    mView->disableCamera();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_disableCamera ) );    
    QVERIFY( mEngine->fullScreenMode() );
}

void UT_LcView::testDisableCameraWhenNotOnFullScreenMode()
{
    mView->init();
    mEngine->setFullScreenMode(false);
    mView->disableCamera();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_disableCamera ) );    
    QVERIFY( !mEngine->fullScreenMode() );
}

void UT_LcView::testSpeaker()
{   
    mView->init();   
    mView->speaker();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_speaker ) );    
    QVERIFY( !mEngine->fullScreenMode() );
}

void UT_LcView::testSwap()
{
    QVERIFY( !mView->mSharedVideoWidget );
    QVERIFY( !mView->mReceivedVideoWidget );
    mView->swap();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutNoCall ) );
    
    lcutStub_LcUiEngine_reset();
    mView->init();
    mView->swap();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutNoCall ) );
}

void UT_LcView::testNotSupported()
{
    HbMainWindow mainWin;
    QVERIFY( mView->mNotSupportedNote );
    QVERIFY( !mView->mNotSupportedNote->isVisible() );
    mView->notSupported();
    QVERIFY( mView->mNotSupportedNote->isVisible() );
}

void UT_LcView::testUpdateVideoRects()
{
    mView->init();
    LcVideoWidget* sharedVideoWidget = mView->mSharedVideoWidget;
    LcVideoWidget* receivedVideoWidget = mView->mReceivedVideoWidget;

    mView->mSharedVideoWidget = 0;
    mView->mReceivedVideoWidget = 0;
    mView->updateVideoRects();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );

    mView->mSharedVideoWidget = sharedVideoWidget;
    mView->mReceivedVideoWidget = 0;
    mView->updateVideoRects();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );

    mView->mSharedVideoWidget = 0;
    mView->mReceivedVideoWidget = receivedVideoWidget;
    mView->updateVideoRects();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );

    mView->mSharedVideoWidget = sharedVideoWidget;
    mView->mReceivedVideoWidget = receivedVideoWidget;
    sharedVideoWidget->hide();
    receivedVideoWidget->hide();
    lcutStub_LcUiEngine_setLocalPlaying( false );
    lcutStub_LcUiEngine_setRemotePlaying( false );
    mView->updateVideoRects();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    QVERIFY( !sharedVideoWidget->isVisible() );
    QVERIFY( !receivedVideoWidget->isVisible() );

    sharedVideoWidget->hide();
    receivedVideoWidget->hide();
    lcutStub_LcUiEngine_setLocalPlaying( true );
    lcutStub_LcUiEngine_setRemotePlaying( true );
    mView->updateVideoRects();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    
    //Test: Shared video widgets and received video wigts are positioned at (0,0)
    //It does not update.
    lcutStub_LcUiEngine_reset();
    mView->isViewReady = false;
    mView->mSharedVideoWidget = sharedVideoWidget;
    mView->mSharedVideoWidget->setPos(QPointF(0,0));
    mView->mReceivedVideoWidget = receivedVideoWidget;
    mView->mReceivedVideoWidget->setPos(QPointF(0,0));
    mView->updateVideoRects();
    QVERIFY( !lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( !lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( !lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    
    
    //Test2: Shared video widgets positioned at (0,0) and other widgets is null
    //It does not update.
    lcutStub_LcUiEngine_reset();
    mView->mSharedVideoWidget = sharedVideoWidget;
    mView->mSharedVideoWidget->setPos(QPointF(0,0));
    mView->mReceivedVideoWidget = 0;
    mView->isViewReady = false;
    mView->updateVideoRects();
    QVERIFY( !lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( !lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( !lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );

    //Test3: Shared video widgets positioned correctly  and other widgets is null
    //It update.
    lcutStub_LcUiEngine_reset();
    mView->mSharedVideoWidget = sharedVideoWidget;
    mView->mSharedVideoWidget->setPos(QPointF(10,10));
    mView->mReceivedVideoWidget = 0;
    mView->isViewReady = false;
    mView->updateVideoRects();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
}

void UT_LcView::testCurrentLayout()
{
    mView->init();
    QString layout;

    //1. Landscape layout
    UT_SET_ORIENTATION( Qt::Horizontal );
    layout = mView->currentLayout();
    QVERIFY( layout == lcLayoutLandscapeDefaultId );    

    //2. Landscape swapped layout    
    // Shared window x coordinate is higher than Received windows x coordinate.

    QRectF pos1 = QRectF(100,300, 400, 400);
    QRectF pos2 = QRectF(5,100, 200, 200);

    UT_SET_ORIENTATION( Qt::Horizontal );
    mView->mEffectHandler->mSwapInProgress = true;
    mView->mSharedVideoWidget->setGeometry(pos1);
    mView->mReceivedVideoWidget->setGeometry(pos2);

    layout = mView->currentLayout();
    QVERIFY( layout == lcLayoutLandscapeSwappedId );

    //3. If it's already swapped, change to the normal landscape mode
    mView->mSharedVideoWidget->setGeometry(pos2);
    mView->mReceivedVideoWidget->setGeometry(pos1);
    layout = mView->currentLayout();
    QVERIFY( layout == lcLayoutLandscapeDefaultId );
}


void UT_LcView::testToFullScreen()
{
    QString layout;
    UT_SET_ORIENTATION( Qt::Horizontal );

    // Test1: to full screen
    mView->init();
    mView->menu()->setVisible( false );
    mView->toFullScreen( true );
    QVERIFY( mEngine->fullScreenMode() );
    // Use the helper function do not invent new one 
    QVERIFY( !HbStubHelper::isTitleBarVisible() );
    QVERIFY( !HbStubHelper::isStatusBarVisible() );
    QVERIFY( !mView->toolBar()->isVisible() );    
    QVERIFY( !mView->mEndCallButton->isVisible() );
    QVERIFY( mView->mDuration->isVisible() );
    QVERIFY( mView->mRecipient->isVisible() );    
    
    // Test2 : not in full screen
    mView->init();
    mView->menu()->setVisible( false );
    mView->toFullScreen( false );
    QVERIFY( !mEngine->fullScreenMode() );
    QVERIFY( HbStubHelper::isTitleBarVisible() );
    QVERIFY( HbStubHelper::isStatusBarVisible() );
    QVERIFY( mView->toolBar()->isVisible() );    
    QVERIFY( mView->mEndCallButton->isVisible() );
    QVERIFY( mView->mDuration->isVisible() );
    QVERIFY( mView->mRecipient->isVisible() );
    int tId = mView->timerId;
    mView->watchInactivity();    
    QVERIFY( mView->timerId != tId );
    
    // Test3 : menu visible
    mView->init();
    mView->menu()->setVisible( false );
    mView->toFullScreen( false );
    mView->menu()->setVisible( true );
    mView->toFullScreen( true );
    QVERIFY( !mEngine->fullScreenMode() );
    QVERIFY( HbStubHelper::isTitleBarVisible() );
    QVERIFY( HbStubHelper::isStatusBarVisible() );
    QVERIFY( mView->toolBar()->isVisible() );    
    QVERIFY( mView->mEndCallButton->isVisible() );
    QVERIFY( mView->mDuration->isVisible() );
    QVERIFY( mView->mRecipient->isVisible() );    
    tId = mView->timerId;
    mView->watchInactivity();    
    QVERIFY( mView->timerId != tId );
    
    // Test3 : dialpad visible
    mView->init();
    delete mView->mDialpad;
    mView->mDialpad = new Dialpad();
    mView->toFullScreen( true );
    QVERIFY( !mEngine->fullScreenMode() );
    QVERIFY( HbStubHelper::isTitleBarVisible() );
    QVERIFY( HbStubHelper::isStatusBarVisible() );
    QVERIFY( mView->toolBar()->isVisible() );    
    QVERIFY( mView->mEndCallButton->isVisible() );
    QVERIFY( mView->mDuration->isVisible() );
    QVERIFY( mView->mRecipient->isVisible() );    
    tId = mView->timerId;
    mView->watchInactivity();    
    QVERIFY( mView->timerId == tId );
}

void UT_LcView::testGestureEvent()
{
    mView->init();
    mView->menu()->setVisible(false);    
    UT_SET_ORIENTATION( Qt::Horizontal );

    mView->mSharedVideoWidget->setGeometry(QRectF(5,50, 100, 100));
    mView->mReceivedVideoWidget->setGeometry(QRectF(5,200, 200, 400));
    QList<QGesture*> list;

    // Test1: Short tap Event toggle the full screen mode
    mEngine->setFullScreenMode(true);
    QPointF sharedVideoPos(10,60);
    QTapGesture *tapGesture = new QTapGesture();
    tapGesture->setPosition(sharedVideoPos);
    HbStubHelper::setGestureStyleHint(HbTapGesture::Tap);
    HbStubHelper::setGestureState(Qt::GestureFinished);
    list.append(tapGesture);
    QGestureEvent event1(list);
    mView->gestureEvent(&event1);
    QVERIFY( !mEngine->fullScreenMode());
    list.clear();
    
    // Test2: Long tap event does not affect full screen mode.
    QTapGesture *tapGesture2 = new QTapGesture();
    QPointF receivedVideoPos(10,210);
    tapGesture2->setPosition(receivedVideoPos);
    HbStubHelper::setGestureState(Qt::GestureUpdated);
    HbStubHelper::setGestureStyleHint(HbTapGesture::TapAndHold);
    list.append(tapGesture2);
    QGestureEvent event3(list);
    mEngine->setFullScreenMode(true);
    mView->gestureEvent(&event3);
    // Ensure it does not affect full screen mode.
    QVERIFY( mEngine->fullScreenMode());
    
    //Test3: Not Handled States.
    HbStubHelper::setGestureState(Qt::GestureStarted);
    mEngine->setFullScreenMode(true);
    mView->gestureEvent(&event3);
    // Ensure its not handled everything remains same
    QVERIFY( mEngine->fullScreenMode());
    list.clear();
    
    //Test4: Not Handled Event
    QSwipeGesture *swipeGesture = new QSwipeGesture();
    list.append(swipeGesture);
    QGestureEvent event4(list);
    mEngine->setFullScreenMode(true);
    mView->gestureEvent(&event4);
    // Ensure it does not affect full screen mode.
    QVERIFY( mEngine->fullScreenMode());
}

void UT_LcView::testContextMenu()
{
    QVERIFY( !mView->mItemContextMenu );    
    mView->createContextMenu();
    QVERIFY( mView->mItemContextMenu );
    QVERIFY( mView->mItemContextMenu->isEmpty() );
}

void UT_LcView::testGestureLongPress()
{
    QVERIFY( !mView->mItemContextMenu );
    QPointF coordsNoHit(10,10);    
    mView->gestureLongPress( coordsNoHit );
    QVERIFY( !mView->mItemContextMenu );
    
    mView->init();
    HbTransparentWindow* sharedVideoWidget = mView->mSharedVideoWidget;
    QVERIFY( sharedVideoWidget != NULL );
    mView->mSharedVideoWidget->setGeometry(QRectF(5,100, 200, 200));
    
    QPointF coordsHitSharedVideo(20,150);    
    mView->gestureLongPress( coordsHitSharedVideo );
    QVERIFY( mView->mItemContextMenu );
    
    delete mView->mItemContextMenu;
    mView->mItemContextMenu = 0;
    
    QPointF coordsNoHit2(10,400);    
    mView->gestureLongPress( coordsNoHit2 );
    QVERIFY( !mView->mItemContextMenu );

    HbTransparentWindow* receivedVideoWidget = mView->mReceivedVideoWidget;
    QVERIFY( receivedVideoWidget != NULL );
    mView->mReceivedVideoWidget->setGeometry(QRectF(5,350, 200, 200));

    QPointF coordsHitRecievedVideo(10,400);    
    mView->gestureLongPress( coordsHitRecievedVideo );
    QVERIFY( !mView->mItemContextMenu );
}


void UT_LcView::testGestureShortPress()
{
    mView->init();
    mView->menu()->setVisible(false);
    mView->mSharedVideoWidget->setGeometry(QRectF(5,50, 100, 100));
    mView->mReceivedVideoWidget->setGeometry(QRectF(5,200, 200, 400));
 
    UT_SET_ORIENTATION( Qt::Horizontal );
    mEngine->setFullScreenMode(true);
    mView->gestureShortPress();
    QVERIFY( !mEngine->fullScreenMode());
    
    // toggled
    mView->gestureShortPress();
    QVERIFY( mEngine->fullScreenMode());
}


void UT_LcView::testTimerEvent()
{
    mView->init();
    // timer time out
    QTimerEvent* event = new QTimerEvent( mView->timerId );
    mView->menu()->setVisible(false);
    mView->timerEvent( event );
    QVERIFY( mEngine->fullScreenMode());
    QVERIFY( !HbStubHelper::isTitleBarVisible() );
    QVERIFY( !HbStubHelper::isStatusBarVisible() );
    QVERIFY( !mView->toolBar()->isVisible() );    
    QVERIFY( !mView->mEndCallButton->isVisible());
    QVERIFY( mView->mDuration->isVisible());
    QVERIFY( mView->mRecipient->isVisible());    
    delete event;
    
    // not a timer we want
    mView->toFullScreen( false );
    event = new QTimerEvent( 22222 ); // some number
    mView->timerEvent( event );
    QVERIFY( !mEngine->fullScreenMode() );
    QVERIFY( HbStubHelper::isTitleBarVisible() );
    QVERIFY( HbStubHelper::isStatusBarVisible() );
    QVERIFY( mView->toolBar()->isVisible() );    
    QVERIFY( mView->mEndCallButton->isVisible());
    QVERIFY( mView->mDuration->isVisible());
    QVERIFY( mView->mRecipient->isVisible());
    delete event;
}

void UT_LcView::testShareImage()
{
    QVERIFY( !lcutStub_LcUiEngine_isImageShared );
    mView->shareImage();
    //QVERIFY( lcutStub_LcUiEngine_isImageShared() );
}

void UT_LcView::testUpdateSwapLayout()
{    
    mView->init();
    mView->menu()->setVisible(false);
    mView->mEndCallButton = mEndCallButton;
    UT_SET_ORIENTATION( Qt::Horizontal );
    mEngine->setFullScreenMode(true);
    mView->updateSwapLayout();
    QVERIFY( mEngine->fullScreenMode());
    QVERIFY( !HbStubHelper::isTitleBarVisible() );
    QVERIFY( !HbStubHelper::isStatusBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
    QVERIFY( !mView->mEndCallButton->isVisible());
    QVERIFY( mView->mDuration->isVisible());
    QVERIFY( mView->mRecipient->isVisible());    
}

void UT_LcView::testMenuAboutToShow()
{
    mView->init();
    mView->menu()->setVisible(false);
    mView->toFullScreen( true );
    mView->menuAboutToShow();
    QVERIFY( !mEngine->fullScreenMode());
    QVERIFY( HbStubHelper::isTitleBarVisible() );
    QVERIFY( HbStubHelper::isStatusBarVisible() );
    QVERIFY( mView->toolBar()->isVisible() );    
    QVERIFY( mView->mEndCallButton->isVisible());
    QVERIFY( mView->mDuration->isVisible());
    QVERIFY( mView->mRecipient->isVisible());
    int tId = mView->timerId;
    mView->watchInactivity();    
    QVERIFY( mView->timerId != tId );

}

void UT_LcView::testTranslateRectForOrientation()
{
    UT_SET_ORIENTATION( Qt::Horizontal );
    QRectF origRect( 10, 10, 100, 200 );
    QRectF modRect = mView->translateRectForOrientation(origRect);
    QVERIFY( modRect.height() == origRect.width() );
    QVERIFY( modRect.width() == origRect.height() );
}

void UT_LcView::testTranslatePointForOrientation()
{
    UT_SET_ORIENTATION( Qt::Horizontal );
    QPointF origPoint( 10, 20 );
    QPointF modPoint = mView->translatePointForOrientation(origPoint);
    QVERIFY( origPoint != modPoint );
}

void  UT_LcView::testOpenDialpad()
{
    delete mView;
    mView = mRepository->twowayView();
    HbMainWindow& window = *(HbInstance::instance()->allMainWindows().at(0));  
    QPointF pos = window.pos();
    window.setOrientation(Qt::Horizontal);
    
    // init
    mView->mRepository.mLayoutSection = lcLayoutLandscapeDefaultId;
    mView->menu()->setVisible(true);
    
    mView->openDialpad();
    
    QVERIFY(mView->mDialpad->isOpen());   
    QVERIFY(!mView->mDialpad->mIsCallButtonEnabled);
    QVERIFY(mView->mRepository.mLayoutSection == lcLayoutLandscapeDialpadId);    
    QVERIFY( HbStubHelper::isTitleBarVisible() );
    QVERIFY( HbStubHelper::isStatusBarVisible() );
    QCOMPARE(mView->menu()->actions().size(), 0);    
    QVERIFY(!mView->mEngine.fullScreenMode());
}

void  UT_LcView::testDialpadOpened()
{
    // no implementation
}

void  UT_LcView::testDialpadClosed()
{
    delete mView;
    mView = mRepository->twowayView();
    
    // test : no fullscreen,landscape , open and close dialpad scenario     
    HbMainWindow& window = *(HbInstance::instance()->allMainWindows().at(0));  
    window.setOrientation( Qt::Horizontal );
    mView->openDialpad();    
    
    // Enter some dummy text
    mView->mDialpad->editor().setText( QString("1") );
    mView->dialpadClosed();
    // Ensure text is cleared.
    
    QCOMPARE(mView->mDialpad->editor().text(), QString::fromAscii(""));
    QVERIFY(!mView->mDialpad->editor().text().length());
    
    // check that layout has changed
    QVERIFY(mView->mRepository.layout()!= lcLayoutLandscapeDialpadId);
    QVERIFY( HbStubHelper::isTitleBarVisible() );
    QVERIFY( HbStubHelper::isStatusBarVisible() );
    QVERIFY(mView->toolBar()->isVisible());
    QVERIFY(mView->mEndCallButton->isVisible());
    QVERIFY(mView->mRecipient->isVisible());
    QVERIFY(mView->mDuration->isVisible());    
    QVERIFY(mView->menu()->isVisible());    
    QVERIFY(!mView->mEngine.fullScreenMode());
    
    // test : swapped,fullscreen,landscape,open and close dialpad scenario
    mView->mRepository.loadLayout( lcLayoutLandscapeSwappedId );    
    mView->openDialpad();
    mView->dialpadClosed();
    // check for swapped view appears
    QVERIFY( mView->mRepository.layout()== lcLayoutLandscapeSwappedId );   
    // check titlebar invisible
    QVERIFY( HbStubHelper::isTitleBarVisible() );
    QVERIFY( HbStubHelper::isStatusBarVisible() );
    // check toolbar invisible
    QVERIFY( mView->toolBar()->isVisible() );    
    QVERIFY(!mView->mEngine.fullScreenMode());

}


void  UT_LcView::testDialpadEditorTextChanged()
{   
    delete mView;
    mView = mRepository->twowayView();
    
    mView->openDialpad();
    //text editor is one character long
    QVERIFY( lcutStub_LcUiEngine_getDialToneChar() == '0');
    mView->mDialpad->editor().setText( QString("1") );
    mView->dialpadEditorTextChanged();
    QVERIFY( lcutStub_LcUiEngine_getDialToneChar() == '1' );

    // We transmit dial tone character sepretly.
    mView->mDialpad->editor().setText( QString("12345") );
    mView->dialpadEditorTextChanged();
    QVERIFY( lcutStub_LcUiEngine_getDialToneChar() == '5' );
}

void  UT_LcView::testBack()
{       
    delete mView;
    mView = mRepository->twowayView();    
    mView->openDialpad();
    mView->back();
    QVERIFY( !mView->mDialpad->isOpen() );    
}

void UT_LcView::testAddOptionsMenuActions()
{
    delete mView;
    mView = mRepository->twowayView();
    
    mView->addOptionsMenuActions();
    //TODO: stub implementation of void QGraphicsWidget::addAction(QAction *action);
    //QCOMPARE(mView->menu()->actions().size(), 2);
}

void UT_LcView::testWatchInactivity()
{       
    mView->init();
    
    // test : in fullscreen mode already, dont start timer again
    int tid = mView->timerId ;
    mView->mEngine.setFullScreenMode( true );    
    mView->watchInactivity();
    QVERIFY( tid==mView->timerId );
  
    // test : not in full screen but dialpad view. dont start timer
    mView->mEngine.setFullScreenMode( false );
    QVERIFY( mView->mDialpad );
    mView->openDialpad();
    mView->watchInactivity();
    QVERIFY( tid==mView->timerId );    
    
    // test : not in full screen and no dialpad , start timer
    mView->mEngine.setFullScreenMode( false );
    QVERIFY( mView->mDialpad );
    mView->mDialpad->closeDialpad();
    mView->watchInactivity();
    QVERIFY( tid!=mView->timerId );
    
}

void UT_LcView::testIsVideoPositionedCorrectly()
{
    //Test1: Null video Widget returns true
    mRepository->mReturnSendVideo = false;
    mView->init();
    QVERIFY( !mView->mSharedVideoWidget );
    QVERIFY(mView->isVideoPositionedCorrectly(mView->mSharedVideoWidget));
    
    //Test2: Valid Video widgets with correct position returns true:
    mRepository->mReturnSendVideo = true;
    mView->init();
    QVERIFY( mView->mSharedVideoWidget );
    QVERIFY( mView->isVideoPositionedCorrectly(mView->mSharedVideoWidget ));
    
    //Test3: Vaild video widgets withi co-ordinates returns false.
    mView->mSharedVideoWidget->setPos(QPointF(0,0));
    QVERIFY( mView->mSharedVideoWidget );
    QVERIFY( !mView->isVideoPositionedCorrectly(mView->mSharedVideoWidget ));
}


void UT_LcView::testIsPositioned()
{   
    //Test: isViewReady is true and proper co-ordinates        
    mRepository->mReturnSendVideo = true;
    mView->init();
    QVERIFY( !mView->isViewReady );        
    QVERIFY( mView->mSharedVideoWidget );
    QVERIFY( mView->isVideoPositionedCorrectly(mView->mSharedVideoWidget ));
    mView->isPositioned();
    QVERIFY( mView->isViewReady );
    
    //Test: isViewReady is false and its first time and co ordinates are 0,0
    mView->isViewReady = false;    
    mView->mSharedVideoWidget->setPos(QPointF(0,0));
    QVERIFY( mView->mSharedVideoWidget );
    QVERIFY( !mView->isVideoPositionedCorrectly(mView->mSharedVideoWidget ));
    mView->isPositioned();
    QVERIFY( !mView->isViewReady );
}


void UT_LcView::testShowVolumeSlider()
{
    mView->showVolumeSlider(5);
    QVERIFY( mView->mRepository.mVolumeSlider->isVisible() );
    QCOMPARE( mView->mRepository.mVolumeSlider->value(),  5);
    QCOMPARE( mView->mRepository.mVolumeSlider->minimum(), lcVolumeMin );
    QCOMPARE( mView->mRepository.mVolumeSlider->maximum(), lcVolumeMax);
}


void UT_LcView::testShowSendVideo()
{
    mRepository->mReturnSendVideo = true;
    mView->init();
    mView->mSharedVideoWidget->setVisible(false);
    mView->showSendVideo();
    QVERIFY( mView->mSharedVideoWidget->isVisible() );
}


void UT_LcView::testshowReceivedVideo()
{
    mView->init();
    mView->mReceivedVideoWidget->setVisible(false);
    mView->showReceivedVideo();
    QVERIFY( mView->mReceivedVideoWidget->isVisible() );
}


// End of File
