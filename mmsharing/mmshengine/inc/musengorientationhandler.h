/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef MUSENGORIENTATIONHANDLER_H
#define MUSENGORIENTATIONHANDLER_H


// INTERNAL INCLUDE
#include "musunittesting.h"

// SYSTEM INCLUDE
#include <e32base.h>

class CMusEngLiveSession;
class MMusEngLiveSessionObserver;

/**
*
* @lib musengine.lib
*/
NONSHARABLE_CLASS(CMusEngOrientationHandler) : public CTimer
                              
    {
    MUS_UNITTEST( UT_CMusEngOrientationHandler )
    
    public:

		/**
        * Creates new instance of CMusEngOrientationHandler
        */
        static CMusEngOrientationHandler* NewL( CMusEngLiveSession& aSession,
                                                MMusEngLiveSessionObserver& aSessionObserver );

        /**
        * Destructor
        *
        */
        ~CMusEngOrientationHandler();


    public: // API
        
        void RefreshOrientationL();
        void UpdateL();
        
    protected:
        
        void RunL();
        TInt RunError( TInt aError );

    private:
        
        CMusEngOrientationHandler( CMusEngLiveSession& aSession,
                                   MMusEngLiveSessionObserver& aSessionObserver );
        void ConstructL();
        
        void DoDelayedResumeL();

    private: // DATA
        
        CMusEngLiveSession& iSession;
        MMusEngLiveSessionObserver& iSessionObserver;
        TBool iRefreshBeginStateCamera;
        TBool iRefreshBeginStateDisplay;

    };

#endif // MUSENGORIENTATIONHANDLER_H
