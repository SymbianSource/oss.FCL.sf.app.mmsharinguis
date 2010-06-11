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


#ifndef __MUSAVACONNECTIONMONITOROBSERVERIMP_H__
#define __MUSAVACONNECTIONMONITOROBSERVERIMP_H__

//  INCLUDES
#include "musavaconnectionmonitorobserver.h"

#include <e32base.h>
#include <e32std.h>

class CConnMonEventBase;

/**
 *  This class Implements mandatory setting baste behavior which is 
 *  need for availability report.
 *
 *  CMMusAvaConnectionMonitorObserverImps is implementation class for madatory 
 *  setting which are needed for availability report. 
 *  class represent the interface of application.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class CMMusAvaConnectionMonitorObserverImp : public CBase,
                                       public MMusAvaConnectionMonitorObserver
	{
public:
    
	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return Returns pointer to CMMusAvaConnectionMonitorObserverImp object
     */
     
	static CMMusAvaConnectionMonitorObserverImp* NewL();
	
	/**
     * Destructor
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	~CMMusAvaConnectionMonitorObserverImp();
	
protected:
	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return 
     */

	CMMusAvaConnectionMonitorObserverImp( );
	
    /**
     * Perform the second phase construction of a
     *             CMMusAvaConnectionMonitorObserverImp object
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	void ConstructL();
	
                           
// from base class MMusAvaConnectionMonitorObserver

public:	

    /**
	 * Event from connection monitor server
	 *
	 */
    virtual void EventL(const CConnMonEventBase& aConnMonEvent); 
    
public: //Data
    
    CConnMonEventBase* iConnMonEventBase;
	};

#endif

