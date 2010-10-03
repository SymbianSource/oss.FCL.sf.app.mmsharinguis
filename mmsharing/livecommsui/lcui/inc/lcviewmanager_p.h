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
#ifndef LCVIEWMANAGER_P_H
#define LCVIEWMANAGER_P_H

#include <QObject>

class HbMainWindow;
class LcUiEngine;
class LcUiComponentRepository;
class LcView;
class HbAction;

#include <hbmainwindow.h>

class LcMainWindow : public HbMainWindow
    {
    Q_OBJECT
    
public:
    
    LcMainWindow();
    virtual ~LcMainWindow();

private slots:

    void handleBackground();
    void handleForeground();


signals:

    void appFocusGained();
    void appFocusLost();
  
    
private:
    friend class UT_LcUiViewManager;    
    };

class LcViewManagerPrivate : public QObject
{
    Q_OBJECT
    friend class UT_LcUiViewManager;

public:
  
    explicit LcViewManagerPrivate(const QString &engineName,
                                  const QString &applicationDisplayName);
    ~LcViewManagerPrivate();

    //this will close engine session asynchronously
    //when session is closed application will be also closed
    void terminateSession();
        
public slots:
    void changeView();
    //for layout switch
    void prepareOrientationChange();
    
private slots:
    void closeApp();
    void activateCurrentView(bool startup);
    
private:  

    void startEngine();
    void setCurrentView(bool startup=false);
    LcView* currentView();
    
private: //data 
    
    LcMainWindow mMainWindow;  
    LcUiEngine* mEngine;
    LcUiComponentRepository* mRepository;
    
private:
    friend class UT_LcUiViewManager;
};
                              
#endif // LCVIEWMANAGER_P_H
