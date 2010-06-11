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


#ifndef __MUSAVANETWORKMODESTATUS_H__
#define __MUSAVANETWORKMODESTATUS_H__

#include "musavatelephonystatusbase.h"
#include "musunittesting.h"

#include <e32def.h>
#include <e32base.h>
#include <e32property.h>
#include <etel.h>
#include <etelmm.h>


/**
 *  This class monitors the phone status and resolves the contact 
 *  information of the remote host in case of a connected cs call. 
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaNetworkModeStatus : public CMusAvaTelephonyStatusBase 
	{
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     */   
	static CMusAvaNetworkModeStatus* NewL( );

   
    /**
     * Default destructor
     *
     * @since S60 v3.2
     */   
    ~CMusAvaNetworkModeStatus();   
    
public: 
    
    /**
	 * Test current network mode 
	 *
	 * @since S60 v3.2
     * @return Returns the network mode      
	 */    
    RMobilePhone::TMobilePhoneNetworkMode PhoneNetworkMode( );
              
protected:

    /**
     * Constructor.
     *
     * @since S60 v3.2
     */   
    CMusAvaNetworkModeStatus();

    /**
     * Second-phase constructor
     *
     * @since S60 v3.2
     */   
    void ConstructL();
    
public: // from CMusAvaTelephonyStatusBase   
    
    /**
     *  
     *
     * @since  S60 v3.2
	 * @param 
     */
    void TelephonyStatus();
    
protected:
    
    /**
	 *
	 * @since S60 v3.2
     *   
	 */   
    void PhoneNetworkModeStatus( RMobilePhone::TMobilePhoneNetworkMode aStatus);
    
protected:  // from CActive

    void RunL();
    
    void DoCancel();
    
private: // data

    
   /**
    * Reference to TMobilePhoneNetworkMode object
    */
    RMobilePhone::TMobilePhoneNetworkMode iNetworkMode;
    
    
    MUS_UNITTEST( UT_CMusAvaNetworkModeStatus )
    };


#endif // __MUSAVANETWORKMODESTATUS_H__