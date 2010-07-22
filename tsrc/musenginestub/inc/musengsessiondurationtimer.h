/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef _MUSENGSESSIONDURATIONTIMER_H_
#define _MUSENGSESSIONDURATIONTIMER_H_


// USER INCLUDES
#include "musunittesting.h"

// KERNEL INCLUDES 
#include <e32base.h>

// FORWARD DECLERATIONS
class MMusEngSessionDurationTimerObserver;

class CMusEngSessionDurationTimer : public CActive
    {
    MUS_UNITTEST( UT_CMusEngSessionDurationTimer )

    public:

		/**
   		*
        * @since S60 v3.2
        */
        static CMusEngSessionDurationTimer* NewL( 
        					MMusEngSessionDurationTimerObserver& aObserver );

		/**
   		*
        * @since S60 v3.2
        */
        ~CMusEngSessionDurationTimer();


	public: // API
	
		/**
   		*
        * @since S60 v3.2
        */
        void Start( TInt aDelay );


    private: 

		/**
   		*
        * @since S60 v3.2
        */
        CMusEngSessionDurationTimer( 
        					MMusEngSessionDurationTimerObserver& aObserver );

		/**
   		*
        * @since S60 v3.2
        */
        void ConstructL();

    
    private: // inherited from CActive

		/**
   		*
        * @since S60 v3.2
        */
        void RunL();
        
		/**
   		*
        * @since S60 v3.2
        */
        void DoCancel();


    private: // DATA

		/**
   		*
        * @since S60 v3.2
        */
        RTimer iTimer;

		/**
   		*
        * @since S60 v3.2
        */
        MMusEngSessionDurationTimerObserver& iObserver;
        
    };

#endif // _MUSENGSESSIONDURATIONTIMER_H_

