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


#ifndef __MUSAVAAVAILABILITYOBSERVERIMP_H__
#define __MUSAVAAVAILABILITYOBSERVERIMP_H__

//  INCLUDES
#include "musavaavailabilityobserver.h"

#include <e32base.h>
#include <e32std.h>

class MMusAvaObserver;

/**
 *  This class Implements mandatory setting baste behavior which is 
 *  need for availability report.
 *
 *  CMusAvaAvailabilityObserverImps is implementation class for madatory 
 *  setting which are needed for availability report. 
 *  class represent the interface of application.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class CMusAvaAvailabilityObserverImp : public CBase,
                                       public MMusAvaAvailabilityObserver
	{
public:
    
	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return Returns pointer to CMusAvaAvailabilityObserverImp object
     */
     
	static CMusAvaAvailabilityObserverImp* NewL();
	
	/**
     * Destructor
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	~CMusAvaAvailabilityObserverImp();
	
protected:
	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return 
     */

	CMusAvaAvailabilityObserverImp( );
	
    /**
     * Perform the second phase construction of a
     *             CMusAvaAvailabilityObserverImp object
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	void ConstructL();
	
                           
// from base class MMusAvaAvailabilitySettingObserver

public:	

    /**
     * Availability report
     *
     * @since  S60 v3.2
     * @param aName Availability name
     * @param aStatus New status for availability.
     */
    virtual void AvailabilityChanged(
        MMusAvaObserver::TAvailabilityName aName,
        MMusAvaObserver::TAvailabilityStatus aStatus );
	
	/**
     * Availability report
     *
     * @since  S60 v3.2
     * @param aName Availability name
     * @param aStatus New status for availability.
     */
    virtual void AvailabilityError(
        MMusAvaObserver::TAvailabilityName aName,
        MMusAvaObserver::TAvailabilityStatus aStatus );
        
    virtual TBool Available( 
        MMusAvaObserver::TAvailabilityName aAvailability );
        
        
    virtual MMusAvaObserver::TAvailabilityStatus AvailabilityPluginState();
    
    
    virtual void AvailabilitiesAbleToShowIndicator();

     
     /**
     * Clear atributes
     *
     * @since  S60 v3.2
     * @param 
     * @param 
     * @return      
     */                        
    void Clear();
    
    
    MMusAvaObserver::TAvailabilityStatus iStatus;
    MMusAvaObserver::TAvailabilityName iName;
    TInt iError;
	};

#endif

