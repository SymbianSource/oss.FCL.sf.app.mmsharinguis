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
#include "ut_muscallmonitor.h"
#include "ut_musvoipcallmonitor.h"
#include "ut_muscallstatusmonitor.h"
#include "ut_muscalleventmonitor.h"
#include "ut_muscallconferenceeventmonitor.h"
#include "ut_muscallconferencemonitor.h"
#include "ut_muscallmonitorbase.h"  
#include "ut_musresourcefinderutil.h"
#include "ut_mustsypropertymonitor.h"
#include "ut_musfactorysettings.h"

//  SYSTEM INCLUDES
#include <digia/eunit/ceunittestsuite.h>


// -----------------------------------------------------------------------------
// Test suite factory function.
// -----------------------------------------------------------------------------
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( _L( "Mush Common" ) );

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
    
    rootSuite->AddL( UT_CMusVoipCallMonitor::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusFactorySettings::NewLC() );
    CleanupStack::Pop();
        
    CleanupStack::Pop( rootSuite );

    return rootSuite;
    }
