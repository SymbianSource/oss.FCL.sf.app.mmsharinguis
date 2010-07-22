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


#ifndef __UT_MUSAVAAVAILABILITYSETTINGSOBSERVERIMP_H__
#define __UT_MUSAVAAVAILABILITYSETTINGSOBSERVERIMP_H__

//  INCLUDES
#include "musavaavailabilitysettingobserver.h"
#include "musavaobserver.h"

#include <e32base.h>
#include <e32std.h>

//class MMusAvaObserver;

/**
 *  This class Implements mandatory setting baste behavior which is 
 *  need for availability report.
 *
 *  CMusAvaAvailabilitySettingObserverImps is implementation class for madatory 
 *  setting which are needed for availability report. 
 *  class represent the interface of application.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

class CMusAvaAvailabilitySettingObserverImp : public CBase,
                                   public MMusAvaAvailabilitySettingObserver
	{
public:
    
	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return Returns pointer to CMusAvaAvailabilitySettingObserverImp object
     */
     
	static CMusAvaAvailabilitySettingObserverImp* NewL();
	
	/**
     * Destructor
     *
     * @since S60 v3.2
     * @param 
     * @return 
     */

	~CMusAvaAvailabilitySettingObserverImp();
	
protected:
	/**
     * Constructor
     *
     * @since S60 v3.2
     * @param aDefaultImp Pointer to observer interface 
     * @return 
     */

	CMusAvaAvailabilitySettingObserverImp( );
	
    /**
     * Perform the second phase construction of a
     *             CMusAvaAvailabilitySettingObserverImp object
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
     * @param aState Current availability State
     * @param aStatus Current availability Status
     * @param aName Availability name
     * @param aSystemError system wide error
     * @return 
     */
    void AvailabilityReport(
                            MMusAvaObserver::TAvailabilityState aState,
                            MMusAvaObserver::TAvailabilityStatus aStatus,
                            MMusAvaObserver::TAvailabilityName aName,
                            TInt aSystemError );
                            
                            
   /**
     * MusAvailability shutdown is completed
     *
     * @since  S60 v3.2
     * @param aState Current availability State
     * @param aName Availability name
     * @param aSystemError system wide error
     * @return      
     */
    void StopComplete( MMusAvaObserver::TAvailabilityState aState,
                               MMusAvaObserver::TAvailabilityName aName,
                               TInt aSystemError );
                               
    MMusAvaObserver::TAvailabilityState iState;
    MMusAvaObserver::TAvailabilityStatus iStatus;
    MMusAvaObserver::TAvailabilityName iName;
    TInt iSystemError;
    TBool iAvailabilityReport;
    TBool iStopComplete;
	};

#endif //__UT_MUSAVAAVAILABILITYSETTINGSOBSERVERIMP_H__

    