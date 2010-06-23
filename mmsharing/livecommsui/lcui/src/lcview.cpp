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
#include "lcview.h"
#include "lclogger.h"
#include "lcuiengine.h"
#include "lcuidefs.h"
#include "lcuicomponentrepository.h"
#include "lceffecthandler.h"
#include "lcvideowidget.h"

//SYSTEM
#include <hblabel.h>
#include <hbdialog.h>
#include <hbtransparentwindow.h>
#include <hbmessagebox.h>
#include <hbaction.h>
#include <hbmenu.h>
#include <hbpushbutton.h>
#include <hbtoolbar.h>
#include <hbinstance.h>
#include <hbzoomsliderpopup.h>
#include <hbeffect.h>
#include <qgraphicssceneevent.h>
#include <qtimer.h>
#include <hblineedit.h>
#include <dialpad.h>
#include <HbTapGesture>



#if ( defined __WINSCW__ ) || ( defined __WINS__ )
const int inActivityTimeout = 5000; //5 secs
#else
const int inActivityTimeout = 2000; //2 secs
#endif

// -----------------------------------------------------------------------------
// LcView::LcView
// -----------------------------------------------------------------------------
//
LcView::LcView(LcUiEngine& engine, LcUiComponentRepository& repository)
 : HbView(0),
   mEngine(engine),
   mRepository(repository),
   mRecipient(0),
   mDuration(0),
   mContactIcon(0),
   mBrandIcon(0),
   mSharedVideoWidget(0),
   mReceivedVideoWidget(0),
   mChangeCameraAction(0),
   mMuteAction(0),
   mSpeakerAction(0),
   mDisableCameraAction(0),
   mDisableCameraMenuAction(0),
   mNotSupportedNote(0),
   mZoomSlider(0),
   mEndCallButton(0),
   mReceivedVideoEffectOverlay(0),
   mSharedVideoEffectOverlay(0),
   mEffectHandler(0),
   mItemContextMenu(0),   
   mIsOptionMenuOpen(false),
   mSoftKeyBackAction(0),
   mDialpad(0),
   timerId(0)
{
    LC_QDEBUG( "livecomms [UI] -> LcView::LcView()" )
    
    createNotSupportedNote();

    // For LiveComms views Dock Wiget Item is explicitly hidden.
    hideItems(Hb::DockWidgetItem);
    
    //Subscribe to the gesture events.
    grabGesture(Qt::TapGesture);        

    LC_QDEBUG( "livecomms [UI] <- LcView::LcView()" )
}


// -----------------------------------------------------------------------------
// LcView::~LcView
// -----------------------------------------------------------------------------
//
LcView::~LcView()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::~LcView()" )

    // Un-subscribe to the gesture events.
    ungrabGesture(Qt::TapGesture);        

    delete mEffectHandler;    
    delete mNotSupportedNote;

    LC_QDEBUG( "livecomms [UI] <- LcView::~LcView()" )
}

// -----------------------------------------------------------------------------
// LcView::init
// -----------------------------------------------------------------------------
//
void LcView::init()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::init()" )
    //find member widgets from repository
    //( can be also found by browsing this view itself )
    
    //due stubs in unit tests, qobject_cast cannot be used. Using static_cast instead
    mRecipient = 
        static_cast<HbLabel*>( mRepository.findWidget( lcLabelRecipientId ) );
    mDuration = 
        static_cast<HbLabel*>( mRepository.findWidget( lcLabelDurationId ) );
    mContactIcon = 
        static_cast<HbLabel*>( mRepository.findWidget( lcIconContactId ) );
    mBrandIcon = 
        static_cast<HbLabel*>( mRepository.findWidget( lcIconBrandId ) );
    mSharedVideoWidget = 
        static_cast<LcVideoWidget*>( 
                mRepository.findWidget( lcWidgetSendVideoId ) );
    mReceivedVideoWidget = 
        static_cast<LcVideoWidget*>( 
                mRepository.findWidget( lcWidgetRecvVideoId ) );
    mChangeCameraAction = 
        static_cast<HbAction*>( mRepository.findObject( lcActChangeCameraId ) );
    mMuteAction = 
        static_cast<HbAction*>( mRepository.findObject( lcActMuteId ) );
    mSpeakerAction = 
        static_cast<HbAction*>( mRepository.findObject( lcActSpeakerId ) );
    mDisableCameraAction = 
        static_cast<HbAction*>( mRepository.findObject( lcActDisableCameraId ) );
        
    mDisableCameraMenuAction =
        static_cast<HbAction*>( mRepository.findObject( lcActMenuDisableCameraId ) );
    
    mEndCallButton =
            static_cast<HbPushButton*>( mRepository.findObject( lcButtonEndCall ) );
    
    if ( mEndCallButton ) {
        connect( mEndCallButton, SIGNAL(clicked()), SLOT(endVideoSession()));
    }

    if ( mSharedVideoWidget ) {
        mZoomSlider = mRepository.zoomSlider();
        mSharedVideoEffectOverlay = static_cast<LcVideoWidget*>( 
                mRepository.findWidget( lcWidgetSendVideoId2 ) );
        if ( mSharedVideoEffectOverlay ) {
            mSharedVideoEffectOverlay->show();
        }
    }

    mReceivedVideoEffectOverlay = 
        static_cast<LcVideoWidget*>( mRepository.findWidget( lcWidgetRecvVideoId2 ) );
        
    if ( mReceivedVideoEffectOverlay ) {
        mReceivedVideoEffectOverlay->show();
    }

    // swap
    mEffectHandler = new LcEffectHandler( mEngine,
            mSharedVideoWidget, mSharedVideoEffectOverlay, 
            mReceivedVideoWidget, mReceivedVideoEffectOverlay);
    connect( mEffectHandler, SIGNAL(swapCompleted()), this, SLOT(updateVideoRects()) );
    connect( mEffectHandler, SIGNAL(loadSwapLayout()), this, SLOT(updateSwapLayout()) );
    
    // menu
    if ( menu()) {
        connect( menu(), SIGNAL(aboutToShow()), this, SLOT(menuAboutToShow()) );
        connect( menu(), SIGNAL(aboutToHide()), this, SLOT(watchInactivity()) );        
    }     
    mSoftKeyBackAction = new HbAction(Hb::BackNaviAction, this);    
    connect(mSoftKeyBackAction, SIGNAL(triggered()), SLOT(back()));
    
    // dialpad
    mDialpad = static_cast<Dialpad*> ( mRepository.findWidget("lc_label_dialpad") );
    connect( mDialpad, SIGNAL(aboutToClose()), SLOT(dialpadClosed()) );
    connect( mDialpad, SIGNAL(aboutToOpen()), SLOT(dialpadOpened()) );
    connect( &mDialpad->editor(), SIGNAL( contentsChanged() ), SLOT( dialpadEditorTextChanged() ) );
        
    // inactivity
    timerId = this->startTimer( inActivityTimeout );    
    connect( this, SIGNAL( contentFullScreenChanged() ), SLOT( watchInactivity() ) ) ;
    
    // activate fullscreen at beginning if inactivity timesout
    toFullScreen( true );
    
    LC_QDEBUG( "livecomms [UI] <- LcView::init()" )
}


// -----------------------------------------------------------------------------
// LcView::updateVideoRects
// -----------------------------------------------------------------------------
//
void LcView::updateVideoRects()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::updateVideoRects()" )

    QRectF sharedContentRect;
    QRectF receivedContentRect;

    if ( mSharedVideoWidget ) {
        sharedContentRect = mSharedVideoWidget->geometry();  
        sharedContentRect.moveTop(
                sharedContentRect.y() + mapToParent(scenePos()).y());
        sharedContentRect = translateRectForOrientation(sharedContentRect);
        mEffectHandler->setVisibility(
                mSharedVideoWidget, mEngine.isLocalPlayerPlaying());
    }
    
    if ( mReceivedVideoWidget ) {
        receivedContentRect = mReceivedVideoWidget->geometry();
        receivedContentRect.moveTop(
                receivedContentRect.y() + mapToParent(scenePos()).y());
        receivedContentRect = translateRectForOrientation(receivedContentRect);
        mEffectHandler->setVisibility(
                mReceivedVideoWidget, mEngine.isRemotePlayerPlaying());
    }
    
    mEngine.setContentAreas( sharedContentRect, receivedContentRect );
    mEngine.setOrientation( HbInstance::instance()->allMainWindows().at(0)->orientation() );
    mEngine.updateSession();
    
    LC_QDEBUG( "livecomms [UI] <- LcView::updateVideoRects()" )
}

// -----------------------------------------------------------------------------
// LcView::swap
// -----------------------------------------------------------------------------
//
void LcView::swap()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::swap()" )
    if ( mEffectHandler ){
        mEffectHandler->swap();
    }
    LC_QDEBUG( "livecomms [UI] <- LcView::swap()" )
}


// -----------------------------------------------------------------------------
// LcView::updateSwapLayout
// -----------------------------------------------------------------------------
//

void LcView::updateSwapLayout()
{
    mRepository.loadLayout( currentLayout() );
    toFullScreen( true );
}

// -----------------------------------------------------------------------------
// LcView::activated
// -----------------------------------------------------------------------------
//
void LcView::activated()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::activated()" )
		
    if ( navigationAction() != mSoftKeyBackAction ) {
        setNavigationAction(mSoftKeyBackAction);
    }

    updateVideoRects();

    //synchronize with engine
    
    if ( mEngine.mainCamera() ) {
        setCameraActionToSecondary();
    } else {
        setCameraActionToMain();
    }
    
    if ( mEngine.isMuted() ) {
        setMuteActionToUnmute();
    } else {
        setMuteActionToMute();
    }
    
    if ( mEngine.isSpeakerOn() ) {
        setSpeakerActionToHandset();
    } else {
        setSpeakerActionToSpeaker();
    }
    
    if ( mChangeCameraAction ) {
        connect( 
            &mEngine, SIGNAL(cameraChangedToMain()),
            this, SLOT(setCameraActionToSecondary()) );
        connect(
            &mEngine, SIGNAL(cameraChangedToSecondary()),
            this, SLOT(setCameraActionToMain()));           
    }
    
    if ( mMuteAction ) {
        connect( 
            &mEngine, SIGNAL(muted()),
            this, SLOT(setMuteActionToUnmute()) );
        connect(
            &mEngine, SIGNAL(unmuted()),
            this, SLOT(setMuteActionToMute()));
    }
    
    if ( mSpeakerAction ) {
        connect( &mEngine, SIGNAL(speakerEnabled()), 
            this, SLOT(setSpeakerActionToHandset()) );
        connect( &mEngine, SIGNAL(speakerDisabled()), 
            this, SLOT(setSpeakerActionToSpeaker()) );
    }

    if ( mDisableCameraAction ) {
        connect( 
            &mEngine, SIGNAL(cameraDisabled()),
            this, SLOT(setCameraActionToEnable()) );
        connect(
            &mEngine, SIGNAL(cameraEnabled()),
            this, SLOT(setCameraActionToDisable()));           
    }

    connect( &mEngine, SIGNAL(blocked()), this, SLOT(disableControls()) );
    connect( &mEngine, SIGNAL(unblocked()), this, SLOT(enableControls()) ); 
    
    if ( mDuration ) {
        // TODO: clearing is needed for label in order to
        // get text changed. Seems like bug in orbit side.
        // This causes that duration is not changed. Workaround
        // would be to call clear always when duration changes.
        mDuration->clear();
        connect( 
        &mEngine, SIGNAL(sessionDurationChanged(const QString &)),
        mDuration, SLOT(setPlainText(const QString &)) );
    }
    
    if ( mRecipient ){
        mRecipient->clear();
        mRecipient->setPlainText( mEngine.recipient() );
        connect( 
           &mEngine, SIGNAL(recipientChanged(const QString &)),
           mRecipient, SLOT(setPlainText(const QString &)) );
    }
    
    if ( mSharedVideoWidget ) {
        connect( &mEngine, SIGNAL(localPlayerPlaying()), 
                 mEffectHandler, SLOT(showSendWindow()) );
        connect( &mEngine, SIGNAL(localPlayerPaused()), 
                 mEffectHandler, SLOT(hideSendWindow()) );
    }

    if ( mReceivedVideoWidget ) {
        connect( &mEngine, SIGNAL(remotePlayerPlaying()), 
                 mEffectHandler, SLOT(showReceiveWindow()) );
        connect( &mEngine, SIGNAL(remotePlayerPaused()), 
                 mEffectHandler, SLOT(hideReceiveWindow()) );
    }
    
    mEngine.setCurrentView(this);
    
    LC_QDEBUG( "livecomms [UI] <- LcView::activated()" )
}

// -----------------------------------------------------------------------------
// LcView::deactivated
// -----------------------------------------------------------------------------
//
void LcView::deactivated()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::deactivated()" )
    QRectF sharedContentRect;//null area
    QRectF receivedContentRect;//null area

    mEngine.setContentAreas( sharedContentRect, receivedContentRect );
    disconnect( &mEngine, 0, this, 0 );
    
    LC_QDEBUG( "livecomms [UI] <- LcView::deactivated()" )
}

// -----------------------------------------------------------------------------
// LcView::notSupported
// -----------------------------------------------------------------------------
//
void LcView::notSupported()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::notSupported()" )
    
    mNotSupportedNote->show();
    toFullScreen( false );
    
    LC_QDEBUG( "livecomms [UI] <- LcView::notSupported()" )
}

// -----------------------------------------------------------------------------
// LcView::shareImage
// -----------------------------------------------------------------------------
//
void LcView::shareImage()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::shareImage()" )
    const char localImageName[] = "c:\\data\\local.jpg";
    QString fileName = QString::fromAscii(localImageName);
    mEngine.shareImage(fileName);
    LC_QDEBUG( "livecomms [UI] <- LcView::shareImage()" )
}

// -----------------------------------------------------------------------------
// LcView::endVideoSession
// -----------------------------------------------------------------------------
//
void LcView::endVideoSession()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::endVideoSession()" )
    
    mEngine.stop();

    LC_QDEBUG( "livecomms [UI] <- LcView::endVideoSession()" )
    
}

// -----------------------------------------------------------------------------
// LcView::disableCamera
// -----------------------------------------------------------------------------
//
void LcView::disableCamera()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::disableCamera()" )
 
    mEffectHandler->setDissappearEffect( LcEffectHandler::NormalDissappear );
    mEngine.toggleDisableCamera();
    toFullScreen( false );
    
    LC_QDEBUG( "livecomms [UI] <- LcView::disableCamera()" ) 
}

// -----------------------------------------------------------------------------
// LcView::mute
// -----------------------------------------------------------------------------
//
void LcView::mute()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::mute()" )
    
    mEngine.toggleMute();
    toFullScreen( false );
    
    LC_QDEBUG( "livecomms [UI] <- LcView::mute()" )
}

// -----------------------------------------------------------------------------
// LcView::changeCamera
// -----------------------------------------------------------------------------
//
void LcView::changeCamera()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::changeCamera()" )
   
    mEffectHandler->setDissappearEffect( LcEffectHandler::DissappearToFlip );
    mEngine.toggleCamera();
    toFullScreen( false );
        
    LC_QDEBUG( "livecomms [UI] <- LcView::changeCamera()" )
}

// -----------------------------------------------------------------------------
// LcView::switchToVoiceCall
// -----------------------------------------------------------------------------
//
void LcView::switchToVoiceCall()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::switchToVoiceCall()" )
    
    mEngine.stop();
    
    LC_QDEBUG( "livecomms [UI] <- LcView::switchToVoiceCall()" )
}

// -----------------------------------------------------------------------------
// LcView::speaker
// -----------------------------------------------------------------------------
//
void LcView::speaker()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::speaker()" )

    mEngine.toggleSpeaker();
    toFullScreen( false );
    
    LC_QDEBUG( "livecomms [UI] <- LcView::speaker()" )    
}

// -----------------------------------------------------------------------------
// LcView::showZoom
// -----------------------------------------------------------------------------
//
void LcView::showZoom()
{
    LC_QDEBUG("livecomms [UI] -> LcView::showZoom()")

    if ( !mZoomSlider ) {
        return;
    }
        
    LcControlValues values;
    mEngine.zoomValues(values);
    LC_QDEBUG_2("livecomms [UI]    MinValue: ", values.mMinValue)
    LC_QDEBUG_2("livecomms [UI]    MaxValue: ", values.mMaxValue)
    LC_QDEBUG_2("livecomms [UI]    CurrentValue: ", values.mValue)
    
    mZoomSlider->setRange(values.mMinValue, values.mMaxValue);
    mZoomSlider->setValue(values.mValue);
    mZoomSlider->setSingleStep(1);
    mZoomSlider->setVisible(true);
        
    LC_QDEBUG("livecomms [UI] <- LcView::showZoom()")    
}

// -----------------------------------------------------------------------------
// LcView::setCameraActionToMain
// -----------------------------------------------------------------------------
//
void LcView::setCameraActionToMain()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::setCameraActionToMain()" )
    
    if ( mChangeCameraAction ){ 
        mChangeCameraAction->setIcon (HbIcon( lcIconNameMainCamera ));
    }
    LC_QDEBUG( "livecomms [UI] <- LcView::setCameraActionToMain()" )   
}

// -----------------------------------------------------------------------------
// LcView::setCameraActionToSecondary
// -----------------------------------------------------------------------------
//
void LcView::setCameraActionToSecondary()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::setCameraActionToSecondary()" )
    
    if ( mChangeCameraAction ){ 
        mChangeCameraAction->setIcon (HbIcon( lcIconNameSecondaryCamera ));
    }
    LC_QDEBUG( "livecomms [UI] <- LcView::setCameraActionToSecondary()" )   
}

// -----------------------------------------------------------------------------
// LcView::setMuteActionToUnmute
// -----------------------------------------------------------------------------
//
void LcView::setMuteActionToUnmute()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::setMuteActionToUnmute()" )
    
    if ( mMuteAction ){ 
        mMuteAction->setIcon (HbIcon( lcIconNameUnmuteMic ));
    }
    LC_QDEBUG( "livecomms [UI] <- LcView::setMuteActionToUnmute()" )   
}

// -----------------------------------------------------------------------------
// LcView::setMuteActionToMute
// -----------------------------------------------------------------------------
//
void LcView::setMuteActionToMute()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::setMuteActionToMute()" )
    if ( mMuteAction ){ 
        mMuteAction->setIcon (HbIcon( lcIconNameMuteMic ));
    }
    LC_QDEBUG( "livecomms [UI] <- LcView::setMuteActionToMute()" )   
}

// -----------------------------------------------------------------------------
// LcView::setSpeakerActionToHandset
// -----------------------------------------------------------------------------
//
void LcView::setSpeakerActionToHandset()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::setSpeakerActionToHandset()" )
    if ( mSpeakerAction ){
        mSpeakerAction->setIcon (HbIcon( lcIconNameHandset ));
    }
    LC_QDEBUG( "livecomms [UI] <- LcView::setSpeakerActionToHandset()" )
}

// -----------------------------------------------------------------------------
// LcView::setSpeakerActionToSpeaker
// -----------------------------------------------------------------------------
//
void LcView::setSpeakerActionToSpeaker()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::setSpeakerActionToSpeaker()" )
    if ( mSpeakerAction ){
        mSpeakerAction->setIcon( HbIcon( lcIconNameLoudspeaker ));
    }
    LC_QDEBUG( "livecomms [UI] <- LcView::setSpeakerActionToSpeaker()" )
}

// -----------------------------------------------------------------------------
// LcView::setCameraActionToEnable
// -----------------------------------------------------------------------------
//
void LcView::setCameraActionToEnable()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::setCameraActionToEnable()" )
    
    if ( mDisableCameraAction ){ 
        mDisableCameraAction->setIcon( HbIcon( lcIconNameEnableCamera ));
    }
    if ( mDisableCameraMenuAction ){ 
        // TODO: get text from repository as loc id is not the same for mus
        mDisableCameraMenuAction->setText( hbTrId( "txt_vt_menu_enable_camera" ));
    }

    LC_QDEBUG( "livecomms [UI] <- LcView::setCameraActionToEnable()" )   
}

// -----------------------------------------------------------------------------
// LcView::setCameraActionToDisable
// -----------------------------------------------------------------------------
//
void LcView::setCameraActionToDisable()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::setCameraActionToDisable()" )
    
    if ( mDisableCameraAction ){ 
        mDisableCameraAction->setIcon (HbIcon( lcIconNameDisableCamera ));
    }
    if ( mDisableCameraMenuAction ){ 
        // TODO: get text from repository as loc id is not the same for mus
        mDisableCameraMenuAction->setText( hbTrId( "txt_vt_menu_disable_camera" ));
    }
    
    LC_QDEBUG( "livecomms [UI] <- LcView::setCameraActionToDisable()" )   
}

// -----------------------------------------------------------------------------
// LcView::disableControls
// -----------------------------------------------------------------------------
//
void LcView::disableControls()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::disableControls()" )
    menu()->setEnabled( false );
    toolBar()->setEnabled( false );
    LC_QDEBUG( "livecomms [UI] <- LcView::disableControls()" )
}

// -----------------------------------------------------------------------------
// LcView::enableControls
// -----------------------------------------------------------------------------
//
void LcView::enableControls()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::enableControls()" )
    menu()->setEnabled( true );
    toolBar()->setEnabled( true );
    LC_QDEBUG( "livecomms [UI] <- LcView::enableControls()" )
}

// -----------------------------------------------------------------------------
// LcView::gestureEvent
// -----------------------------------------------------------------------------
//
void LcView::gestureEvent(QGestureEvent *event)
{
    LC_QDEBUG( "livecomms [UI] -> LcView::gestureEvent()" )
    if(HbTapGesture *tap = static_cast<HbTapGesture *>(event->gesture(Qt::TapGesture))) {        
        if ((tap->state() == Qt::GestureUpdated) &&
            (tap->tapStyleHint() == HbTapGesture::TapAndHold)) {            
            gestureLongPress(translatePointForOrientation(tap->position()));
        } 
        
        else if ( (tap->state() == Qt::GestureFinished ) &&
                ( tap->tapStyleHint() == HbTapGesture::Tap)) {
        
            gestureShortPress();
        }        
    }
    LC_QDEBUG( "livecomms [UI] <- LcView::gestureEvent()" )
}


// -----------------------------------------------------------------------------
// LcView::gestureLongPress
// -----------------------------------------------------------------------------
//
void LcView::gestureLongPress(QPointF coords)
{
    LC_QDEBUG_2( "livecomms [UI] -> LcView::gestureLongPress(), coords:", coords )

    bool sharedHit = false;
    
    if ( mSharedVideoWidget && mSharedVideoWidget->windowFrameGeometry().contains(coords) ) {
        LC_QDEBUG( "livecomms [UI] LcView mapped to mSharedVideoWidget" )
        sharedHit = true;
    }
    
    if ( sharedHit ) {
        LC_QDEBUG( "livecomms [UI] LcView mapped to mSharedVideoWidget" )

        createContextMenu();
        mRepository.sharedVideoContextMenuActions( mItemContextMenu, *this );

        if ( mItemContextMenu->actions().count() > 0 ) {
            mItemContextMenu->open(this);
        }
    }

    LC_QDEBUG( "livecomms [UI] <- LcView::gestureLongPress()" )
}



// -----------------------------------------------------------------------------
// LcView::gestureShortPress
// -----------------------------------------------------------------------------
//
void LcView::gestureShortPress()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::gestureShortPress()" )
    toFullScreen( !mEngine.fullScreenMode() );
    LC_QDEBUG( "livecomms [UI] <- LcView::gestureShortPress()" )
}

// -----------------------------------------------------------------------------
// LcView::createNotSupportedNote
// -----------------------------------------------------------------------------
//
void LcView::createNotSupportedNote()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::createNotSupportedNote()" )

    if ( !mNotSupportedNote ){
        mNotSupportedNote = new HbMessageBox(QString("Not supported"));
        mNotSupportedNote->setTimeout(HbDialog::StandardTimeout);
        mNotSupportedNote->setZValue(LC_NOTE_ON_TOP);
        mNotSupportedNote->hide();
    }

    LC_QDEBUG( "livecomms [UI] <- LcView::createNotSupportedNote()" )
}



// -----------------------------------------------------------------------------
// LcView::createContextMenu
// -----------------------------------------------------------------------------
//
void LcView::createContextMenu()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::createContextMenu()" )

    // mItemContextMenu is destroyed upon close 
    mItemContextMenu = new HbMenu();
    mItemContextMenu->setDismissPolicy(HbDialog::TapOutside);
    mItemContextMenu->setTimeout(HbDialog::ContextMenuTimeout);
    mItemContextMenu->setAttribute(Qt::WA_DeleteOnClose);
    
    LC_QDEBUG( "livecomms [UI] <- LcView::createContextMenu()" )
}
    
// -----------------------------------------------------------------------------
// LcView::currentLayout
// -----------------------------------------------------------------------------
//
QString LcView::currentLayout()
{
    QString layout( lcLayoutPortraitDefaultId );

    if ( !isLandscapeOrientation() ) {
        if ( mEffectHandler->isSwapInProgress() &&
             mSharedVideoWidget &&
             mReceivedVideoWidget &&
             mSharedVideoWidget->geometry().top() > mReceivedVideoWidget->geometry().top() ) {

            layout = lcLayoutPortraitSwappedId;
        } else {
            layout = lcLayoutPortraitDefaultId;
        }
    } else {
        if ( mEffectHandler->isSwapInProgress() &&
             mSharedVideoWidget &&
             mReceivedVideoWidget &&
             mSharedVideoWidget->geometry().left() > mReceivedVideoWidget->geometry().left() ) {

            layout = lcLayoutLandscapeSwappedId;
        } else {
            layout = lcLayoutLandscapeDefaultId;
        }
    }
    LC_QDEBUG_2( "livecomms [UI] -> LcView::currentLayout()", layout )
    return layout;
}


// -----------------------------------------------------------------------------
// LcView::menuAboutToShow
// -----------------------------------------------------------------------------
//
void LcView::menuAboutToShow()
{
    LC_QDEBUG( "livecomms [UI] -> LcView::menuAboutToShow()" )
    toFullScreen( false );
    LC_QDEBUG( "livecomms [UI] <- LcView::menuAboutToShow()" )
}

// -----------------------------------------------------------------------------
// LcView::isLandscapeOrientation
// -----------------------------------------------------------------------------
//
bool LcView::isLandscapeOrientation()
{
    return ( !HbInstance::instance()->allMainWindows().isEmpty() && 
             HbInstance::instance()->allMainWindows().at(0)->orientation() == Qt::Horizontal );
}

// -----------------------------------------------------------------------------
// LcView::translateRectForOrientation
// Video windows at engine side do not change their coordinate system
// when orientation at orbit UI changes. Therefore we need to convert video
// rects from landscape coordinate system to portait coordinate system.
// NOTE: Landscape orientation has different rotation in emulator than in HW.
// -----------------------------------------------------------------------------
//
QRectF LcView::translateRectForOrientation(const QRectF& origRect)
{
    if ( !isLandscapeOrientation() ){
        return origRect;
    }
    QRectF newRect = origRect;
    QMatrix m;
#if ( defined __WINSCW__ ) || ( defined __WINS__ ) 
    m.rotate(270);
    newRect = m.mapRect(newRect);
    newRect.translate(0,HbInstance::instance()->allMainWindows().at(0)->rect().height());
#else
    m.rotate(90);
    newRect = m.mapRect(newRect);
    newRect.translate(HbInstance::instance()->allMainWindows().at(0)->rect().width(),0);
#endif
    return newRect; 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
QPointF LcView::translatePointForOrientation(const QPointF& origPoint)
{
    if ( !isLandscapeOrientation() ) {
        return origPoint;
    }
    QPointF newPoint = origPoint;
    QMatrix m;
#if ( defined __WINSCW__ ) || ( defined __WINS__ ) 
    m.translate(HbInstance::instance()->allMainWindows().at(0)->rect().height(),0);
    m.rotate(90);
    newPoint = m.map(newPoint);
#else
    m.translate(0,HbInstance::instance()->allMainWindows().at(0)->rect().width());
    m.rotate(270);
    newPoint = m.map(newPoint);
#endif
    return newPoint; 
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcView::setVisibility( QGraphicsItem* item, bool visible )
{
    if ( item ) {
        item->setVisible(visible);
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcView::addOptionsMenuActions()
{
    HbAction* swapAction = static_cast<HbAction*>(
        mRepository.findObject(lcActSwapViewsId));
    menu()->addAction(swapAction);

    HbAction* openKeypadAction = static_cast<HbAction*>(
        mRepository.findObject(lcActOpenKeypadId));
    menu()->addAction(openKeypadAction);
}

// -----------------------------------------------------------------------------
// LcView::openDialpad()
// -----------------------------------------------------------------------------
//
void LcView::openDialpad()
{    
    LC_QDEBUG("livecomms [UI] -> LcView::openDialpad()")
    mRepository.loadLayout( lcLayoutLandscapeDialpadId ); 
    if ( mEffectHandler )mEffectHandler->startEffects();    
    mDialpad->openDialpad();
    mDialpad->setCallButtonEnabled(false);
    menu()->clearActions();    
    LC_QDEBUG("livecomms [UI] <- LcView::openDialpad()")
}

// -----------------------------------------------------------------------------
// LcView::dialpadOpened()
// -----------------------------------------------------------------------------
//
void LcView::dialpadOpened()
{
    LC_QDEBUG("livecomms [UI] <-> LcView::dialpadOpened()")   
    toolBar()->setVisible( false );
    setTitleBarVisible( true );
}

// -----------------------------------------------------------------------------
// LcView::dialpadClosed()
// -----------------------------------------------------------------------------
//
void LcView::dialpadClosed()
{
    LC_QDEBUG("livecomms [UI] -> LcView::dialpadClosed()")    
    addOptionsMenuActions();    
    // switch back to the previous layout
    QString pLayout = mRepository.previousLayout();
    QString layout;
    bool isSwapped = ( pLayout == lcLayoutPortraitDefaultId || 
                       pLayout == lcLayoutLandscapeDefaultId )
                     ? false : true;
    // take care orientation changes
    bool isLandscape = isLandscapeOrientation();
    if ( isLandscape ){
        layout = ( isSwapped ) ? lcLayoutLandscapeSwappedId 
                               : lcLayoutLandscapeDefaultId;
        
    }
    else {
        layout = ( isSwapped ) ? lcLayoutPortraitSwappedId 
                               : lcLayoutPortraitDefaultId;
    }    
    mRepository.loadLayout( layout );    
    if ( mEffectHandler ){
        mEffectHandler->startEffects();
    }
    toFullScreen(false);
    LC_QDEBUG("livecomms [UI] <- LcView::dialpadClosed()")
}

// -----------------------------------------------------------------------------
// LcView::dialpadEditorTextChanged()
// -----------------------------------------------------------------------------
//
void LcView::dialpadEditorTextChanged()
{   
    LC_QDEBUG_2("livecomms [UI] -> Dial Pad Field ", mDialpad->editor().text());
    LC_QDEBUG_2("livecomms [UI] -> Last Dialled Charcter ", mDialpad->editor().text().right(1));
    bool dialPadStatus = mEngine.SendDialTone(mDialpad->editor().text().right(1).at(0));
    LC_QDEBUG_2("livecomms [UI] -> Dialpad Send Tone Status ",dialPadStatus)
}

// -----------------------------------------------------------------------------
// LcView::back
// -----------------------------------------------------------------------------
//
void LcView::back()
{
    LC_QDEBUG("livecomms [UI] -> LcView::back()")
    if ( mDialpad && mDialpad->isOpen()) {
        LC_QDEBUG("livecomms [UI] - Closing dialpad")
        mDialpad->closeDialpad();    
    } else {
        mEngine.minimize();
    }
    
    LC_QDEBUG("livecomms [UI] <- LcView::back()")
}

// -----------------------------------------------------------------------------
// LcView::timerEvent
// -----------------------------------------------------------------------------
//
void LcView::timerEvent( QTimerEvent * event )
{    
    if ( event->timerId() == timerId ){
        LC_QDEBUG("livecomms [UI] -> LcView::timerEvent() inActivity Timeout")
        killTimer( timerId );
        toFullScreen(true);
    }    
}

// -----------------------------------------------------------------------------
// LcView::watchInactivity
// -----------------------------------------------------------------------------
//
void LcView::watchInactivity()
{    
    if( !mEngine.fullScreenMode()){
        LC_QDEBUG("livecomms [UI] - LcView::watchInactivity() start watching inactivity") 
        killTimer( timerId );
        timerId = startTimer( inActivityTimeout );
    }    
}

// -----------------------------------------------------------------------------
// LcView::toFullScreen utility function
// -----------------------------------------------------------------------------
//
void LcView::toFullScreen( bool fullscreen )
{
    LC_QDEBUG_2("livecomms [UI] - LcView::toFullScreen(),",fullscreen)
    if( menu()->isVisible() || mDialpad && mDialpad->isOpen() ) return;
    mEngine.setFullScreenMode( fullscreen );    
    setTitleBarVisible( !fullscreen );
    toolBar()->setVisible( !fullscreen );
    setVisibility( mEndCallButton, !fullscreen );
    setVisibility( mBrandIcon, !fullscreen );
    setVisibility( mDuration, !fullscreen );
    setVisibility( mRecipient, !fullscreen );
    emit contentFullScreenChanged();
}
// End of file
