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
* Description:  Defines a gesture that can be added to a gesture filter.
*
*/


#ifndef HB_GESTURE_H
#define HB_GESTURE_H

#include <QObject>
#include <QPointF>

class HbGesture;
class HbGesturePrivate;

class HbGesture : public QObject
{

public:

    /**
    * Direction of the gesture.
    */
    enum Direction
    {
        /* empty gesture */
        none=0,
        /** Left */
        left= 0x0000001,
        /** Right */
        right= 0x0000002,
        /** Up */
        up= 0x0000004,
        /** Down */
        down= 0x0000008,
        /** Pan */
        pan= 0x0000010,
        /** Longpress */
        longpress= 0x0000020
    };

    /*
    * Defines that the default minimum pixel distance should be used for the gesture.
    * The actual amount of pixels might vary based on the screen resolution.
    */
    static const int HbGestureDefaultMinDistance = -1;

public:

    explicit HbGesture( Direction direction,
    int minDistance = HbGestureDefaultMinDistance,
    QObject *parent = 0 );

    virtual ~HbGesture();

signals:

    void longPress( QPointF delta );

};

#endif // HB_GESTURE_H
