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


#ifndef __UT_MUSENGMCEUTILS_H__
#define __UT_MUSENGMCEUTILS_H__


//  EXTERNAL INCLUDES
#include "mussipprofileuser.h"
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMceManager;
class CMusSipProfileHandler;
class CMceSession;

#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 * Tester class for MusEngMceUtils. 
 * 
 */
NONSHARABLE_CLASS( UT_MusEngMceUtils ):
	public CEUnitTestSuiteClass,
    public MMusSipProfileUser
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_MusEngMceUtils* NewL();
        static UT_MusEngMceUtils* NewLC();
                
        /**
         * Destructor
         */
        ~UT_MusEngMceUtils();

	public: // From MMusSipProfileUser    
        
        TBool IsRoamingBetweenAPsAllowed();

    private: // Constructors and destructors

        UT_MusEngMceUtils();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methdods
    
        void UT_MusEngMceUtils_IsVideoInStreamL();
        void UT_MusEngMceUtils_IsAudioInStreamL();
        void UT_MusEngMceUtils_GetVideoOutStreamLL();
        void UT_MusEngMceUtils_GetVideoInStreamLL();
        void UT_MusEngMceUtils_GetRecordingStreamL();
        void UT_MusEngMceUtils_GetMediaSinkL();  // From stream
        void UT_MusEngMceUtils_GetMediaSinkLL(); // From stream
        void UT_MusEngMceUtils_GetMediaSink2L(); // From session
        void UT_MusEngMceUtils_GetMediaSinkL2L(); // From session
        void UT_MusEngMceUtils_GetCameraLL();
        void UT_MusEngMceUtils_GetFileSourceLL();
        void UT_MusEngMceUtils_GetDisplayL();
        void UT_MusEngMceUtils_GetDisplayLL();
        void UT_MusEngMceUtils_AddDisplayLL();
        void UT_MusEngMceUtils_AddSpeakerLL();
        void UT_MusEngMceUtils_DisableStreamLL();
        
    private:    // Data

        CMceManager* iManager;
        CMusSipProfileHandler* iProfileHandler;
        CMceSession* iEmptySession;
        CMceSession* iVideoOutSession;
        CMceSession* iVideoInSession;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGMCEUTILS_H__

// End of file
