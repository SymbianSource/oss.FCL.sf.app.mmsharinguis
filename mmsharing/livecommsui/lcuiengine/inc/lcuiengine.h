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
#ifndef LCUIENGINE_H
#define LCUIENGINE_H

//USER

//SYSTEM
#include <QObject>
#include <QString>
#include <QTime>

class QTimer;
class HbProgressDialog;
class HbDialog;
class QRectF;
class LcUiEnginePrivate;
class HbView;

const char lcEngSignalNameMuted[] = "muted";
const char lcEngSignalNameUnmuted[] = "unmuted";
const char lcEngSignalNameSpeakerEnabled[] = "speakerEnabled";
const char lcEngSignalNameSpeakerDisabled[] = "speakerDisabled";
const char lcEngSignalNameCameraChangedToMain[] = "cameraChangedToMain";
const char lcEngSignalNameCameraChangedToSecondary[] = "cameraChangedToSecondary";
const char lcEngSignalNameCameraEnabled[] = "cameraEnabled";
const char lcEngSignalNameCameraDisabled[] = "cameraDisabled";


/*
 * Supported views
 */
enum LcViewLayoutHint {
    Lc_NoViewLayout = -1,
    Lc_IdleViewLayout,
    Lc_SharedViewLayout,
    Lc_ReceivedViewLayout,
    Lc_ReceivedOnlyViewLayout,
    Lc_TwowayViewLayout,
    Lc_AllInOneViewLayout
};


/*
 * Struct of values for control
 */
class LcControlValues {
public:
  int mMaxValue;
  int mMinValue;
  int mValue;
};

/*
 * Live Comms UI Engine 
 */
class LcUiEngine : public QObject
{
    Q_OBJECT
    friend class UT_LcUiEngine;

public:
    //constructor
    explicit LcUiEngine(const QString &engineName,
                        const QString &applicationDisplayName);
    //destructor
    ~LcUiEngine();
    
signals:
  
    //signaled when engine is stopped
    void stopped();
    //signaled when layout is changed
    void layoutChanged();
    //signaled when camera changes to main
    void cameraChangedToMain();
    //signaled when camera changes to secondary
    void cameraChangedToSecondary();
    //signaled when mic is muted
    void muted();
    //signaled when mic is unmuted
    void unmuted();
    //signaled when speaker is enabled
    void speakerEnabled();
    //signaled when speaker is disabled
    void speakerDisabled();
    //signaled when camera is disabled
    void cameraDisabled();
    //signaled when camera is enabled
    void cameraEnabled();
    //signaled when engine is blocked, 
    //no calls expected until signaled unblocked    
    void blocked();
    //signaled when engine is free 
    void unblocked();
    //signaled continuously while session is up and running
    //carries current session duration
    void sessionDurationChanged(const QString &duration);
    void recipientChanged(const QString &recipient);
    void phoneNumberChanged(const QString &number);    
    //signaled when player state changed
    void localPlayerPlaying();
    void remotePlayerPlaying();
    void localPlayerPreparing();
    void remotePlayerPreparing();
    void localPlayerBuffering();
    void remotePlayerBuffering();
    void localPlayerPaused();
    void remotePlayerPaused();
    void localPlayerUnavailable();
    void remotePlayerUnavailable();
    
public slots:

    void zoom( int value );
    void stop();
    void minimize();
    void maximize();   
        
public: // API
    
    void start();
    const QString& sessionTitle() const;
    const QString& recipient() const;
    const QString& phoneNumber() const;
    LcViewLayoutHint viewLayout() const;
    void setContentAreas(const QRectF& sharedContent,
                         const QRectF& receivedContent);
    bool setOrientation(Qt::Orientation orientation);
    void setUiComponents(HbProgressDialog* invitingNote,
                         HbProgressDialog* waitingNote,
                         HbDialog* acceptQuery,
                         HbDialog* recipientQuery,
                         HbDialog* shareOwnVideoQuery);
    void toggleMute();
    bool isMuted() const;
    void toggleSpeaker();
    bool isSpeakerOn() const;
    int zoomValues(LcControlValues& values) const;
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
    bool isMaximized()const{return !isMinimized();}; 
    void shareImage(const QString&);
    void shareCamera();
    void setCurrentView(HbView* view);
    bool SendDialTone( const QChar aKey );
   

private:

    LcUiEnginePrivate* d;

friend class LcUiEnginePrivate; // access to signals
};

#endif // LCUIENGINE_H


// End of file
