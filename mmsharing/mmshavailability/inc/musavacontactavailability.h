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
#include "muscallmonitorobserver.h"
#include <badesca.h>
#include <PbkFields.hrh>

class MMusAvaAvailabilityObserver;
class MMusAvaObserver;
class CMusAvaSharedObject;
class CPbkContactEngine;
class CContactIdArray;
class CPbkContactItem;
class CContactItem;
class CMusAvaSettingsImp;
class CMusAvaCallEventMonitor;

/**
 *  Implemets register vailability.
 *
 *  Provide interface for the client to availability class.
 *
 *  @lib musavailabilityplugin.lib
 */
class CMusAvaContactAvailability : public CMusAvaAvailability,
								   public MMusCallMonitorObserver
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
    // from class MMusCallMonitorObserver
    /**
    * Call connected
    *
    * @since  S60 v3.2
    * @return
    */
    virtual void CallConnectedL( const TDesC& aTelNumber );
    
    /**
    * Call on hold
    *
    * @since  S60 v3.2
    * @return
    */
    virtual void CallHoldL( const TDesC& aTelNumber );

    /**
    * Call disconnected
    *
    * @since  S60 v3.2
    * @return
    */
    virtual void NoActiveCallL();

    /**
    * Call on hold
    *
    * @since  S60 v3.2
    * @return
    */
    virtual void ConferenceCallL();

private:

     /**
     *
     */
	TBool ActiveCallL( CDesCArrayFlat*& aContactsArray );

     /**
     *
     */
    TBool ContactL( CContactIdArray*& aContactIdArray,
                    CPbkContactEngine& aPbkContactEngine );

     /**
     *
     */
    void UpdateContactNameL( CContactItem& aContactItem );

     /**
     *
     */
    TInt ContactArrayL(  CContactIdArray*& aContactIdArray,
                         CPbkContactEngine& aPbkContactEngine );

     /**
     *
     */
    TInt ContactItem( CContactIdArray& aContactIdArray,
                       CPbkContactItem*& aPbkContactItem,
                       CPbkContactEngine& aPbkContactEngine );

    /**
    *
    */
    TBool ContactId( CPbkContactItem& aPbkItem,
                    TPbkFieldId aTPbkFieldId  );
    /**
    *
    */
    TPbkFieldId ContactIdSearch( CContactIdArray& aContactIdArray,
                                  CPbkContactEngine& aPbkContactEngine );

     /**
     *
     */
    TBool PhoneResolutionL( CPbkContactItem& aPbkItem,
                            CDesCArrayFlat*& aContactsArray  );


     /**
     *
     */
    TBool ResolveAddressesL( CPbkContactItem& aPbkItem,
                             CDesCArrayFlat*& aContactsArray,
                             TPbkFieldId aTPbkFieldId  );

     /**
     *
     */
    TBool ResolvePhoneContactL( CPbkContactItem& aPbkItem,
                                CDesCArrayFlat*& aContactsArray );

    /* -------------------------------------------------------------------------
    *  Checks that whether aDesItem present in aDesArray.
    *  Utility function used to avoid dublication.
    * -------------------------------------------------------------------------
    */
    TBool IsExistAlready(const MDesCArray& aDesArray,
                                          const TDesC& aDesItem);

private:
    /**
    * iPhoneStatus subscribe property resource. When the resource 
    * changed somewhere,availability will get notification.
    */
    CMusAvaCallEventMonitor* iPhoneStatus;

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