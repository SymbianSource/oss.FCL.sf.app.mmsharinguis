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
* Description:
*
*/
#ifndef LCVIEWMANAGER_H
#define LCVIEWMANAGER_H

#include <QObject>
#include <lcexport.h>

class LcViewManagerPrivate;
class HbMainWindow;

/**
 * 
 */
class LcViewManager : public QObject
{
    Q_OBJECT
    friend class UT_LcUiViewManager;
    
public: // The exported API

    LIVECOMMS_EXPORT LcViewManager(
        const QString &engineName,
        const QString &applicationDisplayName);
        
    LIVECOMMS_EXPORT ~LcViewManager();
        
public slots:

    //terminateSession will close engine session asynchronously
    //when session is closed application will be also closed
    LIVECOMMS_EXPORT void terminateSession();
    
private: //data 
    
    LcViewManagerPrivate* d_ptr;
};
                  
#endif // LCVIEWMANAGER_H
