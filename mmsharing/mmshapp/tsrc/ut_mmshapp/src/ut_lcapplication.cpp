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
#include "lcapplication.h"
#include "ut_lcapplication.h"

#include <QtTest/QtTest>


void UT_LcApplication::initTestCase()
{
}

void UT_LcApplication::cleanupTestCase()
{
}


void UT_LcApplication::init()
{
    mArgc = 0;
    mLcApplication = new LcHbApplication( mArgc, 0 );
}

void UT_LcApplication::cleanup()
{
    delete mLcApplication;
}

void UT_LcApplication::testConstructorDestructor()
    {
    QVERIFY( mLcApplication );
    QVERIFY( LcHbApplication::self == mLcApplication );
    
    delete mLcApplication;
    mLcApplication = 0;
    QVERIFY( !LcHbApplication::self );
    }

void UT_LcApplication::testQuit()
{
    QSignalSpy spy( mLcApplication, SIGNAL(handleQuit()) );
    LcHbApplication::quit();
    QCOMPARE( spy.count(), 1 );
    
    mLcApplication->self = 0;
    spy.clear();
    LcHbApplication::quit();
    QCOMPARE( spy.count(), 0 );
}
