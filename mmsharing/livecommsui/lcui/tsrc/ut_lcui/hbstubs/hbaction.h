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
#ifndef HBACTION_H
#define HBACTION_H

#include <QAction>
#include <hbnamespace.h>
#include <hbglobal.h>
#include <hbicon.h>


class HbAction : public QAction
{
public:
    explicit HbAction(QObject *parent = 0);
    explicit HbAction(const QString &text, QObject *parent = 0);
    explicit HbAction(Hb::NavigationAction action, QObject* parent = 0);
    virtual ~HbAction();

    using QAction::setIcon;
    using QAction::icon;
    void setIcon(const HbIcon &icon);
    HbIcon icon() const;
    
private: // Data added for the unit testing stub
    HbIcon mIcon;   
};

#endif // HBACTION_H
