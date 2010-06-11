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
* Description:  Provide interface for the client requestin network availability.
*
*/


#ifndef __MUSAVAAVAILABILITYSTUB_H__
#define __MUSAVAAVAILABILITYSTUB_H__

#include "musavaavailability.h"

#include <e32base.h>

class MMusAvaAvailabilityObserver;
class MMusAvaObserver;
class CMusAvaSharedObject;
class CMusAvaSettingsImp;

/**
 *  Implemets register vailability. 
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */

 

class CMusAvaAvailabilityStub : public CMusAvaAvailability
	{
	public:
    
    /**
     * Two-phased constructor.
     *
     * @param aObserver Pointer to observer interface.
     * @param aSettings Reference to settings object.
     * @return Returns pointer to CMusAvaInterface object.
     */
	static CMusAvaAvailabilityStub* NewL(
	                                MMusAvaAvailabilityObserver& aObserver,
                                    CMusAvaSettingsImp& aConcreteSettings );

	/**
     * Two-phased constructor.
     *
     * @param aObserver Pointer to observer interface.
     * @param aSettings Reference to settings object.
     * @return Returns pointer to CMusAvaInterface object.
     */
	static CMusAvaAvailabilityStub* NewLC(
	                                MMusAvaAvailabilityObserver& aObserver,
                                    CMusAvaSettingsImp& aConcreteSettings );

	/**
     * Destructor
     */
	~CMusAvaAvailabilityStub();


protected:

	CMusAvaAvailabilityStub( MMusAvaAvailabilityObserver& aObserver,
                                CMusAvaSettingsImp& aConcreteSettings );

	void ConstructL();
	

	
// from base class CMusAvaAvailability	
	
public: 
    
     /**
     * Executes for the availability.
     */
    virtual void DoExecuteL();

    /**
     * From CMusAvaAvailability.
     * Returns availability name.
     *
     * @return Name of *this* availability.
     */
    virtual MMusAvaObserver::TAvailabilityName Name();
    
    virtual void PrepareForReceivedInviteL();

public:

    TInt iExecuteError;
    TBool iExecuteLeave;
    MMusAvaObserver::TAvailabilityStatus iStatusStub;
    MMusAvaObserver::TAvailabilityName iNameStub;
    TBool iPrepareForInviteCalled;
	
private:
    
	 /**
     * Pointer to CMusAvaSharedObject object
     */
	 CMusAvaSharedObject* iSharedObj; 
	 
	 /**
     * Pointer to CMusAvaSettingsImp object
     */
	 CMusAvaSettingsImp& iSettings;
    };


#endif // __MUSAVANETWORKAVAILABILITY_H__
