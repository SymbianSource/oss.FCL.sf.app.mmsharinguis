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
* Description:  This is implementation class of availability setting class. 
*
*/


#ifndef __MUSAVATELEPHONYSTATUSOBSERVERIMP_H__
#define __MUSAVATELEPHONYSTATUSOBSERVERIMP_H__

//  INCLUDES
#include "musavatelephonystatusobserver.h"

#include <e32base.h>
#include <e32std.h>
#include <etelmm.h>


/**
 *  This class Implements MMusAvaTelephonyStatusObserver.
 *
 *  MMusAvaTelephonyStatusObserver is implementation class 
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class CMusAvaTelephonyStatusObserverImp : public CBase,
                                          public MMusAvaTelephonyStatusObserver
	{
public:
    
	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return Returns pointer to CMusAvaTelephonyStatusObserverImp object
     */
     
	static CMusAvaTelephonyStatusObserverImp* NewL();
	
	/**
     * Destructor
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	~CMusAvaTelephonyStatusObserverImp();
	
protected:
	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return 
     */

	CMusAvaTelephonyStatusObserverImp( );
	
    /**
     * Perform the second phase construction of a
     *             CMusAvaTelephonyStatusObserverImp object
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	void ConstructL();
	
                           
// from base class MMusAvaTelephonyStatusObserver

public:	

    /**
     * Current Network Mode Status
     *
     * @since  S60 v3.2
     * @return 
     */
    void PhoneNetworkModeStatus( 
                            RMobilePhone::TMobilePhoneNetworkMode aStatus );
    
   /**
     * Current Network Registration Status
     *
     * @since  S60 v3.2
     * @return 
     */
    void NetworkRegistrationStatus( 
                RMobilePhone::TMobilePhoneRegistrationStatus aRegStatus );
                
 public: //Data
 
    RMobilePhone::TMobilePhoneRegistrationStatus iPhoneRegistrationStatus;
    RMobilePhone::TMobilePhoneRegistrationStatus* iPhoneRegistrationStatusPointer;
    RMobilePhone::TMobilePhoneNetworkMode iPhoneNetworkMode;
    
    
	};

#endif

