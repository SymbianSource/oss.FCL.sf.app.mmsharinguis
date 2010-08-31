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
#include "testresultxmlparser.h"
#include <qapplication.h>
#include <QtTest/QtTest>
#include <stdio.h>

#include <QFile>
#include <QTextStream>

#define UT_ARGS( args, file )\
    QStringList args( "ut_lcui");\
    args << "-silent" << "-xml" << "-o" << file

int main(int argc, char *argv[]) 
{
    bool promptOnExit(true);
    for (int i=0; i<argc; i++) {
        if (QString(argv[i]) == "-noprompt")
            promptOnExit = false;
    }
    printf("Running tests...\n");
    QApplication app(argc, argv);
    TestResultXmlParser parser;
    
    UT_LcUiViewManager ut_lcViewManager;
    QString resultFileName = "c:/ut_lcui_LcViewManager.xml";
    UT_ARGS( args_lcViewManager, resultFileName );
    QTest::qExec( &ut_lcViewManager, args_lcViewManager );
    parser.parseAndPrintResults(resultFileName);
    
    UT_LcUiComponentRepository ut_lcUiComponentRepository;
    resultFileName = "c:/ut_lcui_LcUiComponentRepository.xml";
    UT_ARGS( args_lcUiComponentRepository, resultFileName );
    QTest::qExec( &ut_lcUiComponentRepository, args_lcUiComponentRepository );
    parser.parseAndPrintResults(resultFileName);
 
    UT_LcView ut_lcView;
    resultFileName = "c:/ut_lcui_LcView.xml";
    UT_ARGS( args_lcView, resultFileName ); 
    QTest::qExec( &ut_lcView, args_lcView );
    parser.parseAndPrintResults(resultFileName);
    
    UT_LcEffectHandler ut_effectHandler;
    resultFileName = "c:/ut_lcui_LcEffectHandler.xml";
    UT_ARGS( args_lcEffectHandler, resultFileName ); 
    QTest::qExec( &ut_effectHandler, args_lcEffectHandler );
    parser.parseAndPrintResults(resultFileName);
    
    UT_LcVideoWidget ut_videoWidget;
    resultFileName = "c:/ut_lcui_LcVideoWidget.xml";
    UT_ARGS( args_lcVideoWidget, resultFileName ); 
    QTest::qExec( &ut_videoWidget, args_lcVideoWidget );
    parser.parseAndPrintResults(resultFileName);
  
    if (promptOnExit) {
        printf("Press any key...\n");
        getchar(); 
    }
    return 0;
}



 
