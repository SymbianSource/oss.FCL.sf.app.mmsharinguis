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
#ifndef HBNOTE_H
#define HBNOTE_H

#include <hbglobal.h>
#include <hbpopup.h>


class HbNote : public HbPopup
{
public:
    explicit HbNote(QGraphicsItem *parent = 0);
    HbNote(const QString &text, QGraphicsItem *parent = 0);
    virtual ~HbNote();

    enum { Type = Hb::ItemType_Note };

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
};

#endif // HBNOTE_H
