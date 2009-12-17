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



#ifndef MUSAOCALLMONITORBASE_H
#define MUSAOCALLMONITORBASE_H

#include "musunittesting.h"
#include "mussesseioninformationapi.h"
#include "mmustsypropertyobserver.h"
#include <etelmm.h>
#include <e32base.h>

/**
 * Monitor, which observes status of current cs call  
 *
 * @lib musaoplugin.dll
 */
class CMusCallMonitorBase : public CActive
    {

    public: // constructors and destructor        

        /**
         * Destructor.
         */
        ~CMusCallMonitorBase();

    public:

        /**
         * Sets the state to PS Keys
         */
        void SetStateL(NMusSessionInformationApi::TMusCallEvent aVal);

        /**
         * Sets the Call Information.
         */
        void SetCallInfoL();

        /**
         * CActive Implentation
         */
        TInt RunError( TInt aError ); 

        
    protected:

        /**
         * C++ constructor.
         */
        CMusCallMonitorBase( const RMobileCall& aCall, MMusTsyPropertyObserver& aObserver );
        
        /**
         * Notify observer that our state changed
         */
        virtual void NotifyCallStateChanged( NMusSessionInformationApi::TMusCallEvent aVal );
        
        /**
         * Inspect the dialled number and set the PS key controlling whether
         * to add Privacy-header accordingly.
         */        
        void SetClirSetting( const TDesC& aDialledNumber ) const;

         /**
         * Should set the privacy ps key to on off based on incoming cs call privacy settings.
         * aCallInfo : Callinfo package read from current cs call.     
         */
        void SetTerminatingPrivacy( const RMobileCall& aCall ) const;
        
        /**
        * Removes privacy prefix away from phone number
        * @return returns dialed number witout prefix.  
        */
        HBufC* RemovePrefix( const TDesC& aOriginator ) const;
        
    protected:            
       
        /**
         * RMobileCall handle.
         */
        const RMobileCall& iCall;  

         /**
          * Store the remote state so that when local call events
          * happen then we can still aware the remote terminal.          
          */
        RMobileCall::TMobileCallEvent iRemoteCallEvent;

        /**
          * Store the local call event state so that when remote call events
          * happen then we can still aware the local terminal.
          */
        RMobileCall::TMobileCallEvent iLocalCallEvent;    
        
        /**
         *  Observer to get nofitied when RCall status changed 
         */
        MMusTsyPropertyObserver& iTsyObserver;

        MUS_UNITTEST( UT_CMusCallMonitor )
        MUS_UNITTEST( UT_CMusCallEventMonitor ) 
        MUS_UNITTEST( UT_CMusCallStatusMonitor )
        MUS_UNITTEST( UT_CMusCallMonitorBase )
    };

#endif // MUSAOCALLMONITORBASE_H
