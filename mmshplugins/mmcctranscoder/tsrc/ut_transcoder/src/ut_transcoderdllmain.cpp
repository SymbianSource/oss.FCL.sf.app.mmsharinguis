/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Unit test
*
*/


//  USER INCLUDES
#include "ut_mcctranscoder.h"
#include "ut_mcctranscoderimpl.h"
#include "ut_mcctranscodersessioninfo.h"

//  SYSTEM INCLUDES
#include <digia/eunit/ceunittestsuite.h>


// -----------------------------------------------------------------------------
// Test suite factory function.
// -----------------------------------------------------------------------------
//
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC( _L( "mcctranscoder" ) );

    rootSuite->AddL( UT_CMccTranscoder::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccTranscoderImpl::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMccTranscoderSessionInfo::NewLC() );
    CleanupStack::Pop();

    CleanupStack::Pop( rootSuite );

    return rootSuite;
    }
