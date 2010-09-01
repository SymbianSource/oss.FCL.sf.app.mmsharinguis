/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Main plugin class
*
*/


#ifndef MUSAIWPROVIDERTEST_H
#define MUSAIWPROVIDERTEST_H


#include <digia/eunit/ceunittestsuiteclass.h>
#include <aiwcommon.h>
#include <eikmobs.h>

class CMusAiwProvider;

/**
 * Main plugin class
 *
 * @lib ut_musAiwProvider.dll
 */
class UT_CMusAiwProvider: public CEUnitTestSuiteClass, 
                          public MEikMenuObserver,
                          public MAiwNotifyCallback
    {
    public:

        /**
         * Symbian two-phase constructor.
         * @return New UT_CMusAiwProvider instance
         */
        static UT_CMusAiwProvider* NewL();

        static UT_CMusAiwProvider* NewLC();
        
        /**
         * C++ destructor.
         */
        ~UT_CMusAiwProvider();


    private: // constructors

        /**
         * C++ constructor.
         */
        UT_CMusAiwProvider();

        /**
         * Symbian second-phase constructor.
         */
        void ConstructL();


    public: // Test case setup and teardown

        void SetupL();

        void Teardown();


    public: // from MEikMenuObserver

        void SetEmphasis( CCoeControl* aMenuControl,TBool aEmphasis );

        void ProcessCommandL( TInt aCommandId );


    public: // from MAiwNotifyCallback

        TInt HandleNotifyL( TInt aCmdId,
                            TInt aEventId,
                            CAiwGenericParamList& aEventParamList,
                            const CAiwGenericParamList& aInParamList);


    public: // Tester functions

        void UT_CMusAiwProvider_InitialiseLL();
        void UT_CMusAiwProvider_HandleServiceCmdLL();
        void UT_CMusAiwProvider_InitializeMenuPaneLL();
        void UT_CMusAiwProvider_InitializeMenuPaneL2L();
        void UT_CMusAiwProvider_InitializeMenuPaneL3L();
        void UT_CMusAiwProvider_InitializeMenuPaneL4L();
        void UT_CMusAiwProvider_InitializeMenuPaneL5L();
        void UT_CMusAiwProvider_InitializeMenuPaneL6L();
        void UT_CMusAiwProvider_InitializeMenuPaneL7L();
        
		
        void UT_CMusAiwProvider_HandleMenuCmdLL();
        void UT_CMusAiwProvider_ApplicationRunningLL();
        void UT_CMusAiwProvider_ShowErrorNoteLL();
        void UT_CMusAiwProvider_ErrorNoteTextLCL();
        void UT_CMusAiwProvider_ErrorNoteText8LL();
        void UT_ImplementationGroupProxyL();    


    private: // data

        CMusAiwProvider* iProvider;
        
        EUNIT_DECLARE_TEST_TABLE; 

    };

#endif // MUSAIWPROVIDERTEST_H
