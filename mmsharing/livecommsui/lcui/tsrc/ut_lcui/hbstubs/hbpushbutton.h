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
* Description:  HbPushButton class definition
*
*/


#ifndef HBPUSHBUTTON_H
#define HBPUSHBUTTON_H

#include <hbabstractbutton.h>
#include <hbglobal.h>

class HbFrameDrawer;

class HbPushButton : public HbAbstractButton
{

public:
    explicit HbPushButton( QGraphicsItem *parent = 0 );
    explicit HbPushButton( const QString &text, QGraphicsItem *parent = 0 );
    virtual ~HbPushButton( );
    void setFrameBackground( HbFrameDrawer *backgroundFrameDrawer );
};

#endif // HBPUSHBUTTON_H
