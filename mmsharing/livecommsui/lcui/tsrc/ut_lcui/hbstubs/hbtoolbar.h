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
#ifndef HBTOOLBAR_H
#define HBTOOLBAR_H

#include <hbglobal.h>
#include <hbnamespace.h>
#include <hbwidget.h>


class HbToolBar : public HbWidget
{
public:
    explicit HbToolBar(QGraphicsItem *parent = 0);
    virtual ~HbToolBar();

    enum { Type = Hb::ItemType_ToolBar };
};

#endif // HBTOOLBAR_H
