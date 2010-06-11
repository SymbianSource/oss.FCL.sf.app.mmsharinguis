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
* Description:  Provides factory method for MuS Manager Server test suite.
*
*/


#include "ut_cmusmanagerservercore.h"
#include "ut_cmusmanagerserverclosetimer.h"
#include "ut_cmusapplicationmanager.h"
#include "ut_cmusavailabilitypluginmanager.h"
#include "ut_cmusmanagerserversession.h"

#include <digia/eunit/ceunittestsuite.h>


// ======== GLOBAL FUNCTIONS ========


// ---------------------------------------------------------------------------
// Constructs and returns EUnit Test suite.
// ---------------------------------------------------------------------------
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( _L( "Mush ManagerServer" ) );

    rootSuite->AddL( UT_CMusApplicationManager::NewL() );
    rootSuite->AddL( UT_CMusAvailabilityPluginManager::NewL() );
    rootSuite->AddL( UT_CMusManagerServerCore::NewL() );
    rootSuite->AddL( UT_CMusManagerServerCloseTimer::NewL() );
    rootSuite->AddL( UT_CMusManagerServerSession::NewL() );

    CleanupStack::Pop( rootSuite );
    return rootSuite;
    }
