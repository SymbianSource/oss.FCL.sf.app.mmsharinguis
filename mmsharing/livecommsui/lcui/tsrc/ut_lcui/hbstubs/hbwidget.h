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

#ifndef HBWIDGET_H
#define HBWIDGET_H

#include <hbglobal.h>
#include <hbstyle.h>
#include <hbnamespace.h>
#include <QGraphicsWidget.h>

class HbEvent;


class HbWidget : public QGraphicsWidget
{
public:
    explicit HbWidget( QGraphicsItem *parent=0, Qt::WindowFlags wFlags=0 );
    virtual ~HbWidget();
    void clearActions();
    HbMainWindow *mainWindow() const;

protected:
    QVariant itemChange ( GraphicsItemChange change, const QVariant & value );

protected:
    /* unit test storage */
    HbMainWindow* mMainWindow;
};

#endif // HBWIDGET_H
