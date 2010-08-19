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
* Description:  This is default implementation class for CMusAvaInterface
*
*/


#ifndef __MUSAVADEFAULTIMP_H__
#define __MUSAVADEFAULTIMP_H__

//  USER
#include "musavainterface.h"
#include "musavaavailabilityobserver.h"
#include "musunittesting.h"

// SYSTEM
#include <e32base.h>
#include <e32std.h>

class MMusAvaObserver;
class CMusAvaSettingsImp;
class CMusAvaSettings;
class CMusAvaSharedObject;
class CMusAvaAvailability;
class MMusAvaAvailabilityObserver;
class MMusAvaSettingsObserver;

/**
 *  CMusAvaDefaultImp class represent the interface of application.
 *
 *  It accept request from the API for creating the corresponding
 *  availability report.
 *
 *  @lib musavailabilityplugin.lib
 */
class CMusAvaDefaultImp : public CMusAvaInterface, 
    private MMusAvaAvailabilityObserver
    {
    public:  // Constructors and destructor

        /**
         * Two-phased constructor
         *
         * @return Returns pointer to CMusAvaDefaultImp object
         */
        static CMusAvaDefaultImp* NewL();

        /**
         * Destructor
         *
         */
        ~CMusAvaDefaultImp();

   protected:  // Constructors

        /**
         * Constructor
         *
         * @param aObserver Pointer to observer intercase
         */
        CMusAvaDefaultImp( );

        /**
         * Perform the second phase construction of a
         *             CMusAvaDefaultImp object
         *
         */
        void ConstructL();


    protected:


        /**
         * Create availability setting.
         *
         */
        void CreateAvailabilityModulesL();


        /**
         * Create work for executing availability report.
         *
         */
        void ExecuteAvailablitityModules();

		 /**
         * Create handler (Invite/Options) availability module.
         *
         */
		void ConstuctHandlerAvailabilityModulesL();
		
		 /**
         * Create static ( Sipprofile / activation ) availability module.
         *
         */
		void ConstructStaticAvailabilityModulesL();

        /**
         * Create dynamic ( call / network / bearer ) availability module.
         *
         */
		void ConstructDynamicAvailabilityModulesL();


    public: // from base class CMusAvaInterface

         /**
         * Called by the ECOM framework when MusAvailability Plug-in is request
         * retuns setting interface for the client.
         *
         */
        virtual MMusAvaSettings& Settings();

        /**
         * Called by the ECOM framework when MusAvailability Plug-in is for
         * Observer interface for the client.
         *
         * @since S60 v3.2
         * @param aObserver bserver interface
         */
        virtual void SetObserver( MMusAvaObserver& aObserver );
        
        /**
         * Called by the ECOM framework when MusAvailability Plug-in is for
         * setting Observer interface for the client.
         *
         * @since S60 v3.2
         * @param aObserver MMusAvaSettingsObserver interface
         */
        virtual void SetSettingsObserver( MMusAvaSettingsObserver& aObserver );

         /**
         * Called by the ECOM framework when MusAvailability Plug-in is request
         * to investigate availabilites for the client.
         *
         */
        virtual void StartL();

        /**
         * Called by the ECOM framework when MusAvailability Plug-in is request
         * to stop investigate or monitoring availabilites for the client.
         *
         */
        virtual void Stop();

        void CalculateAvailability();
        virtual MMusAvaObserver::TAvailabilityName CurrentAvailability();
        virtual MMusAvaObserver::TAvailabilityStatus AvailabilityStatus();
        virtual TBool MinimumAvailability();
        virtual TBool MandatoryAvailability();
        virtual TBool ExtensionAvailability();
        virtual MMusAvaObserver::TAvailabilityStatus AvailabilityState( MMusAvaObserver::TAvailabilityName aAvailability );

    public: // from base class MMusAvaAvailabilityObserver

       /**
         * Availability report
         *
         * @param aName Availability name
         * @param aStatus Current availability Status
         */
    virtual void AvailabilityChanged(
        MMusAvaObserver::TAvailabilityName aName,
        MMusAvaObserver::TAvailabilityStatus aStatus );
        
		 /**
         * Availability error report
         *
         * @param aName Availability name
         * @param aStatus Current availability Status
         */
    virtual void AvailabilityError(
        MMusAvaObserver::TAvailabilityName aName,
        MMusAvaObserver::TAvailabilityStatus aStatus );
        
    virtual TBool Available( MMusAvaObserver::TAvailabilityName aAvailability );
        
    virtual MMusAvaObserver::TAvailabilityStatus AvailabilityPluginState();
    
    virtual void AvailabilitiesAbleToShowIndicator();
    

    private: // data

        RPointerArray<CMusAvaAvailability> iAvailabilities;

        /**
         * Pointer to Observer interface for the ECOM Client
         */
        MMusAvaObserver* iObserver;

        /**
         * Pointer to CMusAvaSettings object
         */
        CMusAvaSettingsImp* iSettings;

        MMusAvaObserver::TAvailabilityName iCurrentAvailability;
        MMusAvaObserver::TAvailabilityStatus iAvailabilityStatus;

        TBool iStopping;
        TBool iExecuteStarted;
        
        TInt iForbidden;

        MUS_UNITTEST( UT_CMusAvaDefaultImp )

        };


#endif // __MUSAVADEFAULTIMP_H__

