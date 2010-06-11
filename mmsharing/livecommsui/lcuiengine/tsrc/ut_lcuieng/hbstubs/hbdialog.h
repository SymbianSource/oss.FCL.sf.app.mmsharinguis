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
#ifndef HBDIALOG_H
#define HBDIALOG_H

#include <hbglobal.h>
#include <hbwidget.h>


class HbAction;


class HbDialog : public HbWidget
{
public:
    enum DismissPolicy
        {
            NoDismiss   = 0,
            TapInside   = 1,
            TapOutside  = 2,
            TapAnywhere = TapInside | TapOutside
        };
    enum DefaultTimeout
      {
          NoTimeout,
          ConfirmationNoteTimeout,
          StandardTimeout,
          ContextMenuTimeout
      };
    
    explicit HbDialog(QGraphicsItem *parent = 0);
    virtual  ~HbDialog();

    void setContentWidget(QGraphicsWidget *contentWidget);
    enum { Type = Hb::ItemType_Popup };
    void setTimeout( int timeout ) ;
    void setDismissPolicy( HbDialog::DismissPolicy dismissPolicy );

public slots:
    void open(QObject* receiver = 0, const char* member = 0);
protected:

    QVariant itemChange ( GraphicsItemChange change, const QVariant & value );
};


#endif // HBDIALOG_H
