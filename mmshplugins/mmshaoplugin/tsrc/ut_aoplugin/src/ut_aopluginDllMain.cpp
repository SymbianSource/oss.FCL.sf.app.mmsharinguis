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

#include "ut_musaoplugin.h"
#include "ut_muspropertymonitor.h"
#include "ut_muspttcallmonitor.h"
#include "ut_muscallmonitor.h"
#include "ut_muscallstatusmonitor.h"
#include "ut_muscalleventmonitor.h"
#include "ut_muscallconferenceeventmonitor.h"
#include "ut_muscallconferencemonitor.h"
#include "ut_muscallmonitorbase.h"  
#include "ut_musresourcefinderutil.h"
#include "ut_mustsypropertymonitor.h"
#include "ut_musclirmonitor.h"

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Mush AOPlugin"));

    rootSuite->AddL( UT_CMusAoPlugin::NewLC() );
    CleanupStack::Pop();   
    
    rootSuite->AddL( UT_CMusPropertyMonitor::NewLC() );
    CleanupStack::Pop();   
    
    rootSuite->AddL( UT_CMusPTTCallMonitor::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusCallConferenceMonitor::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusCallStatusMonitor::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusCallEventMonitor::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusCallMonitor::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusTsyPropertyMonitor::NewLC() );
    CleanupStack::Pop();  

    rootSuite->AddL( UT_MusResourceFinderUtil::NewLC() );
    CleanupStack::Pop();  

    rootSuite->AddL( UT_CMusCallConferenceEventMonitor::NewLC() );
    CleanupStack::Pop();  

    rootSuite->AddL( UT_CMusCallMonitorBase::NewLC() );
    CleanupStack::Pop();  
   
    rootSuite->AddL( UT_CMusClirMonitor::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop( rootSuite );
    return rootSuite;
    }



//  END OF FILE




