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


#ifndef __UT_CMUSAVASIPHEADERUTIL_H__
#define __UT_CMUSAVASIPHEADERUTIL_H__

//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>

//  INTERNAL INCLUDES
#include <e32def.h>
#include <stringpool.h>

//  FORWARD DECLARATIONS 
class CSIPHeaderBase;     
//  CLASS DEFINITION

// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files // include this file. Directive can be removed when // EUnit is fixed.  
#pragma warn_illtokenpasting off
/**
 *
 * EUnitWizard generated test class. 
 *
 */
class UT_CMusAvaSipheaderUtil
     : public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusAvaSipheaderUtil* NewL();
        static UT_CMusAvaSipheaderUtil* NewLC();
        /**
         * Destructor
         */
        ~UT_CMusAvaSipheaderUtil();

    private:    // Constructors and destructors

        UT_CMusAvaSipheaderUtil();
        void ConstructL();

	
    private:    // New methods

        void SetupL();

        void Teardown();

        void UT_AddAcceptContactHeaderLL();   

        void UT_AddAcceptEncodingHeaderLL();
		
        void UT_AddAcceptLanguageHeaderLL();
         		
        void UT_AddPreferredIdentityHeaderLL();
          
        void UT_AddAcceptSdpHeaderLL();
        
        void UT_AddSdpLL();

        void UT_LocalHostLL();

    private :
        RPointerArray<CSIPHeaderBase> iRequestHeaders;
        

	EUNIT_DECLARE_TEST_TABLE; 
    };

#endif      //  __UT_CMUSAVASIPHEADERUTIL_H__

// End of file
