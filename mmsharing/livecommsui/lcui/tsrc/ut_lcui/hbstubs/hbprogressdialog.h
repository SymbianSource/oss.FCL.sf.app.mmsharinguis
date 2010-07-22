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
#ifndef HBPROGRESSDIALOG_H
#define HBPROGRESSDIALOG_H

#include <hbdialog.h>

class HbProgressDialog : public HbDialog
{
public:

    enum ProgressDialogType { ProgressNote,WaitNote };
    explicit HbProgressDialog(QGraphicsItem *parent = 0);

    enum { Type = Hb::ItemType_ProgressDialog };
        
    void setText(const QString &text);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant& value);
};

#endif // HBPROGRESSDIALOG_H
