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

#ifndef LCACTIVITYMANAGER_H
#define LCACTIVITYMANAGER_H

//  INCLUDES
#include <QObject>

// FORWARD DECLARATION

// CLASS DECLARATION

/**
 * 
 */
class LcActivityManager : public QObject
{
    friend class UT_LcActivityManager;
    Q_OBJECT

public:  

    explicit LcActivityManager();
    ~LcActivityManager();
        
public slots:

    void enableActivitySimulation(bool enable);
    
public:
    
    bool isActivitySimulationEnabled();
    
    
private:
    
    int initInactivityTimeout();
    void stopTimer();
    void restartTimer();
    
    //from QObject
    void timerEvent(QTimerEvent *event);

private:

    int mTimerId;
    int mInactivityTimeout;
    
};

#endif      // LCACTIVITYMANAGER_H


// End of File
