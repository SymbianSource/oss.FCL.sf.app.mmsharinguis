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
#include "testrunner.h"

#if defined(Q_OS_SYMBIAN)
#include "ut_lcactivitymanager.h"
#endif

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
    
    TestRunner testRunner("LcEngine", combinedOutputFileName);
    
    UT_LcUiEngine ut_lcUiEngine;
    testRunner.runTests(ut_lcUiEngine);

#if defined(Q_OS_SYMBIAN)
    UT_LcActivityManager ut_lcActivityManager;
    testRunner.runTests(ut_lcActivityManager);
#endif
    
    testRunner.printResults();
    
    return 0;   
}



 
