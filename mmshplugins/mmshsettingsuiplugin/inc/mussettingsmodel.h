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
*  Version     : %version: 8.1.2.1.3 % << Don't touch! Updated by Synergy at check-out.
*
*/



#ifndef  C_MUSSETTINGSMODEL_H
#define  C_MUSSETTINGSMODEL_H

#include 	"mussettings.h"

class MMusSIPProfileHandler;
class CMusSIPProfileModel;
class MultimediaSharingSettings;

class CAknMemorySelectionDialogMultiDrive;


/**
 *  CMusSettingsModel is the model class of GS application.
 *  It provides functions to get and set setting values.
 *
 *  @since S60 v3.2
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
     * Returns operator variant mode.
     * @return 0: Standard (MusSettingsKeys::EStandard)
     *         1: Operator Specific (MusSettingsKeys::EOperatorSpecific)
     */
    MusSettingsKeys::TOperatorVariant VSSettingsOperatorVariantL();

    /**
     * Returns VS activation mode.
     * @return 0: Always (MusSettingsKeys::EAlwaysActive)
     *         1: Automatic (MusSettingsKeys::EActiveInHomeNetworks)
     *         2: Off (MusSettingsKeys::ENever)
     */
    MusSettingsKeys::TActivation VSSettingsActivationL();

    /**
     * Returns SIP profile mode.
     * @return 0: Use default SIP profile.
     *         1: Use user selected SIP profile.
     */
    TInt VSSettingsProfileL();

    /**
     * Returns video autorecord mode.
     * @return 0: on  (MusSettingsKeys::EAutoRecordOn)
     *		   1: off (MusSettingsKeys::EAutoRecordOff)
     */
    MusSettingsKeys::TAutoRecord VSSettingsAutoRecordL();

    /**
     * Returns recorded video saving mode.
     * @return 2: phone memory 
     *         4: memory card 
     */
    TInt VSSettingsRecordedVideoSavingL();

    /**
     * Returns state of note (on/off). This setting is called "Capability
     * auditory note" in standard variant and "Alerts" in operator specific
     * variant.
     * @return 0: enabled (MusSettingsKeys::EAuditoryNotificationOn)
     *         1: disabled (MusSettingsKeys::EAuditoryNotificationOff)
     */
    MusSettingsKeys::TAuditoryNotification VSSettingsNoteL();

    /**
     * Sets packet data VS activation mode.
     * @param aActivation Activation mode.
     */
    void SetVSSettingsActivationL(
    		const MusSettingsKeys::TActivation aActivation );

    /**
     * Sets SIP profile mode.
     * @param aProfile Profile id.
     */
    void SetVSSettingsProfileL( const TInt aProfile );

    /**
     * Sets video autorecord mode.
     * @param aAutoRecord save mode of video.
     */
    void SetVSSettingsAutoRecordL(
    		const MusSettingsKeys::TAutoRecord aAutoRecord );

    /**
    * Sets location of video saving.
    * @param aVideoLocation location of video
    */
    void SetVSSettingsRecordedVideoSavingL( const TInt aVideoLocation );

    /**
     * Sets mode of note ("Capability auditory note" in standard variant and
     * "Alerts" in operator specific variant).
     * @param aValue New state for note (enabled/disabled).
     */
    void SetVSSettingsNoteL(
    		const MusSettingsKeys::TAuditoryNotification aValue );
    
    
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
    TBool ActivationItem( );
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
    
    /**
    * Initialize SIP profile enabler data.
    */
    void InitializeProfileEnablerL();

    void ConstructL();

private: // data

    /**
     * SIP profile handler.
     */
    MMusSIPProfileHandler& iHandler;

    /**
     * Central repository keys of video sharing.
     * Own.
     */
    MultimediaSharingSettings* iMSSettingsKeys;
    
    /**
     * Operator variant. If SIP profile disabled
     */
    TBool iProfileDisabled;

    };

#endif // C_MUSSETTINGSMODEL_H
