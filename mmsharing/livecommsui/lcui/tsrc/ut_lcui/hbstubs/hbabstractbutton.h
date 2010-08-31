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
* Description:  HbAbstractButton class definition
*
*/


#ifndef HBABSTRACTBUTTON_H
#define HBABSTRACTBUTTON_H

#include <hbglobal.h>
#include <hbwidget.h>
#include <hbnamespace.h>

class HbAbstractButton : public HbWidget
{
public:
    explicit HbAbstractButton( QGraphicsItem *parent = 0 );
    virtual ~HbAbstractButton();


signals:
    void clicked( bool checked = false );
};

#endif // HBABSTRACTBUTTON_H
