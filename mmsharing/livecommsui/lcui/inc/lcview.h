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
class HbZoomSliderPopup;
class HbTransparentWindow;
class HbMenu;
class LcVideoWidget;
class LcEffectHandler;
class Dialpad;

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
    void showControl();
    void hideControl();
    QString currentLayout();
    void deActivateFullScreen();
    void activatePortrait();
    void updateUiElements();

public slots:

    void endVideoSession();    
    void disableCamera();
    void mute();
    void changeCamera();    
    void switchToVoiceCall();    
    void speaker();
    void showZoom();
    void activateFullScreen();
    void updateVideoRects();
    void swap();
    void shareImage();
    void updateSwapLayout();
    void menuAboutToShow();
    void menuAboutToHide();
    void back();
//temp
    void notSupported();
    void changeOrientation_Temporary();


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
    void landscapeTimerTimeout();

protected: // From HbView
    
    void gestureEvent(QGestureEvent *event);
 
protected slots:
    void openDialpad();    
    virtual void dialpadOpened();
    virtual void dialpadClosed();
    virtual void dialpadEditorTextChanged();
    
protected:
    void setDialpadPosition();
    void enableDialpadCallButton( bool enable );
    
private:
    void createNotSupportedNote();
    void createContextMenu();
    bool isLandscapeOrientation();
    QRectF translateRectForOrientation(const QRectF& origRect);
    QPointF translatePointForOrientation(const QPointF& origPoint);
    void setVisibility( QGraphicsItem* item, bool visible );
    void resetLandscapeTimer();
    void addOptionsMenuActions();
    void gestureLongPress( QPointF coords );
    void gestureShortPress();

private://not owned
    
    LcUiEngine& mEngine;
    LcUiComponentRepository& mRepository;
    HbLabel* mRecipient;
    HbLabel* mDuration;
    HbLabel* mContactIcon;
    HbLabel* mBrandIcon;
    LcVideoWidget* mSharedVideoWidget;
    LcVideoWidget* mReceivedVideoWidget;
    HbAction* mChangeCameraAction;
    HbAction* mMuteAction;
    HbAction* mSpeakerAction;
    HbAction* mDisableCameraAction;
    HbAction* mDisableCameraMenuAction;
    HbMessageBox* mNotSupportedNote;
    HbZoomSliderPopup* mZoomSlider;
    HbPushButton* mEndCallButton; 

    LcVideoWidget* mReceivedVideoEffectOverlay;
    LcVideoWidget* mSharedVideoEffectOverlay;
    LcEffectHandler* mEffectHandler;

private:

    HbMenu* mItemContextMenu;
    QTimer* mLandscapeTimer;
    bool mIsOptionMenuOpen;
    HbAction* mSoftKeyBackAction;
    Dialpad* mDialpad; // Not owned
};


#endif // LCVIEW_H
