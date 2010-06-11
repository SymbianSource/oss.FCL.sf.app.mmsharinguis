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


#ifndef UT_LCVIEW_H
#define UT_LCVIEW_H

#include <QObject>

class LcView;
class LcUiEngine;
class HbLabel;
class LcUiComponentRepository;
class HbAction;
class HbPushButton;
class HbMainWindow;

class UT_LcView : public QObject
{
    Q_OBJECT
     
private slots:

/*
 * In addition, there are four private slots that are not treated as testfunctions. 
 * They will be executed by the testing framework and can be used to initialize and clean up 
 * either the entire test or the current test function.
 * 
 * initTestCase() will be called before the first testfunction is executed.
 * cleanupTestCase() will be called after the last testfunction was executed.
 * init() will be called before each testfunction is executed.
 * cleanup() will be called after every testfunction.
*/
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();


private slots: //test methods
     
    void testConstructor();
    void testActivated();
    void testInit();
    void testDeactivated();
    void testEndVideoSession();
    void testMute();
    void testChangeCamera();    
    void testSwitchToVoiceCall();    
    void testDisableCamera();    
    void testSpeaker();
    void testSwap();
    void testShowZoom();
    void testNotSupported();
    void testSetCameraActionToMain();
    void testSetCameraActionToSecondary();
    void testSetMuteActionToUnmute();
    void testSetMuteActionToMute();
    void testSetSpeakerActionToHandset();
    void testSetSpeakerActionToSpeaker();
    void testSetCameraActionToEnable();
    void testSetCameraActionToDisable();
    void testDisableControls();
    void testEnableControls();
    void testUpdateVideoRects();
    void testCurrentLayout();
    void testActivateFullScreen();
    void testDeactivateFullScreen();
    void testGestureEvent();
    void testContextMenu();
    void testGestureLongPress();
    void testGestureShortPress();
    void testActivatePortrait();
    void testUpdateUiElements();
    void testLandscapeTimerTimeout();
    void testShareImage();
    void testShowControl();
    void testHideControl();
    void testUpdateSwapLayout();
    void testMenuAboutToShow();
    void testMenuAboutToHide();
    void testTranslateRectForOrientation();
    void testTranslatePointForOrientation();
    void testResetLandscapeTimer();
    void testOpenDialpad();
    void testDialpadOpened();
    void testDialpadClosed();
    void testDialpadEditorTextChanged();
    void testBack();
    void testAddOptionsMenuActions();

private:
 
    HbMainWindow* mMainWindow;   
    LcView* mView;
    LcUiEngine* mEngine;
    HbLabel* mRecipient;
    LcUiComponentRepository* mRepository;
    HbAction* mAction;
    HbAction* mAction2;
    HbPushButton* mEndCallButton;
};


#endif //UT_LCVIEW_H
