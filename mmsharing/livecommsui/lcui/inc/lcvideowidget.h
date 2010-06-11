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
#ifndef LCVIDEOWIDGET_H
#define LCVIDEOWIDGET_H

#include <hbtransparentwindow.h>


class LcVideoWidget : public HbTransparentWindow
{
    Q_OBJECT

public:

    explicit LcVideoWidget(QGraphicsItem *parent = 0, bool showVideo = true);
    ~LcVideoWidget();
    void setShowVideo(bool showVideo);
    bool isShowingVideo();
    void setLocalPlayer(bool local){mLocalPlayer = local ;};

public: // From HbTransparentWindow
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
               QWidget *widget = 0);
private:
    bool mShowVideo;
    bool mLocalPlayer;
};

#endif // LCVIDEOWIDGET_H
