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
#include <hbzoomsliderpopup.h>
#include <hbtransparentwindow.h>
#include <qgraphicssceneevent.h>
#include <qgraphicsitem.h>
#include <QGesture.h>
#include <dialpad.h>
#include <HbTapGesture.h>

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
    mAction = new HbAction();
    mAction2 = new HbAction();
}

void UT_LcView::cleanup()
{
    lcutStub_LcUiEngine_reset();
    delete mRecipient;
    delete mView;
    delete mEngine;
    delete mRepository;
    delete mAction;
    delete mAction2;
    delete mEndCallButton;
}

void UT_LcView::testConstructor()
{
    QVERIFY( mView );
}

void UT_LcView::testActivated()
{
    lcutStub_LcUiEngine_reset();
    QVERIFY( lcutStub_LcUiEngine_getCurrentView() == 0 );
    mView->activated();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_updateSession, 0 ) );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setOrientation, 1 ) );
    QVERIFY( lcutStub_LcUiEngine_getCurrentView() == mView );
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas, 2 ) );
    
    lcutStub_LcUiEngine_reset();
    mView->mChangeCameraAction = mAction;
    mView->mMuteAction = mAction;
    mView->mSpeakerAction = mAction;

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
}

void UT_LcView::testInit()
{
    mRepository->mReturnSendVideo = false;
    mView->init();
    QVERIFY( mView->mRecipient );
    QVERIFY( !mView->mSharedVideoWidget );
    QVERIFY( !mView->mZoomSlider );
    
    mRepository->mReturnSendVideo = true;
    mView->init();
    QVERIFY( mView->mRecipient );
    QVERIFY( mView->mSharedVideoWidget );
    QVERIFY( mView->mZoomSlider );
    QVERIFY( mView->menu());
}

void UT_LcView::testDeactivated()
{
    mView->deactivated();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_setContentAreas ) );
}

void UT_LcView::testSetCameraActionToMain()
{
    mView->mChangeCameraAction = mAction;
    mView->mMuteAction = mAction;
    mView->mSpeakerAction = mAction;
    mView->mDisableCameraAction = mAction;

    mView->setCameraActionToMain();
    QVERIFY( mView->mChangeCameraAction->text().isEmpty() );
    QCOMPARE( mView->mChangeCameraAction->icon(), HbIcon(lcIconNameMainCamera) );
}

void UT_LcView::testSetCameraActionToSecondary()
{
    mView->mChangeCameraAction = mAction;
    mView->mMuteAction = mAction;
    mView->mSpeakerAction = mAction;
    mView->mDisableCameraAction = mAction;

    mView->setCameraActionToSecondary();
    QVERIFY( mView->mChangeCameraAction->text().isEmpty() );
    QCOMPARE( mView->mChangeCameraAction->icon(), HbIcon(lcIconNameSecondaryCamera) );
}

void UT_LcView::testSetMuteActionToUnmute()
{
    mView->mChangeCameraAction = mAction;
    mView->mMuteAction = mAction;
    mView->mSpeakerAction = mAction;
    mView->mDisableCameraAction = mAction;
    
    mView->setMuteActionToUnmute();
    QVERIFY( mView->mMuteAction->text().isEmpty() );
    QCOMPARE (mView->mMuteAction->icon(), HbIcon(lcIconNameUnmuteMic));
}

void UT_LcView::testSetMuteActionToMute()
{
    mView->mChangeCameraAction = mAction;
    mView->mMuteAction = mAction;
    mView->mSpeakerAction = mAction;
    mView->mDisableCameraAction = mAction;
    
    mView->setMuteActionToMute();
    QVERIFY( mView->mMuteAction->text().isEmpty() );
    QCOMPARE (mView->mMuteAction->icon(), HbIcon(lcIconNameMuteMic));
}

void UT_LcView::testSetSpeakerActionToHandset()
{
    mView->mChangeCameraAction = mAction;
    mView->mMuteAction = mAction;
    mView->mSpeakerAction = mAction;
    mView->mDisableCameraAction = mAction;

    mView->setSpeakerActionToHandset();
    QVERIFY( mView->mSpeakerAction->text().isEmpty() );
    QCOMPARE( mView->mSpeakerAction->icon(), HbIcon(lcIconNameHandset) );
}

void UT_LcView::testSetSpeakerActionToSpeaker()
{
    mView->mChangeCameraAction = mAction;
    mView->mMuteAction = mAction;
    mView->mSpeakerAction = mAction;
    mView->mDisableCameraAction = mAction;

    mView->setSpeakerActionToSpeaker();
    QVERIFY( mView->mSpeakerAction->text().isEmpty() );
    QCOMPARE( mView->mSpeakerAction->icon(), HbIcon(lcIconNameLoudspeaker) );
}

void UT_LcView::testSetCameraActionToEnable()
{
    mView->setCameraActionToEnable();

    mView->mDisableCameraAction = mAction;
    mView->mDisableCameraMenuAction = mAction2;

    mView->setCameraActionToEnable();
    QVERIFY( mView->mDisableCameraAction->text().isEmpty() );
    QCOMPARE( mView->mDisableCameraAction->icon(), HbIcon(lcIconNameEnableCamera) );
    QCOMPARE( mView->mDisableCameraMenuAction->text(), hbTrId( "txt_vt_menu_enable_camera" ) );   
}

void UT_LcView::testSetCameraActionToDisable()
{
    mView->setCameraActionToDisable();

    mView->mDisableCameraAction = mAction;
    mView->mDisableCameraMenuAction = mAction2;

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
    QVERIFY( !mView->mLandscapeTimer );
    mView->mLandscapeTimer = new QTimer();
    mView->mLandscapeTimer->start();
    int oldTimerId = mView->mLandscapeTimer->timerId();
    mView->mute();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_mute ) );
    QVERIFY( mView->mLandscapeTimer->isActive() );
    QVERIFY( oldTimerId != mView->mLandscapeTimer->timerId() );
}

void UT_LcView::testChangeCamera()
{
    mView->init();
    QVERIFY( mView->mLandscapeTimer );
    QVERIFY( mView->mLandscapeTimer->isActive() );
    int oldTimerId = mView->mLandscapeTimer->timerId();
    mView->changeCamera();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_toggleCamera ) );
    QVERIFY( mView->mLandscapeTimer->isActive() );
    QVERIFY( oldTimerId != mView->mLandscapeTimer->timerId() );
}

void UT_LcView::testSwitchToVoiceCall()
{
    mView->switchToVoiceCall();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_stop ) );
}

void UT_LcView::testDisableCamera()
{
    mView->init();
    QVERIFY( mView->mLandscapeTimer );
    QVERIFY( mView->mLandscapeTimer->isActive() );
    int oldTimerId = mView->mLandscapeTimer->timerId();
    mView->disableCamera();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_disableCamera ) );
    QVERIFY( mView->mLandscapeTimer->isActive() );
    QVERIFY( oldTimerId != mView->mLandscapeTimer->timerId() );
}

void UT_LcView::testSpeaker()
{
    QVERIFY( !mView->mLandscapeTimer );
    mView->mLandscapeTimer = new QTimer();
    mView->mLandscapeTimer->start();
    int oldTimerId = mView->mLandscapeTimer->timerId();
    mView->speaker();
    QVERIFY( lcutStub_LcUiEngine_expectCall( lcutStub_LcUiEngine_speaker ) );
    QVERIFY( mView->mLandscapeTimer->isActive() );
    QVERIFY( oldTimerId != mView->mLandscapeTimer->timerId() );
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

void UT_LcView::testShowZoom()
{
    QVERIFY( !mView->mSharedVideoWidget );
    QVERIFY( !mView->mZoomSlider );
    mView->showZoom();
    QVERIFY( !mView->mZoomSlider );
    QVERIFY( !mView->mSharedVideoWidget );

    LcVideoWidget sendVideo;
    mView->mSharedVideoWidget = &sendVideo;
    HbZoomSliderPopup zoomSlider;
    mView->mZoomSlider = &zoomSlider;
    
    QVERIFY( mView->mSharedVideoWidget );
    QVERIFY( mView->mZoomSlider );
    mView->showZoom();
    QVERIFY( mView->mSharedVideoWidget );
    QVERIFY( mView->mZoomSlider );
    QVERIFY( mView->mZoomSlider->isVisible() );
    LcControlValues values;
    mEngine->zoomValues( values );
    QCOMPARE( mView->mZoomSlider->value(), values.mValue );
    QCOMPARE( mView->mZoomSlider->minimum(), values.mMinValue );
    QCOMPARE( mView->mZoomSlider->maximum(), values.mMaxValue );
    
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
    // TODO: effecthandler->setvisibility temporarily NOP
    /*QVERIFY( sharedVideoWidget->isVisible() );
    QVERIFY( receivedVideoWidget->isVisible() );*/
}

void UT_LcView::testCurrentLayout()
{
    mView->init();
    QString layout;
    
    //1. Portrait layout
    UT_SET_ORIENTATION( Qt::Vertical );
    layout = mView->currentLayout();
    QVERIFY( layout == lcLayoutPortraitDefaultId );

    //2. Landscape layout
    UT_SET_ORIENTATION( Qt::Horizontal );
    layout = mView->currentLayout();
    QVERIFY( layout == lcLayoutLandscapeDefaultId );    

    //3. Landscape Swaped lcLayoutPortraitSwappedId
    
    // Shared window x cordintate is higher 
    // than Received windows x Cordinate.
    
    QRectF pos1 = QRectF(100,300, 400, 400);
    QRectF pos2 = QRectF(5,100, 200, 200);

    UT_SET_ORIENTATION( Qt::Horizontal );
    mView->mEffectHandler->mSwapInProgress = true;
    mView->mSharedVideoWidget->setGeometry(pos1);
    mView->mReceivedVideoWidget->setGeometry(pos2);

    layout = mView->currentLayout();
    QVERIFY( layout == lcLayoutLandscapeSwappedId );

    //4. If Its already Swaped Change to the normal Landscape Mode
    mView->mSharedVideoWidget->setGeometry(pos2);
    mView->mReceivedVideoWidget->setGeometry(pos1);
    layout = mView->currentLayout();
    QVERIFY( layout == lcLayoutLandscapeDefaultId );


    //5. Portrait Swaped lcLayoutPortraitSwappedId

    // Shared window y cordintate is higher 
    // than Received windows y Cordinate.

    UT_SET_ORIENTATION( Qt::Vertical );
    mView->mEffectHandler->mSwapInProgress = true;
    mView->mSharedVideoWidget->setGeometry(pos1);
    mView->mReceivedVideoWidget->setGeometry(pos2);

    layout = mView->currentLayout();
    QVERIFY( layout == lcLayoutPortraitSwappedId );

    //6. If its swaped in portait mode switch not normal portrait mode
    mView->mEffectHandler->mSwapInProgress = true;
    mView->mSharedVideoWidget->setGeometry(pos2);
    mView->mReceivedVideoWidget->setGeometry(pos1);
    layout = mView->currentLayout();
    QVERIFY( layout == lcLayoutPortraitDefaultId );

}


void UT_LcView::testActivateFullScreen()
{
    QString layout;
    UT_SET_ORIENTATION( Qt::Horizontal );

    // Test1: Initial Layout is Vertical
    mEngine->setFullScreenMode(false);
    mView->activateFullScreen();
    QVERIFY( !mEngine->fullScreenMode());

    mView->init();
    mView->mEndCallButton = mEndCallButton;
    mEngine->setFullScreenMode(false);
    mView->activateFullScreen();
    QVERIFY( mEngine->fullScreenMode());

    QVERIFY( !mView->isItemVisible(Hb::TitleBarItem ) );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
    QVERIFY( !mView->mEndCallButton->isVisible());

    layout = mView->currentLayout();
    QVERIFY( layout == lcLayoutLandscapeDefaultId );
    
    
    // Test2: Retry Full Screen if already Enabled.
    QVERIFY( mEngine->fullScreenMode());
    layout = mView->currentLayout();
    QVERIFY( layout == lcLayoutLandscapeDefaultId );
    
}

void UT_LcView::testDeactivateFullScreen()
{
    mView->init();
    mView->mEndCallButton = mEndCallButton;
    QString layout;
    UT_SET_ORIENTATION( Qt::Horizontal );
    layout = mView->currentLayout();
    
    // Test2: Deactivate from Full Screen Mode
    mEngine->setFullScreenMode(true);
    mView->deActivateFullScreen();

    QVERIFY( !mEngine->fullScreenMode());
    QVERIFY( mView->isTitleBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
    QVERIFY( mView->mEndCallButton->isVisible());

    QVERIFY( layout == lcLayoutLandscapeDefaultId );

    // Test2: Trying to Deactivate full sceeen 
    // when its not full screen
    mEngine->setFullScreenMode(false);
    mView->deActivateFullScreen();
    
    QVERIFY( mView->isTitleBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
    QVERIFY( mView->mEndCallButton->isVisible());
    QVERIFY( layout == lcLayoutLandscapeDefaultId );
}


void UT_LcView::testGestureEvent()
{
    mView->init();
    UT_SET_ORIENTATION( Qt::Horizontal );
    mView->mItemContextMenu = 0;
    mView->mSharedVideoWidget->setGeometry(QRectF(5,50, 100, 100));
    mView->mReceivedVideoWidget->setGeometry(QRectF(5,200, 200, 400));
    QList<QGesture*> list;
    
    // Test1: Short tap Event anywhere on screen
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
    // no contet menu
    QVERIFY( !mView->mItemContextMenu );
    mView->mItemContextMenu = 0;
    list.clear();
    
    // Test2: Long tap event on Shared Video Widget.
    UT_SET_ORIENTATION( Qt::Vertical );
    QTapGesture *tapGesture1 = new QTapGesture();
    tapGesture1->setPosition(sharedVideoPos);
    HbStubHelper::setGestureState(Qt::GestureUpdated);
    HbStubHelper::setGestureStyleHint(HbTapGesture::TapAndHold);
    list.append(tapGesture1);
    QGestureEvent event2(list);
    mEngine->setFullScreenMode(true);
    mView->gestureEvent(&event2);
    // No affect to the full screen mode it remains same  
    QVERIFY( mEngine->fullScreenMode());
    // Ensure context menu is created
    QVERIFY( mView->mItemContextMenu );
    list.clear();
    mView->mItemContextMenu = 0;
    
    // Test3: Long tap event on Received Video Widget
    UT_SET_ORIENTATION( Qt::Vertical );
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
    QVERIFY( !mView->mItemContextMenu );
    
    //Test4: Not Handled States.
    HbStubHelper::setGestureState(Qt::GestureStarted);
    mEngine->setFullScreenMode(true);
    mView->gestureEvent(&event3);
    // Ensure its not handled everything remains same
    QVERIFY( mEngine->fullScreenMode());
    QVERIFY( !mView->mItemContextMenu );
    list.clear();
    
    //Test5: Not Handled Event
    QSwipeGesture *swipeGesture = new QSwipeGesture();
    list.append(swipeGesture);
    QGestureEvent event4(list);
    mEngine->setFullScreenMode(true);
    mView->gestureEvent(&event4);
    // Ensure it does not affect full screen mode.
    QVERIFY( mEngine->fullScreenMode());
    QVERIFY( !mView->mItemContextMenu );

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
    mView->mSharedVideoWidget->setGeometry(QRectF(5,50, 100, 100));
    mView->mReceivedVideoWidget->setGeometry(QRectF(5,200, 200, 400));
 
    UT_SET_ORIENTATION( Qt::Horizontal );
    mEngine->setFullScreenMode(true);
    mView->gestureShortPress();
    QVERIFY( !mEngine->fullScreenMode());
    
    // toggled
    mView->gestureShortPress();
    QVERIFY( mEngine->fullScreenMode());

    // no action if its not landscape.
    UT_SET_ORIENTATION( Qt::Vertical );
    mView->gestureShortPress();
    QVERIFY( mEngine->fullScreenMode());
}



void UT_LcView::testActivatePortrait()
{

    mView->activatePortrait();

    mView->init();
    mView->mEndCallButton = mEndCallButton;
    mView->activateFullScreen();
    QVERIFY( mEngine->fullScreenMode());
    QVERIFY( !mView->isTitleBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
    QVERIFY( !mView->mEndCallButton->isVisible());

    mView->activatePortrait();
    QVERIFY( !mEngine->fullScreenMode());
    QVERIFY( mView->isTitleBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
    QVERIFY( mView->mEndCallButton->isVisible());

}

void UT_LcView::testUpdateUiElements()
{
    mView->init();
    UT_SET_ORIENTATION( Qt::Horizontal );
    mView->updateUiElements();
    QVERIFY( !mEngine->fullScreenMode());
    QVERIFY( mView->isTitleBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );

    UT_SET_ORIENTATION( Qt::Vertical );
    mView->updateUiElements();
    QVERIFY( !mEngine->fullScreenMode());
    QVERIFY( mView->isTitleBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
}

void UT_LcView::testLandscapeTimerTimeout()
{
    mView->init();
    mView->deActivateFullScreen();

    // No effect as in vertical zontal mode
    UT_SET_ORIENTATION(Qt::Vertical);
    QVERIFY( !mEngine->fullScreenMode());
    QVERIFY( mView->isTitleBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );

    mView->landscapeTimerTimeout();
    QVERIFY( !mEngine->fullScreenMode());
    QVERIFY( mView->isTitleBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
    
    // Effective in horizontal mode
    UT_SET_ORIENTATION(Qt::Horizontal);
    mView->landscapeTimerTimeout();
    
    QVERIFY( mEngine->fullScreenMode());
    QVERIFY( !mView->isTitleBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );

    // Test: If menu is open than it should not go into full screen mode.
    mView->deActivateFullScreen();
    mView->mIsOptionMenuOpen = true;
    mView->landscapeTimerTimeout();
    QVERIFY( !mEngine->fullScreenMode());
    QVERIFY( mView->isTitleBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
}

void UT_LcView::testShareImage()
{
    QVERIFY( !lcutStub_LcUiEngine_isImageShared );
    mView->shareImage();
    //QVERIFY( lcutStub_LcUiEngine_isImageShared() );
}


void UT_LcView::testShowControl()
{
    mView->showControl();
    QVERIFY( mView->isTitleBarVisible() );
    QVERIFY( mView->toolBar()->isVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
    QVERIFY( mView->mEndCallButton->isVisible());
    QVERIFY( mView->mRecipient->isVisible());
}

void UT_LcView::testHideControl()
{
    mView->hideControl();
    QVERIFY( !mView->isTitleBarVisible() );
    QVERIFY( !mView->toolBar()->isVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
    QVERIFY( !mView->mEndCallButton->isVisible());
    QVERIFY( !mView->mRecipient->isVisible());
}

void UT_LcView::testUpdateSwapLayout()
{
    //Test1: In Full Screen Mode controls Should be hidden.
    mView->init();
    mView->mEndCallButton = mEndCallButton;
    UT_SET_ORIENTATION( Qt::Horizontal );
    mEngine->setFullScreenMode(true);

    mView->updateSwapLayout();
    QVERIFY( !mView->isItemVisible(Hb::TitleBarItem ) );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
    QVERIFY( !mView->mEndCallButton->isVisible());

    // Test2: If its not full screen show the controls
    mEngine->setFullScreenMode(false);
    mView->updateSwapLayout();
    QVERIFY( mView->isTitleBarVisible() );
    QVERIFY( !mView->isItemVisible(Hb::DockWidgetItem ) );
    QVERIFY( mView->mEndCallButton->isVisible());
}

void UT_LcView::testMenuAboutToShow()
{
    mView->init();
    QVERIFY( !mView->mIsOptionMenuOpen  );
    mView->menuAboutToShow();
    QVERIFY( mView->mIsOptionMenuOpen  );
    QVERIFY( !mView->mLandscapeTimer->isActive());

}

void UT_LcView::testMenuAboutToHide()
{
    mView->init();
    mView->menuAboutToHide();
    QVERIFY( !mView->mIsOptionMenuOpen  );
    QVERIFY( mView->mLandscapeTimer->isActive());
}

void UT_LcView::testTranslateRectForOrientation()
{
    UT_SET_ORIENTATION( Qt::Vertical );
    QRectF origRect( 10, 10, 100, 200 );
    
    QRectF modRect = mView->translateRectForOrientation(origRect);
    QVERIFY( origRect == modRect );
    
    UT_SET_ORIENTATION( Qt::Horizontal );
    modRect = mView->translateRectForOrientation(origRect);
    QVERIFY( modRect.height() == origRect.width() );
    QVERIFY( modRect.width() == origRect.height() );
}

void UT_LcView::testTranslatePointForOrientation()
{
    UT_SET_ORIENTATION( Qt::Vertical );
    QPointF origPoint( 10, 20 );
    
    QPointF modPoint = mView->translatePointForOrientation(origPoint);
    QVERIFY( origPoint == modPoint );
    
    UT_SET_ORIENTATION( Qt::Horizontal );
    modPoint = mView->translatePointForOrientation(origPoint);
    QVERIFY( origPoint != modPoint );
}

void UT_LcView::testResetLandscapeTimer()
{
    // No timer
    QVERIFY( !mView->mLandscapeTimer );
    mView->resetLandscapeTimer();
    QVERIFY( !mView->mLandscapeTimer );
    
    // Timer not acitve
    mView->mLandscapeTimer = new QTimer();
    QVERIFY( !mView->mLandscapeTimer->isActive() );
    mView->resetLandscapeTimer();
    QVERIFY( !mView->mLandscapeTimer->isActive() );

    // Timer is active
    mView->mLandscapeTimer->start();
    int oldTimerId = mView->mLandscapeTimer->timerId();
    mView->resetLandscapeTimer();
    QVERIFY( mView->mLandscapeTimer->isActive() );
    QVERIFY( oldTimerId != mView->mLandscapeTimer->timerId() );
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
    QVERIFY(mView->mRepository.mLayoutSection == lcLayoutLandscapeDialpadId);    
    QVERIFY(mView->isTitleBarVisible());
    QCOMPARE(mView->menu()->actions().size(), 0);
    QVERIFY(!mView->mLandscapeTimer->isActive());
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
    mView->dialpadClosed();
    // check that layout has changed
    QVERIFY(mView->mRepository.layout()!= lcLayoutLandscapeDialpadId);
    QVERIFY(mView->isTitleBarVisible());
    QVERIFY(mView->toolBar()->isVisible());
    QVERIFY(mView->mEndCallButton->isVisible());
    QVERIFY(mView->mRecipient->isVisible());
    QVERIFY(mView->mDuration->isVisible());    
    QVERIFY(mView->menu()->isVisible());
    QVERIFY(mView->mLandscapeTimer->isActive());
    QVERIFY(!mView->mEngine.fullScreenMode());
    
    // test : swapped,fullscreen,landscape,open and close dialpad scenario
    mView->mRepository.loadLayout( lcLayoutLandscapeSwappedId );    
    mView->openDialpad();
    mView->dialpadClosed();
    // check for swapped view appears
    QVERIFY( mView->mRepository.layout()== lcLayoutLandscapeSwappedId );   
    // check titlebar invisible
    QVERIFY( mView->isTitleBarVisible() );
    // check toolbar invisible
    QVERIFY( mView->toolBar()->isVisible() );
    QVERIFY(mView->mLandscapeTimer->isActive());
    QVERIFY(!mView->mEngine.fullScreenMode());
    
    // test : orientation to portrait 
    //        swapped,fullscreen,landscape,open and close dialpad scenario
    mView->mRepository.loadLayout( lcLayoutLandscapeSwappedId );    
    mView->openDialpad();
    window.setOrientation( Qt::Vertical );
    mView->dialpadClosed();
    // check for swapped view appears
    QVERIFY( mView->mRepository.layout()== lcLayoutPortraitSwappedId );
    // check titlebar visible
    QVERIFY( mView->isTitleBarVisible() );
    // check toolbar visible
    QVERIFY( mView->toolBar()->isVisible() );
    QVERIFY(!mView->mLandscapeTimer->isActive());
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
