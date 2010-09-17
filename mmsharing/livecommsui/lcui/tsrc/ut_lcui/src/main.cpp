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


#include "ut_lcviewmanager.h"
#include "ut_lcuicomponentrepository.h"
#include "ut_lcview.h"
#include "ut_lceffecthandler.h"
#include "ut_lcvideowidget.h"
#include "testrunner.h"
#include <qapplication.h>
#include <QtTest/QtTest>
#include <stdio.h>

#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[]) 
{
    printf("Running tests...\n");
            
    QApplication app(argc, argv);
    QStringList args = app.arguments();
    QString combinedOutputFileName;
    for ( int i = 0; i < args.count(); i++ ){
        QString arg = args.at(i);
        if ( arg == QString("-o") && i + 1 < args.count() ){
            i++;
            combinedOutputFileName = args.at(i);
        }
    }
    
    TestRunner testRunner("LcUi", combinedOutputFileName);
    
    UT_LcUiViewManager ut_lcViewManager;
    testRunner.runTests(ut_lcViewManager);
   
    UT_LcUiComponentRepository ut_lcUiComponentRepository;
    testRunner.runTests(ut_lcUiComponentRepository);
 
    UT_LcView ut_lcView;
    testRunner.runTests(ut_lcView);
    
    UT_LcEffectHandler ut_effectHandler;
    testRunner.runTests(ut_effectHandler);
    
    UT_LcVideoWidget ut_videoWidget;
    testRunner.runTests(ut_videoWidget);
  
    testRunner.printResults();
    
    return 0;
}



 
