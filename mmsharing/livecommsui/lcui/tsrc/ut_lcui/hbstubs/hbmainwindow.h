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

#ifndef HBMAINWINDOW_H
#define HBMAINWINDOW_H

#include <QGraphicsView>
#include <hbglobal.h>
#include <hbnamespace.h>

class QGraphicsWidget;
class HbAction;
class HbView;


class HbMainWindow : public QGraphicsView
{
public:
    explicit HbMainWindow(QWidget *parent = 0, Hb::WindowFlags windowFlags = Hb::WindowFlagNone);
    ~HbMainWindow();

    HbView *addView(QGraphicsWidget *widget = 0);
    QGraphicsWidget *removeView(int index);
    void removeView(QGraphicsWidget *widget);
    HbView *currentView() const;
    void setCurrentView(HbView *view, bool animate = false);
    int viewCount() const;
    void setOrientation(Qt::Orientation orientation, bool animate = true);
    Qt::Orientation orientation() const;
    void show(){};
    QList<HbView *> views() const;
    QRectF sceneRect() const;
    
private: // Data added for the unit testing stub
	HbView* mCurrentView; // Not owned
	int mViewCount;
	Qt::Orientation mOrientation;
  QList<HbView*> mTestViews;
};

#endif // HBMAINWINDOW_H
