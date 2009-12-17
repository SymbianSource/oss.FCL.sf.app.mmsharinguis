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
* Description:  Provide interface for the client requestin availability class.
*
*/


#ifndef __MUSAVANETWORKREGISTRATIONSTATUS_H__
#define __MUSAVANETWORKREGISTRATIONSTATUS_H__

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
class CMusAvaNetworkRegistrationStatus : public CMusAvaTelephonyStatusBase 
	{
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     */   
	static CMusAvaNetworkRegistrationStatus* NewL( );

   
    /**
     * Default destructor
     *
     * @since S60 v3.2
     */   
    ~CMusAvaNetworkRegistrationStatus();   
    
public: 
    
    /**
	 * Current network registration status 
	 *
	 * @since S60 v3.2
     * @return TMobilePhoneRegistrationStatus 
     * EFalse otherwise
	 */
    void NetworkRegistration(
        RMobilePhone::TMobilePhoneRegistrationStatus& aStatus );
              
protected:

    /**
     * Constructor.
     *
     * @since S60 v3.2
     */   
    CMusAvaNetworkRegistrationStatus();

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
     *
     * @since  S60 v3.2
	 * @param 
     */
    void NetworkRegistrationStatus( 
            RMobilePhone::TMobilePhoneRegistrationStatus aRegStatus );
    
    /**
     * descr
     *
     * @since S60 v3.2
     */   
    void RunL();
    
    
private: // data

    
    /**
    * Reference to TMobilePhoneRegistrationStatus object
    */
    RMobilePhone::TMobilePhoneRegistrationStatus iRegistrationStatus;
    
    
    MUS_UNITTEST( UT_CMusAvaNetworkRegistrationStatus )
    MUS_UNITTEST( UT_CMusAvaConnectionAvailability )
    };


#endif // __MUSAVANETWORKREGISTRATIONSTATUS_H__