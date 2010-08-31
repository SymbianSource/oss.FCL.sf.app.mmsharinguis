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
#include <QDebug>

LcHbApplication* LcHbApplication::self = 0;


// -----------------------------------------------------------------------------
// LcHbApplication::LcHbApplication
// -----------------------------------------------------------------------------
//
LcHbApplication::LcHbApplication(int &argc, char *argv[]) :
    HbApplication(argc, argv)
{    
    self = this;
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
