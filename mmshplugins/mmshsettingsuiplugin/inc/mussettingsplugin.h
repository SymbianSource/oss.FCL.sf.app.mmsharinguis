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
* Description:  MUSSettingsPlugin implementation.
*  Version     : %version: 21 % << Don't touch! Updated by Synergy at check-out.
*
*/



#ifndef C_MUSSETTINGSPLUGIN_H
#define C_MUSSETTINGSPLUGIN_H


#include <aknview.h>
#include <ConeResLoader.h>
#include <eikclb.h>
#include <gsplugininterface.h>
#include <gsbaseview.h>
#include <disknotifyhandler.h>


class CMusSettingsContainer;
class CAknNavigationDecorator;
class CAknViewAppUi;
class CMusSettingsModel;
class CMusSIPProfileModel;


/**  UID of *this* plugin. */
const TUid KGSVSSettingsPluginUID = { 0x1028238F };

/**  VS resource filename. */
_LIT( KVSSettingsResourceFileName, "mussettingsuirsc.rsc" );

/**  Name of icon of *this* plugin. Directory is KDC_BITMAP_DIR. */
_LIT( KGSVSSettingsPluginIconDirAndName, "mussettingsplugin.mif");


/**
 *  CMusSettingsPlugin view class.
 *  This class implements view of VS settings plugin.
 *
 *  @since S60 v3.2
 */
class CMusSettingsPlugin : public CGSBaseView, public MDiskNotifyHandlerCallback
    {
public:

    /**
     * Two-phased constructor.
     * @param aInitParams Params from framework. Not used.
     * @return Newly created instance of CMusSettingsPlugin class.
     */
    static CMusSettingsPlugin* NewL( TAny* aInitParams );

    /**
     * Destructor.
     */
    ~CMusSettingsPlugin();

    /**
     * Returns CGSBaseContainer sub-class.
     * Hides non-virtual base class function. (PC-Lint warning #1511).
     * @return CGSBaseContainer sub-class.
     */
    CMusSettingsContainer* Container();

public: // From CAknView.

    /**
    * From CAknView.
    * Returns views id.
    * @return id for this view.
    */
    virtual TUid Id() const;

    /**
     * From CAknView.
     * Command handling function. Handles user initiated commands.
     * @param aCommand ID of the command to respond to.
     */
    virtual void HandleCommandL( TInt aCommand );

public: // From CGSPluginInterface.

    /**
     * From CGSPluginInterface.
     * Method for getting caption of this plugin. This should be the
     * localized name of the settings view to be shown in parent view.
     *
     * @param aCaption Reference to descriptor to store caption.
     */
    virtual void GetCaptionL( TDes& aCaption ) const;

    /**
     * From CGSPluginInterface.
     * Creates a new icon of desired type. Overrided to provide custom
     * icons. Ownership of the created icon is transferred to the caller.
     *
     * Icon type UIDs (use these defined constants):
     * KGSIconTypeLbxItem  -   ListBox item icon.
     * KGSIconTypeTab      -   Tab icon.
     *
     * @param aIconType UID Icon type UID of the icon to be created.
     * @return Pointer of the icon. NOTE: Ownership of this icon is
     *         transferred to the caller.
     */
    virtual CGulIcon* CreateIconL( const TUid aIconType );

    /**
     * From CGSPluginInterface.
     * Method for reading the ID of the plugin provider category. See
     * TGSPluginProviderCategory. PluginProviderCategory is used for
     * sorting plugins.
     *
     * @return Plugin provider category ID defined in
     *         TGSPluginProviderCategory.
     */
    virtual TInt PluginProviderCategory() const;
	

public: // From MEikMenuObserver    
    
	void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
    
public:     // From observer interface
    void HandleNotifyDisk( TInt aError, const TDiskEvent& aEvent );

protected:

    CMusSettingsPlugin();

    void ConstructL();
    
    void OpenResourceFileL();
    void CloseResourceFile();

    /**
    * Hides non-virtual member from base class CGSBaseView.
    * (PC-Lint warning #1511).
    * Handles a change in client rectangle.
    */
    void HandleClientRectChange();

// from base class CAknView.

    /**
     * From CAknView.
     * Handles a view activation and passes the message of type
     * @c aCustomMessageId. This function is called by @c AknViewActivatedL().
     * @param aPrevViewId Specifies the view previously active.
     * @param aCustomMessageId Specifies the message type.
     * @param aCustomMessage The activation message.
     */
    virtual void DoActivateL( const TVwsViewId& aPrevViewId,
                              TUid aCustomMessageId,
                              const TDesC8& aCustomMessage );

    /**
     * From CAknView.
     * This function is called by AknViewDeactivated() to indicate
     * deactivation of *this* view.
     */
    virtual void DoDeactivate();

// from base class CGSBaseView.

    /**
    * From CGSBaseView.
    * Creates a container for *this* view. Called by DoActivateL().
    */
    virtual void NewContainerL();

private:

    /**
    * Shows Profile Setting dialog.
    */
    void ShowVSSettingsProfileSettingDialogL();

    /**
    * Shows SIP profile selection dialog.
    */
    void ShowVSSettingsSelectSipProfileDialogL();

    /**
    * Shows Recorded Video Saving Setting dialog.
    */
    void ShowVSSettingsRecordedVideoSavingSettingDialogL();

    /**
    * Shows note setting dialog. In standard branch this is the "Capability
    * auditory note" setting and in operator variant this is the "Alerts"
    * setting displayed to user.
    */
    void ShowVSSettingsNoteSettingDialogL();

    /**
    * Shows notification of no profiles defined.
    */
    void ShowNoProfilesNotificationL();

    /**
    * Switches between two possible values from one to another.
    * Leaves with KErrArgument if setting is not recognised, otherwise
    * tries to toggle specified setting value and on failure leaves on
    * systemwide errorcode.
    * @param aValue Id of setting to toggle.
    */
    void SwitchOnOffValueL( TInt aValue );
    
    void ShowGlobalInformationDialogL( TInt aResourceId );

// from base class CGSBaseView.

    /**
    * From CGSBaseView.
    * Handles listbox selection. Called by HandleListBoxEventL.
    */
    virtual void HandleListBoxSelectionL();

protected: // data

	HBufC* iCaption;
	bool iResourceFileOpen;

    /**
     * Instance of SIP profile handler to manage SIP profiles.
     * Own.
     */
    CMusSIPProfileModel* iHandler;


    /**
     * Pointer of model class of this application.
     * Own.
     */
    CMusSettingsModel* iModel;

    /**
     * Resource loader to load resources.
     */
    RConeResourceLoader iResources;

    /**
     * Disk notify handler
     */
    CDiskNotifyHandler* iDiskNotifyHandler;
    };

#endif // C_MUSSETTINGSPLUGIN_H
