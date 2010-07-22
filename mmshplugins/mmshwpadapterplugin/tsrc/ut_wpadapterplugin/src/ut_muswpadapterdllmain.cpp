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
#include "ut_muswpadapter.h"
#include "ut_muswpitem.h"

//  SYSTEM INCLUDES
#include <digia/eunit/ceunittestsuite.h>


// -----------------------------------------------------------------------------
// Test suite factory function.
// -----------------------------------------------------------------------------
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( _L( "Mush WPAdapterplugin" ) );

    rootSuite->AddL( UT_CMusWpAdapter::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusWpItem::NewLC() );
    CleanupStack::Pop();
    
    CleanupStack::Pop( rootSuite );

    return rootSuite;
    }
