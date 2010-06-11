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
* Description:  Provide interface for the client requestin network availability.
*
*/


#ifndef __MUSAVACONTACTAVAILABILITY_H__
#define __MUSAVACONTACTAVAILABILITY_H__

#include "musavaavailability.h"
#include "musunittesting.h"
#include "mussettingskeys.h"

#include <badesca.h>

class MMusAvaAvailabilityObserver;
class MMusAvaObserver;
class CMusAvaSharedObject;
class CContactIdArray;
class CContactItem;
class CMusAvaSettingsImp;


/**
 *  Implemets register vailability.
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 */
class CMusAvaContactAvailability : public CMusAvaAvailability
	{

public:

	/**
     * Two-phased constructor.
     *
     * @param aObserver Pointer to observer interface.
     * @param aSettings Reference to settings object.
     * @return Returns pointer to CMusAvaInterface object.
     */
	static CMusAvaContactAvailability* NewL(
	                                MMusAvaAvailabilityObserver& aObserver,
                                    CMusAvaSettingsImp& aConcreteSettings );

	/**
     * Two-phased constructor.
     *
     * @param aObserver Pointer to observer interface.
     * @param aSettings Reference to settings object.
     * @return Returns pointer to CMusAvaInterface object.
     */
	static CMusAvaContactAvailability* NewLC(
	                                MMusAvaAvailabilityObserver& aObserver,
                                    CMusAvaSettingsImp& aConcreteSettings );

	/**
     * Destructor
     */
	~CMusAvaContactAvailability();


protected:

	CMusAvaContactAvailability( MMusAvaAvailabilityObserver& aObserver,
                                CMusAvaSettingsImp& aConcreteSettings );

	void ConstructL();


public: // from base class CMusAvaAvailability

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

private:

     /**
     *
     */
    TBool ActiveCallL( CDesCArrayFlat*& aContactsArray );


     /**
     *
     */
    TBool ResolveAddressesL( const TDesC& aContactId,
                             CDesCArrayFlat*& aContactsArray );

    
private:

     /**
     * resolved contact name
     */
     HBufC* iContactName;

	 /**
     * Pointer to CMusAvaSharedObject object
     */
	 CMusAvaSharedObject* iSharedObj;

	 CMusAvaSettingsImp& iSettings;
	 
	 /**
	  * Flag indicating if the contact name should begin 
	  * with first name or last name
	  */
	 TBool iBeginWithFirstName;
	 
	 MusSettingsKeys::TOperatorVariant iOperatorVariant;

	 MUS_UNITTEST( UT_CMusAvaContactAvailability )
    };


#endif // __MUSAVACONTACTAVAILABILITY_H__
