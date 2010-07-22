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
* Description:  Provide interface for the client requestin availability class.
*
*/


#ifndef __MUSAVATELEPHONYSTATUSBASE_H__
#define __MUSAVATELEPHONYSTATUSBASE_H__

#include "musunittesting.h"

#include <e32def.h>
#include <e32base.h>
#include <e32property.h>
#include <etel.h>
#include <etelmm.h>

class MMusAvaTelephonyStatusObserver;

/**
 *  This class monitors the phone status and resolves the contact 
 *  information of the remote host in case of a connected cs call. 
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaTelephonyStatusBase : public CActive 
	{
public:
   
    /**
     * Default destructor
     *
     * @since S60 v3.2
     */   
    ~CMusAvaTelephonyStatusBase();   
    
public: 
    
    /**
     *  
     *
     * @since  S60 v3.2
	 * @param 
     */
    void AddAdapterL( MMusAvaTelephonyStatusObserver& aAdapter );
      
     /**
     *  
     *
     * @since  S60 v3.2
	 * @param 
     */
    void RemoveAdapter( MMusAvaTelephonyStatusObserver& aAdapter );

    
    /**
     *  
     *
     * @since  S60 v3.2
	 * @param 
     */
    void virtual TelephonyStatus();
      
protected: // from CActive
    
    /**
     * descr
     *
     * @since S60 v3.2
     */   
    void RunL();
    
    /**
     * descr
     *
     * @since S60 v3.2
     */   
    void DoCancel();
    
    /**
     * descr
     *
     * @since S60 v3.2
     */   
    TInt RunError( TInt aError );
    
protected:

    /**
     * Constructor.
     *
     * @since S60 v3.2
     */   
    CMusAvaTelephonyStatusBase();

    
protected: // data

    /**
     * Array of MMusAvaConnectionMonitorObserver the callback interfaces
     */
    RPointerArray<MMusAvaTelephonyStatusObserver> iObserver;  
    
    /**
    * Reference to RMobilePhone object
    */
    RMobilePhone iPhone;
    
    /**
    * Reference to RTelServer object
    */
    RTelServer iTelServer;
    
    
    MUS_UNITTEST( UT_CMusAvaTelephonyStatusBase )
    MUS_UNITTEST( UT_CMusAvaConnectionAvailability )
    };


#endif // __MUSAVATELEPHONYSTATUSBASE_H__