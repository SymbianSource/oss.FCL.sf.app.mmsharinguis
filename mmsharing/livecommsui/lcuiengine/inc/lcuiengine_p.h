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

//USER
#include "lcsessionobserver.h"
#include "lcuiprovider.h"

//SYSTEM
#include <QObject>
#include <QString>
#include <QTime>
#include <XQSettingsKey>
#include <XQSettingsManager>
#include <QVariant>
#include <lcuiengine.h>
#include <lcengine.h>


class MLcSession;
class MLcAudioControl;
class MLcZoomControl;
class HbDialog;
class HbMessageBox;
class QTimer;
class QRectF;
class LcUiEngine;
class HbView;
class LcActivityManager;

/*
 * Live Comms UI Engine 
 */
class LcUiEnginePrivate : public QObject, 
                   public MLcSessionObserver,
                   public MLcUiProvider
{
    Q_OBJECT
    friend class UT_LcUiEngine;

public:
    
    explicit LcUiEnginePrivate(LcUiEngine& uiEngine,
                               const QString &engineName,
                               const QString &applicationDisplayName);
    ~LcUiEnginePrivate();
    
public slots:

    void zoom( int value );
    void stop();
    void stopLocalVideo();
    void startLocalVideo();
        
public: // API
    
    void start();
    const QString& sessionTitle() const;
    const QString& recipient() const;
    const QString& phoneNumber() const;
    LcViewLayoutHint viewLayout() const;
    void setContentAreas(const QRectF& sharedContent,
                         const QRectF& receivedContent);
    bool setOrientation(Qt::Orientation orientation );
    void setUiComponents(HbProgressDialog* invitingNote,
                         HbProgressDialog* waitingNote,
                         HbDialog* acceptQuery,
                         HbDialog* recipientQuery,
                         HbDialog* shareOwnVideoQuery );
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
    void shareImage(const QString&){};
    void shareCamera(){};
    void setCurrentView(HbView* view);
    bool SendDialTone(const QChar aKey );
    
protected: // From QObject
    
    void timerEvent(QTimerEvent *event);

private slots:
    
    void startReceiving();
    void stopForcefully();
    void handleEngineForegroundStatus();
    void volumeLevelChanged(const XQSettingsKey& aKey, const QVariant& aValue);
    
private: // From MLcSessionObserver
    
    void StateChanged( MLcSession& aSession );
    void StateChanged( MLcVideoPlayer& aPlayer );
    void Updated( MLcSession& aSession );
    void Updated( MLcVideoPlayer& aPlayer );
    void Failed( MLcSession& aSession, TInt aError );
    void Failed( MLcVideoPlayer& aPlayer, TInt aError );
    void SessionTimeChanged( MLcSession& aSession, const TTimeIntervalSeconds& aSeconds );

private: // From MLcUiProvider

    TBool SelectRecipient( MDesCArray& aRecipientAddresses, TDes& aSelectedRecipientAddress );        
    TBool InputRecipient( TDes& aRecipientAddress );
    void HandleForegroundStatus( TBool aForeground );
    void BlockUi( TBool aBlocked );    

private: // New functions
    
    LcViewLayoutHint setViewLayout();
    void emitViewLayoutChanged();
    void startLiveContent();
    void startPlayback(); 
    void startRemoteVideo();
    void startCloseTimer();
    void cancelCloseTimer();
    void hideNotes( bool hideWaitingNote );
    void completeAction( const char* actionMethod );
    void setPendingAction( const char* actionMethod );
    void completePendingAction();
    bool usingVideoTelephoneEngine();
    MLcSession& session() const;
    MLcVideoPlayer* visibleLocalPlayer() const;
    MLcVideoPlayer* visibleRemotePlayer() const;
    MLcAudioControl* audioControl() const;
    MLcZoomControl* zoomControl() const;
    bool featureSupported( CLcEngine::TLcFeature feature );
    void startSessionDurationTimer();
    void stopSessionDurationTimer();
    void fillRemoteInfo(bool informChanges);
    void startStopGuardTimer();
    void showSendVideoQueryWhenNecessary();

    void subscribeVolumeEvents();
    void unSubscribeVolumeEvents();
    void doUpdate( MLcVideoPlayer& aPlayer );
    void doUpdate( MLcSession& aSession );
    
private: // New functions wrapping the leaving functions in LC plug-in API
    
    int establishSession();
    int terminateSession();
    int setWindowRect( MLcVideoPlayer* player, TRect rect );
    int enableWindow( MLcVideoPlayer* player, bool enable );
    int play( MLcVideoPlayer* player );
    int pause( MLcVideoPlayer* player );
    int enableSpeaker( bool enable );
    int muteMic( bool mute );

private: // Data
    
    QByteArray mEngineName;
    QString mSessionTitle;
    QString mRecipient;
    QString mPhoneNumber;
    LcViewLayoutHint mViewLayout;
    CLcEngine* mLiveCommsEngine;
    QTimer* mCloseTimer;
    HbProgressDialog* mInvitingNote; // Not owned
    HbProgressDialog* mWaitingNote; // Not owned
    HbDialog* mAcceptQuery; // Not owned
    HbDialog* mRecipientQuery; // Not owned
    HbDialog* mShareOwnVideoQuery; // Not Owned
    QString mPendingAction;
    bool mBlocked;
    bool mFullScreen;
    int mSessionDurationTimerId;
    QTime mSessionDurationStartTime;
    bool mMainCamera;
    LcUiEngine& mUiEngine;
    bool mIsMinimized;
    bool mFirstForegroundSwitch;
    HbView* mCurrentView; // Not owned
    TRect mLocalRect;
    TRect mRemoteRect;
    LcActivityManager* mActivityManager;
    
    XQSettingsManager* mSettingsMgr;
    XQSettingsKey* mEarVolumeKey; 
    XQSettingsKey* mLoudSpeakerKey; 
    
friend class LcUiEngine;    
};

#endif // LCUIENGINEPRIVATE_H


// End of file
