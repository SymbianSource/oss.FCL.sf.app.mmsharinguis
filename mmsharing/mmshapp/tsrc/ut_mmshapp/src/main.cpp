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

#include "ut_lcapplication.h"
#include "testresultxmlparser.h"


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
    
    UT_LcApplication ut_lcApplication;
    QString resultFileName = "c:/ut_lcapplication.xml";
    QStringList args_lcApplication( "ut_lcapplication");
    args_lcApplication << "-xml" << "-o" << resultFileName;
    QTest::qExec(&ut_lcApplication, args_lcApplication);
    parser.parseAndPrintResults(resultFileName,true); 

    if (promptOnExit) {
        printf("Press any key...\n");
        getchar(); 
    }
    return 0;   
}
