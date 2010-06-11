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

#ifndef UT_MUSENGCLIPVIDEOPLAYER_H
#define UT_MUSENGCLIPVIDEOPLAYER_H


// INCLUDES
#include <ceunittestsuiteclass.h>
#include <eunitmacros.h>
#include "musengstubs.h"

//  FORWARD DECLARATIONS
class CMceSession;
class CMusEngClipVideoPlayer;


//  CLASS DEFINITION
/**
 *
 */
NONSHARABLE_CLASS( UT_CMusEngClipVideoPlayer ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngClipVideoPlayer* NewL();
        static UT_CMusEngClipVideoPlayer* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngClipVideoPlayer();

    private: // Constructors and destructors

        UT_CMusEngClipVideoPlayer();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();
        void Teardown();

    private: // Test methdods
    
        void UT_HasClipEndedL();
        void UT_LcVideoPlayerStateL();
        void UT_LcPlayL(); 
        void UT_LcPauseL(); 
        void UT_LcIsPlayingL();
        void UT_SetLcFileNameL();
        void UT_LcFastForwardL();
        void UT_LcFastRewindL();
        void UT_LcFilePositionL();
        void UT_LcFileDurationL();
        void UT_SetLcFilePositionL();     
        void UT_LcWindowL();
        void UT_LcCameraControlL();
        void UT_LcSourceFileControlL();
        void UT_LcDestinationFileControlL();
        void UT_LcAudioControlL();
        void UT_LcZoomControlL();
        void UT_LcBrightnessControlL();
        
    private:    // Data

        TLcAudioControlStub       iLcAudioControlStub;
        CMceSession*              iMceSession;
        CMusEngClipVideoPlayer*   iClipVideoPlayer;
        TMusEngDisplayHandlerStub iDisplayHandlerStub;
        EUNIT_DECLARE_TEST_TABLE;
    };

#endif // UT_MUSENGCLIPVIDEOPLAYER_H

// End of file
