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
* Description:  Provide interface for the client requestin network availability.
*
*/


#ifndef __MUSAVACONNECTIONMONITOR_H__
#define __MUSAVACONNECTIONMONITOR_H__

#include "musavaavailability.h"
#include "musunittesting.h"

#include <e32base.h>
#include <rconnmon.h>
#include <sipprofileregistryobserver.h>


class MMusAvaAvailabilityObserver;
class MMusAvaObserver;
class CMusAvaSharedObject;
class CMusAvaPhoneStatus;
class CConnMonEventBase;
class MMusAvaConnectionMonitorObserver;

/**
 *  Implemets register vailability. 
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

 

class CMusAvaConnectionMonitor : public CBase,
                                 public MConnectionMonitorObserver
	{
	public:
	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aSharedObject reference to CMusAvaSharedObject
     * @return Returns pointer to CMusAvaInterface object
     */
     
	static CMusAvaConnectionMonitor* NewL( CMusAvaSharedObject& aSharedObject );
	
	/**
     * Destructor
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

    ~CMusAvaConnectionMonitor();
	
	protected:
	
	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aSharedObject reference to CMusAvaSharedObject
     * @return 
     */

	CMusAvaConnectionMonitor( CMusAvaSharedObject& aSharedObject );
	
	                                        
	
	/**
     * Perform the second phase construction of a
     *             CMusAvaConnectionMonitor object
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	void ConstructL();
	
public: // from MConnectionMonitorObserver 
	
	/**
	 * Event from connection monitor server
	 *
	 */   
    void EventL(const CConnMonEventBase& aConnMonEvent); 
    
public:

    /**
     *  
     *
     * @since  S60 v3.2
	 * @param 
     */
    void AddAdapterL( MMusAvaConnectionMonitorObserver& aAdapter );
	  
    /**
	 * Test Current bearer type 
	 *
	 * @since S60 v3.2
     * @return ETrue if bearer type is activated, 
     * EFalse otherwise
	 */
    TBool BearerType( TInt aBearer  = EBearerWCDMA );
    

	/**
	 * Created connection ID for multimedia sharing is returned 
	 *
	 * @since S60 v3.2
     * @return KErrNone if parameter was not found, 
     * Connection ID otherwise
	 */
	TUint CreatedConnectionID();
   

	/**
	 * Store connection ID  
	 *
	 * @since S60 v3.2
	 */
	void SetConnectionID( TUint aConnectionID );
   
     /**
	 * Active connection ID for multimedia sharing is returned. 
	 *
	 * @since S60 v3.2
     * @return KErrNotFound if parameter was not found, 
     * Connection ID otherwise
	 */
    TUint ConnectionIDL();
  
    /**
     *  
     *
     * @since  S60 v3.2
	 * @param 
     */
     TBool KillPdpContext();
      
     /**
     *  
     *
     * @since  S60 v3.2
	 * @param 
     */
    void RemoveAdapter( MMusAvaConnectionMonitorObserver& aAdapter );
    
    /**
     *  
     *
     * @since  S60 v3.2
	 * @param 
     */
    void NotifyEventL();
    
    /**
	 * Active connection count for multimedia sharing is returned. 
	 *
	 * @since S60 v3.2
     * @return KErrNotFound if parameter was not found, 
     * Connection count otherwise
	 */
	 
    TUint ConnectionCount();
   
	 /**
	 * Active Access Point ID for multimedia sharing is returned 
	 *
	 * @since S60 v3.2
     * @return KErrNotFound if parameter was not found, 
     * Access Point ID otherwise
	 */
    TUint MultimediaSharingAccessPointID();
    
private:
    
    
    /**
     * Session to connection monitor server
     */ 
    RConnectionMonitor iConnectionMonitor;
     
     /**
     * Array of MMusAvaConnectionMonitorObserver instances
     */
    RPointerArray<MMusAvaConnectionMonitorObserver> iConnectionMonitorObserver;
    
     /**
     * Pointer to CMusAvaSharedObject
     */ 
    CMusAvaSharedObject* iSharedObj;
    
     /**
     * Connection ID
     */
    TUint iConnectionID;
    
    /**
     * Profile ID
     */
    TUint32 iProfileId;
    
    MUS_UNITTEST( UT_CMusAvaConnectionMonitor )
    MUS_UNITTEST( UT_CMusAvaConnectionAvailability )
    };


#endif // __MUSAVACONNECTIONMONITOR_H__
