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
* Description:  This observer is used by
*                ECOM clients to get function call from the availability Plug-in
*
*/



#ifndef __MUSAVAAVAILABILITYOBSERVERIMP_H__
#define __MUSAVAAVAILABILITYOBSERVERIMP_H__

#include "musavaavailabilityobserver.h"
//#include "musavaobserver.h"
#include "musunittesting.h"
#include <e32base.h>
#include <e32def.h>

#ifdef MUS_UNITTEST
#undef IMPORT_C
#define IMPORT_C
#endif

class CMusAvaController;
class MMusAvaObserver;
/**
 *  MusAvailability Observer base class 
 *
 *  Observer interface for ECOM clients.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class CMusAvaAvailabilityObserverImp :  public CBase, 
                            public MMusAvaAvailabilityObserver
                                
	{
public:
   
     /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param 
     * @return Returns pointer to CMusAvaController object
     */
     
	static CMusAvaAvailabilityObserverImp* NewL(  );
	
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
     * @return 
     */

	CMusAvaAvailabilityObserverImp( );
	
    /**
     * Perform the second phase construction of a
     *             CMusAvaController object
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	void ConstructL();
	
// from base class MMusAvaAvailabilityObserver

public:

     /**
     * Availability report
     *
     * @since  S60 v3.2
     * @param aStatus New status for availability.
     */
    virtual void AvailabilityChanged(
        MMusAvaObserver::TAvailabilityName aName,
        MMusAvaObserver::TAvailabilityStatus aStatus );

    virtual void AvailabilityError(
        MMusAvaObserver::TAvailabilityName aName,
        MMusAvaObserver::TAvailabilityStatus aStatus );
        
    virtual TBool Available( 
        MMusAvaObserver::TAvailabilityName aAvailability );
        
        
    virtual MMusAvaObserver::TAvailabilityStatus AvailabilityPluginState();
   

public:

    MMusAvaObserver::TAvailabilityStatus iStatus;
    MMusAvaObserver::TAvailabilityName iName;
    MUS_UNITTEST ( UT_CMusAvaDefaultImp )
    };


#endif // __MUSAVAAVAILABILITYOBSERVERIMP_H__

