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
* Description:  Monitors the Kodiak Ptt call events.
*
*/


#ifndef MUSPTTCALLMONITOR_H
#define MUSPTTCALLMONITOR_H

#include "mmusptteventobserver.h"
#include "musunittesting.h"

#include <e32base.h>    // CBase
#include <e32property.h> // RProperty


/**
 * this class is listening call monitor P&S keys
 *
 * @lib musaoplugin.dll
 */

class CMusPttCallMonitor : public CActive
    {

    public:

        /**
         * Two-phased constructor
         *
         * @since S60 v3.2
         * @return New CMusAoPlugin instance
         */
        static CMusPttCallMonitor* NewL(MMusPttEventObserver& aObserver);

        /**
         * C++ destructor.
         */
        virtual ~CMusPttCallMonitor();
  

     
    private: // constructors

        /**
         * C++ constructor.
         */
        CMusPttCallMonitor(MMusPttEventObserver& aObserver);

        /**
         * Symbian second-phase constructor.
         */
        void ConstructL();


    public: //from base class CActive

        /**
         * Handles an active object's request completion event.
         */
        void RunL();

        /**
         * cancelsof an outstanding request
         */
        void DoCancel();

        /**
         * Returns ETrue of PTT Call Exist else otherwise.
         */
        TBool IsPTTCallExist();

         /*
         * Handles a leave occurring in the request completion 
         * event handler RunL().
         * Always return KErrNone since it must handle this error
         * if it comes to this place.
         */
        TInt RunError( TInt aError );
  

    private: // data
        
        /**
         * properties for P&S
         */
        RProperty iPropertyEvent;  

        /**
         * Observer for Kodiak Events
         */
        MMusPttEventObserver& iObserver;
             
        
        MUS_UNITTEST( UT_CMusPTTCallMonitor )
    };

#endif // MUSPTTCALLMONITOR_H
