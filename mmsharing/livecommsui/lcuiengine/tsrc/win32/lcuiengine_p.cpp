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
#include "lcuienginesimulator.h"

//SYSTEM
#include <QTimer>
#include <hbdialog.h>
#include <hblineedit.h>
#include <hbinstance.h>


#define LC_VTPLUGIN_NAME "Videotelephony";

const char localMovieName[] = "c:\\data\\local.3gp";
const char local2MovieName[] = "c:\\data\\local2.mpeg";
const char remoteMovieName[] = "c:\\data\\remote.3gp";
const char mPlayer[] = "c:\\data\\mplayer.exe";
const char msPaint[] = "mspaint";

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::LcUiEnginePrivate
// -----------------------------------------------------------------------------
//
LcUiEnginePrivate::LcUiEnginePrivate( LcUiEngine& uiEngine,
                                      const QString& engineName,
                                      const QString& applicationDisplayName)
    :mUiEngine( uiEngine )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::LcUiEnginePrivate()" )                 
    mSimulator = new LcUiEngineSimulator( *this );
    mViewLayout = (LcViewLayoutHint)mSimulator->layout();
    mSessionTitle = mSimulator->sessionTitle();
    mRecipient =  mSimulator->remoteAddress();
    mPhoneNumber = mSimulator->phoneNo();        
    mLocalPlayerProcess = new QProcess();
    mRemotePlayerProcess = new QProcess();
    mProgram = QString::fromAscii(mPlayer);
    mProgramImage = QString::fromAscii(msPaint);
    mLocalMovieName = QString::fromAscii(localMovieName);
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::LcUiEnginePrivate()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::~LcUiEnginePrivate
// -----------------------------------------------------------------------------
//
LcUiEnginePrivate::~LcUiEnginePrivate()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::~LcUiEnginePrivate()" )
    delete mSimulator;
    delete mLocalPlayerProcess;
    delete mRemotePlayerProcess;    
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::~LcUiEnginePrivate()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::zoom
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::zoom( int value )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::zoom()" )

    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::zoom()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::stop
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::stop()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::stop()" )    
    mSimulator->stop();    
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::stop()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::start
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::start()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::start()" )
    mSimulator->start();
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
    return mRecipient;
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
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::setContentAreas()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setOrientation
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::setOrientation( Qt::Orientation orientation )
{
    bool ok( false );
    return ok;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setUiComponents
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::setUiComponents(
        HbDialog* invitingNote,
        HbDialog* waitingNote,
        HbDialog* acceptQuery,
        HbDialog* recipientQuery )
{
    mInvitingNote = invitingNote;
    mWaitingNote = waitingNote;
    mAcceptQuery = acceptQuery;
    mRecipientQuery = recipientQuery;
    

}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::toggleMute
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::toggleMute()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::toggleMute()" )        
    if( mMuteState ){
        unmuted();
    }else{
        muted();
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::toggleMute()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isMuted
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::isMuted() const
{
    return mMuteState;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::toggleSpeaker
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::toggleSpeaker()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::toggleSpeaker()" )
    if ( mSpeakerState ){
        speakerDisabled();
    }else{
        speakerEnabled();
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::toggleSpeaker()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isSpeakerOn
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::isSpeakerOn() const
{    
    return mSpeakerState;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::zoomValues
// -----------------------------------------------------------------------------
//
int LcUiEnginePrivate::zoomValues( LcControlValues& values ) const
{
    int err( 1 );
    return err;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::toggleEnableCamera
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::toggleDisableCamera()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEnginePrivate::toggleDisableCamera()" )            
    if( mPlayerStateLocal == EPlaying ){
        localPlayerPaused();
        cameraDisabled();
    }else if( mPlayerStateLocal == EPaused){
        localPlayerPlaying();
        cameraEnabled();
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::toggleDisableCamera()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::setCameraMain
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::setMainCamera(const bool aMainCamera)
{
    LC_QDEBUG_2( "livecomms [UI] -> LcUiEnginePrivate::setCameraMain()", aMainCamera )
    if(aMainCamera){
        cameraChangedToMain();
    }else{
        cameraChangedToSecondary();
    }
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
    if( mMainCamera ){
        cameraChangedToSecondary();
    }else{
        cameraChangedToMain();
    }
    if( mPlayerStateLocal==EPlaying ){
        localPlayerPaused();
        mSimulator->setSimulation(2);
        mSimulator->start();
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
    
    LC_QDEBUG( "livecomms [UI] <- LcUiEnginePrivate::updateSession()" )
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isLocalPlayerPlaying
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::isLocalPlayerPlaying() const
{
    return ( mPlayerStateLocal == EPlaying )? true : false;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isRemotePlayerPlaying
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::isRemotePlayerPlaying() const
{
    return ( mSimulatorRemote == EPlaying )? true : false;
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::localPlayerPlaying
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::localPlayerPlaying()
{    
    QStringList args;
    args << mLocalMovieName;
    mLocalPlayerProcess->start(mProgram,args);        
    mPlayerStateLocal=EPlaying;
    emit mUiEngine.localPlayerPlaying();
}


// -----------------------------------------------------------------------------
// LcUiEnginePrivate::remotePlayerPlaying
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::remotePlayerPlaying()
{
    QStringList args;
    args << QString::fromAscii(remoteMovieName);    
    mRemotePlayerProcess->start(mProgram,args);    
    mSimulatorRemote=EPlaying;
    emit mUiEngine.remotePlayerPlaying();
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::localPlayerPaused
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::localPlayerPaused()
{    
    mLocalPlayerProcess->close();       
    mPlayerStateLocal=EPaused;
    emit mUiEngine.localPlayerPaused();
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::remotePlayerPaused
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::remotePlayerPaused()
{    
    mRemotePlayerProcess->close();    
    mSimulatorRemote=EPaused;
    emit mUiEngine.remotePlayerPaused();
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::cameraChangedToSecondary
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::cameraChangedToSecondary()
{
    mLocalMovieName= QString::fromAscii(local2MovieName);
    mMainCamera=false;
    emit mUiEngine.cameraChangedToSecondary();
}


// -----------------------------------------------------------------------------
// LcUiEnginePrivate::cameraChangedToSecondary
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::cameraChangedToMain()
{
    mLocalMovieName= QString::fromAscii(localMovieName);
    mMainCamera=true;
    emit mUiEngine.cameraChangedToMain();
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::isMinimized
// -----------------------------------------------------------------------------
//
bool LcUiEnginePrivate::isMinimized() const
{
    return (QApplication::activeWindow()->windowState()==Qt::WindowMinimized);
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::minimize
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::minimize()
{
    QApplication::activeWindow()->setWindowState(Qt::WindowMinimized);
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::maximize
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::maximize()
{
    QApplication::activeWindow()->setWindowState(Qt::WindowMaximized);
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::shareImage
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::shareImage(const QString& fileName)
{
    localPlayerPaused();    
    QStringList args;
    args << fileName;
    mLocalPlayerProcess->start(mProgramImage,args);
    mPlayerStateLocal=EPlaying;
    emit mUiEngine.localPlayerPlaying();
}

// -----------------------------------------------------------------------------
// LcUiEnginePrivate::shareCamera
// -----------------------------------------------------------------------------
//
void LcUiEnginePrivate::shareCamera()
{
    localPlayerPaused();
    localPlayerPlaying();
}
// End of File
