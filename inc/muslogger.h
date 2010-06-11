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


#ifndef MUSLOGGER_H
#define MUSLOGGER_H

// Disabled PC_lint warning for "Undefined preprocessor variable 'UNIT_TESTING'"
/*lint -e553 */
#if ( !_DEBUG || UNIT_TESTING ) // release or unit test build; no logs

#define MUS_LOG_TDESC( aPrefix, aString )
#define MUS_LOG_TDESC8( aPrefix, aString )
#define MUS_LOG( aFmt )
#define MUS_LOG1( aFmt, aVar1 )
#define MUS_LOG2( aFmt, aVar1, aVar2 )
#define MUS_LOG3( aFmt, aVar1, aVar2, aVar3 )

#else // debug build; logs

#include <e32debug.h>
#include <flogger.h>

_LIT( KMusLogDir, "mus" );           // MultimediaSharing logging directory.
_LIT( KMusLogFile, "musdebug.txt" ); // MultimediaSharing log file name.

// File logging can be disabled by commenting above definition. 
#define MUS_FILE_LOGGING_ENABLED

#if ( defined __WINSCW__ ) || ( defined __WINS__ )
// No sense to have file logging enabled in wins
#undef MUS_FILE_LOGGING_ENABLED
#endif

#ifdef MUS_FILE_LOGGING_ENABLED
const TBool KMusFileLoggingEnabled = ETrue;
#else
const TBool KMusFileLoggingEnabled = EFalse;
#endif

#define MUS_LOG_TDESC( aPrefix, aString )\
    {\
    _LIT( KLogPrefix, aPrefix );\
    HBufC* logString = HBufC::NewLC( KLogPrefix().Length()+\
                                     aString.Length() );\
    logString->Des().Append( KLogPrefix() );\
    logString->Des().Append( aString );\
    if ( KMusFileLoggingEnabled )\
        {\
        RFileLogger::Write( KMusLogDir, KMusLogFile,\
                              EFileLoggingModeAppend,\
                              *logString );\
        }\
    RDebug::Print( *logString );\
    CleanupStack::PopAndDestroy( logString );\
    }                                     

#define MUS_LOG_TDESC8( aPrefix, aString )\
    {\
    _LIT8( KLogPrefix8, aPrefix );\
    HBufC8* logString8 = HBufC8::NewLC( KLogPrefix8().Length()+\
                                     aString.Length() );\
    logString8->Des().Append( KLogPrefix8() );\
    logString8->Des().Append( aString );\
    if ( KMusFileLoggingEnabled )\
        {\
        RFileLogger::Write( KMusLogDir, KMusLogFile,\
                              EFileLoggingModeAppend,\
                              *logString8 );\
        }\
    RDebug::RawPrint( *logString8 );\
    CleanupStack::PopAndDestroy( logString8 );\
    }    

#define MUS_LOG( aFmt )\
    {\
    _LIT( KLogMsg, aFmt );\
    if ( KMusFileLoggingEnabled )\
        {\
        RFileLogger::WriteFormat( KMusLogDir, KMusLogFile,\
                                  EFileLoggingModeAppend,\
                                  KLogMsg() );\
        }\
    RDebug::Print( KLogMsg() );\
    }

#define MUS_LOG1( aFmt, aVar1 )\
    {\
    _LIT( KLogMsg, aFmt );\
    if ( KMusFileLoggingEnabled )\
        {\
        RFileLogger::WriteFormat( KMusLogDir, KMusLogFile,\
                                  EFileLoggingModeAppend,\
                                  KLogMsg(), aVar1 );\
        }\
    RDebug::Print( KLogMsg(), aVar1 );\
    }

#define MUS_LOG2( aFmt, aVar1, aVar2 )\
    {\
    _LIT( KLogMsg, aFmt );\
    if ( KMusFileLoggingEnabled )\
        {\
        RFileLogger::WriteFormat( KMusLogDir, KMusLogFile,\
                              EFileLoggingModeAppend,\
                              KLogMsg(), aVar1, aVar2 );\
        }\
    RDebug::Print( KLogMsg(), aVar1, aVar2 );\
    }

#define MUS_LOG3( aFmt, aVar1, aVar2, aVar3 )\
    {\
    _LIT( KLogMsg, aFmt );\
    if ( KMusFileLoggingEnabled )\
        {\
        RFileLogger::WriteFormat( KMusLogDir, KMusLogFile,\
                              EFileLoggingModeAppend,\
                              KLogMsg(), aVar1, aVar2, aVar3 );\
        }\
    RDebug::Print( KLogMsg(), aVar1, aVar2, aVar3 );\
    }
 
#endif

#define LEAVE_IF_NOT( x, y ) \
    if( x != y && x != KErrNone )\
        {\
        MUS_LOG1( "mus: [      ]     Leaving with code %d", x )\
        User::Leave( x );\
        }

#endif // MUSLOGGER_H

// End of File
