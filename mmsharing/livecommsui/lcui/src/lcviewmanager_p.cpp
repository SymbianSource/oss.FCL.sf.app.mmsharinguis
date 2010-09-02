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

// USER INCLUDES
#include "lcviewmanager_p.h"
#include "lcuicomponentrepository.h"
#include "lcuidefs.h"
#include "lcuiengine.h"
#include "lcview.h"
#include "lclogger.h"

// SYSTEM INCLUDES
#include <hbmainwindow.h>
#include <hbdialog.h>
#include <hbaction.h>
#include <qcoreapplication.h>
#include <hbactivitymanager.h>
#include <hbapplication.h>
#include <hbapplication.h>

// CONSTANTS
#define LC_VIEWMANAGER_SLOT_ACTIVATE_CURRENT_VIEW "activateCurrentView" 



// -----------------------------------------------------------------------------
// LcMainWindow::LcMainWindow
// -----------------------------------------------------------------------------
//
LcMainWindow::LcMainWindow() : 
    HbMainWindow(0, Hb::WindowFlagFixedHorizontal | Hb::WindowFlagTransparent)
{
    // TODO: remove flag WindowFlagFixedHorizontal if portait layout
    // is going to be supported.
    qApp->installEventFilter(this);
}

// -----------------------------------------------------------------------------
// LcMainWindow::~LcMainWindow
// -----------------------------------------------------------------------------
//
LcMainWindow::~LcMainWindow()
{
}

// -----------------------------------------------------------------------------
// LcMainWindow::eventFilter
// -----------------------------------------------------------------------------
//
bool LcMainWindow::eventFilter ( QObject * watched, QEvent * event )
{    
    if ( event->type() == QEvent::ApplicationActivate ){
        LC_QDEBUG( "livecomms [UI] -> LcMainWindow::eventFilter(), ApplicationActivate" )
        emit appFocusGained();
    } else if (( event->type() == QEvent::ApplicationDeactivate ) && 
            ( !this->windowSurface()) ){
        LC_QDEBUG( "livecomms [UI] -> LcMainWindow::eventFilter(), ApplicationDeactivate" )
        emit appFocusLost();
    }
    return QObject::eventFilter(watched, event);
}


// -----------------------------------------------------------------------------
// LcViewManagerPrivate::LcViewManagerPrivate
// -----------------------------------------------------------------------------
//
LcViewManagerPrivate::LcViewManagerPrivate(
    const QString& engineName,
    const QString& applicationDisplayName) :
    mEngine( 0 ),
    mRepository( 0 )
{
    LC_QDEBUG( "livecomms [UI] -> LcViewManagerPrivate::LcViewManagerPrivate()" )
    
    mEngine = new LcUiEngine( engineName, applicationDisplayName );
    mRepository = new LcUiComponentRepository( *mEngine );
    setCurrentView( true );
        
    connect(&mMainWindow, SIGNAL(appFocusGained()), mEngine, SLOT(maximize()));
    connect(&mMainWindow, SIGNAL(appFocusLost()), mEngine, SLOT(minimize()));
    
    LC_QDEBUG( "livecomms [UI] <- LcViewManagerPrivate::LcViewManagerPrivate()" )
}

// -----------------------------------------------------------------------------
// LcViewManagerPrivate::~LcViewManagerPrivate
// -----------------------------------------------------------------------------
//
LcViewManagerPrivate::~LcViewManagerPrivate()
{
    LC_QDEBUG( "livecomms [UI] -> LcViewManagerPrivate::~LcViewManagerPrivate()" )

    delete mEngine;
    QList<HbView *> views = mMainWindow.views();
    foreach( HbView* view, views ){
        mMainWindow.removeView(view);
    }
    delete mRepository;    

    LC_QDEBUG( "livecomms [UI] <- LcViewManagerPrivate::~LcViewManagerPrivate()" ) 
}

// -----------------------------------------------------------------------------
// LcViewManagerPrivate::terminateSession
// -----------------------------------------------------------------------------
//
void LcViewManagerPrivate::terminateSession()
{
    LC_QDEBUG( "livecomms [UI] -> LcViewManagerPrivate::terminateSession" )
    mEngine->stop(); 
    LC_QDEBUG( "livecomms [UI] <- LcViewManagerPrivate::terminateSession" )
}


// -----------------------------------------------------------------------------
// LcViewManagerPrivate::prepareOrientationChange
// -----------------------------------------------------------------------------
//
void LcViewManagerPrivate::prepareOrientationChange()
{
    LC_QDEBUG( "livecomms [UI] -> LcViewManagerPrivate::prepareOrientationChange()" )
    LC_QDEBUG( "livecomms [UI] <- LcViewManagerPrivate::prepareOrientationChange()" )
}

// -----------------------------------------------------------------------------
// LcViewManagerPrivate::activateCurrentView
// -----------------------------------------------------------------------------
//
void LcViewManagerPrivate::activateCurrentView(bool startup)
{
    LC_QDEBUG( "livecomms [UI] -> LcViewManagerPrivate::activateCurrentView()" )
    if ( mMainWindow.currentView() ) {
        static_cast<LcView*>( mMainWindow.currentView() )->activated();
    }
    
    if ( startup ){
        LC_QDEBUG( "livecomms [UI]  Start engine" )
        startEngine();
    }
    LC_QDEBUG( "livecomms [UI] <- LcViewManagerPrivate::activateCurrentView()" )
}

// -----------------------------------------------------------------------------
// LcViewManagerPrivate::startEngine
// -----------------------------------------------------------------------------
//
void LcViewManagerPrivate::startEngine()
{
    connect(mEngine, SIGNAL(layoutChanged()), this, SLOT(changeView()), 
        Qt::QueuedConnection );
    connect(mEngine, SIGNAL(stopped()), this, SLOT(closeApp()), 
        Qt::QueuedConnection );
    
    mEngine->setUiComponents( mRepository->invitingNote(), 
                              mRepository->waitingNote(), 
                              mRepository->acceptQuery(), 
                              mRepository->recipientQuery(),
                              mRepository->shareOwnVideoQuery() );    
    mEngine->start();
}

// -----------------------------------------------------------------------------
// LcViewManagerPrivate::setCurrentView
// -----------------------------------------------------------------------------
//
void LcViewManagerPrivate::setCurrentView(bool startup)
{
    LC_QDEBUG( "livecomms [UI] -> LcViewManagerPrivate::setCurrentView()" )

    LcView* currView = currentView();
    
    bool found(false);
    foreach ( HbView* view, mMainWindow.views() ){
        found |= ( view == currView );
    }
    if ( !found ){
        mMainWindow.addView( currView );
    }
    mMainWindow.setCurrentView( currView );
    
    LcUtil::asyncSlotCall( 
        this, LC_VIEWMANAGER_SLOT_ACTIVATE_CURRENT_VIEW, Q_ARG( bool, startup ) );
    
    LC_QDEBUG( "livecomms [UI] <- LcViewManagerPrivate::setCurrentView()" )
}

// -----------------------------------------------------------------------------
// LcViewManagerPrivate::currentView
// -----------------------------------------------------------------------------
//
LcView* LcViewManagerPrivate::currentView()
{
    LC_QDEBUG( "livecomms [UI] -> LcViewManagerPrivate::currentView()" )

    LcViewLayoutHint hint = mEngine->viewLayout();
    LcView* nextView = 0;
    
    switch ( hint ) {
        case Lc_ReceivedViewLayout:
            LC_QDEBUG( "livecomms [UI] - switching to receive view -" )
            nextView = mRepository->receiveView();
            break;
        case Lc_ReceivedOnlyViewLayout:
            LC_QDEBUG( "livecomms [UI] - switching to receive only view -" )
            nextView = mRepository->receiveOnlyView();
            break;
        case Lc_AllInOneViewLayout:
            LC_QDEBUG( "livecomms [UI] - switching to allinone view -" )
            nextView = mRepository->allInOneView();
            break;
        case Lc_TwowayViewLayout:
            LC_QDEBUG( "livecomms [UI] - switching to twoway view -" )
            nextView = mRepository->twowayView();
            break;
        case Lc_SharedViewLayout:
            LC_QDEBUG( "livecomms [UI] - switching to send view -" )
            nextView = mRepository->sendView();
            break;
        default:
            LC_QDEBUG( "livecomms [UI] - switching to idle view -" )
            nextView = mRepository->idleView();
            break;
    }
    if ( !nextView && hint > Lc_IdleViewLayout ) {
        LC_QDEBUG( "livecomms [UI] ! could not get view !" )
        LC_QDEBUG( "livecomms [UI] - switching to idle view -" )
        nextView = mRepository->idleView();
    }
    
    if ( !nextView ) {
        LC_QCRITICAL( "livecomms [UI] ! still could not get view !. ABORT" )
        LC_PANIC();
    }
    
    LC_QDEBUG( "livecomms [UI] <- LcViewManagerPrivate::currentView()" )
    return nextView;
}

// -----------------------------------------------------------------------------
// LcViewManagerPrivate::changeView
// -----------------------------------------------------------------------------
//
void LcViewManagerPrivate::changeView()
{
    LC_QDEBUG( "livecomms [UI] -> LcViewManagerPrivate::changeView()" )

    LcView* currentView = static_cast<LcView*>( mMainWindow.currentView() );
    currentView->deactivated();
    setCurrentView();
    
    LC_QDEBUG( "livecomms [UI] <- LcViewManagerPrivate::changeView()" )
}

// -----------------------------------------------------------------------------
// LcViewManagerPrivate::closeApp
// -----------------------------------------------------------------------------
//
void LcViewManagerPrivate::closeApp()
{
    LC_QDEBUG( "livecomms [UI] -> LcViewManagerPrivate::closeApp()" )
    
    qApp->exit(0);
    
    LC_QDEBUG( "livecomms [UI] <- LcViewManagerPrivate::closeApp()" )
}

// End of file
