/*
* Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/


#ifndef HBEFFECT_H
#define HBEFFECT_H

#include <QRect>
#include <hbglobal.h>
#include <hbnamespace.h>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QVariant;
QT_END_NAMESPACE

class HbEffect
{

public:

    struct EffectStatus {
        QGraphicsItem *item;
        QString effectEvent;
        Hb::EffectEvent reason;
        QVariant userData;
    };

    static bool start(QGraphicsItem *item, 
                            const QString &itemType, 
                            const QString &effectEvent,
                            QObject *receiver = 0,
                            const char *member = 0,
                            const QVariant &userData = QVariant());

    static bool add(const QString &itemType, const QString &filePath, const QString &effectEvent = QString() );

    static bool effectRunning(QGraphicsItem *item, const QString &effectEvent = QString());

    static bool mRunning;
    
private:
    HbEffect();
};

#endif // HBEFFECT_H
