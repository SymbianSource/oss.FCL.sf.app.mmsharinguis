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
*  Version     : %version: 9 % << Don't touch! Updated by Synergy at check-out.
*
*/



#ifndef C_MUSSIPPROFILEMODEL_H
#define C_MUSSIPPROFILEMODEL_H


#include "mmussipprofilehandler.h"
#include <e32base.h>
#include <sipprofileregistryobserver.h>
#include <sipobserver.h>
#include <sipconnectionobserver.h>

class CSIPManagedProfileRegistry;
class CMusSettingsModel;

/**
 *  CMusSIPProfileModel model class.
 *  Model class implementing SIP profile model.
 */
class CMusSIPProfileModel : public CBase,
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
     * default SIP profile.
     */
    TInt DefaultProfileIndex();

    /**
     * From MMusSIPProfileHandler.
     * Returns the id of the default SIP profile.
     * @return KErrNotFound, if one does not exist, otherwise id of the
     * default SIP profile.
     */
    TUint32 DefaultProfileId();
	
    /*
     * Disables MUS profile
     */
    void DisableProfileL();
	
    /*
     * Enables MUS profile
     */
    void EnableProfileL();
	
    /*
     * Checks, if MUS profile is enabled
     */
    TBool ProfileEnabledL();

    /**
     * From MMusSIPProfileHandler.
     * Returns SIP profile index on locally cached array based on id.
     * @param aId Profile id
     * @return Profile index or KErrNotFound if no matching profile is found.
     */
    TInt ProfileIndexByIdL( TUint32 aId );

    /**
     * From MMusSIPProfileHandler.
     * Returns profile id based on index.
     * @param aIndex Profile index
     * @return Profile id or KUnknownProfileId if no matching profile is found.
     */
    TUint32 ProfileIdByIndex( TUint aIndex );

    /**
     * From MMusSIPProfileHandler.
     * Returns a reference to the SIP profile array.
     * Note that previously returned value goes out of scope after a new
     * call to ProfileArrayL. So it is not advised to store return value
     * in long persistent variable, but rather to be used like stack
     * variables for safety.
     *
     * @return Reference to internally cached SIP profile array.
     */
    RPointerArray<CSIPProfile>& ProfileArrayL();

public: // From base class MSIPProfileRegistryObserver.

    /**
     * From MSIPProfileRegistryObserver.
     * SIP profile information event.
     * @param aProfileId Id of SIP profile in question.
     * @param aEvent Type of information event.
     */
    void ProfileRegistryEventOccurred(
	    TUint32 aSIPProfileId,
	    TEvent aEvent );

    /**
    * From MSIPProfileRegistryObserver.
    * An asynchronous error has occurred related to SIP profile.
    * @param aSIPProfileId Id of the failed profile.
    * @param aError An error code.
    */
    void ProfileRegistryErrorOccurred(
        TUint32 aSIPProfileId,
        TInt aError );


public:
    
    /**
     * Set CMusSettingsModel to handle ProfileRegistry Event.
     * @param aCMusSettingsModel the CMusSettingsModel to handle ProfileRegistry Event
     */		
    void SetCMusSettingsModel(CMusSettingsModel* aCMusSettingsModel);

protected:

    /**
     * Sorts internal array of SIP profiles by id.
     */
    void SortProfilesL();


private:

    CMusSIPProfileModel();

    void ConstructL();
    

private: // data

    /**
    * Instance of the SIP profile registry engine for managing SIP profiles.
    * Own.
    */
    CSIPManagedProfileRegistry* iEngine;

    /**
    * Locally cached array of SIP profiles.
    */
    RPointerArray<CSIPProfile> iProfiles;
    
    /**
    * CMusSettingsModel to handle ProfileRegistry 
    * Event
    */
    CMusSettingsModel* iCMusSettingsModel;
    
    

    };

#endif // C_MUSSIPPROFILEMODEL_H
