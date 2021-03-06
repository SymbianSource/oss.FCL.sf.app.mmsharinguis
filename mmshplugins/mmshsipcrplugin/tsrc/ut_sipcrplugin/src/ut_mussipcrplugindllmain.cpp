/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuite.h>

#include "mussipcrplugintest.h"

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Mush SIPCRPlugin"));

    rootSuite->AddL( CMusSipCrPluginTest::NewL() );
    
    CleanupStack::Pop( rootSuite );
    return rootSuite;
    }



//  END OF FILE






