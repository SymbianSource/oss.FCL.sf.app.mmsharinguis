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
#ifndef HBMENU_H
#define HBMENU_H

#include <hbdialog.h>


class HbMenu : public HbDialog
{
public:

    explicit HbMenu(QGraphicsItem *parent = 0);
    ~HbMenu();

    using HbDialog::addAction;
    HbAction *addAction(const QString &text);

    bool isEmpty() const;

    enum { Type = Hb::ItemType_Menu };

public slots:
    void open(QObject* receiver = 0, const char* member = 0);
    
private: // Data added for the unit testing stub
    bool mIsEmpty;     
};

#endif // HBMENU_H
