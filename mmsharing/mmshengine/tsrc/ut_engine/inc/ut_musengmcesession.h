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


#ifndef __UT_MUSENGMCESESSION_H__
#define __UT_MUSENGMCESESSION_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>
#include <mcetransactiondatacontainer.h>

//  FORWARD DECLARATIONS
class CMusEngLiveSession;
class CMusEngReceiveSession;
class CMusEngClipSession;
class CMusEngObserverStub;
class CMceSession;
class CMusEngMceSession;

//  CLASS DEFINITION
/**
* Tester class for CMusEngMceSession. 
* Since CMusEngMceSession is abstract, it is tested via object of its'
* concrete descendant class CMusEngLiveSession. Only concrete 
* implementations are tested in this tester class. 
* 
*/
NONSHARABLE_CLASS( UT_CMusEngMceSession ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngMceSession* NewL();
        static UT_CMusEngMceSession* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngMceSession();

    private: // Constructors and destructors

        UT_CMusEngMceSession();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();
        void Teardown();

    private: // Test methdods
    
        void UT_CMusEngMceSession_TerminateLL();
        void UT_CMusEngMceSession_GetSessionTimeL();
        void UT_CMusEngMceSession_ConnectionActiveL();
        void UT_CMusEngMceSession_ContainsAudioLL();
        void UT_CMusEngMceSession_IsMutedLL();
        void UT_CMusEngMceSession_OrientationLL();
        void UT_CMusEngMceSession_SetOrientationLL();
        void UT_CMusEngMceSession_VolumeUpLL();
        void UT_CMusEngMceSession_VolumeDownLL();
        void UT_CMusEngMceSession_SetVolumeLL();
        void UT_CMusEngMceSession_EnableDisplayL();
        void UT_CMusEngMceSession_MuteLL();
        void UT_CMusEngMceSession_UnmuteLL();
        void UT_CMusEngMceSession_HandleTerminationL();

        void UT_CMusEngMceSession_AdjustVideoCodecLL();
        void UT_CMusEngMceSession_AdjustAudioCodecLL();
        void UT_CMusEngMceSession_RectChangedLL();
        void UT_CMusEngMceSession_SetSessionSdpLinesLL();
        void UT_CMusEngMceSession_AdjustStreamsAndCodecsLL();
        void UT_CMusEngMceSession_IncomingSessionL();
        void UT_CMusEngMceSession_IncomingUpdateL();
        void UT_CMusEngMceSession_StreamStateChangedL();
        void UT_CMusEngMceSession_StreamStateChangedWithSourceL();
        void UT_CMusEngMceSession_StreamStateChangedWithSinkL();
        void UT_CMusEngMceSession_SessionStateChangedL();
        void UT_CMusEngMceSession_SessionConnectionStateChangedL();
        void UT_CMusEngMceSession_FailedL();
        void UT_CMusEngMceSession_UpdateFailedL();
        
        void UT_CMusEngMceSession_SRReceivedL();
        void UT_CMusEngMceSession_RRReceivedL();
        void UT_CMusEngMceSession_InactivityTimeoutL();
        void UT_CMusEngMceSession_SsrcAddedL();
        void UT_CMusEngMceSession_SsrcRemovedL();
        void UT_CMusEngMceSession_UpdateTimerEventL();
        void UT_CMusEngMceSession_IsRoamingBetweenAPsAllowedL(); 
        void UT_CMusEngMceSession_IsDisplayEnabledLL();
        
        void UT_CMusEngMceSession_VolumeChangedL();
    
    private:
        
        void SetMaxVolForSpeakersL(CMceSession& aSession, TInt aMaxVol);
        TBool VerifySpeakersVolume(CMusEngMceSession& aSession, TInt aVolume);
        
    private:    // Data

        CMusEngLiveSession* iLiveSession;
        CMusEngReceiveSession* iReceiveSession;
        CMusEngClipSession* iClipSession;
        CMusEngLiveSession* iSomeOtherSession;
        CMusEngObserverStub* iObserver;

        TMceTransactionDataContainer iContainer;
        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_MUSENGMCESESSION_H__

// End of file
