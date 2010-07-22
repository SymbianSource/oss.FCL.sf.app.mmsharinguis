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
* Description:  Monitors the VOIP calls.
*
*/


#ifndef MUSVOIPCALLMONITOR_H_
#define MUSVOIPCALLMONITOR_H_

#include <e32base.h>

#include <ccallinformation.h>
#include <mcall.h>
#include <ccallinfoiter.h>

#include "mussesseioninformationapi.h"
#include "musunittesting.h"
#include "mmuscallstateobserver.h"

/**
 * Monitors the Internet telephony calls ( VOIP )  
 * Same condition of CS call criteria applies here to
 * for Mush. 
 * @lib musaoplugin.dll
 */
class CMusVoipCallMonitor : public CActive
    {

    public: // constructors and destructor
        /**
         * Symbian two-phased constructor.
         */
        static CMusVoipCallMonitor* NewL( TName& aCallName, 
											MMusCallStateObserver& aCallStateObserver );
        
        /**
         * Symbian two-phased constructor.
         */
        static CMusVoipCallMonitor* NewLC( TName& aCallName );

        /**
         * Destructor.
         */
        ~CMusVoipCallMonitor();
        
    private: // functions from base class CActive
        
        void RunL();

        void DoCancel();

        TInt RunError( TInt aError );
        
    private:

        /**
         * C++ constructor.
         */
        CMusVoipCallMonitor( TName& aCallName, MMusCallStateObserver& aCallStateObserver );

        /**
         * Symbian 2nd-phase constructor.
         */
        void ConstructL( );
    
    public:
        
        /**
         * Compares the call names (aName == iCallName) 
         * 
         * @return ETrue if matches else EFalse
         */
        TBool IsEqual(TName& aName);

        /**
         * Sets the call state PS Key value.
         * 
         * @aVal one of the call event types
         */
        void SetStateL(NMusSessionInformationApi::TMusCallEvent aVal);        
        
        /**
         * Checks from voip call monitor if required data is ready.
         */
         TBool IsDataReadyL();

    private:
        
        /**
         * Sets the call Information like call direction and remote address
         */
        void SetCallInfoL();   

        /**
         * Sets the call Provider Information (CallProvider Name is PS Key)
         */
        void SetCallProviderInfoL( TUint32 aServiceId );   
        
        TBool RemoteInfoExistsL();
        
        void MonitorForRemoteInfoL();
        
        void StopRemoteInfoMonitoring();
        
        void ReportStateChangeL( NMusSessionInformationApi::TMusCallEvent aVal );
        
        static TInt GuardTimerExpired( TAny* aPtr );
        
        /**
         * Returns current connected call Service ID.
         * @return Current connected call service ID, if there is at least one
         * call in connected state, otherwise KErrNotFound
         */
        TUint32 CallServiceIdL(); 
        
        void AddDomainFromOwnUsernameL( TUint32 aServiceId, TDes16& aUri );
        
        // Resets the Call Provider Information.
        void ResetCallProviderName();
        
        
    private:
        /*
         * Name of the call.
         */
        TName& iCallName ; 
        
        RProperty iPropertyEvent;
        
        /*
         * Reference of callstate observer not owned
         */
        MMusCallStateObserver& iCallStateObserver;
        
        CDeltaTimer* iGuardTimer;
        TCallBack iGuardTimerCallBack;
        TDeltaTimerEntry iGuardTimerEntry;
        
        MUS_UNITTEST( UT_CMusVoipCallMonitor )

    };

#endif /* MUSVOIPCALLMONITOR_H_ */
