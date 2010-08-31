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
#include "lcuiengine_p.h"
#include "lcuiengine.h"
#include "lclogger.h"


// -----------------------------------------------------------------------------
// LcUiEngine::LcUiEngine
// -----------------------------------------------------------------------------
//
LcUiEngine::LcUiEngine( const QString& engineName,
                        const QString& applicationDisplayName )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::LcUiEngine()" )
    d = new LcUiEnginePrivate( *this,engineName,applicationDisplayName );
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::LcUiEngine()" )   
}

// -----------------------------------------------------------------------------
// LcUiEngine::~LcUiEngine
// -----------------------------------------------------------------------------
//
LcUiEngine::~LcUiEngine()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::~LcUiEngine()" )
    delete d;
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::~LcUiEngine()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::zoom
// -----------------------------------------------------------------------------
//
void LcUiEngine::zoom( int value )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::zoom()" )
    d->zoom(value);
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::zoom()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::stop
// -----------------------------------------------------------------------------
//
void LcUiEngine::stop()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::stop()" )    
    d->stop();
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::stop()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::start
// -----------------------------------------------------------------------------
//
void LcUiEngine::start()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::start()" )    
    d->start();
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::start()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::sessionTitle
// -----------------------------------------------------------------------------
//
const QString& LcUiEngine::sessionTitle() const
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::sessionTitle()" )
    return d->sessionTitle();
}

// -----------------------------------------------------------------------------
// LcUiEngine::recipient
// -----------------------------------------------------------------------------
//
const QString& LcUiEngine::recipient() const
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::recipient()" )
    return d->recipient();    
}

// -----------------------------------------------------------------------------
// LcUiEngine::phoneNumber
// -----------------------------------------------------------------------------
//
const QString& LcUiEngine::phoneNumber() const
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::phoneNumber()" )
    return d->phoneNumber();    
}

// -----------------------------------------------------------------------------
// LcUiEngine::viewLayout
// -----------------------------------------------------------------------------
//
LcViewLayoutHint LcUiEngine::viewLayout() const
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::viewLayout()" )
    return d->viewLayout();    
}

// -----------------------------------------------------------------------------
// LcUiEngine::setContentAreas
// -----------------------------------------------------------------------------
//
void LcUiEngine::setContentAreas( const QRectF& sharedContent,
                                  const QRectF& receivedContent )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::setContentAreas()" )
    d->setContentAreas( sharedContent,receivedContent );
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::setContentAreas()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::setOrientation
// -----------------------------------------------------------------------------
//
bool LcUiEngine::setOrientation( Qt::Orientation orientation )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::setOrientation()" )
    bool ret = d->setOrientation( orientation );
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::setOrientation()" )
    return ret;
}

// -----------------------------------------------------------------------------
// LcUiEngine::setUiComponents
// -----------------------------------------------------------------------------
//
void LcUiEngine::setUiComponents( HbProgressDialog* invitingNote,
                                  HbProgressDialog* waitingNote,
                                  HbDialog* acceptQuery,
                                  HbDialog* recipientQuery,
                                  HbDialog* shareOwnVideoQuery )
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::setUiComponents()" )
    d->setUiComponents( invitingNote,waitingNote,acceptQuery,recipientQuery,
                                                          shareOwnVideoQuery );
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::setUiComponents()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::toggleMute
// -----------------------------------------------------------------------------
//
void LcUiEngine::toggleMute()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::toggleMute()" )
    d->toggleMute();
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::toggleMute()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::isMuted
// -----------------------------------------------------------------------------
//
bool LcUiEngine::isMuted() const
{    
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::isMuted()" )
    return d->isMuted();
}

// -----------------------------------------------------------------------------
// LcUiEngine::toggleSpeaker
// -----------------------------------------------------------------------------
//
void LcUiEngine::toggleSpeaker()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::toggleSpeaker()" )    
    d->toggleSpeaker();
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::toggleSpeaker()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::isSpeakerOn
// -----------------------------------------------------------------------------
//
bool LcUiEngine::isSpeakerOn() const
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::isSpeakerOn()" )
    return d->isSpeakerOn();
}

// -----------------------------------------------------------------------------
// LcUiEngine::zoomValues
// -----------------------------------------------------------------------------
//
int LcUiEngine::zoomValues( LcControlValues& values ) const
{    
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::zoomValues()" )
    return d->zoomValues(values);
}

// -----------------------------------------------------------------------------
// LcUiEngine::toggleEnableCamera
// -----------------------------------------------------------------------------
//
void LcUiEngine::toggleDisableCamera()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::toggleDisableCamera()" )
    d->toggleDisableCamera();
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::toggleDisableCamera()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::setCameraMain
// -----------------------------------------------------------------------------
//
void LcUiEngine::setMainCamera(const bool aMainCamera)
{
    LC_QDEBUG_2( "livecomms [UI] -> LcUiEngine::setCameraMain()", aMainCamera )
    d->setMainCamera( aMainCamera );
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::setCameraMain()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::cameraIndex
// -----------------------------------------------------------------------------
//
bool LcUiEngine::mainCamera() const
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::mainCamera()" )
    return d->mainCamera();
}

// -----------------------------------------------------------------------------
// LcUiEngine::toggleCamera
// -----------------------------------------------------------------------------
//
void LcUiEngine::toggleCamera()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::toggleCamera()" )
    d->toggleCamera();
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::toggleCamera()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::fullScreenMode
// -----------------------------------------------------------------------------
//
bool LcUiEngine::fullScreenMode()
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::fullScreenMode()" )
    return d->fullScreenMode();
}

// -----------------------------------------------------------------------------
// LcUiEngine::setFullScreenMode
// -----------------------------------------------------------------------------
//
void LcUiEngine::setFullScreenMode(bool aMode)
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::setFullScreenMode()" )
    d->setFullScreenMode( aMode );
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::setFullScreenMode()" )
}


// -----------------------------------------------------------------------------
// LcUiEngine::updateSession
// -----------------------------------------------------------------------------
//
void LcUiEngine::updateSession()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngine::updateSession()" )
    d->updateSession();
    LC_QDEBUG( "livecomms [UI] <- LcUiEngine::updateSession()" )
}

// -----------------------------------------------------------------------------
// LcUiEngine::isLocalPlayerPlaying
// -----------------------------------------------------------------------------
//
bool LcUiEngine::isLocalPlayerPlaying() const
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::isLocalPlayerPlaying()" )
    return d->isLocalPlayerPlaying();
}

// -----------------------------------------------------------------------------
// LcUiEngine::isRemotePlayerPlaying
// -----------------------------------------------------------------------------
//
bool LcUiEngine::isRemotePlayerPlaying() const
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::isRemotePlayerPlaying()" )
    return d->isRemotePlayerPlaying();
}

// -----------------------------------------------------------------------------
// LcUiEngine::isMinimized
// -----------------------------------------------------------------------------
//
bool LcUiEngine::isMinimized() const
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::isMinimized()" )
    return d->isMinimized();
}

// -----------------------------------------------------------------------------
// LcUiEngine::minimize
// -----------------------------------------------------------------------------
//
void LcUiEngine::minimize()
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::minimize()" )
    // TODO : Remove this function when XqServiceUtil taken into use compleatly 
    d->minimize();
}

// -----------------------------------------------------------------------------
// LcUiEngine::maximize
// -----------------------------------------------------------------------------
//
void LcUiEngine::maximize()
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::maximize()" )
    // TODO : Remove this function when XqServiceUtil taken into use compleatly
    d->maximize();
}

// -----------------------------------------------------------------------------
// LcUiEngine::shareImage
// -----------------------------------------------------------------------------
//
void LcUiEngine::shareImage(const QString& fileName)
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::shareImage()" )
    d->shareImage(fileName);
}

// -----------------------------------------------------------------------------
// LcUiEngine::shareCamera
// -----------------------------------------------------------------------------
//
void LcUiEngine::shareCamera()
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::shareCamera()" )
    d->shareCamera();
}

// -----------------------------------------------------------------------------
// LcUiEngine::setCurrentView
// -----------------------------------------------------------------------------
//
void LcUiEngine::setCurrentView(HbView* view)
{
    LC_QDEBUG( "livecomms [UI] <-> LcUiEngine::setCurrentView()" )
    d->setCurrentView(view);
}


// -----------------------------------------------------------------------------
// LcUiEngine::SendDialTone
// -----------------------------------------------------------------------------
//

bool LcUiEngine::SendDialTone( const QChar aKey )
{
    return d->SendDialTone(aKey);
}


// End of File
