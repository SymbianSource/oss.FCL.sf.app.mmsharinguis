/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Monitor that observes the calling line identity restriction
*                setting
*
*/



#ifndef MUSCLIRMONITOR_H
#define MUSCLIRMONITOR_H

#include "musunittesting.h"

#include <etelmm.h>
#include <e32base.h>
#include <etel3rdparty.h>


/**
 * Monitor, which observes status of CLIR setting that controls whether the
 * caller's phone number/id is shown to the called party.
 *
 * @lib musaoplugin.dll
 */
class CMusClirMonitor : public CActive
    {

    public: // constructors and destructor

        /**
         * Two-phased constructor. Leaves on failure.
         */
        static CMusClirMonitor* NewL();

        /**
         * Destructor.
         */
        ~CMusClirMonitor();


    public: // API

         /**
          * Starts the asynchronous reading of CLIR setting. When reading 
          * completes, the PS key is updated.
          */
         void ReadClir();

    private:

        /**
         * C++ constructor.
         */
        CMusClirMonitor();

    private: // functions from base class CActive

        void ConstructL();

        void RunL();

        void DoCancel();

        TInt RunError( TInt aError );

        void TranslateResult( CTelephony::TIdentityServiceStatus aResult );
        
    private: // Data

        CTelephony* iTelephony; //owned
        CTelephony::TIdentityServiceV1 iTIdentityServiceV1;
        CTelephony::TIdentityServiceV1Pckg iTIdentityServiceV1Pckg;
        
        // Tells if phone's number/id is allowed to be sent to the remove party
        TBool iAllowed;

		// How many times RunL has been completed with an error in a row
		TInt iErrorCount;
		
        MUS_UNITTEST( UT_CMusClirMonitor )

    };

#endif // MUSCLIRMONITOR_H
