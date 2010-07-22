/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  ?Description
*
*/


#ifndef LCLOGGER_H
#define LCLOGGER_H

#include <QDebug>


// Disabled PC_lint warning for "Undefined preprocessor variable 'UNIT_TESTING'"
//lint -e553 
#if ( !defined(_DEBUG) || defined(QT_NO_DEBUG_OUTPUT) ) // release or unit test build; no logs

#define LC_QDEBUG( a )
#define LC_QDEBUG_2( a, b)
#define LC_QDEBUG_3( a, b, c)
#define LC_QDEBUG_4( a, b, c, d)
#define LC_QWARNING( a )
#define LC_QWARNING_2( a, b )
#define LC_QCRITICAL( a )
#define LC_QCRITICAL_2( a, b )
#define LC_QCRITICAL_3( a, b, c)

#else // debug build; logs


const char lcDebugPrefix[]    = "Debug: ";
const char lcWarningPrefix[]  = "Warning: ";
const char lcCriticalPrefix[] = "Critical: ";
const char lcFatalPrefix[]    = "Fatal: ";

#include <e32debug.h>    // RDebug
#include <QFile>
#include <QTextStream>
#include <qapplication.h>
#include <stdio.h>
#include <stdlib.h>

const char lcLogFile[] = "C:/logs/mus/musdebug.txt";// MultimediaSharing logging file.


#define LC_QDEBUG( a )\
    qDebug() << a;

#define LC_QDEBUG_2( a, b)\
    qDebug() << a << b;

#define LC_QDEBUG_3( a, b, c)\
    qDebug() << a << b << c;

#define LC_QDEBUG_4( a, b, c, d)\
    qDebug() << a << b << c << d;

#define LC_QWARNING( a )\
    qWarning() << a;

#define LC_QWARNING_2( a, b )\
    qWarning() << a << b;      

#define LC_QCRITICAL( a )\
    qCritical() << a;  

#define LC_QCRITICAL_2( a, b )\
    qCritical() << a << b;  
    
#define LC_QCRITICAL_3( a, b, c)\
    qCritical() << a << b << c;


// File logging can be disabled by commenting above definition. 
#define LCUI_FILE_LOGGING_ENABLED

#if ( defined __WINSCW__ ) || ( defined __WINS__ )
// No sense to have file logging enabled in wins
#undef LCUI_FILE_LOGGING_ENABLED
#endif

#ifdef LCUI_FILE_LOGGING_ENABLED
const bool lcFileLoggingEnabled = ETrue;
#else
const bool lcFileLoggingEnabled = EFalse;
#endif


#endif

#endif // LCLOGGER_H

// End of File
