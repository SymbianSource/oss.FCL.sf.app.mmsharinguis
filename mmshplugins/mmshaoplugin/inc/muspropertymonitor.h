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
* Description:  Monitors the call.
*
*/



#ifndef MUSAOCALLMONITOR_H
#define MUSPROPERTYMONITOR_H

#include "musunittesting.h"
#include "mmuscallstateobserver.h"
#include <etelmm.h>
#include <e32base.h>
#include <e32property.h>

class CMusManager;

/**
 * Monitor, which observes status of current cs call  
 *
 * @lib musaoplugin.dll
 */
class CMusPropertyMonitor : public CActive
    {

    public: // constructors and destructor

        /**
         * Two-phased constructor. Leaves on failure.
         */
        static CMusPropertyMonitor* NewL( MMusCallStateObserver& aCallStateObserver );

        /**
         * Destructor.
         */
        ~CMusPropertyMonitor();
        
        
    public: // other public functions
        
        /*
         * Checks from P/S Keys about call state
         * @return EFalse if call state is ENoCall otherwise ETrue  
         */
        TBool IsCallConnected();


    private:

        /**
         * C++ constructor.
         */
        CMusPropertyMonitor( MMusCallStateObserver& aCallStateObserve );

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
         * Instance of RProperty.
         */
        RProperty iPropertyEvent;    
     
        /*
         * Reference of callstate observer not owned
         */
        MMusCallStateObserver& iCallStateObserver;

        MUS_UNITTEST( UT_CMusPropertyMonitor )

    };

#endif // MUSPROPERTYMONITOR_H
