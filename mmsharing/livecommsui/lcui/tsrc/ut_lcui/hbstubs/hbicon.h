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
#ifndef HBICON_H
#define HBICON_H

#include <hbglobal.h>


class HbIcon
{
public:
    HbIcon(const QString &iconName);
    ~HbIcon();
    
    bool operator==(const HbIcon &other) const;
    
private: // Data added for the unit testing stub
    QString mName;    
};

#endif // HBICON_H
