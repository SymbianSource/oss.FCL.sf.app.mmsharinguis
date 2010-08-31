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
    setFlag(QGraphicsItem::ItemUsesExtendedStyleOption, true);
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
    QPointF initialPosition(0,0);
    
    //Workaround: widgets positioning takes time, do not paint if they are not 
    //positioned correctly.
    
    if ( this->pos() != initialPosition ) {
    
    painter->save();
    if ( mShowVideo ){

        QPainter::CompositionMode origCompositionMode = painter->compositionMode();
        painter->setCompositionMode(QPainter::CompositionMode_Source);
#if ( defined __WINSCW__ ) || ( defined __WINS__ )
        if( mLocalPlayer ){
            painter->setBrush(QColor(Qt::darkGreen));            
        }else{
            painter->setBrush(QColor(Qt::darkRed));
        }
#else
        painter->setBrush(QColor(0,0,0,0));
#endif
        painter->drawRoundedRect( option->exposedRect, 
            lcVideoWidgetCornerRounding, lcVideoWidgetCornerRounding);
        
        painter->setCompositionMode(origCompositionMode); // restore
        } else {
            painter->setBrush(QColor(0,0,0,255));
            painter->drawRoundedRect( option->exposedRect, 
                lcVideoWidgetCornerRounding, lcVideoWidgetCornerRounding);
        }
        painter->restore();
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
