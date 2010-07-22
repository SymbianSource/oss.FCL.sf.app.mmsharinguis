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
#ifndef LCUIENGINESIMULATOR_H
#define LCUIENGINESIMULATOR_H

//SYSTEM
#include <QObject>
#include <QString>
#include <QTime>

class QTimer;
class LcUiEnginePrivate;
class LcConfiguration;

/*
 * Live Comms simulator
 */
class LcUiEngineSimulator : public QObject
{
    Q_OBJECT

public:

    LcUiEngineSimulator ( LcUiEnginePrivate& uiEngine );
    ~LcUiEngineSimulator ();
    int layout();
    QString sessionTitle();
    QString remoteAddress();
    QString phoneNo();
    void setSimulation(int index){iSimulatorCount = index;};
    int simulation(){return iSimulatorCount;};
public:
    void start();
    void stop();

public slots:

     void timerEvent();

private:
     void simulateLiveSharingSend();
     void simulateLiveSharingReceive();
     void simulateTwoWaySharing();
     void simulateVideoTelephony();

private: // Data

    QTimer* mTimer;
    LcConfiguration* mConfig;
    LcUiEnginePrivate& mUiEngine;    
    int iSimulatorCount;

};

#endif // LCUIENGINESIMULATOR_H


// End of file
