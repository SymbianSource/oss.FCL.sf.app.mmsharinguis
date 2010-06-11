/*
* Copyright (c) 2007-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides information about ongoing calls.
*
*/


#ifndef C_CALLINFORMATION_H
#define C_CALLINFORMATION_H

#include <e32base.h>
#include "callinformation.h"
#include "mcall.h"


// forward declarations 
class MCallInformationObserver;
class CCallInformationImpl;
class CCallInfoIter;


/**
 *  Class to be used for receiving information about ongoing calls. 
 *
 *  @code
 *  // create the CCallInformation instance e.g. when the client component is created
 *  iCallInfo = CCallInformation::NewL(); 
 *  iCallInfo->NotifyCallInformationChanges(*iObserver);
 *  
 *  // and use it when needed
 *  CCallInfoIter& iter = iCallInfo->GetCallsL();    
 *  @endcode
 *
 *  @lib telephonyservice.lib
 *  @since S60 v5.1 
 */
NONSHARABLE_CLASS( CCallInformation ) : public CBase
    {

public:

    /**
     * Two-phased constructor.          
     */
    IMPORT_C static CCallInformation* NewL();
    

    /**
    * Destructor.
    */
    virtual ~CCallInformation();

    /**
     * Allows a client to be notified when there are changes in current calls.
     * There can be only one observer per CCallInformation instance.
     *
     * @since S60 v5.1 
     * @param aObserver Observer for notifications.
     */
     IMPORT_C void NotifyCallInformationChanges( MCallInformationObserver& aObserver );
    
    /**
     * Used to cancel previously placed NotifyCallInformationChanges call.
     *
     * @since S60 v5.1      
     */
     IMPORT_C void CancelNotification();
     
     /**
     * Gets the snapshot of ongoing calls.
     *
     * @since S60 v5.1      
     * @return Ongoing calls.
     */
     IMPORT_C CCallInfoIter& GetCallsL( );
			
	 /**
     * Get calls by given state. 
     *
     * @since S60 v5.1 
     * @param aState Specifies a call state that is used as a search key.
     * @return Calls in state aSate, e.g. in case of conference call several 
     *         calls have the same state.
     */     
     IMPORT_C CCallInfoIter& GetCallsL( CCPCall::TCallState aState );   
     
     
private:

    CCallInformation();

    void ConstructL();


private: // data    
    
	CCallInformationImpl* iImpl;	
	
	// Helper Data Needed by STUB Implemenation.
	CCallInfoIter* iInfoIter;
	CCallInfos* iInfos;
    };


#endif // C_CALLINFORMATION_H
