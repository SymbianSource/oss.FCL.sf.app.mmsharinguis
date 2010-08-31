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

#ifndef HBLINEEDIT_H
#define HBLINEEDIT_H

#include <hbglobal.h>
#include <hbnamespace.h>
#include <hbwidget.h>


class HbLineEdit: public HbWidget
{
public:

    explicit HbLineEdit(const QString &text, QGraphicsItem *parent = 0);
    virtual ~HbLineEdit();

    enum {
        Type = Hb::ItemType_LineEdit
    };

};

#endif // HBLINEEDIT_H
