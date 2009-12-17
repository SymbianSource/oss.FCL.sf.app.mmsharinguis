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


#ifndef __UT_MUSENGSESSION_H__
#define __UT_MUSENGSESSION_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusEngLiveSession;
class CMusEngObserverStub;


//  CLASS DEFINITION
/**
 * Tester class for CMusEngSession. 
 * Since CMusEngSession is abstract, it is tested via object of its'
 * concrete descendant class CMusEngLiveSession. Only concrete 
 * implementations are tested in this tester class. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusEngSession ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngSession* NewL();
        static UT_CMusEngSession* NewLC();
        
        /**
         * Destructor
         */
        ~UT_CMusEngSession();

    private: // Constructors and destructors

        UT_CMusEngSession();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methods
        
        void UT_RectL();
        void UT_SetRectLL();
        void UT_AudioRoutingCanBeChangedL();
        void UT_EnableLoudspeakerLL();
        void UT_IsLoudSpeakerEnabledL();
        void UT_VolumeLL();
        void UT_MuteMicLL();
        void UT_IsMicMutedLL();
        void UT_SetAudioRoutingObserverL();
        void UT_VolumeUpLL();
        void UT_VolumeDownLL();
        void UT_SetVolumeLL();

    private:    // Data

        CMusEngLiveSession* iLiveSession;
        CMusEngObserverStub* iObserver;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGSESSION_H__

// End of file
