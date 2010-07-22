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

    enum DefaultTimeout
    {
        NoTimeout,
        ConfirmationNoteTimeout,
        StandardTimeout,
        ContextMenuTimeout,
    };

    enum DismissPolicy
    {
        NoDismiss   = 0,
        TapInside   = 1,
        TapOutside  = 2,
        TapAnywhere = TapInside | TapOutside
    };

    explicit HbDialog(QGraphicsItem *parent = 0);
    virtual  ~HbDialog();

    int timeout() const;
    void setTimeout(int timeout);
    void setTimeout(HbDialog::DefaultTimeout timeout);
    void setHeadingWidget(QGraphicsWidget *headingWidget);
    void setContentWidget(QGraphicsWidget *contentWidget);
    Hb::SceneItems unfadedItems() const;
    void setUnfadedItems(Hb::SceneItems unfadedItems);
    HbDialog::DismissPolicy dismissPolicy() const;
    void setDismissPolicy(HbDialog::DismissPolicy dismissPolicy);
    enum { Type = Hb::ItemType_Popup };

protected:

    QVariant itemChange ( GraphicsItemChange change, const QVariant & value );
    
private: // Data added for the unit testing stub
	
	Hb::SceneItems mUnfadedItems;
	DismissPolicy mDismissPolicy;
        int mTimeout;
};


#endif // HBDIALOG_H
