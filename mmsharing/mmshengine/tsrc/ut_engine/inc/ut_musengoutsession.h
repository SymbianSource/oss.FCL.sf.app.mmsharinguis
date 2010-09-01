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


#ifndef __UT_MUSENGOUTSESSION_H__
#define __UT_MUSENGOUTSESSION_H__


//  EXTERNAL INCLUDES
#include <digia/eunit/ceunittestsuiteclass.h>
#include <digia/eunit/eunitmacros.h>

//  FORWARD DECLARATIONS
class CMusEngLiveSession;
class CMusEngObserverStub;

// CONSTANTS
_LIT8( KMusPPreferredIdentityTestHeader, "P-Preferred-Identity: sip:user@domain.com" );
_LIT8( KMusTestUri, "sip:user@domain.com" );


//  CLASS DEFINITION
/**
 * Tester class for CMusEngOutSession. 
 * Since CMusEngOutSession is abstract, it is tested via object of its'
 * concrete child class CMusEngLiveSession. Only concrete implementations
 * are tested in this tester class. 
 * 
 */
NONSHARABLE_CLASS( UT_CMusEngOutSession ): public CEUnitTestSuiteClass
    {
    public:     // Constructors and destructors

        /**
         * Two phase construction
         */
        static UT_CMusEngOutSession* NewL();
        static UT_CMusEngOutSession* NewLC();
                
        /**
         * Destructor
         */
        ~UT_CMusEngOutSession();

    private: // Constructors and destructors

        UT_CMusEngOutSession();
        void ConstructL();
        
    private: // Test case setup and teardown

        void SetupL();

        void Teardown();

    private: // Test methdods
    
        void UT_CMusEngOutSession_InviteLL();
        void UT_CMusEngOutSession_CancelInviteLL();
        void UT_CMusEngOutSession_EstablishSessionLL();
        void UT_CMusEngOutSession_HandleTerminationL();
        void UT_CMusEngOutSession_AdjustVideoCodecLL();
        void UT_CMusEngOutSession_AdjustAudioCodecLL();
        void UT_CMusEngOutSession_CreateMceSessionStructureLL();

    private:    // Data

        CMusEngLiveSession* iLiveSession;
        CMusEngObserverStub* iObserver;

        EUNIT_DECLARE_TEST_TABLE;

    };

#endif      //  __UT_MUSENGOUTSESSION_H__

// End of file
