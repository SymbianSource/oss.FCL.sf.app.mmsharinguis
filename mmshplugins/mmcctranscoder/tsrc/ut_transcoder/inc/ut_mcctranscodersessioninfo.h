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


#ifndef __UT_CMCCTRANSCODERSESSIONINFO_H__
#define __UT_CMCCTRANSCODERSESSIONINFO_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>
#include <mmccevents.h>
#include <mmcctranscoder.h>

// Forward declaration
class CMccTranscoderSessionInfo;
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

NONSHARABLE_CLASS( UT_CMccTranscoderSessionInfo ): public CEUnitTestSuiteClass
    {
    public:
        //Two phase construction
        static UT_CMccTranscoderSessionInfo* NewL();
        static UT_CMccTranscoderSessionInfo* NewLC();
        // Destructor                  
        ~UT_CMccTranscoderSessionInfo();

    private: // Constructor
        UT_CMccTranscoderSessionInfo();
        void ConstructL();

    private: // Test case setup and teardown
        void SetupL();
        void Teardown();

    private: // Test methdods

        
        void UT_CMccTranscoderSessionInfo_SetQualityL();
        void UT_CMccTranscoderSessionInfo_CreateMovieLL();
        void UT_CMccTranscoderSessionInfo_SetDesFileLL();
        void UT_CMccTranscoderSessionInfo_SetVideoCodecL();
        void UT_CMccTranscoderSessionInfo_SetAudioCodecL();
        void UT_CMccTranscoderSessionInfo_SetVideoTypeL();
        void UT_CMccTranscoderSessionInfo_SetAudioTypeL(); 
        void UT_CMccTranscoderSessionInfo_CheckVideoCodecLL();
        void UT_CMccTranscoderSessionInfo_CheckAudioCodecLL();
        void UT_CMccTranscoderSessionInfo_GetPutputParametersL();
        void UT_CMccTranscoderSessionInfo_ProgressedL();
        void UT_CMccTranscoderSessionInfo_ClipAddedL();
        void UT_CMccTranscoderSessionInfo_QualityChangeNeededL();
    
    private:    // Data        
        CMccTranscoderSessionInfo* iSession;

        CVedMovie* iMovie;
        
        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_CMCCTRANSCODERSESSIONINFO_H__

// End of file
