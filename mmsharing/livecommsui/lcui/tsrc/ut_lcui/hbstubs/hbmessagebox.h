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

#ifndef HBMESSAGEBOX_H
#define HBMESSAGEBOX_H

#include <hbglobal.h>
#include <hbdialog.h>
#include <hbicon.h>

class HbMessageBoxPrivate;
class HbStyleOptionMessageBox;

class HB_WIDGETS_EXPORT HbMessageBox : public HbDialog
{

public:
    enum MessageBoxType {
        MessageTypeInformation,
        MessageTypeQuestion,
        MessageTypeWarning
    };
    explicit HbMessageBox(MessageBoxType type=MessageTypeInformation,QGraphicsItem *parent = 0);
    explicit HbMessageBox(const QString &text,MessageBoxType type =MessageTypeInformation, QGraphicsItem *parent = 0);
    virtual ~HbMessageBox();

    void setText(const QString &text);
    QString text() const;

    void setIcon(const HbIcon &icon);
    HbIcon icon() const;

    void setIconAlignment(Qt::Alignment align);
    Qt::Alignment iconAlignment() const;

    enum { Type = Hb::ItemType_MessageBox };
    int type() const { return Type; }

public:
    static bool launchQuestionMessageBox(const QString &questionText,
                                         const QString &primaryButtonText = tr("Yes"),
                                         const QString &secondaryButtonText = tr("No"),
                                         QGraphicsWidget *headWidget = 0,
                                         QGraphicsScene *scene = 0,
                                         QGraphicsItem *parent = 0 );

    static void launchInformationMessageBox(const QString &informationText,
                                            QGraphicsWidget *headWidget = 0,
                                            QGraphicsScene *scene = 0,
                                            QGraphicsItem *parent = 0 );

    static void launchWarningMessageBox(const QString &warningText,
                                        QGraphicsWidget *headWidget = 0,
                                        QGraphicsScene *scene = 0,
                                        QGraphicsItem *parent = 0 );

public slots:
    void updatePrimitives();

protected:
    HbMessageBox(HbMessageBoxPrivate &dd, QGraphicsItem *parent);
    void initStyleOption(HbStyleOptionMessageBox *option) const;
    void keyPressEvent(QKeyEvent *event);
};

#endif // HB_MESSAGEBOX_H

