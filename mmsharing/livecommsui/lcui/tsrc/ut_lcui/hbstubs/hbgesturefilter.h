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
* Description:  Gesture filter for implementing touch gesture actions.
*
*/


#ifndef HB_GESTURE_FILTER_H
#define HB_GESTURE_FILTER_H

#include <hbwidget.h>

#include <QObject>
#include <qnamespace.h>
#include <QGraphicsItem>

class HbGesture;
class HbGestureFilterPrivate;
class HbGestureSceneFilterPrivate;
class HbLongPressVisualizer;


class HbGestureSceneFilter : public HbWidget
{
public:
    explicit HbGestureSceneFilter(Qt::MouseButton button = Qt::LeftButton, QGraphicsItem *parent = 0 );
    ~HbGestureSceneFilter();
    void addGesture( HbGesture *gesture );
 
 public:
 
 // Stub stuff 
    QList<HbGesture*> gestures;
};

#endif // HB_GESTURE_FILTER_H

