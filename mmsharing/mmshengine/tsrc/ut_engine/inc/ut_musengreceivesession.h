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


#ifndef __UT_MUSENGRECEIVESESSION_H__
#define __UT_MUSENGRECEIVESESSION_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMceInSession;
class CMusEngReceiveSession;
class CLcSessionObserverStub;
class CMceVideoStream;
class CMceAudioStream;
class CLcUiProviderStub;

#include <e32def.h>
#ifndef NONSHARABLE_CLASS
    #define NONSHARABLE_CLASS(x) class x
#endif

//  CLASS DEFINITION
/**
 *
 */
NONSHARABLE_CLASS( UT_CMusEngReceiveSession ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngReceiveSession* NewL();
        static UT_CMusEngReceiveSession* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngReceiveSession();

    private: // Constructors and destructors

        UT_CMusEngReceiveSession();
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

    private: // Test methdods
    
        void UT_NewLL();
        void UT_LcSessionStateL();
        void UT_EstablishLcSessionL();
        void UT_TerminateLcSessionL();
        void UT_RemoteVideoPlayerL();        
        void UT_RemoteDisplayName();        
        void UT_HandleSessionStateChangedL();
        void UT_AdjustVideoCodecLL();
        void UT_AdjustAudioCodecLL();
        void UT_DoCodecSelectionLL();
        void UT_IncomingSessionL();
        void UT_IncomingUpdateL();
        void UT_StreamStateChangedL();
        void UT_CompleteSessionStructureL_WithoutSessionL();
        void UT_CompleteSessionStructureL_WithoutStreamsL();
        void UT_CompleteSessionStructureL_WithOutgoingStreamL();
        void UT_CompleteSessionStructureL_WithVideoInL();
        void UT_CompleteSessionStructureL_WithVideoOutBoundToVideoInL();
        void UT_CompleteSessionStructureL_WithVideoInBoundToVideoOutL();
        void UT_CompleteSessionStructureL_WithMultipleVideoInsL();
        void UT_CompleteSessionStructureL_WithVideoAndMultipleAudioInsL();
        void UT_CompleteSessionStructureL_OperatorVariant();   
        void UT_CompleteSessionStructureL_SdpBandwidthAttributesL();   
        void UT_ParseAssertedIdentityL();
        void UT_ContactSavingLL();
        void UT_IsDisplayActive();
        void UT_InactivityTimeoutL();
        
    private:    // Data

        CMusEngReceiveSession* iReceiveSession;
        CLcSessionObserverStub* iLcSessionObserver;
        CLcUiProviderStub* iLcUiProvider;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGRECEIVESESSION_H__

// End of file
