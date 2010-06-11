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

//USER
#include "lcvideowidget.h"

//SYSTEM
#include <QPainter>

const qreal lcVideoWidgetCornerRounding = 10.0;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LcVideoWidget::LcVideoWidget(QGraphicsItem *parent, bool showVideo) :
    HbTransparentWindow(parent),
    mShowVideo(showVideo)
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
LcVideoWidget::~LcVideoWidget()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcVideoWidget::paint(
        QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    
    if ( mShowVideo ){

        QPainter::CompositionMode origCompositionMode = painter->compositionMode();
        painter->setCompositionMode(QPainter::CompositionMode_Source);
        painter->setBrush(QColor(0,0,0,0));
        painter->drawRoundedRect( option->exposedRect, 
            lcVideoWidgetCornerRounding, lcVideoWidgetCornerRounding);
        
        painter->setCompositionMode(origCompositionMode); // restore
    } else {
        painter->setBrush(QColor(0,0,0,255));
        painter->drawRoundedRect( option->exposedRect, 
            lcVideoWidgetCornerRounding, lcVideoWidgetCornerRounding);
    }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
void LcVideoWidget::setShowVideo(bool showVideo)
{
    mShowVideo = showVideo;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
//
bool LcVideoWidget::isShowingVideo()
{
    return mShowVideo;
}

// End of file
