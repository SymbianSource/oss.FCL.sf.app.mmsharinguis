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
#include "ut_musengsession.h"
#include "ut_musengmcesession.h"
#include "ut_musengoutsession.h"
#include "ut_musenglivesession.h"
#include "ut_musengclipsession.h"
#include "ut_musengreceivesession.h"
#include "ut_musengsessiondurationtimer.h"
#include "ut_musengmceutils.h"
#include "ut_musengtelephoneutils.h"
#include "ut_musenguriparser.h"
#include "ut_musengsipprofilehandler.h"
#include "ut_musengorientationhandler.h"


//  SYSTEM INCLUDES
#include <digia/eunit/ceunittestsuite.h>


// -----------------------------------------------------------------------------
// Test suite factory function.
// -----------------------------------------------------------------------------
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( _L( "Mush Engine" ) );

    rootSuite->AddL( UT_CMusEngSipProfileHandler::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_MusEngMceUtils::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusEngSessionDurationTimer::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusEngSession::NewLC() );
    CleanupStack::Pop();  
    
    rootSuite->AddL( UT_CMusEngMceSession::NewLC() );
    CleanupStack::Pop();  
    
    rootSuite->AddL( UT_CMusEngOutSession::NewLC() );
    CleanupStack::Pop();  
    
    rootSuite->AddL( UT_CMusEngLiveSession::NewLC() );
    CleanupStack::Pop();  
    
    rootSuite->AddL( UT_CMusEngReceiveSession::NewLC() );
    CleanupStack::Pop();  
    
    rootSuite->AddL( UT_CMusEngClipSession::NewLC() );
    CleanupStack::Pop();      
    
    rootSuite->AddL( UT_CMusEngTelephoneUtils::NewLC() );
    CleanupStack::Pop();      
    
    rootSuite->AddL( UT_TMusEngUriParser::NewLC() );
    CleanupStack::Pop();     
    
    rootSuite->AddL( UT_CMusEngOrientationHandler::NewLC() );
    CleanupStack::Pop();     

    CleanupStack::Pop( rootSuite );

    return rootSuite;
    }
