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
#ifndef HBLABEL_H
#define HBLABEL_H

#include <hbglobal.h>
#include <hbnamespace.h>
#include <hbwidget.h>

class QGraphicsItem;


class HbLabel : public HbWidget 
{
public:
    explicit HbLabel (QGraphicsItem *parent = 0);
    explicit HbLabel (const QString &displayText, QGraphicsItem *parent = 0);
    ~HbLabel ();

    virtual void setGeometry(const QRectF &rect);
    enum { Type = Hb::ItemType_Label };

    void setIcon( const HbIcon &icon );
    void clear();

public slots:
    void setPlainText(const QString &text);
};

#endif // HBLABEL_H
