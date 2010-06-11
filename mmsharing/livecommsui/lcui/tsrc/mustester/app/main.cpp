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

//SYSTEM
#include <lcviewmanager.h>
#include <QObject.h>

int main(int argc, char *argv[])
{
    int err=-1;
    QString useCase = "0"; // live case
    QString engineName("MultimediaSharing");
    QString applicationName("Video Sharing");
    if (argc >= 2)
    {
    	  useCase  = QString::fromUtf8( argv[2] );
    }
    LcHbApplication app(argc,argv,useCase);    
    LcViewManager viewManager(engineName,applicationName);
    QObject::connect( &app, SIGNAL( handleQuit() ), 
                 &viewManager, SLOT( terminateSession() ),
                 Qt::QueuedConnection );
        
    err = app.exec();
    return err;
}
