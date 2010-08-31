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
#include "lceffecthandler.h"
#include "lclogger.h"
#include "lcuidefs.h"
#include "lcuiengine.h"

//SYSTEM
#include <hbinstance.h>
#include <hbwidget.h>


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LcEffectHandler::LcEffectHandler(LcUiEngine& engine,
                                 HbWidget* sharedVideo, 
                                 HbWidget* sharedVideoOverlay,
                                 HbWidget* receivedVideo,
                                 HbWidget* receivedVideoOverlay) :
    mEngine(engine),
    mSharedVideo(sharedVideo),
    mSharedVideoOverlay(sharedVideoOverlay),
    mReceivedVideo(receivedVideo),
    mReceivedVideoOverlay(receivedVideoOverlay),
    mSwapSharedCompleted(false),
    mSwapReceivedCompleted(false),
    mSwapInProgress(false)
{
    HbEffect::add(lcEffectWinId, lcEffectWinDisappear, lcEffectWinDisappearId );
    HbEffect::add(lcEffectWinId, lcEffectWinAppear, lcEffectWinAppearId );
    HbEffect::add(lcEffectWinId, lcEffectWinFlip, lcEffectWinFlipId );
    HbEffect::add(lcEffectWinId, lcEffectWinSwapDown, lcEffectWinSwapDownId );
    HbEffect::add(lcEffectWinId, lcEffectWinSwapUp, lcEffectWinSwapUpId );
    
    mEffectCallBack = lcEffectWinDefaultCallback;
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LcEffectHandler::~LcEffectHandler()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::setVisibility(HbWidget* widget, bool visible)
{
    if ( !HbEffect::effectRunning( widget )) {
        widget->setVisible( visible );
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::setDissappearEffect( 
        LcEffectHandler::DissapperEffect aEffectType )
{
    if ( aEffectType == LcEffectHandler::DissappearToFlip ) {
        mEffectCallBack = "windowDisappearToFlipComplete";
    } else if ( aEffectType == LcEffectHandler::NormalDissappear ) {
        mEffectCallBack = "windowDisappearComplete"; 
    } else {
        mEffectCallBack = lcEffectWinDefaultCallback;
    }
}
// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::showSendWindow()
{
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::showSendWindow()" )
    if ( mSharedVideo ) {
        if ( mSwapSharedCompleted || !HbEffect::effectRunning( mSharedVideoOverlay )) {
            startEffect( mSharedVideoOverlay, 
                         lcEffectWinAppearId, "windowAppearComplete" );
                         
            mSharedVideo->show();
        } else {
            LC_QDEBUG( "livecomms [UI] LcEffectHandler: postponed due ongoing animation" )        
        }
    }
    LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::showSendWindow()" )
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::showReceiveWindow()
{
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::showReceiveWindow()" )
    if ( mReceivedVideo ) {
        if ( mSwapReceivedCompleted || !HbEffect::effectRunning( mReceivedVideoOverlay )) {
            startEffect( mReceivedVideoOverlay, 
                         lcEffectWinAppearId, "windowAppearComplete" );
            mReceivedVideo->show();
        } else {
            LC_QDEBUG( "livecomms [UI] LcEffectHandler: postponed due ongoing animation" )        
        }
    }
   LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::showReceiveWindow()" )
 }


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::hideSendWindow()
{
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::hideSendWindow()" )
    if ( mSharedVideo && mSharedVideo->isVisible() ) {
        startEffect( mSharedVideoOverlay, 
                     lcEffectWinDisappearId, mEffectCallBack.toAscii().data() );
        mSharedVideo->hide();
    }

    mEffectCallBack = lcEffectWinDefaultCallback;
    LC_QDEBUG( "livecomms [UI] <- LcEffectHandlerLcEffectHandler::hideSendWindow()" )
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::hideReceiveWindow()
{
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::hideReceiveWindow()" )
    if ( mReceivedVideo && mReceivedVideo->isVisible() ) {
        startEffect( mReceivedVideoOverlay, 
                     lcEffectWinDisappearId, "windowDisappearComplete" );
        mReceivedVideo->hide();
    }

   LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::hideReceiveWindow()" )
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::swap()
{
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::swap()" )

    // Emit the signal which triggers to loading of new swapped section
    mSwapInProgress = true;
    emit loadSwapLayout();

    startEffects();
    
    LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::swap()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::startEffects()
{
    if( mSharedVideo && mReceivedVideo ) {
     
         mSwapSharedCompleted = false;
         mSwapReceivedCompleted = false;
     
         startEffect( mSharedVideoOverlay, lcEffectWinDisappearId, "windowSwapSharedComplete" );
         startEffect( mReceivedVideoOverlay, lcEffectWinDisappearId, "windowSwapReceivedComplete" );
             
         mSharedVideo->hide();
         mReceivedVideo->hide();
     }
    
}    

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::startEffect( 
    HbWidget* widget, const char *effectId, const char *callBack )
{
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::startEffect()" )
    if( widget ) {
        widget->setOpacity(1.0); // Previous effect might have set opacity to zero
        widget->show();
        LC_QDEBUG_2( "livecomms [UI] window: ",  widget->geometry() )
        LC_QDEBUG_2( "livecomms [UI] effect name: ",  QString(effectId) )
        HbEffect::start( widget, lcEffectWinId, effectId, this, callBack );
    }
    LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::startEffect()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::windowAppearComplete(HbEffect::EffectStatus status)
{
    Q_UNUSED(status);
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::windowAppearComplete()" )
    
    if ( status.item && status.item == mSharedVideoOverlay  && 
         !HbEffect::effectRunning( mSharedVideoOverlay ) ) {
        mSharedVideoOverlay->hide();
                
    }
    if ( status.item && status.item == mReceivedVideoOverlay  && 
         !HbEffect::effectRunning( mReceivedVideoOverlay ) ) {
        mReceivedVideoOverlay->hide();
                
    }

    LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::windowAppearComplete()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::windowDisappearComplete(HbEffect::EffectStatus status)
{
    Q_UNUSED(status);
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::windowDisappearComplete()" )
    LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::windowDisappearComplete()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::windowDisappearToFlipComplete(HbEffect::EffectStatus status)
{
    Q_UNUSED(status);
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::windowDisappearToFlipComplete()" )
    if( mSharedVideo ) {
        mSharedVideo->hide();       
        startEffect( mSharedVideoOverlay, lcEffectWinFlipId, "windowFlipComplete" );
    }
    LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::windowDisappearToFlipComplete()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::windowFlipComplete(HbEffect::EffectStatus status)
{
    Q_UNUSED(status);
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::flipHideComplete()" )
    if( mEngine.isLocalPlayerPlaying() ) {
        showSendWindow();
    }
    LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::flipHideComplete()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::windowSwapSharedComplete(HbEffect::EffectStatus status)
{
    Q_UNUSED(status);
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::windowSwapSharedComplete()" )
    if( mSharedVideo ) {    
        LC_QDEBUG_2( "livecomms [UI] window: ",  mSharedVideo->geometry() )
        LC_QDEBUG_2( "livecomms [UI] effect window: ",  mSharedVideoOverlay->geometry() )
        
        mSwapSharedCompleted = true;
        
        handleSwapCompletion();
    }
    LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::windowSwapSharedComplete()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::windowSwapReceivedComplete(HbEffect::EffectStatus status)
{
    Q_UNUSED(status);
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::windowSwapReceivedComplete()" )
    if( mReceivedVideo ) {    
        LC_QDEBUG_2( "livecomms [UI] window: ",  mReceivedVideo->geometry() )
        LC_QDEBUG_2( "livecomms [UI] effect window: ",  mReceivedVideoOverlay->geometry() )
        
        mSwapReceivedCompleted = true;
        
        handleSwapCompletion();
    }
    LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::windowSwapReceivedComplete()" )
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
bool LcEffectHandler::isSharedVideoAtTop() const
{
    return ( mSharedVideo && mReceivedVideo &&
             mSharedVideo->geometry().top() < mReceivedVideo->geometry().top());
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
bool LcEffectHandler::videosSideBySide() const
{
     return ( mSharedVideo && mReceivedVideo &&
              mSharedVideo->geometry().top() == mReceivedVideo->geometry().top() );
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcEffectHandler::handleSwapCompletion()
{
    LC_QDEBUG( "livecomms [UI] -> LcEffectHandler::handleSwapCompletion()" )
    if ( mSwapSharedCompleted && mSwapReceivedCompleted ){
        LC_QDEBUG( "livecomms [UI]  Both swaps done, complete" )
        
        emit swapCompleted();
        
        if( mEngine.isLocalPlayerPlaying() ) {
            showSendWindow();
        }
        if( mEngine.isRemotePlayerPlaying() ) {
            showReceiveWindow();
        }

    mSwapInProgress = false;
    }
    LC_QDEBUG( "livecomms [UI] <- LcEffectHandler::handleSwapCompletion()" )
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
bool LcEffectHandler::isSwapInProgress()
{
    return mSwapInProgress;
}


// End of file

