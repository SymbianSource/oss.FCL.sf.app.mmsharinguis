/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Provides factory function for EUnit test suite.
*
*/



#include "ut_cmusindicatorapi.h"
#include "ut_cmussoundplayer.h"
#include <digia/eunit/ceunittestsuite.h>


_LIT( KRootSuiteName, "Mus Indicator" );


/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( KRootSuiteName );

    rootSuite->AddL( UT_CMusIndicatorApi::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusSoundPlayer::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop( rootSuite );

    return rootSuite;
    }
