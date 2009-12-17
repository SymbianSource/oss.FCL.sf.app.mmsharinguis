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
*
*/


#ifndef __MUSAVASIPSTUB_H__
#define __MUSAVASIPSTUB_H__

//  INCLUDES
#include <e32base.h>
#include <sip.h>

class CMusAvaSharedObject;
class CSIP;
class CMusAvaSipObserver;
class MSIPObserver;
class MSIPConnectionObserver;
class CMusAvaSipConnectionObserver;

/**
 *  CMusAvaSIPStub class represent the interface for module fetcher.
 *
 *  This interface class encapsulates module selection for availability Plug-in.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class CMusAvaSIPStub : public CBase
	{
public:
    
   
   	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aCMusAvaSharedObj Pointer to CMusAvaSharedObject
     * @return Returns pointer to CMusAvaSIPStub object
     */
	static CMusAvaSIPStub* NewL( CMusAvaSharedObject& aMusAvaSharedObject  );
	
	/**
     * Destructor
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	virtual ~CMusAvaSIPStub();
	
	protected:
	
	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aCMusAvaSharedObj Pointer to CMusAvaSharedObject
     * @return 
     */

	CMusAvaSIPStub( CMusAvaSharedObject& aMusAvaSharedObject );
	
    /**
     * Perform the second phase construction of a
     *             CMusAvaSIPStub object
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	void ConstructL();
	
	public:
	
	/**
     * Returns MSIPConnectionObserver object 
     *
     * @since  S60 v3.2
	 * @param 
	 * @return referense to MSIPConnectionObserver
     */
	MSIPConnectionObserver& SIPConnection();
	
	 /**
     * Returns CSip object 
     *
     * @since  S60 v3.2
	 * @param 
	 * @return referense to Sip Object
     */
     CSIP& Sip();
     
    /**
     * Returns MSIPObserver object 
     *
     * @since  S60 v3.2
	 * @param 
	 * @return rreference to MSIPObserver object
     */
    MSIPObserver& SIPObserver();
  
   private:
        
     /**
     * Pointer to CMusAvaSharedObject object
     */
     CMusAvaSharedObject* iMusAvaSharedObject;
    
     /**
     * Pointer to CSIP object
     */
	 CSIP* iSip;
	 
	 /**
     * SIP reference counter
     */
	 TInt iSipRefCounter;
     
     /**
     * Pointer to CMusAvaSipObserver object
     */
     CMusAvaSipObserver* iSipObserver;
     
     /**
     * Pointer to CMusAvaSipConnectionObserver object
     */
     CMusAvaSipConnectionObserver* iSipConnectionObserver;
    
	};

#endif

