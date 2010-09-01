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
* Description:  Monitor the event of a call
*
*/



#ifndef MUSCALLEVENTMONITOR_H
#define MUSCALLEVENTMONITOR_H

#include "musunittesting.h"
#include "muscallmonitorbase.h"

#include <etelmm.h>
#include <e32base.h>

/**
 * Monitors the event of call and set the state to property. 
 *
 * @lib musaoplugin.dll
 */
class CMusCallEventMonitor : public CMusCallMonitorBase
    {

    public: 
        /**
         * Two-phased constructor. Leaves on failure.
         * aCall is not owned.
         * @return CMusCallEventMonitor object.
         */
        static CMusCallEventMonitor* NewL(const RMobileCall& aCall, MMusTsyPropertyObserver& aObserver );
                                
        /**
         * Destructor.
         */
        ~CMusCallEventMonitor();       

    private:

        /**
         * C++ constructor.
         */
        CMusCallEventMonitor( const RMobileCall& aCall, MMusTsyPropertyObserver& aObserver );

        /**
         * Symbian 2nd-phase constructor.
         */
        void ConstructL();            

    private:

        /**
         * Handles an active object's request completion event.
         */
        void RunL();
        
        /**
         * cancels of an outstanding request.
         */        
        void DoCancel();

       
    private:        
        /**
         * Mobile Call Events.
         */
        RMobileCall::TMobileCallEvent iCallEvent;

        MUS_UNITTEST( UT_CMusCallEventMonitor )        
        
    };

#endif // MUSCALLMONITOR_H
