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


#ifndef UT_LCUIDEFS_H_
#define UT_LCUIDEFS_H_

#include <QStringList>

class HbView;

const char lcutNoCall[] = "NOCALL";
const char lcutStub_LcUiEngine_start[] = "LcUiEngine::start";
const char lcutStub_LcUiEngine_setOrientation[] = "LcUiEngine::setOrientation";
const char lcutStub_LcUiEngine_setContentAreas[] = "LcUiEngine::setContentAreas";
const char lcutStub_LcUiEngine_mute[] = "LcUiEngine::mute";
const char lcutStub_LcUiEngine_zoom[] = "LcUiEngine::zoom";
const char lcutStub_LcUiEngine_enableCamera[] = "LcUiEngine::enableCamera";
const char lcutStub_LcUiEngine_disableCamera[] = "LcUiEngine::disableCamera";
const char lcutStub_LcUiEngine_toggleCamera[] = "LcUiEngine::toggleCamera";
const char lcutStub_LcUiEngine_speaker[] = "LcUiEngine::speaker";
const char lcutStub_LcUiEngine_stop[] = "LcUiEngine::stop";
const char lcutStub_LcUiEngine_updateSession[] = "LcUiEngine::updateSession";


const char lcEngineName[] = "engineName";
const char lcDisplayName[] = "displayName";

static QStringList* lcutStub_LcUiEngine_calledMethods = 0;
static int lcutStub_LcUiEngine_failure = 0;
static int lcutStub_LcUiEngine_layout = -1;

static bool lcutStub_LcUiEngine_cameraMain = false;
static bool lcutStub_LcUiEngine_muted = false;
static bool lcutStub_LcUiEngine_speakerOn = false;
static bool lcutStub_LcUiEngine_localplaying = true;
static bool lcutStub_LcUiEngine_remoteplaying = true;
static bool lcutStub_LcUiEngine_windowState_minimized = false;
static bool lcutStub_LcUiEngine_isImageShared = false;

extern QStringList& lcutStub_LcUiEngine_calledMethodsInstance();
extern bool lcutStub_LcUiEngine_expectCall( const QString& call, int callNdx = 0);
extern void lcutStub_LcUiEngine_reset();
extern void lcutStub_LcUiEngine_setFailure( int failure );
extern void lcutStub_LcUiEngine_setLayout( int layout );
extern void lcutStub_LcUiEngine_setCameraMain( bool val );
extern void lcutStub_LcUiEngine_setMuted( bool val );
extern void lcutStub_LcUiEngine_setSpeaker( bool val );
extern void lcutStub_LcUiEngine_setLocalPlaying( bool val );
extern void lcutStub_LcUiEngine_setRemotePlaying( bool val );
extern HbView* lcutStub_LcUiEngine_getCurrentView();
extern QChar lcutStub_LcUiEngine_getDialToneChar();

// End of file




#endif /* UT_LCUIDEFS_H_ */
