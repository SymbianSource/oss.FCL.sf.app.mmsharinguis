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



#ifndef MUSCALLCONFERENCEMONITOR_H
#define MUSCALLCONFERENCEMONITOR_H

#include "musunittesting.h"
#include "mussesseioninformationapi.h"
#include "muscallmonitor.h"

#include <etelmm.h>
#include <e32base.h>

class CMusCallConferenceEventMonitor;

/**
 * Monitors the event of call and set the state to property. 
 *
 * @lib musaoplugin.dll
 */
class CMusCallConferenceMonitor : public CActive
    {

    public: 
        /**
         * Two-phased constructor. Leaves on failure.
         * aCall is not owned.
         * @return CMusCallConferenceMonitor object.
         */
        static CMusCallConferenceMonitor* NewL(
                               RMobilePhone& aPhone, RMobileLine& aLine,
                               RPointerArray<CMusCallMonitor>& aCallMonitorArray);
                                
        /**
         * Destructor.
         */
        ~CMusCallConferenceMonitor();     

         /**
          * Checks the Conference Status and 
          * sets the multimediasharing propery key value          
          */
        void SetStateL();  

         /**
          * Get Conference Call Status
          */
        
        RMobileConferenceCall::TMobileConferenceStatus GetConfStatus();

    private:

        /**
         * C++ constructor.
         */
        CMusCallConferenceMonitor(
                             RMobileLine& aLine,
                             RPointerArray<CMusCallMonitor>& aCallMonitorArray );

        /**
         * Symbian 2nd-phase constructor.
         */
        void ConstructL(RMobilePhone& aPhone);       

        /**
          * Sets the multimediasharing propery key value
          * if the value is same before then dont report it.
          */
        void SetStateL(NMusSessionInformationApi::TMusCallEvent aVal);

        /**
          * Add / Remove Call Monitors based on Mus criteria          
          */
        void MonitorCallL();

    private:

        /**
         * Handles an active object's request completion event.
         */
        void RunL();
        
        /**
         * cancels of an outstanding request
         */        
        void DoCancel();

         /*
         * Handles a leave occurring in the request completion 
         * event handler RunL().
         * Always return KErrNone since it must handle this error
         * if it comes to this place.
         */
        TInt RunError( TInt aError );


    private: 

        /**
         * Reference of RMobileLine.Not Owned.
         */
        
        RMobileLine& iLine;
       
        /**
         * Reference of RMobileConferenceCall.
         */
        RMobileConferenceCall iConfCall;

        /**
         * Conference Event.
         */
        RMobileConferenceCall::TMobileConferenceStatus iConfStatus;

         /**
         * Array of Call Monitors.Not Owned
         */
        RPointerArray<CMusCallMonitor>& iCallMonitorArray;

        /**
         * Monitors the conference call events. For example a particular call
         * which is in conference dropped will be reported as part of
         * conference event.
         */
        CMusCallConferenceEventMonitor* iConfCallEvent;
      
        MUS_UNITTEST( UT_CMusCallConferenceMonitor )   
        MUS_UNITTEST( UT_CMusTsyPropertyMonitor )
    };

#endif // MUSCALLCONFERENCEMONITOR_H
