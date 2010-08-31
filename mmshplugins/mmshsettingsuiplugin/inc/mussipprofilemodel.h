/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class implementing SIP profile model.
*
*/



#ifndef C_MUSSIPPROFILEMODEL_H
#define C_MUSSIPPROFILEMODEL_H


#include "mmussipprofilehandler.h"
#include <e32base.h>
#include <sipmanagedprofile.h>
#include <gsplugininterface.h>
#include <sipprofileregistryobserver.h>


class CSIPManagedProfileRegistry;


/**
 *  CMusSIPProfileModel model class.
 *  Model class implementing SIP profile model.
 *
 *  @since S60 v3.2
 */
class CMusSIPProfileModel
    :public CBase,
     public MSIPProfileRegistryObserver,
	 public MMusSIPProfileHandler
	 {
public:

    static CMusSIPProfileModel* NewL();

    /**
     * Destructor.
     */
    virtual ~CMusSIPProfileModel();

public: // From base class MMusSIPProfileHandler.

    /**
     * From MMusSIPProfileHandler.
     * Returns the index of the default SIP profile.
     * @return KErrNotFound, if one does not exist, otherwise index of the
     *         default SIP profile.
     */
    virtual TInt DefaultProfileIndex();

    /**
     * From MMusSIPProfileHandler.
     * Returns the id of the default SIP profile.
     * @return KErrNotFound, if one does not exist, otherwise id of the
     *         default SIP profile.
     */
	virtual TUint32 DefaultProfileId();

    /**
     * From MMusSIPProfileHandler.
     * Returns SIP profile index on locally cached array based on id.
     * @param aId Profile id
     * @return Profile index or KErrNotFound if no matching profile is found.
     */
    virtual TInt ProfileIndexByIdL( TUint32 aId );

    /**
     * From MMusSIPProfileHandler.
     * Returns profile id based on index.
     * @param aIndex Profile index
     * @return Profile id or KUnknownProfileId if no matching profile is found.
     */
    virtual TUint32 ProfileIdByIndex( TUint aIndex );

    /**
     * From MMusSIPProfileHandler.
     * Returns pointer to the SIP profile array, does not change ownership.
     * Note that previously returned value goes out of scope after a new
     * call to ProfileArrayL. So it is not advised to store return value
     * in long persistent variable, but rather to be used like stack
     * variables for safety.
     *
     * @return Pointer to internally cached SIP profile array.
     */
    virtual CArrayPtr<CSIPManagedProfile>* ProfileArrayL();

public: // From base class MSIPProfileRegistryObserver.

    /**
     * From MSIPProfileRegistryObserver.
     * SIP profile information event.
     * @param aProfileId Id of SIP profile in question.
     * @param aEvent Type of information event.
	 */
	virtual void ProfileRegistryEventOccurred(
	    TUint32 aSIPProfileId,
	    TEvent aEvent );

	/**
     * From MSIPProfileRegistryObserver.
	 * An asynchronous error has occurred related to SIP profile.
     * @param aSIPProfileId Id of the failed profile.
	 * @param aError An error code.
	 */
	virtual void ProfileRegistryErrorOccurred(
	    TUint32 aSIPProfileId,
	    TInt aError );

protected:

    /**
     * Reads the profile array from SIP Profile Client.
     */
    void ReadArrayFromEngineL();

    /**
     * Sorts internal array of SIP profiles by id.
     */
    void SortProfilesL();

    /**
     * Deletes internally cached profiles.
     */
    void DeleteProfiles();

    /**
     * Reads profile list from the engine; if reading fails,
     * keeps the old list safe.
     */
    void ReadProfileListFromEngineSafeL();

private:

    CMusSIPProfileModel();

    void ConstructL();

    /**
     * For deleting RPointerArray in case of leave.
     */
    static void ResetAndDestroy( TAny* aPointerArray );

private: // data

    /**
     * Instance of the SIP profile registry engine for managing SIP profiles.
     * Own.
     */
    CSIPManagedProfileRegistry* iEngine;

    /**
     * Locally cached array of SIP profiles.
     * Own.
     */
    CArrayPtrFlat<CSIPManagedProfile>* iProfiles;

    };

#endif // C_MUSSIPPROFILEMODEL_H
