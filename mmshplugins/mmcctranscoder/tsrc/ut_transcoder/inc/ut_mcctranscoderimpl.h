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



#ifndef __UT_CMCCTRANSCODERIMPL_H__
#define __UT_CMCCTRANSCODERIMPL_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>
#include <mmccevents.h>
#include <mmcctranscoder.h>

// Forward declaration
class CMccTranscoderImpl;
class CVedMovie;


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

NONSHARABLE_CLASS( UT_CMccTranscoderImpl ): public CEUnitTestSuiteClass,
                                            public MMccTranscoderObserver
    {
    public:
        //Two phase construction
        static UT_CMccTranscoderImpl* NewL();
        static UT_CMccTranscoderImpl* NewLC();
        // Destructor                  
        ~UT_CMccTranscoderImpl();

    private: // Constructor
        UT_CMccTranscoderImpl();
        void ConstructL();

    private: // Test case setup and teardown
        void SetupL();
        void Setup2L();
        void Teardown();

    private: // from MMccTranscoderObserver

        void MccTranscoderEventReceived( TMccEvent& aEvent );

    private: // helpers
        void Reset( TInt aError = KErrNone );
        
    private: // Test methdods

         void UT_CMccTranscoderImpl_RegisterEventObserverL();
         void UT_CMccTranscoderImpl_UnregisterEventObserverL();
         void UT_CMccTranscoderImpl_TranscodeFileLL();
         void UT_CMccTranscoderImpl_CancelTranscodeFileLL();
         void UT_CMccTranscoderImpl_NotifyVideoClipAddedL();
         void UT_CMccTranscoderImpl_NotifyVideoClipAdded2L();
         void UT_CMccTranscoderImpl_NotifyVideoClipAddingFailedL();
         void UT_CMccTranscoderImpl_NotifyVideoClipRemovedL();
         void UT_CMccTranscoderImpl_NotifyMovieQualityChangedL();
         void UT_CMccTranscoderImpl_NotifyMovieProcessingStartedLL();
         void UT_CMccTranscoderImpl_NotifyMovieProcessingProgressedL();                                                                                                                                                                                                                                                                                    
         void UT_CMccTranscoderImpl_NotifyMovieProcessingCompletedL();
         void UT_CMccTranscoderImpl_NotifyMovieOutputParametersChangedL();
         void UT_CMccTranscoderImpl_AsyncTimerExpiredL();

    
    private:    // Data        
        CMccTranscoderImpl* iMccTranscoder;

        CVedMovie* iMovie;

        TMccTranscodeFileMsg iData;
        
        TMccEvent iMccEvent;
        
        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMCCTRANSCODERIMPL_H__

// End of file
