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
#ifndef LCVIEW_H
#define LCVIEW_H

#include <hbview.h>

//temp
#define LC_UNMUTE_TEXT "Unmute"
#define LC_MUTE_TEXT "Mute"
#define LC_HANDSET_TEXT "Handset"
#define LC_SPEAKER_TEXT "Speaker"

class LcUiEngine;
class LcUiComponentRepository;
class HbLabel;
class HbMessageBox;
class HbAction;
class HbPushButton;
class HbTransparentWindow;
class HbMenu;
class LcVideoWidget;
class LcEffectHandler;
class Dialpad;
class DialpadVtKeyHandler;

class LcView : public HbView    
{
    Q_OBJECT
    friend class UT_LcView;
    friend class UT_LcUiComponentRepository;    
    
public:    

    explicit LcView(LcUiEngine& engine, LcUiComponentRepository& repository);
    ~LcView();

public:
    
    void init();
    void activated();
    void deactivated();
    QString currentLayout();
    
public slots:

    void endVideoSession();    
    void disableCamera();
    void mute();
    void changeCamera();    
    void switchToVoiceCall();    
    void speaker();
    void updateVideoRects();
    void swap();
    void shareImage();
    void updateSwapLayout();
    void menuAboutToShow();    
    void back();
//temp
    void notSupported();

private slots:

    void setMuteActionToUnmute();
    void setMuteActionToMute();
    void setCameraActionToMain();
    void setCameraActionToSecondary();
    void setSpeakerActionToHandset();
    void setSpeakerActionToSpeaker();
    void setCameraActionToDisable();
    void setCameraActionToEnable();
    void disableControls();
    void enableControls();    
    void watchInactivity();

protected: // From HbView
    
    void gestureEvent(QGestureEvent *event);
 
protected slots:
    void openDialpad();    
    virtual void dialpadOpened();
    virtual void dialpadClosed();
    virtual void dialpadEditorTextChanged();
    
protected:
    void setDialpadPosition();
    
private: // fullscreen
    void timerEvent( QTimerEvent * event );
    void toFullScreen( bool fullscreen );
    
private:
    void createNotSupportedNote();
    void createContextMenu();
    QRectF translateRectForOrientation(const QRectF& origRect);
    QPointF translatePointForOrientation(const QPointF& origPoint);
    void setVisibility( QGraphicsItem* item, bool visible );    
    void addOptionsMenuActions();
    void gestureLongPress( QPointF coords );
    void gestureShortPress();
    bool isVideoPositionedCorrectly( LcVideoWidget* mVideoWidget );
    bool isPositioned();

private://not owned
    
    LcUiEngine& mEngine;
    LcUiComponentRepository& mRepository;
    HbLabel* mRecipient;
    HbLabel* mDuration;
    HbLabel* mContactIcon;    
    LcVideoWidget* mSharedVideoWidget;
    LcVideoWidget* mReceivedVideoWidget;
    HbAction* mChangeCameraAction;
    HbAction* mMuteAction;
    HbAction* mSpeakerAction;
    HbAction* mDisableCameraAction;
    HbAction* mDisableCameraMenuAction;
    HbMessageBox* mNotSupportedNote;
    HbPushButton* mEndCallButton; 

    LcVideoWidget* mReceivedVideoEffectOverlay;
    LcVideoWidget* mSharedVideoEffectOverlay;
    LcEffectHandler* mEffectHandler;

private:

    HbMenu* mItemContextMenu;    
    bool mIsOptionMenuOpen;
    Dialpad* mDialpad; // Not owned
    DialpadVtKeyHandler* mDialpadVtKeyHandler; 
    int timerId;
    bool isViewReady;
};


#endif // LCVIEW_H
