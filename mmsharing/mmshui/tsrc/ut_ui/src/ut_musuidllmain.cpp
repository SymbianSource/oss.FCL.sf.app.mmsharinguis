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

//  USER INCLUDES
#include "ut_musuiclipsharingcontroller.h"
#include "ut_musuieventcontroller.h"
#include "ut_musuilivesharingcontroller.h"
#include "ut_musuipropertywatch.h"
#include "ut_musuireceivecontroller.h"
#include "ut_musuistartcontroller.h"

//  SYSTEM INCLUDES
#include <digia/eunit/ceunittestsuite.h>


// -----------------------------------------------------------------------------
// Test suite factory function.
// -----------------------------------------------------------------------------
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {

    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( _L( "Main suite" ) );

    //rootSuite->AddL( UT_MusEngMceUtils::NewLC() );
    //CleanupStack::Pop();

    rootSuite->AddL( UT_CMusUiClipSharingController::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusUiEventController::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusUiLiveSharingController::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusUiPropertyWatch::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusUiReceiveController::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusUiStartController::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop( rootSuite );

    return rootSuite;
    }
