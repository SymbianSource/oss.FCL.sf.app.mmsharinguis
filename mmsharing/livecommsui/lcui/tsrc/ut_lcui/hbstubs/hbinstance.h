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
* Description: Orbit stubs for Live Comms UI unit tests
*
*/
#ifndef HBINSTANCE_H
#define HBINSTANCE_H

#include <hbglobal.h>
#include <hbmainwindow.h>
#include <hbtheme.h>
#include <QList.h>

class QGraphicsScene;

#define hbInstance (HbInstance::instance())

class HbInstance
{
public:
    static HbInstance *instance();
    QList<HbMainWindow *> allMainWindows() const;

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);
    
    QGraphicsScene* scene() const;

private:
    HbInstance();
    ~HbInstance();
    
private: // Data added for the unit testing stub
    QList<HbMainWindow *> mAllMainWindows;
    QGraphicsScene* mScene;
    Qt::Orientation mOrientation;
};

#endif // HBINSTANCE_H
