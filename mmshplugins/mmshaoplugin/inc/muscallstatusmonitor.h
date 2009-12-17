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
* Description:  Monitor the status of the call.
*
*/



#ifndef MUSCALLSTATUSMONITOR_H
#define MUSCALLSTATUSMONITOR_H

#include "mussesseioninformationapi.h"
#include "muscallmonitorbase.h"
#include "musunittesting.h"

#include <etelmm.h>
#include <e32base.h>


/**
 * Monitors the event of call and set the state to property. 
 *
 * @lib musaoplugin.dll
 */
class CMusCallStatusMonitor : public CMusCallMonitorBase
    {

    public: 
        /**
         * Two-phased constructor. Leaves on failure.
         * aCall is not owned.
         * @return CMusCallStatusMonitor object.
         */
        static CMusCallStatusMonitor* NewL( const RMobileCall& aCall, MMusTsyPropertyObserver& aObserver );
                                
        /**
         * Destructor.
         */
        ~CMusCallStatusMonitor();       

    private:

        /**
         * C++ constructor.
         */
        CMusCallStatusMonitor(const RMobileCall& aCall, MMusTsyPropertyObserver& aObserver );

        /**
         * Symbian 2nd-phase constructor.
         */
        void ConstructL();  

        /**
          * Checks the callstatus and sets the multimediasharing property
          * key value.
          */
        void CheckStateL();            

    private:

        /**
         * Handles an active object's request completion event.
         */
        void RunL();
        
        /**
         * cancels of an outstanding request
         */        
        void DoCancel();        


    private: 
       
        /**
         * Call Status.
         */
        RMobileCall::TMobileCallStatus iCallStatus;
    
      
        MUS_UNITTEST( UT_CMusCallStatusMonitor )        
        
    };

#endif // MUSCALLSTATUSMONITOR_H
