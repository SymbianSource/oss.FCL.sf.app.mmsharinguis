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
#include "lcapplication.h"
#include "mustestdirector.h"
#include <musmanagercommon.h>

//SYSTEM
#include <QDebug.h>
#include <Qstring>

LcHbApplication* LcHbApplication::self = 0;


// -----------------------------------------------------------------------------
// LcHbApplication::LcHbApplication
// -----------------------------------------------------------------------------
//
LcHbApplication::LcHbApplication(int &argc, char *argv[], QString useCase) :
    HbApplication(argc, argv)
{    
	  qDebug() << "LcHbApplication::LcHbApplication() ->";
    self = this;
    iTestDirector =  CMusTestDirector::NewL();
    setUseCase( useCase );    
    qDebug() << "LcHbApplication::LcHbApplication() -<";
}

// -----------------------------------------------------------------------------
// LcHbApplication::~LcHbApplication
// -----------------------------------------------------------------------------
//
LcHbApplication::~LcHbApplication()
{
    self = 0;
}


// -----------------------------------------------------------------------------
// LcHbApplication::quit
// -----------------------------------------------------------------------------
//
void LcHbApplication::quit()
{
    qDebug() << "LcHbApplication::quit()";
    if (self) {
        emit self->handleQuit();
    }
}

// -----------------------------------------------------------------------------
// LcHbApplication::quit
// -----------------------------------------------------------------------------
//
void LcHbApplication::setUseCase( QString useCase )
{
    qDebug() << "LcHbApplication::setUseCase()";
    if( useCase == "0") // live
    {
    	  qDebug() << "LcHbApplication::LiveUseCase()";
        iTestDirector->ConfigureLiveSharing();
    }
    else if( useCase == "1") // clip
    {
        if (self) emit self->handleQuit();
    }
    else if( useCase == "2") // stil
    {
        if (self) emit self->handleQuit();
    }
    else if( useCase == "3") // EMusReceive
    {
        qDebug() << "LcHbApplication::ReceiveUseCase()";
        iTestDirector->ConfigureReceiveSharing();
    }
    else if( useCase == "4") // EMusContinue
    {
        if (self) emit self->handleQuit();
    }
    else if( useCase == "5") // EMusTwoWayVideo
    {
        if (self) emit self->handleQuit();
    }
    else if( useCase == "6") // EMusReceiveTwoWayVideo
    {
        if (self) emit self->handleQuit();
    }
    else
    {
        if (self) emit self->handleQuit();
    }
}
