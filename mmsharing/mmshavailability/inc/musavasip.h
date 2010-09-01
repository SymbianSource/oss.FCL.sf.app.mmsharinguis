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
* Description:  This is client interface class for SIP-server 
*  Version     : %version: 23 % << Don't touch! Updated by Synergy at check-out.
*
*/


#ifndef __MUSAVASIP_H__
#define __MUSAVASIP_H__

//  INCLUDES

#include "musunittesting.h"

#include <e32base.h>
#include <sip.h>

class CMusAvaSipConnectionObserver;
class CMusAvaSipObserver;
class CMusAvaSipProfileRegistryObserver;
class CSIP;
class CSIPProfile;
class CSIPProfileRegistry;
class MMusAvaSipConnectionAdapter;
class MMusAvaSipProfileRegistryAdapter;
class MSIPConnectionObserver;
class MMusAvaSipAdapter;
class CSIPProfileRegistryBase;
class CMusAvaClientResolverUtil;

/**
 *  CMusAvaSip class represent the interface for module fetcher.
 *
 *  This interface class encapsulates module selection for availability Plug-in.
 *
 *  @lib musavailabilityplugin.lib

 */

class CMusAvaSip : public CBase
	{
public:
   
   	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aCMusAvaSharedObj Pointer to CMusAvaSharedObject
     * @return Returns pointer to CMusAvaSip object
     */
	static CMusAvaSip* NewL();
	
	/**
     * Destructor
     *

     * @param 
     * @return 
     */

	~CMusAvaSip();
	
	protected:
	
	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aCMusAvaSharedObj Pointer to CMusAvaSharedObject
     * @return 
     */

	CMusAvaSip();
	
    /**
     * Perform the second phase construction of a
     *             CMusAvaSip object
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	void ConstructL();
	
public:
    /**
     *  
     *
	 * @param 
     */
    void AddAdapterL( MMusAvaSipProfileRegistryAdapter& aAdapter );

    /**
     *  
     *
	 * @param 
     */
    void RemoveAdapter( MMusAvaSipProfileRegistryAdapter& aAdapter );	 

    /**
     *  
     *
	 * @param 
     */
    void AddAdapterL( MMusAvaSipConnectionAdapter& aAdapter, TInt aIndex = KErrNotFound );

	 /**
     * Returns CSip object 
     *
	 * @param 
     */
    void RemoveAdapter( MMusAvaSipConnectionAdapter& aAdapter );
	
    /**
     *  
     *
	 * @param 
     */
    void AddAdapterL( MMusAvaSipAdapter& aAdapter, TInt aIndex = KErrNotFound  );

	 /**
     * Returns CSip object. 
     *
	 * @param 
     */
    void RemoveAdapter( MMusAvaSipAdapter& aAdapter );

public:	 

	/**
     * Returns CSIPConnection object. 
     * Ownership is not transferred.
     * @param aForceCreation, ETrue if connection should be created
     * if it does not exists yet.
	 * @return referense to Sip Object.
     */
	CSIPConnection* ConnectionL( TBool aForceCreation = ETrue );

	 /**
     * Returns CSIPConnection object .
     * Ownership is not transferred.
     *
	 * @return referense to Sip Object.
     */
	MSIPConnectionObserver& ConnectionObserver();
    	
	 /**
     * Create Profile object.
     * 
	 * return EFalse if Profile was not created, 
	 * ETrue otherwise.
     */
     TBool CreateProfileL();
     
     /**
     * Create CSIPProfile object.
     * 
	 * return EFalse if CSIPProfile was not created, 
	 * ETrue otherwise.
     */
     TBool CreateSipProfile();
    	
	 /**
     * Returns CSIPProfile object.
     * Ownership is not transferred.
     *
     * @since  S60 v3.2
	 * @return referense to Sip Object.
     */
    CSIPProfile* Profile();

	 /**
     * Returns CSIPProfileRegistry object.
     * Ownership is not transferred.
     *
	 * @return reference to Sip Object.
     */
    CSIPProfileRegistry& ProfileRegistryL();
	
	 /**
     * Returns a buffer containing the own domain, if profile registered.
     * Ownership is transferred.
     *
	 * @return Pointer to a buffer containing the own domain name.
     */
    HBufC* OwnDomainLC();
    
    /**
     * Returns CSip object 
     * Ownership is not transferred.
     *
	 * @return reference to Sip Object.
     */
    CSIP& Sip();
    
    /**
     * Returns SipProfileId 
     *
	 * @param aSipProfileId on return will contain SipProfileId value
     * @return KErrNotFound if parameter was not found, KErrNone otherwise.
     */
    TInt SipProfileId( TUint32& aSipProfileId );
    
    /**
     * Returns a pointer to utility object that handles special client resolving 
     * in __VOIP enabled terminals. In __VOIP disabled terminals this will 
     * return a NULL pointer. This function does not transfer ownership.
     *
     * @return Pointer to resolver utility object. Make sure to be prepared 
     *         that this pointer is NULL.
     */
    CMusAvaClientResolverUtil* ClientResolverUtil() const;
    
    /**
    * Deletes unnecessary CSIPConnection   
    */
    void DeleteSIPConnection();


private:
	
	 /**
     * Create SIPConnection object.
     * 
     */  
	void CreateSIPConnectionL();
        
    /**
     * Gets profile to be used with Mus.
     *
     * @param Reference to used profile registry.
     * @return Pointer to profile meant to be used with Mus. If not possible,
     *         returns poiner to default profile.  Ownership is transferred.
     * @leave KErrNotFound if not able to return even default profile.
     */		
    CSIPProfile* GetMusProfileL( CSIPProfileRegistryBase& aRegistry );
    
    
private:
        
     /**
     * Pointer to CSIP object.
     * Owns.
     */
	 CSIP* iSip;
	 
     /**
      * 
      * Owns.
      */
	 CSIPConnection* iSipConnection;
	 
     /**
      *
      * Owns.
      */
     CSIPProfile* iSipProfile;
     
     /**
      *
      * Owns.
      */
     CSIPProfileRegistry* iSipProfileRegistry;

     /**
     * Pointer to CMusAvaSipObserver object.
     * Owns.
     */
     CMusAvaSipObserver* iSipObserver;
     
     /**
     * Pointer to CMusAvaSipConnectionObserver object
     * Owns.
     */
     CMusAvaSipConnectionObserver* iSipConnectionObserver;
     
     /**
     * Pointer to CMusAvaSipProfileRegistryObserver object
     * Owns.
     */
     CMusAvaSipProfileRegistryObserver* iSipProfileRegistryObserver;
     
     /**
     * Pointer to utility object that handles special client resolving in
     * __VOIP enabled terminals. In __VOIP disabled terminals creation of
     * this object will fail because of missing UID in CenRep and thus this 
     * pointer can be NULL. If this object does not exist, standard client
     * resolving will be used.
     */
     CMusAvaClientResolverUtil* iClientResolverUtil;
     
     TUint32 iIapId;
     
     MUS_UNITTEST( UT_CMusAvaOptionHandler )
     MUS_UNITTEST( UT_CMusAvaSip )
     MUS_UNITTEST( UT_CMusAvaRegisterAvailability )
     MUS_UNITTEST( UT_CMusAvaConnectionMonitor )
     MUS_UNITTEST( UT_CMusAvaCapabilitySipAgent )
     MUS_UNITTEST( UT_CMusAvaCapabilityExchange )
	};

#endif

