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
#ifndef MUSTESTHELP_H_
#define MUSTESTHELP_H_


#define MUS_EUNIT_ASSERT_LEAVE( func ) { TRAPD( leaveErr, func );\
if ( leaveErr == KErrNoMemory )\
    {\
    User::Leave( KErrNoMemory );\
    }\
EUNIT_ASSERT( leaveErr != KErrNone );}

#define MUS_EUNIT_ASSERT_EQUALS( func, val ) \
{\
TInt musUnitTestRetVal = func;\
if ( musUnitTestRetVal != KErrNoMemory ){\
    EUNIT_ASSERT_EQUALS( musUnitTestRetVal, val );\
    }\
else{\
    User::Leave( KErrNoMemory );\
    }\
}

#define MUS_EUNIT_ASSERT_SPECIFIC_LEAVE( func, val ) \
{\
TRAPD( musUnitTestRetVal, func );\
if ( val != KErrNoMemory && musUnitTestRetVal == KErrNoMemory ){\
    User::Leave( KErrNoMemory );\
    }\
else{\
    EUNIT_ASSERT_EQUALS( musUnitTestRetVal, val );\
    }\
}

#define MUS_EUNIT_ASSERT_NO_LEAVE( func ) MUS_EUNIT_ASSERT_SPECIFIC_LEAVE( func, KErrNone )

#define MUS_LEAVE_HANDLING( func, oomMappedError ) {\
TRAPD( musUnitTestLeaveError, func );\
if ( musUnitTestLeaveError == KErrNoMemory || musUnitTestLeaveError == oomMappedError )\
    {\
    User::Leave( KErrNoMemory );\
    }\
}

// MUS memory leave asserting can be used to help on cases where memory leave
// which occured deep inside function, cannot be determined from function return
// values (i.e. memory leave was ignored and execution did not go as expected)
#define MUS_MEMORY_LEAVE_CHECKING_ON

#ifdef MUS_MEMORY_LEAVE_CHECKING_ON
#define MUS_CHECK_MEMORY_LEAVE( conditions ) \
if ( !(conditions) )\
    {\
    User::Leave( KErrNoMemory );\
    }

#else
#define MUS_CHECK_MEMORY_LEAVE( conditions ) \
    EUNIT_ASSERT( conditions )
#endif

#ifdef MUS_MEMORY_LEAVE_CHECKING_ON
#define MUS_CHECK_MEMORY_LEAVE_NEG( conditions ) \
if ( conditions )\
    {\
    User::Leave( KErrNoMemory );\
    }

#else
#define MUS_CHECK_MEMORY_LEAVE_NEG( conditions )
#endif

#endif /*MUSTESTHELP_H_*/
