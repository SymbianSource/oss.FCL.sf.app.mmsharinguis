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

//USER
#include "lcuiengine_p.h"
#include "lcuidefs.h"
#include "lclogger.h"
#include "lcactivitymanager.h"

//SYSTEM
#include <w32std.h>
#include <e32err.h>
#include <bacline.h>
#include <coecntrl.h>
#include <apgtask.h>
#include <eikenv.h>

#include <QTimer>
#include <QChar>
#include <hbdialog.h>
#include <hbaction.h>
#include <hblabel.h>
#include <hblineedit.h>
#include <hbinstance.h>
#include <xqserviceutil.h>
#include <QApplication>
#include <hbview.h>
#include <hbmessagebox.h>
#include <hbprogressdialog.h>

#include <lcvideoplayer.h>
#include <lcwindow.h>
#include <lcaudiocontrol.h>
#include <lcsession.h>
#include <lczoomcontrol.h>
#include <lccameracontrol.h>
#include <xqsettingsmanager.h>
#include <xqsettingskey.h>
#include <settingsinternalcrkeys.h>



#define LC_VTPLUGIN_NAME "Videotelephony";

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::LcUiEnginePrivate
// -----------------------------------------------------------------------------
//
LcUiEnginePrivate::LcUiEnginePrivate(LcUiEngine& uiEngine,
                                     const QString& engineName,
                                     const QString& applicationDisplayName)
    : mEngineName( engineName.toUtf8() ),
      mSessionTitle( applicationDisplayName ),
      mViewLayout( Lc_IdleViewLayout ),
      mLiveCommsEngine( 0 ),
      mCloseTimer(0),
      mInvitingNote(0),
      mWaitingNote(0),
      mAcceptQuery(0),
      mRecipientQuery(0),
      mShareOwnVideoQuery(0),
      mBlocked(false),
      mFullScreen(false),
      mSessionDurationTimerId(0),
      mMainCamera(true),
      mUiEngine( uiEngine ),
      mIsMinimized(false),
      mFirstForegroundSwitch(true),
      mCurrentView(0),
      mActivityManager(0)
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::LcUiEnginePrivate()" )
        
    mPendingAction.clear();
    
    TPtrC8 name( reinterpret_cast< const TUint8* >( mEngineName.constData() ) );
    // TODO: 
    // Replace the TRAPs with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    TRAPD( err, mLiveCommsEngine = CLcEngine::NewL( name ) );
    if ( err != KErrNone ) {
        // Cannot proceed with the execution, exit the application
        LC_QDEBUG_2( "livecomms [UI] <- LcUiEnginePrivate::LcUiEnginePrivate(), failed:", err )
        qApp->exit(err);
		return;
    }
    
    mLiveCommsEngine->Session().SetLcSessionObserver( this );
    mLiveCommsEngine->Session().SetLcUiProvider( this );
    
    fillRemoteInfo( false );
        
    if ( usingVideoTelephoneEngine() ) {
        mViewLayout = Lc_AllInOneViewLayout;
    }
        
    LC_QDEBUG( "livecomms [UI] Setting window for plug-in" )
    CCoeControl* coeControl = 
        reinterpret_cast<CCoeControl*>( 
            HbInstance::instance()->allMainWindows().at(0)->winId());
    if ( coeControl ) {
        TInt windowPtr = reinterpret_cast<TInt>(coeControl->DrawableWindow());
        // Return value ignored as plugin may not be interested about window
        mLiveCommsEngine->Session().SetParameter(0,windowPtr);
    }

    mActivityManager = new LcActivityManager;
      
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::LcUiEnginePrivate()" )   
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::~LcUiEnginePrivate
// -----------------------------------------------------------------------------
//
LcUiEnginePrivate::~LcUiEnginePrivate()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::~LcUiEnginePrivate()" )
    delete mLiveCommsEngine;
    delete mCloseTimer;
    delete mActivityManager;
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::~LcUiEnginePrivate()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::zoom
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::zoom( int value )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::zoom()" )

    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    MLcZoomControl* zoom = zoomControl();
    if ( zoom ) {
        TRAP_IGNORE( zoom->SetLcZoomValueL( value ) ); 
    }
    
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::zoom()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::stop
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::stop()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::stop()" )
    
    mActivityManager->enableActivitySimulation(false);

    stopSessionDurationTimer();
    cancelCloseTimer();
    hideNotes( true );    
    
    // Start guard timer already before terminate call just in case
    // something goes horribly wrong in termination
    startStopGuardTimer();
    
    int err = terminateSession();
    
    if ( !err && session().LcSessionState() ==  MLcSession::EClosing ) {        
        LC_QDEBUG( "livecomms [UI] not closing UI yet, waiting for session termination..." )
    } else {
        LC_QDEBUG( "livecomms [UI] closing UI..." )
        emit mUiEngine.stopped();
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::stop()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::start
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::start()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::start()" )
    
    mActivityManager->enableActivitySimulation(true);
    
    if ( session().LcSessionState() != MLcSession::EReceived ) {
        startLiveContent();
    } else {
        //wait callback
        LC_QDEBUG( "livecomms [UI] MT start waiting callback" )
    }
    
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::start()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::sessionTitle
// -----------------------------------------------------------------------------
//
const QString& LcUiEnginePrivate::sessionTitle() const
{
    return mSessionTitle;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::recipient
// -----------------------------------------------------------------------------
//
const QString& LcUiEnginePrivate::recipient() const
{
    return ( mRecipient.isEmpty() ) ? mPhoneNumber : mRecipient;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::phoneNumber
// -----------------------------------------------------------------------------
//
const QString& LcUiEnginePrivate::phoneNumber() const
{
    return mPhoneNumber;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::viewLayout
// -----------------------------------------------------------------------------
//
LcViewLayoutHint LcUiEnginePrivate::viewLayout() const
{
    return mViewLayout;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setContentAreas
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::setContentAreas( const QRectF& sharedContent,
                                  const QRectF& receivedContent )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::setContentAreas()" )
    LC_QDEBUG_2( "livecomms [UI] shared window: ",  sharedContent )
    LC_QDEBUG_2( "livecomms [UI] received window: ",  receivedContent )
    
    mLocalRect = TRect();
    mRemoteRect = TRect();
    
    if ( sharedContent.height() != 0 ) {
        mLocalRect.SetRect( sharedContent.topLeft().x(), 
                           sharedContent.topLeft().y(),
                           sharedContent.bottomRight().x(),
                           sharedContent.bottomRight().y() );
    }
    
    if ( receivedContent.height() != 0 ) {
        mRemoteRect.SetRect( receivedContent.topLeft().x(), 
                           receivedContent.topLeft().y(),
                           receivedContent.bottomRight().x(),
                           receivedContent.bottomRight().y() );
    }
    
    setWindowRect( session().LocalVideoPlayer(), mLocalRect );
    setWindowRect( session().RemoteVideoPlayer(), mRemoteRect );
  
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::setContentAreas()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setOrientation
// Currently the only supported orientation is landscape.
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::setOrientation( Qt::Orientation orientation )
{
    bool ok( false );
    TInt error1( KErrNone );
    TInt error2( KErrNone );
    MLcVideoPlayer* localPlayer = session().LocalVideoPlayer();
    MLcVideoPlayer* remotePlayer = session().RemoteVideoPlayer();
  
    Q_UNUSED(orientation);
    
    if ( localPlayer && localPlayer->LcWindow() ) {
        TRAP(error1, localPlayer->LcWindow()->SetLcWindowOrientationL( MLcWindow::ELandscape ) );
    }
    if ( remotePlayer && remotePlayer->LcWindow() ) {
        TRAP(error2, remotePlayer->LcWindow()->SetLcWindowOrientationL( MLcWindow::ELandscape ));
    }
  
    if ( (error1 == KErrNone) && (error2 == KErrNone) ) {
        ok = true;
    }
        
    return ok;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setUiComponents
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::setUiComponents(
        HbProgressDialog* invitingNote,
        HbProgressDialog* waitingNote,
        HbDialog* acceptQuery,
        HbDialog* recipientQuery,
        HbDialog* shareOwnVideoQuery )
{
    mInvitingNote = invitingNote;
    mWaitingNote = waitingNote;
    mAcceptQuery = acceptQuery;
    mRecipientQuery = recipientQuery;
    mShareOwnVideoQuery = shareOwnVideoQuery;
    
    if (mAcceptQuery && (mAcceptQuery->actions().size() == 2)) {
        connect(mAcceptQuery->actions().at(0), SIGNAL(triggered()), 
                this, SLOT(startReceiving()) );
        connect(mAcceptQuery->actions().at(1), SIGNAL(triggered()), 
                this, SLOT(stop()) );
    }

    if (mInvitingNote && (mInvitingNote->actions().size() == 1)) {
        connect(mInvitingNote->actions().at(0), SIGNAL(triggered()), 
                this, SLOT(stop()) );
    }
    if (mWaitingNote && (mWaitingNote->actions().size() == 1)) {
        connect(mWaitingNote->actions().at(0), SIGNAL(triggered()), 
                this, SLOT(stop()) );
    }   
    if(mShareOwnVideoQuery && (mShareOwnVideoQuery->actions().size() == 2)) {
        connect(mShareOwnVideoQuery->actions().at(0), SIGNAL(triggered()), 
                this, SLOT(startLocalVideo()) );
        connect(mShareOwnVideoQuery->actions().at(1), SIGNAL(triggered()), 
                this, SLOT(stopLocalVideo()) );
    
    }
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::toggleMute
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::toggleMute()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::toggleMute()" )
    
    bool mute( !isMuted() );
    muteMic( mute );
    
    // Muting / unmuting the microphone is an asynchronous operation.
    // Assuming here that the operation will eventually succeed.  
    if ( mute ) {
        completeAction( lcEngSignalNameMuted );
    } else {
        completeAction( lcEngSignalNameUnmuted );
    }       
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::toggleMute()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isMuted
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::isMuted() const
{
    bool micMuted( false );
    MLcAudioControl* audio = audioControl();
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    if ( audio ) {
        TRAP_IGNORE( micMuted = audio->IsLcMicMutedL() );
    }
    return micMuted;      
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::toggleSpeaker
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::toggleSpeaker()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::toggleSpeaker()" )
    
    bool enable( !isSpeakerOn() );
    enableSpeaker( enable );        

    // Enabling / disabling the speaker is an asynchronous operation.
    // Assuming here that the operation will eventually succeed.    
    if ( enable ) {
        completeAction( lcEngSignalNameSpeakerEnabled );
    } else {
        completeAction( lcEngSignalNameSpeakerDisabled );
    }       
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::toggleSpeaker()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isSpeakerOn
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::isSpeakerOn() const
{
    return audioControl()->IsLcLoudspeakerEnabled();
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::zoomValues
// -----------------------------------------------------------------------------
//
int LcUiEnginePrivate::zoomValues( LcControlValues& values ) const
{
    int err( -1 );
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    MLcZoomControl* zoom = zoomControl();
    if ( zoom ) {
        TRAP( err,
              values.mMaxValue = zoom->MaxLcZoomL();
              values.mMinValue = zoom->MinLcZoomL();
              values.mValue = zoom->LcZoomValueL(); );
    }
    return err;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::toggleEnableCamera
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::toggleDisableCamera()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::toggleDisableCamera()" )
    MLcVideoPlayer* player = visibleLocalPlayer();
    if( player ) {
        pause( session().LocalVideoPlayer() );
        completeAction( lcEngSignalNameCameraDisabled );
    } else {
        play( session().LocalVideoPlayer() );
        completeAction( lcEngSignalNameCameraEnabled );
    }
    emitViewLayoutChanged();
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::toggleDisableCamera()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setCameraMain
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::setMainCamera(const bool aMainCamera)
{
    LC_QDEBUG_2( "livecomms [UI] -> LcUiEnginePrivate::setCameraMain()", aMainCamera )
    mMainCamera = aMainCamera;
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::setCameraMain()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::cameraIndex
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::mainCamera() const
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::mainCamera()" )
    LC_QDEBUG_2( "livecomms [UI] <- LcUiEnginePrivate::mainCamera()", mMainCamera )
    return mMainCamera;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::toggleCamera
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::toggleCamera()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::toggleCamera()" )
 
    MLcVideoPlayer* localPlayer = session().LocalVideoPlayer();
    
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    if ( localPlayer && localPlayer->LcCameraControl() ) {
        TRAP_IGNORE( localPlayer->LcCameraControl()->ToggleLcCameraL() );
    }

    setMainCamera( !mMainCamera );
    
    // Enabling / disabling the camera is an asynchronous operation.
    // Assuming here that the operation will eventually succeed.  
    if ( mMainCamera ) {
        completeAction( lcEngSignalNameCameraChangedToMain );
    } else {
        completeAction( lcEngSignalNameCameraChangedToSecondary );
    }
        
    LC_QDEBUG_2( "livecomms [UI] <- LcUiEnginePrivate::toggleCamera(), mainCamera:", mMainCamera )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::fullScreenMode
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::fullScreenMode()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::fullScreenMode()" )
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::fullScreenMode()" )
    return mFullScreen;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setFullScreenMode
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::setFullScreenMode(bool aMode)
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::setFullScreenMode()" )
    mFullScreen = aMode;
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::setFullScreenMode()" )
}


// -----------------------------------------------------------------------------
// LcUiEnginePrivate::updateSession
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::updateSession()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::updateSession()" )
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    TRAP_IGNORE( session().UpdateLcSessionL() );
    
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::updateSession()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isLocalPlayerPlaying
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::isLocalPlayerPlaying() const
{
    MLcVideoPlayer* player = session().LocalVideoPlayer();
    return ( player && 
             player->LcVideoPlayerState() == MLcVideoPlayer::EPlaying );
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isRemotePlayerPlaying
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::isRemotePlayerPlaying() const
{
    MLcVideoPlayer* player = session().RemoteVideoPlayer();
    return ( player && 
             player->LcVideoPlayerState() == MLcVideoPlayer::EPlaying );
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::timerEvent
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::timerEvent(QTimerEvent *event)
{
    if ( event->timerId() == mSessionDurationTimerId ) {
        QTime curr(0, 0, 0, 0);
        curr = curr.addSecs( 
                mSessionDurationStartTime.secsTo( QTime::currentTime() ) );
        emit mUiEngine.sessionDurationChanged(curr.toString());
    }
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::startReceiving
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::startReceiving()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::startReceiving()" )
    
    if ( mAcceptQuery ) {
        mAcceptQuery->hide();
    }
    
    establishSession();
    
    if ( mWaitingNote && featureSupported( CLcEngine::ELcShowWaitingNote ) ) {
        mWaitingNote->show();
    }
    
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::startReceiving()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::stopForcefully
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::stopForcefully()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::stopForcefully()" )
    
    // This is only way to force exit in case CActiveScheduler is blocked
    // by engine plugin.
    User::Exit(0);
    
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::stopForcefully()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::StateChanged
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::StateChanged( MLcSession& aSession )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::StateChanged( session )" )
    
    MLcSession::TLcSessionState newState = MLcSession::EUninitialized;
    newState = aSession.LcSessionState();
    
    LC_QDEBUG_2( "livecomms [UI] new state=", newState )
    
    if ( newState == MLcSession::EOpen ) {
        LC_QDEBUG( "livecomms [UI] state is MLcSession::EOpen" )
        cancelCloseTimer();
        hideNotes( false );
        if( isAllowedToShareVideo() ) {
            startLocalVideo();
        }
        startRemoteVideo();
        fillRemoteInfo( true );
        emitViewLayoutChanged();
        startSessionDurationTimer();
        
    } else if ( newState == MLcSession::EReceived ) {
        LC_QDEBUG( "livecomms [UI] state is MLcSession::EReceived" )
        mRecipient = DESC_TO_QSTRING( session().RemoteDisplayName() );        
        if ( mAcceptQuery && 
             featureSupported( CLcEngine::ELcShowAcceptQuery ) ) {
            mAcceptQuery->show();
        } else {
            startReceiving();
        }        
    } else if ( newState == MLcSession::EClosed ) {
        LC_QDEBUG( "livecomms [UI] state is MLcSession::EClosed, closing UI..." )
        emit mUiEngine.stopped();
            
    } else if ( newState == MLcSession::EOpening ){
        LC_QDEBUG( "livecomms [UI] state is MLcSession::EOpening" )
    }

    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::StateChanged( session )" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::StateChanged
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::StateChanged( MLcVideoPlayer& aPlayer )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::StateChanged( player )" )
    
    emitViewLayoutChanged();
    
    if ( aPlayer.LcVideoPlayerState() == MLcVideoPlayer::EPlaying ) { 
        if ( &aPlayer == session().RemoteVideoPlayer() ) {
            if ( mWaitingNote ) {
                mWaitingNote->hide();
            }
            emit mUiEngine.remotePlayerPlaying();
        }
        if ( &aPlayer == session().LocalVideoPlayer() ) {
            emit mUiEngine.localPlayerPlaying();
        }
    } else if ( aPlayer.LcVideoPlayerState() == MLcVideoPlayer::EPreparing ) {
        if ( &aPlayer == session().RemoteVideoPlayer() ) {
            emit mUiEngine.remotePlayerPreparing();
        }
        if ( &aPlayer == session().LocalVideoPlayer() ) {           
            emit mUiEngine.localPlayerPreparing();
        }    
    } else if ( aPlayer.LcVideoPlayerState() == MLcVideoPlayer::EBuffering ) {
        if ( &aPlayer == session().RemoteVideoPlayer() ) {
            emit mUiEngine.remotePlayerBuffering();
        }
        if ( &aPlayer == session().LocalVideoPlayer() ) {
            emit mUiEngine.localPlayerBuffering();
        }    
    } else if ( aPlayer.LcVideoPlayerState() == MLcVideoPlayer::EPaused ) {
        if ( &aPlayer == session().RemoteVideoPlayer() ) {
            emit mUiEngine.remotePlayerPaused();
        }
        if ( &aPlayer == session().LocalVideoPlayer() ) {
            emit mUiEngine.localPlayerPaused();
        }    
    } else if ( aPlayer.LcVideoPlayerState() == MLcVideoPlayer::EUnavailable ) {
        if ( &aPlayer == session().RemoteVideoPlayer() ) {
            emit mUiEngine.remotePlayerUnavailable();
        }
        if ( &aPlayer == session().LocalVideoPlayer() ) {
            emit mUiEngine.localPlayerUnavailable();
        }    
    }
    
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::StateChanged( player )" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::Updated
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::Updated( MLcSession& /*aSession*/ )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::Updated( session )" )
    emitViewLayoutChanged();
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::Updated( session )" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::Updated
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::Updated( MLcVideoPlayer& /*aPlayer*/ )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::Updated( player )" )
    emitViewLayoutChanged();
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::Updated( player )" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::Failed
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::Failed( MLcSession& /*aSession*/, TInt /*aError*/ )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::Failed( session )" )
    stop();
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::Failed( session )" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::Failed
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::Failed( MLcVideoPlayer& /*aPlayer*/, TInt /*aError*/ )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::Failed( player )" )
    stop();
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::Failed( player )" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::SessionTimeChanged
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::SessionTimeChanged( MLcSession& /*aSession*/,
                        const TTimeIntervalSeconds& /*aSeconds*/ )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::SessionTimeChanged()" )
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::SessionTimeChanged()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::SelectRecipient
// -----------------------------------------------------------------------------
//
TBool LcUiEnginePrivate::SelectRecipient( MDesCArray& aRecipientAddresses, 
                                  TDes& aSelectedRecipientAddress )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::SelectRecipient()" )
    aSelectedRecipientAddress = aRecipientAddresses.MdcaPoint(0);
    mRecipient = DESC_TO_QSTRING( aSelectedRecipientAddress );
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::SelectRecipient()" )
    return ETrue;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::SelectRecipient
// -----------------------------------------------------------------------------
//
TBool LcUiEnginePrivate::InputRecipient( TDes& aRecipientAddress )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::InputRecipient()" )
    
    TBool answered( EFalse );
    
    if ( mRecipientQuery ) {
        mRecipient = DESC_TO_QSTRING( aRecipientAddress );
        HbLineEdit* lineEdit = new HbLineEdit( mRecipient );
        mRecipientQuery->setContentWidget( lineEdit );
        mRecipientQuery->open();
        TPtrC16 textPtr( 
            reinterpret_cast< const TUint16* >( mRecipient.utf16() ) );
        aRecipientAddress.Copy( textPtr );
        answered = ETrue;
    }

    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::InputRecipient()" )
    return answered;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::HandleForegroundStatus
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::HandleForegroundStatus( TBool aForeground )
{
    LC_QDEBUG_2( "livecomms [UI] -> LcUiEnginePrivate::HandleForegroundStatus(), fg:", 
                 aForeground )
    
    mActivityManager->enableActivitySimulation(aForeground);
    
    if ( aForeground && !HbInstance::instance()->allMainWindows().isEmpty() ){
        HbInstance::instance()->allMainWindows().at(0)->show();
    }
    XQServiceUtil::toBackground(!aForeground);

    mIsMinimized = !aForeground;
    
    handleEngineForegroundStatus();
    
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::HandleForegroundStatus()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::BlockUi
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::BlockUi( TBool aBlocked )
{
    LC_QDEBUG_2( "livecomms [UI] -> LcUiEnginePrivate::BlockUi, blocked:", aBlocked )
    mBlocked = aBlocked;
    
    if ( mBlocked ){
        emit mUiEngine.blocked();
    }
    else{
        emit mUiEngine.unblocked();
        completePendingAction();
    }   
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::BlockUi()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setViewLayout
// -----------------------------------------------------------------------------
//
LcViewLayoutHint LcUiEnginePrivate::setViewLayout()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::setViewLayout()" )
    
    LcViewLayoutHint hint = (LcViewLayoutHint)Lc_NoViewLayout;
    
    MLcVideoPlayer* localPlayer = visibleLocalPlayer();
    MLcVideoPlayer* remotePlayer = visibleRemotePlayer();
    
    if ( usingVideoTelephoneEngine() ) {
        hint = Lc_AllInOneViewLayout;
        LC_QDEBUG( "livecomms [UI] use Lc_AllInOneViewLayout" )
    } else if ( localPlayer && remotePlayer ) {
        hint = Lc_TwowayViewLayout;
        LC_QDEBUG( "livecomms [UI] use Lc_TwowayViewLayout" )
    } else if ( localPlayer ) {
        hint = Lc_SharedViewLayout;
        LC_QDEBUG( "livecomms [UI] use Lc_SharedViewLayout" )
    } else if ( remotePlayer && !session().LocalVideoPlayer() ) {
        hint = Lc_ReceivedOnlyViewLayout;
        LC_QDEBUG( "livecomms [UI] use Lc_ReceivedOnlyViewLayout" )
    } else if ( remotePlayer ) {
        hint = Lc_ReceivedViewLayout;
        LC_QDEBUG( "livecomms [UI] use Lc_ReceivedViewLayout" )
    } else {
        hint = Lc_IdleViewLayout;
        LC_QDEBUG( "livecomms [UI] ! no players ! use Lc_IdleViewLayout" )
    }
    
    mViewLayout = hint;
    
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::setViewLayout()" )
    
    return viewLayout();
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::emitViewLayoutChanged
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::emitViewLayoutChanged()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::emitViewLayoutChanged()" )

    LcViewLayoutHint oldLayout = viewLayout();
    LcViewLayoutHint layout = setViewLayout();
    
    if ( layout != oldLayout ) {
        LC_QDEBUG( "livecomms [UI] layout has changed. signal" )
        emit mUiEngine.layoutChanged();
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::emitViewLayoutChanged()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::startLiveContent
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::startLiveContent()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::startLiveContent()" )
    
    if ( mInvitingNote && featureSupported( CLcEngine::ELcShowInvitingNote ) ) {
        mInvitingNote->show();
    }
    enableSpeaker( true );
    establishSession();
        
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::startLiveContent()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::startPlayback
// Utility style function to start both local and remote players if available.
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::startPlayback()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::startPlayback()" )
    startLocalVideo();
    startRemoteVideo();
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::startPlayback()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::startRemoteVideo
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::startRemoteVideo()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::startRemotePlayback()" )
    MLcVideoPlayer* remotePlayer = session().RemoteVideoPlayer();
    if ( remotePlayer ) {
        LC_QDEBUG( "livecomms [UI] enable remote player window" )
        enableWindow( remotePlayer, true );
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::startRemotePlayback()" )
}
// -----------------------------------------------------------------------------
// LcUiEnginePrivate::startCloseTimer
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::startCloseTimer()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::startCloseTimer()" )
    delete mCloseTimer;
    mCloseTimer = 0;
    mCloseTimer = new QTimer(this);
    mCloseTimer->setSingleShot(true);
    connect(mCloseTimer, SIGNAL(timeout()), this, SLOT(stop()));
    mCloseTimer->start(60000);
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::startCloseTimer()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::cancelCloseTimer
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::cancelCloseTimer()
{
    if ( mCloseTimer && mCloseTimer->isActive()) {
        mCloseTimer->stop();
    }
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::hideNotes
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::hideNotes( bool hideWaitingNote )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::hideNotes()" )
    if ( mAcceptQuery ) {
        mAcceptQuery->hide();
    }
    if ( mWaitingNote && hideWaitingNote ) {
        mWaitingNote->hide();
    }
    if ( mInvitingNote ) {
        mInvitingNote->hide();
    }
    if ( mRecipientQuery ) {
        mRecipientQuery->hide();
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::hideNotes()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::completeAction
// Completes method passed as argument. If engine is in blocked state
// action completion will be delayed until unblock occurs.
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::completeAction( const char* actionMethod )
{
    LC_QDEBUG_2( "livecomms [UI] -> LcUiEnginePrivate::completeAction :", actionMethod )
    if ( mBlocked ) {
        LC_QDEBUG( "livecomms [UI]  Blocked, invoke later" )
        // store action and invoke it once unblocked
        setPendingAction( actionMethod );     
    }
    else {
        QMetaObject::invokeMethod( &mUiEngine, actionMethod );
    }     
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::completeAction" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setPendingAction
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::setPendingAction( const char* actionMethod )
{
    mPendingAction = actionMethod;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::completePendingAction
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::completePendingAction()
{
    if ( !mPendingAction.isEmpty() ) {
        completeAction( mPendingAction.toAscii().constData() );
        mPendingAction.clear();
    }
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::videoTelephoneLayout
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::usingVideoTelephoneEngine()
{
    return mEngineName == LC_VTPLUGIN_NAME;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::session
// -----------------------------------------------------------------------------
//
MLcSession& LcUiEnginePrivate::session() const
{
    return mLiveCommsEngine->Session();
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::visibleLocalPlayer
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* LcUiEnginePrivate::visibleLocalPlayer() const
{
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    MLcVideoPlayer* localPlayer = session().LocalVideoPlayer();
    if ( localPlayer ) {
        bool playing( true );
        TRAP_IGNORE( playing = localPlayer->LcIsPlayingL() );
        if ( !playing ) {
            localPlayer = 0;
        }
    }
    return localPlayer;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::visibleRemotePlayer
// -----------------------------------------------------------------------------
//
MLcVideoPlayer* LcUiEnginePrivate::visibleRemotePlayer() const
{
    MLcVideoPlayer* remotePlayer = session().RemoteVideoPlayer();
    if ( remotePlayer && 
         remotePlayer->LcVideoPlayerState() == MLcVideoPlayer::EPaused ) {
        remotePlayer = 0;
    }
    return remotePlayer;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::audioControl
// -----------------------------------------------------------------------------
//
MLcAudioControl* LcUiEnginePrivate::audioControl() const
{
    MLcAudioControl* audioControl( 0 );
    if ( session().LocalVideoPlayer() ) {
        audioControl = session().LocalVideoPlayer()->LcAudioControl();
    }
    if ( !audioControl && session().RemoteVideoPlayer() ) {
        audioControl = session().RemoteVideoPlayer()->LcAudioControl();
    }
    return audioControl;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::zoomControl
// -----------------------------------------------------------------------------
//
MLcZoomControl* LcUiEnginePrivate::zoomControl() const
{
    MLcZoomControl* zoomControl( 0 );
    if ( session().LocalVideoPlayer() ) {
        zoomControl = session().LocalVideoPlayer()->LcZoomControl();
    }
    return zoomControl;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isFeatureSupported
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::featureSupported( CLcEngine::TLcFeature feature )
{
    bool isSupported( false );
    if ( mLiveCommsEngine ) {
        isSupported = mLiveCommsEngine->IsFeatureSupported( feature );
    }
    return isSupported;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::startSessionDurationTimer
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::startSessionDurationTimer()
{
    const int sessionDurationTimerIntervalInMs = 1000;
    if ( !mSessionDurationTimerId ){
        mSessionDurationTimerId = startTimer( sessionDurationTimerIntervalInMs );
        LC_QDEBUG_2( "livecomms [UI] -> duration timer id:", mSessionDurationTimerId )
        mSessionDurationStartTime = QTime::currentTime();
    }
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::stopSessionDurationTimer
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::stopSessionDurationTimer()
{
    if ( mSessionDurationTimerId ){
        killTimer( mSessionDurationTimerId );
        mSessionDurationTimerId = 0;
    }
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::establishSession
// -----------------------------------------------------------------------------
//
int LcUiEnginePrivate::establishSession()
{
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    TRAPD( err, mLiveCommsEngine->Session().EstablishLcSessionL() );
    if ( !err ) {
        startCloseTimer();
    }
    LC_QDEBUG_2( "livecomms [UI] LcUiEnginePrivate::establishSession, err", err )
    return err;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::terminateSession
// -----------------------------------------------------------------------------
//
int LcUiEnginePrivate::terminateSession()
{
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    TRAPD( err, mLiveCommsEngine->Session().TerminateLcSessionL() );
    LC_QDEBUG_2( "livecomms [UI] LcUiEnginePrivate::terminateSession, err", err )
    return err;    
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setWindowRect
// -----------------------------------------------------------------------------
//
int LcUiEnginePrivate::setWindowRect( MLcVideoPlayer* player, TRect rect )
{
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    int err( -1 );
    if ( player &&
         player->LcWindow() &&
         player->LcWindow()->LcWindowRect() != rect ) {
        TRAP( err, player->LcWindow()->SetLcWindowRectL( rect ) );
    }
    LC_QDEBUG_2( "livecomms [UI] LcUiEnginePrivate::setWindowRect, err", err )
    return err;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::enableWindow
// -----------------------------------------------------------------------------
//
int LcUiEnginePrivate::enableWindow( MLcVideoPlayer* player, bool enable )
{
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    int err( -1 );
    if ( player && 
         player->LcWindow() ) {
        TRAP( err, player->LcWindow()->EnableLcWindowL( enable ) );
    }
    LC_QDEBUG_2( "livecomms [UI] LcUiEnginePrivate::enableWindow, err", err )
    return err;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::play
// -----------------------------------------------------------------------------
//
int LcUiEnginePrivate::play( MLcVideoPlayer* player )
{
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    int err( -1 );
    if ( player ) {
        TRAP( err, player->LcPlayL() );
    }
    LC_QDEBUG_2( "livecomms [UI] LcUiEnginePrivate::play, err", err )
    return err;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::pause
// -----------------------------------------------------------------------------
//
int LcUiEnginePrivate::pause( MLcVideoPlayer* player )
{
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    int err( -1 );
    if ( player ) {
        TRAP( err, player->LcPauseL() );
    }
    LC_QDEBUG_2( "livecomms [UI] LcUiEnginePrivate::pause, err", err )
    return err;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::enableSpeaker
// -----------------------------------------------------------------------------
//
int LcUiEnginePrivate::enableSpeaker( bool enable )
{
    int err( -1 );
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    MLcAudioControl* audio = audioControl();
    if ( audio ) {
        TRAP( err, audio->EnableLcLoudspeakerL( enable ) );
    }
    LC_QDEBUG_2( "livecomms [UI] LcUiEnginePrivate::enableSpeaker, err", err )
    return err;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::muteMic
// -----------------------------------------------------------------------------
//
int LcUiEnginePrivate::muteMic( bool mute )
{
    int err( -1 );
    // TODO: Replace TRAP with QT_TRANSLATE_SYMBIAN_LEAVE_TO_EXCEPTION (Qt4.5.2)
    MLcAudioControl* audio = audioControl();
    if ( audio ) {
        TRAP( err, audio->MuteLcMicL( mute ) );
    }
    return err;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::fillRemoteInfo
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::fillRemoteInfo(bool informChanges)
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::fillRemoteInfo()" )
    
    QString phoneNum = DESC_TO_QSTRING( session().RemoteDetails() );
    if ( phoneNum != mPhoneNumber) {
        LC_QDEBUG_2( "remote number:", phoneNum )
        mPhoneNumber = phoneNum;
        if ( informChanges ) {
            emit mUiEngine.phoneNumberChanged(mPhoneNumber);
        }
    }  
    // If no recipient even at opened state and no phonenum, use unknown recipient
    QString recipient = DESC_TO_QSTRING( session().RemoteDisplayName() );
    if ( recipient.isEmpty() ){
        recipient = mPhoneNumber;
    }
    if ( session().LcSessionState() == MLcSession::EOpen && recipient.isEmpty() ) {
        recipient = hbTrId("txt_vt_custom_unknown_number");
    }
    if ( recipient != mRecipient ){
        LC_QDEBUG_2( "remote name:", recipient )
        mRecipient = recipient;
        if ( informChanges ) {
            emit mUiEngine.recipientChanged(mRecipient);
        }
    } 
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::fillRemoteInfo()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isMinimized
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::isMinimized() const
{
    return mIsMinimized;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::minimize
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::minimize()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::minimize()" )
    
    if ( !isMinimized() ){
        TRect emptyRect;
        setWindowRect( session().LocalVideoPlayer(), emptyRect );
        setWindowRect( session().RemoteVideoPlayer(), emptyRect );
        HandleForegroundStatus(EFalse);
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::minimize()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::maximize
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::maximize()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::maximize()" )  
    if ( isMinimized() ){
        setWindowRect( session().LocalVideoPlayer(), mLocalRect );
        setWindowRect( session().RemoteVideoPlayer(), mRemoteRect );
        HandleForegroundStatus(ETrue);
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::maximize()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setCurrentView
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::setCurrentView(HbView* view)
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEnginePrivate::setCurrentView()" )  
    mCurrentView = view;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::startStopGuardTimer
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::startStopGuardTimer()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::startStopGuardTimer()" ) 
    const int stoppingGuardTimerInMs = 5000;
    QTimer::singleShot(stoppingGuardTimerInMs, this, SLOT(stopForcefully()));
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::startStopGuardTimer()" ) 
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::handleEngineForegroundStatus
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::handleEngineForegroundStatus()
{
    bool foreground = !mIsMinimized;
    LC_QDEBUG_2( "livecomms [UI] -> LcUiEnginePrivate::handleEngineForegroundStatus(), fg:", 
                 foreground )
    HbMainWindow *mainWindow = HbInstance::instance()->allMainWindows().at(0);

    if ( mLiveCommsEngine ){
        bool setStatusToEngine(true);
        if ( foreground && mFirstForegroundSwitch ){
            // Have to wait that first paint has occured, otherwise video
            // windows are shown too early. 
            // TODO: wk8 does not yet have viewReady signal so simulate it
            // by using timer. Timer can be removed later.
            if ( mCurrentView ){
                LC_QDEBUG( "livecomms [UI]  Wait for first paint" )
                connect( mainWindow, SIGNAL(viewReady()), 
                         this, SLOT(handleEngineForegroundStatus()) );
                setStatusToEngine = false;
            }
            
        }
        if ( setStatusToEngine ) {
            LC_QDEBUG( "livecomms [UI]  Set fg status to engine plugin" ) 
            session().SetForegroundStatus(foreground);
            disconnect( mainWindow, SIGNAL(viewReady()), 
                        this, SLOT(handleEngineForegroundStatus()) );
        }
        mFirstForegroundSwitch = false;
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::handleEngineForegroundStatus()" )
}    


// -----------------------------------------------------------------------------
// LcUiEnginePrivate::SendDialTone
// -----------------------------------------------------------------------------
//
bool  LcUiEnginePrivate::SendDialTone(const QChar  aKey)
{
    if ( mLiveCommsEngine ){
        const TChar dialChar = static_cast<TChar>(aKey.unicode());
        return mLiveCommsEngine->Session().SendDialTone(dialChar);
    }
    return false;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isAllowedToShareVideo
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::isAllowedToShareVideo()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::isAllowedToShareVideo()" )
    if (!featureSupported( CLcEngine::ELcSendVideoQuery )) //outgoing videocall
        return true;
    else { //incoming videocall
        int ownVtVideoSendingSetting = vtVideoSendingSetting();
        if (VTSETTING_SHOW_AUTOMATICALLY == ownVtVideoSendingSetting)
            return true;
        else if (VTSETTING_DO_NOT_SHOW == ownVtVideoSendingSetting)
            return false;
        else {
            if (mShareOwnVideoQuery)
                mShareOwnVideoQuery->show();
            return false;
        }
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::isAllowedToShareVideo()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::vtVideoSendingSetting
// -----------------------------------------------------------------------------
//
int LcUiEnginePrivate::vtVideoSendingSetting()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::vtVideoSendingSetting()" )
    XQSettingsManager settings;
    XQSettingsKey settingsKey(XQSettingsKey::TargetCentralRepository, 
                              KCRUidTelephonySettings.iUid, 
                              KSettingsVTVideoSending);
    QVariant ownVtVideoSendingSetting = settings.readItemValue(settingsKey);
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::vtVideoSendingSetting()" )
    return ownVtVideoSendingSetting.toInt();
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::stopLocalVideo
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::stopLocalVideo()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::stopLocalVideo()" )
    MLcVideoPlayer* localPlayer = session().LocalVideoPlayer();
    if( localPlayer ) {
        pause( localPlayer );
        enableWindow( localPlayer, false );
        completeAction( lcEngSignalNameCameraDisabled );
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::stopLocalVideo()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::startLocalVideo
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::startLocalVideo()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::startLocalVideo()" )
    MLcVideoPlayer* localPlayer = session().LocalVideoPlayer();
    if( localPlayer ){
        play( localPlayer );
        enableWindow( localPlayer, true );
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::startLocalVideo()" )
}
// End of File
