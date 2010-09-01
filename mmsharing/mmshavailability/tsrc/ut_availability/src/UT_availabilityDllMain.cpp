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


//  CLASS HEADER
#include "UT_CMusAvaDefaultImp.h"
#include "UT_CMusAvaContactAvailability.h"
#include "UT_CMusAvaRegisterAvailability.h"
#include "UT_CMusAvaInviteResponder.h"
#include "UT_CMusAvaSip.h"
#include "UT_CMusAvaSharedObject.h"
#include "UT_CMusAvaConnectionAvailability.h"
#include "UT_CMusAvaConnectionMonitor.h"
#include "UT_CMusAvaNetworkRegistrationStatus.h"
#include "UT_CMusAvaNetworkModeStatus.h"
#include "UT_CMusAvaNetworkAvailability.h"
#include "UT_availability.h"
#include "UT_CMusAvaInviteHandler.h"
#include "UT_CMusAvaDefaultImp.h"
#include "UT_CMusAvaSettingsImp.h"
#include "UT_CMusAvaOptionHandler.h"
#include "UT_CMusAvaCapabilityExchange.h"
#include "UT_CMusAvaCapability.h"
#include "UT_CMusAvaTerminal.h"
#include "UT_CMusAvaCapabilitySipAgent.h"
#include "UT_CMusAvaDefaultOptionHandler.h"
#include "UT_CMusAvaCapabilityQuery.h"
#include "UT_CMusAvaSipprofileAvailability.h"
#include "UT_CMusAvaSettingAvailability.h"
#include "UT_CMusAvaSipObserver.h"
#include "UT_CMusAvaSipConnectionObserver.h"
#include "UT_CMusAvaSipProfileRegistryObserver.h"
#include "UT_CMusAvaSipheaderUtil.h"
#include "UT_CMusAvaClientResolverUtil.h"


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuite.h>

/**
 * Test suite factory function.
 */
EXPORT_C MEUnitTest* CreateTestSuiteL()
    {
    CEUnitTestSuite* rootSuite = CEUnitTestSuite::NewLC(_L("Mush Availability"));
    
	rootSuite->AddL( UT_CMusAvaOptionHandler::NewLC() );  
    CleanupStack::Pop();	
    	
    rootSuite->AddL( UT_CMusAvaSharedObject::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusAvaContactAvailability::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusAvaRegisterAvailability::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusAvaSip::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusAvaSipObserver::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusAvaSipConnectionObserver::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusAvaInviteResponder::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusAvaConnectionAvailability::NewLC() ); 
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusAvaConnectionMonitor::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusAvaNetworkRegistrationStatus::NewLC() );  
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusAvaNetworkModeStatus::NewLC() );  
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusAvaNetworkAvailability::NewLC() );  
    CleanupStack::Pop();    

	rootSuite->AddL( UT_CMusAvaInviteHandler::NewLC() );  
    CleanupStack::Pop();	
    	
    rootSuite->AddL( UT_CMusAvaDefaultImp::NewLC() );  
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusAvaSettingsImp::NewLC() );  
    CleanupStack::Pop();
    
	rootSuite->AddL( UT_CMusAvaCapabilityExchange::NewLC() );  
    CleanupStack::Pop();	
    	
	rootSuite->AddL( UT_CMusAvaCapability::NewLC() );  
    CleanupStack::Pop();	
    	
	rootSuite->AddL( UT_CMusAvaTerminal::NewLC() );  
    CleanupStack::Pop();	
    	
	rootSuite->AddL( UT_CMusAvaCapabilitySipAgent::NewLC() );  
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusAvaDefaultOptionHandler::NewLC() );  
    CleanupStack::Pop();
    		
    rootSuite->AddL( UT_CMusAvaCapabilityQuery::NewLC() );  
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusAvaSipprofileAvailability::NewLC() );  
    CleanupStack::Pop();		
    	
    rootSuite->AddL( UT_CMusAvaSipProfileRegistryObserver::NewLC() );  
    CleanupStack::Pop();		

    rootSuite->AddL( UT_CMusAvaSettingAvailability::NewLC() );
    CleanupStack::Pop();

    rootSuite->AddL( UT_CMusAvaSipheaderUtil::NewLC() );
    CleanupStack::Pop();
    
    rootSuite->AddL( UT_CMusAvaClientResolverUtil::NewLC() );
    CleanupStack::Pop();
    	
    CleanupStack::Pop(); // rootSuite

    return rootSuite;
    }



//  END OF FILE






