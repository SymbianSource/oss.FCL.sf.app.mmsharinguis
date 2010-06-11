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


#include "ut_lcvideowidget.h"
#include "ut_lcuidefs.h"
#include "lcvideowidget.h"
#include <QtTest/QtTest>
#include <hbmainwindow.h>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

void UT_LcVideoWidget::initTestCase()
{
}

void UT_LcVideoWidget::cleanupTestCase()
{
}


void UT_LcVideoWidget::init()
{
     mVideoWidget = new LcVideoWidget();
     mVideoWidget2 = new LcVideoWidget(0,false);
}

void UT_LcVideoWidget::cleanup()
{
    delete mVideoWidget;
    delete mVideoWidget2;
}

void UT_LcVideoWidget::testConstructor()
{
    QVERIFY( mVideoWidget );
    QVERIFY( mVideoWidget->isShowingVideo() );
    QVERIFY( mVideoWidget2 );
    QVERIFY( !mVideoWidget2->isShowingVideo() );
}

void UT_LcVideoWidget::testSetShowVideo()
{
    QVERIFY( mVideoWidget->isShowingVideo() );
    mVideoWidget->setShowVideo(false);
    QVERIFY( !mVideoWidget->isShowingVideo() );
    mVideoWidget->setShowVideo(true);
    QVERIFY( mVideoWidget->isShowingVideo() );
}

void UT_LcVideoWidget::testPaint()
{
    QPainter painter;
    QStyleOptionGraphicsItem style;
    mVideoWidget->paint(&painter,&style,0);
    mVideoWidget2->paint(&painter,&style,0);
}

