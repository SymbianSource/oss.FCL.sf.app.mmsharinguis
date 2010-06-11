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


#include <QtTest/QtTest>

#include "ut_lcuiengine.h"
#include "testresultxmlparser.h"

#if defined(Q_OS_SYMBIAN)
#include "ut_lcactivitymanager.h"
#endif

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
    
    UT_LcUiEngine ut_lcUiEngine;
    QString resultFileName = "c:/ut_lcui_lcUiEngine.xml";
    QStringList args_lcUiEngine( "ut_lcuiengine");
    args_lcUiEngine << "-xml" << "-o" << resultFileName;
    QTest::qExec(&ut_lcUiEngine, args_lcUiEngine);
    parser.parseAndPrintResults(resultFileName,true); 

#if defined(Q_OS_SYMBIAN)
    UT_LcActivityManager ut_lcActivityManager;
    resultFileName = "c:/ut_lcui_lcActivityManager.xml";
    QStringList args_lcActivityManager( "ut_lcuiengine");
    args_lcActivityManager << "-xml" << "-o" << resultFileName;
    QTest::qExec(&ut_lcActivityManager, args_lcActivityManager);
    parser.parseAndPrintResults(resultFileName,true); 
#endif
    
    if (promptOnExit) {
        printf("Press any key...\n");
        getchar(); 
    }
    return 0;   
}



 
