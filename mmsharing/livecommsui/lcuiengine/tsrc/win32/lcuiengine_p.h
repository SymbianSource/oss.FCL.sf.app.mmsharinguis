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
#ifndef LCUIENGINEPRIVATE_H
#define LCUIENGINEPRIVATE_H

//SYSTEM
#include <QObject>
#include <QString>
#include <QTime>
#include <QProcess>
#include <lcuiengine.h>

class QTimer;
class QRectF;
class HbDialog;
class LcUiEngine;
class LcUiEngineSimulator;

enum simulationState {
    ENone,
    EPreparing,
    EBuffering,
    EPlaying,
    EPaused,
    EStopped,
    EUnavailable
};

/*
 * Live Comms UI Engine 
 */
class LcUiEnginePrivate
                  
{

public:
    
    LcUiEnginePrivate ( LcUiEngine& uiEngine,
                        const QString &engineName,
                        const QString &applicationDisplayName );
    ~LcUiEnginePrivate ();
    
public :

    void zoom( int value );
    void stop();

private:

    //signaled when engine is stopped
    void stopped(){ emit mUiEngine.stopped();};
    //signaled when layout is changed
    void layoutChanged(){ emit mUiEngine.layoutChanged();};
    //signaled when camera changes to main
    void cameraChangedToMain();
    //signaled when camera changes to secondary
    void cameraChangedToSecondary();
    //signaled when mic is muted
    void muted(){ mMuteState = true; emit mUiEngine.muted();};
    //signaled when mic is unmuted
    void unmuted(){ mMuteState = false; emit mUiEngine.unmuted();};
    //signaled when speaker is enabled
    void speakerEnabled(){ mSpeakerState = true; emit mUiEngine.speakerEnabled();};
    //signaled when speaker is disabled
    void speakerDisabled(){ mSpeakerState = false; emit mUiEngine.speakerDisabled();};
    //signaled when camera is disabled
    void cameraDisabled(){ mPlayerStateLocal = EPaused; emit mUiEngine.cameraDisabled();};
    //signaled when camera is enabled
    void cameraEnabled(){  mPlayerStateLocal = EPlaying; emit mUiEngine.cameraEnabled();};
    //signaled when engine is blocked,
    //no calls expected until signaled unblocked
    void blocked(){ emit mUiEngine.blocked();};
    //signaled when engine is free
    void unblocked(){ emit mUiEngine.unblocked();};
    //signaled continuously while session is up and running
    //carries current session duration
    void sessionDurationChanged(const QString &duration){ emit mUiEngine.sessionDurationChanged(duration);};
    void recipientChanged(const QString &recipient){ emit mUiEngine.recipientChanged(recipient);};
    void phoneNumberChanged(const QString &number){ emit mUiEngine.phoneNumberChanged(number);};
    //signaled when player state changed
    void localPlayerPlaying();
    void remotePlayerPlaying();
    void localPlayerPreparing(){ mPlayerStateLocal=EPreparing;emit mUiEngine.localPlayerPreparing();};
    void remotePlayerPreparing(){ mSimulatorRemote=EPreparing;emit mUiEngine.remotePlayerPreparing();};
    void localPlayerBuffering(){ mPlayerStateLocal=EBuffering;emit mUiEngine.localPlayerBuffering();};
    void remotePlayerBuffering(){ mSimulatorRemote=EBuffering;emit mUiEngine.remotePlayerBuffering();};
    void localPlayerPaused();
    void remotePlayerPaused();
    void localPlayerUnavailable(){ mPlayerStateLocal=EUnavailable;emit mUiEngine.localPlayerUnavailable();};
    void remotePlayerUnavailable(){ mSimulatorRemote=EUnavailable;emit mUiEngine.remotePlayerUnavailable();};

public:
    
    void start();
    const QString& sessionTitle() const;
    const QString& recipient() const;
    const QString& phoneNumber() const;
    LcViewLayoutHint viewLayout() const;
    void setContentAreas( const QRectF& sharedContent,
                          const QRectF& receivedContent );
    bool setOrientation( Qt::Orientation orientation );
    void setUiComponents(HbDialog* invitingNote,
                         HbDialog* waitingNote,
                         HbDialog* acceptQuery,
                         HbDialog* recipientQuery);
    void toggleMute();
    bool isMuted() const;
    void toggleSpeaker();
    bool isSpeakerOn() const;
    int zoomValues( LcControlValues& values ) const;
    void toggleDisableCamera();
    void setMainCamera(const bool aMainCamera);
    bool mainCamera() const;
    void toggleCamera();
    bool fullScreenMode();
    void setFullScreenMode(bool aMode);
    void updateSession();
    bool isLocalPlayerPlaying() const;
    bool isRemotePlayerPlaying() const;
    bool isMinimized()const;
    void minimize();
    void maximize();
    void shareImage(const QString&);
    void shareCamera();

public: // Data
    
    QByteArray mEngineName;
    QString mSessionTitle;
    QString mRecipient;
    QString mPhoneNumber;
    LcViewLayoutHint mViewLayout;
    QTimer* mCloseTimer;
    HbDialog* mInvitingNote; // Not owned
    HbDialog* mWaitingNote; // Not owned
    HbDialog* mAcceptQuery; // Not owned
    HbDialog* mRecipientQuery; // Not owned
    QString mPendingAction;
    bool mBlocked;
    bool mFullScreen;
    int mSessionDurationTimerId;
    QTime mSessionDurationStartTime;    
    LcUiEngine& mUiEngine;

public:
    simulationState mPlayerStateLocal;
    simulationState mSimulatorRemote;
    bool mSpeakerState;
    bool mMuteState;
    bool mMainCamera;    
    QProcess *mLocalPlayerProcess;
    QProcess *mRemotePlayerProcess;
    QString mProgram;
    QString mProgramImage;
    QString mLocalMovieName;

private:
    LcUiEngineSimulator* mSimulator;

friend class LcUiEngineSimulator;
};

#endif // LCUIENGINEPRIVATE_H


// End of file
