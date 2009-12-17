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
* Description:  This is client interface class for chared objects 
*                MusAvailability Plug-in 
*
*/


#ifndef __MUSAVASHAREDOBJ_H__
#define __MUSAVASHAREDOBJ_H__

//  INCLUDES
#include "musunittesting.h"
#include <e32base.h>

class CSIP;
class CMusAvaSipObserver;
class MSIPObserver;
class CMusAvaSip;
class MSIPConnectionObserver;
class MMusAvaSipConnectionAdapter;
class CMusAvaConnectionMonitor;
class CMusAvaNetworkRegistrationStatus;
class CMusAvaNetworkModeStatus;

/**
 *  CMusAvaSharedObject class represent the interface for module fetcher.
 *
 *  This interface class encapsulates module selection for availability Plug-in.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaSharedObject : public CBase
	{
    public:
        
        /**
         * Returns singleton object 
         *
         * @since  S60 v3.2
    	 * @return pointer to CMusAvaSharedObject Object
         */
        static CMusAvaSharedObject* GetSingletonL();
        
         /**
         * Delete singleton object 
         *
         * @since  S60 v3.2
         */
        static void DeleteSingleton();
    	
    	
    protected:
    	
    	/**
         * Two-phased constructor
         *
         * @since S60 v3.2
         * @return Returns pointer to CMusAvaSharedObject object
         */
    	static CMusAvaSharedObject* NewL(  );
    	
    	/**
         * Constructor
         *
         * @since S60 v3.2
         * @param aObserver Pointer to observer intercase 
         */
    	CMusAvaSharedObject( );
    	
        /**
         * Perform the second phase construction of a
         *             CMusAvaSharedObject object
         *
         * @since S60 v3.2
         */
    	void ConstructL();
    	
    private:
    
 	    /**
         * Destructor
         *
         * @since S60 v3.2
         */
    	~CMusAvaSharedObject();
    	
    	
    public:

         /**
         * Returns CMusAvaConnectionMonitor object 
         *
         * @since  S60 v3.2
    	 * @return CMusAvaConnectionMonitor to Sip Object
         */
         CMusAvaConnectionMonitor& ConnectionMonitor();
        
        /**
         * Returns MSIPConnectionObserver object 
         *
         * @since  S60 v3.2
    	 * @return referense to MSIPConnectionObserver
         */
    	MSIPConnectionObserver& SIPConnection();
    	
    	/**
         * Returns CMusAvaSip object 
         *
         * @since  S60 v3.2
    	 * @return referense to Sip Object
         */
         CMusAvaSip& MusAvaSip();
        
    	/**
         * Returns CSIP object 
         *
         * @since  S60 v3.2
    	 * @return referense to Sip Object
         */
         CSIP& Sip();
         
        /**
         * Returns MSIPObserver object 
         *
         * @since  S60 v3.2
    	 * @return rreference to MSIPObserver object
         */
        MSIPObserver& SIPObserver();
      
        /**
         * Returns CMusAvaNetworkRegistrationStatus object 
         *
         * @since  S60 v3.2
    	 * @return rreference to CMusAvaNetworkRegistrationStatus object
         */
        CMusAvaNetworkRegistrationStatus& MusAvaTelephonyStatus();
        
        /**
         * Returns CMusAvaNetworkModeStatus object 
         *
         * @since  S60 v3.2
    	 * @return rreference to CMusAvaNetworkModeStatus object
         */

        CMusAvaNetworkModeStatus& NetworkModeStatus();

      
    private: //Data
            
         /**
         * Pointer to Singleton object
         */
         static CMusAvaSharedObject* iSelf;
         
         /**
         *
         */
         static TInt iSelfCounter;
         
         /**
         * Pointer to CMusAvaSip object
         */
         CMusAvaSip* iMusAvaSip;  
         
         /**
         * Pointer to CMusAvaConnectionMonitor object
         */
         CMusAvaConnectionMonitor* iConnectionMonitor;
         
         /**
         * Pointer to CMusAvaNetworkRegistrationStatus object
         */
         CMusAvaNetworkRegistrationStatus* iNetworkRegistrationStatus;
         
         CMusAvaNetworkModeStatus* iNetworkModeStatus;  
          
         MUS_UNITTEST( UT_CMusAvaSharedObject )
         MUS_UNITTEST( UT_CMusAvaConnectionMonitor )
                    
	};

#endif

