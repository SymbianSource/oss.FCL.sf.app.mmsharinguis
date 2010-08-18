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
#include "ut_lcuiengine.h"
#include "lcuiengine.h"
#include "lcuiengine_p.h"
#include "lcactivitymanager.h"

#include "lcenginestub.h"
#include "lcsessionstub.h"
#include "lcvideoplayerstub.h"
#include "lcuidefs.h"
#include "qthighway_stub_helper.h"

#include <QtTest/QtTest>
#include <QMap>

#include <hbmainwindow.h>
#include <hbdialog.h>
#include <hbprogressdialog.h>
#include <hbmessagebox.h>
#include <hbaction.h>
#include <hblabel.h>
#include <QSignalSpy>
#include <hbview.h>

#include <xqsettingsmanager.h>
#include <xqsettingskey.h>
#include <settingsinternalcrkeys.h>

const char lcutMultimediaSharingEng[] = "MultimediaSharing";
const char lcutVideotelephonyEng[] = "Videotelephony";

const char lcutMultimediaSharingApp[] = "Multimedia Sharing";
const char lcutVideotelephonyApp[] = "Videotelephony";

#define LC_ENGINE( uiengine )\
    (static_cast<CLcEngineStub*>( uiengine->d->mLiveCommsEngine ) )

#define LC_SESSION( uiengine )\
    (LC_ENGINE(uiengine)->iSession)

void UT_LcUiEngine::initTestCase()
{
    lcutStub_LcEngine_reset();
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();

}

void UT_LcUiEngine::cleanupTestCase()
{
    lcutStub_LcEngine_reset();
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
}


void UT_LcUiEngine::init()
{
    lcutStub_LcEngine_reset();
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    mEngine = new LcUiEngine( lcutMultimediaSharingEng,
                              lcutMultimediaSharingApp );
    mInvitingNote = new HbProgressDialog();
    mWaitingNote = new HbProgressDialog();
    mAcceptQuery = new HbDialog();
    mRecipientQuery = new HbDialog();
    mShareOwnVideoQuery = new HbDialog();
    
}

void UT_LcUiEngine::cleanup()
{
    lcutStub_LcEngine_reset();
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    delete mEngine;
    delete mInvitingNote;
    delete mWaitingNote;
    delete mAcceptQuery;
    delete mRecipientQuery;
    delete mShareOwnVideoQuery;
}

void UT_LcUiEngine::testConstructor()
{
    // "MuS plug-in" loaded
    QVERIFY( mEngine );
    QCOMPARE( mEngine->d->mViewLayout, Lc_IdleViewLayout );
    QVERIFY( !mEngine->d->mInvitingNote );
    QVERIFY( !mEngine->d->mWaitingNote );
    QVERIFY( !mEngine->d->mAcceptQuery );
    QVERIFY( !mEngine->d->mRecipientQuery );
    QVERIFY( !mEngine->d->mCloseTimer );
    QVERIFY( mEngine->d->mLiveCommsEngine );
    QCOMPARE( mEngine->d->mEngineName, QByteArray( lcutMultimediaSharingEng ) );
    QVERIFY( mEngine->d->recipient() != hbTrId("txt_vt_custom_unknown_number") );
    QCOMPARE( mEngine->d->sessionTitle(), QString( lcutMultimediaSharingApp ) );
    int engineWindowParam = 0;
    QVERIFY( LC_SESSION( mEngine )->mParams.contains( engineWindowParam ) );
    QVERIFY( mEngine->d->mActivityManager );
    QVERIFY( !mEngine->d->mActivityManager->isActivitySimulationEnabled() );
    QVERIFY( !mEngine->d->mShareOwnVideoQuery );
    
    // "VT plug-in" loaded
    lcutStub_LcEngine_reset();
    lcutStub_LcSession_reset();    
    delete mEngine;
    mEngine = 0;
    mEngine = new LcUiEngine( lcutVideotelephonyEng,
                              lcutVideotelephonyApp );
    QVERIFY( mEngine );
    QCOMPARE( mEngine->d->mViewLayout, Lc_AllInOneViewLayout );
    QVERIFY( !mEngine->d->mInvitingNote );
    QVERIFY( !mEngine->d->mWaitingNote );
    QVERIFY( !mEngine->d->mAcceptQuery );
    QVERIFY( !mEngine->d->mRecipientQuery );
    QVERIFY( !mEngine->d->mCloseTimer );
    QVERIFY( mEngine->d->mLiveCommsEngine );
    QCOMPARE( mEngine->d->mEngineName, QByteArray( lcutVideotelephonyEng ) );
    QVERIFY( mEngine->d->recipient() != hbTrId("txt_vt_custom_unknown_number") );
    QCOMPARE( mEngine->d->sessionTitle(), QString( lcutVideotelephonyApp ) );
    QVERIFY( LC_SESSION( mEngine )->mParams.contains( engineWindowParam ) );
    QVERIFY( !mEngine->d->mFullScreen );
    QVERIFY( mEngine->d->mActivityManager );
    QVERIFY( !mEngine->d->mActivityManager->isActivitySimulationEnabled() );
    QVERIFY( !mEngine->d->mShareOwnVideoQuery );
}

void UT_LcUiEngine::testFeatureSupported()
{
    // Engine not loaded
    CLcEngine* enginePtr = mEngine->d->mLiveCommsEngine;
    mEngine->d->mLiveCommsEngine = 0;
    QVERIFY( !mEngine->d->featureSupported( CLcEngine::ELcShowAcceptQuery ) );
    
    // Engine loaded, feature not supported
    mEngine->d->mLiveCommsEngine = enginePtr;
    QVERIFY( !mEngine->d->featureSupported( CLcEngine::ELcShowAcceptQuery ) );
    
    // Feature supported
    lcutStub_LcEngine_setLcFeatureSupported( true );
    QVERIFY( mEngine->d->featureSupported( CLcEngine::ELcShowAcceptQuery ) );
}

void UT_LcUiEngine::testEstablishSession()
{
    // Leave 
    lcutStub_LcEngine_setFailure( -1 );
    QCOMPARE( mEngine->d->establishSession(), -1 );
    QVERIFY( !mEngine->d->mCloseTimer );
    
    // No leave
    lcutStub_LcEngine_reset();
    QCOMPARE( mEngine->d->establishSession(), 0 );
    QVERIFY( mEngine->d->mCloseTimer );
}

void UT_LcUiEngine::testTerminateSession()
{
    // Leave 
    lcutStub_LcEngine_setFailure( -1 );
    QCOMPARE( mEngine->d->terminateSession(), -1 );
    
    // No leave
    lcutStub_LcEngine_reset();
    QCOMPARE( mEngine->d->establishSession(), 0 );
}

void UT_LcUiEngine::testSetWindowRect()
{
    TRect rect;
    MLcVideoPlayer* player = 
        mEngine->d->mLiveCommsEngine->Session().LocalVideoPlayer();
    
    // No player
    QCOMPARE( mEngine->d->setWindowRect( 0, rect ), -1 );
    
    // Leave
    lcutStub_LcEngine_setFailure( -2 );
    QCOMPARE( mEngine->d->setWindowRect( player, rect ), -2 );    
    
    // No leave
    lcutStub_LcEngine_reset();
    QCOMPARE( mEngine->d->setWindowRect( player, rect ), 0 );
}

void UT_LcUiEngine::testEnableWindow()
{
    MLcVideoPlayer* player = 
        mEngine->d->mLiveCommsEngine->Session().LocalVideoPlayer();
    
    // No player
    QCOMPARE( mEngine->d->enableWindow( 0, true ), -1 );
    
    // Leave
    lcutStub_LcEngine_setFailure( -2 );
    QCOMPARE( mEngine->d->enableWindow( player, true ), -2 );    
    
    // No leave
    lcutStub_LcEngine_reset();
    QCOMPARE( mEngine->d->enableWindow( player, true ), 0 );
}

void UT_LcUiEngine::testPlay()
{
    MLcVideoPlayer* player = 
        mEngine->d->mLiveCommsEngine->Session().LocalVideoPlayer();
    
    // No player
    QCOMPARE( mEngine->d->play( 0 ), -1 );
    
    // Leave
    lcutStub_LcEngine_setFailure( -2 );
    QCOMPARE( mEngine->d->play( player ), -2 );    
    
    // No leave
    lcutStub_LcEngine_reset();
    QVERIFY( !player->LcIsPlayingL() );
    QCOMPARE( mEngine->d->play( player ), 0 );
    QVERIFY( player->LcIsPlayingL() );
}

void UT_LcUiEngine::testPause()
{
    MLcVideoPlayer* player = 
        mEngine->d->mLiveCommsEngine->Session().LocalVideoPlayer();
    
    // No player
    QCOMPARE( mEngine->d->pause( 0 ), -1 );
    
    // Leave
    lcutStub_LcEngine_setFailure( -2 );
    QCOMPARE( mEngine->d->pause( player ), -2 );    
    
    // No leave
    lcutStub_LcEngine_reset();
    QCOMPARE( mEngine->d->play( player ), 0 );
    QVERIFY( player->LcIsPlayingL() );
    QCOMPARE( mEngine->d->pause( player ), 0 );
    QVERIFY( !player->LcIsPlayingL() );    
}

void UT_LcUiEngine::testEnableSpeaker()
{
    // Leave 
    lcutStub_LcEngine_setFailure( -1 );
    QCOMPARE( mEngine->d->enableSpeaker( true ), -1 );
    QVERIFY( !mEngine->d->isSpeakerOn() );
    
    // No leave
    lcutStub_LcEngine_reset();
    QCOMPARE( mEngine->d->enableSpeaker( true ), 0 );
    QVERIFY( mEngine->d->isSpeakerOn() );
}

void UT_LcUiEngine::testMuteMic()
{
    // Leave 
    lcutStub_LcEngine_setFailure( -1 );
    QCOMPARE( mEngine->d->muteMic( true ), -1 );
    lcutStub_LcEngine_reset();
    QVERIFY( !mEngine->d->isMuted() );
    
    // No leave
    QCOMPARE( mEngine->d->muteMic( true ), 0 );
    QVERIFY( mEngine->d->isMuted() );
}

void UT_LcUiEngine::testStartLiveContent()
{
    // mInvitingNote not set
    mInvitingNote->hide();
    mEngine->d->startLiveContent();
    QVERIFY( !mInvitingNote->isVisible() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_establishSession ) );
    
    // mInvitingNote set, but the engine does not support the feature
    lcutStub_LcSession_reset();
    mEngine->d->mInvitingNote = mInvitingNote;
    mEngine->d->startLiveContent();
    QVERIFY( !mInvitingNote->isVisible() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_establishSession ) );    
    
    // mInvitingNote set and the engine supports the feature
    lcutStub_LcSession_reset();
    lcutStub_LcEngine_setLcFeatureSupported( true );
    mEngine->d->startLiveContent();
    QVERIFY( mInvitingNote->isVisible() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_establishSession ) ); 
}

void UT_LcUiEngine::testStart()
{    
    LC_SESSION( mEngine )->iState = MLcSession::EReceived;
    mEngine->d->start();
    QVERIFY( mEngine->d->mActivityManager->isActivitySimulationEnabled() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_NoCall ) );
    
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    
    LC_SESSION( mEngine )->iState = MLcSession::EInitialized;
    mEngine->d->start();
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_establishSession ) );

    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    
    mEngine->d->setUiComponents( mInvitingNote, mWaitingNote,
                              mAcceptQuery, mRecipientQuery,mShareOwnVideoQuery );
    LC_SESSION( mEngine )->iState = MLcSession::EInitialized;
    mEngine->d->start();
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_establishSession ) );
}
    
void UT_LcUiEngine::testSetUiComponents()
{
    QVERIFY( !mEngine->d->mInvitingNote );
    QVERIFY( !mEngine->d->mWaitingNote );
    QVERIFY( !mEngine->d->mAcceptQuery );
    QVERIFY( !mEngine->d->mRecipientQuery );
    
    mEngine->d->setUiComponents(0,0,0,0,0);
    
    QVERIFY( !mEngine->d->mInvitingNote );
    QVERIFY( !mEngine->d->mWaitingNote );
    QVERIFY( !mEngine->d->mAcceptQuery );
    QVERIFY( !mEngine->d->mRecipientQuery );
    
    mEngine->d->setUiComponents(mInvitingNote,mWaitingNote,mAcceptQuery,mRecipientQuery,mShareOwnVideoQuery);
    QVERIFY( mEngine->d->mInvitingNote );
    QVERIFY( mEngine->d->mWaitingNote );
    QVERIFY( mEngine->d->mAcceptQuery );
    QVERIFY( mEngine->d->mRecipientQuery );
}

void UT_LcUiEngine::testSessionStateChanged()
{        
    LC_SESSION( mEngine )->iState = MLcSession::EUninitialized;
    mEngine->d->StateChanged( *LC_SESSION( mEngine ) );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_NoCall ) );
        
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    
    QVERIFY( mEngine->d->mSessionDurationTimerId == 0 );
    LC_SESSION( mEngine )->iState = MLcSession::EOpen;
    mEngine->d->StateChanged( *LC_SESSION( mEngine ) );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_NoCall ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_EnableLcWindowL ) );
    // Session duration is started when session opened
    QVERIFY( mEngine->d->mSessionDurationTimerId != 0 );

    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    
    LC_SESSION( mEngine )->iState = MLcSession::EClosed;
    QSignalSpy spy(mEngine, SIGNAL(stopped()));
    mEngine->d->StateChanged( *LC_SESSION( mEngine ) );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_NoCall ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    QCOMPARE( spy.count(), 1 );
    
    // mAcceptQuery not set
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();    
    mAcceptQuery->hide();
    LC_SESSION( mEngine )->iState = MLcSession::EReceived;
    mEngine->d->StateChanged( *LC_SESSION( mEngine ) );
    QVERIFY( !mAcceptQuery->isVisible() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_establishSession ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    
    // mAcceptQuery set, but the engine does not support the feature
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    LC_SESSION( mEngine )->iState = MLcSession::EReceived;
    mEngine->d->mAcceptQuery = mAcceptQuery;
    mEngine->d->StateChanged( *LC_SESSION( mEngine ) );
    QVERIFY( !mAcceptQuery->isVisible() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_establishSession ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    
    // mAcceptQuery set and the engine supports the feature
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    LC_SESSION( mEngine )->iState = MLcSession::EReceived;
    lcutStub_LcEngine_setLcFeatureSupported( true );
    mEngine->d->StateChanged( *LC_SESSION( mEngine ) );
    QVERIFY( mAcceptQuery->isVisible() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_NoCall ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );    
    
    // MT call and sharing own video not allowed by default , popup query shown    
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    mShareOwnVideoQuery->hide();
    mEngine->d->setUiComponents(mInvitingNote,mWaitingNote,mAcceptQuery,mRecipientQuery,mShareOwnVideoQuery);
    LC_SESSION( mEngine )->iState = MLcSession::EOpen;
    lcutStub_LcEngine_setLcFeatureSupported( true );
    mEngine->d->StateChanged( *LC_SESSION( mEngine ) );
    QVERIFY( mEngine->d->mShareOwnVideoQuery );
    QVERIFY( mEngine->d->mShareOwnVideoQuery->isVisible() );
    CLcVideoPlayer* localPlayer = LC_SESSION( mEngine )->iLocalPlayer;
    //QVERIFY( localPlayer->iState == MLcVideoPlayer::EPaused );
    
    // MO call and sharing own video allowed by default , popup query not shown
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    mShareOwnVideoQuery->hide();
    mEngine->d->setUiComponents(mInvitingNote,mWaitingNote,mAcceptQuery,mRecipientQuery,mShareOwnVideoQuery);
    LC_SESSION( mEngine )->iState = MLcSession::EOpen;
    lcutStub_LcEngine_setLcFeatureSupported( false );
    mEngine->d->StateChanged( *LC_SESSION( mEngine ) );    
    QVERIFY( mEngine->d->mShareOwnVideoQuery );
    QVERIFY( !mEngine->d->mShareOwnVideoQuery->isVisible() );
    localPlayer = LC_SESSION( mEngine )->iLocalPlayer;
    QVERIFY( localPlayer->iState == MLcVideoPlayer::EPlaying );
        
}

void UT_LcUiEngine::testPlayerStateChanged()
{
    CLcVideoPlayer* localPlayer = LC_SESSION( mEngine )->iLocalPlayer;
    QVERIFY( localPlayer != NULL );
    QSignalSpy localPlayerSignalSpy( mEngine, SIGNAL( localPlayerPlaying() ) );
    QSignalSpy localPlayerSignalSpy2( mEngine, SIGNAL( localPlayerPreparing() ) );
    QSignalSpy localPlayerSignalSpy3( mEngine, SIGNAL( localPlayerBuffering() ) );
    QSignalSpy localPlayerSignalSpy4( mEngine, SIGNAL( localPlayerPaused() ) );
    QSignalSpy localPlayerSignalSpy5( mEngine, SIGNAL( localPlayerUnavailable() ) );
    
    CLcVideoPlayer* remotePlayer = LC_SESSION( mEngine )->iRemotePlayer;
    QVERIFY( remotePlayer != NULL );
    QSignalSpy remotePlayerSignalSpy( mEngine, SIGNAL( remotePlayerPlaying() ) );
    QSignalSpy remotePlayerSignalSpy2( mEngine, SIGNAL( remotePlayerPreparing() ) );
    QSignalSpy remotePlayerSignalSpy3( mEngine, SIGNAL( remotePlayerBuffering() ) );
    QSignalSpy remotePlayerSignalSpy4( mEngine, SIGNAL( remotePlayerPaused() ) );
    QSignalSpy remotePlayerSignalSpy5( mEngine, SIGNAL( remotePlayerUnavailable() ) );

    // Use local player, player not playing
    mEngine->d->StateChanged( *localPlayer );
    QCOMPARE( localPlayerSignalSpy.count(), 0 );
    QCOMPARE( localPlayerSignalSpy2.count(), 0 );
    QCOMPARE( localPlayerSignalSpy3.count(), 0 );
    QCOMPARE( localPlayerSignalSpy4.count(), 0 );
    QCOMPARE( localPlayerSignalSpy5.count(), 0 );
    
    // Use local player, player preparing
    localPlayer->iState = MLcVideoPlayer::EPreparing;
    mEngine->d->StateChanged( *localPlayer );
    QCOMPARE( localPlayerSignalSpy2.count(), 1 );

    // Use local player, player buffering
    localPlayer->iState = MLcVideoPlayer::EBuffering;
    mEngine->d->StateChanged( *localPlayer );
    QCOMPARE( localPlayerSignalSpy3.count(), 1 );

    // Use local player, player playing
    localPlayer->iState = MLcVideoPlayer::EPlaying;
    mEngine->d->StateChanged( *localPlayer );
    QCOMPARE( localPlayerSignalSpy.count(), 1 );
    
    // Use local player, player paused
    localPlayer->iState = MLcVideoPlayer::EPaused;
    mEngine->d->StateChanged( *localPlayer );
    QCOMPARE( localPlayerSignalSpy4.count(), 1 );
    
    // Use local player, player unavailable
    localPlayer->iState = MLcVideoPlayer::EUnavailable;
    mEngine->d->StateChanged( *localPlayer );
    QCOMPARE( localPlayerSignalSpy5.count(), 1 );
    
    // Use remote player, no waiting note
    mEngine->d->StateChanged( *remotePlayer );
    QVERIFY( mWaitingNote->isVisible() );
    QCOMPARE( remotePlayerSignalSpy.count(), 0 );
    QCOMPARE( remotePlayerSignalSpy2.count(), 0 );  
    QCOMPARE( remotePlayerSignalSpy3.count(), 0 );  
    QCOMPARE( remotePlayerSignalSpy4.count(), 0 );  
    QCOMPARE( remotePlayerSignalSpy5.count(), 0 );  

    // Use remote player, waiting note present, player not playing
    mEngine->d->mWaitingNote = mWaitingNote;
    mEngine->d->StateChanged( *remotePlayer );
    QVERIFY( mWaitingNote->isVisible() );
    QCOMPARE( remotePlayerSignalSpy.count(), 0 );
    QCOMPARE( remotePlayerSignalSpy2.count(), 0 );  
    QCOMPARE( remotePlayerSignalSpy3.count(), 0 );  
    QCOMPARE( remotePlayerSignalSpy4.count(), 0 );  
    QCOMPARE( remotePlayerSignalSpy5.count(), 0 );  

    // Use remote player, player preparing
    remotePlayer->iState = MLcVideoPlayer::EPreparing;
    mEngine->d->StateChanged( *remotePlayer );
    QVERIFY( mWaitingNote->isVisible() );
    QCOMPARE( remotePlayerSignalSpy2.count(), 1 );  
    
    // Use remote player, player buffering
    remotePlayer->iState = MLcVideoPlayer::EBuffering;
    mEngine->d->StateChanged( *remotePlayer );
    QVERIFY( mWaitingNote->isVisible() );
    QCOMPARE( remotePlayerSignalSpy3.count(), 1 );  

    // Use remote player, waiting note present, player playing
    remotePlayer->iState = MLcVideoPlayer::EPlaying;
    mEngine->d->StateChanged( *remotePlayer );
    QVERIFY( !mWaitingNote->isVisible() );
    QCOMPARE( remotePlayerSignalSpy.count(), 1 );  

    // Use remote player, waiting note present, player playing
    remotePlayer->iState = MLcVideoPlayer::EUnavailable;
    mEngine->d->StateChanged( *remotePlayer );
    QVERIFY( !mWaitingNote->isVisible() );
    QCOMPARE( remotePlayerSignalSpy5.count(), 1 );  

    // Use remote player, player paused
    remotePlayer->iState = MLcVideoPlayer::EPaused;
    mEngine->d->StateChanged( *remotePlayer );
    QVERIFY( !mWaitingNote->isVisible() );
    QCOMPARE( remotePlayerSignalSpy4.count(), 1 );  

    QCOMPARE( localPlayerSignalSpy.count(), 1 );
    QCOMPARE( localPlayerSignalSpy2.count(), 1 );
    QCOMPARE( localPlayerSignalSpy3.count(), 1 );
    QCOMPARE( localPlayerSignalSpy4.count(), 1 );
    QCOMPARE( localPlayerSignalSpy5.count(), 1 );

    QCOMPARE( remotePlayerSignalSpy.count(), 1 );
    QCOMPARE( remotePlayerSignalSpy2.count(), 1 );  
    QCOMPARE( remotePlayerSignalSpy3.count(), 1 );  
    QCOMPARE( remotePlayerSignalSpy4.count(), 1 );  
    QCOMPARE( remotePlayerSignalSpy5.count(), 1 );  
    
}

void UT_LcUiEngine::testSetContentAreas()
{
    QRectF shared;
    QRectF received;
    
    QVERIFY( LC_SESSION( mEngine )->iLocalPlayer );
    QVERIFY( LC_SESSION( mEngine )->iRemotePlayer );
    QVERIFY( LC_SESSION( mEngine )->iLocalPlayer->iRect.iTl.iX != 0);
    QVERIFY( LC_SESSION( mEngine )->iRemotePlayer->iRect.iTl.iX != 0);
    
    mEngine->d->setContentAreas( shared, received );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_SetLcWindowRectL ) );
    QVERIFY( LC_SESSION( mEngine )->iLocalPlayer->iRect.iTl.iX == 0);
    QVERIFY( LC_SESSION( mEngine )->iRemotePlayer->iRect.iTl.iX == 0);
    
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    
    mEngine->d->setContentAreas( shared, received );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    
    shared.setHeight(100);
    received.setHeight(100);
    
    mEngine->d->setContentAreas( shared, received );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_SetLcWindowRectL ) );
    
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    
    delete LC_SESSION( mEngine )->iLocalPlayer;
    LC_SESSION( mEngine )->iLocalPlayer = 0;
    delete LC_SESSION( mEngine )->iRemotePlayer;
    LC_SESSION( mEngine )->iRemotePlayer = 0;
    
    mEngine->d->setContentAreas( shared, received );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
}

void UT_LcUiEngine::testSetOrientation()
{
    bool ok( false );

    //1. setting landscape
    QVERIFY( LC_SESSION( mEngine )->iLocalPlayer );
    QVERIFY( LC_SESSION( mEngine )->iRemotePlayer );

    ok = mEngine->d->setOrientation( Qt::Horizontal );
    QVERIFY( ok );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_SetLcWindowOrientationL ) );
    QVERIFY( LC_SESSION( mEngine )->iLocalPlayer->iOrientation == MLcWindow::ELandscape );
    QVERIFY( LC_SESSION( mEngine )->iRemotePlayer->iOrientation == MLcWindow::ELandscape );

    //2. setting layout failed
    // Leave
    lcutStub_LcEngine_setFailure( -1 );
    lcutStub_LcPlayer_reset();
    ok = mEngine->d->setOrientation( Qt::Horizontal );
    QVERIFY( !ok );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    
    //3. setting layout with no players
    lcutStub_LcEngine_reset();
    lcutStub_LcPlayer_reset();
    delete LC_SESSION( mEngine )->iLocalPlayer;
    LC_SESSION( mEngine )->iLocalPlayer = 0;
    delete LC_SESSION( mEngine )->iRemotePlayer;
    LC_SESSION( mEngine )->iRemotePlayer = 0;
    
    ok = mEngine->d->setOrientation( Qt::Horizontal );
    QVERIFY( ok );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );    
}


void UT_LcUiEngine::testToggleMute()
{
    QSignalSpy mutedSpy(mEngine, SIGNAL(muted()));
    QSignalSpy unmutedSpy(mEngine, SIGNAL(unmuted()));
    
    
    // Leave from isMuted
    mEngine->d->toggleMute();
    QVERIFY( mEngine->d->isMuted() );
    lcutStub_LcEngine_setFailure( -2 );
    QVERIFY( !mEngine->d->isMuted() );
    
    // No leave
    lcutStub_LcEngine_reset();
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    mutedSpy.clear();
    unmutedSpy.clear();
    
    mEngine->d->toggleMute();
    QVERIFY( !mEngine->d->isMuted() );
    mutedSpy.clear();
    unmutedSpy.clear();
    
    mEngine->d->toggleMute();
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_MuteLcMicL ) );
    QVERIFY( mEngine->d->isMuted() );
    QCOMPARE(mutedSpy.count(), 1); // make sure the signal was emitted exactly one time
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    mutedSpy.clear();
    unmutedSpy.clear();
    
    mEngine->d->toggleMute();
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_MuteLcMicL ) );
    QVERIFY( !mEngine->d->isMuted() );
    QCOMPARE(unmutedSpy.count(), 1); // make sure the signal was emitted exactly one time
    
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    mutedSpy.clear();
    unmutedSpy.clear();
    
    // test async mute
    lcutStub_LcPlayer_asyncCompletion( true );
    mEngine->d->toggleMute();
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_MuteLcMicL ) );
    QVERIFY( mEngine->d->isMuted() );
    QVERIFY( mEngine->d->mBlocked );
    QVERIFY( mEngine->d->mPendingAction == "muted" );
    
    // nothing yet signaled as completion is pending
    QCOMPARE(mutedSpy.count(), 0);
    QCOMPARE(unmutedSpy.count(), 0);
    
    
    lcutStub_LcPlayer_asyncCompletion( false );
}

void UT_LcUiEngine::testToggleSpeaker()
{
    QSignalSpy speakerEnabledSpy(mEngine, SIGNAL(speakerEnabled()));
    QSignalSpy speakerDisabledSpy(mEngine, SIGNAL(speakerDisabled()));

    QVERIFY( !mEngine->d->isSpeakerOn() );

    mEngine->d->toggleSpeaker();
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_EnableLcLoudspeakerL ) );
    QVERIFY( mEngine->d->isSpeakerOn() );
    QCOMPARE(speakerEnabledSpy.count(), 1);

    
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    speakerEnabledSpy.clear();
    speakerDisabledSpy.clear();
    
    mEngine->d->toggleSpeaker();
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_EnableLcLoudspeakerL ) );
    QVERIFY( !mEngine->d->isSpeakerOn() );
    QCOMPARE(speakerDisabledSpy.count(), 1);
    
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    speakerEnabledSpy.clear();
    speakerDisabledSpy.clear();
    
    // test asynch speaker functionality.
    
    lcutStub_LcPlayer_asyncCompletion( true );
    mEngine->d->toggleSpeaker();
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_EnableLcLoudspeakerL ) );
    QVERIFY( mEngine->d->isSpeakerOn() );
    QVERIFY( mEngine->d->mBlocked );
    QVERIFY( mEngine->d->mPendingAction == "speakerEnabled" );
    
    // nothing yet signaled as completion is pending
    QCOMPARE(speakerEnabledSpy.count(), 0);
    QCOMPARE(speakerDisabledSpy.count(), 0);
    
    lcutStub_LcPlayer_asyncCompletion( false );
}

void UT_LcUiEngine::testToggleDisableCamera()
{   
    QSignalSpy enabledSpy(mEngine, SIGNAL(cameraEnabled()));
    QSignalSpy disabledSpy(mEngine, SIGNAL(cameraDisabled()));

    QVERIFY( LC_SESSION( mEngine )->iLocalPlayer );
    LC_SESSION( mEngine )->iLocalPlayer->iPlaying = ETrue;    
    LC_SESSION( mEngine )->iRemotePlayer->iState = MLcVideoPlayer::EPlaying;
    QVERIFY( LC_SESSION( mEngine )->iLocalPlayer->iPlaying );
        
    mEngine->d->toggleDisableCamera();
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_LcPauseL ) );
    QVERIFY( !LC_SESSION( mEngine )->iLocalPlayer->iPlaying );
    QVERIFY( LC_SESSION( mEngine )->iLocalPlayer->iState == MLcVideoPlayer::EPaused );
    QCOMPARE(enabledSpy.count(), 0);
    QCOMPARE(disabledSpy.count(), 1);

    mEngine->d->toggleDisableCamera();
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_LcPlayL ) );   
    QVERIFY( LC_SESSION( mEngine )->iLocalPlayer->iPlaying );
    QVERIFY( LC_SESSION( mEngine )->iLocalPlayer->iState == MLcVideoPlayer::EPlaying );
    QCOMPARE(enabledSpy.count(), 1);
    QCOMPARE(disabledSpy.count(), 1);
}

void UT_LcUiEngine::testToggleCamera()
{
    // Leave
    lcutStub_LcEngine_setFailure( -1 );
    mEngine->d->toggleCamera();
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    
    // No leave
    lcutStub_LcEngine_reset();
    mEngine->d->toggleCamera();
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_ToggleLcCameraL ) );
}

void UT_LcUiEngine::testStartReceiving()
{
    // mAcceptQuery not set, mWaitingNote not set
    mAcceptQuery->show();
    mWaitingNote->hide();
    mEngine->d->startReceiving();
    QVERIFY( mAcceptQuery->isVisible() );
    QVERIFY( !mWaitingNote->isVisible() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_establishSession ) );    
    
    // mAcceptQuery set, mWaitingNote not set
    lcutStub_LcSession_reset();
    mEngine->d->mAcceptQuery = mAcceptQuery;
    mEngine->d->startReceiving();
    QVERIFY( !mAcceptQuery->isVisible() );
    QVERIFY( !mWaitingNote->isVisible() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_establishSession ) );
    
    // mWaitingNote set, but the engine does not support the feature
    lcutStub_LcSession_reset();
    mEngine->d->mWaitingNote = mWaitingNote;
    mEngine->d->startReceiving();
    QVERIFY( !mAcceptQuery->isVisible() );
    QVERIFY( !mWaitingNote->isVisible() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_establishSession ) );    
    
    // mWaitingNote set and the engine supports the feature
    lcutStub_LcSession_reset();
    lcutStub_LcEngine_setLcFeatureSupported( true );
    mEngine->d->startReceiving();
    QVERIFY( !mAcceptQuery->isVisible() );
    QVERIFY( mWaitingNote->isVisible() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_establishSession ) );
}

void UT_LcUiEngine::testStartPlayback()
{   
    mEngine->d->startPlayback();
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_NoCall ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_EnableLcWindowL,0 ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_EnableLcWindowL,1 ) );
    
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    
    delete LC_SESSION( mEngine )->iLocalPlayer;
    LC_SESSION( mEngine )->iLocalPlayer = 0;
    mEngine->d->startPlayback();
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_NoCall ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_EnableLcWindowL,0 ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall,1 ) );
    
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    
    delete LC_SESSION( mEngine )->iRemotePlayer;
    LC_SESSION( mEngine )->iRemotePlayer = 0;
    
    mEngine->d->startPlayback();
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_NoCall ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall,0 ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall,1 ) );
}

void UT_LcUiEngine::testStop()
{
    // Ensure that in MLcSession::EClosing state, "stopped" signal won't be emited
    // Check also that session duration timer is stopped
    LC_SESSION( mEngine )->iState = MLcSession::EClosing;
    mEngine->d->startSessionDurationTimer();
    QVERIFY( mEngine->d->mSessionDurationTimerId != 0 );
    QSignalSpy spy(mEngine, SIGNAL(stopped()));
    mEngine->d->mActivityManager->enableActivitySimulation(true);
    mEngine->d->stop();
    QVERIFY( !mEngine->d->mActivityManager->isActivitySimulationEnabled() );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_terminateSession ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    QCOMPARE( spy.count(), 0 );
    QVERIFY( mEngine->d->mSessionDurationTimerId == 0 );

    // Ensure that any other state than MLcSession::EClosing, will cause emition
    // of "stopped" signal
    lcutStub_LcSession_reset();
    lcutStub_LcPlayer_reset();
    LC_SESSION( mEngine )->iState = MLcSession::EInitialized;
    mEngine->d->stop();
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_terminateSession ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    QCOMPARE( spy.count(), 1 );
}

void UT_LcUiEngine::testSetViewLayout()
{
    QVERIFY( LC_SESSION( mEngine )->iLocalPlayer );
    QVERIFY( LC_SESSION( mEngine )->iRemotePlayer );
    QVERIFY( !LC_SESSION( mEngine )->iLocalPlayer->iPlaying );
    QVERIFY( !LC_SESSION( mEngine )->iRemotePlayer->iPlaying );
    LC_SESSION( mEngine )->iRemotePlayer->iState = MLcVideoPlayer::EPaused;
    QVERIFY( !mEngine->d->usingVideoTelephoneEngine() );
    
    mEngine->d->setViewLayout();
    QVERIFY( mEngine->d->viewLayout() == Lc_IdleViewLayout );
    
    LC_SESSION( mEngine )->iLocalPlayer->iPlaying = ETrue;
    
    mEngine->d->setViewLayout();
    QVERIFY( mEngine->d->viewLayout() == Lc_SharedViewLayout );
    
    LC_SESSION( mEngine )->iLocalPlayer->iPlaying = EFalse;
    LC_SESSION( mEngine )->iRemotePlayer->iState = MLcVideoPlayer::EPlaying;
    
    mEngine->d->setViewLayout();
    QVERIFY( mEngine->d->viewLayout() == Lc_ReceivedViewLayout );
    
    LC_SESSION( mEngine )->iLocalPlayer->iPlaying = ETrue;
    LC_SESSION( mEngine )->iRemotePlayer->iState = MLcVideoPlayer::EPlaying;
    
    mEngine->d->setViewLayout();
    QVERIFY( mEngine->d->viewLayout() == Lc_TwowayViewLayout );
    
    delete LC_SESSION( mEngine )->iLocalPlayer;
    LC_SESSION( mEngine )->iLocalPlayer = 0;
    
    mEngine->d->setViewLayout();
    QVERIFY( mEngine->d->viewLayout() == Lc_ReceivedOnlyViewLayout );
    
    delete LC_SESSION( mEngine )->iRemotePlayer;
    LC_SESSION( mEngine )->iRemotePlayer = 0;
    
    mEngine->d->setViewLayout();
    QVERIFY( mEngine->d->viewLayout() == Lc_IdleViewLayout );
    
    mEngine->d->mEngineName = lcutVideotelephonyApp;
    QVERIFY( mEngine->d->usingVideoTelephoneEngine() );
    
    mEngine->d->setViewLayout();
    QVERIFY( mEngine->d->viewLayout() == Lc_AllInOneViewLayout );   
}

void UT_LcUiEngine::testHideNotes()
{
    // Notes not set
    mEngine->d->hideNotes( true );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_NoCall ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    QVERIFY( mInvitingNote->isVisible() );
    QVERIFY( mWaitingNote->isVisible() );
    QVERIFY( mAcceptQuery->isVisible() );
    QVERIFY( mRecipientQuery->isVisible() );    
    
    // Notes set, hide all
    mEngine->d->setUiComponents( mInvitingNote, mWaitingNote,
                              mAcceptQuery, mRecipientQuery,mShareOwnVideoQuery );
    mEngine->d->hideNotes( true );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_NoCall ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    QVERIFY( !mInvitingNote->isVisible() );
    QVERIFY( !mWaitingNote->isVisible() );
    QVERIFY( !mAcceptQuery->isVisible() );
    QVERIFY( !mRecipientQuery->isVisible() );

    // Notes not set, do not hide waiting note
    mInvitingNote->show();
    mWaitingNote->show();
    mAcceptQuery->show();
    mRecipientQuery->show();       
    mEngine->d->hideNotes( false );
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_NoCall ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    QVERIFY( !mInvitingNote->isVisible() );
    QVERIFY( mWaitingNote->isVisible() );
    QVERIFY( !mAcceptQuery->isVisible() );
    QVERIFY( !mRecipientQuery->isVisible() ); 
}

void UT_LcUiEngine::testZoom()
{
    LcControlValues zoomData;
    
    // Leave
    lcutStub_LcEngine_setFailure( -1 );
    mEngine->d->zoom( 1 );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    QCOMPARE( mEngine->d->zoomValues( zoomData ), -1 );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_NoCall ) );
    
    // No leave
    lcutStub_LcEngine_reset();
    lcutStub_LcPlayer_reset();
    QCOMPARE( mEngine->d->zoomValues( zoomData ), 0 );
    mEngine->d->zoom( 2 );
    mEngine->d->zoom( 1 );
    mEngine->d->zoom( 3 );
    
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_SetLcZoomValueL, 0 ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_SetLcZoomValueL, 1 ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcPlayer_SetLcZoomValueL, 2 ) );
    QVERIFY( lcutStub_LcPlayer_expectCall( lcutStub_LcSession_NoCall, 3 ) );
}

void UT_LcUiEngine::testBlockUi()
{
    QSignalSpy blockedSpy(mEngine, SIGNAL(blocked()));
    QSignalSpy unblockedSpy(mEngine, SIGNAL(unblocked()));
    QSignalSpy mutedSpy(mEngine, SIGNAL(muted()));
    
    mEngine->d->BlockUi( ETrue );

    QCOMPARE(blockedSpy.count(), 1);
    QCOMPARE(unblockedSpy.count(), 0);
    QCOMPARE(mutedSpy.count(), 0);
    
    // no pending action once unblocked
    blockedSpy.clear();
    
    mEngine->d->BlockUi( EFalse );
    QCOMPARE(blockedSpy.count(), 0);
    QCOMPARE(unblockedSpy.count(), 1);
    QCOMPARE(mutedSpy.count(), 0);
    
    
    // pending action once unblocked
    unblockedSpy.clear();
    mEngine->d->setPendingAction( "muted" );
    mEngine->d->BlockUi( EFalse );
    QCOMPARE(blockedSpy.count(), 0);
    QCOMPARE(unblockedSpy.count(), 1);
    QCOMPARE(mutedSpy.count(), 1);
    QVERIFY( mEngine->d->mPendingAction.isEmpty() );  
}

void UT_LcUiEngine::testCloseTimer()
{
    // Try to cancel without timer
    mEngine->d->cancelCloseTimer();
    QVERIFY( !mEngine->d->mCloseTimer );
    
    // Try to cancel without timer being active
    mEngine->d->startCloseTimer();
    QVERIFY( mEngine->d->mCloseTimer );
    QVERIFY( mEngine->d->mCloseTimer->isActive() );
    mEngine->d->mCloseTimer->stop();
    QVERIFY( !mEngine->d->mCloseTimer->isActive() );
    mEngine->d->cancelCloseTimer();
    QVERIFY( mEngine->d->mCloseTimer );
    QVERIFY( !mEngine->d->mCloseTimer->isActive() );
    
    // Cancel an active timer
    mEngine->d->startCloseTimer();
    QVERIFY( mEngine->d->mCloseTimer );
    QVERIFY( mEngine->d->mCloseTimer->isActive() );
    mEngine->d->cancelCloseTimer();
    QVERIFY( mEngine->d->mCloseTimer );
    QVERIFY( !mEngine->d->mCloseTimer->isActive() );    
}


void UT_LcUiEngine::testfullScreenMode()
{
    QVERIFY( !mEngine->d->fullScreenMode() );
}


void UT_LcUiEngine::testsetFullScreenMode()
{
    mEngine->d->setFullScreenMode(true);
    QVERIFY( mEngine->d->fullScreenMode() );
}

void UT_LcUiEngine::testTimerEvent()
{
    mEngine->d->startSessionDurationTimer();
    QSignalSpy spy(mEngine, SIGNAL(sessionDurationChanged(const QString &)));
    
    QTimerEvent wrongTimerEvent(9999);
    mEngine->d->timerEvent(&wrongTimerEvent);
    QVERIFY( spy.count() == 0 );
    
    QTimerEvent correctTimerEvent(mEngine->d->mSessionDurationTimerId);
    mEngine->d->timerEvent(&correctTimerEvent);
    QVERIFY( spy.count() == 1 );
    QVERIFY( spy.at(0).count() == 1 ); // one arg which is duration as string
    QTime currDuration = QTime::fromString(spy.at(0).at(0).toString());
    QVERIFY( currDuration.isValid() );
}

void UT_LcUiEngine::testFillRemoteInfo()
{
    QSignalSpy recipientSpy(mEngine, SIGNAL(recipientChanged(const QString &)));
    QSignalSpy numberSpy(mEngine, SIGNAL(phoneNumberChanged(const QString &)));
    
    // test filling without signal emitting
    
    // number and name changed 
    mEngine->d->mRecipient = "";
    mEngine->d->mPhoneNumber = "";
    mEngine->d->fillRemoteInfo( false );
    QVERIFY( mEngine->d->mRecipient.length() > 0 );
    QVERIFY( mEngine->d->mPhoneNumber.length() > 0 );
    QVERIFY( recipientSpy.count() == 0 );
    QVERIFY( numberSpy.count() == 0 );
    
    // nothing changed
    QString prevRecipient = mEngine->d->mRecipient;
    QString prevNumber = mEngine->d->mPhoneNumber;
    mEngine->d->fillRemoteInfo( false );
    QVERIFY( mEngine->d->mRecipient == prevRecipient );
    QVERIFY( mEngine->d->mPhoneNumber == prevNumber );
    QVERIFY( recipientSpy.count() == 0 );
    QVERIFY( numberSpy.count() == 0 );
    
    // test filling with signal emitting
    
    // number and name changed
    mEngine->d->mRecipient = "";
    mEngine->d->mPhoneNumber = "";
    mEngine->d->fillRemoteInfo( true );
    QVERIFY( mEngine->d->mRecipient == prevRecipient );
    QVERIFY( mEngine->d->mPhoneNumber == prevNumber );
    QVERIFY( recipientSpy.count() == 1 );
    QVERIFY( numberSpy.count() == 1 );
    
    // state is opened and name not found and no number, unknown name is used
    LC_SESSION( mEngine )->iState = MLcSession::EOpen;
    delete LC_SESSION( mEngine )->iRemoteDisplayName;
    LC_SESSION( mEngine )->iRemoteDisplayName = NULL;
    LC_SESSION( mEngine )->iRemoteDisplayName = KNullDesC().AllocL();
    delete LC_SESSION( mEngine )->iRemoteDetails;
    LC_SESSION( mEngine )->iRemoteDetails = NULL;
    LC_SESSION( mEngine )->iRemoteDetails = KNullDesC().AllocL();
    mEngine->d->mPhoneNumber = "";
    mEngine->d->fillRemoteInfo( true );
    QVERIFY( mEngine->d->mRecipient == hbTrId("txt_vt_custom_unknown_number") );
    QVERIFY( mEngine->d->mPhoneNumber.isEmpty() );
    QVERIFY( recipientSpy.count() == 2 );
    QVERIFY( numberSpy.count() == 1 );
    
    // name not found but number exists, number is used at opened state as recipient
    mEngine->d->mRecipient = "";
    mEngine->d->mPhoneNumber = "";
    delete LC_SESSION( mEngine )->iRemoteDetails;
    LC_SESSION( mEngine )->iRemoteDetails = NULL;
    LC_SESSION( mEngine )->iRemoteDetails = _L("123445").AllocL();
    mEngine->d->fillRemoteInfo( true );
    QVERIFY( mEngine->d->mRecipient == "123445" );
    QVERIFY( mEngine->d->mPhoneNumber == "123445" );
    QVERIFY( recipientSpy.count() == 3 );
    QVERIFY( numberSpy.count() == 2 );
    
}

void UT_LcUiEngine::testUpdateSession()
{
    mEngine->d->updateSession();
    QVERIFY( lcutStub_LcSession_expectCall( lcutStub_LcSession_updateSession ) );
}

void UT_LcUiEngine::testIsLocalPlayerPlaying()
{
    // Not playing
    QVERIFY( !mEngine->d->isLocalPlayerPlaying() );

    // Playing
    LC_SESSION( mEngine )->iLocalPlayer->LcPlayL();
    QVERIFY( mEngine->d->isLocalPlayerPlaying() );
}

void UT_LcUiEngine::testIsRemotePlayerPlaying()
{
    // Not playing
    QVERIFY( !mEngine->d->isRemotePlayerPlaying() );
    
    // Playing
    LC_SESSION( mEngine )->iRemotePlayer->LcPlayL();
    QVERIFY( mEngine->d->isRemotePlayerPlaying() );
}

void UT_LcUiEngine::testMinimized()
{  
    mEngine->minimize();    
    QVERIFY( mEngine->isMinimized() );
    mEngine->minimize(); // Doing nothing
    QVERIFY( mEngine->isMinimized() );
}

void UT_LcUiEngine::testMaximized()
{    
    mEngine->maximize();    
    QVERIFY(mEngine->isMaximized() ); 
    mEngine->maximize(); // Doing nothing
    QVERIFY(mEngine->isMaximized() );
}

void UT_LcUiEngine::testIsMinimized()
{
    mEngine->minimize();
    QVERIFY( mEngine->isMinimized() );    
}

void UT_LcUiEngine::testHandleForegroundStatus()
{
    QtHighwayStubHelper::reset();
    
    // First time to foreground, engine is not set to foreground until viewReady signal
    HbView currView;
    mEngine->d->setCurrentView(&currView);
    QVERIFY( mEngine->d->mFirstForegroundSwitch );
    QVERIFY( !mEngine->d->mActivityManager->isActivitySimulationEnabled() );
    
    mEngine->d->HandleForegroundStatus(ETrue);
    QVERIFY( mEngine->d->mActivityManager->isActivitySimulationEnabled() );
    QVERIFY( !mEngine->d->session().IsBackgroundStartup() ); // Stub returns fg status in this
    QVERIFY( !QtHighwayStubHelper::isAtBg() );
    QVERIFY( !mEngine->d->mFirstForegroundSwitch );
    
    //Simulate viewReady signal happens, funciton will be called again.
    mEngine->d->HandleForegroundStatus(ETrue);
    QVERIFY( mEngine->d->session().IsBackgroundStartup() ); // Stub returns fg status in this
    QVERIFY( !mEngine->d->mFirstForegroundSwitch );
       
    // Bg switch
    QtHighwayStubHelper::reset();
    QVERIFY( mEngine->d->mActivityManager->isActivitySimulationEnabled() );
    mEngine->d->HandleForegroundStatus(EFalse);
    QVERIFY( !mEngine->d->mActivityManager->isActivitySimulationEnabled() );
    QVERIFY( QtHighwayStubHelper::isAtBg() );
    QVERIFY( !mEngine->d->session().IsBackgroundStartup() ); // Stub returns fg status in this

}


void UT_LcUiEngine::testSendDialTone()
{
    //Dumy test: Ensure Conversion does not cause any problem.
    QVERIFY( mEngine->d->SendDialTone('3') );
}


void UT_LcUiEngine::testVtVideoSendingSetting() 
{
    int settingsKeyValueDoNotShow = 1;
    if (!setVtVideoSendingSetting(settingsKeyValueDoNotShow))
        QFAIL("Writing test key to central repository failed.");
    QCOMPARE(mEngine->d->vtVideoSendingSetting(), settingsKeyValueDoNotShow);
}


void UT_LcUiEngine::testShareVideoIsAllowedShownAutomatically() 
{
    mShareOwnVideoQuery->hide();
    mEngine->d->setUiComponents(mInvitingNote,mWaitingNote,mAcceptQuery,mRecipientQuery,mShareOwnVideoQuery);
    int settingsKeyValueShowAutomatically = 2;
    if (!setVtVideoSendingSetting(settingsKeyValueShowAutomatically))
        QFAIL("Writing test key to central repository failed.");
    QVERIFY(mEngine->d->isAllowedToShareVideo());
    QVERIFY(mEngine->d->mShareOwnVideoQuery && !mEngine->d->mShareOwnVideoQuery->isVisible());
}


void UT_LcUiEngine::testShareVideoIsNotAllowedShown() 
{
    mShareOwnVideoQuery->hide();
    mEngine->d->setUiComponents(mInvitingNote,mWaitingNote,mAcceptQuery,mRecipientQuery,mShareOwnVideoQuery);
    int settingsKeyValueDoNotShow = 1;
    if (!setVtVideoSendingSetting(settingsKeyValueDoNotShow))
        QFAIL("Writing test key to central repository failed.");
    QVERIFY(!mEngine->d->isAllowedToShareVideo());
    QVERIFY(mEngine->d->mShareOwnVideoQuery && !mEngine->d->mShareOwnVideoQuery->isVisible());    
}


void UT_LcUiEngine::testShareVideoPermissionAskedAlways() 
{
    mShareOwnVideoQuery->hide();
    mEngine->d->setUiComponents(mInvitingNote,mWaitingNote,mAcceptQuery,mRecipientQuery,mShareOwnVideoQuery);
    int settingsKeyValueAlwaysAsk = 0;
    if (!setVtVideoSendingSetting(settingsKeyValueAlwaysAsk))
        QFAIL("Writing test key to central repository failed.");
    QVERIFY(!mEngine->d->isAllowedToShareVideo());
    QVERIFY(mEngine->d->mShareOwnVideoQuery && mEngine->d->mShareOwnVideoQuery->isVisible());
}


bool UT_LcUiEngine::setVtVideoSendingSetting(int key)
{
    QVariant settingsKeyValue(key);  
    XQSettingsManager settings;
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, 
                              KCRUidTelephonySettings.iUid, 
                              KSettingsVTVideoSending);
    return settings.writeItemValue(settingsKey, settingsKeyValue);
}
