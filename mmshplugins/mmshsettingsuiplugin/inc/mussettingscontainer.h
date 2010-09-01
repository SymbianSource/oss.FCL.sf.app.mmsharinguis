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
* Description:  Container for MUSSettingsPlugin.
*
*/



#ifndef C_CMUSSETTINGSCONTAINER_H
#define C_CMUSSETTINGSCONTAINER_H


#include	"mussettings.h"
#include    <gsbasecontainer.h>
#include    <bldvariant.hrh>


class CGSListBoxItemTextArray;
class CEikTextListBox;
class CMusSettingsModel;


/**  Buffer size for general settings menu items */
const TInt KGSBufSize128 = 128;


/**
 *  CMusSettingsContainer container class.
 *  Container class for MUSSettingsPlugin.
 *
 *  @since S60 v3.2
 */
class CMusSettingsContainer : public CGSBaseContainer
    {
public:

    /**
     * C++ constructor, public since no NewL factory function implemented.
     *
     * @param aModel Reference to CMuSSettingsModel class.
     */
    CMusSettingsContainer( CMusSettingsModel& aModel );

    /**
     * Symbian OS second phase constructor. Public for owner to specify size
     * of client rectangle.
     *
     * @param aRect Rectangle of container area.
     */
    void ConstructL( const TRect& aRect );

    /**
     * Destructor.
     */
    ~CMusSettingsContainer();

    /**
     * Updates listbox's item's value.
     *
     * @param aFeatureId An item which is updated.
     */
    void UpdateListBoxL( TInt aFeatureId );

    /**
     * Retrieves the currently selected listbox feature id.
     * @return feature id.
     */
    TInt CurrentFeatureId() const;

    /**
     * Shows notification of newly selected SIP profile taken to use after
     * active call.
     */
    void ShowNewProfileActiveAfterCallL();

protected:

// from base class CGSBaseContainer.

    /**
     * From CGSBaseContainer.
     * Creates listbox. See baseclass for further description.
     *
     * @param aResLbxId Resource identifier for listbox to be created.
     */
    virtual void ConstructListBoxL( TInt aResLbxId );

private:

	/**
	 * Add an item to list of settings.
	 */
    void AddItemL( TInt aId, const TPtrC aText );

	/**
	 * Create settings list items.
	 */
    void CreateListBoxItemsL();

    /**
     * Create VS activation settings list item.
     */
    void MakeActivationItemL();

    /**
     * Create Operator VS activation settings list item.
     */
    void MakeOperatorActivationItemL();

    /**
     * Create SIP profile settings list item.
     */
    void MakeSIPProfileItemL();

    /**
     * Create autorecord settings list item.
     */
    void MakeAutoRecordItemL();

    /**
     * Create note settings list item.
     */
    void MakeNoteItemL();

    /**
     * Create save recorded video settings list item.
     */
    void MakeRecordedVideoSavingItemL();

    /**
     * Required for help.
     */
    void GetHelpContext( TCoeHelpContext& aContext ) const;

    /**
     * Hide some itesm if needed in operator variant mode
     */
    void HideItemsL( TInt aItemIndex );

private: // data

    /**
     * Reference to data model.
     */
    CMusSettingsModel& iModel;

    /**
     * List items in VS activation mode.
     * Own.
     */
    CDesCArrayFlat* iActivationItems;

    /**
     * List items in SIP profile mode.
     * Own.
     */
    CDesCArrayFlat* iProfileItems;

    /**
     * List items in autorecord mode.
     * Own.
     */
    CDesCArrayFlat* iAutoRecordItems;

    /**
     * List items in note.
     * Own.
     */
    CDesCArrayFlat* iNoteItems;

    /**
     * Operator variant current value.
     */
    MusSettingsKeys::TOperatorVariant iOperatorVariant;

    /**
     * Listbox items.
     */
    CGSListBoxItemTextArray* iListboxItemArray;

    };

#endif // C_CMUSSETTINGSCONTAINER_H
