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


#include "ut_lceffecthandler.h"
#include "ut_lcuidefs.h"
#include "lceffecthandler.h"
#include "lcuiengine.h"
#include "lcuidefs.h"
#include "lcvideowidget.h"
#include <QtTest/QtTest>
#include <hbmainwindow.h>
#include <hbinstance.h>

#define UT_SET_ORIENTATION(orient) \
HbInstance::instance()->allMainWindows().at(0)->setOrientation(orient);

void UT_LcEffectHandler::initTestCase()
{
    lcutStub_LcUiEngine_reset();
    //must have main window, even if it not used
    mMainWindow = new HbMainWindow();
}

void UT_LcEffectHandler::cleanupTestCase()
{
    lcutStub_LcUiEngine_reset();
    delete mMainWindow;
}


void UT_LcEffectHandler::init()
{
    mEngine = new LcUiEngine( lcEngineName, lcDisplayName );

    mSharedVideo = new LcVideoWidget();
    mSharedVideoOverlay = new LcVideoWidget(0,false);
    mReceivedVideo = new LcVideoWidget();
    mReceivedVideoOverlay = new LcVideoWidget(0,false);
    mEffectHandler = new LcEffectHandler(
            *mEngine,mSharedVideo,mSharedVideoOverlay,
            mReceivedVideo,mReceivedVideoOverlay);
    
}

void UT_LcEffectHandler::cleanup()
{
    lcutStub_LcUiEngine_reset();
    delete mEffectHandler;
    delete mEngine;
    delete mSharedVideo;
    delete mSharedVideoOverlay;
    delete mReceivedVideoOverlay;
    delete mReceivedVideo;
}

void UT_LcEffectHandler::testConstructor()
{
    QVERIFY( mEffectHandler );
}

void UT_LcEffectHandler::testSwap()
{
    clearWidgets();
    QSignalSpy spy( mEffectHandler, SIGNAL(loadSwapLayout()) );
    QVERIFY( !mEffectHandler->mSharedVideo );
    QVERIFY( !mEffectHandler->mReceivedVideo );
    QVERIFY( !mEffectHandler->mSwapInProgress );
    QCOMPARE( spy.count(), 0 );

    mEffectHandler->swap();
    setWidgets();

    QVERIFY( mEffectHandler->mSwapInProgress );
    QCOMPARE( spy.count(), 1 );

    HbWidget* sharedVideoWidget = mEffectHandler->mSharedVideo;
    HbWidget* receivedVideoWidget = mEffectHandler->mReceivedVideo;
    HbWidget* sharedVideoWidget2 = mEffectHandler->mSharedVideoOverlay;
    HbWidget* receivedVideoWidget2 = mEffectHandler->mReceivedVideoOverlay;
    QRectF sharedOrig = mEffectHandler->mSharedVideo->geometry();
    QRectF recvOrig = mEffectHandler->mReceivedVideo->geometry();
    
    // Swapping not started because other widget missing
    mEffectHandler->mSwapSharedCompleted = true;
    mEffectHandler->mSwapReceivedCompleted = true;
    mEffectHandler->mSharedVideo = 0;
    mEffectHandler->swap();
    QCOMPARE( sharedVideoWidget->geometry(), sharedOrig );
    QCOMPARE( receivedVideoWidget->geometry(), recvOrig );
    QCOMPARE( mEffectHandler->mSwapSharedCompleted, true );
    QCOMPARE( mEffectHandler->mSwapReceivedCompleted, true );
    
    // Swapping not started because other widget missing
    mEffectHandler->mSharedVideo = sharedVideoWidget;
    mEffectHandler->mReceivedVideo = 0;
    mEffectHandler->swap();
    QCOMPARE( sharedVideoWidget->geometry(), sharedOrig );
    QCOMPARE( receivedVideoWidget->geometry(), recvOrig );
    QCOMPARE( mEffectHandler->mSwapSharedCompleted, true );
    QCOMPARE( mEffectHandler->mSwapReceivedCompleted, true );
    
    // Swapping started, however geometry is change only once swapping completes
    mEffectHandler->mSharedVideo = sharedVideoWidget;
    mEffectHandler->mReceivedVideo = receivedVideoWidget;
    mEffectHandler->swap();
    
    QCOMPARE( sharedVideoWidget->geometry(), sharedOrig );
    QCOMPARE( receivedVideoWidget->geometry(), recvOrig );
    QCOMPARE( mEffectHandler->mSwapSharedCompleted, false );
    QCOMPARE( mEffectHandler->mSwapReceivedCompleted, false );
    
    // Swapping started, however geometry is change only once swapping completes
    mEffectHandler->mSwapSharedCompleted = true;
    mEffectHandler->mSwapReceivedCompleted = true;
    mEffectHandler->swap();
    QCOMPARE( sharedVideoWidget->geometry(), sharedOrig );
    QCOMPARE( receivedVideoWidget->geometry(), recvOrig );
    QCOMPARE( mEffectHandler->mSwapSharedCompleted, false );
    QCOMPARE( mEffectHandler->mSwapReceivedCompleted, false );
}

void UT_LcEffectHandler::testShowSendWindow()
{
    // Shared video widget does not exist
    setWidgets();
    HbWidget* sharedVideoWidget = mEffectHandler->mSharedVideo;
    QVERIFY( sharedVideoWidget != 0 );
    sharedVideoWidget->hide();
    QVERIFY( !sharedVideoWidget->isVisible() );
    mEffectHandler->mSharedVideo = 0;
    mEffectHandler->showSendWindow();
    QVERIFY( !sharedVideoWidget->isVisible() );

    // Shared video widget exists, effects running, dont show
    HbEffect::mRunning = true;
    mEffectHandler->mSharedVideo = sharedVideoWidget;
    mEffectHandler->showSendWindow();
    QVERIFY( !mEffectHandler->mSharedVideo->isVisible() );

    // Shared video widget exists, show
    HbEffect::mRunning = false;
    mEffectHandler->mSharedVideo = sharedVideoWidget;
    mEffectHandler->showSendWindow();
    QVERIFY( mEffectHandler->mSharedVideo->isVisible() );
}

void UT_LcEffectHandler::testHideSendWindow()
{
 // Shared video widget does not exist
    setWidgets();
    HbWidget* sharedVideoWidget = mEffectHandler->mSharedVideo;
    QVERIFY( sharedVideoWidget != 0 );
    mEffectHandler->mSharedVideo = 0;
    mEffectHandler->hideSendWindow();

    // Shared video widget exists, show
    mEffectHandler->mSharedVideo = sharedVideoWidget;
    mEffectHandler->showSendWindow();
    QVERIFY( mEffectHandler->mSharedVideo->isVisible() );
    
    // Shared video widget exists, hide
    mEffectHandler->hideSendWindow();
    QVERIFY( !mEffectHandler->mSharedVideo->isVisible() );     
}

void UT_LcEffectHandler::testShowReceiveWindow()
{
    // Shared video widget does not exist
    setWidgets();
    HbWidget* receivedVideoWidget = mEffectHandler->mReceivedVideo;
    QVERIFY( receivedVideoWidget != 0 );
    receivedVideoWidget->hide();
    QVERIFY( !receivedVideoWidget->isVisible() );
    mEffectHandler->mReceivedVideo = 0;
    mEffectHandler->showReceiveWindow();
    QVERIFY( !receivedVideoWidget->isVisible() );
    
    // Received video widget exists, show
    mEffectHandler->mReceivedVideo = receivedVideoWidget;
    mEffectHandler->showReceiveWindow();
    QVERIFY( mEffectHandler->mReceivedVideo->isVisible() );
}

void UT_LcEffectHandler::testHideReceiveWindow()
{
    // Shared video widget does not exist
    setWidgets();
    HbWidget* receivedVideoWidget = mEffectHandler->mReceivedVideo;
    QVERIFY( receivedVideoWidget != 0 );
    mEffectHandler->mReceivedVideo = 0;
    mEffectHandler->hideReceiveWindow();

    
    // Received video widget exists, show
    mEffectHandler->mReceivedVideo = receivedVideoWidget;
    mEffectHandler->showReceiveWindow();
    QVERIFY( mEffectHandler->mReceivedVideo->isVisible() );
    
    // Receive video widget exists, hide
    mEffectHandler->hideReceiveWindow();
    QVERIFY( !mEffectHandler->mReceivedVideo->isVisible() );    
}


// windowFlipComplete
// windowDisappearToFlipComplete
// windowDisappearComplete
// windowAppearComplete
// WindowSwapSharedComplete
// WindowSwapReceivedComplete

void UT_LcEffectHandler::testEffectComplete()
{
    clearWidgets();
    
    // Video widgets does not exist
    HbEffect::EffectStatus status;
    mEffectHandler->windowAppearComplete( status );
    mEffectHandler->windowDisappearComplete( status );
    mEffectHandler->windowDisappearToFlipComplete( status );
    mEffectHandler->windowFlipComplete( status );

    setWidgets();

    HbWidget* sharedVideoWidget = mEffectHandler->mSharedVideo;
    HbWidget* receivedVideoWidget = mEffectHandler->mReceivedVideo;
    sharedVideoWidget->hide();
    receivedVideoWidget->hide();

    mEffectHandler->showSendWindow();
    mEffectHandler->windowAppearComplete( status );
    QVERIFY( sharedVideoWidget->isVisible() );

    mEffectHandler->windowDisappearComplete( status );
    QVERIFY( sharedVideoWidget->isVisible() );

    mEffectHandler->windowDisappearToFlipComplete( status );
    QVERIFY( !sharedVideoWidget->isVisible() );

        lcutStub_LcUiEngine_setLocalPlaying( false );
    mEffectHandler->windowFlipComplete( status );
    QVERIFY( !sharedVideoWidget->isVisible() );

        lcutStub_LcUiEngine_setLocalPlaying( true );
    mEffectHandler->windowFlipComplete( status );
    QVERIFY( sharedVideoWidget->isVisible() );

    sharedVideoWidget->hide();
    receivedVideoWidget->hide();
        lcutStub_LcUiEngine_setLocalPlaying( true );
    mEffectHandler->windowFlipComplete( status );
    QVERIFY( sharedVideoWidget->isVisible() );

    // Swapping, real video widgets are set visible only after
    // both swap effects have completeded swapping completes and completion
    // is also signal

    // Geomerty changes are handled by the view using DoCML's
    // when swapping completes and completion is also signal

    QRectF pos1 = QRectF(5,100, 200, 200);
    QRectF pos2 = QRectF(5,300, 400, 400);
    mEffectHandler->mSharedVideo->setGeometry(pos1);
    mEffectHandler->mReceivedVideo->setGeometry(pos2);
    QSignalSpy spy( mEffectHandler, SIGNAL(swapCompleted()) );
    sharedVideoWidget->hide();
    receivedVideoWidget->hide();
        lcutStub_LcUiEngine_setLocalPlaying( true );
    mEffectHandler->windowSwapSharedComplete( status );
    QVERIFY( !sharedVideoWidget->isVisible() );
    QCOMPARE( spy.count(), 0 );

    sharedVideoWidget->hide();
    receivedVideoWidget->hide();
        lcutStub_LcUiEngine_setRemotePlaying( true );
    mEffectHandler->windowSwapReceivedComplete( status );
    QVERIFY( receivedVideoWidget->isVisible() );
    QVERIFY( sharedVideoWidget->isVisible() );
    QCOMPARE( spy.count(), 1 );

    // Players are disabled so even when swap completes, widgets are not set
    // visible
    sharedVideoWidget->hide();
    receivedVideoWidget->hide();
        lcutStub_LcUiEngine_setLocalPlaying( false );
    mEffectHandler->windowSwapSharedComplete( status );
    QVERIFY( !sharedVideoWidget->isVisible() );
    
    sharedVideoWidget->hide();
    receivedVideoWidget->hide();
        lcutStub_LcUiEngine_setRemotePlaying( false );
    mEffectHandler->windowSwapReceivedComplete( status );
    QVERIFY( !receivedVideoWidget->isVisible() );
    QVERIFY( !sharedVideoWidget->isVisible() );
}

void UT_LcEffectHandler::testStartEffect()
{
    clearWidgets();
    
    const char *effectId = "myeffect";
    const char *callBack = "mycallback";
    
    QVERIFY( !mEffectHandler->mSharedVideo );
    QVERIFY( !mEffectHandler->mSharedVideoOverlay );
    mEffectHandler->startEffect( mEffectHandler->mSharedVideoOverlay,effectId, callBack);
    
    setWidgets();
    
    QVERIFY( mEffectHandler->mSharedVideo );
    QVERIFY( mEffectHandler->mSharedVideoOverlay );
    
    HbWidget* sharedVideoWidget = mEffectHandler->mSharedVideo;
    HbWidget* sharedVideoWidget2 = mEffectHandler->mSharedVideoOverlay;
    QRectF sharedOrig = mEffectHandler->mSharedVideo->geometry();
    
    mEffectHandler->mSharedVideo = 0;
    mEffectHandler->startEffect( mEffectHandler->mSharedVideoOverlay,effectId, callBack);
    
    mEffectHandler->mSharedVideo = sharedVideoWidget;
    mEffectHandler->mSharedVideoOverlay = 0;
    mEffectHandler->startEffect( mEffectHandler->mSharedVideoOverlay,effectId, callBack);
    
    mEffectHandler->mSharedVideo = sharedVideoWidget;
    mEffectHandler->mSharedVideoOverlay = sharedVideoWidget2;
    mEffectHandler->startEffect( mEffectHandler->mSharedVideoOverlay,effectId, callBack);
    
    QCOMPARE( sharedVideoWidget->geometry(), sharedVideoWidget2->geometry() );
}

void UT_LcEffectHandler::testSetVisibility()
{
    setWidgets();

    QVERIFY( mEffectHandler->mSharedVideo );
    HbWidget* sharedVideoWidget = mEffectHandler->mSharedVideo;
    sharedVideoWidget->setVisible(false);

    //If Effects are not running than we can set the visibility of widget
    mEffectHandler->setVisibility(sharedVideoWidget,true);
    // TODO: setvisibility temporarily NOP
    /*QVERIFY( mEffectHandler->mSharedVideo->isVisible() );

    //If Effects are running we can not set the visibility of widget.
    HbEffect::mRunning = true;
    mEffectHandler->setVisibility(sharedVideoWidget,false);
    QVERIFY( mEffectHandler->mSharedVideo->isVisible() );*/
}

void UT_LcEffectHandler::testSetDissappearEffect()
{
    QCOMPARE( mEffectHandler->mEffectCallBack,
        QString::fromAscii(lcEffectWinDefaultCallback ));
    mEffectHandler->setDissappearEffect(LcEffectHandler::DissappearToFlip );
    QCOMPARE( mEffectHandler->mEffectCallBack,
        QString::fromAscii("windowDisappearToFlipComplete" ));
    mEffectHandler->setDissappearEffect(LcEffectHandler::NormalDissappear );
    QCOMPARE( mEffectHandler->mEffectCallBack,
        QString::fromAscii("windowDisappearComplete" ));
}

void UT_LcEffectHandler::testIsSharedVideoAtTop()
{
    setWidgets();

    QRectF pos1 = QRectF(5,100, 200, 200);
    QRectF pos2 = QRectF(5,300, 400, 400);
    mEffectHandler->mSharedVideo->setGeometry(pos1);
    mEffectHandler->mReceivedVideo->setGeometry(pos2);

    QVERIFY( mEffectHandler->isSharedVideoAtTop() );

    mEffectHandler->mSharedVideo->setGeometry(pos2);
    mEffectHandler->mReceivedVideo->setGeometry(pos1);
    QVERIFY( !mEffectHandler->isSharedVideoAtTop() );

}

void UT_LcEffectHandler::testVideosSideBySide()
{
    setWidgets();
    QRectF pos1 = QRectF(5,100, 200, 200);
    QRectF pos2 = QRectF(5,300, 400, 400);
    QRectF pos3 = QRectF(5,100, 400, 400);

    mEffectHandler->mSharedVideo->setGeometry(pos1);
    mEffectHandler->mReceivedVideo->setGeometry(pos2);

    QVERIFY( !mEffectHandler->videosSideBySide() );
    mEffectHandler->mReceivedVideo->setGeometry(pos3);
    QVERIFY( mEffectHandler->videosSideBySide() );
}

void UT_LcEffectHandler::testHandleSwapCompletion()
{
    QSignalSpy spy( mEffectHandler, SIGNAL(swapCompleted()) );
    mEffectHandler->mSwapInProgress = true;
    mEffectHandler->mSwapSharedCompleted = false;
    mEffectHandler->mSwapReceivedCompleted = false;

    // Test1: Swap is not completed.
    mEffectHandler->handleSwapCompletion();
    QVERIFY( mEffectHandler->mSwapInProgress );
    QCOMPARE( spy.count(), 0 );

    // Test2: Swap is partially completed.
    mEffectHandler->mSwapSharedCompleted = true;
    mEffectHandler->handleSwapCompletion();
    QVERIFY( mEffectHandler->mSwapInProgress );
    QCOMPARE( spy.count(), 0 );

    // Test 3. Swap Completed completely.
    mEffectHandler->mSwapSharedCompleted = true;
    mEffectHandler->mSwapReceivedCompleted = true;
    mEffectHandler->handleSwapCompletion();
    QVERIFY( !mEffectHandler->mSwapInProgress );
    QCOMPARE( spy.count(), 1 );

    //Test 4. Local & Remote are player are availble.
    lcutStub_LcUiEngine_setLocalPlaying( true );
    lcutStub_LcUiEngine_setRemotePlaying( true );

    mEffectHandler->handleSwapCompletion();
    QVERIFY( mEffectHandler->mSharedVideo->isVisible() );
    QVERIFY( mEffectHandler->mReceivedVideo->isVisible() );
}

void UT_LcEffectHandler::testIsSwapInProgress()
{
    QVERIFY( !mEffectHandler->isSwapInProgress() );
    mEffectHandler->swap();
    QVERIFY( mEffectHandler->isSwapInProgress() );
}


// Helper Utility Function for the Unit Test.
void UT_LcEffectHandler::clearWidgets()
{
    mEffectHandler->mReceivedVideo = 0;
    mEffectHandler->mReceivedVideoOverlay = 0;
    mEffectHandler->mSharedVideo = 0;
    mEffectHandler->mSharedVideoOverlay = 0;
}

void UT_LcEffectHandler::setWidgets()
{
    mEffectHandler->mReceivedVideo = mReceivedVideo;
    mEffectHandler->mReceivedVideoOverlay = mReceivedVideoOverlay;
    mEffectHandler->mSharedVideo = mSharedVideo;
    mEffectHandler->mSharedVideoOverlay = mSharedVideoOverlay;
}



