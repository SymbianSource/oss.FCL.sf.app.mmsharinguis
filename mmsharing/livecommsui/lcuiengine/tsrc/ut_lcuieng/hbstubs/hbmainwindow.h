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

#ifndef HBMAINWINDOW_H
#define HBMAINWINDOW_H

#include <QGraphicsView>
#include <hbglobal.h>
#include <hbnamespace.h>

class QGraphicsWidget;
class HbAction;
class HbView;


class HbMainWindow : public QGraphicsView
{
public:
    explicit HbMainWindow(QWidget *parent = 0, Hb::WindowFlags windowFlags = Hb::WindowFlagNone);
    ~HbMainWindow();
};

#endif // HBMAINWINDOW_H
