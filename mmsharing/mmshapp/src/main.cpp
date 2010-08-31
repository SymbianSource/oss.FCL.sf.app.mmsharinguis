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
#include <hbmainwindow.h>
#include <lcviewmanager.h>
#include <QObject>

int main(int argc, char *argv[])
{
    int err=-1;
    if (argc >= 2)
    {
        LcHbApplication app(argc, argv);    
        QString engineName(QString::fromUtf8(argv[1]));
        QString applicationName("Video Sharing");
        LcViewManager viewManager(engineName,applicationName);
        
        QObject::connect( &app, SIGNAL( handleQuit() ), 
                 &viewManager, SLOT( terminateSession() ),
                 Qt::QueuedConnection );
        
        err = app.exec();
    }
    return err;
}
