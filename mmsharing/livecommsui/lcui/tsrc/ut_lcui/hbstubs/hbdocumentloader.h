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
#ifndef HBDOCUMENTLOADER_H
#define HBDOCUMENTLOADER_H

#include <hbglobal.h>
#include <QString>
#include <QObjectList>

class QObject;
class QGraphicsWidget;
class HbLabel;
class HbAction;
class HbPushButton;

class HbDocumentLoader
{
public:
    HbDocumentLoader();
    virtual ~HbDocumentLoader();

    QObjectList load( const QString &fileName, const QString &section , bool *ok = 0 );
    QObjectList load( const QString &fileName, bool *ok = 0 );
    QGraphicsWidget *findWidget(const QString &name);
    QObject *findObject(const QString &name) const;
    bool setObjectTree( QObjectList roots );
    void reset();
    
protected:
    virtual QObject *createObject(const QString& type, const QString &name);
    
public: // Data added for the unit testing stub
    HbLabel* mLabel;
    HbAction* mAction;
    HbPushButton* mPushButton;
    QList<QGraphicsWidget*> mWidgets;
    bool mReturnSendVideo;
    QString mLayoutSection;

    QList<HbAction*> mActions;

    friend class UT_LcView;
    friend class UT_LcUiComponentRepository;
    
};

#endif // HBDOCUMENTLOADER_H
