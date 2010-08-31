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


#include "lcuiengine_p.h"
#include "ut_lcuidefs.h"
#include "lcuidefs.h"
#include <hbview.h>

HbView* lcutStub_LcUiEngine_currentView = 0;
QChar lcStub_LcUiEngine_dialtone('0');

LcUiEnginePrivate::LcUiEnginePrivate(  LcUiEngine& uiEngine,
                                        const QString &engineName,
                                        const QString &applicationDisplayName )
    : mUiEngine ( uiEngine ),
      mEngineName( engineName.toUtf8() ),
      mSessionTitle( applicationDisplayName )

{
    mViewLayout = Lc_NoViewLayout;
	mLiveCommsEngine = 0;
    mCloseTimer = 0;
    mInvitingNote = 0;
    mWaitingNote = 0;
    mAcceptQuery = 0;
    mRecipientQuery = 0;
    mFullScreen = false;
}

LcUiEnginePrivate::~LcUiEnginePrivate()
{
}

void LcUiEnginePrivate::start()
{
    lcutStub_LcUiEngine_calledMethodsInstance().insert(0,lcutStub_LcUiEngine_start );
}

void LcUiEnginePrivate::setContentAreas( const QRectF& /*sharedContent*/,
                                  const QRectF& /*receivedContent*/ )
{
    lcutStub_LcUiEngine_calledMethodsInstance().insert(0,lcutStub_LcUiEngine_setContentAreas );

}

bool LcUiEnginePrivate::setOrientation( Qt::Orientation /*orientation*/ )
{
    lcutStub_LcUiEngine_calledMethodsInstance().insert(0,lcutStub_LcUiEngine_setOrientation );
    return true;
}

void LcUiEnginePrivate::zoom( int /*value*/ )
{
    lcutStub_LcUiEngine_calledMethodsInstance().insert(0,lcutStub_LcUiEngine_zoom );

}

void LcUiEnginePrivate::stop()
{
    lcutStub_LcUiEngine_calledMethodsInstance().insert(0,lcutStub_LcUiEngine_stop );
    
}

void LcUiEnginePrivate::updateSession()
{
    lcutStub_LcUiEngine_calledMethodsInstance().insert(0,lcutStub_LcUiEngine_updateSession );
    
}

bool LcUiEnginePrivate::isLocalPlayerPlaying() const
{
    return lcutStub_LcUiEngine_localplaying;
}

bool LcUiEnginePrivate::isRemotePlayerPlaying() const
{
    return lcutStub_LcUiEngine_remoteplaying;
}

const QString& LcUiEnginePrivate::sessionTitle() const
{
    return mSessionTitle;
}
const QString& LcUiEnginePrivate::recipient() const
{
    return mRecipient;
}

const QString& LcUiEnginePrivate::phoneNumber() const
{
    return mPhoneNumber;
}

LcViewLayoutHint LcUiEnginePrivate::viewLayout() const
{
    return (LcViewLayoutHint)lcutStub_LcUiEngine_layout;
}

void LcUiEnginePrivate::setUiComponents(HbProgressDialog* invitingNote,
                                 HbProgressDialog* waitingNote,
                                 HbDialog* acceptQuery,
                                 HbDialog* recipientQuery,
                                 HbDialog* sharedVideoQuery)
{
    mInvitingNote = invitingNote;
    mWaitingNote = waitingNote;
    mAcceptQuery = acceptQuery;
    mRecipientQuery = recipientQuery;
    mShareOwnVideoQuery = sharedVideoQuery;
    
}

void LcUiEnginePrivate::startReceiving()
{
    
}

void LcUiEnginePrivate::stopForcefully()
{

}

void LcUiEnginePrivate::emitViewLayoutChanged()
{
    
}

bool LcUiEnginePrivate::usingVideoTelephoneEngine()
{
    return mEngineName == "Videotelephony";
}

void LcUiEnginePrivate::setMainCamera(const bool aMainCamera)
{
    mMainCamera = aMainCamera;    
}

bool LcUiEnginePrivate::mainCamera() const
{
    return mMainCamera;
}

bool LcUiEnginePrivate::isMuted() const
{
    return lcutStub_LcUiEngine_muted;
}

bool LcUiEnginePrivate::isSpeakerOn() const
{
    return lcutStub_LcUiEngine_speakerOn;
}

void LcUiEnginePrivate::toggleMute()
{
    lcutStub_LcUiEngine_muted = !lcutStub_LcUiEngine_muted;
    lcutStub_LcUiEngine_calledMethodsInstance().insert(0,lcutStub_LcUiEngine_mute );
}

void LcUiEnginePrivate::toggleCamera()
{
    lcutStub_LcUiEngine_cameraMain = !lcutStub_LcUiEngine_cameraMain;
    lcutStub_LcUiEngine_calledMethodsInstance().insert(0,lcutStub_LcUiEngine_toggleCamera );
}

void LcUiEnginePrivate::toggleDisableCamera()
{
    lcutStub_LcUiEngine_calledMethodsInstance().insert(0,lcutStub_LcUiEngine_disableCamera );
}

void LcUiEnginePrivate::toggleSpeaker()
{
    lcutStub_LcUiEngine_speakerOn = !lcutStub_LcUiEngine_speakerOn;
    lcutStub_LcUiEngine_calledMethodsInstance().insert(0,lcutStub_LcUiEngine_speaker );
}

int LcUiEnginePrivate::zoomValues( LcControlValues& values ) const
{
    values.mMaxValue = 3;
    values.mMinValue = 1;
    values.mValue = 2;
    return 0;
}

void LcUiEnginePrivate::timerEvent(QTimerEvent */*event*/)
{
}

void LcUiEnginePrivate::StateChanged( MLcSession& /*aSession*/ )
{
    
}

void LcUiEnginePrivate::StateChanged( MLcVideoPlayer& /*aPlayer*/ )
{
    
}

void LcUiEnginePrivate::Updated( MLcSession& /*aSession*/ )
{
    
}

void LcUiEnginePrivate::Updated( MLcVideoPlayer& /*aPlayer*/ )
{
    
}


void LcUiEnginePrivate::Failed( MLcSession& /*aSession*/, TInt /*aError*/ )
{
    
}

void LcUiEnginePrivate::Failed( MLcVideoPlayer& /*aPlayer*/, TInt /*aError*/ )
{
    
}

void LcUiEnginePrivate::SessionTimeChanged( MLcSession& /*aSession*/, 
                                     const TTimeIntervalSeconds& /*aSeconds*/ )
{
    
}

TBool LcUiEnginePrivate::SelectRecipient( MDesCArray& /*aRecipientAddresses*/, 
                                   TDes& /*aSelectedRecipientAddress*/ )        
{
    return true;
}

TBool LcUiEnginePrivate::InputRecipient( TDes& /*aRecipientAddress*/ )
{
    return true;
    
}

void LcUiEnginePrivate::HandleForegroundStatus( TBool /*aForeground*/ )
{
    
}

void LcUiEnginePrivate::BlockUi( TBool /*aBlocked*/ )
{

}


bool LcUiEnginePrivate::fullScreenMode()
{
    return mFullScreen;
}


void LcUiEnginePrivate::setFullScreenMode(bool aMode)
{
    mFullScreen = aMode;
}

bool LcUiEnginePrivate::isMinimized()const
{
    return lcutStub_LcUiEngine_windowState_minimized;
}

void LcUiEnginePrivate::minimize()
{
    lcutStub_LcUiEngine_windowState_minimized = true;
}

void LcUiEnginePrivate::maximize()
{
    lcutStub_LcUiEngine_windowState_minimized = false;
}

void LcUiEnginePrivate::setCurrentView(HbView* view)
{
    lcutStub_LcUiEngine_currentView = view;
}

void LcUiEnginePrivate::handleEngineForegroundStatus()
{
}
 
bool LcUiEnginePrivate::SendDialTone(const QChar aKey )
{
    lcStub_LcUiEngine_dialtone = aKey;
    return true;
}

//stub control

bool lcutStub_LcUiEngine_expectCall( const QString& call, int callNdx )
{
    if ( callNdx >= lcutStub_LcUiEngine_calledMethodsInstance().count() ) {
        return call == lcutNoCall;
    }
    return lcutStub_LcUiEngine_calledMethodsInstance().count() > 0 ?
            lcutStub_LcUiEngine_calledMethodsInstance()[callNdx] == call :
           call == lcutNoCall;
}

    
void lcutStub_LcUiEngine_reset()
{
    delete lcutStub_LcUiEngine_calledMethods;
    lcutStub_LcUiEngine_calledMethods = 0;
    lcutStub_LcUiEngine_failure = 0;
    lcutStub_LcUiEngine_layout = -1;
    lcutStub_LcUiEngine_cameraMain = false;
    lcutStub_LcUiEngine_muted = false;
    lcutStub_LcUiEngine_speakerOn = false;
    lcutStub_LcUiEngine_localplaying = true;
    lcutStub_LcUiEngine_remoteplaying = true;
    lcutStub_LcUiEngine_currentView = 0;
    lcStub_LcUiEngine_dialtone = '0';
}

void lcutStub_LcUiEngine_setFailure( int failure )
{
    lcutStub_LcUiEngine_failure = failure;
}

void lcutStub_LcUiEngine_setLayout( int layout )
{
    lcutStub_LcUiEngine_layout = layout;
}

QStringList& lcutStub_LcUiEngine_calledMethodsInstance()
{
    if ( !lcutStub_LcUiEngine_calledMethods ) {
        lcutStub_LcUiEngine_calledMethods = new QStringList();
    }
    return *lcutStub_LcUiEngine_calledMethods;
}

void lcutStub_LcUiEngine_setCameraMain( bool val )
{
    lcutStub_LcUiEngine_cameraMain = val;
}

void lcutStub_LcUiEngine_setMuted( bool val )
{
    lcutStub_LcUiEngine_muted = val;
}

void lcutStub_LcUiEngine_setSpeaker( bool val )
{
    lcutStub_LcUiEngine_speakerOn = val;
}

void lcutStub_LcUiEngine_setLocalPlaying( bool val )
{
    lcutStub_LcUiEngine_localplaying = val;
}

void lcutStub_LcUiEngine_setRemotePlaying( bool val )
{
    lcutStub_LcUiEngine_remoteplaying = val;
}

HbView* lcutStub_LcUiEngine_getCurrentView()
{
    return lcutStub_LcUiEngine_currentView;
}


QChar lcutStub_LcUiEngine_getDialToneChar()
{
    return lcStub_LcUiEngine_dialtone;
}


void LcUiEnginePrivate::stopLocalVideo()
{
    lcutStub_LcUiEngine_localplaying = false;
}

void LcUiEnginePrivate::startLocalVideo()
{
    lcutStub_LcUiEngine_localplaying = true;
}

void LcUiEnginePrivate::volumeLevelChanged(const XQSettingsKey& /*aKey*/, const QVariant& /*aValue*/)
{
}

// End of file



