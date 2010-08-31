/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Monitor that observes the Line and create call monitor
*
*/


#ifndef MUSTSYPROPERTYMONITOR_H
#define MUSTSYPROPERTYMONITOR_H

// Enable the below line if Kodiak Ptt has to be monitered
// #include "mmusptteventobserver.h"
#include <etel.h>
#include <etelmm.h>
#include <e32base.h>
#include <e32property.h>
#include <ctsydomainpskeys.h>

#include "musunittesting.h"
#include "mussesseioninformationapi.h"
#include "mmustsypropertyobserver.h"
#include "mmuscallstateobserver.h"

class CMusCallMonitor;
class CMusVoipCallMonitor;
class CMusPTTCallMonitor;
class CMusCallConferenceMonitor;

/**
 * This class monitors the line event,if call is connected
 * it creats new call event monitor . If call is disconnected
 * it removes the call monitor . Also it responds to Ptt Events.
 *
 * @lib musaoplugin.dll
 */

class CMusTsyPropertyMonitor : public CActive,
                               public MMusTsyPropertyObserver
// Enable the below line if Kodiak Ptt has to be monitered
// ,public MMusPttEventObserver
    {

    public: // constructors and destructor

        /**
         * Two-phased constructor.
         */
        static CMusTsyPropertyMonitor* NewL( RMobilePhone& aPhone, 
                                             MMusCallStateObserver& aCallStateObserver );

        /**
         * Destructor.
         */
        ~CMusTsyPropertyMonitor();

        /*
         * Returns the current call object.
         */

        const RCall& Call();
        
        /**
         * Function from MMusTsyPropertyObserver
         */
        void NotifyCallStateChanged(NMusSessionInformationApi::TMusCallEvent aVal, TName& aCallName);
        
        /**
         * Checks from the corresponding monitors if the data is ready.
         */
        TBool IsDataReadyL();

    private:

        /**
         * C++ constructor.
         */
        CMusTsyPropertyMonitor( RMobilePhone& aPhone, MMusCallStateObserver& aCallStateObserve );

        /**
         * Symbian 2nd-phase constructor.
         */
        void ConstructL();
        

    private: // functions from base class CActive

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
        /*
         * If the call Monitor doesnt exist then it creats one new
         * CMusCallMonitor class and add into array. 
         * Leavs if objects creation fails.
         */
        void AddCallMonitorL( TName& aCallName );

        /*
         * It removs the CMusCallMonitor object from array
         * based on the call name.         
         */

        void RemoveCallMonitor( TName& aCallName );

        /*
         * It checks the property key of PTT call.If call exist ETrue
         * else otherwise.
         */
        TBool IsPttCallExist();      

        /**
          * Sets the multimediasharing propery key value
          * if the value is same before then dont report it.
          */
        void SetStateL(NMusSessionInformationApi::TMusCallEvent aVal);          

        /**
          * Add / Remove Call Monitors based on Mus criteria          
          */
        void MonitorCallL(const TPSCTsyCallState& aCallState,
                const TPSCTsyCallType& aCallType);
        
        /**
          * Add / Remove Call Monitors based on Mus criteria          
          */
        void MonitorCSCallL();
        
    private: // Voip related 
        /*
         * If the call Monitor doesnt exist then it creats one new
         * CMusCallMonitor class and add into array. 
         * Leavs if objects creation fails.
         */
        void AddVoipCallMonitorL( TName& aCallName );

        /*
         * It removs the CMusCallMonitor object from array
         * based on the call name.         
         */

        void RemoveVoipCallMonitor( TName& aCallName );
        
        /**
         * Add / Remove Call Monitors based on Mus criteria          
         */
       void MonitorVoipCallL(const TPSCTsyCallState& aCallState);

        /**
         * Removes and reset the voip call monitor array
         */
       void RemoveAllVoipCallMonitors( );
       
        /**
         * Checks multimediasharing call criteria's
         */
       void CheckCallCriteriaL();


         /**
          * Delete all call monitor        
          */
        void RemoveAllCallMonitor();

        /**
          * Check Call Staus from Call.   
          */
        RMobileCall::TMobileCallStatus CallStatus(const TDesC& aCallName);
        
        /**
          * Remove Unused Call monitorts. Sometime the call gets disconnected
          * and call state goes unnoticed.so its good to check and remove unused 
          * call monitors.
          */
        
        void RemoveUnUsedCallMonitors();

   private:
        /*
         * It will be called from CMusPttCallMonitor class incase any Ptt
         * event occurs. It will stop or start call monitoring based on this.
         * It could leave if start/stop of call monitor fails.         
         */
	
	// Enable the below line if Kodiak Ptt has to be monitered
        // void PttCallEventOccuredL( TPSCTsyKodiakPtt& aVal);


    private:    // Data               

        /**
         * RMobilePhone handle.It is not owned . So don't close it.
         */
        RMobilePhone& iPhone;

        /**
         * RMobileLine handle
         */
        RMobileLine iLine;

        /**
         * Array of Call Monitors.
         */
        RPointerArray<CMusCallMonitor> iCallMonitorArray;

        /**
         * Array of voip Call Monitors.
         */
        RPointerArray<CMusVoipCallMonitor> iVoipCallMonitorArray;
        
        /**
         * Conference call monitor, owned.
         */
        CMusCallConferenceMonitor* iConferenceMonitor;
        
        /**
         * Instance of RProperty.
         */
        RProperty iPropertyEvent;  
        
        /*
         * Reference of callstate observer not owned
         */
        MMusCallStateObserver& iCallStateObserver;

        MUS_UNITTEST( UT_CMusTsyPropertyMonitor )
        MUS_UNITTEST( UT_CMusAoPlugin )
        
    };

#endif // MUSTSYPROPERTYMONITOR_H
