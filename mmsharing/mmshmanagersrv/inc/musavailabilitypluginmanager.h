/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Handles interaction with availability plugin
*
*/


#ifndef MUSAVAILABILITYPLUGINMANAGER_H
#define MUSAVAILABILITYPLUGINMANAGER_H

#include "musmanagercommon.h"
#include "musavaobserver.h"
#include "musunittesting.h"
#include "musindicatorobserver.h"
#include "musavasettingsobserver.h"

#include <badesca.h>
#include <e32base.h>

class CMusAvailabilityPlugin;
class MMusAvailabilityPluginManagerObserver;
class MDesCArray;
class CMusAvaInterface;
class CMusIndicatorApi;
class CMusApplicationManager;

/**
 *  ?one_line_short_description
 *
 *  ?more_complete_description
 *
 *  @lib ?library
 *  @since S60 ?S60_version *** for example, S60 v3.0
 */
class CMusAvailabilityPluginManager : public CBase,
    								  public MMusAvaObserver,
    								  public MMusIndicatorObserver,
    								  public MMusAvaSettingsObserver
    {

    public: // first phase constructors & destructor

        /**
        *
        */
        static CMusAvailabilityPluginManager* NewL(
                            MMusAvailabilityPluginManagerObserver& aObserver,
                            CMusApplicationManager& aApplicationManager );

        /**
        *
        */
        static CMusAvailabilityPluginManager* NewLC(
        					MMusAvailabilityPluginManagerObserver& aObserver,
        					CMusApplicationManager& aApplicationManager );

        /**
        *
        */
        ~CMusAvailabilityPluginManager();


    protected:

        /**
        *
        */
        void ConstructL();

        /**
        *
        */
        CMusAvailabilityPluginManager(
                            MMusAvailabilityPluginManagerObserver& aObserver,
                            CMusApplicationManager& aApplicationManager );


    public: // API

		/**
        *
        */
        TBool ApplicationAllowed();
        

        /**
        * Returning composed availability status
        */
        MultimediaSharing::TMusAvailabilityStatus Availability();

        /**
        *
        */
        void InvestigateAvailabilityL();

        /**
        *
        */
        void InvitationReceivedL();

        /**
        *
        */
        void OptionsReceivedL();

        /**
        *
        */
        void LoadPluginL();

        /**
        *
        */
        void StopPluginL();

        /**
        *
        */
        MDesCArray& SessionParametersL();

         /**
        *
        */
        void ManualQueryL();
        
        
         /**
        *
        */
        TBool PossibleToLaunch();

        

    private: // from MMusAvaObserver

	// TBD: COMMENTS
	virtual void AvailabilityChanged( 
		TAvailabilityName aName, TAvailabilityStatus aStatus );
		
	virtual void AvailabilityError( 
		TAvailabilityName aName, TAvailabilityStatus aStatus );
	

    public:  //from MMusAvaSettingsObserver
    
        TApplicationState ApplicationState();
        
        TBool OptionAllowed();
    
    private: // from base class MMusIndicatorObserver.

        /**
        * From MMusIndicatorObserver.
        * Starts live video sharing.
        */
        virtual void StartLiveSharingL();

    private: /// helper functions

        /**
        * Cross-link status codes
        */
        MultimediaSharing::TMusAvailabilityStatus MapAvailabilities(
                                                TAvailabilityStatus aStatus );

        /**
        *
        */
        void UpdateSessionParametersL();

        /**
        *
        */
        HBufC* SipAddressesLC();

        /**
        *
        */
        HBufC* VideoCodecsLC();
        /**
        *
        */
        HBufC* TelNumberLC();

        /**
        *
        */
        HBufC* ContactNameLC();


        /**
        *
        */
        void IndicateAvailabilityL();
        
       

    private: // owned data

        /**
        *
        */
        CMusAvaInterface* iPlugin;

        /**
        *
        */
        MMusAvailabilityPluginManagerObserver& iObserver;

        /**
        *
        */
        CDesCArrayFlat* iSessionParameters;

        CMusIndicatorApi* iIndicator;
        
        CMusApplicationManager& iApplicationManager;    

        /**
        *
        */
        TBool iPluginStarted;
        MUS_UNITTEST( UT_CMusManagerServerCore )
    	MUS_UNITTEST( UT_CMusAvailabilityPluginManager )

    };

#endif // ? MUSAVAILABILITYPLUGINMANAGER_H

