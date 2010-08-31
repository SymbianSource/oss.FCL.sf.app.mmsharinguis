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
#ifndef HBTRANSPARENTWINDOW_H
#define HBTRANSPARENTWINDOW_H

#include <hbglobal.h>
#include <hbwidget.h>


class HbTransparentWindow : public HbWidget
{
public:

    explicit HbTransparentWindow(QGraphicsItem *parent = 0);
    ~HbTransparentWindow();
    
    enum { Type = Hb::ItemType_TransparentWindow };
};

#endif // HBTRANSPARENTWINDOW_H
