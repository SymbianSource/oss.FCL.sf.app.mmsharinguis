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



#ifndef __UT_CMCCTRANSCODER_H__
#define __UT_CMCCTRANSCODER_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>
#include <mmcctranscoder.h>
#include <mmccevents.h>

// Forward declaration


// Next row is to disable warning emerging from EUnit code.
// Placement is due to a fact that most test case files
// include this file. Directive can be removed when 
// EUnit is fixed.  
#pragma warn_illtokenpasting off

//  CLASS DEFINITION
/**
 * Tester class for CMusCallMonitorBase. 
 * 
 */
NONSHARABLE_CLASS( UT_CMccTranscoder ): public CEUnitTestSuiteClass,
                                        public MMccTranscoderObserver

    {
    public:
        //Two phase construction
        static UT_CMccTranscoder* NewL();
        static UT_CMccTranscoder* NewLC();
        // Destructor                  
        ~UT_CMccTranscoder();

    private: // Constructor
        UT_CMccTranscoder();
        void ConstructL();
        
    private: // from MMccTranscoderObserver

        void MccTranscoderEventReceived( TMccEvent& aEvent );

        
    private: // Test case setup and teardown
        void SetupL();
        void Teardown();

        
    private: // Test methdods
        
        void UT_CMccTranscoder_RegisterEventObserverL();
        void UT_CMccTranscoder_UnregisterEventObserverL();
        void UT_CMccTranscoder_TranscodeFileLL();
        void UT_CMccTranscoder_CancelTranscodeFileLL();
    
    private:    // Data        
        CMccTranscoder* iMccTranscoder;
        TMccEvent iMccEvent;

        TMccTranscodeFileMsg iData;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMCCTRANSCODER_H__

// End of file
