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
#ifndef HBVIEW_H
#define HBVIEW_H

#include <hbwidget.h>
#include <hbglobal.h>
#include <hbnamespace.h>

class HbMenu;
class HbToolBar;
class HbAction;

class HbView : public HbWidget
{
public:
    explicit HbView(QGraphicsItem *parent = 0);
    virtual ~HbView();

    HbMenu *menu() const;
    HbToolBar *toolBar() const;
    
    void showItems(Hb::SceneItems items);
    void hideItems(Hb::SceneItems items);
    bool isItemVisible(Hb::SceneItem item)  const;

    void setTitleBarVisible(bool visible);
    bool isTitleBarVisible();
    
    void setContentFullScreen(bool enable = true);
    HbAction *navigationAction() const;
    void setNavigationAction(HbAction *action);

signals:
     void contentFullScreenChanged();
     
public slots:
    void setTitle(const QString &title);
    
private: // Data added for the unit testing stub
    HbMenu* mMenu;
    HbToolBar* mToolBar;
    bool mDockWidgetVisible;
    bool mTitlebarVisible;
};

#endif // HBVIEW_H
