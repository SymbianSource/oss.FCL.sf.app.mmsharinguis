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
* Description:  Provide interface for the client requestin register availability.
*
*/



#ifndef __MUSAVASIPPROFILEAVAILABILITY_H__
#define __MUSAVASIPPROFILEAVAILABILITY_H__


#include "musunittesting.h"
#include "musavaavailability.h"
#include "musavasipconnectionadapter.h"
#include "musavasipprofileregistryadapter.h"
#include <sipprofileregistryobserver.h>
#include <e32base.h>


class CMusAvaSettingsImp;
class CSIPProfileRegistry;
class CSIPProfile;
class MMusAvaObserver;
class CMusAvaSharedObject;
class CSIPProfile;
class CSIPProfileRegistry;
class CSIPConnection;


/**
 *  Implemets register vailability.
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 *  @since S60 v3.2
 */
class CMusAvaSipprofileAvailability : public CMusAvaAvailability
                                    
	{
public:

	/**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @param aModules is data container
     * @return Returns pointer to CMusAvaInterface object
     */
	static CMusAvaSipprofileAvailability* NewL(
        MMusAvaAvailabilityObserver& aObserver);


	static CMusAvaSipprofileAvailability* NewLC(
	    MMusAvaAvailabilityObserver& aObserver);

    /**
     * Destructor
     */

    ~CMusAvaSipprofileAvailability();

    protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aObserver Pointer to observer intercase
     * @param aName availability name
     * @param aModules is data container
     * @return
     */
    CMusAvaSipprofileAvailability(
        MMusAvaAvailabilityObserver& aObserver);

    void ConstructL();

// from base class CMusAvaAvailability

public:

   /**
    * Executes for the availability.
    */
    virtual void DoExecuteL();

   /**
    * Stop the execution.
    */
    virtual void Stop();
    
    /**
     * From CMusAvaAvailability.
     * Returns availability name.
     *
     * @return Name of *this* availability.
     */
    virtual MMusAvaObserver::TAvailabilityName Name();

private: // Helpers

   /**
	* Creates proposal sip address and send event.
	*
	*/
    TBool IsProfileExist();

    /**
	* Sets the iState to current state of sip profile existence.
	*
	*/
    void SetProfileState();

private: // Data

    /**
     * Pointer to CMusAvaSharedObject object
     */
     CMusAvaSharedObject* iSharedObj;    


    MUS_UNITTEST( UT_CMusAvaSipprofileAvailability )
    };


#endif // __MUSAVASIPPROFILEAVAILABILITY_H__

