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


#ifndef __UT_MUSENGTWOWAYRECVSESSION_H__
#define __UT_MUSENGTWOWAYRECVSESSION_H__


//  EXTERNAL INCLUDES
#include <ceunittestsuiteclass.h>
#include <eunitmacros.h>

//  FORWARD DECLARATIONS
class CMceInSession;
class CMusEngTwoWayRecvSession;
class CMusEngReceiveSession;
class CMceVideoStream;
class CMceAudioStream;
class CLcSessionObserverStub;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 */
NONSHARABLE_CLASS( UT_CMusEngTwoWayRecvSession ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngTwoWayRecvSession* NewL();
        static UT_CMusEngTwoWayRecvSession* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngTwoWayRecvSession();

    private: // Constructors and destructors

        UT_CMusEngTwoWayRecvSession();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test helpers
    
        CMceVideoStream* ConstructVideoInStreamLC();
        CMceVideoStream* ConstructVideoOutStreamLC();
        CMceAudioStream* ConstructAudioInStreamLC();
        CMceAudioStream* ConstructAudioOutStreamLC();
        CMceInSession* ConstructInSessionLC( TInt aVideoStreamCount,
                                             TInt aAudioStreamCount,
                                             TBool aConstructSinks = ETrue );

        CMceInSession* ConstructTwoWayInSessionLC( TBool aConstructSinks = ETrue );
    
    private: // Test methdods
    
        void UT_NewLL();
        void UT_EnableDisplayLL();
        void UT_SetOrientationLL();
        void UT_OrientationLL();
        void UT_SetLocalRectLL();
        void UT_SetRectsLL();
        void UT_CameraL();
        void UT_LocalVideoPlayerL();
        void UT_HandleSessionStateChangedL();
        void UT_AdjustVideoCodecLL();
        void UT_AdjustAudioCodecLL();
        void UT_DoCodecSelectionLL();
        void UT_IncomingSessionL();        
        void UT_CompleteSessionStructureL_WithoutSessionL();
        void UT_CompleteSessionStructureL_WithoutStreamsL();
        void UT_CompleteSessionStructureL_WithOutgoingStreamL();
        void UT_CompleteSessionStructureL_WithVideoInL();
        void UT_CompleteSessionStructureL_WithVideoOutBoundToVideoInL();
        void UT_CompleteSessionStructureL_WithVideoInBoundToVideoOutL();
        void UT_CompleteSessionStructureL_WithMultipleVideoInsL();
        void UT_CompleteSessionStructureL_WithVideoAndMultipleAudioInsL();
        void UT_CompleteSessionStructureL_OperatorVariant();
                
    private:    // Data

        CMusEngTwoWayRecvSession* iReceiveSession;
        CLcSessionObserverStub* iLcSessionObserver;

        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_MUSENGRECEIVESESSION_H__

// End of file
