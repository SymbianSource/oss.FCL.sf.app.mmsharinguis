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


#ifndef __UT_CMUSAVACAPABILITY_H__
#define __UT_CMUSAVACAPABILITY_H__

#ifdef __MWERKS__
#pragma warn_emptydecl off
#pragma warn_illtokenpasting off
#endif

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

#include "mussettingskeys.h"

//  INTERNAL INCLUDES


//  FORWARD DECLARATIONS
class CMusAvaCapability;
class CMusAvaCapabilityExchange;
class CSIP;
class CSIPConnection;
class CSIPProfile;
class CMusAvaOptionHandler;
class CMusAvaObserverImp;
class CMusAvaSettingsImp;
class CMusAvaAvailabilityObserverImp;
class CMusAvaCapabilityQueryObserverImp;	

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 * EUnitWizard generated test class. 
 *
 */
NONSHARABLE_CLASS( UT_CMusAvaCapability )
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaCapability* NewL();
        static UT_CMusAvaCapability* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaCapability();

    private:    // Constructors and destructors

        UT_CMusAvaCapability();
        void ConstructL();

    public:     // From observer interface

        

    private:    // New methods

        void SetupL();

        void Teardown();
         
        void Setup1L();

        void Teardown1();            
        
        void UT_CMusAvaCapability_NewLL();

        void UT_CMusAvaCapability_DoPopulateResponseLL();
        
        void UT_CMusAvaCapability_SupportsL();
        
        void UT_CMusAvaCapability_ResolveCodecsLL();
        
        void UT_CMusAvaCapability_AttributeL();
        
        void UT_CMusAvaCapability_PopulateResponseL_OpSpecificL();

    private:    // Data

        EUNIT_DECLARE_TEST_TABLE; 

    CMusAvaCapabilityExchange* iExchange;
    
    
    CSIP* iSIP;
    CSIPConnection* iSIPConnection;
    CSIPProfile* iProfile;

    CMusAvaOptionHandler* iOptionHandler;
    CMusAvaAvailabilityObserverImp* iAvailabilityObserver;
    CMusAvaSettingsImp* iConcreteSettings;
    CMusAvaCapabilityQueryObserverImp* iCapabQueryObserver;

    CMusAvaCapability* iCapability;
    
    MusSettingsKeys::TOperatorVariant iOpVariantSetting;

    };

#endif      //  __UT_CMUSAVACAPABILITY_H__

// End of file
