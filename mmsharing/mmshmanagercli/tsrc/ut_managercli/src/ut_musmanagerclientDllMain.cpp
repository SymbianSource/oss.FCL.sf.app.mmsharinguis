/*
* Copyright (c) 2004-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provides main factory function for unit test suite.
*
*/



#include "ut_cmusmanagerimpl.h"
#include "ut_cmusmanager.h"
#include <digia/eunit/ceunittestsuite.h>


// Name of test suite
_LIT( KTestSuiteName, "MusMangerClient TestSuite" );


// ======== LOCAL FUNCTIONS ========


/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( KTestSuiteName );

    rootSuite->AddL( UT_CMusManagerImpl::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusManager::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop( rootSuite );
    return rootSuite;
    }
