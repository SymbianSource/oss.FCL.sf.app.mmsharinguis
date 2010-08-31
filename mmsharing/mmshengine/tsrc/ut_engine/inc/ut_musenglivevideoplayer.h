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

#ifndef UT_MUSENGLIVEVIDEOPLAYER_H
#define UT_MUSENGLIVEVIDEOPLAYER_H


// INCLUDES
#include <ceunittestsuiteclass.h>
#include <eunitmacros.h>
#include "musengcamerahandler.h"
#include "musengstubs.h"

//  FORWARD DECLARATIONS
class CMceSession;
class CMusEngLiveVideoPlayer;


//  CLASS DEFINITION
/**
 *
 */
NONSHARABLE_CLASS( UT_CMusEngLiveVideoPlayer ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngLiveVideoPlayer* NewL();
        static UT_CMusEngLiveVideoPlayer* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngLiveVideoPlayer();

    private: // Constructors and destructors

        UT_CMusEngLiveVideoPlayer();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();
        void Teardown();

    private: // Test methdods
    
        void UT_NewLL();       
        void UT_LcVideoPlayerStateL();
        void UT_LcPlayL(); 
        void UT_LcWindowL();
        void UT_LcCameraControlL();
        void UT_LcSourceFileControlL();
        void UT_LcDestinationFileControlL();
        void UT_LcAudioControlL();
        void UT_LcZoomControlL();
        void UT_LcBrightnessControlL();
        void UT_EnableLcWindowL();
        void UT_LcWindowRectL();
        void UT_LcWindowOrientationL();
        void UT_SetLcFileNameL();
        void UT_LcRecordL();
        
    private:    // Data

        TLcAudioControlStub iLcAudioControlStub;
        CMceSession* iMceSession;
        TMusEngCameraHandler iCameraHandler;
        CMusEngLiveVideoPlayer* iLiveVideoPlayer;
        TMusEngDisplayHandlerStub iDisplayHandlerStub;
        EUNIT_DECLARE_TEST_TABLE;
    };

#endif // UT_MUSENGLIVEVIDEOPLAYER_H

// End of file
