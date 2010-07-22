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
* Description:  ?Description
*
*/


#ifndef MUSAPPLICATIONMANAGER_H
#define MUSAPPLICATIONMANAGER_H

#include "musmanagercommon.h"
#include "musunittesting.h"

#include <apgcli.h>
#include <apgtask.h>
#include <e32base.h>
#include <w32std.h>


const TInt KMusMgrMaxPluginNameLen = 256;

//class TApaTask;

/**
 *  Multimedia Sharing application handler
 *
 *  Handles application launch and close functionality. Also
 *  but application to foreground if all ready running.
 *
 *  @lib none
 */
class CMusApplicationManager : public CBase
    {
    MUS_UNITTEST( UT_CMusApplicationManager )
		
	public: // first phase constructors & destructor
	
		/**
		*
		*/
	    static CMusApplicationManager* NewL();
	
		/**
		*
		*/
	    static CMusApplicationManager* NewLC();
	
		/**
		*
		*/
	    ~CMusApplicationManager();
	

    protected: // second phase constructors
	
		/**
		*
		*/
	    void ConstructL();
	
		/**
		*
		*/
	    CMusApplicationManager();
	

	public: // new public functions
	
		/**
		* @return TBool ETrue if application is runing ( MultimediaSharing )
		*/
	    TBool ApplicationRunning();
	
		/**
		*
		*/
	    void StartApplicationL();
	
		/**
		*
		*/
	    void StopApplicationL();
	
		/**
		* Brings application to foreground if running all reaydy.
		*/
	    void ShowApplicationL();

		/**
		* Writes given array and other parameter spefic date to Publish & 
		* Subscripe database.
		*/
	    void WriteSessionPropertiesL( 
	    								MultimediaSharing::TMusUseCase aUseCase,
	                    MultimediaSharing::TMusAvailabilityStatus aStatus,
	                    MDesCArray& aSessionParameters );
	
		/**
		* Overrided version whitout use case.
		*
		*/
	    void WriteSessionPropertiesL( 
	    								MultimediaSharing::TMusAvailabilityStatus aStatus,
	                    MDesCArray& aSessionParameters );
	
		/**
		*
		*/
	    void SetStatusL( MultimediaSharing::TMusAvailabilityStatus aStatus );
	    
	    /**
		*
		*/
	    void SetIndicatorStatusL( TBool aLaunch );
        
        /**
         * Resolves name of the ecom plugin which implements the livecomms engine api
         * by using the callprovider
         */
        void ResolvePluginNameL(TDes8& aPluginName);
	

	private: // helper functions
	
		/**
		*
		*/
	    void SetPropertyL( TUint aProperty, const TDesC& aValue );
	
		/**
		*
		*/
	    void SetPropertyL( TUint aProperty, TInt aValue );
	    
	    /**
        * Reads the call provider information from PS key.
        */
	    void CallProviderL(TDes8& aProvider);
	    
	    /**
	    * Creates command line used when starting application.
	    * 
	    */
	    HBufC* CreateCommandLineArgsLC();
    };


#endif // ? MUSAPPLICATIONMANAGER_H
