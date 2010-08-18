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
#ifndef UT_LCUIENGINE_H
#define UT_LCUIENGINE_H

#include <QObject>
class LcUiEngine;
class HbProgressDialog;
class HbDialog;
class HbMainWindow;
class LcSignalSink;


class UT_LcUiEngine : public QObject                 
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
    void testFeatureSupported();
    void testEstablishSession();
    void testTerminateSession();
    void testSetWindowRect();
    void testEnableWindow();
    void testPlay();
    void testPause();
    void testEnableSpeaker();
    void testMuteMic();
    void testStartLiveContent();
    void testStart();
    void testSetContentAreas();
    void testSetOrientation();
    void testToggleMute();
    void testToggleSpeaker();
    void testToggleDisableCamera();
    void testToggleCamera();
    void testSessionStateChanged();
    void testPlayerStateChanged();
    void testStartReceiving();
    void testStartPlayback();
    void testStop();
    void testSetViewLayout();
    void testHideNotes();
    void testSetUiComponents();
    void testZoom();
    void testBlockUi();
    void testCloseTimer();
    void testfullScreenMode();
    void testsetFullScreenMode();
    void testTimerEvent();
    void testFillRemoteInfo();
    void testUpdateSession();
    void testIsLocalPlayerPlaying();
    void testIsRemotePlayerPlaying();
    void testMinimized();
    void testMaximized();    
    void testIsMinimized();
    void testHandleForegroundStatus();
    void testSendDialTone();
    void testVtVideoSendingSetting(); 
    void testShareVideoIsAllowedShownAutomatically(); 
    void testShareVideoIsNotAllowedShown();
    void testShareVideoPermissionAskedAlways(); 
    
private: // utility functions
 
    bool setVtVideoSendingSetting(int key);

private:
    
    LcUiEngine* mEngine;
    HbProgressDialog* mInvitingNote;
    HbProgressDialog* mWaitingNote;
    HbDialog* mAcceptQuery;
    HbDialog* mRecipientQuery;
    HbDialog* mShareOwnVideoQuery;
};




#endif //UT_LCUIENGINE_H
