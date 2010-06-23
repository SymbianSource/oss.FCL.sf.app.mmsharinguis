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
#include "lcuicomponentrepository.h"
#include "lcview.h"
#include "lcuidefs.h"
#include "lclogger.h"
#include "lcuiengine.h"
#include "lcvideowidget.h"

//SYSTEM
#include <hbaction.h>
#include <hbmenu.h>
#include <hbtoolbar.h>
#include <hbmessagebox.h>
#include <hbdialog.h>
#include <hbprogressdialog.h>
#include <hblabel.h>
#include <hbinstance.h>
#include <hbzoomsliderpopup.h>
#include <hbdialog.h>
#include <hbpushbutton.h>
#include <hbframedrawer.h>
#include <hbstyleloader.h>
#include <dialpad.h>

// -----------------------------------------------------------------------------
// LcUiComponentRepository::LcUiComponentRepository
// -----------------------------------------------------------------------------
//
LcUiComponentRepository::LcUiComponentRepository(LcUiEngine& engine)
    : HbDocumentLoader(),
      mEngine(engine),
      mSlots(0),
      mIdleView(0),
      mReceiveView(0),
      mSendView(0),
      mTwowayView(0),
      mAllInOneView(0),
      mReceiveOnlyView(0),
      mAcceptQuery(0),
      mInvitingNote(0),
      mWaitingNote(0),
      mRecipientQuery(0),
      mZoomSlider(0),
      mShareOwnVideoQuery(0)
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::LcUiComponentRepository()" )
    
    mSlots = new QHash<QString,const char*>;
    mSlots->insert( lcActFullscreenId, SLOT( notSupported() ) );
    mSlots->insert( lcActVideopreferenceId, SLOT( notSupported() ) );
    mSlots->insert( lcActDialerId, SLOT( notSupported() ) );
    mSlots->insert( lcActEndactivecallId, SLOT( notSupported() ) );
    mSlots->insert( lcActStopSharingId, SLOT( endVideoSession() ) );
    mSlots->insert( lcActEnableCameraId, SLOT( disableCamera() ) );
    mSlots->insert( lcActMuteId, SLOT( mute() ) );
    mSlots->insert( lcActGalleryId, SLOT( notSupported() ) );
    mSlots->insert( lcActChangeCameraId, SLOT( notSupported() ) );    
    mSlots->insert( lcActAdjustVideoId, SLOT( notSupported() ) );
    mSlots->insert( lcActShareImageId, SLOT( notSupported() ) );
    mSlots->insert( lcActSwapViewsId, SLOT( swap() ) );
    mSlots->insert( lcActZoomId, SLOT( notSupported() ) );
    mSlots->insert( lcActSwitchToVoiceCallId, SLOT( switchToVoiceCall() ) );    
    mSlots->insert( lcActOpenKeypadId, SLOT( openDialpad() ) );
    mSlots->insert( lcActDisableCameraId, SLOT( disableCamera() ) );
    mSlots->insert( lcActSpeakerId, SLOT( speaker() ) );

    mSlots->insert( lcActMenuDisableCameraId, SLOT( disableCamera() ) );    
    mSlots->insert( lcActMenuChangeCameraId, SLOT( notSupported() ) );  
    
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::LcUiComponentRepository()" )
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::~LcUiComponentRepository
// -----------------------------------------------------------------------------
//
LcUiComponentRepository::~LcUiComponentRepository()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::~LcUiComponentRepository()" )
    
    delete mSlots;
    
    mIdleViewComponents.clear();
    mReceiveViewComponents.clear();
    mSendViewComponents.clear();
    mTwowayViewComponents.clear();
    mAllInOneViewComponents.clear();
    mReceiveOnlyViewComponents.clear();
    
    delete mAcceptQuery;
    delete mInvitingNote;
    delete mWaitingNote;
    delete mRecipientQuery;
    delete mZoomSlider;
    delete mShareOwnVideoQuery;
 
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::~LcUiComponentRepository()" )
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::createObject
// -----------------------------------------------------------------------------
//
QObject* LcUiComponentRepository::createObject(
        const QString& type, const QString &name)
{
    QObject* object = 0;
    
    if ( type == lcLcViewTypeName || 
         name == lcViewIdleId ||
         name == lcViewReceiveId || 
         name == lcViewReceiveOnlyId || 
         name == lcViewAllInOneId || 
         name == lcViewSendId ||
         name == lcViewTwowayId ) {
        LC_QDEBUG_2( "-> LcUiComponentRepository::createObject() ",
        "- creating LcView -" )
        object = new LcView( mEngine, *this );           
    } 
    else if ( name == lcWidgetSendVideoId ) {
        LC_QDEBUG_2( "-> LcUiComponentRepository::createObject() ",
        "- creating shared video widget -" )
        object = new LcVideoWidget();
        static_cast<LcVideoWidget*>(object)->setLocalPlayer(true);
    }
    else if ( name == lcWidgetSendVideoId2 ) {
        LC_QDEBUG_2( "-> LcUiComponentRepository::createObject() ",
        "- creating shared video widget effect overlay -" )
        object = new LcVideoWidget(0, false);
        static_cast<LcVideoWidget*>(object)->setLocalPlayer(true);
    }
    else if ( name == lcWidgetRecvVideoId ) {
        LC_QDEBUG_2( "-> LcUiComponentRepository::createObject() ",
        "- creating received video widget -" )
        object = new LcVideoWidget();
        static_cast<LcVideoWidget*>(object)->setLocalPlayer(false);
                
    }  
    else if ( name == lcWidgetRecvVideoId2 ) {
        LC_QDEBUG_2( "-> LcUiComponentRepository::createObject() ",
        "- creating received video widget effect overlay -" )
        object = new LcVideoWidget(0, false);
        static_cast<LcVideoWidget*>(object)->setLocalPlayer(false);
    }
    else if ( name == lcButtonEndCall ){
        object = HbDocumentLoader::createObject( type, name );
        HbFrameDrawer*frameDrawer = new HbFrameDrawer("qtg_fr_btn_red_normal",HbFrameDrawer::NinePieces);
        static_cast<HbPushButton*>(object)->setFrameBackground(frameDrawer);
        HbStyleLoader::registerFilePath(":/hbpushbutton_color.css");
    }
    else if ( name == lcWidgetDialpad ){
      HbMainWindow* pWindow = HbInstance::instance()->allMainWindows().at(0);
        if( pWindow ){
            object = new Dialpad( *pWindow );
        }
    }
    else {
        object = HbDocumentLoader::createObject( type, name );
    }
    if (object) {
        object->setObjectName(name);
    }
    
    LC_QDEBUG_2( "livecomms [UI] <- LcUiComponentRepository::createObject(), name=", name )
    
    return object;
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::idleView
// -----------------------------------------------------------------------------
//
LcView* LcUiComponentRepository::idleView()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::idleView()" );

    reset();
    setObjectTree( mIdleViewComponents );
    
    if ( !mIdleView ) {
        mIdleView = loadView( 
            mIdleViewComponents, lcViewIdleId, lcIdleViewFile );
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::idleView()" );
    return mIdleView;
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::receiveView
// -----------------------------------------------------------------------------
//
LcView* LcUiComponentRepository::receiveView()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::receiveView()" );

    reset();
    setObjectTree( mReceiveViewComponents );
    if ( !mReceiveView ) {
        mReceiveView = loadView( 
            mReceiveViewComponents, lcViewReceiveId, lcReceiveViewFile );
    } 
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::receiveView()" );
    return mReceiveView;
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::receiveOnlyView
// -----------------------------------------------------------------------------
//
LcView* LcUiComponentRepository::receiveOnlyView()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::receiveOnlyView()" );

    reset();
    setObjectTree( mReceiveOnlyViewComponents );
    if ( !mReceiveOnlyView ) {
        mReceiveOnlyView = loadView( 
            mReceiveOnlyViewComponents, lcViewReceiveOnlyId, lcReceiveOnlyViewFile );
    } 
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::receiveOnlyView()" );
    return mReceiveOnlyView;
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::twowayView
// -----------------------------------------------------------------------------
//
LcView* LcUiComponentRepository::twowayView()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::twowayView()" );

    reset();
    setObjectTree( mTwowayViewComponents );
    if ( !mTwowayView ) {
        mTwowayView = loadView( 
            mTwowayViewComponents, lcViewTwowayId, lcTwowayViewFile );
    } 
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::twowayView()" );
    return mTwowayView; 
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::sendView
// -----------------------------------------------------------------------------
//
LcView* LcUiComponentRepository::sendView()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::sendView()" );

    reset();
    setObjectTree( mSendViewComponents );
    if ( !mSendView ) {
        mSendView = 
            loadView( mSendViewComponents, lcViewSendId, lcSendViewFile );
    } 
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::sendView()" );
    return mSendView; 
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::allInOneView
// -----------------------------------------------------------------------------
//
LcView* LcUiComponentRepository::allInOneView()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::allInOneView()" );

    reset();
    setObjectTree( mAllInOneViewComponents );
    if ( !mAllInOneView ) {
        mAllInOneView = loadView( 
            mAllInOneViewComponents, lcViewAllInOneId, lcAllInOneViewFile );
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::allInOneView()" );
    return mAllInOneView;   
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::acceptQuery
// -----------------------------------------------------------------------------
//
HbDialog* LcUiComponentRepository::acceptQuery()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::acceptQuery()" );

    if ( !mAcceptQuery ) {
        mAcceptQuery = new HbDialog();
        HbAction* acceptAction = new HbAction( LC_OK );
        HbAction* rejectAction = new HbAction( LC_CANCEL );
        mAcceptQuery->addAction(acceptAction);
        mAcceptQuery->addAction(rejectAction);
        mAcceptQuery->setVisible(false);
        HbLabel* label = new HbLabel( LC_ACCEPT_SESSION );
        mAcceptQuery->setHeadingWidget(label);
        HbLabel* label2 = new HbLabel();
        mAcceptQuery->setContentWidget(label2);
        mAcceptQuery->setTimeout(0);
        mAcceptQuery->setDismissPolicy(HbDialog::NoDismiss);
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::acceptQuery()" );
    return mAcceptQuery;
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::invitingNote
// -----------------------------------------------------------------------------
//
HbProgressDialog* LcUiComponentRepository::invitingNote()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::invitingNote()" );
    
    if ( !mInvitingNote ) {
        mInvitingNote = new HbProgressDialog();
        mInvitingNote->setVisible(false);
        mInvitingNote->setText( LC_INVITING );
        HbAction* rejectAction = new HbAction( LC_CANCEL );
        mInvitingNote->addAction(rejectAction);
    } 
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::invitingNote()" );
    return mInvitingNote;
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::waitingNote
// -----------------------------------------------------------------------------
//
HbProgressDialog* LcUiComponentRepository::waitingNote()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::waitingNote()" );
    
    if ( !mWaitingNote ) {
        mWaitingNote = new HbProgressDialog();
        mWaitingNote->setVisible(false);
        HbAction* rejectAction = new HbAction( LC_CANCEL );
        mWaitingNote->addAction(rejectAction);   
        mWaitingNote->setText( LC_WAITING_VIDEO );
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::waitingNote()" );
    return mWaitingNote;
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::recipientQuery
// -----------------------------------------------------------------------------
//
HbDialog* LcUiComponentRepository::recipientQuery()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::recipientQuery()" );

    if ( !mRecipientQuery ) {
        mRecipientQuery = new HbDialog();
        HbLabel* label = new HbLabel( LC_RECIPIENT_QUERY );
        mRecipientQuery->setHeadingWidget(label);
        mRecipientQuery->setTimeout(HbDialog::NoTimeout);
        mRecipientQuery->setDismissPolicy(HbDialog::TapOutside);
        HbAction* acceptAction = new HbAction( LC_OK );
        HbAction* rejectAction = new HbAction( LC_CANCEL );
        mRecipientQuery->addAction(acceptAction);
        mRecipientQuery->addAction(rejectAction);
        mRecipientQuery->setVisible(false);
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::recipientQuery()" );
    return mRecipientQuery;
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::shareOwnVideoQuery
// -----------------------------------------------------------------------------
//
HbDialog* LcUiComponentRepository::shareOwnVideoQuery()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::shareOwnVideoQuery()" );

    if ( !mShareOwnVideoQuery ) {
        mShareOwnVideoQuery = new HbMessageBox( HbMessageBox::MessageTypeQuestion );
        mShareOwnVideoQuery->setText( hbTrId( "txt_vt_info_allow_own_image_to_be_sent" ));
        mShareOwnVideoQuery->setZValue(LC_NOTE_ON_TOP);
        mShareOwnVideoQuery->setTimeout( HbDialog::NoTimeout );
        mShareOwnVideoQuery->setDismissPolicy( HbDialog::NoDismiss );        
        mShareOwnVideoQuery->setVisible(false);
    }
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::shareOwnVideoQuery()" );
    return mShareOwnVideoQuery;
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::zoomSlider
// -----------------------------------------------------------------------------
//
HbZoomSliderPopup* LcUiComponentRepository::zoomSlider()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::zoomSlider()" );
    
    if ( !mZoomSlider ) {
        mZoomSlider = new HbZoomSliderPopup();
        mZoomSlider->setDismissPolicy(HbDialog::TapOutside);
        mZoomSlider->setTimeout(HbDialog::StandardTimeout);
        mZoomSlider->hide();
        QObject::connect( mZoomSlider, SIGNAL(valueChanged(int)), 
                          &mEngine, SLOT(zoom(int)) );
        
    }
    
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::zoomSlider()" );
    return mZoomSlider;
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::sharedVideoContextMenuActions
// -----------------------------------------------------------------------------
//
void LcUiComponentRepository::sharedVideoContextMenuActions( HbMenu* menu, LcView& view )
{
    Q_UNUSED(view)
    LC_QDEBUG( 
   "livecomms [UI] -> LcUiComponentRepository::sharedVideoContextMenuActions()" );
    
    menu->actions().clear();
    //due stubs in unit tests, qobject_cast cannot be used. 
    //Using static_cast instead
    HbAction* swap = static_cast<HbAction*>( findObject( lcActSwapViewsId ) );
    HbAction* zoom = static_cast<HbAction*>( findObject( lcActZoomId ) );
    HbAction* changeCamera = 
        static_cast<HbAction*>( findObject( lcActMenuChangeCameraId ) );
    HbAction* disableCamera = 
        static_cast<HbAction*>( findObject( lcActMenuDisableCameraId ) );
              
    if ( swap ) {
        LC_QDEBUG_2( "Adding ", swap->text() );
        menu->addAction( swap );
    }
    if ( zoom ) {
        LC_QDEBUG_2( "Adding ", zoom->text() );
        menu->addAction( zoom );
    }
    if ( changeCamera ) {
        LC_QDEBUG_2( "Adding ", changeCamera->text() );
        menu->addAction( changeCamera );
    }
    if ( disableCamera ) {
        LC_QDEBUG_2( "Adding ", disableCamera->text() );
        menu->addAction( disableCamera );
    }

  
    LC_QDEBUG( 
   "livecomms [UI] <- LcUiComponentRepository::sharedVideoContextMenuActions()" );
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::loadView
// -----------------------------------------------------------------------------
//
LcView* LcUiComponentRepository::loadView( QObjectList& components,
                                           const QString& viewId, 
                                           const QString& xmlFile )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::loadView()" );
    
    LcView* view = 0;
    bool ok( false );
    // Load the XML file
    components = load( xmlFile, &ok );

    if ( !ok ) {
        LC_QCRITICAL_3( "! loading of XML file ", xmlFile , " failed !" )
        return 0;    		
    }
    mLastLoadedView = xmlFile;

    // Load default layout for the current device orientation    
    if ( !HbInstance::instance()->allMainWindows().isEmpty() && 
         HbInstance::instance()->allMainWindows().at(0)->orientation() == Qt::Horizontal ) {  
        ok = loadLayout( lcLayoutLandscapeDefaultId );
        
    } else {
        ok = loadLayout( lcLayoutPortraitDefaultId );
    }        
    if ( !ok ) {
         LC_QCRITICAL( "! loading of layout failed !" )
         return 0;           
     }      

    view = qobject_cast<LcView*>( findWidget( viewId ) );
    if ( view ) {
    	connectActions( *view );
    	view->init(); 
    } 
    
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::loadView()" );
    return view;
}
    
// -----------------------------------------------------------------------------
// LcUiComponentRepository::connectActions
// -----------------------------------------------------------------------------
//
void LcUiComponentRepository::connectActions( LcView& view ) const
{
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::connectActions()" );
    foreach( QAction* hbAction, view.menu()->actions() ) {
        connect( *hbAction, view );
    }
    foreach( QAction* hbAction, view.toolBar()->actions() ) {
        connect( *hbAction, view );
    }

    HbAction* changeCamera = 
        static_cast<HbAction*>( findObject( lcActMenuChangeCameraId ) );
    HbAction* disableCamera = 
        static_cast<HbAction*>( findObject( lcActMenuDisableCameraId ) );

    QObject::connect( changeCamera, SIGNAL(triggered()), 
      &view, SLOT(notSupported()) );

    QObject::connect( disableCamera, SIGNAL(triggered()), 
      &view, SLOT(disableCamera()) );
    
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::connectActions()" )  
}

// -----------------------------------------------------------------------------
// LcUiComponentRepository::connect
// -----------------------------------------------------------------------------
//
bool LcUiComponentRepository::connect( QAction& hbAction, LcView& view ) const
{
    LC_QDEBUG_2( "livecomms [UI] -> LcUiComponentRepository::connect() action=",
                 hbAction.objectName() )
    
    const char* method = mSlots->value( hbAction.objectName() ); 
    bool slotFound = ( QString( method ).length() > 0 );
    
    if ( slotFound ) {
        QObject::connect( &hbAction, SIGNAL(triggered()), &view, method );  
    }
    
    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::connect()" )
    
    return slotFound;
}


// -----------------------------------------------------------------------------
// LcUiComponentRepository::loadLayout
// -----------------------------------------------------------------------------
//
bool LcUiComponentRepository::loadLayout( const QString& layoutName )
    {
    LC_QDEBUG( "livecomms [UI] -> LcUiComponentRepository::loadLayout()" )    
    bool ok( false );
  
    if ( mLastLoadedView.length() > 0) {
        LC_QDEBUG_4( "layout = ", layoutName, ", view =",  mLastLoadedView )

        QObjectList objects = load( mLastLoadedView, layoutName, &ok );
        if (!ok) {
            LC_QCRITICAL( "! loading of XML failed !" )
        }
        else{
            mPreviousLayout = mLayout; 
            mLayout = layoutName;
            setObjectTree( objects );
        }
    } else {
        LC_QCRITICAL( "! not loading layout, since view is not loaded!" )
    }

    LC_QDEBUG( "livecomms [UI] <- LcUiComponentRepository::loadLayout()" )
    return ok;
    }

// -----------------------------------------------------------------------------
// LcUiComponentRepository::layout()const
// -----------------------------------------------------------------------------
//
QString LcUiComponentRepository::layout()const
    {
    LC_QDEBUG_2( "livecomms [UI] -> LcUiComponentRepository::layout()",mLayout )    
    return mLayout;
    }

// -----------------------------------------------------------------------------
// LcUiComponentRepository::previousLayout()const
// -----------------------------------------------------------------------------
//
QString LcUiComponentRepository::previousLayout()const
    {
    LC_QDEBUG_2( "livecomms [UI] -> LcUiComponentRepository::previousLayout()",
                                                              mPreviousLayout )    
    return mPreviousLayout;
    }
// End of file
