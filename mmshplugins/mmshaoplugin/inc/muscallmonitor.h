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
* Description:  Monitors the multimediasharing property.
*
*/



#ifndef MUSAOCALLMONITOR_H
#define MUSAOCALLMONITOR_H

#include "musunittesting.h"
#include "mustsypropertymonitor.h"
#include <etelmm.h>
#include <e32base.h>

class CMusCallConferenceMonitor;
class CMusCallStatusMonitor;
class CMusCallEventMonitor;

/**
 * Monitor, which observes status of current cs call  
 *
 * @lib musaoplugin.dll
 */
class CMusCallMonitor : public CBase
    {

    public: // constructors and destructor

        /**
         * Two-phased constructor. Leaves on failure.
         */
        static CMusCallMonitor* NewL(  TName& aCallName,TBool aPttCallExist );

        /**
         * Destructor.
         */
        ~CMusCallMonitor();

    public:

        /**
         * Starts to monitor call status,event and conference.
         */
        void StartMonitorL( RMobileLine& aLine, MMusTsyPropertyObserver& aObserver );

        /**
         * Stops monitoring call status,event and conference.
         */
        void StopMonitor();

        /**
         * Compares the name of the call.
         */
        TBool IsEqual(TName& aName);

        /**
         * Returns the CallName.
         */
        const TName Name();

        /**
         * ETrue if monitoring stared already. Else otherwise
         */
        TBool IsMonitored();

        /*
         * Returns the name of last call occured
         */
        TName LastCallNameL(RMobileLine& aLine);
                
        /**
         * Call CMusCallMonitorBase::SetStateL to set 
         * state of call status monitor and
         * state of call event monitor
         */
        void SetStateL(NMusSessionInformationApi::TMusCallEvent aVal);
        
        
    private:

        /**
         * C++ constructor.
         */
        CMusCallMonitor(  TBool aPttCallExist  );

        /**
         * Symbian 2nd-phase constructor.
         */
        void ConstructL( TName& aCallName);

        /*
         * Sets the call count based on callmonitors.
         * ETrue to increase. EFalse to decrease. 
         */        
        void SetCallCountL(TBool aIncrease);

    private:            

        /**
         * Call status monitor, owned.
         */
        CMusCallStatusMonitor* iCallStatusMonitor;

        /**
         * Call event monitor, owned.
         */
        CMusCallEventMonitor* iCallEventMonitor;

        /**
         * RMobilePhone handle.
         */
        RMobileCall iCall;

        /**
         * Name of the call.
         */
        TName iCallName;

        /**
         * Existence of Ptt call when this cs call started.
         */
        TBool iPttCallExist;

        MUS_UNITTEST( UT_CMusCallMonitor )

    };

#endif // MUSAOCALLMONITOR_H
