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
* Description:  MUSSettingsPlugin model class.
*  Version     : %version: 11 % << Don't touch! Updated by Synergy at check-out.
*
*/



#ifndef  C_MUSSETTINGSMODEL_H
#define  C_MUSSETTINGSMODEL_H

#include <e32base.h>
#include <badesca.h>

class MMusSIPProfileHandler;
class CMusSIPProfileModel;
class CAknMemorySelectionDialogMultiDrive;


/**
 *  CMusSettingsModel is the model class of GS application.
 *  It provides functions to get and set setting values.
 */
class CMusSettingsModel : public CBase
    {
public:

    /**
     * Two-phased constructor.
     * @param aHandler Reference to SIP profile manipulator class.
     * return Returns pointer to a new instance of CMusSettings class.
     */
    static CMusSettingsModel* NewL( MMusSIPProfileHandler& aHandler );

    /**
     * Destructor.
     */
    ~CMusSettingsModel();

    /**
     * Returns recorded video saving mode.
     * @return 2: phone memory 
     *         4: memory card 
     */
    TInt VSSettingsRecordedVideoSavingL();
    
    /**
     * operator specific variant
     * Sets activation item 
     * @param aActive (enabled/disabled).
     */
    void SetActivationItem( TBool aActive );

    /**
     * operator specific variant
     * @return activation item 
     */
    TBool ActivationItem();
    
    /**
     * Returns the array containing the profile names.
     * NOTE! The ownership of the array is transferred to the caller.
     * @return Array consisting of the names of the SIP profiles.
     */
    CDesCArray* ListOfProfileNamesL();

    /**
     * Gets a SIP profile name by ID.
     * Ownership is transferred to caller.
     * @return SIP profile name.
     */
    HBufC* ProfileNameL( TInt aId );

    /**
     * Gets the index of the default SIP profile.
     * @return KErrNotFound, if one does not exist, otherwise index of the
     *         default SIP profile.
     */
    TInt DefaultProfileIndex();

    /**
     * Gets the id of the default SIP profile.
     * @return KErrNotFound, if one does not exist, otherwise id of the
     *         default SIP profile.
     */
    TUint32 DefaultProfileId();

    /**
     * Returns SIP profile index on locally cached array based on id.
     * @param aId Profile id
     * @return Profile index or KErrNotFound if no matching profile is found.
     */
    TInt ProfileIndexByIdL( TUint32 aId );

    /**
     * Returns profile id based on index.
     * @param aIndex Profile index
     * @return Profile id or KUnknownProfileId if no matching profile is found.
     */
    TUint32 ProfileIdByIndex( TUint aIndex );
    

    /**
     * Returns memory section dialog
     * @return Memory selction dialog reference
     */
    CAknMemorySelectionDialogMultiDrive* MemorySelectionDialogLC();

    /**
    * Initialize SIP profile enabler data.
    */
    void InitializeProfileEnablerL();
    
public: // constants

    /**
     * Default SIP profile.
     */
    static const TInt KVsSipProfileDefault;
    
    /**
     * SIP profile select.
     */
    static const TInt KVsSipProfileSelect;

    /**
     * Null selection SIP profile.
     */
    static const TInt KVsSipProfileSelectNone;

private:

    /**
     * C++ constructor.
     * @param aHandler Reference to SIP profile manipulator.
     */
    CMusSettingsModel( MMusSIPProfileHandler& aHandler );
    
    void ConstructL();

private: // data

    /**
     * SIP profile handler.
     */
    MMusSIPProfileHandler& iHandler;
    
    /**
     * Operator variant. If SIP profile disabled
     */
    TBool iProfileDisabled;

    };

#endif // C_MUSSETTINGSMODEL_H
