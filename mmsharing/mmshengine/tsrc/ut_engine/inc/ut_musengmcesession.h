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
class CLcSessionObserverStub;
class CLcUiProviderStub;

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

        void UT_LcSessionStateL();
        void UT_RemoteVideoPlayerL();
        void UT_LocalVideoPlayerL();          
        void UT_LocalDisplayNameL();
        void UT_RemoteDisplayNameL();
        void UT_RemoteDetailsL();
        void UT_SetParameterL();
        void UT_ParameterValueL();
        void UT_IsLcAudioMutedL();
        void UT_MuteLcAudioL();
        void UT_MuteLcMicL();
        void UT_EnableLcLoudspeakerL();
        void UT_SetLcVolumeL();        
        void UT_GetSessionTimeL();
        void UT_HandleTerminationL();
        void UT_AdjustVideoCodecL();
        void UT_AdjustAudioCodecL();
        void UT_RectChangedL();
        void UT_SetSessionSdpLinesL();
        void UT_SetSessionSdpLines_OperatorL();
        void UT_SetMediaSdpLinesL();
        void UT_SetMediaSdpLines_OperatorL();
        void UT_AdjustStreamsAndCodecsL();
        void UT_IncomingSessionL();
        void UT_IncomingUpdateL();
        void UT_StreamStateChangedL();
        void UT_StreamStateChangedWithSourceL();
        void UT_StreamStateChangedWithSinkL();
        void UT_SessionStateChangedL();
        void UT_HandleSessionStateChanged_EncoderKeyStoringL();
        void UT_SessionConnectionStateChangedL();
        void UT_FailedL();
        void UT_UpdateFailedL();
        void UT_SRReceivedL();
        void UT_RRReceivedL();        
        void UT_SsrcAddedL();
        void UT_SsrcRemovedL();
        void UT_UpdateTimerEventL();
        void UT_IsRoamingBetweenAPsAllowedL();
        void UT_SaveContactL();
        void UT_RectL();
        void UT_SetRectLL();
        void UT_SecondaryRectL();
        void UT_SetSecondaryRectLL();
        void UT_EnableDisplayLL();
        void UT_CMusEngMceSession_SetOrientationLL();
        void UT_SetEncodingDeviceLL();
        void UT_SetCodecConfigKeyLL();
        void UT_ReadCodecConfigKeyLL();
        void UT_StoreEncoderConfigInfoLL();
        void UT_StreamStreamingL();
        void UT_InformObserverAboutSessionStateChangeL();
        void UT_InformObserverAboutSessionUpdateL();
        void UT_InformObserverAboutSessionFailureL();
        void UT_InformObserverAboutPlayerStateChangeL();
        void UT_InformObserverAboutPlayerUpdateL();
        void UT_InformObserverAboutPlayerFailureL();      
        void UT_LcUiProviderL(); 
        void UI_IsBackgroundStartupL();
        void UT_InformUiProviderAboutReceivingStartL();
        void UT_SetForegroundStatusL();
        void UT_UpdateLcSessionLL();
        
    private:    // Data

        CMusEngLiveSession* iLiveSession;
        CMusEngReceiveSession* iReceiveSession;
        CMusEngClipSession* iClipSession;
        CMusEngLiveSession* iSomeOtherSession;
        CLcSessionObserverStub* iLcSessionObserver;
        CLcUiProviderStub* iLcUiProvider;
        TMceTransactionDataContainer iContainer;
        EUNIT_DECLARE_TEST_TABLE;
    };

#endif      //  __UT_MUSENGMCESESSION_H__

// End of file
