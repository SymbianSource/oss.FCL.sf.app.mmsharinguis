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
#include "lcconfiguration.h"
#include "LcUiEngine_p.h"
#include "lcuidefs.h"
#include "lclogger.h"
#include "lcuienginesimulator.h"


//SYSTEM
#include <QTimer>
#include <hbdialog.h>
#include <hbaction.h>
#include <hblabel.h>
#include <hblineedit.h>
#include <hbinstance.h>
#include <hbmessagebox.h>


#define LC_VTPLUGIN_NAME "Videotelephony";

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::LcUiEngineSimulator
// -----------------------------------------------------------------------------
//
LcUiEngineSimulator::LcUiEngineSimulator( LcUiEnginePrivate& uiEngine)
    :mUiEngine( uiEngine )
{    
    mConfig = new LcConfiguration();
    mConfig->parse();
    mTimer = new QTimer( this );
    connect( mTimer, SIGNAL(timeout()), this, SLOT(timerEvent()) );    
    iSimulatorCount = 0;
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::~LcUiEngineSimulator
// -----------------------------------------------------------------------------
//
LcUiEngineSimulator::~LcUiEngineSimulator()
{
    LC_QDEBUG( "livecomms [UI] -> LcUiEngineSimulator::~LcUiEngineSimulator()" )
    delete mConfig;
    delete mTimer;
    LC_QDEBUG( "livecomms [UI] <- LcUiEngineSimulator::~LcUiEngineSimulator()" )
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::~LcUiEngineSimulator
// -----------------------------------------------------------------------------
//
void LcUiEngineSimulator::start()
{
   mTimer->start(mConfig->mTimerDuration);
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::~LcUiEngineSimulator
// -----------------------------------------------------------------------------
//
void LcUiEngineSimulator::stop()
{
   mTimer->stop();
   mUiEngine.stopped();
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::~LcUiEngineSimulator
// -----------------------------------------------------------------------------
//
int LcUiEngineSimulator::layout()
{
   return mConfig->mLayout;
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::~LcUiEngineSimulator
// -----------------------------------------------------------------------------
//
QString LcUiEngineSimulator::phoneNo()
{
   return mConfig->mPhoneNumber;
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::~LcUiEngineSimulator
// -----------------------------------------------------------------------------
//
QString LcUiEngineSimulator::sessionTitle()
{
   return mConfig->mSessionTitle;
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::~LcUiEngineSimulator
// -----------------------------------------------------------------------------
//
QString LcUiEngineSimulator::remoteAddress()
{
   return mConfig->mRecipient;
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::zoom
// -----------------------------------------------------------------------------
//
void LcUiEngineSimulator::timerEvent()
{    
    switch( mConfig->mLayout ){
    case 0 : {
            break;
        }
    case 1:{
            simulateLiveSharingSend();
            break;
        }
    case 2:{
            simulateLiveSharingReceive();
            break;
        }
    case 3:{
            // TODO : Add simulation
            break;
        }
    case 4:{
            simulateTwoWaySharing();
            break;
        }
    case 5:{
            simulateVideoTelephony();
            break;
        }
    default:{
            
            break;
        }
    }

    iSimulatorCount++;
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::zoom
// -----------------------------------------------------------------------------
//
void LcUiEngineSimulator::simulateLiveSharingSend()
{
    switch ( iSimulatorCount ){
        case 0:{
                mUiEngine.mInvitingNote->show();
                mUiEngine.localPlayerPreparing();
                break;
            }
        case 1:{
                mUiEngine.mInvitingNote->hide();;
                mUiEngine.localPlayerBuffering();
                break;
            }
        case 2:{
                mUiEngine.localPlayerPlaying();
                break;
            }
        default:{
                mUiEngine.sessionDurationChanged(QString::number(iSimulatorCount));
                break;
            }
    }    
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::zoom
// -----------------------------------------------------------------------------
//
void LcUiEngineSimulator::simulateLiveSharingReceive()
{
    switch ( iSimulatorCount ){
    case 0:{
            mUiEngine.mAcceptQuery->show();
            mUiEngine.remotePlayerPreparing();
            break;
        }
    case 1:{
            mUiEngine.mAcceptQuery->hide();
            mUiEngine.mWaitingNote->show();            
            mUiEngine.remotePlayerBuffering();
            break;
        }
    case 2:{
            mUiEngine.mWaitingNote->hide();            
            mUiEngine.remotePlayerPlaying();
            break;
        }
    default:{
            mUiEngine.sessionDurationChanged(QString::number(iSimulatorCount));
            break;
        }
    }    
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::zoom
// -----------------------------------------------------------------------------
//
void LcUiEngineSimulator::simulateTwoWaySharing()
{
    switch ( iSimulatorCount ){
    case 0:{
            mUiEngine.localPlayerPreparing();
            mUiEngine.remotePlayerPreparing();
            break;
        }
    case 1:{
            mUiEngine.localPlayerBuffering();
            mUiEngine.remotePlayerBuffering();
            break;
        }
    case 2:{
            mUiEngine.localPlayerPlaying();
            mUiEngine.remotePlayerPlaying();
            break;
        }
    default:{
            mUiEngine.sessionDurationChanged(QString::number(iSimulatorCount));
            break;
        }
    }    
}

// -----------------------------------------------------------------------------
// LcUiEngineSimulator::zoom
// -----------------------------------------------------------------------------
//
void LcUiEngineSimulator::simulateVideoTelephony()
{
    switch ( iSimulatorCount ){
    case 0:{
            mUiEngine.localPlayerPreparing();            
            mUiEngine.remotePlayerPreparing();
            break;
        }
    case 1:{
            mUiEngine.localPlayerBuffering();
            mUiEngine.remotePlayerBuffering();
            break;
        }    
    case 2:{
            if( !mUiEngine.isRemotePlayerPlaying() ){
                mUiEngine.remotePlayerPlaying();
            }
            break;
        }
    case 3:{
            if( !mUiEngine.isLocalPlayerPlaying() ){
                mUiEngine.localPlayerPlaying();
            }
            break;
        }
    default:{
            mUiEngine.sessionDurationChanged(QString::number(iSimulatorCount));
            break;
        }
    }    
}

// End of File
